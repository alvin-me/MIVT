#include "trianglemeshgeometry.h"

namespace tgt {

  bool VertexBase::equals(const VertexBase& other, double epsilon) const {
    if (glm::distance(pos_, other.pos_) > epsilon)
      return false;
    else
      return true;
  }

  void VertexBase::setupVertexAttributePointers(size_t stride) {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), 0);
  }

  void VertexBase::disableVertexAttributePointers() {
    glDisableVertexAttribArray(0);
  }

  //-------------------------------------------------------------------------------------------------

  VertexVec3::VertexVec3(glm::vec3 pos, glm::vec3 attr1)
    : VertexBase(pos)
    , attr1_(attr1)
  {}

  bool VertexVec3::equals(const VertexVec3& other, double epsilon) const {
    if (glm::distance(pos_, other.pos_) > epsilon)
      return false;

    if (glm::distance(attr1_, other.attr1_) > epsilon)
      return false;

    return true;
  }

  void VertexVec3::setupVertexAttributePointers() {
    VertexBase::setupVertexAttributePointers(sizeof(VertexVec3));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVec3), (void*)sizeof(glm::vec3));
  }

  void VertexVec3::disableVertexAttributePointers() {
    VertexBase::disableVertexAttributePointers();

    glDisableVertexAttribArray(1);
  }

  //-------------------------------------------------------------------------------------------------

  VertexVec4Vec3::VertexVec4Vec3(glm::vec3 pos, glm::vec4 attr1, glm::vec3 attr2)
    : VertexBase(pos)
    , attr1_(attr1)
    , attr2_(attr2)
  {}

  bool VertexVec4Vec3::equals(const VertexVec4Vec3& other, double epsilon) const {
    if (glm::distance(pos_, other.pos_) > epsilon)
      return false;

    if (glm::distance(attr1_, other.attr1_) > epsilon)
      return false;

    if (glm::distance(attr2_, other.attr2_) > epsilon)
      return false;

    return true;
  }

  void VertexVec4Vec3::setupVertexAttributePointers() {
    VertexBase::setupVertexAttributePointers(sizeof(VertexVec4Vec3));

    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexVec4Vec3), (void*)sizeof(glm::vec3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexVec4Vec3), (void*)(sizeof(glm::vec3) + sizeof(glm::vec4)));
  }

  void VertexVec4Vec3::disableVertexAttributePointers() {
    VertexBase::disableVertexAttributePointers();

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  //------------------------------------------------------------------------------

  TriangleMeshGeometryBase::TriangleMeshGeometryBase()
    : Geometry()
  {}

  //------------------------------------------------------------------------------

  TriangleMeshGeometrySimple* TriangleMeshGeometrySimple::createCube(glm::vec3 coordLlf, glm::vec3 coordUrb) {
    // expecting coordLlf < coordUrb
    if (coordLlf.x > coordUrb.x) {
      std::swap(coordLlf.x, coordUrb.x);
    }
    if (coordLlf.y > coordUrb.y) {
      std::swap(coordLlf.y, coordUrb.y);
    }
    if (coordLlf.z > coordUrb.z) {
      std::swap(coordLlf.z, coordUrb.z);
    }

    TriangleMeshGeometrySimple* ret = new TriangleMeshGeometrySimple();

    VertexBase llf(glm::vec3(coordLlf.x, coordLlf.y, coordLlf.z));
    VertexBase lrf(glm::vec3(coordUrb.x, coordLlf.y, coordLlf.z));
    VertexBase lrb(glm::vec3(coordUrb.x, coordLlf.y, coordUrb.z));
    VertexBase llb(glm::vec3(coordLlf.x, coordLlf.y, coordUrb.z));

    VertexBase ulb(glm::vec3(coordLlf.x, coordUrb.y, coordUrb.z));
    VertexBase ulf(glm::vec3(coordLlf.x, coordUrb.y, coordLlf.z));
    VertexBase urf(glm::vec3(coordUrb.x, coordUrb.y, coordLlf.z));
    VertexBase urb(glm::vec3(coordUrb.x, coordUrb.y, coordUrb.z));

    ret->addTriangle(TriangleType(urb, urf, ulf));
    ret->addTriangle(TriangleType(urb, ulf, ulb));

    ret->addTriangle(TriangleType(llf, ulf, urf));
    ret->addTriangle(TriangleType(llf, urf, lrf));

    ret->addTriangle(TriangleType(llf, llb, ulb));
    ret->addTriangle(TriangleType(llf, ulb, ulf));

    ret->addTriangle(TriangleType(urb, ulb, llb));
    ret->addTriangle(TriangleType(urb, llb, lrb));

    ret->addTriangle(TriangleType(urb, lrb, lrf));
    ret->addTriangle(TriangleType(urb, lrf, urf));

    ret->addTriangle(TriangleType(llf, lrf, lrb));
    ret->addTriangle(TriangleType(llf, lrb, llb));

    return ret;
  }

  //------------------------------------------------------------------------------

  TriangleMeshGeometryVec3* TriangleMeshGeometryVec3::createCube(VertexType llfVertex, VertexType urbVertex) {
    TriangleMeshGeometryVec3* ret = new TriangleMeshGeometryVec3();
    ret->addCube(llfVertex, urbVertex);
    return ret;
  }

  void TriangleMeshGeometryVec3::addCube(VertexType llfVertex, VertexType urbVertex) {
    // expecting llfVertex.pos_ < urbVertex.pos_
    if (llfVertex.pos_.x > urbVertex.pos_.x) {
      std::swap(llfVertex.pos_.x, urbVertex.pos_.x);
      std::swap(llfVertex.attr1_.x, urbVertex.attr1_.x);
    }
    if (llfVertex.pos_.y > urbVertex.pos_.y) {
      std::swap(llfVertex.pos_.y, urbVertex.pos_.y);
      std::swap(llfVertex.attr1_.y, urbVertex.attr1_.y);
    }
    if (llfVertex.pos_.z > urbVertex.pos_.z) {
      std::swap(llfVertex.pos_.z, urbVertex.pos_.z);
      std::swap(llfVertex.attr1_.z, urbVertex.attr1_.z);
    }

    VertexVec3 llf(glm::vec3(llfVertex.pos_.x, llfVertex.pos_.y, llfVertex.pos_.z), glm::vec3(llfVertex.attr1_.x, llfVertex.attr1_.y, llfVertex.attr1_.z));
    VertexVec3 lrf(glm::vec3(urbVertex.pos_.x, llfVertex.pos_.y, llfVertex.pos_.z), glm::vec3(urbVertex.attr1_.x, llfVertex.attr1_.y, llfVertex.attr1_.z));
    VertexVec3 lrb(glm::vec3(urbVertex.pos_.x, llfVertex.pos_.y, urbVertex.pos_.z), glm::vec3(urbVertex.attr1_.x, llfVertex.attr1_.y, urbVertex.attr1_.z));
    VertexVec3 llb(glm::vec3(llfVertex.pos_.x, llfVertex.pos_.y, urbVertex.pos_.z), glm::vec3(llfVertex.attr1_.x, llfVertex.attr1_.y, urbVertex.attr1_.z));

    VertexVec3 ulb(glm::vec3(llfVertex.pos_.x, urbVertex.pos_.y, urbVertex.pos_.z), glm::vec3(llfVertex.attr1_.x, urbVertex.attr1_.y, urbVertex.attr1_.z));
    VertexVec3 ulf(glm::vec3(llfVertex.pos_.x, urbVertex.pos_.y, llfVertex.pos_.z), glm::vec3(llfVertex.attr1_.x, urbVertex.attr1_.y, llfVertex.attr1_.z));
    VertexVec3 urf(glm::vec3(urbVertex.pos_.x, urbVertex.pos_.y, llfVertex.pos_.z), glm::vec3(urbVertex.attr1_.x, urbVertex.attr1_.y, llfVertex.attr1_.z));
    VertexVec3 urb(glm::vec3(urbVertex.pos_.x, urbVertex.pos_.y, urbVertex.pos_.z), glm::vec3(urbVertex.attr1_.x, urbVertex.attr1_.y, urbVertex.attr1_.z));

    addTriangle(TriangleType(urb, urf, ulf));
    addTriangle(TriangleType(urb, ulf, ulb));

    addTriangle(TriangleType(llf, ulf, urf));
    addTriangle(TriangleType(llf, urf, lrf));

    addTriangle(TriangleType(llf, llb, ulb));
    addTriangle(TriangleType(llf, ulb, ulf));

    addTriangle(TriangleType(urb, ulb, llb));
    addTriangle(TriangleType(urb, llb, lrb));

    addTriangle(TriangleType(urb, lrb, lrf));
    addTriangle(TriangleType(urb, lrf, urf));

    addTriangle(TriangleType(llf, lrf, lrb));
    addTriangle(TriangleType(llf, lrb, llb));
  }

  //------------------------------------------------------------------------------

  TriangleMeshGeometryVec4Vec3* TriangleMeshGeometryVec4Vec3::createCube(glm::vec3 coordLlf, glm::vec3 coordUrb, glm::vec3 colorLlf, glm::vec3 colorUrb, float alpha, glm::vec3 texLlf, glm::vec3 texUrb) {
    // expecting coordLlf < coordUrb
    if (coordLlf.x > coordUrb.x) {
      std::swap(coordLlf.x, coordUrb.x);
      std::swap(texLlf.x, texUrb.x);
      std::swap(colorLlf.x, colorUrb.x);
    }
    if (coordLlf.y > coordUrb.y) {
      std::swap(coordLlf.y, coordUrb.y);
      std::swap(texLlf.y, texUrb.y);
      std::swap(colorLlf.y, colorUrb.y);
    }
    if (coordLlf.z > coordUrb.z) {
      std::swap(coordLlf.z, coordUrb.z);
      std::swap(texLlf.z, texUrb.z);
      std::swap(colorLlf.z, colorUrb.z);
    }

    TriangleMeshGeometryVec4Vec3* ret = new TriangleMeshGeometryVec4Vec3();

    VertexVec4Vec3 llf(glm::vec3(coordLlf.x, coordLlf.y, coordLlf.z), glm::vec4(colorLlf.x, colorLlf.y, colorLlf.z, alpha), glm::vec3(texLlf.x, texLlf.y, texLlf.z));
    VertexVec4Vec3 lrf(glm::vec3(coordUrb.x, coordLlf.y, coordLlf.z), glm::vec4(colorUrb.x, colorLlf.y, colorLlf.z, alpha), glm::vec3(texUrb.x, texLlf.y, texLlf.z));
    VertexVec4Vec3 lrb(glm::vec3(coordUrb.x, coordLlf.y, coordUrb.z), glm::vec4(colorUrb.x, colorLlf.y, colorUrb.z, alpha), glm::vec3(texUrb.x, texLlf.y, texUrb.z));
    VertexVec4Vec3 llb(glm::vec3(coordLlf.x, coordLlf.y, coordUrb.z), glm::vec4(colorLlf.x, colorLlf.y, colorUrb.z, alpha), glm::vec3(texLlf.x, texLlf.y, texUrb.z));

    VertexVec4Vec3 ulb(glm::vec3(coordLlf.x, coordUrb.y, coordUrb.z), glm::vec4(colorLlf.x, colorUrb.y, colorUrb.z, alpha), glm::vec3(texLlf.x, texUrb.y, texUrb.z));
    VertexVec4Vec3 ulf(glm::vec3(coordLlf.x, coordUrb.y, coordLlf.z), glm::vec4(colorLlf.x, colorUrb.y, colorLlf.z, alpha), glm::vec3(texLlf.x, texUrb.y, texLlf.z));
    VertexVec4Vec3 urf(glm::vec3(coordUrb.x, coordUrb.y, coordLlf.z), glm::vec4(colorUrb.x, colorUrb.y, colorLlf.z, alpha), glm::vec3(texUrb.x, texUrb.y, texLlf.z));
    VertexVec4Vec3 urb(glm::vec3(coordUrb.x, coordUrb.y, coordUrb.z), glm::vec4(colorUrb.x, colorUrb.y, colorUrb.z, alpha), glm::vec3(texUrb.x, texUrb.y, texUrb.z));

    ret->addTriangle(TriangleType(urb, urf, ulf));
    ret->addTriangle(TriangleType(urb, ulf, ulb));

    ret->addTriangle(TriangleType(llf, ulf, urf));
    ret->addTriangle(TriangleType(llf, urf, lrf));

    ret->addTriangle(TriangleType(llf, llb, ulb));
    ret->addTriangle(TriangleType(llf, ulb, ulf));

    ret->addTriangle(TriangleType(urb, ulb, llb));
    ret->addTriangle(TriangleType(urb, llb, lrb));

    ret->addTriangle(TriangleType(urb, lrb, lrf));
    ret->addTriangle(TriangleType(urb, lrf, urf));

    ret->addTriangle(TriangleType(llf, lrf, lrb));
    ret->addTriangle(TriangleType(llf, lrb, llb));

    return ret;
  }

  TriangleMeshGeometryVec4Vec3* TriangleMeshGeometryVec4Vec3::createCube(glm::vec3 coordLlf, glm::vec3 coordUrb, glm::vec3 colorLlf, glm::vec3 colorUrb, float alpha) {
    // expecting coordLlf < coordUrb
    if (coordLlf.x > coordUrb.x) {
      std::swap(coordLlf.x, coordUrb.x);
      std::swap(colorLlf.x, colorUrb.x);
    }
    if (coordLlf.y > coordUrb.y) {
      std::swap(coordLlf.y, coordUrb.y);
      std::swap(colorLlf.y, colorUrb.y);
    }
    if (coordLlf.z > coordUrb.z) {
      std::swap(coordLlf.z, coordUrb.z);
      std::swap(colorLlf.z, colorUrb.z);
    }

    TriangleMeshGeometryVec4Vec3* ret = new TriangleMeshGeometryVec4Vec3();

    VertexVec4Vec3 llf(glm::vec3(coordLlf.x, coordLlf.y, coordLlf.z), glm::vec4(colorLlf.x, colorLlf.y, colorLlf.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 lrf(glm::vec3(coordUrb.x, coordLlf.y, coordLlf.z), glm::vec4(colorUrb.x, colorLlf.y, colorLlf.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 lrb(glm::vec3(coordUrb.x, coordLlf.y, coordUrb.z), glm::vec4(colorUrb.x, colorLlf.y, colorUrb.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 llb(glm::vec3(coordLlf.x, coordLlf.y, coordUrb.z), glm::vec4(colorLlf.x, colorLlf.y, colorUrb.z, alpha), glm::vec3(0.0f));

    VertexVec4Vec3 ulb(glm::vec3(coordLlf.x, coordUrb.y, coordUrb.z), glm::vec4(colorLlf.x, colorUrb.y, colorUrb.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 ulf(glm::vec3(coordLlf.x, coordUrb.y, coordLlf.z), glm::vec4(colorLlf.x, colorUrb.y, colorLlf.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 urf(glm::vec3(coordUrb.x, coordUrb.y, coordLlf.z), glm::vec4(colorUrb.x, colorUrb.y, colorLlf.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 urb(glm::vec3(coordUrb.x, coordUrb.y, coordUrb.z), glm::vec4(colorUrb.x, colorUrb.y, colorUrb.z, alpha), glm::vec3(0.0f));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 1.0f, 0.0f);
    ret->addTriangle(TriangleType(urb, urf, ulf));
    ret->addTriangle(TriangleType(urb, ulf, ulb));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 0.0f, -1.0f);
    ret->addTriangle(TriangleType(llf, ulf, urf));
    ret->addTriangle(TriangleType(llf, urf, lrf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(-1.0f, 0.0f, 0.0f);
    ret->addTriangle(TriangleType(llf, llb, ulb));
    ret->addTriangle(TriangleType(llf, ulb, ulf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 0.0f, 1.0f);
    ret->addTriangle(TriangleType(urb, ulb, llb));
    ret->addTriangle(TriangleType(urb, llb, lrb));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(1.0f, 0.0f, 0.0f);
    ret->addTriangle(TriangleType(urb, lrb, lrf));
    ret->addTriangle(TriangleType(urb, lrf, urf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, -1.0f, 0.0f);
    ret->addTriangle(TriangleType(llf, lrf, lrb));
    ret->addTriangle(TriangleType(llf, lrb, llb));

    return ret;
  }

  void TriangleMeshGeometryVec4Vec3::addCube(VertexVec3 llfVertex, VertexVec3 urbVertex) {
    // expecting llfVertex.pos_ < urbVertex.pos_
    if (llfVertex.pos_.x > urbVertex.pos_.x) {
      std::swap(llfVertex.pos_.x, urbVertex.pos_.x);
      std::swap(llfVertex.attr1_.x, urbVertex.attr1_.x);
    }
    if (llfVertex.pos_.y > urbVertex.pos_.y) {
      std::swap(llfVertex.pos_.y, urbVertex.pos_.y);
      std::swap(llfVertex.attr1_.y, urbVertex.attr1_.y);
    }
    if (llfVertex.pos_.z > urbVertex.pos_.z) {
      std::swap(llfVertex.pos_.z, urbVertex.pos_.z);
      std::swap(llfVertex.attr1_.z, urbVertex.attr1_.z);
    }

    float alpha = 1.0f;

    VertexVec4Vec3 llf(glm::vec3(llfVertex.pos_.x, llfVertex.pos_.y, llfVertex.pos_.z), glm::vec4(llfVertex.attr1_.x, llfVertex.attr1_.y, llfVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 lrf(glm::vec3(urbVertex.pos_.x, llfVertex.pos_.y, llfVertex.pos_.z), glm::vec4(urbVertex.attr1_.x, llfVertex.attr1_.y, llfVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 lrb(glm::vec3(urbVertex.pos_.x, llfVertex.pos_.y, urbVertex.pos_.z), glm::vec4(urbVertex.attr1_.x, llfVertex.attr1_.y, urbVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 llb(glm::vec3(llfVertex.pos_.x, llfVertex.pos_.y, urbVertex.pos_.z), glm::vec4(llfVertex.attr1_.x, llfVertex.attr1_.y, urbVertex.attr1_.z, alpha), glm::vec3(0.0f));

    VertexVec4Vec3 ulb(glm::vec3(llfVertex.pos_.x, urbVertex.pos_.y, urbVertex.pos_.z), glm::vec4(llfVertex.attr1_.x, urbVertex.attr1_.y, urbVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 ulf(glm::vec3(llfVertex.pos_.x, urbVertex.pos_.y, llfVertex.pos_.z), glm::vec4(llfVertex.attr1_.x, urbVertex.attr1_.y, llfVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 urf(glm::vec3(urbVertex.pos_.x, urbVertex.pos_.y, llfVertex.pos_.z), glm::vec4(urbVertex.attr1_.x, urbVertex.attr1_.y, llfVertex.attr1_.z, alpha), glm::vec3(0.0f));
    VertexVec4Vec3 urb(glm::vec3(urbVertex.pos_.x, urbVertex.pos_.y, urbVertex.pos_.z), glm::vec4(urbVertex.attr1_.x, urbVertex.attr1_.y, urbVertex.attr1_.z, alpha), glm::vec3(0.0f));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 1.0f, 0.0f);
    addTriangle(TriangleType(urb, urf, ulf));
    addTriangle(TriangleType(urb, ulf, ulb));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 0.0f, -1.0f);
    addTriangle(TriangleType(llf, ulf, urf));
    addTriangle(TriangleType(llf, urf, lrf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(-1.0f, 0.0f, 0.0f);
    addTriangle(TriangleType(llf, llb, ulb));
    addTriangle(TriangleType(llf, ulb, ulf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, 0.0f, 1.0f);
    addTriangle(TriangleType(urb, ulb, llb));
    addTriangle(TriangleType(urb, llb, lrb));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(1.0f, 0.0f, 0.0f);
    addTriangle(TriangleType(urb, lrb, lrf));
    addTriangle(TriangleType(urb, lrf, urf));

    llf.attr2_ = lrf.attr2_ = lrb.attr2_ = llb.attr2_ = ulb.attr2_ = ulf.attr2_ = urf.attr2_ = urb.attr2_ = glm::vec3(0.0f, -1.0f, 0.0f);
    addTriangle(TriangleType(llf, lrf, lrb));
    addTriangle(TriangleType(llf, lrb, llb));
  }

  void TriangleMeshGeometryVec4Vec3::addQuad(const VertexVec3& v1, const VertexVec3& v2, const VertexVec3& v3, const VertexVec3& v4) {
    glm::vec3 a = v2.pos_ - v1.pos_;
    glm::vec3 b = v3.pos_ - v1.pos_;
    glm::vec3 n = glm::normalize(glm::cross(a, b));
    float alpha = 1.0f;

    addTriangle(TriangleType(VertexVec4Vec3(v1.pos_, glm::vec4(v1.attr1_, alpha), n), VertexVec4Vec3(v2.pos_, glm::vec4(v2.attr1_, alpha), n), VertexVec4Vec3(v3.pos_, glm::vec4(v3.attr1_, alpha), n)));
    addTriangle(TriangleType(VertexVec4Vec3(v1.pos_, glm::vec4(v1.attr1_, alpha), n), VertexVec4Vec3(v3.pos_, glm::vec4(v3.attr1_, alpha), n), VertexVec4Vec3(v4.pos_, glm::vec4(v4.attr1_, alpha), n)));
  }

} // end namespace tgt