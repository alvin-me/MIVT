#include "rendervolume.h"
#include "rendertarget.h"
#include "shadermanager.h"
#include "camera.h"
#include "trackball.h"
#include "textureunit.h"
#include "rendercolorcube.h"
#include "trianglemeshgeometry.h"
#include "volume.h"
#include "transfunc1d.h"

namespace mivt {

  RenderVolume::RenderVolume()
    : VolumeRaycaster()
    , output_(0)
    , shader_(0)
    , camera_(0)
    , proxyGeometry_(0)
    , renderColorCube_(0)
    , trackball_(0)
    , volume_(0)
    , transfunc_(0)
  {
  }

  RenderVolume::~RenderVolume()
  {
  }

  void RenderVolume::Initialize()
  {
    VolumeRaycaster::Initialize();

    shader_ = ShdrMgr.loadSeparate("passthrough.vert", "rc_basic.frag", generateHeader(), false);

    output_ = new tgt::RenderTarget();
    output_->initialize(GL_RGB);

    camera_ = new tgt::Camera(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    trackball_ = new tgt::Trackball(camera_);

    renderColorCube_ = new RenderColorCube();
    renderColorCube_->Initialize();

    // temperory create a geometry.
    const glm::vec3 texLlf(0, 0, 0);
    const glm::vec3 texUrb(1, 1, 1);
    proxyGeometry_ = tgt::TriangleMeshGeometryVec4Vec3::createCube(texLlf, texUrb, texLlf, texUrb, 1.0f);

    glm::vec3 cubeSize = texUrb - texLlf;
    float scale = 2.0f / glm::hmax(cubeSize);
    glm::mat4 matrix = glm::translate(-0.5f * scale * cubeSize) *
      glm::scale(glm::vec3(scale));
    proxyGeometry_->transform(matrix);
  }

  void RenderVolume::Deinitialize()
  {
    VolumeRaycaster::Deinitialize();

    ShdrMgr.dispose(shader_);
    shader_ = 0;

    output_->deinitialize();
    DELPTR(output_);

    DELPTR(camera_);
    DELPTR(trackball_);

    renderColorCube_->Deinitialize();
    DELPTR(renderColorCube_);

  }

  void RenderVolume::GetPixels(unsigned char* buffer, int length)
  {
    if (!buffer)
      return;

    Process();
    output_->readColorBuffer(buffer, length);
  }

  void RenderVolume::Resize(const glm::ivec2& newSize)
  {
    output_->resize(newSize);
    renderColorCube_->Resize(newSize);
  }

  void RenderVolume::Process()
  {
    // create front & back color cube texture.
    renderColorCube_->Process(proxyGeometry_, camera_);

    output_->activateTarget();
    output_->clearTarget();

    if (volume_) {
      // activate shader and set common uniforms
      shader_->activate();
      setGlobalShaderParameters(shader_, camera_, output_->getSize());
      LGL_ERROR;

      // bind entry and exit params and pass texture units to the shader
      tgt::TextureUnit entryUnit, entryDepthUnit, exitUnit, exitDepthUnit;
      renderColorCube_->GetFrontFace()->bindTextures(entryUnit, entryDepthUnit, GL_NEAREST);
      shader_->setUniform("entryPoints_", entryUnit.getUnitNumber());
      //shader_->setUniform("entryPointsDepth_", entryDepthUnit.getUnitNumber());
      renderColorCube_->GetFrontFace()->setTextureParameters(shader_, "entryParameters_");

      renderColorCube_->GetBackFace()->bindTextures(exitUnit, exitDepthUnit, GL_NEAREST);
      shader_->setUniform("exitPoints_", exitUnit.getUnitNumber());
      //shader_->setUniform("exitPointsDepth_", exitDepthUnit.getUnitNumber());
      renderColorCube_->GetBackFace()->setTextureParameters(shader_, "exitParameters_");
      LGL_ERROR;

      // bind volume texture and pass it to the shader
      tgt::TextureUnit volUnit;
      VolumeStruct volumeTexture(volume_, &volUnit, "volume_", "volumeStruct_",
        GL_CLAMP_TO_EDGE, glm::vec4(0.f), GL_LINEAR);
      bindVolume(shader_, volumeTexture, camera_, lightPosition_);
      LGL_ERROR;

      // bind transfer function and pass it to the shader
      tgt::TextureUnit transferUnit;
      if (transfunc_) {
        transferUnit.activate();
        bindTransfuncTexture(classificationMode_, transfunc_, CalculateSamplingStepSize(volume_));
        transfunc_->setUniform(shader_, "transFuncStruct_", "transFuncTex_", transferUnit.getUnitNumber());
      }

      // render screen aligned quad
      renderQuad();

      glFinish();

      // clean up
      shader_->deactivate();
    }

    output_->deactivateTarget();

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  void RenderVolume::Rotate(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->rotate(newMouse, lastMouse);
  }

  void RenderVolume::Zoom(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    const glm::vec2 zoomInDirection(0.f, 1.f);
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->zoom(newMouse, lastMouse, zoomInDirection);
  }

  void RenderVolume::Pan(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->move(newMouse, lastMouse);
  }

  glm::vec2 RenderVolume::scaleMouse(const glm::ivec2& coords, const glm::ivec2& viewport) const {
    return glm::vec2(static_cast<float>(coords.x*2.f) / static_cast<float>(viewport.x) - 1.f,
      static_cast<float>(coords.y*2.f) / static_cast<float>(viewport.y) - 1.0f);
  }

  void RenderVolume::SetVolume(tgt::Volume *volume)
  {
    volume_ = volume;

    // reset camera
    camera_->reset(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    // generate geometry
    const glm::vec3 texLlf(0, 0, 0);
    const glm::vec3 texUrb(1, 1, 1);
    DELPTR(proxyGeometry_);
    proxyGeometry_ = tgt::TriangleMeshGeometryVec4Vec3::createCube(texLlf, texUrb, texLlf, texUrb, 1.0f);
    proxyGeometry_->transform(volume->getTextureToWorldMatrix());
  }

  void RenderVolume::SetTransfunc(tgt::TransFunc1D *transfunc)
  {
    transfunc_ = transfunc;
  }
}

