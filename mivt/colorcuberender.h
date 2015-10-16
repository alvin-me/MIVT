#pragma once

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
}

class ColorCubeRender
{
public:
  ColorCubeRender();
  ~ColorCubeRender();

//  void Initialize();
//  void Deinitialize();
//  void Process(Geometry *geometry, Camera *camera);
//  void Resize(const glm::ivec2& size);
//
//  RenderTarget* GetFrontFace();
//  RenderTarget* GetBackFace();
//
//private:
//  void renderGeometry(const Geometry* geometry,
//    Camera *camera,
//    RenderTarget* outport,
//    GLenum depthFunc,
//    float clearDepth,
//    GLenum cullFace);

private:
  tgt::RenderTarget  *frontFace_;
  tgt::RenderTarget  *backFace_;
  tgt::Shader        *shader_;
};

