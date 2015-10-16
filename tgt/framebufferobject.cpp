#include "framebufferobject.h"
#include "texture.h"
#include "logmanager.h"

namespace tgt {

  const std::string FramebufferObject::loggerCat_("FramebufferObject");

  FramebufferObject::FramebufferObject()
    : id_(0)
  {
    generateId();
  }

  FramebufferObject::~FramebufferObject()
  {
    glDeleteFramebuffersEXT(1, &id_);
  }

  void FramebufferObject::activate()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_);
  }

  void FramebufferObject::deactivate()
  {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  }

  void FramebufferObject::attachTexture(Texture* texture, GLenum attachment, int mipLevel, int zSlice)
  {
    switch (texture->getType()) {
    case GL_TEXTURE_1D:
      glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, texture->getId(), mipLevel);
      break;
    case GL_TEXTURE_3D:
      glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, texture->getId(), mipLevel, zSlice);
      break;
    case GL_TEXTURE_2D_ARRAY_EXT:
      glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, attachment, texture->getId(), mipLevel, zSlice);
      break;
    default: //GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, texture->getType(), texture->getId(), mipLevel);
      break;
    }
    attachedTextures_[attachment] = texture;
  }

  Texture* FramebufferObject::getTextureAtAttachment(GLenum attachment) {
    std::map<GLenum, Texture*>::iterator iter = attachedTextures_.find(attachment);
    if (iter != attachedTextures_.end()) {
      return attachedTextures_[attachment];
    }
    else
      return 0;
  }

  void FramebufferObject::detachTexture(GLenum attachment) {
    std::map<GLenum, Texture*>::iterator iter = attachedTextures_.find(attachment);
    if (iter != attachedTextures_.end()) {
      attachedTextures_.erase(iter);
    }
    else {
      LWARNING("Trying to detach unknown texture!");
    }

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, 0, 0);
  }

  void FramebufferObject::detachAll() {
    while (!attachedTextures_.empty()) {
      detachTexture(attachedTextures_.begin()->first);
    }
  }

  bool FramebufferObject::isComplete() const
  {
    bool complete = false;

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      complete = true;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      LERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      LERROR("GL_FRAMEBUFFER_UNSUPPORTED_EXT");
      break;
    default:
      LERROR("Unknown error!");
    }
    return complete;
  }

  bool FramebufferObject::isActive() const {
    return ((getActiveObject() == id_) && (id_ != 0));
  }

  GLuint FramebufferObject::getActiveObject() {
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fbo);
    return static_cast<GLuint>(fbo);
  }

  GLuint FramebufferObject::generateId() {
    id_ = 0;
    glGenFramebuffersEXT(1, &id_);
    return id_;
  }

} // end namespace tgt