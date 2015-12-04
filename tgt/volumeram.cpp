#include "volumeram.h"

namespace tgt {

  VolumeRAM::VolumeRAM(const glm::ivec3& dimensions)
    : VolumeRepresentation(dimensions)
  {}

    float VolumeRAM::getVoxelNormalizedLinear(const glm::vec3& pos) const {
    glm::vec3 posAbs = glm::max(pos - 0.5f, glm::vec3(0.0f));
    glm::vec3 p = posAbs - glm::floor(posAbs); // get decimal part
    glm::ivec3 llb = glm::ivec3(posAbs);
    glm::ivec3 urf = glm::ivec3(glm::ceil(posAbs));
    urf = glm::min(urf, dimensions_ - glm::ivec3(1)); // clamp so the lookups do not exceed the dimensions
    llb = glm::min(llb, dimensions_ - glm::ivec3(1)); // dito

    /*
    interpolate linearly
    */
    return  getVoxelNormalized(glm::ivec3(llb.x, llb.y, llb.z)) * (1.f-p.x)*(1.f-p.y)*(1.f-p.z) // llB
          + getVoxelNormalized(glm::ivec3(urf.x, llb.y, llb.z)) * (    p.x)*(1.f-p.y)*(1.f-p.z) // lrB
          + getVoxelNormalized(glm::ivec3(urf.x, urf.y, llb.z)) * (    p.x)*(    p.y)*(1.f-p.z) // urB
          + getVoxelNormalized(glm::ivec3(llb.x, urf.y, llb.z)) * (1.f-p.x)*(    p.y)*(1.f-p.z) // ulB
          + getVoxelNormalized(glm::ivec3(llb.x, llb.y, urf.z)) * (1.f-p.x)*(1.f-p.y)*(    p.z) // llF
          + getVoxelNormalized(glm::ivec3(urf.x, llb.y, urf.z)) * (    p.x)*(1.f-p.y)*(    p.z) // lrF
          + getVoxelNormalized(glm::ivec3(urf.x, urf.y, urf.z)) * (    p.x)*(    p.y)*(    p.z) // urF
          + getVoxelNormalized(glm::ivec3(llb.x, urf.y, urf.z)) * (1.f-p.x)*(    p.y)*(    p.z);// ulF
  }

} // end namespace tgt