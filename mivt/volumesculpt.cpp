#include "volumesculpt.h"
#include "logmanager.h"
#include "volumeatomic.h"
#include "volume.h"
#include "camera.h"
#include "scanline.h"
#include "shadermanager.h"
#include "volumegl.h"

namespace mivt {

  const std::string VolumeSculpt::loggerCat_ = "VolumeSculpt";

  VolumeSculpt::VolumeSculpt(bool computeOnGPU)
    : maskVolume_(0)
    , computeOnGPU_(computeOnGPU)
  {
    if (computeOnGPU_) {
      shaderProgram_ = ShdrMgr.loadCompute("sculpt.comp", false);

      glGenBuffers(1, &polygonBufferId_);
      
      LGL_ERROR;
    }
  }


  VolumeSculpt::~VolumeSculpt()
  {
    if (computeOnGPU_) {
      ShdrMgr.dispose(shaderProgram_);
      shaderProgram_ = 0;

      glDeleteBuffers(1, &polygonBufferId_);
    }
  }

  void VolumeSculpt::SetMaskVolume(tgt::Volume *volume)
  {
    maskVolume_ = volume;
  }

  void VolumeSculpt::Process(const std::vector<glm::vec2> &polygon,
    tgt::Camera *cam, const glm::ivec2 viewSize,
    const glm::mat4& voxelToWorld)
  {
    if (computeOnGPU_)
      SculptGPU(polygon, cam, viewSize, voxelToWorld);
    else
      SculptCPU(polygon, cam, viewSize, voxelToWorld);
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

    maskVolume_->SyncData();
    return true;
  }

  bool VolumeSculpt::SculptGPU(const std::vector<glm::vec2> &polygon,
    tgt::Camera *cam, const glm::ivec2 viewSize, const glm::mat4& voxelToWorld)
  {
    bool successful = true;

    // mask
    shaderProgram_->activate();
    int maskid = maskVolume_->getRepresentation<tgt::VolumeGL>()->getTexture()->getId();
    glBindImageTexture(0, maskid, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);

    // pack polygon data
    int poly_number = (int)polygon.size();
    float* poly_data = new float[poly_number * 2];
    for (int i = 0; i < poly_number; ++i) {
      poly_data[i * 2] = (float)polygon[i].x;
      poly_data[i * 2 + 1] = (float)polygon[i].y;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, polygonBufferId_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)* poly_number * 2, poly_data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, polygonBufferId_);
    delete[] poly_data;

    // polygon number
    successful &= shaderProgram_->setUniform("polyNumb", poly_number);

    // volume dim
    successful &= shaderProgram_->setUniform("vol_dim", glm::vec3(maskVolume_->getDimensions()));

    // mvp
    glm::mat4 volume2ClipCoord = cam->getProjectionMatrix(viewSize)
      * cam->getViewMatrix()
      * voxelToWorld;
    successful &= shaderProgram_->setUniform("mvp", volume2ClipCoord);

    // view size
    successful &= shaderProgram_->setUniform("view_size", glm::vec2(viewSize));

    // run compute shader!
    glm::ivec3 volume_dim = maskVolume_->getDimensions();
    glDispatchCompute((volume_dim.x + 15) / 16, (volume_dim.y + 15) / 16, volume_dim.z);

    // cleanup
    glBindImageTexture(0, 0, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    shaderProgram_->deactivate();

    glFinish();
    return successful;
  }
}
