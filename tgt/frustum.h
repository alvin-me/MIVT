#pragma once

#include "boundingbox.h"

namespace tgt {

  class Camera;

  /**
  * This class is used by the Camera for culling in connection with axis aligned bounding boxes
  * or points.
  */
  class Frustum {
  public:

    /** Constructor
    *
    * @param fovy the viewing angle in vertical direction in degrees.
    */
    TGT_API Frustum(float fovy, float ratio, float nearDist, float farDist);
    TGT_API Frustum(float left, float right, float bottom, float top, float nearDist, float farDist);

    /// These methodes can be used to find out wether or not a bounding-box or a point are visible.
    /// Do not forget to update() before useage if camera position or viewing axis changed!
    TGT_API bool isCulledXZ(const BoundingBox& b) const;
    TGT_API bool isCulled(const BoundingBox& b) const;
    TGT_API bool isCulled(const glm::vec3& v) const;

    /// update normals and points on 6 planes to fit to current frustem parameters and to
    /// parameters of the camera given
    TGT_API void update(Camera* c);

    /**
    * Returns the viewing angle in vertical direction in degrees.
    */
    TGT_API float getFovy() const;
    TGT_API float getRatio() const;
    TGT_API float getNearDist() const;
    TGT_API float getFarDist() const;
    TGT_API float getLeft() const;
    TGT_API float getRight() const;
    TGT_API float getTop() const;
    TGT_API float getBottom() const;
    /// set the viewing angle in vertical direction in degrees.
    TGT_API void setFovy(float fovy);
    /// set width of frustum according to current height (or y viewing angle)
    TGT_API void setRatio(float ratio);
    TGT_API void setNearDist(float nearDist);
    TGT_API void setFarDist(float farDist);
    TGT_API void setLeft(float v);
    TGT_API void setRight(float v);
    TGT_API void setTop(float v);
    TGT_API void setBottom(float v);
    TGT_API const glm::vec3& leftn() const;
    TGT_API const glm::vec3& rightn() const;
    TGT_API const glm::vec3& bottomn() const;
    TGT_API const glm::vec3& topn() const;
    TGT_API const glm::vec3& nearn() const;
    TGT_API const glm::vec3& farn() const;
    TGT_API const glm::vec3& nearp() const;
    TGT_API const glm::vec3& farp() const;
    TGT_API const glm::vec3& campos() const;

    /// Get the 1 of the 6 normals leftn, rightn, bottomn, topn, nearn, farn specified by the
    /// number num.
    ///
    /// Can be used to handle normals within a for-loop e.g. when culling against frustum, see
    /// Camara::isCulled, Camera::isCulledXZ.
    TGT_API const glm::vec3& getNormal(int num) const;
    TGT_API bool operator==(const Frustum& rhs) const;

  private:
    /// the distances from the center of the near plane to the left, right, top and bottom borders
    float lnear_;
    float rnear_;
    float bnear_;
    float tnear_;
    /// the distances from the position of the camera to the near- and far-plane
    float nearDist_;
    float farDist_;

    enum normalsEnum { LEFTN, RIGHTN, TOPN, BOTTOMN, NEARN, FARN, NORMALS_NUM };

    glm::vec3 normals_[NORMALS_NUM];

    /// Points on the 6 planes; the position campos_ of the Camera lies on
    /// the top, bottom, left and right planes so we need only this point for the four
    /// These are used in culling methods.
    glm::vec3 nearp_;
    glm::vec3 farp_;
    glm::vec3 campos_;
  };

} // end namespace tgt