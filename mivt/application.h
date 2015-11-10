#pragma once

#include "config.h"
#include "progressbar.h"
#include <string>

namespace tgt {
  class OffScreenRender;
  class LogManager;
  class Volume;
  class TransFunc1D;
}

namespace mivt {

  class RenderVolume;
  class Application
  {
  public:
    MIVT_API Application();

    MIVT_API ~Application();

    MIVT_API void Initialize();

    MIVT_API void Deinitialize();

    MIVT_API void GetPixels(unsigned char* buffer, int length, bool downsampling = false);

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

    MIVT_API void LoadVolume(const std::string &fileName, tgt::ProgressCallback callback);

    MIVT_API void SetTransfunc(const std::string& fileName);
    MIVT_API std::string GetTransfunc();

    MIVT_API void SetClassificationMode(const std::string& mode);
    MIVT_API std::string GetClassificationMode();

    MIVT_API void SetLightAmbient(const float v[4]);
    MIVT_API void GetLightAmbient(float v[4]);

    MIVT_API void SetLightDiffuse(const float v[4]);
    MIVT_API void GetLightDiffuse(float v[4]);

    MIVT_API void SetLightSpecular(const float v[4]);
    MIVT_API void GetLightSpecular(float v[4]);

    MIVT_API void SetMaterialShininess(float v);
    MIVT_API float GetMaterialShininess();

    MIVT_API void SetFirstBgColor(const float v[4]);
    MIVT_API void GetFirstBgColor(float v[4]);

    MIVT_API void SetSecondBgColor(const float v[4]);
    MIVT_API void GetSecondBgColor(float v[4]);

    MIVT_API void SetBgColorMode(const std::string& mode);
    MIVT_API std::string GetBgColorMode();

  private:
    std::string getBasePath(const std::string& filename = "") const;
    std::string getProgramPath() const;
    std::string getUserDataPath(const std::string& filename = "") const;
    std::string getResourcePath(const std::string& filename = "") const;
    void initLogging();
    void initTransfunc();

  private:
    tgt::OffScreenRender    *offscreen_;
    tgt::LogManager         *logManager_;
    tgt::Volume             *volume_;
    tgt::TransFunc1D        *transfunc_;
    std::string             transfuncName_;

    RenderVolume            *render_;

    std::string             programPath_;
    std::string             basePath_;
    std::string             userDataPath_;

    static const std::string loggerCat_;
  };

}


