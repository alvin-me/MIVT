#pragma once

namespace tgt {
  class RenderTarget;
  class Shader;
  class Camera;
}

namespace mivt {

  class Raycaster
  {
  public:
    Raycaster();

    ~Raycaster();

    void Initialize();

    void Deinitialize();

    void GetPixels(unsigned char* buffer, int length);

    void Resize(int width, int height);

  private:
    void Process();

  private:
    tgt::RenderTarget     *output_;
    tgt::Shader           *mainShader_;
    tgt::Camera           *camera_;
  };

}


