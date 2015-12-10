#pragma once

#include "config.h"
#include "progressbar.h"
#include <string>
#include <vector>

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
    MIVT_API Application(bool useOffScreenRender = false);

    MIVT_API ~Application();

    MIVT_API void Initialize(bool useOffScreenRender);

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

    MIVT_API void SaveToImage(const std::string& filename);
    MIVT_API void SaveToImage();
    MIVT_API void SaveToImage(const std::string& filename, int width, int height);
    MIVT_API void SaveToImage(int width, int height);

    MIVT_API void ChangeClipRight(float val);
    MIVT_API void ChangeClipLeft(float val);
    MIVT_API void ChangeClipBack(float val);
    MIVT_API void ChangeClipFront(float val);
    MIVT_API void ChangeClipBottom(float val);
    MIVT_API void ChangeClipTop(float val);
    MIVT_API void resetClipPlanes();
    MIVT_API void EnableClip(bool flag);
    MIVT_API void getClipMaximum(int val[3]);
    MIVT_API float GetClipRight();
    MIVT_API float GetClipLeft();
    MIVT_API float GetClipBack();
    MIVT_API float GetClipFront();
    MIVT_API float GetClipBottom();
    MIVT_API float GetClipTop();
    MIVT_API bool IsClipEnabled();

    MIVT_API void DoSculpt(const std::vector<glm::vec2> & polygon);

    MIVT_API void getWindowingDomain(float val[2]);
    MIVT_API void setWindowingDomain(float val[2]);

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


