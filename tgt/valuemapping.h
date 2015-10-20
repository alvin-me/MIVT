#pragma once

#include "config.h"
#include <string>

namespace tgt {

  class ValueMapping {
  public:
    TGT_API ValueMapping();
    TGT_API ValueMapping(float scale, float offset, std::string unit = "");

    /**
    * @brief Transforms value.
    * @return (value * scale) + offset
    */
    TGT_API float map(float value) const;
    /**
    * @brief Transforms value.
    * @return (value - offset) / scale
    */
    TGT_API float unmap(float value) const;

    /**
    * Returns the inverted mapping.
    */
    TGT_API ValueMapping getInverseMapping() const;

    TGT_API std::string getUnit() const;
    TGT_API void setUnit(std::string unit);

    TGT_API float getScale() const;
    TGT_API void setScale(float scale);
    
    TGT_API float getOffset() const;
    TGT_API void setOffset(float offset);

    /// Create a combined mapping, first applying m1, then m2 (using unit from m2).
    TGT_API static ValueMapping combine(const ValueMapping& m1, const ValueMapping& m2) {
      return ValueMapping(m1.getScale()*m2.getScale(), (m2.getScale()*m1.getOffset()) + m2.getOffset(), m2.getUnit());
    }

  private:
    float scale_;
    float offset_;

    std::string unit_;
  };

} // end namespace tgt
