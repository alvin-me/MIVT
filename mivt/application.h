#pragma once

#include "config.h"
#include <string>

namespace tgt {
  class OffScreenRender;
  class LogManager;
  class Volume;
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

    MIVT_API void LoadVolume(const std::string &fileName, 
      const std::string& format,
      const int dimension[3], 
      const float spacing[3],
      float intercept, 
      float slope, 
      float windowWidth,
      float windowCenter);

  private:
    std::string getBasePath(const std::string& filename = "") const;
    std::string getProgramPath() const;
    std::string getUserDataPath(const std::string& filename = "") const;
    std::string getResourcePath(const std::string& filename = "") const;
    void initLogging();

  private:
    tgt::OffScreenRender    *offscreen_;
    tgt::LogManager         *logManager_;
    tgt::Volume             *volume_;
    Raycaster               *raycaster_;

    std::string             programPath_;
    std::string             basePath_;
    std::string             userDataPath_;

    static const std::string loggerCat_;
  };

}


