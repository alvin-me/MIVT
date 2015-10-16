#pragma once

#include "tgt_gl.h"
#include <map>

namespace tgt {

  class Texture;

  class FramebufferObject {
  public:
    TGT_API FramebufferObject();
    TGT_API virtual ~FramebufferObject();

    TGT_API void activate();
    TGT_API static void deactivate();

    TGT_API bool isComplete() const;

    TGT_API bool isActive() const;

    /// Bind a texture to the "attachment" point of this FBO
    TGT_API void attachTexture(Texture* texture,
      GLenum attachment = GL_COLOR_ATTACHMENT0_EXT,
      int mipLevel = 0,
      int zSlice = 0);

    TGT_API void detachTexture(GLenum attachment);

    TGT_API void detachAll();

    TGT_API Texture* getTextureAtAttachment(GLenum attachment);

    TGT_API static GLuint getActiveObject();

  protected:
    GLuint generateId();

    GLuint id_;
    std::map<GLenum, Texture*> attachedTextures_;

    static const std::string loggerCat_; ///< category used in logging
  };

} // end namespace tgt