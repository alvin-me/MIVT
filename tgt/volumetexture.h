#pragma once

#include "texture.h"

namespace tgt {

  /**
  * This class is basically a Texture.
  */
  class VolumeTexture : public Texture {
  public:
    /**
    * With data and internalformat argument, type_ is set to GL_TEXTURE_3D
    * and \e NO new chunk of data will be allocated on the heap.
    */
    TGT_API VolumeTexture(const GLubyte* data, const glm::ivec3& dimensions,
      GLint format,
      GLint internalformat,
      GLenum dataType,
      Texture::Filter filter);
  };

} // end namespace tgt