#include "rendertarget.h"
#include "logmanager.h"
#include "gpucapabilities.h"
#include "texture.h"
#include "shadermanager.h"

#include <IL/il.h>
#include <IL/ilu.h>

namespace tgt {

  const std::string RenderTarget::loggerCat_ = "RenderTarget";

  RenderTarget::RenderTarget()
    : fbo_(0), colorTex_(0), depthTex_(0), cleared_(true)
  {
  }

  RenderTarget::~RenderTarget() {
    if (fbo_ || colorTex_ || depthTex_) {
      LERROR("~RenderTarget(): not deinitialized before destruction");
    }
  }

  void RenderTarget::initialize(GLint internalColorFormat, GLint internalDepthFormat) {

    if (fbo_)
      return;

    if (!GpuCaps.isNpotSupported() && !GpuCaps.areTextureRectanglesSupported()) {
      LWARNING("Neither non-power-of-two textures nor texture rectangles seem to be supported!");
    }

    glm::ivec3 size(128, 128, 1);

    switch (internalColorFormat) {
    case GL_RGB:
      colorTex_ = new Texture(0, size, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, Texture::LINEAR);
      break;
    case GL_RGBA:
      colorTex_ = new Texture(0, size, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, Texture::LINEAR);
      break;
    case GL_RGBA16:
      colorTex_ = new Texture(0, size, GL_RGBA, GL_RGBA16, GL_UNSIGNED_SHORT, Texture::LINEAR);
      break;
    case GL_RGB16F_ARB:
      colorTex_ = new Texture(0, size, GL_RGB, GL_RGB16F_ARB, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_RGBA16F_ARB:
      colorTex_ = new Texture(0, size, GL_RGBA, GL_RGBA16F_ARB, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_RGBA32F_ARB:
      colorTex_ = new Texture(0, size, GL_RGBA, GL_RGBA32F_ARB, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_RED:
      colorTex_ = new Texture(0, size, GL_RED, GL_RED, GL_UNSIGNED_BYTE, Texture::LINEAR);
      break;
    case GL_R32F:
      colorTex_ = new Texture(0, size, GL_RED, GL_R32F, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_R32UI:
      colorTex_ = new Texture(0, size, GL_RED_INTEGER, GL_R32UI, GL_UNSIGNED_INT, Texture::LINEAR);
      break;
    case GL_LUMINANCE32F_ARB:
      colorTex_ = new Texture(0, size, GL_LUMINANCE, GL_LUMINANCE32F_ARB, GL_FLOAT, Texture::LINEAR);
      break;
    default:
      LERROR("Unknown internal format!");
    }
    colorTex_->uploadTexture();
    colorTex_->setWrapping(Texture::CLAMP_TO_EDGE);

    switch (internalDepthFormat) {
    case GL_DEPTH_COMPONENT16:
      depthTex_ = new Texture(0, size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_DEPTH_COMPONENT24:
      depthTex_ = new Texture(0, size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_FLOAT, Texture::LINEAR);
      break;
    case GL_DEPTH_COMPONENT32:
      depthTex_ = new Texture(0, size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32, GL_FLOAT, Texture::LINEAR);
      break;
#ifdef GL_DEPTH_COMPONENT32F
    case GL_DEPTH_COMPONENT32F:
      depthTex_ = new Texture(0, size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F, GL_FLOAT, Texture::LINEAR);
      break;
#endif
    default:
      depthTex_ = new Texture(0, size, GL_DEPTH_COMPONENT, internalDepthFormat, GL_FLOAT, Texture::LINEAR);
      LERROR("Unknown internal depth format!");
    }
    depthTex_->uploadTexture();
    depthTex_->setWrapping(Texture::CLAMP_TO_EDGE);

    fbo_ = new FramebufferObject();
    if (!fbo_) {
      LERROR("Failed to initialize framebuffer object!");
      return;
    }
    fbo_->activate();

    fbo_->attachTexture(colorTex_);
    fbo_->isComplete();

    fbo_->attachTexture(depthTex_, GL_DEPTH_ATTACHMENT_EXT);
    fbo_->isComplete();

    fbo_->deactivate();

    cleared_ = true;
  }

  void RenderTarget::deinitialize() {
    delete fbo_;
    fbo_ = 0;

    delete colorTex_;
    colorTex_ = 0;

    delete depthTex_;
    depthTex_ = 0;
  }

  void RenderTarget::activateTarget() {
    if (!fbo_) {
      LERROR("activateTarget(): not initialized");
      return;
    }
    fbo_->activate();
    glViewport(0, 0, colorTex_->getWidth(), colorTex_->getHeight());

    cleared_ = false;
  }

  void RenderTarget::deactivateTarget() {
    if (fbo_)
      fbo_->deactivate();

    cleared_ = false;
  }

  bool RenderTarget::isActive() const {
    return (fbo_ && fbo_->isActive());
  }

  void RenderTarget::clearTarget() {
    if (!isActive())
      LERROR("clearTarget() called on inactive outport");
    else {
      glClearColor(0.f, 0.f, 0.f, 0.f);
      glClearDepth(1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
  }

  void RenderTarget::clear() {
    if (!cleared_) {
      activateTarget();
      clearTarget();
      deactivateTarget();
      cleared_ = true;
    }
  }

  void RenderTarget::resize(glm::ivec2 newsize) {
    if (!fbo_) {
      //LERROR("resize(): not initialized");
      return;
    }

    // no change
    if (newsize == colorTex_->getDimensions().xy())
      return;

    if (newsize == glm::ivec2(0)) {
      LWARNING("resize(): invalid size " << glm::to_string(newsize));
      return;
    }

    // recreate fbo textures
    glActiveTexture(GL_TEXTURE0);
    colorTex_->destroy();
    colorTex_->setDimensions(glm::ivec3(newsize.x, newsize.y, 1));
    colorTex_->uploadTexture();

    depthTex_->destroy();
    depthTex_->setDimensions(glm::ivec3(newsize.x, newsize.y, 1));
    depthTex_->uploadTexture();
  }

  void RenderTarget::setTextureParameters(Shader* shader, const std::string& uniform) {
    assert(shader);
    bool oldIgnoreError = shader->getIgnoreUniformLocationError();
    shader->setIgnoreUniformLocationError(true);
    shader->setUniform(uniform + ".dimensions_", glm::vec2(getSize()));
    shader->setUniform(uniform + ".dimensionsRCP_", glm::vec2(1.f) / glm::vec2(getSize()));
    shader->setUniform(uniform + ".matrix_", glm::mat4(1));
    shader->setIgnoreUniformLocationError(oldIgnoreError);
  }

  void RenderTarget::bindColorTexture() {
    assert(colorTex_);
    if (colorTex_)
      colorTex_->bind();
  }

  void RenderTarget::bindColorTexture(GLint texUnit, GLint filterMode/* = GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= glm::vec4(0.f)*/) {
    glActiveTexture(texUnit);
    bindColorTexture();

    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    LGL_ERROR;

    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapMode);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
    LGL_ERROR;
  }

  void RenderTarget::bindColorTexture(TextureUnit& texUnit, GLint filterMode /*= GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= vec4(0.f)*/) {
    bindColorTexture(texUnit.getEnum(), filterMode, wrapMode, borderColor);
  }

  void RenderTarget::bindDepthTexture() {
    assert(depthTex_);
    if (depthTex_)
      depthTex_->bind();
  }

  void RenderTarget::bindDepthTexture(GLint texUnit, GLint filterMode/* = GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= glm::vec4(0.f)*/) {
    glActiveTexture(texUnit);
    bindDepthTexture();

    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    LGL_ERROR;

    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapMode);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
    LGL_ERROR;
  }

  void RenderTarget::bindDepthTexture(TextureUnit& texUnit, GLint filterMode /*= GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= vec4(0.f)*/) {
    bindDepthTexture(texUnit.getEnum(), filterMode, wrapMode, borderColor);
  }

  void RenderTarget::bindTextures(GLint colorUnit, GLint depthUnit, GLint filterMode /*= GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= vec4(0.f)*/) {
    bindColorTexture(colorUnit, filterMode, wrapMode, borderColor);
    bindDepthTexture(depthUnit, filterMode, wrapMode, borderColor);
  }

  void RenderTarget::bindTextures(TextureUnit& colorUnit, TextureUnit& depthUnit, GLint filterMode /*= GL_LINEAR*/, GLint wrapMode /*= GL_CLAMP_TO_EDGE*/, glm::vec4 borderColor /*= vec4(0.f)*/) {
    bindColorTexture(colorUnit, filterMode, wrapMode, borderColor);
    bindDepthTexture(depthUnit, filterMode, wrapMode, borderColor);
  }

  glm::ivec2 RenderTarget::getSize() const {
    if (colorTex_)
      return colorTex_->getDimensions().xy();
    else if (depthTex_)
      return depthTex_->getDimensions().xy();
    else
      return glm::ivec2(0, 0);
  }

  glm::vec4 RenderTarget::getColorAtPos(glm::ivec2 pos) {
    activateTarget();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    float pixels[4];
    glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_FLOAT, pixels);
    deactivateTarget();
    return glm::make_vec4(pixels);
  }

  void RenderTarget::readColorBuffer(uint8_t* buffer, size_t numBytesAllocated) {
    if (!getColorTexture()) {
      throw Exception("RenderTarget::readColorBuffer() called on an empty render target");
    }
    getColorTexture()->downloadTextureToBuffer(buffer, numBytesAllocated);
  }

  void RenderTarget::saveToImage(const std::string& filename) throw (Exception) {

    if (filename.empty())
      throw Exception("filename is empty");
    else if (FileSystem::fileExtension(filename).empty())
      throw Exception("filename has no extension");

    // get color buffer content
    uint16_t* colorBuffer = readColorBuffer<uint16_t>();
    glm::ivec2 size = getSize();

    // create Devil image from image data and write it to file
    ILuint img;
    ilGenImages(1, &img);
    ilBindImage(img);
    // put pixels into IL-Image
    ilTexImage(size.x, size.y, 1, 3, IL_RGB, IL_UNSIGNED_SHORT, colorBuffer);
    ilEnable(IL_FILE_OVERWRITE);
    ilResetWrite();
    ILboolean success = ilSaveImage(const_cast<char*>(filename.c_str()));
    ilDeleteImages(1, &img);

    delete[] colorBuffer;

    if (!success) {
      ILenum error = ilGetError();
      throw Exception(std::string(iluErrorString(error)));
    }
  }

} // end namespace tgt