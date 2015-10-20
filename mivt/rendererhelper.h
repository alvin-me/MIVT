#pragma once
#include "tgt_math.h"

namespace tgt {
  class Shader;
  class Camera;
}

namespace mivt {
  class RendererHelper
  {
  public:
    RendererHelper();
    ~RendererHelper();

    /**
    * Sets some uniforms potentially needed by every shader.
    * @note This function should be called for every shader before every rendering pass!
    *
    * @param shader the shader to set up
    * @param camera camera whose position is passed to uniform cameraPosition_, also needed for passing matrices
    * @param screenDim dimensions of the render target's viewport
    */
    static void setGlobalShaderParameters(tgt::Shader* shader, const tgt::Camera* camera, glm::ivec2 screenDim);
  };
}


