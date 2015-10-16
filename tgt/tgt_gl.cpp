#include "tgt_gl.h"
#include "logmanager.h"

namespace tgt {
  GLenum _lGLError(int line, const char* file) {
    GLenum err = glGetError();

    if (err != GL_NO_ERROR) {
      const GLubyte* exp = gluErrorString(err);

      std::ostringstream tmp1, tmp2, loggerCat;
      tmp2 << " File: " << file << "@" << line;
      tmp1 << (exp ? (const char*)exp : "unknown");
      loggerCat << "gl-error:" << file << ':' << line;

      LogMgr.log(loggerCat.str(), tgt::Error, tmp1.str(), tmp2.str());
    }

    return err;
  }
}
