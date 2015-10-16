#pragma once

#include "tgt_math.h"
#include "config.h"

namespace tgt {

  /**
  *   Axis-aligned bounding box
  */
  class BoundingBox {
    unsigned int points_;   //points added to the box
    glm::vec3 llf_; //lower left front
    glm::vec3 urb_; //upper right back

  public:
    /**
    *   Constructs an undefined boundingbox
    */
    TGT_API BoundingBox() { points_ = 0; }

    /**
    *   Constructs an undefined boundingbox containing v
    */
    TGT_API BoundingBox(const glm::vec3& v)
      : points_(1),
      llf_(v),
      urb_(v)
    {}

    /**
    *   Constructs a bounding box containing v1 and v2
    */
    TGT_API BoundingBox(const glm::vec3& v1, const glm::vec3& v2)
      : points_(1),
      llf_(v1),
      urb_(v1) {

      addPoint(v2);
    }

    /**
    *   Enlarges the box (if necessary) to contain v
    */
    TGT_API void addPoint(const glm::vec3& v);
    TGT_API void addPoint(const glm::vec4& v);

    /**
    *   Enlarges the box (if necessary) to contain b
    */
    TGT_API void addVolume(const BoundingBox& b) {
      addPoint(b.llf_);
      addPoint(b.urb_);
    }

    /**
    *   Returns the Lower Left Front point
    */
    TGT_API glm::vec3 getLLF() const { return llf_; }

    /**
    *   Returns the Upper Right Back point
    */
    TGT_API glm::vec3 getURB() const { return urb_; }

    /**
    *   Returns the center of this box
    */
    TGT_API glm::vec3 center() const {
      return (diagonal() * 0.5f + llf_);
    }

    /**
    *   Returns the diagonal from llf to urb
    */
    TGT_API glm::vec3 diagonal() const {
      return (urb_ - llf_);
    }

    /**
    *   Returns the volume of this box
    */
    TGT_API float volume() const {
      return std::abs((llf_.x - urb_.x) * (llf_.y - urb_.y) * (llf_.z - urb_.z));
    }

    /**
    *   Returns true if box is defined (not only a point)
    */
    TGT_API bool isDefined() const { return (points_ == 2); }

    /**
    *   Returns true if box is only a point
    */
    TGT_API bool onlyPoint() const { return (points_ == 1); }

    /**
    *   Returns true if point is contained in this box
    */
    TGT_API bool containsPoint(const glm::vec3& p) {
      return ((p.x >= llf_.x) && (p.y >= llf_.y) && (p.z >= llf_.z)
        && (p.x <= urb_.x) && (p.y <= urb_.y) && (p.z <= urb_.z));
    }

    /**
    *   Returns true if b is contained in this box
    *   Box has to be defined!
    */
    TGT_API bool containsVolume(const BoundingBox& b) {
      return (containsPoint(b.llf_) && containsPoint(b.urb_));
    }

    /**
    *   Returns true if the boxes intersect
    *   Box has to be defined!
    */
    TGT_API bool intersects(const BoundingBox& b) const {
      // Look for a separating axis on each box for each axis
      if ((llf_.x > b.urb_.x) || (b.llf_.x > urb_.x)) return false;
      if ((llf_.y > b.urb_.y) || (b.llf_.y > urb_.y)) return false;
      if ((llf_.z > b.urb_.z) || (b.llf_.z > urb_.z)) return false;

      // No separating axis ... they must intersect
      return true;
    }

    TGT_API bool insideXZ(const BoundingBox& bounds) const;
    TGT_API bool insideXZ(const glm::vec3& v) const;

    ///Returns true if bounds is inside this.
    TGT_API bool inside(const BoundingBox& bounds) const;
    ///Returns true if v is inside this.
    TGT_API bool inside(const glm::vec3& v) const;

    /**
    * Transform this BB using m.
    *
    * @return A BoundingBox containing the 8 vertices defined by this BB, transformed using m.
    */
    TGT_API BoundingBox transform(const glm::mat4& m) const;
  };

} // end namespace tgt

/// ostream-operator
TGT_API std::ostream& operator<< (std::ostream& o, const tgt::BoundingBox& b);