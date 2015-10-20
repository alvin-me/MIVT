#pragma once
#include "tgt_gl.h"
#include "tgt_math.h"

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
  class Geometry;
}

namespace mivt {
  class ColorCubeRender
  {
  public:
    ColorCubeRender();
    ~ColorCubeRender();

    void Initialize();
    void Deinitialize();
    void Process(tgt::Geometry *geometry, tgt::Camera *camera);
    void Resize(const glm::ivec2& size);

    tgt::RenderTarget* GetFrontFace();
    tgt::RenderTarget* GetBackFace();

  private:
    void renderGeometry(const tgt::Geometry* geometry,
      tgt::Camera *camera,
      tgt::RenderTarget* outport,
      GLenum depthFunc,
      float clearDepth,
      GLenum cullFace);

  private:
    tgt::RenderTarget  *frontFace_;
    tgt::RenderTarget  *backFace_;
    tgt::Shader        *shader_;
  };
}


