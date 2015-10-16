#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "config.h"

namespace tgt {
  TGT_API GLenum _lGLError(int line, const char* file);
} 

#ifdef _DEBUG
  #define LGL_ERROR tgt::_lGLError(__LINE__, __FILE__)
#else
  #define LGL_ERROR
#endif

