#pragma once

#include "volumederiveddata.h"

namespace tgt {

  class VolumeMinMax : public VolumeDerivedData {
  public:
    /// Empty default constructor required by VolumeDerivedData interface.
    TGT_API VolumeMinMax();
    TGT_API VolumeMinMax(float min, float max, float minHu, float maxHu);

    TGT_API virtual VolumeDerivedData* createFrom(Volume* handle) const;

    /// Minimum (raw data)
    TGT_API float getMin() const;

    /// Maximum (raw data)
    TGT_API float getMax() const;

    /// Minimum (hu)
    TGT_API float getMinHu() const;

    /// Maximum (hu)
    TGT_API float getMaxHu() const;

  protected:
    float minValue_;
    float maxValue_;
    float minHuValue_;
    float maxHuValue_;
  };

} // end namespace tgt