#pragma once

#include "tgt_math.h"
#include "config.h"

namespace tgt {

  /*
  * A VolumeRepresentation is a class storing the volume data (the voxels) in some form.
  * Code should generally work with Volumes instead of VolumeRepresentations.
  */
  class VolumeRepresentation {
  public:
    TGT_API VolumeRepresentation(const glm::ivec3& dimensions);
    TGT_API virtual ~VolumeRepresentation() {}

    /// Returns the VolumeRepresentation's dimensions in voxel coordinates.
    TGT_API virtual glm::ivec3 getDimensions() const;

    /// Returns the number of voxels contained by the VolumeRepresentation.
    TGT_API virtual size_t getNumVoxels() const;

    TGT_API virtual size_t getBytesPerVoxel() const = 0;

  protected:
    // protected default constructor
    VolumeRepresentation() {}

    glm::ivec3  dimensions_;
    size_t      numVoxels_;
  };

} // end namespace tgt