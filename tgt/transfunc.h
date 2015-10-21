#pragma once

#include "texture.h"
#include "serializable.h"

namespace tgt {

  class Shader;

  /**
  * Base class for transfer functions.
  *
  * The lookup table can be defined by passing pixel data that is directly
  * assigned to the transfer function's texture.
  */
  class TransFunc : public Serializable {
  public:
    /**
    * Constructor.
    *
    * @param width width of transfer function
    * @param height of transfer function. Pass one for a 1D transfer function.
    * @param depth of transfer function. Pass one for a 1D or 2D transfer function.
    * @param format transfer function texture's format (see Texture)
    * @param dataType transfer function texture's data type (see Texture)
    * @param filter transfer function texture's filtering mode (see Texture)
    */
    TGT_API TransFunc(int width = 256, int height = 1, int depth = 1,
      GLint format = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE, Texture::Filter filter = Texture::NEAREST);

    TGT_API virtual ~TransFunc();

    /**
    * Marks the texture of the transfer function as invalid,
    * so it will be updated on next access.
    */
    TGT_API virtual void invalidateTexture();

    /**
    * Returns whether the texture will be updated on next access.
    */
    TGT_API bool isTextureInvalid() const;

    TGT_API virtual void setUniform(Shader* shader, const std::string& uniform, const std::string& uniformTex, const GLint texUnit);

    /**
    * Binds the transfer function texture. If it has been marked invalid,
    * updateTexture() is called before.
    *
    * @note If the texture is not present when calling this function, it is created.
    *    Therefore, the caller has to make sure that a valid OpenGL context is active.
    */
    TGT_API void bind();

    /**
    * Updates the texture of the transfer function or creates it, if it is not present.
    * The base class implementation uploads the texture data to the GPU.
    *
    * @note This function is called automatically on each texture access, when the texture is marked as invalid.
    *    You might call it directly in order to force an immediate update. But then you have to make sure that
    *    an valid OpenGL context is active.
    *
    */
    TGT_API virtual void updateTexture();

    /**
    * Returns the texture of the transfer function. If it has been marked invalid,
    * updateTexture() is called before.
    *
    * @note If the texture is not present when calling this function, it is created.
    *    Therefore, the caller has to make sure that a valid OpenGL context is active.
    */
    TGT_API Texture* getTexture();

    /**
    * Sets the dimensions of the transfer function to the given values.
    * The texture is not updated at this point, but mark as invalid so it will be updated on next access.
    *
    * @param width width of the transfer function.
    * @param height height of the transfer function. Set to one for 1D transfer functions.
    * @param depth height of the transfer function. Set to one for 1D or 2D transfer functions.
    */
    TGT_API virtual void resize(int width, int height = 1, int depth = 1);

    /**
    * Returns the dimensions of the transfer function's texture.
    */
    TGT_API glm::ivec3 getDimensions() const;

    //TODO: make pure virtual:
    TGT_API virtual int getNumDimensions() const;

    /**
    * Use this function to define the transfer function's look up table.
    * The passed pixel data is assigned to the transfer function's texture.
    *
    * @param data the pixel data to assign. Must match the transfer function
    *      in dimension, format and data type. Use reinterpret_cast in order
    *      to pass data of other type than GLubyte.
    *
    * @warning The transfer function object takes ownership of the passed memory chunk.
    *      Therefore, the caller must not delete it!
    */
    TGT_API virtual void setPixelData(GLubyte* data);

    /**
    * Returns the transfer function texture's pixel data.
    */
    TGT_API virtual GLubyte* getPixelData();

    /**
    * Resets the transfer function to its default value.
    *
    * The default implementation clears the transfer function texture,
    * but the behavior of concrete sub-classes may differ.
    */
    TGT_API virtual void reset();

    /**
    * Returns the transfer function's domain, i.e., the intensity range it covers,
    * for the specified dimension.
    *
    * @note This function should be overridden by concrete subclasses.
    *  The default implementation returns [0.0, 255.0].
    */
    TGT_API virtual glm::vec2 getDataDomain(int dimension = 0) const;

    /**
    * Sets the transfer function's domain, i.e., the intensity range it covers,
    * for the specified dimension.
    *
    * @note This function should be overridden by concrete subclasses.
    *  The default implementation is empty.
    */
    TGT_API virtual void setDataDomain(glm::vec2 domain, int dimension = 0);

    TGT_API virtual glm::vec2 getWindowingDomain() const;
    TGT_API virtual void setWindowingDomain(glm::vec2 domain);

    TGT_API void setGammaValue(float gamma);
    TGT_API float getGammaValue() const;

    /**
    * Creates a transfer function out of the data contained in the file given by filename.
    *
    * @param filename The path to the file in which the data is stored
    * @return true when the load was successful, false otherwise
    */
    TGT_API virtual bool load(const std::string& filename);

    /**
    * Saves the transfer function to a file. Any data in the file will be overwritten.
    * The supported extensions include:
    *
    * @param filename the name of the file the transfer function will be saved to
    * @return true, if the operation was successfull, false otherwise
    */
    TGT_API virtual bool save(const std::string& filename) const;

    TGT_API virtual void serialize(XmlSerializer& s) const;

    TGT_API virtual void deserialize(XmlDeserializer& s);

  protected:

    /**
    * Generates the transfer function texture according to the specified parameters.
    */
    virtual void createTex();

    /**
    * Deletes the transfer function texture.
    */
    virtual void deleteTexture();

    /**
    * Adapts the given width and height of transfer function to graphics board capabilities.
    * The result is stored in the reference parameters.
    *
    * @param width desired width for the transfer function
    * @param height desired height for the transfer function
    * @param depth desired depth for the transfer function
    */
    void fitDimensions(int& width, int& height, int& depth) const;

  protected:
    Texture* tex_;                ///< the texture of the transfer function, is generated internally
    Texture::Filter filter_;      ///< filtering mode of the transfer function texture.
    glm::ivec3 dimensions_;       ///< dimensions of the transfer function texture
    GLint format_;                ///< format of the transfer function texture
    GLenum dataType_;             ///< data type of the transfer function texture
    float gammaValue_;            ///< value for gamma correction

    bool textureInvalid_;         ///< indicates whether the transfer function texture has to be updated

    static const std::string loggerCat_; ///< logger category
  };

} // end namespace tgt