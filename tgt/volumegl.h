#pragma once

#include "volumerepresentation.h"
#include "volumeram.h"
#include "volumetexture.h"
#include "exception.h"
#include "valuemapping.h"

namespace tgt {

  /**
  * This class is the OpenGL interface for volume objects.
  * One or several 3D textures, which hold the complete data set, are created.
  */
  class VolumeGL : public VolumeRepresentation {
  public:

    /**
    * Creates the VolumeTexture instance.
    *
    * @note The volume must have a size greater one in all dimensions
    *
    * @throw VoreenException if the volume type is not supported by OpenGL
    * @throw std::bad_alloc
    */
    TGT_API VolumeGL(const VolumeRAM* volume)
      throw (tgt::Exception, std::bad_alloc);

    TGT_API virtual ~VolumeGL();

    /**
    * Returns a const pointer to the texture with index \p i.
    * The VolumeTexture pointers \e DO \e NOT have any special order. If you
    * want to sort them by distance use \a getSortedTextures
    *
    * @param i The index of the desired VolumeTexture
    * @return The desired VolumeTexture
    */
    TGT_API const VolumeTexture* getTexture() const;

    /// @overload
    TGT_API VolumeTexture* getTexture();

    TGT_API virtual size_t getNumChannels() const;
    TGT_API virtual size_t getBytesPerVoxel() const;

    /**
    * Returns the real-world mapping that has been applied during texture upload
    * (GL_*_SCALE, GL_*_BIAS).
    *
    * Signed integer types are mapped from [-1.0:1.0] to [0.0:1.0] in order to avoid
    * clamping of negative values. For all other data types, no mapping is applied
    * (except the standard OpenGL normalization of integer types).
    */
    TGT_API virtual ValueMapping getPixelTransferMapping() const;

  protected:
    /**
    * Determines the volume type and creates the according OpenGL texture.
    * Internally called by the constructor.
    *
    * @throw VoreenException if the volume type is not supported by OpenGL
    * @throw std::bad_alloc
    */
    void generateTexture(const VolumeRAM* vol)
      throw (tgt::Exception, std::bad_alloc);

    VolumeTexture* texture_;

    /// scale factor and offset used during texture upload (GL_*_SCALE, GL_*_BIAS)
    ValueMapping pixelTransferMapping_;

    static const std::string loggerCat_;

  private:
    /// Used internally for destruction of the data.
    void destroy();
  };

} // end namespace tgt