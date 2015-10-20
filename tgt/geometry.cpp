#include "geometry.h"
#include "logmanager.h"

namespace tgt {

  const std::string Geometry::loggerCat_("Geometry");

  Geometry::Geometry()
    : transformationMatrix_(glm::mat4(1))
  {}

  void Geometry::transform(const glm::mat4& m) {
    transformationMatrix_ = m * transformationMatrix_;
  }

  glm::mat4 Geometry::getTransformationMatrix() const {
    return transformationMatrix_;
  }

  glm::mat4 Geometry::getInvertedTransformationMatrix() const {
    return glm::inverse(transformationMatrix_);
  }

  void Geometry::setTransformationMatrix(const glm::mat4& m) {
    transformationMatrix_ = m;
  }

  void Geometry::render() const {
    LWARNING("render() not implemented");
  }

  BoundingBox Geometry::getBoundingBox(bool /*transformed*/) const {
    LWARNING("getBoundingBox() not implemented");
    return BoundingBox();
  }

} // end namespace tgt