#include "volumesculpt.h"

namespace mivt {

  VolumeSculpt::VolumeSculpt()
    : volume_(0)
  {
  }


  VolumeSculpt::~VolumeSculpt()
  {
  }

  void VolumeSculpt::SetVolume(tgt::Volume *volume)
  {
    volume_ = volume;
  }

  bool VolumeSculpt::SculptCPU(const std::vector<glm::vec2> &polygon)
  {
    polygon;
    return false;
  }
}
