#pragma once

#include "tgt_math.h"
#include "boundingbox.h"

namespace tgt {

  class Camera;

  /**
  *  This class implements a Trackball which can be used to freely rotate an object
  *  around a given center.
  */
  class Trackball {

  public:
    TGT_API Trackball(Camera* camera, float maxValue = 500.f);
    TGT_API virtual ~Trackball();

    /// Returns the camera this trackball operates on.
    TGT_API Camera* getCamera() const;

    /// Assigns the camera to be modified by the trackball.
    TGT_API void setCamera(Camera* camera);

    /// Rotate the trackball according to Quaternion quat.
    /// @param quat Quaternion represention rotation.
    TGT_API void rotate(const glm::quat& quat);

    /// Rotate the trackball by angle phi around axis axis.
    /// @param axis axis in camera coordinates.
    TGT_API virtual void rotate(const glm::vec3& axis, float phi);

    /// Rotate the trackball according to new mouse position.
    /// @param mouse coodinates of mouse, scaled to [-1, 1]x[-1, 1]
    TGT_API void rotate(const glm::vec2& newMouse, const glm::vec2& lastMouse);

    /// Move the trackball along axis.
    /// @param length Distance relative to frustum dimensions at trackball center.
    ///               Trackball center will get out of sight when >> 1.
    /// @param axis Axis in camera coordinates along which to move.
    TGT_API virtual void move(float length, glm::vec3 axis);

    /// Move the trackball according to new mouse position.
    /// @param mouse coodinates of mouse, scaled to [-1, 1]x[-1, 1]
    TGT_API void move(const glm::vec2& newMouse, const glm::vec2& lastMouse);

    /// Zoom in by factor factor.
    TGT_API virtual void zoom(float factor);

    /// Zoom in according to new mouse position.
    /// @param mouse coodinates of mouse, scaled to [-1, 1]x[-1, 1]
    TGT_API void zoom(const glm::vec2& newMouse, const glm::vec2& lastMouse, const glm::vec2& mouseZoomInDirection);

    /// set an absolute Distance from Focus
    TGT_API void zoomAbsolute(float focallength);

    /// last rotation applied to trackball
    TGT_API glm::quat getLastOrientationChange() const { return lastOrientationChange_; }

    /// If the center of the trackball should be moved relative to objects.
    /// This has large influence on trackball behavior. If you look at one object located at
    /// world coordinates origin, you might want the center not to move.
    /// If you use trackball to look at mutiple objects, located at varying position, you might
    /// want the center to move.
    /// See trackball demo in samples folder.
    TGT_API void setMoveCenter(bool b = true) { moveCenter_ = b; };
    TGT_API bool getMoveCenter() const { return moveCenter_; };

    /// Set trackballs center. Most people do not need this method. Use with caution!
    TGT_API void setCenter(const glm::vec3& center) { center_ = center; };
    TGT_API glm::vec3 getCenter() const { return center_; };

    /// Move the camera in a certain direction
    TGT_API void moveCamera(const glm::vec3& motionvector);

    /// set the radius of the trackball to a given value
    TGT_API void setSize(float size) { size_ = size; };
    TGT_API float getSize() const { return size_; };

    /// Returns the orthogonal distance between camera position and center of the trackball
    /// with respect to the camera's look vector.
    TGT_API float getCenterDistance();

    /// Set the minimum absolute value that an element of the camera position vector can have
    TGT_API void setMinValue(float val);

    /// \sa setMinValue
    TGT_API float getMinValue() const;

    /// Set the maximum absolute value that an element of the camera position vector can have
    TGT_API void setMaxValue(float val);

    /// \sa setMaxValue
    TGT_API float getMaxValue() const;

    TGT_API void adaptInteractionToScene(const BoundingBox& bound, float nearDist = 0.f);

  protected:
    /// projection math
    glm::vec3 projectToSphere(const glm::vec2& xy) const;

    /// transform vectors given in camera coordinates to vectors in world coordinates
    glm::vec3 coordTransform(const glm::vec3& axis) const;

  protected:

    Camera* camera_;

    /// this holds the center around which the camera will be rotated
    glm::vec3 center_;

    /// Wheather to change the center in move method.
    /// center_ will alway be (0,0,0) if false.
    bool moveCenter_;

    /// the trackball size effects the trackball sensitivity
    float size_;

    /// last rotation applied to trackball
    glm::quat lastOrientationChange_;

    float minValue_;
    float maxValue_;
  };

} // end namespace tgt
