#pragma once

#include "volumederiveddata.h"

#include <vector>

namespace tgt {

  class Volume;

  class VolumePreview : public VolumeDerivedData {
  public:
    TGT_API VolumePreview();
    TGT_API VolumePreview(int height, const std::vector<unsigned char>& data);

    TGT_API virtual VolumeDerivedData* createFrom(Volume *handle) const;

    TGT_API int getHeight() const;

    TGT_API const std::vector<unsigned char>& getData() const;

  private:
    int height_;

    std::vector<unsigned char> prevData_;

    static const std::string loggerCat_;
  };

} // end namespace tgt