#pragma once
#include "tgt_math.h"

namespace tgt {
  class RenderTarget;
  class Shader;
}

namespace mivt {
  class RenderToScreen
  {
  public:
    RenderToScreen();
    ~RenderToScreen();

    void Initialize();
    void Deinitialize();
    void Process(tgt::RenderTarget *input);
    void Resize(const glm::ivec2& newSize);

  private:
    /// Renders a screen-aligned quad with depth func GL_ALWAYS.
    void RenderQuad();

  private:
    tgt::Shader *shader_;
    glm::ivec2  size_;
  };
}


