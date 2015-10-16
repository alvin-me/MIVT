#include "matrixstack.h"
#include "tgt_gl.h"

using glm::vec3;
using glm::mat4;

#define MAT_COMPAT_MODE

namespace tgt {

  const std::string MatrixStack::loggerCat_("MatrixStack");

  MatrixStack::MatrixStack() {
    currentStack_ = MODELVIEW;
    modelViewStack_.push(mat4(1));
    projectionStack_.push(mat4(1));
    textureStack_.push(mat4(1));
  }

  MatrixStack::~MatrixStack() {}

  void MatrixStack::loadMatrix(const mat4& m) {
    getStack().top() = m;
#ifdef MAT_COMPAT_MODE
    glLoadMatrixf(glm::value_ptr(getStack().top()));
#endif
  }

  void MatrixStack::multMatrix(const mat4& m) {
    getStack().top() = getStack().top() * m;
#ifdef MAT_COMPAT_MODE
    glMultMatrixf(glm::value_ptr(m));
#endif
  }

  void MatrixStack::loadIdentity() {
    getStack().top() = mat4(1);
#ifdef MAT_COMPAT_MODE
    glLoadMatrixf(glm::value_ptr(getStack().top()));
#endif
  }

  void MatrixStack::pushMatrix() {
    getStack().push(getStack().top());
#ifdef MAT_COMPAT_MODE
    glPushMatrix();
#endif
  }

  void MatrixStack::popMatrix() {
    getStack().pop();
    if (getStack().empty())
      getStack().push(mat4(1));
#ifdef MAT_COMPAT_MODE
    glPopMatrix();
#endif
  }

  void MatrixStack::matrixMode(StackMode c) {
    currentStack_ = c;
#ifdef MAT_COMPAT_MODE
    switch (currentStack_) {
    case MODELVIEW:
      glMatrixMode(GL_MODELVIEW);
      break;
    case PROJECTION:
      glMatrixMode(GL_PROJECTION);
      break;
    case TEXTURE:
      glMatrixMode(GL_TEXTURE);
      break;
    }
#endif
  }

  void MatrixStack::scale(const vec3& factors) {
    multMatrix(glm::scale(factors));
  }

  void MatrixStack::scale(float x, float y, float z) {
    multMatrix(glm::scale(vec3(x, y, z)));
  }

  void MatrixStack::translate(float x, float y, float z) {
    multMatrix(glm::translate(vec3(x, y, z)));
  }

  void MatrixStack::translate(const vec3& trans) {
    multMatrix(glm::translate(trans));
  }

  void MatrixStack::rotate(float angle, const vec3& axis) {
    multMatrix(glm::rotate(glm::deg2rad(angle), axis));
  }

  void MatrixStack::rotate(float angle, float x, float y, float z) {
    multMatrix(glm::rotate(glm::deg2rad(angle), vec3(x, y, z)));
  }

} // end namespace tgt