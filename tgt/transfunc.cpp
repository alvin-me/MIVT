#include "transfunc.h"

#include "logmanager.h"
#include "shadermanager.h"
#include "gpucapabilities.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"

namespace tgt {

  const std::string TransFunc::loggerCat_("TransFunc");

  TransFunc::TransFunc(int width, int height, int depth,
    GLint format, GLenum dataType, Texture::Filter filter)
    : tex_(0)
    , dimensions_(width, height, depth)
    , format_(format)
    , dataType_(dataType)
    , filter_(filter)
    , gammaValue_(1.f)
    , textureInvalid_(true)
  {}

  TransFunc::~TransFunc() {
    DELPTR(tex_);
    LGL_ERROR;
  }

  void TransFunc::invalidateTexture() {
    textureInvalid_ = true;
  }

  bool TransFunc::isTextureInvalid() const {
    return textureInvalid_;
  }

  void TransFunc::bind() {
    if (textureInvalid_) {
      updateTexture();
    }

    assert(tex_);
    tex_->bind();
  }

  void TransFunc::createTex() {
    DELPTR(tex_);

    tex_ = new Texture(dimensions_, format_, dataType_, filter_);
    tex_->setWrapping(Texture::CLAMP_TO_BORDER);
    LGL_ERROR;
  }

  void TransFunc::deleteTexture() {
    DELPTR(tex_);
    textureInvalid_ = true;
    LGL_ERROR;
  }

  void TransFunc::updateTexture() {
    if (!tex_ || (tex_->getDimensions() != dimensions_))
      createTex();

    if (!tex_) {
      LERROR("Failed to create texture");
      return;
    }

    tex_->uploadTexture();
    textureInvalid_ = false;
  }

  Texture* TransFunc::getTexture() {
    if (textureInvalid_)
      updateTexture();

    return tex_;
  }

  void TransFunc::setUniform(Shader* shader, const std::string& uniform, const std::string& uniformTex, const GLint texUnit) {
    assert(shader);
    bool oldIgnoreError = shader->getIgnoreUniformLocationError();
    shader->setIgnoreUniformLocationError(true);
    shader->setUniform(uniformTex, texUnit);

    shader->setUniform(uniform + ".windowingLowerHu_", getWindowingDomain().x);
    shader->setUniform(uniform + ".windowingUpperHu_", getWindowingDomain().y);

    //if (getNumDimensions() == 1) {
    //  shader->setUniform(uniform + ".domainLower_", glm::vec3(getDataDomain(0).x));
    //  shader->setUniform(uniform + ".domainUpper_", glm::vec3(getDataDomain(0).y));
    //}
    //else if (getNumDimensions() == 2) {
    //  shader->setUniform(uniform + ".domainLower_", glm::vec3(getDataDomain(0).x, getDataDomain(1).x, 0.0f));
    //  shader->setUniform(uniform + ".domainUpper_", glm::vec3(getDataDomain(0).y, getDataDomain(1).y, 0.0f));
    //}
    //else {
    //  LERROR("Unhandled dimensionality in glsl TransFunc object");
    //}

    shader->setIgnoreUniformLocationError(oldIgnoreError);
  }

  void TransFunc::resize(int width, int height, int depth) {
    fitDimensions(width, height, depth);

    if (width != dimensions_.x) {
      dimensions_.x = width;
      textureInvalid_ = true;
    }
    if (height != dimensions_.y) {
      dimensions_.y = height;
      textureInvalid_ = true;
    }
    if (depth != dimensions_.z) {
      dimensions_.z = depth;
      textureInvalid_ = true;
    }
  }

  glm::ivec3 TransFunc::getDimensions() const {
    return dimensions_;
  }

  void TransFunc::fitDimensions(int& width, int& height, int& depth) const {
    int maxTexSize = 1024;
    if (Singleton<GpuCapabilities>::isInited()) {
      if (depth == 1)
        maxTexSize = GpuCaps.getMaxTextureSize();
      else
        maxTexSize = GpuCaps.getMax3DTextureSize();
    }

    if (maxTexSize < width)
      width = maxTexSize;

    if (maxTexSize < height)
      height = maxTexSize;

    if (maxTexSize < depth)
      depth = maxTexSize;
  }

  int TransFunc::getNumDimensions() const {
    if (dimensions_.z == 1) {
      if (dimensions_.y == 1)
        return 1;
      else
        return 2;
    }
    else
      return 3;
  }

  void TransFunc::setPixelData(GLubyte* data) {
    if (!tex_ || (tex_->getDimensions() != dimensions_))
      createTex();
    assert(tex_);

    if (tex_->getPixelData() != reinterpret_cast<GLubyte*>(data))
      tex_->destroy();
    tex_->setPixelData(data);

    textureInvalid_ = true;
  }

  GLubyte* TransFunc::getPixelData() {
    if (textureInvalid_)
      updateTexture();
    assert(tex_);
    return tex_->getPixelData();
  }

  void TransFunc::reset() {
    createTex();
  }

  glm::vec2 TransFunc::getDataDomain(int /*dimension*/) const {
    return glm::vec2(0.0f, 255.0f);
  }

  void TransFunc::setDataDomain(glm::vec2 /*domain*/, int /*dimension*/) {
    // no-op
  }

  glm::vec2 TransFunc::getWindowingDomain() const {
    return glm::vec2(0.0f, 255.0f);
  }

  void TransFunc::setWindowingDomain(glm::vec2 /*domain*/) {
    // no-op
  }

  void TransFunc::setGammaValue(float gamma) {
    if (gamma != gammaValue_) {
      gammaValue_ = gamma;
      invalidateTexture();
    }
  }

  float TransFunc::getGammaValue() const {
    return gammaValue_;
  }

  bool TransFunc::load(const std::string& /*filename*/) {
    return false;  // override in a subclass
  }

  bool TransFunc::save(const std::string& /*filename*/) const {
    return false;  // override in a subclass
  }

  void TransFunc::serialize(XmlSerializer& s) const {
    s.serialize("gammaValue", gammaValue_);
  }

  void TransFunc::deserialize(XmlDeserializer& s) {
    s.deserialize("gammaValue", gammaValue_);
  }

} // end namespace tgt