#include "boundingbox.h"

#include <algorithm>

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace tgt {

  void BoundingBox::addPoint(const vec3& v) {
    if (points_ < 2) {
      ++points_;
      if (points_ == 1) {
        llf_ = v; urb_ = v;
        return;
      }
    }

    llf_ = glm::min(llf_, v);
    urb_ = glm::max(urb_, v);
  }

  void BoundingBox::addPoint(const vec4& v) {
    addPoint(v.xyz());
  }

  bool BoundingBox::insideXZ(const BoundingBox& bounds) const {
    assert(isDefined());
    assert(bounds.isDefined());

    vec3 llfb = bounds.getLLF();
    vec3 urbb = bounds.getURB();

    float r0x = std::min(llf_[0], urb_[0]);
    float r1x = std::max(llf_[0], urb_[0]);
    float r0y = std::min(llf_[2], urb_[2]);
    float r1y = std::max(llf_[2], urb_[2]);
    float r2x = std::min(llfb[0], urbb[0]);
    float r3x = std::max(llfb[0], urbb[0]);
    float r2y = std::min(llfb[2], urbb[2]);
    float r3y = std::max(llfb[2], urbb[2]);

    return (r0x >= r2x) && (r0y >= r2y)
      && (r1x <= r3x) && (r1y <= r3y);
  }

  bool BoundingBox::insideXZ(const vec3& v) const {
    assert(isDefined());

    return (llf_[0] <= v[0]) && (v[0] <= urb_[0])
      && (llf_[2] <= v[2]) && (v[2] <= urb_[2]);
  }

  bool BoundingBox::inside(const BoundingBox& bounds) const {
    assert(isDefined());
    assert(bounds.isDefined());

    vec3 llfb = bounds.getLLF();
    vec3 urbb = bounds.getURB();

    float r0x = std::min(llf_[0], urb_[0]);
    float r1x = std::max(llf_[0], urb_[0]);
    float r0y = std::min(llf_[1], urb_[1]);
    float r1y = std::max(llf_[1], urb_[1]);
    float r0z = std::min(llf_[2], urb_[2]);
    float r1z = std::max(llf_[2], urb_[2]);

    float r2x = std::min(llfb[0], urbb[0]);
    float r3x = std::max(llfb[0], urbb[0]);
    float r2y = std::min(llfb[1], urbb[1]);
    float r3y = std::max(llfb[1], urbb[1]);
    float r2z = std::min(llfb[2], urbb[2]);
    float r3z = std::max(llfb[2], urbb[2]);

    return (r0x >= r2x) && (r1x <= r3x)
      && (r0y >= r2y) && (r1y <= r3y)
      && (r0z >= r2z) && (r1z <= r3z);
  }

  bool BoundingBox::inside(const vec3& v) const {
    assert(isDefined());

    return (llf_[0] <= v[0]) && (v[0] <= urb_[0])
      && (llf_[1] <= v[1]) && (v[1] <= urb_[1])
      && (llf_[2] <= v[2]) && (v[2] <= urb_[2]);
  }

  BoundingBox BoundingBox::transform(const mat4& m) const {
    BoundingBox b;
    b.addPoint(m * vec4(llf_.x, llf_.y, llf_.z, 1.0f));
    b.addPoint(m * vec4(urb_.x, llf_.y, llf_.z, 1.0f));
    b.addPoint(m * vec4(llf_.x, urb_.y, llf_.z, 1.0f));
    b.addPoint(m * vec4(llf_.x, llf_.y, urb_.z, 1.0f));
    b.addPoint(m * vec4(urb_.x, urb_.y, llf_.z, 1.0f));
    b.addPoint(m * vec4(llf_.x, urb_.y, urb_.z, 1.0f));
    b.addPoint(m * vec4(urb_.x, llf_.y, urb_.z, 1.0f));
    b.addPoint(m * vec4(urb_.x, urb_.y, urb_.z, 1.0f));
    return b;
  }

} // end namespace tgt

std::ostream& operator<< (std::ostream& o, const tgt::BoundingBox& b) {
  return (o << "(llf: " << glm::to_string(b.getLLF()) << " urb: " << glm::to_string(b.getURB()) << ")");
}