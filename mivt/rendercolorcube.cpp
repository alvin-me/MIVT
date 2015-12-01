#include "rendercolorcube.h"
#include "matrixstack.h"
#include "rendertarget.h"
#include "shadermanager.h"
#include "camera.h"
#include "geometry.h"

namespace mivt {
  RenderColorCube::RenderColorCube()
    : frontTarget_(0)
    , backTarget_(0)
    , tmpTarget_(0)
    , shader_(0)
    , shaderInsideVolume_(0)
  {
  }

  RenderColorCube::~RenderColorCube()
  {
  }

  void RenderColorCube::Initialize()
  {
    frontTarget_ = new tgt::RenderTarget();
    frontTarget_->initialize();
    backTarget_ = new tgt::RenderTarget();
    backTarget_->initialize();
    tmpTarget_ = new tgt::RenderTarget();
    tmpTarget_->initialize();

    shader_ = ShdrMgr.load("eep_simple", "", false);
    shaderInsideVolume_ = ShdrMgr.loadSeparate("passthrough.vert", "eep_insidevolume.frag", "", false);
  }

  void RenderColorCube::Deinitialize()
  {
    frontTarget_->deinitialize();
    DELPTR(frontTarget_);

    backTarget_->deinitialize();
    DELPTR(backTarget_);

    tmpTarget_->deinitialize();
    DELPTR(tmpTarget_);

    ShdrMgr.dispose(shader_);
    shader_ = 0;

    ShdrMgr.dispose(shaderInsideVolume_);
    shaderInsideVolume_ = 0;
  }

  void RenderColorCube::Process(tgt::Geometry *geometry, tgt::Camera *camera)
  {
    // set modelview and projection matrices
    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.pushMatrix();
    MatStack.loadMatrix(camera->getProjectionMatrix(frontTarget_->getSize()));

    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.pushMatrix();
    MatStack.loadMatrix(camera->getViewMatrix());

    glEnable(GL_DEPTH_TEST);

    // render front texture
    if (frontTarget_) {
      // simple implement
      //glEnable(GL_DEPTH_CLAMP);
      //renderGeometry(geometry, camera, frontTarget_, GL_LESS, 1.0f, GL_BACK);
      //glDisable(GL_DEPTH_CLAMP);

      // improved method for situation when camera inside volume.
      renderGeometry(geometry, camera, tmpTarget_, GL_LESS, 1.0f, GL_BACK); // render first front face
      renderGeometry(geometry, camera, backTarget_, GL_LESS, 1.0f, GL_FRONT); // render first back face
      fillEntryPoints(backTarget_, tmpTarget_, frontTarget_, geometry, camera);
    }

    // render back texture
    if (backTarget_) {
      renderGeometry(geometry, camera, backTarget_, GL_GREATER, 0.0f, GL_FRONT);
    }

    // restore OpenGL state
    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.popMatrix();
    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.popMatrix();

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  void RenderColorCube::Resize(const glm::ivec2& size)
  {
    frontTarget_->resize(size);
    backTarget_->resize(size);
    tmpTarget_->resize(size);
  }

  tgt::RenderTarget* RenderColorCube::GetFrontFace()
  {
    return frontTarget_;
  }

  tgt::RenderTarget* RenderColorCube::GetBackFace()
  {
    return backTarget_;
  }

  void RenderColorCube::renderGeometry(const tgt::Geometry* geometry,
    tgt::Camera *camera,
    tgt::RenderTarget* outport,
    GLenum depthFunc,
    float clearDepth,
    GLenum cullFace)
  {
    // activate shader program
    shader_->activate();

    setGlobalShaderParameters(shader_, camera, outport->getSize());
    shader_->setUniform("modelMatrix_", geometry->getTransformationMatrix());

    // enable culling
    glEnable(GL_CULL_FACE);

    outport->activateTarget();
    outport->clearTarget();
    glClearDepth(clearDepth);
    glDepthFunc(depthFunc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(cullFace);

    geometry->render();

    outport->deactivateTarget();

    glFinish();

    // deactivate shader program
    shader_->deactivate();

    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearDepth(1.0f);
    LGL_ERROR;
  }

  void RenderColorCube::fillEntryPoints(tgt::RenderTarget *firstBackTarget,
    tgt::RenderTarget *firstFrontTarget,
    tgt::RenderTarget *output,
    const tgt::Geometry *geometry,
    tgt::Camera *camera)
  {
    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.pushMatrix();
    MatStack.loadIdentity();

    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.pushMatrix();
    MatStack.loadIdentity();

    shaderInsideVolume_->activate();
    setGlobalShaderParameters(shaderInsideVolume_, camera, output->getSize());

    // bind texture
    tgt::TextureUnit firstFrontUnit, firstFrontDepthUnit, firstBackUnit, firstBackDepthUnit;

    // bind firstFront points texture and depth texture
    firstFrontTarget->bindColorTexture(firstFrontUnit.getEnum());
    shaderInsideVolume_->setUniform("firstFront_", firstFrontUnit.getUnitNumber());
    firstFrontTarget->bindDepthTexture(firstFrontDepthUnit.getEnum());
    shaderInsideVolume_->setUniform("firstFrontDepth_", firstFrontDepthUnit.getUnitNumber());
    firstFrontTarget->setTextureParameters(shaderInsideVolume_, "firstFrontParameters_");

    // bind firstBack points texture
    firstBackTarget->bindColorTexture(firstBackUnit.getEnum());
    shaderInsideVolume_->setUniform("firstBack_", firstBackUnit.getUnitNumber());
    firstBackTarget->bindDepthTexture(firstBackDepthUnit.getEnum());
    //shaderInsideVolume_->setUniform("firstBackDepth_", firstBackDepthUnit.getUnitNumber());
    firstBackTarget->setTextureParameters(shaderInsideVolume_, "firstBackParameters_");

    shaderInsideVolume_->setUniform("near_", camera->getNearDist());
    shaderInsideVolume_->setUniform("far_", camera->getFarDist());

    tgt::BoundingBox bounds = geometry->getBoundingBox(false);
    shaderInsideVolume_->setUniform("llf_", bounds.getLLF());
    shaderInsideVolume_->setUniform("urb_", bounds.getURB());

    glm::mat4 worldToTextureMatrix = glm::inverse(geometry->getTransformationMatrix());
    shaderInsideVolume_->setUniform("worldToTexture_", worldToTextureMatrix);

    output->activateTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render screen aligned quad
    renderQuad();

    shaderInsideVolume_->deactivate();
    output->deactivateTarget();

    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.popMatrix();
    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.popMatrix();
    LGL_ERROR;
  }
}

