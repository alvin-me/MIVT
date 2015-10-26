#pragma once

#include "tgt_gl.h"
#include "renderbase.h"

namespace tgt {
  class Volume;
  class TextureUnit;
  class TransFunc1D;
}

namespace mivt {

  class PreIntegration;

  /**
  * All processors that access volume data in shaders
  * should be derived from this base class.
  *
  * It extends the generateHeader and setGlobalShaderParameters methods
  * inherited from RenderProcessor and provides a function for passing
  * volumes to a shader. Additionally, it declares light source properties:
  * add these in your derived class' constructor to actually use them.
  *
  * @see VolumeRaycaster
  */
  class VolumeRaycaster : public RenderBase
  {
  public:
    VolumeRaycaster();
    virtual ~VolumeRaycaster();

    virtual void Initialize();
    virtual void Deinitialize();

    std::string GetClassificationMode();

  protected:
    /**
    * This struct contains information about a volume. It is exclusively used
    * as parameter type for the bindVolumes() function.
    */
    struct VolumeStruct {
      VolumeStruct();
      VolumeStruct(tgt::Volume* volume, const tgt::TextureUnit* texUnit,
        const std::string& volumeIdentifier,
        const std::string& volumeStructIdentifier,
        GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f),
        GLint filterMode = GL_LINEAR);

      tgt::Volume* volume_;                        ///< the volume whose texture is to be bound
      const tgt::TextureUnit* texUnit_;                    ///< the texture unit to bind to
      GLint wrapMode_;                                ///< the texture wrapping mode to set
      glm::vec4 borderColor_;                         ///< the texture's border color
      GLint filterMode_;                              ///< the filtering mode to use for this texture
      std::string volumeIdentifier_;                  ///< the identifier of the volume
      std::string volumeStructIdentifier_;            ///< the identifier of the volume parameter struct
      ///  for this volume in the shader
    };

    /**
    * Defines ray-casting macros to be used in the raycasting shader.
    */
    virtual std::string generateHeader();

    /**
    * Sets the parameters of the light source.
    * The camera parameter is passed to the super class function.
    *
    * @see RenderProcessor::setGlobalShaderParameters
    */
    virtual void setGlobalShaderParameters(tgt::Shader* shader, const tgt::Camera* camera, glm::ivec2 screenDim);

    /**
    * This function binds the volume textures used by the volume renderer and passes the
    * corresponding samplers (texture units) to the shader.
    * \note This function also sets additional meta data like the volume's dimensions and spacing.
    *       For this reason, every volume renderer that uses a volume texture must call this function
    *       for all volumes accessed during a rendering pass. Do not bypass this function by binding
    *       volume textures directly unless you know exactly what you are doing!
    * @param shader the shader that performs the volume rendering
    * @param volume volume to bind
    * @param camera the scene's camera position. Is transformed to volume object coordinates for lighting calculations.
    * @param lightPosition the scene's light position. Is transformed to volume object coordinates for lighting calculations.
    *
    * @return true, if all volumes have been successfully bound
    */
    virtual bool bindVolume(tgt::Shader* shader, const VolumeStruct &volume,
      const tgt::Camera* camera = 0,
      const glm::vec4& lightPosition = glm::vec4(0.f));

    /**
    * Bind look-up table texture.
    *
    * @param mode the classification mode
    * @param tf the transfer function.
    */
    virtual void bindTransfuncTexture(const std::string mode, tgt::TransFunc1D* tf, float samplingStepSize);

    /// Calculate sampling step size for a given volume using the current sampling rate
    float CalculateSamplingStepSize(tgt::Volume* vh);

  private:

    static bool bindVolumeTexture(tgt::Volume* vh, const tgt::TextureUnit* texUnit,
      GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));

    static void setUniform(tgt::Shader* shader, const std::string& volumeUniform, const std::string& structUniform,
      tgt::Volume* vh, const tgt::TextureUnit* texUnit, const tgt::Camera* camera = 0,
      const glm::vec4& lightPosition = glm::vec4(0.f));

    /**
    * Get classification type string used in shader.
    *
    * @param mode the classification mode
    * @param defineName the define name used in shader.
    * @return string as define.
    */
    static std::string getShaderDefineSamplerType(const std::string mode, 
      const std::string& defineName = "TF_SAMPLER_TYPE");

    /**
    * Get classification texture lookup string used in shader.
    *
    * @param mode the classification mode
    * @param defineName the define name used in shader.
    * @return string as define.
    */
    static std::string getShaderDefineFunction(const std::string mode, 
      const std::string& defineName = "RC_APPLY_CLASSIFICATION");


  protected:
    /// The position of the light source used for lighting calculations in world coordinates
    glm::vec4 lightPosition_;

    /// The light source's ambient color according to the Phong lighting model
    glm::vec4 lightAmbient_;
    /// The light source's diffuse color according to the Phong lighting model
    glm::vec4 lightDiffuse_;
    /// The light source's specular color according to the Phong lighting model
    glm::vec4 lightSpecular_;

    /// The light source's attenuation factors (x = constant, y = linear, z = quadratic)
    glm::vec3 lightAttenuation_;
    /// Apply light attenuation?
    bool applyLightAttenuation_;

    /// The ambient material color according to the Phong lighting model
    glm::vec4 materialAmbient_;
    /// The diffuse material color according to the Phong lighting model
    glm::vec4 materialDiffuse_;
    /// The specular material color according to the Phong lighting model
    glm::vec4 materialSpecular_;

    /// The material's specular exponent according to the Phong lighting model
    float materialShininess_;

    ///< Sampling rate of the raycasting, specified relative to the size of one voxel
    float samplingRate_;          

    std::string gradientMode_;                ///< What type of calculation should be used for on-the-fly gradients
    std::string classificationMode_;          ///< What type of transfer function should be used for classification
    std::string shadeMode_;                   ///< What shading method should be applied
    std::string compositingMode_;             ///< What compositing mode should be applied

    int interactionCoarseness_;               ///< RenderPorts are resized to size_/interactionCoarseness_ in interactionmode
    //float interactionQuality_;
    //bool interactionMode_;

    PreIntegration *preintegration_;     ///< compute and cache pre-integration table

    static const std::string loggerCat_;
  };
}


