#pragma once

#include "frustum.h"

namespace tgt {

  /**
  * This class implements a standard Camera with a position, a focus point
  * and an up-vector which make up its orientation.
  */
  class Camera {
  public:
    enum ProjectionMode {
      ORTHOGRAPHIC,
      PERSPECTIVE,
      FRUSTUM
    };

    /**
    * Constructor.  A standard starting speed and orientation are given.
    *
    * @param position coordinates of the point the camera should be located at
    * @param focus coordinates of the point the camera should look to
    * @param up the up-vector
    * @param fovy the field of view angle, in degrees, in the y direction
    * @param ratio the aspect ratio, ratio of x (width) to y (height)
    * @param distn distance to nearplane
    * @param distf distance to farplane
    */
    TGT_API Camera(const glm::vec3& position = glm::vec3(0.f, 0.f, 0.f),
      const glm::vec3& focus = glm::vec3(0.f, 0.f, -1.f),
      const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
      float fovy = 45.f,
      float ratio = 1.0f,
      float distn = 0.1f,
      float distf = 50.f,
      ProjectionMode pm = PERSPECTIVE);

    /// Copy constructor.
    TGT_API Camera(const Camera& cam);

    TGT_API virtual ~Camera();

    /// Setter / Getter
    TGT_API void setPosition(const glm::vec3& pos) { position_ = pos; }
    TGT_API void setFocus(const glm::vec3& foc)    { focus_ = foc; }
    TGT_API void setUpVector(const glm::vec3& up)  { upVector_ = glm::normalize(up); }

    TGT_API void setFrustum(const Frustum& frust) { frust_ = frust; }

    /// get Camera's strafe vector - a vector directing to the 'right'
    TGT_API glm::vec3 getStrafe()             const { return glm::normalize(glm::cross(getLook(), upVector_)); }
    TGT_API glm::vec3 getUpVector() const { return upVector_; }
    TGT_API glm::vec3 getPosition() const { return position_; }
    TGT_API glm::vec3 getLook()            const { return glm::normalize(focus_ - position_); }
    TGT_API glm::vec3 getFocus() const { return focus_; }
    TGT_API const Frustum& getFrustum() const { return frust_; }

    TGT_API float getFovy() const { return frust_.getFovy(); }
    TGT_API float getRatio() const { return frust_.getRatio(); }
    TGT_API float getNearDist() const;
    TGT_API float getFarDist() const;
    TGT_API float getFrustLeft() const { return frust_.getLeft(); }
    TGT_API float getFrustRight() const { return frust_.getRight(); }
    TGT_API float getFrustTop() const { return frust_.getTop(); }
    TGT_API float getFrustBottom() const { return frust_.getBottom(); }

    /// Set vertical viewing angle of camera.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    /// @param fovy angle in degree
    TGT_API void setFovy(float fovy) {
      if (projectionMode_ == PERSPECTIVE) {
        float oldRatio = frust_.getRatio();
        frust_.setFovy(fovy);
        frust_.setRatio(oldRatio);
      }
    }

    /// Set aspect ratio.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setRatio(float ratio) {
      if (projectionMode_ == PERSPECTIVE)
        frust_.setRatio(ratio);
    }

    /// Set distance from camera to nearplane.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!  Also, in perspective mode, changing the near distance also internally
    //  changes the paramters "Field of Vision" and thereby "Aspect ratio", so we have to take care not to damage
    //  those values.
    TGT_API void setNearDist(float neardist) {
      if (projectionMode_ == PERSPECTIVE) {
        float oldFovy = frust_.getFovy();
        float oldRatio = frust_.getRatio();
        frust_.setNearDist(neardist);
        frust_.setFovy(oldFovy);
        frust_.setRatio(oldRatio);
      }
      else
        frust_.setNearDist(neardist);
    }

    /// Set distance from camera to farplane.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setFarDist(float fardist) {
      frust_.setFarDist(fardist);
    }

    /// Set coordiante of left clipping plane.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setFrustLeft(float v) {
      frust_.setLeft(v);
      if (projectionMode_ == PERSPECTIVE)
        frust_.setRight(-v);
    }

    /// Set coordiante of right clipping plane.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setFrustRight(float v) {
      frust_.setRight(v);
      if (projectionMode_ == PERSPECTIVE)
        frust_.setLeft(-v);
    }

    /// Set coordiante of top clipping plane.
    ///
    /// Normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setFrustTop(float v) {
      frust_.setTop(v);
      if (projectionMode_ == PERSPECTIVE)
        frust_.setBottom(-v);
    }

    /// Set coordiante of bottom clipping plane.
    ///
    /// normals of frustum do not get updated by this method, call updateFrustum manually before
    /// using e.g. culling methods!
    TGT_API void setFrustBottom(float v) {
      frust_.setBottom(v);
      if (projectionMode_ == PERSPECTIVE)
        frust_.setTop(-v);
    }

    TGT_API void setFocalLength(float f)   {
      if (f == 0.f)
        return;

      setFocus(position_ + f * getLook());
    }

    TGT_API float getFocalLength() const   {
      return glm::distance(getFocus(), getPosition());
    }

    /// Used to reposition the Camera.
    TGT_API void positionCamera(const glm::vec3& pos, const glm::vec3& focus, const glm::vec3& up) {
      setPosition(pos);
      setFocus(focus);
      setUpVector(up);
    }

    /// set projection mode
    TGT_API void setProjectionMode(ProjectionMode pm) {
      projectionMode_ = pm;
      // reintroduce symmetry for perspective and orthogonal mode
      if (pm == PERSPECTIVE) {
        setFrustRight(-getFrustLeft());
        setFrustTop(-getFrustBottom());
      }
    }

    /// get projection mode
    TGT_API ProjectionMode getProjectionMode() const {
      return projectionMode_;
    }

    /// actually turns on the Camera.
    TGT_API void look(float windowRatio);
    /// @overload
    TGT_API void look(glm::ivec2 windowSize);

    /// Update the frustum with the current camera parameters.
    /// This method MUST be called before a culling-method is used.  The reason this is not called
    /// in the culling-methods themselves is that we may have a lot of culling-querys without
    /// acutally changing the camera and thus, the frustum; in that case, frequently updating the
    /// frustum would be a waste of time.
    TGT_API void updateFrustum();

    /// If you need the view matrix that would be used if look() was called, but without
    /// actually setting it in OpenGL, use this function.
    TGT_API glm::mat4 getViewMatrix() const;

    /// Sets a new view matrix; "reverse engineers" \a position_ and other parameters
    TGT_API void setViewMatrix(const glm::mat4& mvMat);

    /// This returns the inverse of the current ViewMatrix.
    TGT_API glm::mat4 getViewMatrixInverse() const;

    /// If you need only the matrix that represents the current rotation of the camera-tripod,
    /// use this function.
    TGT_API glm::mat4 getRotateMatrix() const;

    /// This method returns the frustum matrix
    TGT_API virtual glm::mat4 getFrustumMatrix(float windowRatio) const;
    ///@overload
    TGT_API virtual glm::mat4 getFrustumMatrix(glm::ivec2 windowSize) const;

    /// This method returns the projection matrix
    TGT_API virtual glm::mat4 getProjectionMatrix(float windowRatio) const;
    ///@overload
    TGT_API virtual glm::mat4 getProjectionMatrix(glm::ivec2 windowSize) const;

    TGT_API glm::vec3 project(glm::ivec2 vp, glm::vec3 point) const;

    TGT_API bool operator==(const Camera& rhs) const;
    TGT_API bool operator!=(const Camera& rhs) const;

  protected:

    /// viewMatrix will not always be up to date according to position-, focus- and upVector.
    /// Make sure it is up to date.
    void updateVM() const {
      viewMatrix_ = glm::lookAt(getPosition(), getFocus(), getUpVector());
    }

    glm::vec3 position_; /// location of the camera
    glm::vec3 focus_;    /// location, the camera looks at
    glm::vec3 upVector_; /// up vector, always normalized

    /// A frustum is saved in order to cull objects that are not lying within the view of the
    /// camera
    Frustum frust_;

    /// This is the actual matrix that holds the current orientation and position of the
    /// Camera.
    mutable glm::mat4 viewMatrix_;

    ProjectionMode projectionMode_;
  };

} // end namespace tgt