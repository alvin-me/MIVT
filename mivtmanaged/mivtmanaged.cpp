// This is the main DLL file.

#include "stdafx.h"

#include "mivtmanaged.h"
#include "application.h"

namespace mivtmanaged {

  std::string FromManaged(String^ str)
  {
    std::string result;
    size_t converted = 0;
    char charBuffer[8192] = { 0 };
    pin_ptr<const wchar_t> pinned_string = PtrToStringChars(str);
    wcstombs_s(&converted, charBuffer, sizeof(charBuffer), pinned_string, str->Length);
    result.assign(charBuffer);

    return result;
  }

  Application::Application() {
    local_ = new mivt::Application();
  }

  Application::~Application() {
    delete local_;
  }

  void Application::GetPixels(array < unsigned char> ^ buffer) {
    pin_ptr<unsigned char> pinned_buffer = &buffer[0];
    local_->GetPixels(pinned_buffer, buffer->Length);
  }

  void Application::Resize(int width, int height) {
    local_->Resize(width, height);
  }

  void Application::Rotate(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    local_->Rotate(newPosX, newPosY, lastPosX, lastPosY);
  }

  void Application::Zoom(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    local_->Zoom(newPosX, newPosY, lastPosX, lastPosY);
  }

  void Application::Pan(int newPosX, int newPosY, int lastPosX, int lastPosY) {
    local_->Pan(newPosX, newPosY, lastPosX, lastPosY);
  }

  void Application::LoadVolume(String^ fileName,
    String^ format,
    array<int>^ dimension,
    array<float>^ spacing,
    float intercept,
    float slope,
    float windowWidth,
    float windowCenter)
  {
    std::string naviteFileName = FromManaged(fileName);
    std::string nativeFormat = FromManaged(format);
    pin_ptr<int> pinned_dimension = &dimension[0];
    pin_ptr<float> pinned_spacing = &spacing[0];
    local_->LoadVolume(naviteFileName, nativeFormat, pinned_dimension, pinned_spacing,
      intercept, slope, windowWidth, windowCenter);
  }
}

