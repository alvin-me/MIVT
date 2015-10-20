#include "valuemapping.h"

namespace tgt {


  ValueMapping::ValueMapping() : scale_(1.0f), offset_(0.0f), unit_("") {
  }

  ValueMapping::ValueMapping(float scale, float offset, std::string unit
    ) : scale_(scale), offset_(offset), unit_(unit) {
  }

  float ValueMapping::map(float normalized) const {
    return (normalized * scale_) + offset_;
  }

  float ValueMapping::unmap(float realWorld) const {
    return (realWorld - offset_) / scale_;
  }

  std::string ValueMapping::getUnit() const {
    return unit_;
  }

  void ValueMapping::setUnit(std::string unit) {
    unit_ = unit;
  }

  float ValueMapping::getScale() const {
    return scale_;
  }

  void ValueMapping::setScale(float scale) {
    scale_ = scale;
  }

  float ValueMapping::getOffset() const {
    return offset_;
  }

  void ValueMapping::setOffset(float offset) {
    offset_ = offset;
  }

  ValueMapping ValueMapping::getInverseMapping() const {
    return ValueMapping(1.f / scale_, -offset_ / scale_, "1/" + unit_);
  }

} // end namespace tgt