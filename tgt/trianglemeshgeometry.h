#pragma once

#include "tgt_gl.h"
#include "tgt_math.h"
#include "matrixstack.h"
#include "geometry.h"

#include <vector>

namespace tgt {

  /*
  * Base class for vertices stored in meshes.
  * Most of the methods have to be reimplemented for subclasses.
  * The methods are intentionally not made virtual for performance reasons.
  */
  struct VertexBase {
    glm::vec3 pos_;

    VertexBase() {}
    VertexBase(glm::vec3 pos) : pos_(pos) {}

    /// Compares this vertex to another vertex. Reimplement in subclass.
    bool equals(const VertexBase& other, double epsilon = 1e-5) const;

    /// Sets up vertex attributs pointers for rendering. Reimplement in subclass.
    static void setupVertexAttributePointers(size_t stride = 0);

    /// De-initializes vertex attributes pointers. Reimplement in subclass.
    static void disableVertexAttributePointers();
  };

  //-------------------------------------------------------------------------------------------------

  /// A vertex with an additional vec3 attribute (e.g., texture coordinate).
  struct VertexVec3 : public VertexBase {
    glm::vec3 attr1_;

    VertexVec3() {}
    VertexVec3(glm::vec3 pos, glm::vec3 attr1);

    bool equals(const VertexVec3& other, double epsilon = 1e-5) const;

    static void setupVertexAttributePointers();
    static void disableVertexAttributePointers();
  };

  //-------------------------------------------------------------------------------------------------

  /// A vertex with an additional vec4 and vec3 attribute (e.g., RGBA color and texture coordinate).
  struct VertexVec4Vec3 : public VertexBase {
    glm::vec4 attr1_;
    glm::vec3 attr2_;

    VertexVec4Vec3() {}
    VertexVec4Vec3(glm::vec3 pos, glm::vec4 attr1, glm::vec3 attr2);

    bool equals(const VertexVec4Vec3& other, double epsilon = 1e-5) const;

    static void setupVertexAttributePointers();

    static void disableVertexAttributePointers();
  };

  //------------------------------------------------------------------------------

  /// Generic triangle, parameterized by the vertex-type. (@see VertexBase)
  template <class V>
  struct Triangle {
    V v_[3];

    typedef V VertexType;

    Triangle() {}
    Triangle(V v1, V v2, V v3)
    {
      v_[0] = v1;
      v_[1] = v2;
      v_[2] = v3;
    }
  };

  //------------------------------------------------------------------------------

  /// Base class for all TriangleMeshGeometries.
  class TriangleMeshGeometryBase : public Geometry {
  public:

    enum VertexLayout {
      SIMPLE,
      VEC3,
      VEC4VEC3
    };

    TGT_API TriangleMeshGeometryBase();

    TGT_API virtual size_t getNumTriangles() const = 0;
    TGT_API virtual bool isEmpty() const = 0;
    TGT_API virtual void clear() = 0;
    TGT_API virtual VertexLayout getVertexLayout() const = 0;

  private:
  };

  //------------------------------------------------------------------------------

  /*
  * Generic triangle-mesh, storing a vector of triangles.
  * Template argument is the vertex (not triangle!) type.
  */
  template <class V>
  class TriangleMeshGeometry : public TriangleMeshGeometryBase {
  public:
    typedef Triangle<V> TriangleType;
    typedef V VertexType;

    TriangleMeshGeometry();

    /// Clears the vector and deletes the OpenGL buffer if necessary.
    virtual ~TriangleMeshGeometry();

    virtual size_t getNumTriangles() const;

    virtual bool isEmpty() const;

    virtual void clear();

    /// Adds a triangle to the mesh. Flags the bounding box and OpenGL buffer as invalid.
    void addTriangle(const TriangleType& t);

    const TriangleType& getTriangle(size_t i) const;

    const TriangleType& operator[] (size_t i) const;

    /// Modifies a triangle of the mesh. Flags the bounding box and OpenGL buffer as invalid.
    void setTriangle(const TriangleType& t, size_t i);

    /// Returns the bounding box in model or transformed coordinates. The BB is cached internally.
    virtual BoundingBox getBoundingBox(bool transformed = true) const;

    /// Flags the bounding box and OpenGL buffer as invalid.
    void invalidate();

    /*
    * Renders the mesh using OpenGL by binding the buffer, setting appropriate vertex attribute pointers and calling glDrawArrays.
    *
    * A shader with appropriate vertexattribute bindings has to be activated before calling render().
    */
    virtual void render() const;

    /**
    * Returns true, if the passed Geometry is a TriangleMeshGeometry of the same type and all its vertices are equal to this one's.
    */
    virtual bool equals(const Geometry* geometry, double epsilon = 1e-5) const;

  protected:
    virtual void updateBoundingBox() const;

    /// Updates the OpenGL buffer.
    virtual void updateBuffer() const;

    mutable BoundingBox boundingBox_;
    mutable bool boundingBoxValid_;

    mutable GLuint bufferObject_;
    mutable bool bufferObjectValid_;

    std::vector<TriangleType> triangles_;
  };

  //-------------------------------------------------------------------------------------------------

  /// A triangle mesh with vertex type VertexBase (i.e., only position)
  class TriangleMeshGeometrySimple : public TriangleMeshGeometry<VertexBase> {
  public:
    TGT_API virtual TriangleMeshGeometryBase::VertexLayout getVertexLayout() const { return SIMPLE; }

    TGT_API static TriangleMeshGeometrySimple* createCube(glm::vec3 coordLlf, glm::vec3 coordUrb);
  };

  //-------------------------------------------------------------------------------------------------

  /// A triangle mesh with vertex type VertexVec3.
  class TriangleMeshGeometryVec3 : public TriangleMeshGeometry<VertexVec3> {
  public:
    TGT_API virtual TriangleMeshGeometryBase::VertexLayout getVertexLayout() const { return VEC3; }

    /// Creates a mesh containing a cube specified by two vertices.
    TGT_API static TriangleMeshGeometryVec3* createCube(VertexType llfVertex, VertexType urbVertex);

    /// Adds a cube to this mesh.
    TGT_API void addCube(VertexType llfVertex, VertexType urbVertex);
  protected:
  };

  //-------------------------------------------------------------------------------------------------

  /// A triangle mesh with vertex type VertexVec4Vec3
  class TriangleMeshGeometryVec4Vec3 : public TriangleMeshGeometry<VertexVec4Vec3> {
  public:
    TGT_API virtual TriangleMeshGeometryBase::VertexLayout getVertexLayout() const { return VEC4VEC3; }

    TGT_API static TriangleMeshGeometryVec4Vec3* createCube(glm::vec3 coordLlf, glm::vec3 coordUrb, glm::vec3 colorLlf, glm::vec3 colorUrb, float alpha, glm::vec3 texLlf, glm::vec3 texUrb);

    /// Adds a cube to this mesh.
    TGT_API void addCube(VertexVec3 llfVertex, VertexVec3 urbVertex);

    TGT_API void addQuad(const VertexVec3& v1, const VertexVec3& v2, const VertexVec3& v3, const VertexVec3& v4);

    /// Creates a cube with color and normals:
    TGT_API static TriangleMeshGeometryVec4Vec3* createCube(glm::vec3 coordLlf, glm::vec3 coordUrb, glm::vec3 colorLlf, glm::vec3 colorUrb, float alpha);
  };

  //------------------------------------------------------------------------------

  template <class V>
  TriangleMeshGeometry<V>::TriangleMeshGeometry()
    : boundingBox_()
    , boundingBoxValid_(false)
    , bufferObject_(0)
    , bufferObjectValid_(false)
  {}

  template <class V>
  TriangleMeshGeometry<V>::~TriangleMeshGeometry() {
    clear();

    if (bufferObject_ != 0) {
      glDeleteBuffers(1, &bufferObject_);
      bufferObject_ = 0;
      bufferObjectValid_ = false;
    }
  }

  template <class V>
  size_t TriangleMeshGeometry<V>::getNumTriangles() const {
    return triangles_.size();
  }

  template <class V>
  bool TriangleMeshGeometry<V>::isEmpty() const {
    return (triangles_.size() == 0);
  }

  template <class V>
  void TriangleMeshGeometry<V>::clear() {
    triangles_.clear();

    invalidate();
  }

  template <class V>
  void TriangleMeshGeometry<V>::addTriangle(const TriangleType& t) {
    triangles_.push_back(t);
    invalidate();
  }

  template <class V>
  const typename TriangleMeshGeometry<V>::TriangleType& TriangleMeshGeometry<V>::getTriangle(size_t i) const {
    assert(i < triangles_.size());
    return triangles_[i];
  }

  template <class V>
  const typename TriangleMeshGeometry<V>::TriangleType& TriangleMeshGeometry<V>::operator[] (size_t i) const {
    assert(i < triangles_.size());
    return triangles_[i];
  }

  template <class V>
  void TriangleMeshGeometry<V>::setTriangle(const TriangleType& t, size_t i) {
    assert(i < triangles_.size());
    triangles_[i] = t;

    invalidate();
  }

  template <class V>
  BoundingBox TriangleMeshGeometry<V>::getBoundingBox(bool transformed) const {
    if (!boundingBoxValid_)
      updateBoundingBox();

    if (transformed)
      return boundingBox_.transform(getTransformationMatrix());
    else
      return boundingBox_;
  }

  template <class V>
  void TriangleMeshGeometry<V>::invalidate() {
    boundingBoxValid_ = false;
    bufferObjectValid_ = false;
  }

  template <class V>
  void TriangleMeshGeometry<V>::render() const {
    updateBuffer();

    if (isEmpty())
      return;

    MatStack.matrixMode(MatrixStack::MODELVIEW);
    MatStack.pushMatrix();
    MatStack.multMatrix(getTransformationMatrix());

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject_);

    VertexType::setupVertexAttributePointers();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangles_.size() * 3));
    VertexType::disableVertexAttributePointers();

    MatStack.popMatrix();
  }

  template <class V>
  bool TriangleMeshGeometry<V>::equals(const Geometry* geometry, double epsilon) const {
    const TriangleMeshGeometry<V>* triMesh = dynamic_cast<const TriangleMeshGeometry<V>*>(geometry);

    if (triMesh) {
      if (getNumTriangles() != triMesh->getNumTriangles())
        return false;

      for (size_t i = 0; i<triangles_.size(); i++) {
        if (!triangles_[i].v_[0].equals(triMesh->triangles_[i].v_[0], epsilon) ||
          !triangles_[i].v_[1].equals(triMesh->triangles_[i].v_[1], epsilon) ||
          !triangles_[i].v_[2].equals(triMesh->triangles_[i].v_[2], epsilon))
        {
          return false;
        }
      }
      return true;
    }
    else
      return false;
  }

  template <class V>
  void TriangleMeshGeometry<V>::updateBoundingBox() const {
    BoundingBox bb;
    for (size_t i = 0; i<triangles_.size(); i++) {
      bb.addPoint(triangles_[i].v_[0].pos_);
      bb.addPoint(triangles_[i].v_[1].pos_);
      bb.addPoint(triangles_[i].v_[2].pos_);
    }
    boundingBox_ = bb;
    boundingBoxValid_ = true;
  }

  template <class V>
  void TriangleMeshGeometry<V>::updateBuffer() const {
    if (bufferObjectValid_)
      return;
    else if (bufferObject_ != 0) {
      glDeleteBuffers(1, &bufferObject_);
      bufferObject_ = 0;
      bufferObjectValid_ = false;
    }

    if (isEmpty())
      return;

    glGenBuffers(1, &bufferObject_);

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleType)* triangles_.size(), &triangles_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    bufferObjectValid_ = true;
  }

} // end namespace tgt