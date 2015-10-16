#include "application.h"
#include "offscreenrender.h"
#include <memory>

namespace mivt {

  Application::Application()
  {
    offscreen_ = new tgt::OffScreenRender();
  }

  Application::~Application()
  {
    delete offscreen_;
  }

  void Application::GetPixels(unsigned char* buffer, int length) 
  {
    memset(buffer, 0, length * sizeof(unsigned char));
  }

  void Application::Resize(int width, int height) 
  {
    width, height;
  }
}