#include "volumegl.h"
#include "gpucapabilities.h"
#include "logmanager.h"
#include "volumeatomic.h"

namespace tgt {

  const std::string VolumeGL::loggerCat_("VolumeGL");

  VolumeGL::VolumeGL(const VolumeRAM* volume) throw (tgt::Exception, std::bad_alloc)
    : VolumeRepresentation(volume->getDimensions())
    , texture_(0)
  {
    assert(volume);
    generateTexture(volume);
  }

  VolumeGL::~VolumeGL() {
    destroy();
  }

  void VolumeGL::destroy() {
    delete texture_;
  }

  const VolumeTexture* VolumeGL::getTexture() const {
    return texture_;
  }

  VolumeTexture* VolumeGL::getTexture() {
    return texture_;
  }

  void VolumeGL::generateTexture(const VolumeRAM* volume)
    throw (tgt::Exception, std::bad_alloc)
  {
    if (!GpuCaps.is3DTexturingSupported()) {
      std::string message = "3D textures apparently not supported by the OpenGL driver";
      LERROR(message);
      throw Exception(message);
    }

    if (!GpuCaps.isNpotSupported() && !glm::all(glm::isPowerOfTwo(getDimensions()))) {
      std::string message = "Non-power-of-two textures apparently not supported by the OpenGL driver";
      LERROR(message);
      throw Exception(message);
    }

    //
    // Determine GL format
    //
    GLint format = 0;         ///< The format of textures which will are created.
    GLint internalFormat = 0; ///< The internal format of the textures which are created.
    GLenum dataType = 0;      ///< The data type of the textures which are created.
    float scale = 1.f;        ///< rescale data from normalized interger to interger. 
    // scalar
    if (dynamic_cast<const VolumeAtomic<uint8_t>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R8;
      dataType = GL_UNSIGNED_BYTE;
      scale = (float)std::numeric_limits<uint8_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<int8_t>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R8;
      dataType = GL_BYTE;
      scale = (float)std::numeric_limits<int8_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<uint16_t>*>(volume)) {
      dataType = GL_UNSIGNED_SHORT;
      scale = (float)std::numeric_limits<uint16_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<int16_t>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R16;
      dataType = GL_SHORT;
      scale = (float)std::numeric_limits<int16_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<uint32_t>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R;
      dataType = GL_UNSIGNED_INT;
      scale = (float)std::numeric_limits<uint32_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<int32_t>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R;
      dataType = GL_INT;
      scale = (float)std::numeric_limits<int32_t>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<uint64_t>*>(volume)) {
      std::string message = "VolumeRAM_UInt64 not supported as OpenGL volume.";
      LERROR(message);
      throw tgt::Exception(message);
    }
    else if (dynamic_cast<const VolumeAtomic<int64_t>*>(volume)) {
      std::string message = "VolumeRAM_Int64 not supported as OpenGL volume.";
      LERROR(message);
      throw tgt::Exception(message);
    }
    else if (dynamic_cast<const VolumeAtomic<float>*>(volume)) {
      format = GL_RED;
      internalFormat = GL_R32F;
      dataType = GL_FLOAT;
      scale = (float)std::numeric_limits<float>::max();
    }
    else if (dynamic_cast<const VolumeAtomic<double>*>(volume)) {
      std::string message = "VolumeRAM_Double not supported as OpenGL volume.";
      LERROR(message);
      throw tgt::Exception(message);
    }
    else {
      LERROR("unknown or unsupported volume type");
    }


    //
    // Create texture
    //
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    VolumeTexture* vTex = 0;
    char* tempVolumeData = 0;
    if (volume->getDimensions().z > 1) { // multi-slice volume => just create OGL texture from it
      vTex = new VolumeTexture(static_cast<const GLubyte*>(volume->getData()),
        volume->getDimensions(),
        format, internalFormat, dataType, Texture::LINEAR);
    }
    else { // single-slice volume (not allowed as OGL texture) => double slice
      LWARNING("OpenGL does not allow 3D textures consisting of only one slice: cloning slice");
      try {
        tempVolumeData = new char[2 * volume->getNumBytes()];
        memcpy(tempVolumeData, volume->getData(), volume->getNumBytes());
        memcpy(tempVolumeData + volume->getNumBytes(), volume->getData(), volume->getNumBytes());

        vTex = new VolumeTexture(reinterpret_cast<const GLubyte*>(tempVolumeData),
          glm::ivec3(volume->getDimensions().x, volume->getDimensions().y, volume->getDimensions().z * 2),
          format, internalFormat, dataType, Texture::LINEAR);
      }
      catch (std::bad_alloc&) {
        LERROR("bad allocation while creating OpenGL texture");
        throw tgt::Exception("bad allocation while creating OpenGL texture");
      }
    }
    assert(vTex);
    LGL_ERROR;

    vTex->bind();

    // map signed integer types from [-MIN_INT:MAX_INT] to [0.0:1.0] in order to avoid clamping of negative values
    if (volume->isInteger() && volume->isSigned()) {
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glPixelTransferf(GL_RED_SCALE, 0.5f);
      glPixelTransferf(GL_GREEN_SCALE, 0.5f);
      glPixelTransferf(GL_BLUE_SCALE, 0.5f);
      glPixelTransferf(GL_ALPHA_SCALE, 0.5f);

      glPixelTransferf(GL_RED_BIAS, 0.5f);
      glPixelTransferf(GL_GREEN_BIAS, 0.5f);
      glPixelTransferf(GL_BLUE_BIAS, 0.5f);
      glPixelTransferf(GL_ALPHA_BIAS, 0.5f);

      pixelTransferMapping_ = ValueMapping(1/scale * 0.5f, 0.5f, "");
    }
    else
      pixelTransferMapping_ = ValueMapping(1 / scale, 0.f, "");

    if (volume->getData())
      vTex->uploadTexture();

    // reset pixel transfer
    if (volume->isInteger() && volume->isSigned()) {
      glPopAttrib();
    }

    // set texture wrap to clamp
    vTex->setWrapping(Texture::CLAMP);

    LGL_ERROR;

    // prevent deleting twice
    vTex->setPixelData(0);

    delete[] tempVolumeData;
    tempVolumeData = 0;

    // append to internal data structure
    texture_ = vTex;

    LGL_ERROR;
  }

  size_t VolumeGL::getNumChannels() const {
    switch (getTexture()->getFormat()) {
    case GL_ALPHA: return 1;
    case GL_LUMINANCE_ALPHA: return 2;
    case GL_RGB: return 3;
    case GL_RGBA: return 4;
    default:
      LERROR("Unsupported volume format!");
      assert(false);
      return 0;
    }
  }

  size_t VolumeGL::getBytesPerVoxel() const {
    int bytesPerChannel = 0;
    switch (getTexture()->getDataType()) {
    case GL_UNSIGNED_BYTE:
    case GL_BYTE:
      bytesPerChannel = 1;
      break;
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
      bytesPerChannel = 2;
      break;
    case GL_UNSIGNED_INT:
    case GL_INT:
      bytesPerChannel = 4;
      break;
    case GL_FLOAT:
      bytesPerChannel = 4;
      break;
    default:
      LERROR("Unsupported data type!");
    }
    return bytesPerChannel * getNumChannels();
  }

  ValueMapping VolumeGL::getPixelTransferMapping() const {
    return pixelTransferMapping_;
  }

} // end namespace tgt