#include "volumehistogram.h"
#include "volume.h"
#include "volumeminmax.h"
#include "volumeram.h"

namespace tgt {

  Histogram1D createHistogram1DFromVolume(Volume *handle, size_t bucketCount) {
    VolumeMinMax* volumeMinMax = handle->getDerivedData<VolumeMinMax>();
    float min = volumeMinMax->getMin();
    float max = volumeMinMax->getMax();

    return createHistogram1DFromVolume(handle, bucketCount, min, max);
  }

  Histogram1D createHistogram1DFromVolume(Volume *handle, size_t bucketCount, float realWorldMin, float realWorldMax) {
    assert(realWorldMin <= realWorldMax);

    Histogram1D h(realWorldMin, realWorldMax, (int)bucketCount);

    const VolumeRAM* volumeRam = handle->getRepresentation<VolumeRAM>();
    assert(volumeRam);

    // iterate over slices
    glm::ivec3 dims = handle->getDimensions();
    glm::ivec3 pos;
    for (pos.z = 0; pos.z < dims.z; ++pos.z) {
      for (pos.y = 0; pos.y < dims.y; ++pos.y) {
        for (pos.x = 0; pos.x < dims.x; ++pos.x) {
          float val = volumeRam->getVoxel(pos);
          h.addSample(val);
        }
      }
    }

    return h;
  }

  //------------------------------------------------------------------------------

  //Histogram2D createHistogram2DFromVolume(Volume* handle, int bucketCountIntensity, int bucketCountGradient) {
  //  const VolumeRAM* vol = handle->getRepresentation<VolumeRAM>();
  //  RealWorldMapping rwm = handle->getRealWorldMapping();
  //  glm::ivec3 dims = vol->getDimensions();
  //  glm::vec3 sp = handle->getSpacing();

  //  float min = handle->getDerivedData<VolumeMinMax>()->getMinNormalized();
  //  float max = handle->getDerivedData<VolumeMinMax>()->getMaxNormalized();
  //  min = rwm.normalizedToRealWorld(min);
  //  max = rwm.normalizedToRealWorld(max);

  //  float minGradLength = 0.0f; // always 0
  //  float maxGradLength = 0.0f;

  //  //TODO: improve performance
  //  glm::ivec3 pos;
  //  for (pos.z = 0; pos.z < dims.z; ++pos.z) {
  //    for (pos.y = 0; pos.y < dims.y; ++pos.y) {
  //      for (pos.x = 0; pos.x < dims.x; ++pos.x) {
  //        //vec3 grad = VolumeOperatorGradient::calcGradientCentralDifferences(vol, sp, pos);
  //        glm::vec3 grad = VolumeOperatorGradient::calcGradientSobel(vol, sp, pos);

  //        float nlength = glm::length(grad) * rwm.getScale();

  //        if (nlength > maxGradLength)
  //          maxGradLength = nlength;
  //      }
  //    }
  //  }

  //  Histogram2D h(min, max, bucketCountIntensity, minGradLength, maxGradLength, bucketCountGradient);
  //  for (pos.z = 0; pos.z < dims.z; ++pos.z) {
  //    for (pos.y = 0; pos.y < dims.y; ++pos.y) {
  //      for (pos.x = 0; pos.x < dims.x; ++pos.x) {
  //        //vec3 grad = VolumeOperatorGradient::calcGradientCentralDifferences(vol, sp, pos);
  //        glm::vec3 grad = VolumeOperatorGradient::calcGradientSobel(vol, sp, pos);

  //        float nlength = glm::length(grad) * rwm.getScale();

  //        float v = vol->getVoxelNormalized(pos);
  //        v = rwm.normalizedToRealWorld(v);

  //        h.addSample(v, nlength);
  //      }
  //    }
  //  }

  //  return h;
  //}

  //------------------------------------------------------------------------------
 
  VolumeHistogramIntensity::VolumeHistogramIntensity()
    : VolumeDerivedData()
    , histogram_()
  {}

  VolumeHistogramIntensity::VolumeHistogramIntensity(const Histogram1D& h) 
    : VolumeDerivedData()
    , histogram_(h)
  {}

  VolumeDerivedData* VolumeHistogramIntensity::createFrom(Volume* handle) const {
    assert(handle);
    VolumeHistogramIntensity* h = new VolumeHistogramIntensity();
    h->histogram_ = createHistogram1DFromVolume(handle, static_cast<size_t>(256));
    return h;
  }

  size_t VolumeHistogramIntensity::getBucketCount() const {
    return histogram_.getNumBuckets();
  }

  uint64_t VolumeHistogramIntensity::getValue(int bucket) const {
    return histogram_.getBucket(bucket);
  }

  uint64_t VolumeHistogramIntensity::getValue(size_t bucket) const {
    return getValue(static_cast<int>(bucket));
  }

  uint64_t VolumeHistogramIntensity::getValue(float i) const {
    size_t bucketCount = histogram_.getNumBuckets();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getValue(bucket);
  }

  float VolumeHistogramIntensity::getNormalized(int i) const {
    return histogram_.getBucketNormalized(i);
  }

  float VolumeHistogramIntensity::getNormalized(float i) const {
    size_t bucketCount = histogram_.getNumBuckets();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getNormalized(bucket);
  }

  float VolumeHistogramIntensity::getLogNormalized(int i) const {
    return histogram_.getBucketLogNormalized(i);
  }

  float VolumeHistogramIntensity::getLogNormalized(float i) const {
    size_t bucketCount = histogram_.getNumBuckets();
    float m = (bucketCount - 1.f);
    int bucket = static_cast<int>(floor(i * m));
    return getLogNormalized(bucket);
  }

  const Histogram1D& VolumeHistogramIntensity::getHistogram() const {
    return histogram_;
  }

  Histogram1D& VolumeHistogramIntensity::getHistogram() {
    return histogram_;
  }

} // end namespace tgt