#include "texture.h"
#include "logmanager.h"
#include "gpucapabilities.h"

namespace tgt {

  const std::string Texture::loggerCat_ = "Texture";

  Texture::Texture(const glm::ivec3& dimensions, GLint format, GLint internalformat,
    GLenum dataType, Filter filter)
    : dimensions_(dimensions)
    , format_(format)
    , internalformat_(internalformat)
    , dataType_(dataType)
    , filter_(filter)
    , wrapping_(REPEAT)
    , priority_(-1.f)
    , pixels_(0)
  {
    init(true);
  }

  Texture::Texture(const glm::ivec3& dimensions, GLint format,
    GLenum dataType, Filter filter)
    : dimensions_(dimensions)
    , format_(format)
    , internalformat_(format)
    , dataType_(dataType)
    , filter_(filter)
    , wrapping_(REPEAT)
    , priority_(-1.f)
    , pixels_(0)
  {
    init(true);
  }

  Texture::Texture(GLubyte* data, const glm::ivec3& dimensions, GLint format, GLint internalformat,
    GLenum dataType, Filter filter)
    : dimensions_(dimensions)
    , format_(format)
    , internalformat_(internalformat)
    , dataType_(dataType)
    , filter_(filter)
    , wrapping_(REPEAT)
    , priority_(-1.f)
    , pixels_(data)
  {
    init(false);
    arraySize_ = glm::hmul(dimensions_) * bpp_;
  }

  Texture::Texture(GLubyte* data, const glm::ivec3& dimensions, GLint format,
    GLenum dataType, Filter filter)
    : dimensions_(dimensions)
    , format_(format)
    , internalformat_(format)
    , dataType_(dataType)
    , filter_(filter)
    , wrapping_(REPEAT)
    , priority_(-1.f)
    , pixels_(data)
  {
    init(false);
    arraySize_ = glm::hmul(dimensions_) * bpp_;
  }

  void Texture::init(bool allocData) {
#ifndef GL_TEXTURE_RECTANGLE_ARB
    textureRectangle = false;
#endif

    calcType();
    bpp_ = (GLubyte)calcBpp(format_, dataType_);

    generateId();

    if (allocData)
      alloc();

    applyFilter();
  }

  int Texture::calcBpp(GLint format, GLenum dataType) {

    int numChannels = calcNumChannels(format);

    int typeSize = 0;
    switch (dataType) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
      typeSize = 1;
      break;

    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
      typeSize = 2;
      break;

    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
      typeSize = 4;
      break;

    case GL_DOUBLE:
      typeSize = 8;
      break;

    default:
      LWARNING("unknown dataType");
    }

    return typeSize * numChannels;
  }

  int Texture::calcBpp(GLint internalformat) {

    int bpp = 0;
    switch (internalformat) {
    case 1:
    case GL_COLOR_INDEX:
    case GL_RED:
    case GL_GREEN:
    case GL_BLUE:
    case GL_ALPHA:
    case GL_INTENSITY:
    case GL_LUMINANCE:
    case GL_DEPTH_COMPONENT:
      bpp = 1;
      break;

    case 2:
    case GL_LUMINANCE_ALPHA:
    case GL_RG:
    case GL_INTENSITY16:
    case GL_DEPTH_COMPONENT16:
      bpp = 2;
      break;

    case GL_RGB:
    case GL_BGR:
    case GL_DEPTH_COMPONENT24:
      bpp = 3;
      break;

    case GL_RGBA:
    case GL_RGBA8:
    case GL_BGRA:
    case GL_DEPTH_COMPONENT32:
    case GL_R32F:
    case GL_R32UI:
    case GL_LUMINANCE32F_ARB:
    case GL_RG32F:
      bpp = 4;
      break;

    case GL_RGB16:
    case GL_RGB16F_ARB:
      bpp = 6;
      break;

    case GL_RGBA16:
    case GL_RGBA16F_ARB:
      bpp = 8;
      break;

    case GL_RGB32F_ARB:
      bpp = 12;
      break;

    case GL_RGBA32F_ARB:
      bpp = 16;
      break;

    default:
      LWARNING("unknown internal format");
      break;
    }

    return bpp;
  }

  int Texture::calcNumChannels(GLint format) {

    switch (format) {
    case 1:
    case GL_COLOR_INDEX:
    case GL_RED:
    case GL_RED_INTEGER:
    case GL_R32F:
    case GL_GREEN:
    case GL_BLUE:
    case GL_ALPHA:
    case GL_INTENSITY:
    case GL_LUMINANCE:
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT24:
    case GL_ALPHA_INTEGER_EXT:
      return 1;
      break;

    case 2:
    case GL_LUMINANCE_ALPHA:
    case GL_RG:
      return 2;
      break;

    case 3:
    case GL_RGB:
    case GL_BGR:
      return 3;
      break;

    case 4:
    case GL_RGBA:
    case GL_BGRA:
    case GL_RGBA16:
    case GL_RGBA16F_ARB:
    case GL_LUMINANCE32F_ARB:
    case GL_RG32F:
      return 4;
      break;

    default:
      LWARNING("unknown format");
      return 0;
    }
  }

  size_t Texture::getSizeOnGPU() const {
    int bpp = calcBpp(internalformat_);
    return bpp * glm::hmul(dimensions_);
  }

  GLenum Texture::calcType(bool textureRectangle) {
    if (dimensions_.z == 1)    {
      if (dimensions_.y == 1)
        type_ = GL_TEXTURE_1D;
      else
        type_ = GL_TEXTURE_2D;
    }
    else {
      type_ = GL_TEXTURE_3D;
    }

#ifdef GL_TEXTURE_RECTANGLE_ARB
    if (type_ == GL_TEXTURE_2D && textureRectangle)
      type_ = GL_TEXTURE_RECTANGLE_ARB;
#endif

    return type_;
  }

  void Texture::setFilter(Filter filter) {
    filter_ = filter;
    applyFilter();
  }

  void Texture::applyFilter() {
    bind();

    switch (filter_) {
    case NEAREST:
      glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      break;

    case LINEAR:
      glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      break;

    case ANISOTROPIC:
      glTexParameterf(type_, GL_TEXTURE_MAX_ANISOTROPY_EXT, GpuCaps.getMaxTextureAnisotropy());

    case MIPMAP:
      glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(type_, GL_SGIS_generate_mipmap, GL_TRUE);
      break;
    }
  }

  void Texture::setWrapping(Wrapping w) {
    wrapping_ = w;
    applyWrapping();
  }

  void Texture::applyWrapping() {
    bind();
    GLint wrap = wrapping_;

    /*
    set wrapping for all diminesions
    */
    glTexParameteri(type_, GL_TEXTURE_WRAP_S, wrap);

    if (type_ == GL_TEXTURE_2D || type_ == GL_TEXTURE_3D)
      glTexParameteri(type_, GL_TEXTURE_WRAP_T, wrap);
    if (GL_TEXTURE_3D)
      glTexParameteri(type_, GL_TEXTURE_WRAP_R, wrap);

    // set clamp border color
    if (wrapping_ == CLAMP_TO_BORDER) {
      float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glTexParameterfv(type_, GL_TEXTURE_BORDER_COLOR, color);
    }
  }

  void Texture::uploadTexture() {
    bind();

    switch (type_) {
    case GL_TEXTURE_1D:
      glTexImage1D(GL_TEXTURE_1D, 0, internalformat_, dimensions_.x, 0,
        format_, dataType_, pixels_);
      break;

    case GL_TEXTURE_2D:
      glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, dimensions_.x, dimensions_.y, 0,
        format_, dataType_, pixels_);
      break;

    case GL_TEXTURE_3D:
      glTexImage3D(GL_TEXTURE_3D, 0, internalformat_,
        dimensions_.x, dimensions_.y, dimensions_.z, 0,
        format_, dataType_, pixels_);
      break;

#ifdef GL_TEXTURE_RECTANGLE_ARB
    case GL_TEXTURE_RECTANGLE_ARB:
      glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, internalformat_, dimensions_.x, dimensions_.y, 0,
        format_, dataType_, pixels_);
      break;
#endif

    }
  }

  void Texture::downloadTexture() const {
    bind();

    if (pixels_ == 0)
      alloc();

    glGetTexImage(type_, 0, format_, dataType_, pixels_);
  }

  GLubyte* Texture::downloadTextureToBuffer() const {
    bind();

    size_t arraySize = glm::hmul(dimensions_) * bpp_;
    GLubyte* pixels = new GLubyte[arraySize];

    glGetTexImage(type_, 0, format_, dataType_, pixels);
    return pixels;
  }

  void Texture::downloadTextureToBuffer(GLubyte* pixels, size_t numBytesAllocated) const {
    bind();

    size_t arraySize = glm::hmul(dimensions_) * bpp_;
    if (numBytesAllocated < arraySize) {
      LWARNING("downloadTextureToBuffer: allocated buffer is too small");
    }
    else {
      glGetTexImage(type_, 0, format_, dataType_, pixels);
    }
  }

  GLubyte* Texture::downloadTextureToBuffer(GLint format, GLenum dataType) const {
    bind();

    size_t arraySize = glm::hmul(dimensions_) * calcBpp(format, dataType);
    GLubyte* pixels = new GLubyte[arraySize];

    glGetTexImage(type_, 0, format, dataType, pixels);
    return pixels;
  }

  void Texture::downloadTextureToBuffer(GLint format, GLenum dataType, GLubyte* pixels, size_t numBytesAllocated) const {
    bind();

    size_t arraySize = glm::hmul(dimensions_) * calcBpp(format, dataType);
    if (numBytesAllocated < arraySize) {
      LWARNING("downloadTextureToBuffer: allocated buffer is too small");
    }
    else {
      glGetTexImage(type_, 0, format, dataType, pixels);
    }
  }

  bool Texture::isTextureRectangle() const {
#ifdef GL_TEXTURE_RECTANGLE_ARB
    return (type_ == GL_TEXTURE_RECTANGLE_ARB);
#else
    return false;
#endif
  }

} // end namespace tgt