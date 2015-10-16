#include "frustum.h"
#include "camera.h"

using glm::vec3;

namespace tgt {

  Frustum::Frustum(float fovy, float ratio, float nearDist, float farDist)
    : nearDist_(nearDist),
    farDist_(farDist)
  {
    float halfheight = tanf(0.5f * glm::deg2rad(fovy)) * nearDist_;
    tnear_ = halfheight;
    bnear_ = -halfheight;
    setRatio(ratio);
  }

  Frustum::Frustum(float left, float right, float bottom, float top, float nearDist, float farDist)
    : lnear_(left),
    rnear_(right),
    bnear_(bottom),
    tnear_(top),
    nearDist_(nearDist),
    farDist_(farDist)
  {}

  bool Frustum::operator==(const Frustum& rhs) const {
    return (rhs.lnear_ == lnear_) && (rhs.rnear_ == rnear_) && (rhs.bnear_ == bnear_) && (rhs.tnear_ == tnear_) &&
      (rhs.nearDist_ == nearDist_) && (rhs.farDist_ == farDist_);
  }

  void Frustum::update(Camera* c) {
    campos_ = c->getPosition();

    normals_[FARN] = c->getLook();
    normals_[NEARN] = -1.f * normals_[FARN];

    vec3 cam2near = normals_[FARN] * nearDist_;

    nearp_ = campos_ + cam2near;
    farp_ = campos_ + normals_[FARN] * farDist_;

    vec3 strafe = c->getStrafe();
    vec3 tmp;

    tmp = cam2near + tnear_ * glm::cross(strafe, c->getLook());
    normals_[TOPN] = glm::normalize(glm::cross(strafe, tmp));

    tmp = cam2near + bnear_ * glm::cross(strafe, c->getLook());
    normals_[BOTTOMN] = glm::normalize(glm::cross(tmp, strafe));

    tmp = cam2near + lnear_ * strafe;
    normals_[LEFTN] = glm::normalize(glm::cross(glm::cross(strafe, c->getLook()), tmp));

    tmp = cam2near + rnear_ * strafe;
    normals_[RIGHTN] = glm::normalize(glm::cross(tmp, glm::cross(strafe, c->getLook())));
  }

  // Is this bounding-box culled?
  bool Frustum::isCulledXZ(const BoundingBox& bounds) const {
    vec3 urb = bounds.getURB();
    vec3 llf = bounds.getLLF();

    vec3 points[4];
    points[0] = vec3(llf.x, 0.f, llf.z);
    points[1] = vec3(urb.x, 0.f, llf.z);
    points[2] = vec3(urb.x, 0.f, urb.z);
    points[3] = vec3(llf.x, 0.f, urb.z);

    // All 6 sides of the frustum are tested against all four corners
    // of the bounding box. If all four points of the box are outside of
    // a frustum-plane, then the bounding box is not visible and we return
    // true.  If the box survives all tests then it is visible.
    for (size_t i = 0; i < 6; i++) {
      int outside = 0;
      for (size_t j = 0; j < 4; j++) {
        vec3 pos = (i < 4) ? campos_
          : ((i == 4) ? nearp() : farp());
        if (glm::dot(getNormal(static_cast<int>(i)), points[j] - pos) >= 0.f)
          ++outside;
      }
      if (outside == 4)
        return true;
    }
    return false;
  }

  // Note: Seems to work, but not really tested.
  bool Frustum::isCulled(const BoundingBox& bounds) const {
    vec3 urb = bounds.getURB();
    vec3 llf = bounds.getLLF();

    vec3 points[8];
    points[0] = vec3(llf.x, llf.y, llf.z);
    points[1] = vec3(urb.x, llf.y, llf.z);
    points[2] = vec3(urb.x, llf.y, urb.z);
    points[3] = vec3(llf.x, llf.y, urb.z);
    points[4] = vec3(llf.x, urb.y, llf.z);
    points[5] = vec3(urb.x, urb.y, llf.z);
    points[6] = vec3(urb.x, urb.y, urb.z);
    points[7] = vec3(llf.x, urb.y, urb.z);

    for (size_t i = 0; i < 6; i++) {
      int outside = 0;
      for (size_t j = 0; j < 8; j++) {
        vec3 pos = (i < 4) ? campos_
          : ((i == 4) ? nearp() : farp());

        if (glm::dot(getNormal(static_cast<int>(i)), points[j] - pos) >= 0.f)
          ++outside;
      }
      if (outside == 8)
        return true;
    }

    return false;

  }

  // Is this point culled?
  bool Frustum::isCulled(const vec3& v) const {
    // the position of the camera is lying on the top, bottom, left, and right
    // plane, so we might as well use it as the reference point for those planes
    vec3 dist = v - campos_;

    // test the point against all 6 planes of the frustum (there are more
    // efficient algorithms for this, if you deem this too slow knock yourself out and
    // code something fancy)
    if ((glm::dot(leftn(), dist) >= 0.f) ||
      (glm::dot(rightn(), dist) >= 0.f) ||
      (glm::dot(bottomn(), dist) >= 0.f) ||
      (glm::dot(topn(), dist) >= 0.f) ||
      (glm::dot(farn(), v - farp()) >= 0.f) ||
      (glm::dot(nearn(), v - nearp()) >= 0.f))
    {
      return true;
    }
    else {
      // all tests survived? then the point is visible
      return false;
    }
  }

  void Frustum::setFovy(float fovy) {
    float halfheight = tanf(0.5f * glm::deg2rad(fovy)) * nearDist_;
    //float oldRatio = getRatio();
    tnear_ = halfheight;
    bnear_ = -halfheight;

    // update left and right to reflect previous ratio
    //setRatio(oldRatio);
  }

  void Frustum::setRatio(float ratio) {
    float halfwidth = 0.5f * (tnear_ - bnear_) * ratio;
    lnear_ = -halfwidth;
    rnear_ = halfwidth;
    bnear_ = -halfwidth / ratio;
    tnear_ = halfwidth / ratio;
  }

  void Frustum::setNearDist(float nearDist) {
    nearDist_ = nearDist;
  }

  const vec3& Frustum::campos() const {
    return campos_;
  }

  const vec3& Frustum::farp() const {
    return farp_;
  }

  const vec3& Frustum::nearp() const {
    return nearp_;
  }

  const vec3& Frustum::farn() const {
    return normals_[FARN];
  }

  const vec3& Frustum::nearn() const {
    return normals_[NEARN];
  }

  const vec3& Frustum::topn() const {
    return normals_[TOPN];
  }

  const vec3& Frustum::bottomn() const {
    return normals_[BOTTOMN];
  }

  const vec3& Frustum::rightn() const {
    return normals_[RIGHTN];
  }

  const vec3& Frustum::leftn() const {
    return normals_[LEFTN];
  }

  void Frustum::setBottom(float v) {
    bnear_ = v;
  }

  void Frustum::setTop(float v) {
    tnear_ = v;
  }

  void Frustum::setRight(float v) {
    rnear_ = v;
  }

  void Frustum::setLeft(float v) {
    lnear_ = v;
  }

  void Frustum::setFarDist(float farDist) {
    farDist_ = farDist;
  }

  float Frustum::getBottom() const {
    return bnear_;
  }

  float Frustum::getFovy() const {
    return glm::rad2deg(atanf(tnear_ / nearDist_) - atanf(bnear_ / nearDist_));
  }

  float Frustum::getRatio() const {
    return (rnear_ - lnear_) / (tnear_ - bnear_);
  }

  float Frustum::getNearDist() const {
    return nearDist_;
  }

  float Frustum::getFarDist() const {
    return farDist_;
  }

  float Frustum::getLeft() const {
    return lnear_;
  }

  float Frustum::getRight() const {
    return rnear_;
  }

  float Frustum::getTop() const {
    return tnear_;
  }

  const vec3& Frustum::getNormal(int num) const {
    return normals_[num];
  }

} // end namespace tgt