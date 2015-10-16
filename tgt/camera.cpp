#include "camera.h"
#include "tgt_gl.h"
#include "matrixstack.h"

using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::mat4;

namespace tgt {

  Camera::Camera(const vec3& position, const vec3& focus, const vec3& up,
    float fovy, float ratio, float distn, float distf, ProjectionMode pm)
    : position_(position),
    focus_(focus),
    upVector_(glm::normalize(up)),
    frust_(Frustum(fovy, ratio, distn, distf)),
    projectionMode_(pm)
  {
    viewMatrix_ = glm::lookAt(position, focus, up);
  }

  Camera::Camera(const Camera& cam)
    : position_(cam.getPosition()),
    focus_(cam.getFocus()),
    upVector_(cam.getUpVector()),
    frust_(cam.getFrustum()),
    projectionMode_(cam.getProjectionMode())
  {
    viewMatrix_ = glm::lookAt(getPosition(), getFocus(), upVector_);
  }

  Camera::~Camera() {
  }

  bool Camera::operator==(const Camera& rhs) const {
    return (rhs.position_ == position_) && (rhs.focus_ == focus_) && (rhs.upVector_ == upVector_) && (rhs.frust_ == frust_) &&
      (rhs.projectionMode_ == projectionMode_);
  }

  bool Camera::operator!=(const Camera& rhs) const {
    return !(*this == rhs);
  }

  // This is called to set up the Camera-View
  void Camera::look(float windowRatio) {
    MatStack.matrixMode(MatrixStack::PROJECTION);
    MatStack.loadIdentity();
    updateFrustum();
    MatStack.loadMatrix(getFrustumMatrix(windowRatio));
    MatStack.matrixMode(MatrixStack::MODELVIEW);
    MatStack.loadIdentity();
    updateVM();
    MatStack.loadMatrix(viewMatrix_);
  }

  void Camera::look(ivec2 windowSize) {
    look(static_cast<float>(windowSize.x) / windowSize.y);
  }

  // Private method that updates the relevant frustum parameters
  void Camera::updateFrustum() {
    frust_.update(this);
  }

  mat4 Camera::getViewMatrix() const {
    updateVM();
    return viewMatrix_;
  }

  void Camera::setViewMatrix(const mat4& mvMat) {
    mat4 inv = glm::inverse(mvMat);
    // preserve the focallength
    float focallength = glm::length(focus_ - position_);

    // calculate world-coordinates
    vec4 pos = (inv * vec4(0.f, 0.f, 0.f, 1.f));
    vec4 look = (inv * vec4(0.f, 0.f, -1.f, 0.f));
    vec4 focus = pos + focallength * look;
    vec4 up = (inv * vec4(0.f, 1.f, 0.f, 0.f));

    positionCamera(pos.xyz(), focus.xyz(), up.xyz());

    updateVM();
  }

  mat4 Camera::getRotateMatrix() const {
    updateVM();
    return glm::getRotationalPart(viewMatrix_);
  }

  mat4 Camera::getViewMatrixInverse() const {
    updateVM();
    return glm::inverse(viewMatrix_);
  }

  mat4 Camera::getFrustumMatrix(float windowRatio) const {
    Frustum f = getFrustum();
    return glm::frustum(f.getLeft() * windowRatio, f.getRight() * windowRatio,
      f.getBottom(), f.getTop(),
      f.getNearDist(), f.getFarDist());
  }

  mat4 Camera::getFrustumMatrix(ivec2 windowSize) const {
    return getFrustumMatrix(static_cast<float>(windowSize.x) / windowSize.y);
  }

  mat4 Camera::getProjectionMatrix(ivec2 windowSize) const {
    return getProjectionMatrix(static_cast<float>(windowSize.x) / windowSize.y);
  }

  mat4 Camera::getProjectionMatrix(float windowRatio) const {
    if (projectionMode_ == ORTHOGRAPHIC) {
      if (windowRatio > 1.0f)
        return glm::ortho(frust_.getLeft() * windowRatio, frust_.getRight() * windowRatio,
        frust_.getTop(), frust_.getBottom(),
        -frust_.getNearDist(), frust_.getFarDist());
      else
        return glm::ortho(frust_.getLeft(), frust_.getRight(),
        frust_.getTop() * (1.0f / windowRatio), frust_.getBottom() * (1.0f / windowRatio),
        -frust_.getNearDist(), frust_.getFarDist());
    }
    else if (projectionMode_ == PERSPECTIVE) {
      float fovy = frust_.getFovy();
      if (fovy < 6.f)
        fovy = 6.f;
      if (fovy > 175.f)
        fovy = 175.f;

      if (windowRatio >= 1.0f)
        return glm::perspective(glm::deg2rad(fovy), frust_.getRatio() * windowRatio, frust_.getNearDist(), frust_.getFarDist());
      else
        return glm::perspective(atan(tan(glm::deg2rad(fovy / 2.f)) / (windowRatio* frust_.getRatio())) * 2, frust_.getRatio() * windowRatio, frust_.getNearDist(), frust_.getFarDist());
    }
    else
      return getFrustumMatrix(windowRatio);
  }

  float Camera::getFarDist() const {
    return frust_.getFarDist();
  }

  float Camera::getNearDist() const {
    return frust_.getNearDist();
  }

  vec3 Camera::project(ivec2 vp, vec3 point) const {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];

    mat4 projection_glm = getProjectionMatrix(vp);
    mat4 modelview_glm = getViewMatrix();
    for (int i = 0; i < 4; ++i) {
      modelview[i + 0] = modelview_glm[i].x;
      modelview[i + 4] = modelview_glm[i].y;
      modelview[i + 8] = modelview_glm[i].z;
      modelview[i + 12] = modelview_glm[i].w;
      projection[i + 0] = projection_glm[i].x;
      projection[i + 4] = projection_glm[i].y;
      projection[i + 8] = projection_glm[i].z;
      projection[i + 12] = projection_glm[i].w;
    }
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = vp.x;
    viewport[3] = vp.y;

    GLdouble pointProjectedGL[3];
    gluProject(point.x, point.y, point.z, modelview, projection, viewport,
      &pointProjectedGL[0], &pointProjectedGL[1], &pointProjectedGL[2]);

    return vec3(static_cast<float>(pointProjectedGL[0]),
      static_cast<float>(pointProjectedGL[1]),
      static_cast<float>(pointProjectedGL[2]));
  }

} // end namespace tgt