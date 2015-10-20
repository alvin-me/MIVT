#include "rendertoscreen.h"
#include "textureunit.h"
#include "shadermanager.h"
#include "rendertarget.h"

namespace mivt {
  RenderToScreen::RenderToScreen()
    : shader_(0)
  {
  }

  RenderToScreen::~RenderToScreen()
  {
  }

  void RenderToScreen::Initialize()
  {
    shader_ = ShdrMgr.loadSeparate("passthrough.vert", "copyimage.frag", "", false);
  }

  void RenderToScreen::Deinitialize()
  {
    ShdrMgr.dispose(shader_);
    shader_ = 0;
    LGL_ERROR;
  }

  void RenderToScreen::Process(tgt::RenderTarget *input)
  {
    if (input) {
      // map texture of input target onto a screen-aligned quad
      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // activate shader
      shader_->activate();

      // manually pass the viewport dimensions to the shader,
      // since setGlobalShaderParameters() expects a render outport, which we do not have
      shader_->setIgnoreUniformLocationError(true);
      shader_->setUniform("screenDim_", glm::vec2(size_));
      shader_->setUniform("screenDimRCP_", 1.f / glm::vec2(size_));
      shader_->setIgnoreUniformLocationError(false);

      // bind input textures
      input->bindTextures(GL_TEXTURE0, GL_TEXTURE1);

      // pass texture parameters to the shader
      shader_->setUniform("colorTex_", 0);
      shader_->setUniform("depthTex_", 1);
      input->setTextureParameters(shader_, "texParams_");
      LGL_ERROR;

      // execute the shader
      RenderQuad();
      shader_->deactivate();
      LGL_ERROR;
    }

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  void RenderToScreen::Resize(const glm::ivec2& newSize)
  {
    size_ = newSize;
  }

  void RenderToScreen::RenderQuad() {
    glDepthFunc(GL_ALWAYS);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f);
    glVertex2f(-1.f, -1.f);

    glTexCoord2f(1.f, 0.f);
    glVertex2f(1.f, -1.f);

    glTexCoord2f(1.f, 1.f);
    glVertex2f(1.f, 1.f);

    glTexCoord2f(0.f, 1.f);
    glVertex2f(-1.f, 1.f);
    glEnd();
    glDepthFunc(GL_LESS);
  }
}

