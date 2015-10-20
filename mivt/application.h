#pragma once

#include "config.h"
#include <string>

namespace tgt {
  class OffScreenRender;
  class LogManager;
}

namespace mivt {

  class Raycaster;
  class Application
  {
  public:
    MIVT_API Application();

    MIVT_API ~Application();

    MIVT_API void Initialize();

    MIVT_API void Deinitialize();

    MIVT_API void GetPixels(unsigned char* buffer, int length);

    MIVT_API void Resize(int width, int height);

    MIVT_API void Rotate(int newPosX, int newPosY, int lastPosX, int lastPosY);

    MIVT_API void Zoom(int newPosX, int newPosY, int lastPosX, int lastPosY);

    MIVT_API void Pan(int newPosX, int newPosY, int lastPosX, int lastPosY);

  private:
    std::string getBasePath(const std::string& filename = "") const;
    std::string getProgramPath() const;
    std::string getUserDataPath(const std::string& filename = "") const;
    std::string getResourcePath(const std::string& filename = "") const;
    void initLogging();

  private:
    tgt::OffScreenRender    *offscreen_;
    tgt::LogManager         *logManager_;
    Raycaster               *raycaster_;

    std::string             programPath_;
    std::string             basePath_;
    std::string             userDataPath_;
  };

}


