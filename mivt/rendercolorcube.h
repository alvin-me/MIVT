#pragma once
#include "tgt_gl.h"
#include "renderbase.h"

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
  class Geometry;
}

namespace mivt {
  class RenderColorCube : public RenderBase
  {
  public:
    RenderColorCube();
    ~RenderColorCube();

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

    void fillEntryPoints(tgt::RenderTarget *firstBackTarget, 
      tgt::RenderTarget *firstFrontTarget, 
      tgt::RenderTarget *output, 
      const tgt::Geometry* geometry,
      tgt::Camera *camera);

  private:
    tgt::RenderTarget  *frontTarget_;
    tgt::RenderTarget  *backTarget_;
    tgt::RenderTarget  *tmpTarget_;
    tgt::Shader        *shader_;
    tgt::Shader        *shaderInsideVolume_;
  };
}


