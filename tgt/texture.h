#pragma once

#include "tgt_gl.h"
#include "tgt_math.h"

namespace tgt {

  /**
  * OpenGL Texture
  */
  class Texture {
  public:
    friend class TextureManager;
    friend class TextureReader;

    enum Filter {
      NEAREST,
      LINEAR,
      MIPMAP,
      ANISOTROPIC
    };

    enum Wrapping {
      REPEAT = GL_REPEAT,
      CLAMP = GL_CLAMP,
      CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
      CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
      MIRRORED_REPEAT = GL_MIRRORED_REPEAT
    };

    TGT_API Texture()
      : priority_(-1.f), pixels_(0), id_(0)
    {}

    //FIXME: these ctors are ambiguous due to the default params, you need to specify all
    //       arguments or the compile won't know which one you mean. joerg

    /**
    * Without data and internalformat argument, type_ is calculated by
    * dimensions and a new chunk of data will be allocated on the heap.
    */
    TGT_API Texture(const glm::ivec3& dimensions, GLint format = GL_RGBA,
      GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR);

    /**
    * Without data and with internalformat argument, type_ is calculated by
    * dimensions and a new chunk of data will be allocated on the heap.
    */
    TGT_API  Texture(const glm::ivec3& dimensions, GLint format, GLint internalformat,
      GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR);

    /**
    * With data and without internalformat argument, type_ is calculated by
    * dimensions and no new chunk of data will be allocated on the heap.
    */
    TGT_API Texture(GLubyte* data, const glm::ivec3& dimensions, GLint format = GL_RGBA,
      GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR);

    /**
    * With data and internalformat argument, type_ is calculated by
    * dimensions and no new chunk of data will be allocated on the heap.
    */
    TGT_API  Texture(GLubyte* data, const glm::ivec3& dimensions, GLint format, GLint internalformat,
      GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR);

    /**
    * The destructor deletes the Texture in OpenGL.
    * Handled by the texturemanager!
    */
    TGT_API virtual ~Texture()
    {
      if (id_)
        glDeleteTextures(1, &id_);

      if (pixels_)
        delete[] pixels_;
    }

    /// allocates an appropriate buffer for the texture
    TGT_API void alloc() const {
      arraySize_ = glm::hmul(dimensions_) * bpp_;
      pixels_ = new GLubyte[arraySize_];
    }

    /// destroys the buffer for the texture and sets arraySize_ to zero
    TGT_API void destroy() {
      arraySize_ = 0;
      if (pixels_)
        delete[] pixels_;
      pixels_ = 0;// so nothing really nasty can happen
    }

    /// calculates the bytes per pixel from format dataType and dataType
    TGT_API static int calcBpp(GLint format, GLenum dataType);

    /// calculates the bytes per pixel from the internal format
    TGT_API static int calcBpp(GLint internalformat);

    /// calculates the number of channels from the passed texture format
    TGT_API static int calcNumChannels(GLint format);

    ///calculates size on the GPU (using internalformat)
    TGT_API  size_t getSizeOnGPU() const;

    /**
    * calculates the type_ (GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D or GL_TEXTURE_RECTANGLE_ARB) from
    * dimensions_
    *
    * @param textureRectangle Determines, wether texture should be a texture rectangle
    *      GL_TEXTURE_RECTANGLE_ARB
    */
    TGT_API GLenum calcType(bool textureRectangle = false);

    /**
    * Bind the texture to the active texture unit and target.
    *
    * Note: This does not enable texturing (use enable()).
    */
    TGT_API void bind() const {
      glBindTexture(type_, id_);
    }

    /**
    * Enable texturing on the active texture unit.
    */
    TGT_API void enable() const
    {
      glEnable(type_);
    }

    /**
    * Disable texturing on the active texture unit
    */
    TGT_API void disable() const
    {
      glDisable(type_);
    }

    /**
    *   Return OpenGL texture ID
    */
    TGT_API GLuint getId() const { return id_; }

    /**
    *   Set OpenGL texture ID.
    */
    TGT_API void setId(GLuint id) { id_ = id; }

    /**
    *   Generate OpenGL texture ID
    *   @return The generated ID
    */
    TGT_API GLuint generateId() {
      id_ = 0;
      glGenTextures(1, &id_);
      return id_;
    }

    TGT_API std::string getName() const { return name_; }
    TGT_API void setName(const std::string& name) { name_ = name; }

    /**
    *   Returns type of the texture (GL_TEXTURE_1D...3D, GL_TEXTURE_RECTANGLE_ARB)
    */
    TGT_API GLenum getType() const { return type_; }

    TGT_API glm::ivec3 getDimensions() const { return dimensions_; }
    TGT_API int getWidth() const { return dimensions_.x; };
    TGT_API int getHeight() const { return dimensions_.y; }
    TGT_API int getDepth() const { return dimensions_.z; }
    TGT_API GLint getFormat() const { return format_; }
    TGT_API GLint getInternalFormat() const { return internalformat_; }
    TGT_API Filter getFilter() const { return filter_; }
    TGT_API GLenum getDataType() const { return dataType_; }
    TGT_API size_t getArraySize() const { return arraySize_; }
    TGT_API size_t getNumChannels() const { return calcNumChannels(format_); }

    TGT_API void setDimensions(glm::ivec3 dimensions) { dimensions_ = dimensions; }
    TGT_API void setBpp(GLubyte bpp) { bpp_ = bpp; }
    TGT_API void setFormat(GLint format) { format_ = format; }
    TGT_API void setInternalFormat(GLint internalformat) { internalformat_ = internalformat; }
    TGT_API void setType(GLenum type) { type_ = type; }
    TGT_API void setDataType(GLenum dataType) { dataType_ = dataType; }

    TGT_API GLubyte* getPixelData() { return pixels_; }
    TGT_API const GLubyte* getPixelData() const { return pixels_; }
    TGT_API void setPixelData(GLubyte* pixels) {
      pixels_ = pixels;
      arraySize_ = pixels ? glm::hmul(dimensions_) * bpp_ : 0;
    }

    /**
    *   Returns the Bytes Per Pixel used,
    *   usually 3 or 4 for 24/32 bits per pixel.
    */
    TGT_API GLubyte getBpp() const { return bpp_; }

    /**
    *   Set Priority of this texture in GL
    *   @param p Priority, [0...1]
    */
    TGT_API void setPriority(GLclampf p)
    {
      glPrioritizeTextures(1, &id_, &p);
    }

    /**
    *   Returns current Priority, -1 if not yet set
    */
    TGT_API GLclampf getPriority() const { return priority_; }

    /**
    *   Check if texture is in resident GL memory
    */
    TGT_API bool isResident() const
    {
      GLboolean resident;
      return glAreTexturesResident(1, &id_, &resident) == GL_TRUE;
    }

    /**
    *   Sets Filtering for Texture. Binds the texture.
    */
    TGT_API void setFilter(Filter filter);

    /**
    * Applies the stored filter once again. Binds the texture.
    */
    TGT_API void applyFilter();

    /// Set texture wrapping mode. Binds the texture.
    TGT_API void setWrapping(Wrapping w);

    /// Applies the textur wrapping mode once againg. Binds the texture.
    TGT_API void applyWrapping();

    TGT_API Wrapping getWrapping() const { return wrapping_; }

    /**
    * Upload Texture to graphics-card. Binds the texture.
    *
    * type_, format_, internalformat_, dimensions, dataType_ and the pixels_ pointer have to
    * be set before calling this method.
    */
    TGT_API void uploadTexture();

    /**
    * Download Texture from graphics-card. Binds the texture.
    *
    * type_, format_, dimensions, dataType_ and the pixels_ pointer have to be set before
    * calling this method! Pixels will be allocated if pixels_ is a nullpointer.
    */
    TGT_API void downloadTexture() const;

    /**
    * Download texture from the GPU to a newly allocated buffer, to which a
    * pointer is returned.  Binds the texture.
    *
    * type_, format_, dimensions, and dataType_ have to be set before
    * calling this method!
    */
    TGT_API GLubyte* downloadTextureToBuffer() const;

    /**
    * Download texture from the GPU to a preallocated buffer. Binds the texture.
    *
    * type_, format_, dimensions, and dataType_ have to be set before
    * calling this method!
    */
    TGT_API void downloadTextureToBuffer(GLubyte* pixels, size_t numBytesAllocated) const;

    /**
    * Download texture from the GPU to a newly allocated buffer with
    * the passed format/data type and the texture's dimensions.
    */
    TGT_API GLubyte* downloadTextureToBuffer(GLint format, GLenum dataType) const;

    /**
    * Returns, wether texture is a texture rectangle (GL_TEXTURE_RECTANGLE_ARB)
    */
    TGT_API bool isTextureRectangle() const;

    /*
    1D access, always possible
    */
    template <class T>
    inline T& texel(size_t i) {
      assert(sizeof(T) == bpp_);
      assert(i < size_t(glm::hmul(dimensions_)));
      return ((T*)pixels_)[i];
    }
    template <class T>
    inline const T& texel(size_t i) const {
      assert(sizeof(T) == bpp_);
      assert(i < size_t(glm::hmul(dimensions_)));
      return ((T*)pixels_)[i];
    }

    /*
    2D access, only possible when type_ == GL_TEXTURE_2D
    */
    template <class T>
    inline T& texel(size_t x, size_t y) {
      assert(sizeof(T) == bpp_);
      assert(dimensions_.x * y + x < size_t(glm::hmul(dimensions_)));
      assert(type_ == GL_TEXTURE_2D);
      return ((T*)pixels_)[dimensions_.x * y + x];
    }
    template <class T>
    inline const T& texel(size_t x, size_t y) const {
      assert(sizeof(T) == bpp_);
      assert(dimensions_.x * y + x < size_t(glm::hmul(dimensions_)));
      assert(type_ == GL_TEXTURE_2D); 
      return ((T*)pixels_)[dimensions_.x * y + x];
    }
    template <class T>
    inline T& texel(const glm::ivec2& pos) {
      assert(sizeof(T) == bpp_);
      assert(dimensions_.x * pos.y + pos.x < glm::hmul(dimensions_));
      assert(type_ == GL_TEXTURE_2D);
      return ((T*)pixels_)[dimensions_.x * pos.y + pos.x];
    }
    template <class T>
    inline const T& texel(const glm::ivec2& pos) const {
      assert(sizeof(T) == bpp_);
      assert(dimensions_.x * pos.y + pos.x < glm::hmul(dimensions_));
      assert(type_ == GL_TEXTURE_2D);
      return ((T*)pixels_)[dimensions_.x * pos.y + pos.x];
    }

    /*
    3D access, only possible when type_ == GL_TEXTURE_3D
    */
    template <class T>
    inline T& texel(size_t x, size_t y, size_t z) {
      assert(sizeof(T) == bpp_);
      assert(z*dimensions_.x*dimensions_.y + y*dimensions_.x + x < size_t(glm::hmul(dimensions_)));
      assert(type_ == GL_TEXTURE_3D);
      return ((T*)pixels_)[z*dimensions_.x*dimensions_.y + y*dimensions_.x + x];
    }
    template <class T>
    inline const T& texel(size_t x, size_t y, size_t z) const {
      assert(sizeof(T) == bpp_);
      assert(z*dimensions_.x*dimensions_.y + y*dimensions_.x + x < size_t(glm::hmul(dimensions_)));
      assert(type_ == GL_TEXTURE_3D);
      return ((T*)pixels_)[z*dimensions_.x*dimensions_.y + y*dimensions_.x + x];
    }
    template <class T>
    inline T& texel(const glm::ivec3& pos) {
      assert(sizeof(T) == bpp_);
      assert(pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x < glm::hmul(dimensions_));
      assert(type_ == GL_TEXTURE_3D);
      return ((T*)pixels_)[pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x];
    }
    template <class T>
    inline const T& texel(const glm::ivec3& pos) const {
      assert(sizeof(T) == bpp_);
      assert(pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x < glm::hmul(dimensions_));
      assert(type_ == GL_TEXTURE_3D);
      return ((T*)pixels_)[pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x];
    }

  protected:
    glm::ivec3 dimensions_;
    GLint format_;          ///< GL_RGB...
    GLint internalformat_;  ///< GL_RGB...
    GLenum dataType_;       ///< GL_UNSIGNED_BYTE
    Filter filter_;
    Wrapping wrapping_;

    GLclampf priority_;     ///< GL texture priority [0...1]
    mutable GLubyte* pixels_;       ///< (temporary) buffer for loading image
    mutable size_t arraySize_;
    GLuint id_;             ///< OpenGL texture id

    GLenum type_;           ///< 1D, 2D, 3D
    GLubyte bpp_;           ///< bytes per pixel

    std::string name_;      ///< optional, e.g. for storing texture file name

    static const std::string loggerCat_;

    // used internally in the constructors
    void init(bool allocData);
  };

} // end namespace tgt