#include "volumeraycaster.h"
#include "tgt_string.h"
#include "shadermanager.h"
#include "camera.h"
#include "volume.h"
#include "volumegl.h"
#include "textureunit.h"
#include "transfunc1d.h"
#include "preintegration.h"

namespace mivt {
  const std::string VolumeRaycaster::loggerCat_("VolumeRaycaster");

  VolumeRaycaster::VolumeRaycaster()
    : RenderBase()
    , lightPosition_(glm::vec4(2.3f, 1.5f, 1.5f, 1.f))
    , lightAmbient_(glm::vec4(0.4f, 0.4f, 0.4f, 1.f))
    , lightDiffuse_(glm::vec4(0.8f, 0.8f, 0.8f, 1.f))
    , lightSpecular_(glm::vec4(0.6f, 0.6f, 0.6f, 1.f))
    , lightAttenuation_(glm::vec3(1.f, 0.f, 0.f))
    , applyLightAttenuation_(false)
    , materialShininess_(60.0f)
    , samplingRate_(2.f)
    , gradientMode_("central-differences")
    , classificationMode_("pre-integrated-gpu") // transfer-function, pre-integrated
    , shadeMode_("phong")
    , compositingMode_("dvr")
    , preintegration_(0)
    , interactionCoarseness_(3) // 1~8
    //, interactionQuality_(1.f)
    //, interactionMode_(false)
  {
  }


  VolumeRaycaster::~VolumeRaycaster()
  {
  }

  void VolumeRaycaster::Initialize() {
    preintegration_ = new PreIntegration(256, true);
  }

  void VolumeRaycaster::Deinitialize() {
    DELPTR(preintegration_);
  }

  std::string VolumeRaycaster::GetClassificationMode() {
    return classificationMode_;
  }

  std::string VolumeRaycaster::generateHeader() 
  {
    std::string headerSource = "#version 330\n";

    // configure gradient calculation
    headerSource += "#define CALC_GRADIENT(volume, volumeStruct, samplePos) ";
    if (gradientMode_ == "central-differences")
      headerSource += "calcGradient(volume, volumeStruct, samplePos);\n";

    if (applyLightAttenuation_)
      headerSource += "#define PHONG_APPLY_ATTENUATION\n";

    // configure classification
    headerSource += getShaderDefineSamplerType(classificationMode_, "TF_SAMPLER_TYPE");
    headerSource += getShaderDefineFunction(classificationMode_, "RC_APPLY_CLASSIFICATION");

    // configure shading mode
    headerSource += "#define APPLY_SHADING(n, pos, lPos, cPos, ka, kd, ks) ";
    if (shadeMode_ == "phong")
      headerSource += "phongShading(n, pos, lPos, cPos, ka, kd, ks);\n";

    return headerSource;
  }

  void VolumeRaycaster::setGlobalShaderParameters(tgt::Shader* shader, const tgt::Camera* camera, glm::ivec2 screenDim) 
  {
    RenderBase::setGlobalShaderParameters(shader, camera, screenDim);

    shader->setIgnoreUniformLocationError(true);

    // light source position in world coordinates
    shader->setUniform("lightSource_.position_", lightPosition_.xyz());
    shader->setUniform("lightSource_.attenuation_", lightAttenuation_);
    shader->setUniform("lightSource_.ambientColor_", lightAmbient_.xyz());
    shader->setUniform("lightSource_.diffuseColor_", lightDiffuse_.xyz());
    shader->setUniform("lightSource_.specularColor_", lightSpecular_.xyz());

    shader->setUniform("shininess_", materialShininess_);

    // provide values needed for correct depth value calculation
    if (camera) {
      float n = camera->getNearDist();
      float f = camera->getFarDist();
      shader->setUniform("const_to_z_e_1", 0.5f + 0.5f*((f + n) / (f - n)));
      shader->setUniform("const_to_z_e_2", ((f - n) / (f*n)));
      shader->setUniform("const_to_z_w_1", ((f*n) / (f - n)));
      shader->setUniform("const_to_z_w_2", 0.5f*((f + n) / (f - n)) + 0.5f);
    }

    shader->setIgnoreUniformLocationError(false);
  }

  bool VolumeRaycaster::bindVolume(tgt::Shader* shader,
    const VolumeStruct& volume,
    const tgt::Camera* camera,
    const glm::vec4& lightPosition) 
  {
    shader->setIgnoreUniformLocationError(true);

    bool success = true;

    const VolumeStruct& volumeStruct = volume;
    tgt::VolumeGL* volumeGL = volumeStruct.volume_->getRepresentation<tgt::VolumeGL>();
    if (!volumeGL || !volumeGL->getTexture()) {
      LERROR("OpenGL volume not available");
      success = false;
    }

    const tgt::TextureUnit* texUnit = volumeStruct.texUnit_;
    if (!texUnit) {
      LERROR("No texture unit while binding volumes");
      success = false;
    }

    success &= bindVolumeTexture(volumeStruct.volume_, texUnit, volumeStruct.filterMode_, volumeStruct.wrapMode_, volumeStruct.borderColor_);

    // set volume meta-data
    setUniform(shader, volumeStruct.volumeIdentifier_, volumeStruct.volumeStructIdentifier_, volumeStruct.volume_, texUnit, camera, lightPosition);

    shader->setUniform("samplingStepSize_", CalculateSamplingStepSize(volumeStruct.volume_));

    LGL_ERROR;

    shader->setIgnoreUniformLocationError(false);
    LGL_ERROR;

    return success;
  }

  bool VolumeRaycaster::bindVolumeTexture(tgt::Volume* vh, const tgt::TextureUnit* texUnit, 
    GLint filterMode, GLint wrapMode, glm::vec4 borderColor) {
    tgt::VolumeGL* volumeGL = vh->getRepresentation<tgt::VolumeGL>();
    if (!volumeGL || !volumeGL->getTexture()) {
      LWARNING("No volume texture while binding volumes");
      return false;
    }
    const tgt::VolumeTexture* volumeTex = volumeGL->getTexture();

    texUnit->activate();

    volumeTex->bind();

    // Returns the residence status of the target texture. If the value returned in params is
    // GL_TRUE, the texture is resident in texture memory
    GLint resident;
    glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_RESIDENT, &resident);
    if (resident != GL_TRUE){
      LWARNING("volume texture not resident");
    }
    LGL_ERROR;

    // texture filtering
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filterMode);
    LGL_ERROR;

    // texture wrapping
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapMode);
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
    LGL_ERROR;

    texUnit->setZeroUnit();

    return true;
  }

  void VolumeRaycaster::setUniform(tgt::Shader* shader, const std::string& volumeUniform, const std::string& structUniform,
    tgt::Volume* vh, const tgt::TextureUnit* texUnit, const tgt::Camera* camera, const glm::vec4& lightPosition) 
  {
    if (texUnit)
      shader->setUniform(volumeUniform, texUnit->getUnitNumber());

    // volume size, i.e. dimensions of the proxy geometry in world coordinates
    shader->setUniform(structUniform + ".datasetDimensions_", glm::vec3(vh->getDimensions()));
    shader->setUniform(structUniform + ".datasetDimensionsRCP_", glm::vec3(1.f) / glm::vec3(vh->getDimensions()));

    // volume spacing, i.e. voxel size
    shader->setUniform(structUniform + ".datasetSpacing_", vh->getSpacing());
    shader->setUniform(structUniform + ".datasetSpacingRCP_", glm::vec3(1.f) / vh->getSpacing());

    // volume's size in its physical coordinates
    shader->setUniform(structUniform + ".volumeCubeSize_", vh->getCubeSize());
    shader->setUniform(structUniform + ".volumeCubeSizeRCP_", glm::vec3(1.f) / vh->getCubeSize());

    shader->setUniform(structUniform + ".volumeOffset_", vh->getOffset());

    // volume's transformation matrix
    shader->setUniform(structUniform + ".physicalToWorldMatrix_", vh->getPhysicalToWorldMatrix());

    glm::mat4 invTm = vh->getWorldToPhysicalMatrix();
    shader->setUniform(structUniform + ".worldToPhysicalMatrix_", invTm);

    shader->setUniform(structUniform + ".worldToTextureMatrix_", vh->getWorldToTextureMatrix());
    shader->setUniform(structUniform + ".textureToWorldMatrix_", vh->getTextureToWorldMatrix());

    // camera position in volume object coords
    if (camera)
      shader->setUniform(structUniform + ".cameraPositionPhysical_", glm::mapPoint(invTm, camera->getPosition()));

    // light position in volume object coords
    shader->setUniform(structUniform + ".lightPositionPhysical_", (invTm*lightPosition).xyz());

    LGL_ERROR;

    // bit depth of the volume
    shader->setUniform(structUniform + ".bitDepth_", (GLint)(vh->getBytesPerVoxel() * 8));

    // construct shader value mapping by combining volume value mapping and pixel transfer mapping
    tgt::ValueMapping rescaleMapping = vh->getRescaleMapping();
    tgt::ValueMapping transferMapping;
    if (vh->getRepresentation<tgt::VolumeGL>())
      transferMapping = vh->getRepresentation<tgt::VolumeGL>()->getPixelTransferMapping();
    else
      LWARNING("setUniform(): no VolumeGL");
    tgt::ValueMapping shaderMapping = tgt::ValueMapping::combine(transferMapping.getInverseMapping(), rescaleMapping);
    shader->setUniform(structUniform + ".vmScale_", shaderMapping.getScale());
    shader->setUniform(structUniform + ".vmOffset_", shaderMapping.getOffset());
  }

  float VolumeRaycaster::CalculateSamplingStepSize(tgt::Volume* vh) {
    glm::ivec3 dim = vh->getDimensions();

    // use dimension with the highest resolution for calculating the sampling step size
    float samplingStepSize = 1.f / (glm::hmax(dim) * samplingRate_);

    //if (interactionMode_)
    //  samplingStepSize /= interactionQuality_;

    return samplingStepSize;
  }

  std::string VolumeRaycaster::getShaderDefineSamplerType(const std::string mode, const std::string& defineName) {
    if (tgt::startsWith(mode, "pre-integrated"))
      return "#define " + defineName + " sampler2D\n";
    else if (mode == "transfer-function")
      return "#define " + defineName + " sampler1D\n";
    else
      return "";
  }

  std::string VolumeRaycaster::getShaderDefineFunction(const std::string mode, const std::string& defineName) {
    std::string def = "#define " + defineName + "(transFunc, transFuncTex, intensity, lastIntensity) ";

    if (mode == "none")
      def += "vec4(intensity);\n";
    else if (mode == "transfer-function")
      def += "applyTF(transFunc, transFuncTex, intensity);\n";
    else if (tgt::startsWith(mode, "pre-integrated"))
      def += "applyTFpi(transFunc, transFuncTex, intensity, lastIntensity);\n";

    return def;
  }

  void VolumeRaycaster::bindTransfuncTexture(const std::string mode, tgt::TransFunc1D* tf, float samplingStepSize) {
    if (tf) {
      if (tgt::startsWith(mode, "pre-integrated")) {
        if (preintegration_->computeOnGPU() != tgt::contains(mode, "gpu")) {
          DELPTR(preintegration_);
          preintegration_ = new PreIntegration(256, tgt::contains(mode, "gpu"), true);
        }
        preintegration_->getTexture(tf, samplingStepSize)->bind();
      }
      else
        tf->bind();
    }
  }

  void VolumeRaycaster::SetLightAmbient(const glm::vec4& v) {
    lightAmbient_ = v;
  }

  glm::vec4 VolumeRaycaster::GetLightAmbient() {
    return lightAmbient_;
  }

  void VolumeRaycaster::SetLightDiffuse(const glm::vec4& v) {
    lightDiffuse_ = v;
  }

  glm::vec4 VolumeRaycaster::GetLightDiffuse() {
    return lightDiffuse_;
  }

  void VolumeRaycaster::SetLightSpecular(const glm::vec4& v) {
    lightSpecular_ = v;
  }

  glm::vec4 VolumeRaycaster::GetLightSpecular() {
    return lightSpecular_;
  }

  void VolumeRaycaster::SetMaterialShininess(float v) {
    materialShininess_ = v;
  }

  float VolumeRaycaster::GetMaterialShininess() {
    return materialShininess_;
  }

  //------------------------------------------------------------------------------

  VolumeRaycaster::VolumeStruct::VolumeStruct()
    : volume_(0)
  {}

  VolumeRaycaster::VolumeStruct::VolumeStruct(tgt::Volume* volume, const tgt::TextureUnit* texUnit,
    const std::string& volumeIdentifier,
    const std::string& volumeStructIdentifier,
    GLint wrapMode,
    glm::vec4 borderColor, GLint filterMode)
    : volume_(volume)
    , texUnit_(texUnit)
    , wrapMode_(wrapMode)
    , borderColor_(borderColor)
    , filterMode_(filterMode)
    , volumeIdentifier_(volumeIdentifier)
    , volumeStructIdentifier_(volumeStructIdentifier)
  {}
}

