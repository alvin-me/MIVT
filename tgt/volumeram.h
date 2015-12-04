#pragma once

#include "volumerepresentation.h"

namespace tgt {

  /**
  * OpenGL-independent base class for volumetric data sets.
  *
  * This class stores the raw data as well as the
  * required meta information about it. It does,
  * however, neither perform any OpenGL operations nor
  * does it hold any OpenGL-related properties.
  */
  class VolumeRAM : public VolumeRepresentation {
  public:
    TGT_API VolumeRAM(const glm::ivec3& dimensions);

    TGT_API virtual ~VolumeRAM() {}

    /// Returns the format of the volume as string (e.g., "uint8" or "float").
    TGT_API virtual std::string getFormat() const = 0;

    /// Returns the number of bytes held in the \a data_ array.
    TGT_API virtual size_t getNumBytes() const = 0;

    /// Returns the number of bits that are allocated by each voxel.
    /// For technical reasons, it may exceed the volume's bit depth.
    TGT_API virtual size_t getBitsAllocated() const = 0;

    /// Returns the number of bytes that are allocated for each voxel.
    TGT_API virtual size_t getBytesPerVoxel() const = 0;

    /// Returns whether the volume's data type is a signed type.
    TGT_API virtual bool isSigned() const = 0;

    /// Returns whether the volume's data type is an integer type.
    TGT_API virtual bool isInteger() const = 0;

    /// Volume operator swap endianness.
    TGT_API virtual void swapEndianness() = 0;

    /**
    * Returns the data set's minimal and maximal possible element values
    * according to its data type converted to float.
    */
    TGT_API virtual glm::vec2 elementRange() const = 0;

    /// Set all volume data to zero
    TGT_API virtual void clear() = 0;

    /// Gets a void* to the data stored with this Volume
    TGT_API virtual const void* getData() const = 0;
    TGT_API virtual void* getData() = 0;

    TGT_API virtual float minValue() const = 0;
    TGT_API virtual float maxValue() const = 0;

    TGT_API virtual float getVoxelNormalized(const glm::ivec3& pos) const = 0;
    TGT_API virtual float getVoxel(const glm::ivec3& pos) const = 0;
    TGT_API virtual float getVoxelNormalizedLinear(const glm::vec3& pos) const;

  protected:
    // protected default constructor
    VolumeRAM() {}
  };

} // end namespace tgt