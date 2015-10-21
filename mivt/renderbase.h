#pragma once

#include "tgt_math.h"

namespace tgt {
  class Shader;
  class Camera;
}

namespace mivt {
  class RenderBase
  {
  public:
    RenderBase();
    virtual ~RenderBase();

  protected:
    /**
    * Sets some uniforms potentially needed by every shader.
    * @note This function should be called for every shader before every rendering pass!
    *
    * @param shader the shader to set up
    * @param camera camera whose position is passed to uniform cameraPosition_, also needed for passing matrices
    * @param screenDim dimensions of the render target's viewport
    */
    virtual void setGlobalShaderParameters(tgt::Shader* shader, const tgt::Camera* camera, glm::ivec2 screenDim);

    /// Renders a screen-aligned quad with depth func GL_ALWAYS.
    virtual void renderQuad();

  protected:
    static const std::string loggerCat_;
  };
}


