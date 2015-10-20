#pragma once
#include "volumeram.h"

#pragma warning(disable:4290)

namespace tgt {

  template<class T>
  class VolumeAtomic : public VolumeRAM {

  public:
    typedef T VoxelType;

    enum {
      BYTES_PER_VOXEL = sizeof(T),
      BITS_PER_VOXEL = BYTES_PER_VOXEL * 8
    };

    /**
    * While using this constructor the class will automatically allocate
    * an appropriate chunk of memory. This memory will be deleted by this class.
    * If allocMem is false, no memory will be allocated.
    */
    VolumeAtomic(const glm::ivec3& dimensions, bool allocMem = true) throw (std::bad_alloc);

    /**
    * While using this constructor the class will use an preallocated chunk
    * of memory given in \p data. This memory will be deleted by this class.
    */
    VolumeAtomic(T* data, const glm::ivec3& dimensions);

    /// Deletes the \a data_ array
    virtual ~VolumeAtomic();

    virtual std::string getFormat() const;

    virtual size_t getBitsAllocated() const;

    virtual size_t getBytesPerVoxel() const;

    virtual bool isSigned() const;

    virtual bool isInteger() const;

    virtual void swapEndianness();

    virtual glm::vec2 elementRange() const;

    virtual size_t getNumBytes() const;

    virtual float minValue() const;

    virtual float maxValue() const;

    /**
    * Returns the minimum data value that is actually contained by the volume.
    *
    * @note Calculated min/max values are cached. Call invalidate() in order to
    *      enforce a recalculation.
    */
    T min() const;

    /**
    * Returns the maximal data value that is actually contained by the volume.
    *
    * @note Calculated min/max values are cached. Call invalidate() in order to
    *      enforce a recalculation.
    */
    T max() const;

    virtual void clear();
    virtual const void* getData() const;
    virtual void* getData();

    virtual float getVoxelNormalized(const glm::ivec3& pos) const;
    virtual float getVoxel(const glm::ivec3& pos) const;

    /**
    * Invalidates cached values (e.g. min/max), should be called when the volume was modified.
    */
    virtual void invalidate() const;

  protected:
    // protected default constructor
    VolumeAtomic() {}

    inline static size_t calcPos(const glm::ivec3& dimensions, size_t x, size_t y, size_t z);

    inline static size_t calcPos(const glm::ivec3& dimensions, const glm::ivec3& pos);

    T* data_;

    glm::vec2 elementRange_;

    mutable T maxValue_;
    mutable T minValue_;
    mutable bool minMaxValid_;
  };

  //------------------------------------------------------------------------------

  typedef VolumeAtomic<uint8_t>   VolumeRAM_UInt8;
  typedef VolumeAtomic<uint16_t>  VolumeRAM_UInt16;
  typedef VolumeAtomic<uint32_t>  VolumeRAM_UInt32;
  typedef VolumeAtomic<uint64_t>  VolumeRAM_UInt64;

  typedef VolumeAtomic<int8_t>    VolumeRAM_Int8;
  typedef VolumeAtomic<int16_t>   VolumeRAM_Int16;
  typedef VolumeAtomic<int32_t>   VolumeRAM_Int32;
  typedef VolumeAtomic<int64_t>   VolumeRAM_Int64;

  typedef VolumeAtomic<float>     VolumeRAM_Float;
  typedef VolumeAtomic<double>    VolumeRAM_Double;
  
} // end namespace tgt
