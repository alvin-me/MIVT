#pragma once

#include "config.h"

namespace tgt {
  class OffScreenRender;
}

namespace mivt {

  class Application
  {
  public:
    MIVT_API Application();

    MIVT_API ~Application();

    MIVT_API void GetPixels(unsigned char* buffer, int length);

    MIVT_API void Resize(int width, int height);

  private:
    tgt::OffScreenRender *offscreen_;
  };

}


