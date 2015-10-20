// This is the main DLL file.

#include "stdafx.h"

#include "mivtmanaged.h"
#include "application.h"

namespace mivtmanaged {

  Application::Application() {
    localApp_ = new mivt::Application();
  }

  Application::~Application() {
    delete localApp_;
  }

  void Application::GetPixels(array < unsigned char> ^ buffer) {
    pin_ptr<unsigned char> pinned_buffer = &buffer[0];
    localApp_->GetPixels(pinned_buffer, buffer->Length);
  }

  void Application::Resize(int width, int height) {
    localApp_->Resize(width, height);
  }

  void Application::Rotate(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    localApp_->Rotate(newPosX, newPosY, lastPosX, lastPosY);
  }

  void Application::Zoom(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    localApp_->Zoom(newPosX, newPosY, lastPosX, lastPosY);
  }

  void Application::Pan(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    localApp_->Pan(newPosX, newPosY, lastPosX, lastPosY);
  }
}

