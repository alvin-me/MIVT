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

  String^ ToManaged(const std::string& str)
  {
    return System::Runtime::InteropServices::Marshal::PtrToStringAnsi(
      static_cast<IntPtr>(const_cast<char*>(str.c_str())));
  }

  Application::Application() {
    local_ = new mivt::Application();
  }

  Application::~Application() {
    delete local_;
  }

  void Application::GetPixels(array < unsigned char> ^ buffer, bool downsampling) {
    pin_ptr<unsigned char> pinned_buffer = &buffer[0];
    local_->GetPixels(pinned_buffer, buffer->Length, downsampling);
  }

  void Application::GetPixels(array<unsigned char>^ buffer) {
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

  void Application::LoadVolume(String^ fileName)
  {
    std::string naviteFileName = FromManaged(fileName);
    local_->LoadVolume(naviteFileName);
  }

  void Application::SetTransfunc(String^ fileName)
  {
    local_->SetTransfunc(FromManaged(fileName));
  }

  String^ Application::GetTransfunc()
  {
    return ToManaged(local_->GetTransfunc());
  }

  void Application::SetClassificationMode(String^ mode)
  {
    local_->SetClassificationMode(FromManaged(mode));
  }

  String^ Application::GetClassificationMode()
  {
    return ToManaged(local_->GetClassificationMode());
  }

  void Application::SetLightAmbient(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->SetLightAmbient(pinned_v);
  }

  void Application::GetLightAmbient(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->GetLightAmbient(pinned_v);
  }

  void Application::SetLightDiffuse(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->SetLightDiffuse(pinned_v);
  }

  void Application::GetLightDiffuse(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->GetLightDiffuse(pinned_v);
  }

  void Application::SetLightSpecular(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->SetLightSpecular(pinned_v);
  }

  void Application::GetLightSpecular(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->GetLightSpecular(pinned_v);
  }

  void Application::SetMaterialShininess(float v)
  {
    local_->SetMaterialShininess(v);
  }

  float Application::GetMaterialShininess()
  {
    return local_->GetMaterialShininess();
  }
}

