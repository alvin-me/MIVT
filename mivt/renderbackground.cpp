#include "renderbackground.h"
#include "texture.h"
#include "matrixstack.h"
#include "rendertarget.h"
#include "shadermanager.h"

namespace mivt {
  const std::string RenderBackground::loggerCat_ = "RenderBackground";

  RenderBackground::RenderBackground()
    : firstcolor_(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
    , secondcolor_(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f))
    , angle_(0)
    , tex_(0)
    , output_(0)
    , privatetarget_(0)
    , program_(0)
    , mode_("radial")
  {
  }

  RenderBackground::~RenderBackground()
  {
  }

  void RenderBackground::Initialize()
  {
    output_ = new tgt::RenderTarget();
    output_->initialize();

    privatetarget_ = new tgt::RenderTarget();
    privatetarget_->initialize();

    program_ = ShdrMgr.loadSeparate("passthrough.vert", "background.frag", "", false);

    loadTexture();
  }

  void RenderBackground::Deinitialize()
  {
    DELPTR(tex_);

    output_->deinitialize();
    DELPTR(output_);

    privatetarget_->deinitialize();
    DELPTR(privatetarget_);

    ShdrMgr.dispose(program_);
    program_ = 0;
  }

  void RenderBackground::Resize(const glm::ivec2& newSize)
  {
    output_->resize(newSize);
    privatetarget_->resize(newSize);
  }

  void RenderBackground::Process(tgt::RenderTarget *input)
  {
    privatetarget_->activateTarget();

    MatStack.matrixMode(tgt::MatrixStack::PROJECTION);
    MatStack.loadIdentity();

    MatStack.matrixMode(tgt::MatrixStack::MODELVIEW);
    MatStack.loadIdentity();

    glClearDepth(1.0);

    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first the background
    renderBackground();

    privatetarget_->deactivateTarget();

    ///
    ///

    glEnable(GL_DEPTH_TEST);

    // blending background with input
    output_->activateTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the shader to to blend the original picture over the background and to keep the
    // depth values

    tgt::TextureUnit colorUnit0, depthUnit0, colorUnit1, depthUnit1;
    privatetarget_->bindTextures(colorUnit0.getEnum(), depthUnit0.getEnum());
    input->bindTextures(colorUnit1.getEnum(), depthUnit1.getEnum());

    // initialize shader
    program_->activate();
    setGlobalShaderParameters(program_, 0, output_->getSize());
    program_->setUniform("colorTex0_", colorUnit0.getUnitNumber());
    program_->setUniform("depthTex0_", depthUnit0.getUnitNumber());
    program_->setUniform("colorTex1_", colorUnit1.getUnitNumber());
    program_->setUniform("depthTex1_", depthUnit1.getUnitNumber());
    privatetarget_->setTextureParameters(program_, "textureParameters0_");
    input->setTextureParameters(program_, "textureParameters1_");

    glDepthFunc(GL_ALWAYS);
    renderQuad();
    glDepthFunc(GL_LESS);
    program_->deactivate();

    output_->deactivateTarget();
    
    tgt::TextureUnit::setZeroUnit();
  }

  void RenderBackground::renderBackground()
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    if (mode_ == "monochrome") {
      glPushAttrib(GL_COLOR_BUFFER_BIT);
      glClearColor(firstcolor_.r, firstcolor_.g, firstcolor_.b, firstcolor_.a);
      glClear(GL_COLOR_BUFFER_BIT);
      glPopAttrib();
      LGL_ERROR;
    }
    else if (mode_ == "gradient") {
      // linear gradient
      MatStack.pushMatrix();
      MatStack.loadIdentity();
      MatStack.rotate(static_cast<float>(angle_), 0.0f, 0.0f, 1.0f);

      // when you rotate the texture, you need to scale it.
      // otherwise the edges don't cover the complete screen
      // magic number: 0.8284271247461900976033774484194f = sqrt(8)-2;
      MatStack.scale(1.0f + (45 - abs(angle_ % 90 - 45)) / 45.0f*0.8284271247461900976033774484194f,
        1.0f + (45 - abs(angle_ % 90 - 45)) / 45.0f*0.8284271247461900976033774484194f, 1.0f);

      glBegin(GL_QUADS);
      glColor4fv(glm::value_ptr(firstcolor_));
      glVertex2f(-1.0, -1.0);
      glVertex2f(1.0, -1.0);
      glColor4fv(glm::value_ptr(secondcolor_));
      glVertex2f(1.0, 1.0);
      glVertex2f(-1.0, 1.0);
      glEnd();
      MatStack.popMatrix();
      LGL_ERROR;
    }
    else if (mode_ == "radial") {
      tgt::TextureUnit::setZeroUnit();

      if (!tex_)
        loadTexture();

      if (tex_) {
        tex_->bind();
        tex_->enable();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        LGL_ERROR;

        MatStack.pushMatrix();
        MatStack.scale(1.44f, 1.44f, 0.0f);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        MatStack.popMatrix();

        glDisable(GL_TEXTURE_2D);
        LGL_ERROR;
      }
      else {
        LWARNING("No texture");
      }
    }
    else if (mode_ == "none") {

      if (!tex_)
        loadTexture();

      if (tex_) {
        glActiveTexture(GL_TEXTURE0);
        tex_->bind();
        tex_->enable();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        LGL_ERROR;

        glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        tex_->disable();
        LGL_ERROR;
      }
    }
    else {
      LWARNING("Unknown background mode");
    }
  }

  void RenderBackground::loadTexture()
  {
    // is a texture already loaded? -> then delete
    DELPTR(tex_);

    // create Texture
    if (mode_ == "none") {
      createEmptyTexture();
    }
    else if (mode_ == "radial") {
      createRadialTexture();
    }
  }

  void RenderBackground::createRadialTexture() {
    int imgx = 256;
    int imgy = 256;
    tex_ = new tgt::Texture(glm::ivec3(imgx, imgy, 1));

    // create pixel data
    float r;
    for (int y = 0; y < imgy; y++) {
      for (int x = 0; x < imgx; x++) {
        // calculate radius (Pythagoras)
        r = static_cast<float>((x - imgx / 2) * (x - imgx / 2) + (y - imgy / 2) * (y - imgy / 2));
        r = sqrt(r);
        // norm to half size of smaller value
        r = 2 * r / ((imgx < imgy) ? imgx : imgy);
        // mix both colors according to radius
        r = glm::clamp(r, 0.f, 1.f);
        glm::vec4 mixedColor = glm::mix(firstcolor_, secondcolor_, r);
        tex_->texel<glm::col4>(x, y) = glm::col4(glm::round(mixedColor * 255.f));;
      }
    }

    tex_->bind();
    tex_->uploadTexture();
    LGL_ERROR;
  }

  void RenderBackground::createEmptyTexture() {
    int imgx = 256;
    int imgy = 256;
    tex_ = new tgt::Texture(glm::ivec3(imgx, imgy, 1));

    // create pixel data
    for (int y = 0; y < imgy; y++) {
      for (int x = 0; x < imgx; x++) {
        tex_->texel<glm::col4>(x, y) = glm::col4(0, 0, 0, 0);
      }
    }

    tex_->bind();
    tex_->uploadTexture();
    LGL_ERROR;
  }

  tgt::RenderTarget* RenderBackground::GetOutput()
  {
    return output_;
  }
}

