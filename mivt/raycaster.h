#pragma once
#include "tgt_math.h"

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
  class Geometry;
  class Trackball;
}

namespace mivt {

  class ColorCubeRender;
  class RenderToScreen;
  class Raycaster
  {
  public:
    Raycaster();

    ~Raycaster();

    void Initialize();

    void Deinitialize();

    void GetPixels(unsigned char* buffer, int length);

    void Resize(const glm::ivec2& newSize);

    void Rotate(const glm::ivec2& newPos, const glm::ivec2& lastPos);

    void Zoom(const glm::ivec2& newPos, const glm::ivec2& lastPos);

    void Pan(const glm::ivec2& newPos, const glm::ivec2& lastPos);

  private:
    void Process();

    /// scale screen-coodinates of mouse to intervall [-1, 1]x[-1, 1]
    glm::vec2 scaleMouse(const glm::ivec2& coords, const glm::ivec2& viewport) const;

  private:
    tgt::RenderTarget     *output_;
    tgt::Shader           *mainShader_;
    tgt::Camera           *camera_;
    tgt::Geometry         *proxyGeometry_;
    tgt::Trackball        *trackball_;

    ColorCubeRender       *colorCubeRender_;
    RenderToScreen        *renderToScreen_;
  };

}


