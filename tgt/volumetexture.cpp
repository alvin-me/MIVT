#include "volumetexture.h"

namespace tgt {

  VolumeTexture::VolumeTexture(const GLubyte* data, const glm::ivec3& dimensions,
    GLint format,
    GLint internalformat,
    GLenum dataType,
    Texture::Filter filter)
    : Texture(const_cast<GLubyte*>(data), dimensions, format, internalformat, dataType, filter)
  {
    assert(glm::all(glm::greaterThan(dimensions, glm::ivec3(1))));

    // Always set to a 3d texture
    setType(GL_TEXTURE_3D);
  }

} // end namespace tgt