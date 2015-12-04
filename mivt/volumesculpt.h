#pragma once
#include <vector>
#include "tgt_math.h"

namespace tgt {
  class Volume;
}

namespace mivt {
  class VolumeSculpt
  {
  public:
    VolumeSculpt();
    ~VolumeSculpt();

    void SetVolume(tgt::Volume *volume);

    void Process();

  private:
    bool SculptCPU(const std::vector<glm::vec2> &polygon);

  private:
    tgt::Volume *volume_;
  };
}


