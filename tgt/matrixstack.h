#pragma once

#include "tgt_math.h"
#include "singleton.h"
#include "config.h"

#include <stack>

namespace tgt {

  class MatrixStack : public Singleton<MatrixStack> {
  public:

    enum StackMode {
      MODELVIEW,
      PROJECTION,
      TEXTURE
    };

    TGT_API MatrixStack();
    TGT_API virtual ~MatrixStack();

    TGT_API void loadMatrix(const glm::mat4& m);
    TGT_API void multMatrix(const glm::mat4& m);
    TGT_API void loadIdentity();
    TGT_API void pushMatrix();
    TGT_API void popMatrix();
    TGT_API void matrixMode(StackMode c);

    TGT_API void scale(const glm::vec3& factors);
    TGT_API void scale(float x, float y, float z);

    TGT_API void translate(const glm::vec3& trans);
    TGT_API void translate(float x, float y, float z);

    TGT_API void rotate(float angle, const glm::vec3& axis);
    TGT_API void rotate(float angle, float x, float y, float z);

    TGT_API glm::mat4 getModelViewMatrix() const {
      return modelViewStack_.top();
    }
    TGT_API glm::mat4 getProjectionMatrix() const {
      return projectionStack_.top();
    }
    TGT_API glm::mat4 getTextureMatrix() const {
      return textureStack_.top();
    }

  private:
    static const std::string loggerCat_;

    std::stack<glm::mat4> modelViewStack_;
    std::stack<glm::mat4> projectionStack_;
    std::stack<glm::mat4> textureStack_;

    StackMode currentStack_;
    std::stack<glm::mat4>& getStack() {
      switch (currentStack_) {
      case MODELVIEW:
        return modelViewStack_;
      case PROJECTION:
        return projectionStack_;
      case TEXTURE:
        return textureStack_;
      default:
        assert(false);
        return modelViewStack_;
      }
    }
  };

} // end namespace tgt

#define MatStack tgt::Singleton<tgt::MatrixStack>::getRef()
