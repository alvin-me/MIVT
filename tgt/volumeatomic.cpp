#include "volumeatomic.h"
#include "volumeelement.h"
#include "logmanager.h"

#include <algorithm>

namespace tgt {

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
        LERROR("Failed to allocate memory: bad allocation");
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

  template class VolumeAtomic<uint8_t>;
  template class VolumeAtomic<uint16_t>;
  template class VolumeAtomic<uint32_t>;
  template class VolumeAtomic<uint64_t>;
  template class VolumeAtomic<int8_t>;
  template class VolumeAtomic<int16_t>;
  template class VolumeAtomic<int32_t>;
  template class VolumeAtomic<int64_t>;
  template class VolumeAtomic<float>;
  template class VolumeAtomic<double>;

} // end namespace tgt