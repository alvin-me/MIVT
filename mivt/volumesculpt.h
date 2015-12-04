#pragma once
#include <vector>
#include "tgt_math.h"

namespace tgt {
  class Volume;
  class Camera;
}

namespace mivt {
  class VolumeSculpt
  {
  public:
    VolumeSculpt();
    ~VolumeSculpt();

    void SetMaskVolume(tgt::Volume *maskVolume);

    //void Process();

    bool SculptCPU(const std::vector<glm::vec2> &polygon,
      tgt::Camera *cam, const glm::ivec2 viewSize, const glm::mat4& voxelToWorld);

  private:
    tgt::Volume *maskVolume_;

    static const std::string loggerCat_;
  };
}


