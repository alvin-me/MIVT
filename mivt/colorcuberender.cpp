#include "colorcuberender.h"
#include "matrixstack.h"
#include "rendertarget.h"
#include "shadermanager.h"
#include "camera.h"
#include "geometry.h"
#include "rendererhelper.h"

namespace mivt {
  ColorCubeRender::ColorCubeRender()
    : frontFace_(0)
    , backFace_(0)
    , shader_(0)
  {
  }

  ColorCubeRender::~ColorCubeRender()
  {
  }

  void ColorCubeRender::Initialize()
  {
    frontFace_ = new tgt::RenderTarget();
    frontFace_->initialize();
    backFace_ = new tgt::RenderTarget();
    backFace_->initialize();

    shader_ = ShdrMgr.load("eep_simple", "", false);
  }

  void ColorCubeRender::Deinitialize()
  {
    frontFace_->deinitialize();
    DELPTR(frontFace_);
    backFace_->deinitialize();
    DELPTR(backFace_);

    ShdrMgr.dispose(shader_);
    shader_ = 0;
  }

  void ColorCubeRender::Process(tgt::Geometry *geometry, tgt::Camera *camera)
  {
    // set modelview and projection matrices
    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.pushMatrix();
    MatStack.loadMatrix(camera->getProjectionMatrix(frontFace_->getSize()));

    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.pushMatrix();
    MatStack.loadMatrix(camera->getViewMatrix());

    // render front texture
    if (frontFace_) {
      renderGeometry(geometry, camera, frontFace_, GL_LESS, 1.0f, GL_BACK);
    }

    // render back texture
    if (backFace_) {
      renderGeometry(geometry, camera, backFace_, GL_GREATER, 0.0f, GL_FRONT);
    }

    // restore OpenGL state
    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.popMatrix();
    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.popMatrix();

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  void ColorCubeRender::Resize(const glm::ivec2& size)
  {
    frontFace_->resize(size);
    backFace_->resize(size);
  }

  tgt::RenderTarget* ColorCubeRender::GetFrontFace()
  {
    return frontFace_;
  }

  tgt::RenderTarget* ColorCubeRender::GetBackFace()
  {
    return backFace_;
  }

  void ColorCubeRender::renderGeometry(const tgt::Geometry* geometry,
    tgt::Camera *camera,
    tgt::RenderTarget* outport,
    GLenum depthFunc,
    float clearDepth,
    GLenum cullFace)
  {
    // activate shader program
    shader_->activate();

    RendererHelper::setGlobalShaderParameters(shader_, camera, outport->getSize());
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

}

