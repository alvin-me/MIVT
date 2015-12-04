#include "volumesculpt.h"
#include "logmanager.h"
#include "volumeatomic.h"
#include "volume.h"
#include "camera.h"
#include "scanline.h"

namespace mivt {

  const std::string VolumeSculpt::loggerCat_ = "VolumeSculpt";

  VolumeSculpt::VolumeSculpt()
    : maskVolume_(0)
  {
  }


  VolumeSculpt::~VolumeSculpt()
  {
  }

  void VolumeSculpt::SetMaskVolume(tgt::Volume *volume)
  {
    maskVolume_ = volume;
  }

  bool VolumeSculpt::SculptCPU(const std::vector<glm::vec2> &polygon,
    tgt::Camera *cam, const glm::ivec2 viewSize, const glm::mat4& voxelToWorld)
  {
    if (polygon.size() < 3) {
      LERROR("polygon points less than 3");
      return false;
    }

    unsigned char* data = reinterpret_cast<uint8_t*>(maskVolume_->getRepresentation<tgt::VolumeRAM>()->getData());
    if (!data) {
      LERROR("mask volume not allocated.");
      return false;
    }

    glm::ivec3 volume_dim = maskVolume_->getDimensions();
    glm::mat4 volume2ClipCoord = cam->getProjectionMatrix(viewSize)
      * cam->getViewMatrix()
      * voxelToWorld;
    float zNear = cam->getNearDist();
    float zFar = cam->getFarDist();

    for (int idz = 0; idz < volume_dim.z; ++idz) {
      for (int idy = 0; idy < volume_dim.y; ++idy) {
        for (int idx = 0; idx < volume_dim.x; ++idx) {
          glm::vec4 point = glm::vec4(idx, idy, idz, 1);
          point = volume2ClipCoord * point;

          // NDC
          point /= point.w;

          // Window Coordinate
          point.x = point.x * viewSize.x * 0.5f + viewSize.x * 0.5f;
          point.y = point.y * viewSize.y * 0.5f + viewSize.y * 0.5f;
          point.z = point.z * (zFar - zNear) * 0.5f + (zFar + zNear) * 0.5f;

          if (ScanLine::PointInPolygon(polygon, glm::dvec2(point))) {
            data[idz * volume_dim.y * volume_dim.x + idy * volume_dim.x + idx] = 255;
          }
        }
      }
    }

    return true;
  }
}
