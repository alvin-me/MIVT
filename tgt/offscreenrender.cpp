#include "offscreenrender.h"
#include "tgt_gl.h"
#include <windows.h>
#include <cassert>
#include <tchar.h>

namespace tgt {
  static PIXELFORMATDESCRIPTOR kOglPfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
    1,                     // version number 
    0,                     // flags to be set for according to rendering mode (off-screen or on-screen)
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 32-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    0,                     // no alpha buffer 
    0,                     // shift bit ignored 
    0,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    24,                    // 24-bit depth buffer 
    8,                     // 8-bit stencil buffer 
    0,                     // no auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
  };

  LRESULT CALLBACK WndProc_GL(HWND handle, UINT message, WPARAM w_param,
    LPARAM l_param)
  {
    return DefWindowProc(handle, message, w_param, l_param);
  }

  OffScreenRender::OffScreenRender(void)
    : window_handle_(NULL)
    , dc_handle_(NULL)
    , rc_handle_(NULL)
    , own_window_(false)
  {
    if (!InitializeContext())  return;
    if (GLEW_OK != glewInit()) {
      assert(0);
    }
    glShadeModel(GL_SMOOTH);

    return;
  }

  OffScreenRender::~OffScreenRender(void)
  {
    glFlush();
    glFinish();
    DestroyContext();
    if (own_window_)  DestroyHiddenWindow();
    return;
  }

  bool OffScreenRender::InitializeContext()
  {
    if (!window_handle_) {
      if (!CreateHiddenWindow())  return false;
    }
    HDC dc_handle = GetDC((HWND)window_handle_);
    dc_handle_ = dc_handle;
    kOglPfd.dwFlags = own_window_ ?
      PFD_SUPPORT_OPENGL
      : PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    int pixel_format = ChoosePixelFormat(dc_handle, &kOglPfd);
    if (!SetPixelFormat(dc_handle, pixel_format, &kOglPfd)) {
      assert(0);
      return false;
    }
    rc_handle_ = wglCreateContext(dc_handle);
    if (!rc_handle_) {
      assert(0);
      return false;
    }
    return MakeCurrent();
  }

  bool OffScreenRender::CreateHiddenWindow()
  {
    static const int kDefaultWidth = 256;
    static const int kDefaultHeight = 256;
    HINSTANCE inst_handle = ::GetModuleHandle(NULL);
    WNDCLASSEX wnd_class = { sizeof(wnd_class), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, (WNDPROC)WndProc_GL,
      0, 0, inst_handle,
      NULL, // Uses default icon.
      NULL, // Uses default cursor.
      NULL, // Uses default background brush.
      NULL, // No menu name.
      _T("MIVT Hidden Window Class"),
      NULL  // No small icon.
    };
    if (!RegisterClassEx(&wnd_class)
      && ERROR_CLASS_ALREADY_EXISTS != GetLastError()) {
      assert(0);
      return false;
    }
    window_handle_ = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
      _T("MIVT Hidden Window Class"), _T("MIVT Window For OpenGL"),
      WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, kDefaultWidth,
      kDefaultHeight, NULL, NULL, inst_handle, NULL);
    if (!window_handle_) {
      assert(0);
      return false;
    }
    return own_window_ = true;
  }

  bool OffScreenRender::MakeCurrent()
  {
    HGLRC current_rc = wglGetCurrentContext();
    if (rc_handle_ == current_rc)  return true;
    if (!wglMakeCurrent((HDC)dc_handle_, (HGLRC)rc_handle_)) {
      assert(0);
      return false;
    }
    return true;
  }

  bool OffScreenRender::MakeUncurrent()
  {
    HGLRC current_rc = wglGetCurrentContext();
    if (rc_handle_ != current_rc)  return true;
    if (!wglMakeCurrent(NULL, NULL)) {
      assert(0);
      return false;
    }
    return true;
  }

  bool OffScreenRender::DestroyContext()
  {
    if (MakeCurrent())  MakeUncurrent();  // Makes sure that OpenGL context in this manager is not used.
    if (rc_handle_) {
      wglDeleteContext((HGLRC)rc_handle_);
      rc_handle_ = NULL;
    }
    if (dc_handle_) {
      ReleaseDC((HWND)window_handle_, (HDC)dc_handle_);
      dc_handle_ = NULL;
    }
    return true;
  }

  bool OffScreenRender::DestroyHiddenWindow()
  {
    if (!own_window_ || !window_handle_)  return false;  // Has nothing to destroy.
    if (!DestroyWindow((HWND)window_handle_) ||
      (!UnregisterClass(_T("MIVT Hidden Window Class"), GetModuleHandle(NULL)) &&
      ERROR_CLASS_HAS_WINDOWS != GetLastError())){
      assert(0);
      return false;
    }
    own_window_ = false;
    window_handle_ = NULL;
    return true;
  }
}