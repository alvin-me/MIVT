#pragma once
#include "volumeram.h"
#include "volumeelement.h"
#include "logmanager.h"

#include <algorithm>

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

    static const std::string loggerCat_;
  };

  //-----------------------------------------------------------------------------

  template<class T>
  VolumeAtomic<T>::VolumeAtomic(const glm::ivec3& dimensions, bool allocMem)
    throw (std::bad_alloc)
    : VolumeRAM(dimensions)
    , data_(0)
    , elementRange_(static_cast<float>(VolumeElement<T>::rangeMinElement()),
    static_cast<float>(VolumeElement<T>::rangeMaxElement()))
    , minMaxValid_(false)
  {
    if (allocMem) {
      try {
        data_ = new T[numVoxels_];
      }
      catch (std::bad_alloc) {
        LERRORC("VolumeAtomic", "Failed to allocate memory: bad allocation");
        throw; // throw it to the caller
      }
    }
  }

  template<class T>
  VolumeAtomic<T>::VolumeAtomic(T* data,
    const glm::ivec3& dimensions)
    : VolumeRAM(dimensions)
    , data_(data)
    , elementRange_(static_cast<float>(VolumeElement<T>::rangeMinElement()),
    static_cast<float>(VolumeElement<T>::rangeMaxElement()))
    , minMaxValid_(false)
  {
  }

  template<class T>
  VolumeAtomic<T>::~VolumeAtomic() {
    delete[] data_;
  }

  template<class T>
  std::string VolumeAtomic<T>::getFormat() const {
    return VolumeElement<T>::getFormat();
  }

  template<class T>
  size_t VolumeAtomic<T>::getBitsAllocated() const {
    return BITS_PER_VOXEL;
  }

  template<class T>
  size_t VolumeAtomic<T>::getBytesPerVoxel() const {
    return BYTES_PER_VOXEL;
  }

  template<class T>
  size_t VolumeAtomic<T>::getNumBytes() const {
    return sizeof(T)* numVoxels_;
  }

  template<class T>
  bool VolumeAtomic<T>::isSigned() const {
    return VolumeElement<T>::isSigned();
  }

  template<class T>
  bool VolumeAtomic<T>::isInteger() const {
    return VolumeElement<T>::isInteger();
  }

  template<class T>
  void VolumeAtomic<T>::swapEndianness() {
    T* v = data_;
    for (size_t i = 0; i < getNumVoxels(); ++i)
      v[i] = VolumeElement<T>::swapEndianness(v[i]);
  }

  template<class T>
  glm::vec2 VolumeAtomic<T>::elementRange() const {
    return elementRange_;
  }

  template<class T>
  inline size_t VolumeAtomic<T>::calcPos(const glm::ivec3& dimensions, size_t x, size_t y, size_t z) {
    return z*dimensions.x*dimensions.y + y*dimensions.x + x;
  }

  template<class T>
  inline size_t VolumeAtomic<T>::calcPos(const glm::ivec3& dimensions, const glm::ivec3& pos) {
    return pos.z*dimensions.x*dimensions.y + pos.y*dimensions.x + pos.x;
  }

  template<class T>
  float VolumeAtomic<T>::minValue() const {
    return static_cast<float>(min());
  }

  template<class T>
  float VolumeAtomic<T>::maxValue() const {
    return static_cast<float>(max());
  }

  template<class T>
  T VolumeAtomic<T>::min() const {
    if (!minMaxValid_) {
      minValue_ = *std::min_element(data_, data_ + getNumVoxels());
      maxValue_ = *std::max_element(data_, data_ + getNumVoxels());
      minMaxValid_ = true;
    }
    return minValue_;
  }

  template<class T>
  T VolumeAtomic<T>::max() const {
    if (!minMaxValid_) {
      minValue_ = *std::min_element(data_, data_ + getNumVoxels());
      maxValue_ = *std::max_element(data_, data_ + getNumVoxels());
      minMaxValid_ = true;
    }
    return maxValue_;
  }

  template<class T>
  void VolumeAtomic<T>::clear() {
    memset(data_, 0, getNumBytes());
    invalidate();
  }

  template<class T>
  void VolumeAtomic<T>::invalidate() const {
    minMaxValid_ = false;
  }

  template<class T>
  const void* VolumeAtomic<T>::getData() const {
    return reinterpret_cast<const void*>(data_);
  }

  template<class T>
  void* VolumeAtomic<T>::getData() {
    return reinterpret_cast<void*>(data_);
  }

  template<class T>
  float VolumeAtomic<T>::getVoxelNormalized(const glm::ivec3& pos) const {
    size_t index = calcPos(dimensions_, pos);
    assert(index < numVoxels_);
    return VolumeElement<T>::getTypeAsFloat(data_[index]);
  }

  template<class T>
  float VolumeAtomic<T>::getVoxel(const glm::ivec3& pos) const {
    size_t index = calcPos(dimensions_, pos);
    assert(index < numVoxels_);
    return static_cast<float>(data_[index]);
  }

  //------------------------------------------------------------------------------

  //template class TGT_API VolumeAtomic<uint8_t>;
  //template class TGT_API VolumeAtomic<uint16_t>;
  //template class TGT_API VolumeAtomic<uint32_t>;
  //template class TGT_API VolumeAtomic<uint64_t>;
  //template class TGT_API VolumeAtomic<int8_t>;
  //template class TGT_API VolumeAtomic<int16_t>;
  //template class TGT_API VolumeAtomic<int32_t>;
  //template class TGT_API VolumeAtomic<int64_t>;
  //template class TGT_API VolumeAtomic<float>;
  //template class TGT_API VolumeAtomic<double>;

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
