#include "rendererhelper.h"
#include "shadermanager.h"
#include "camera.h"

namespace mivt {
  RendererHelper::RendererHelper()
  {
  }

  RendererHelper::~RendererHelper()
  {
  }

  void RendererHelper::setGlobalShaderParameters(tgt::Shader* shader, const tgt::Camera* camera, glm::ivec2 screenDim) {
    shader->setIgnoreUniformLocationError(true);

    shader->setUniform("screenDim_", glm::vec2(screenDim));
    shader->setUniform("screenDimRCP_", 1.f / glm::vec2(screenDim));
    LGL_ERROR;

    // camera position in world coordinates, and corresponding transformation matrices
    if (camera) {
      shader->setUniform("cameraPosition_", camera->getPosition());
      shader->setUniform("viewMatrix_", camera->getViewMatrix());
      shader->setUniform("projectionMatrix_", camera->getProjectionMatrix(screenDim));
      glm::mat4 viewInvert = glm::inverse(camera->getViewMatrix());
      shader->setUniform("viewMatrixInverse_", viewInvert);
      glm::mat4 projInvert = glm::inverse(camera->getProjectionMatrix(screenDim));
      shader->setUniform("projectionMatrixInverse_", projInvert);
    }

    shader->setIgnoreUniformLocationError(false);

    LGL_ERROR;
  }
}

