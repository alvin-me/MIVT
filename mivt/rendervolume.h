#pragma once

#include "volumeraycaster.h"

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
  class Geometry;
  class Trackball;
  class Volume;
}

namespace mivt {

  class RenderColorCube;
  class RenderBackground;
  class RenderToScreen;

  class RenderVolume : public VolumeRaycaster
  {
  public:
    RenderVolume();

    ~RenderVolume();

    void Initialize();

    void Deinitialize();

    void GetPixels(unsigned char* buffer, int length, bool downsampling = false);

    void Resize(const glm::ivec2& newSize);

    void Rotate(const glm::ivec2& newPos, const glm::ivec2& lastPos);

    void Zoom(const glm::ivec2& newPos, const glm::ivec2& lastPos);

    void Pan(const glm::ivec2& newPos, const glm::ivec2& lastPos);

    void SetVolume(tgt::Volume *volume);

    void SetTransfunc(tgt::TransFunc1D *transfunc);

    void SetClassificationMode(const std::string& mode);

  private:
    void Process(bool downsampling);

    /// scale screen-coodinates of mouse to intervall [-1, 1]x[-1, 1]
    glm::vec2 scaleMouse(const glm::ivec2& coords, const glm::ivec2& viewport) const;

  private:
    tgt::RenderTarget     *privatetarget_;
    tgt::RenderTarget     *smallprivatetarget_;
    tgt::RenderTarget     *output_;
    tgt::Shader           *shader_;
    tgt::Camera           *camera_;
    tgt::Geometry         *proxyGeometry_;
    tgt::Trackball        *trackball_;
    tgt::Volume           *volume_;
    tgt::TransFunc1D      *transfunc_;

    RenderColorCube       *renderColorCube_;    
    RenderBackground      *renderBackground_;
    RenderToScreen        *renderToScreen_;
  };

}


