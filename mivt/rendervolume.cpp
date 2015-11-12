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
#include "renderbackground.h"
#include "rendertoscreen.h"

namespace mivt {

  RenderVolume::RenderVolume()
    : VolumeRaycaster()
    , privatetarget_(0)
    , smallprivatetarget_(0)
    , output_(0)
    , shader_(0)
    , camera_(0)
    , proxyGeometry_(0)
    , renderColorCube_(0)
    , trackball_(0)
    , volume_(0)
    , transfunc_(0)
    , renderBackground_(0)
    , renderToScreen_(0)
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
    output_->initialize();

    privatetarget_ = new tgt::RenderTarget();
    privatetarget_->initialize();

    smallprivatetarget_ = new tgt::RenderTarget();
    smallprivatetarget_->initialize();

    camera_ = new tgt::Camera(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    trackball_ = new tgt::Trackball(camera_);
    trackball_->setSize(0.7f); // sets trackball sensitivity

    renderColorCube_ = new RenderColorCube();
    renderColorCube_->Initialize();

    renderBackground_ = new RenderBackground();
    renderBackground_->Initialize();

    renderToScreen_ = new RenderToScreen();
    renderToScreen_->Initialize();
  }

  void RenderVolume::Deinitialize()
  {
    VolumeRaycaster::Deinitialize();

    ShdrMgr.dispose(shader_);
    shader_ = 0;

    output_->deinitialize();
    DELPTR(output_);

    privatetarget_->deinitialize();
    DELPTR(privatetarget_);

    smallprivatetarget_->deinitialize();
    DELPTR(smallprivatetarget_);

    DELPTR(camera_);
    DELPTR(trackball_);

    renderColorCube_->Deinitialize();
    DELPTR(renderColorCube_);

    renderBackground_->Deinitialize();
    DELPTR(renderBackground_);

    renderToScreen_->Deinitialize();
    DELPTR(renderToScreen_);

  }

  void RenderVolume::GetPixels(unsigned char* buffer, int length, bool downsampling)
  {
    if (!buffer)
      return;

    Process(downsampling);
    output_->readColorBuffer<unsigned char>(buffer, length);
  }

  void RenderVolume::Resize(const glm::ivec2& newSize)
  {
    privatetarget_->resize(newSize);
    smallprivatetarget_->resize(newSize / interactionCoarseness_);
    output_->resize(newSize);
    renderColorCube_->Resize(newSize);
    renderBackground_->Resize(newSize);
    renderToScreen_->Resize(newSize);
  }

  void RenderVolume::Process(bool downsampling)
  {
    // create front & back color cube texture.
    if (proxyGeometry_)
      renderColorCube_->Process(proxyGeometry_, camera_);


    const bool renderCoarse = downsampling && interactionCoarseness_ > 1;
    glm::ivec2 renderSize;
    tgt::RenderTarget* renderDestination;
    if (renderCoarse) {
      renderSize = smallprivatetarget_->getSize();
      renderDestination = smallprivatetarget_;
    }
    else {
      renderSize = privatetarget_->getSize();
      renderDestination = privatetarget_;
    }

    // bind transfer function before active shader and target, because it may re-compute 
    // transfer function table use gpu by another shader in PreIntegration.
    if (transfunc_ && volume_ && volume_->IsReady()) {
      bindTransfuncTexture(classificationMode_, transfunc_, CalculateSamplingStepSize(volume_));
    }

    renderDestination->activateTarget();
    renderDestination->clearTarget();

    if (volume_ && volume_->IsReady()) {
      // activate shader and set common uniforms
      shader_->activate();
      setGlobalShaderParameters(shader_, camera_, renderSize);
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

      // pass transfer function to the shader
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

    renderDestination->deactivateTarget();

    glFinish();

    // blend with background
    renderBackground_->Process(renderDestination);

    // copy to the output
    output_->activateTarget();
    renderToScreen_->Process(renderBackground_->GetOutput());
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

    // 
    tgt::TriangleMeshGeometryVec4Vec3* boundingbox = tgt::TriangleMeshGeometryVec4Vec3::createCube(volume->getLLF(),
      volume->getURB(), glm::vec3(0.f), glm::vec3(1.f), 1.0f);
    boundingbox->transform(volume->getPhysicalToWorldMatrix());
    trackball_->adaptInteractionToScene(boundingbox->getBoundingBox(), glm::hmin(volume->getSpacing()));

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

  void RenderVolume::SetClassificationMode(const std::string& mode) 
  {
    if (classificationMode_ != mode) {
      classificationMode_ = mode;
      shader_->setFragmentHeader(generateHeader());
      shader_->rebuild();
    }
  }

  void RenderVolume::SetFirstColor(const glm::vec4 color)
  {
    renderBackground_->SetFirstColor(color);
  }

  glm::vec4 RenderVolume::GetFirstColor()
  {
    return renderBackground_->GetFirstColor();
  }

  void RenderVolume::SetSecondColor(const glm::vec4 color)
  {
    renderBackground_->SetSecondColor(color);
  }

  glm::vec4 RenderVolume::GetSecondColor()
  {
    return renderBackground_->GetSecondColor();
  }

  void RenderVolume::SetColorMode(const std::string& mode)
  {
    renderBackground_->SetColorMode(mode);
  }

  std::string RenderVolume::GetColorMode()
  {
    return renderBackground_->GetColorMode();
  }

  void RenderVolume::SaveToImage(const std::string& filename)
  {
    output_->saveToImage(filename);
  }

  void RenderVolume::SaveToImage(const std::string& filename, const glm::ivec2& newSize)
  {
    if (output_->getSize() != newSize) {
      glm::ivec2 oldSize = output_->getSize();
      Resize(newSize);
      Process(false);
      output_->saveToImage(filename);
      Resize(oldSize);
    }
    else {
      output_->saveToImage(filename);
    }
  }
}

