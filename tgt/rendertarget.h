#pragma once

#include "exception.h"
#include "tgt_math.h"
#include "framebufferobject.h"
#include "textureunit.h"

namespace tgt {

  class Shader;

  class RenderTarget {
  public:

    TGT_API RenderTarget();
    TGT_API virtual ~RenderTarget();

    TGT_API void initialize(GLint internalColorFormat = GL_RGBA16, GLint internalDepthFormat = GL_DEPTH_COMPONENT24);
    TGT_API void deinitialize();

    TGT_API void activateTarget();
    TGT_API void deactivateTarget();
    TGT_API bool isActive() const;

    /**
    * Clears the contents of an activated RenderTarget,
    * by calling glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT).
    *
    * @note May only be called on when activated.
    */
    TGT_API void clearTarget();
    TGT_API void clear();

    /**
    * @brief Passes the assigned textures' TextureParameters struct to the passed shader.
    *  Needs to be called for each inport whose textures are to be accessed by the shader.
    *
    * @param uniform The name of the struct used in the shader.
    */
    TGT_API void setTextureParameters(Shader* shader, const std::string& uniform);

    TGT_API void bindColorTexture(GLint texUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));
    TGT_API void bindColorTexture(TextureUnit& texUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));
    TGT_API void bindColorTexture();

    TGT_API void bindDepthTexture(GLint texUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));
    TGT_API void bindDepthTexture(TextureUnit& texUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));
    TGT_API void bindDepthTexture();

    TGT_API void bindTextures(GLint colorUnit, GLint depthUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));
    TGT_API void bindTextures(TextureUnit& colorUnit, TextureUnit& depthUnit, GLint filterMode = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE, glm::vec4 borderColor = glm::vec4(0.f));

    TGT_API const Texture* getColorTexture() const { return colorTex_; }
    TGT_API const Texture* getDepthTexture() const { return depthTex_; }

    TGT_API Texture* getColorTexture() { return colorTex_; }
    TGT_API Texture* getDepthTexture() { return depthTex_; }

    //Get the color at position pos. This method activates the RenderTarget!
    TGT_API glm::vec4 getColorAtPos(glm::ivec2 pos);

    TGT_API glm::ivec2 getSize() const;
    TGT_API void resize(glm::ivec2 newsize);

    /**
    * \brief Returns the current content of the color buffer, converted
    * to RGBA format. The data type is specified as template parameter.
    *
    * @tparam T Specifies the data type of the returned image data.
    *         Supported types: uint8_t, uint16_t, float
    *
    * @note Releasing the allocated memory is up to the caller!
    *
    * @throw Exception if the color buffer content could not be read
    *        or converted
    */
    template<typename T>
    T* readColorBuffer() throw (Exception);

    template<typename T>
    void readColorBuffer(T* buffer, size_t numBytesAllocated) throw (Exception);

    TGT_API void readColorBuffer(uint8_t* buffer, size_t numBytesAllocated) throw (Exception);

    /**
    * Writes the currently stored rendering to an image file.
    *
    * @note This function requires Voreen to be built with DevIL support.
    *
    * @param filename the filename of the output file. Must have an
    *      extension known by the DevIL library. *.jpg and *.png
    *      should work fine.
    *
    * @throw VoreenException if the image saving failed
    */
    TGT_API void saveToImage(const std::string &filename) throw (Exception);

  protected:
    FramebufferObject* fbo_;

    Texture* colorTex_;
    Texture* depthTex_;

    bool cleared_;    //< false, if the render buffer is empty

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  template<typename T>
  T* RenderTarget::readColorBuffer() throw (Exception) {
    if (!getColorTexture()) {
      throw Exception("RenderTarget::readColorBuffer() called on an empty render target");
    }

    // determine OpenGL data type from template parameter
    GLenum dataType;
    if (typeid(T) == typeid(uint8_t))
      dataType = GL_UNSIGNED_BYTE;
    else if (typeid(T) == typeid(uint16_t))
      dataType = GL_UNSIGNED_SHORT;
    else if (typeid(T) == typeid(float))
      dataType = GL_FLOAT;
    else
      throw Exception("RenderTarget::readColorBuffer(): unsupported data type. "
      "Expected: uint8_t, uint16_t, float");

    GLubyte* pixels = 0;
    try {
      pixels = getColorTexture()->downloadTextureToBuffer(GL_BGRA, dataType);
    }
    catch (std::bad_alloc&) {
      throw Exception("RenderTarget::readColorBuffer(): bad allocation");
    }
    LGL_ERROR;

    if (pixels)
      return reinterpret_cast<T*>(pixels);
    else
      throw Exception("RenderTarget::readColorBuffer(): failed to download texture");
  }

  template<typename T>
  void RenderTarget::readColorBuffer(T* pixels, size_t numBytesAllocated) throw (Exception) {
    if (!getColorTexture()) {
      throw Exception("RenderTarget::readColorBuffer() called on an empty render target");
    }

    // determine OpenGL data type from template parameter
    GLenum dataType;
    if (typeid(T) == typeid(uint8_t))
      dataType = GL_UNSIGNED_BYTE;
    else if (typeid(T) == typeid(uint16_t))
      dataType = GL_UNSIGNED_SHORT;
    else if (typeid(T) == typeid(float))
      dataType = GL_FLOAT;
    else
      throw Exception("RenderTarget::readColorBuffer(): unsupported data type. "
      "Expected: uint8_t, uint16_t, float");

    try {
      getColorTexture()->downloadTextureToBuffer(GL_BGRA, dataType, pixels, numBytesAllocated);
      
//      this->activateTarget();
//      float *depth = new float[getSize().x * getSize().y];
//      glReadPixels(0, 0, getSize().x, getSize().y, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
//
//#pragma warning(disable:4996)
//
//      FILE* fp = fopen("D:/test.dat", "wb");
//      fwrite(depth, getSize().x * getSize().y * sizeof(float), 1, fp);
//      fclose(fp);
//
//      delete[] depth;
//      this->deactivateTarget();
    }
    catch (std::bad_alloc&) {
      throw Exception("RenderTarget::readColorBuffer(): bad allocation");
    }
    LGL_ERROR;
  }

} // end namespace tgt