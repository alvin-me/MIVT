#pragma once

#include "tgt_math.h"
#include "boundingbox.h"

namespace tgt {

  /**
  * Abstract base class for Geometry objects that
  * can be passed through GeometryPorts.
  *
  * @note In order to enable serialization (necessary for caching),
  *  a factory has to be provided for each concrete subtype.
  */
  class Geometry {
  public:
    TGT_API Geometry();
    TGT_API virtual ~Geometry() {}

    /**
    * Returns whether the geometry is equal to the passed one.
    *
    * This function is supposed to be overridden by subclasses.
    * The default implementation compares the geometries' hashes.
    *
    * @param geometry Geometry object to compare to
    * @param epsilon maximum distance at which two vertices are to be considered equal
    */
    TGT_API virtual bool equals(const Geometry* geometry, double epsilon = 1e-5) const = 0;

    /// Transforms the geometry by multiplying the given transformation matrix to the existing one.
    TGT_API virtual void transform(const glm::mat4& m);

    TGT_API glm::mat4 getTransformationMatrix() const;
    TGT_API glm::mat4 getInvertedTransformationMatrix() const;

    TGT_API void setTransformationMatrix(const glm::mat4& m);

    /**
    * Renders the geometry.
    *
    * This function is supposed to be overridden by subclasses.
    * The default implementation is a no-op.
    */
    TGT_API virtual void render() const;

    /**
    * Returns the axis-aligned bounding box of the geometry.
    *
    * This function is supposed to be overridden by subclasses.
    * The default implementation returns undefined bounds.
    *
    * @param transformed Apply transformation matrix?
    */
    TGT_API virtual BoundingBox getBoundingBox(bool transformed = true) const;

  private:
    glm::mat4 transformationMatrix_;    ///< Usually model to world

    static const std::string loggerCat_;
  };

} // end namespace tgt