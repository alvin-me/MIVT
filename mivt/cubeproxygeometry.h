#pragma once
#include <string>

namespace tgt {
  class Volume;
  class Geometry;
}

namespace mivt {

  class CubeProxyGeometry
  {
  public:
    CubeProxyGeometry();
    ~CubeProxyGeometry();

    void SetVolume(tgt::Volume *volume);

    void Process();

    void ChangeClipRight(float val);
    void ChangeClipLeft(float val);
    void ChangeClipBack(float val);
    void ChangeClipFront(float val);
    void ChangeClipBottom(float val);
    void ChangeClipTop(float val);
    void EnableClip(bool flag);

    float GetClipRight();
    float GetClipLeft();
    float GetClipBack();
    float GetClipFront();
    float GetClipBottom();
    float GetClipTop();
    bool IsClipEnabled();

    void resetClipPlanes();

    tgt::Geometry* GetGeometry();

  private:
    /// Adapt ranges of clip plane properties to the input volume's dimensions.
    void adjustClipPropertiesRanges();

  private:
    bool enableClipping_;    ///< Clipping enable / disable property.
    float clipRight_;        ///< Right clipping plane position (x).
    float clipLeft_;         ///< Left clipping plane position (-x).
    float clipFront_;        ///< Front clipping plane position (y).
    float clipBack_;         ///< Back clipping plane position property (-y).
    float clipBottom_;       ///< Bottom clipping plane position property (z).
    float clipTop_;          ///< Top clipping plane position property (-z).

    tgt::Volume *volume_;
    tgt::Geometry *geometry_;

    static const std::string loggerCat_;
  };

}

