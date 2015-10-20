#include "volumeminmax.h"
#include "volume.h"
#include "volumeram.h"
#include "valuemapping.h"

namespace tgt {

  VolumeMinMax::VolumeMinMax()
    : VolumeDerivedData()
  {}

  VolumeMinMax::VolumeMinMax(float min, float max, float minHu, float maxHu)
    : VolumeDerivedData()
    , minValue_(min), maxValue_(max), minHuValue_(minHu), maxHuValue_(maxHu)
  {}

  VolumeDerivedData* VolumeMinMax::createFrom(Volume* handle) const {
    assert(handle);

    float min = 0.f;
    float max = 1.f;
    const VolumeRAM* v = handle->getRepresentation<VolumeRAM>();
    assert(v);

    min = v->minValue();
    max = v->maxValue();

    assert(min <= max);

    float minHu = handle->getRescaleMapping().map(min);
    float maxHu = handle->getRescaleMapping().map(max);

    return new VolumeMinMax(min, max, minHu, maxHu);
  }

  float VolumeMinMax::getMin() const {
    return minValue_;
  }

  float VolumeMinMax::getMax() const {
    return maxValue_;
  }

  float VolumeMinMax::getMinHu() const {
    return minHuValue_;
  }

  float VolumeMinMax::getMaxHu() const {
    return maxHuValue_;
  }


} // end namespace tgt