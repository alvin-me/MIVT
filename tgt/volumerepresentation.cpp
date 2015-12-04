#include "volumerepresentation.h"

namespace tgt {

  VolumeRepresentation::VolumeRepresentation(const glm::ivec3& dimensions)
    : dimensions_(dimensions)
  {
    numVoxels_ = glm::hmul(dimensions_);
  }

  glm::ivec3 VolumeRepresentation::getDimensions() const {
    return dimensions_;
  }

  size_t VolumeRepresentation::getNumVoxels() const {
    return numVoxels_;
  }

} // end namespace tgt