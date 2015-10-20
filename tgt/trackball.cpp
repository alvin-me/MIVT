#include "trackball.h"
#include "camera.h"
#include "logmanager.h"

using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace tgt {

  Trackball::Trackball(Camera* camera, float maxValue)
    : camera_(camera)
    , moveCenter_(false)
    , size_(1.f)
    , maxValue_(maxValue)
    , minValue_(maxValue / 50000.f)
  {
    assert(camera);
    center_ = glm::vec3(0.f);
  }

  Trackball::~Trackball() {
  }

  /* Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
  if we are away from the center of the sphere. */
  vec3 Trackball::projectToSphere(const vec2& xy) const {

    static const float sqrt2 = sqrtf(2.f);
    vec3 result;

    float d = glm::length(xy);
    if (d < size_ * sqrt2 / 2.f) {
      // Inside sphere
      // The factor "sqrt2/2.f" make a smooth changeover from sphere to hyperbola. If we leave
      // factor 1/sqrt(2) away, the trackball would bounce at the changeover.
      result.z = sqrtf(size_ * size_ - d*d);
    }
    else {
      // On hyperbola
      float t = size_ / sqrt2;
      result.z = t*t / d;
    }

    result.x = xy.x;
    result.y = xy.y;
    return glm::normalize(result);
  }

  vec3 Trackball::coordTransform(const vec3& axis) const {
    mat4 rotation = glm::inverse(camera_->getRotateMatrix());
    //rotation = mat4(glm::vec4(0.327715963f, -0.486170501f, 0.810086668f, 0.f),
    //  glm::vec4(-0.0568156242f, -0.866026998f, -0.496758491f, 0.f),
    //  glm::vec4(0.943066359f, 0.116770126f, -0.311433017f, 0.f),
    //  glm::vec4(0.f, 0.f, 0.f, 1.f));
    return glm::mapVector(rotation, axis);
  }

  void Trackball::rotate(const glm::quat& quat) {
    if (moveCenter_)
      center_ = camera_->getFocus();

    vec3 position = camera_->getPosition();
    position -= center_;
    position = glm::rotate(quat, position);
    position += center_;

    vec3 focus = camera_->getFocus();
    focus -= center_;
    focus = glm::rotate(quat, focus);
    focus += center_;

    vec3 upVector = camera_->getUpVector();
    upVector = glm::rotate(quat, upVector);

    camera_->positionCamera(position, focus, upVector);

    lastOrientationChange_ = quat;
  }

  void Trackball::rotate(const vec3& axis, float phi) {
    // use coordTransform to get axis in world coordinates according to the axis given in camera coordinates
    rotate(glm::angleAxis(phi, glm::normalize(coordTransform(axis))));
  }

  void Trackball::rotate(const vec2& newMouse, const vec2& oldMouse) {

    /* Project the points onto the virtual trackball,
    * then figure out the axis of rotation, which is the cross
    * product of P1-P2 and O-P1 (O is the center of the ball, 0,0,0)
    * Note:  This is a deformed trackball -- it is a trackball in the center,
    * but is deformed into a hyperbolic sheet of rotation away from the
    * center (projectToSphere does that job).  This particular function was
    * chosen after trying out several variations.
    *
    * It is assumed that the arguments to this routine are in the range
    * (-1.0 ... 1.0) */

    if (newMouse == oldMouse) {
      // Zero rotation -> do nothing
      return;
    }

    // First, figure out z-coordinates for projection of P1 and P2 to deformed sphere
    vec3 p1 = projectToSphere(oldMouse);
    vec3 p2 = projectToSphere(newMouse);

    // Now, find the axis we are going to rotate about
    vec3 axis = glm::cross(p2, p1);

    //... and calculate the angle phi between the two vectors which is the
    //  angle we need to rotate about
    vec3 d = p1 - p2;
    float t = glm::length(d) / (2.0f * getSize());

    // avoid problems with out-of-control values...
    if (t > 1.0f) t = 1.0f;
    if (t < -1.0f) t = -1.0f;

    float phi = 2.f * asinf(t);

    rotate(axis, phi);
  }

  void Trackball::move(float length, vec3 axis) {

    if (length == 0.f || glm::length(axis) == 0.f)
      return;

    float frustFactor = camera_->getFocalLength() / camera_->getFrustum().getNearDist();
    float frustWidth = (camera_->getFrustum().getRight() - camera_->getFrustum().getLeft());
    float frustHeight = (camera_->getFrustum().getTop() - camera_->getFrustum().getBottom());
    axis = glm::normalize(axis) * length * frustFactor;
    axis.x *= frustWidth;
    axis.y *= frustHeight;

    // find axis in world coordinates according to the axis given in camera coordinates
    axis = coordTransform(axis);

    moveCamera(-axis);
  }

  void Trackball::move(const vec2& newMouse, const vec2& lastMouse) {
    vec2 mouseMotion = newMouse - lastMouse;
    vec3 axis;
    axis.x = mouseMotion.x;
    axis.y = mouseMotion.y;
    axis.z = 0;

    move(glm::length(mouseMotion), axis);
  }

  void Trackball::moveCamera(const vec3& motionvector) {
    camera_->setPosition(camera_->getPosition() + motionvector);
    camera_->setFocus(camera_->getFocus() + motionvector);
  }

  void Trackball::zoom(float factor) {
    // zoom factor is inverse proportional to scaling of the look vector, so invert:
    factor = 1.f / factor;
    camera_->setPosition((1.f - factor) * camera_->getFocus()
      + factor * camera_->getPosition());
  }

  void Trackball::zoom(const vec2& newMouse, const vec2& lastMouse, const vec2& mouseZoomInDirection) {
    zoom(1 + glm::dot((lastMouse - newMouse), mouseZoomInDirection));
  }

  void Trackball::zoomAbsolute(float focallength) {
    camera_->setPosition(camera_->getFocus() - focallength * camera_->getLook());
  }

  float Trackball::getCenterDistance() {
    if (moveCenter_)
      center_ = camera_->getFocus();
    return glm::length(camera_->getPosition() - center_);
  }

  Camera* Trackball::getCamera() const {
    return camera_;
  }

  void Trackball::setCamera(Camera* camera) {
    assert(camera);
    camera_ = camera;
  }

  void Trackball::setMinValue(float val) {
    minValue_ = val;
  }

  float Trackball::getMinValue() const {
    return minValue_;
  }

  void Trackball::setMaxValue(float val) {
    maxValue_ = val;
  }

  float Trackball::getMaxValue() const {
    return maxValue_;
  }

  void Trackball::adaptInteractionToScene(const BoundingBox& bound, float nearDist) {
    if (bound.volume() == 0.f) {
      LWARNINGC("common.Trackball", "Tried to set scene bounds with extent 0, not adapting to scene.");
      return;
    }

    // adapt only maxValue, far plane and trackball center if there was no previous scene geometry
    float newMaxDist = 250.f * glm::hmax(bound.diagonal());
    float newMinDist = (nearDist > 0.f ? nearDist : newMaxDist / 50000.f);
    getCamera()->setNearDist(newMinDist);
    getCamera()->setFarDist(newMaxDist + glm::hmax(bound.diagonal()));

    setCenter(bound.center());
    setMinValue(newMinDist);
    setMaxValue(newMaxDist);
  }

} // end namespace tgt