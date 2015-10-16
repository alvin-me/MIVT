#include "application.h"
#include <memory>

namespace mivt {
  Application::Application()
  {
  }


  Application::~Application()
  {
  }

  void Application::GetPixels(unsigned char* buffer, int length) {
    memset(buffer, 0, length * sizeof(unsigned char));
  }

  void Application::Resize(int width, int height) {
    width, height;
  }
}