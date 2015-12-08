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
    local_ = new mivt::Application(true);
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

  void Application::LoadVolume(String^ fileName, NativeDelegate^ callback)
  {
    using System::IntPtr;
    using System::Runtime::InteropServices::Marshal;

    IntPtr pointer = Marshal::GetFunctionPointerForDelegate(callback);

    std::string naviteFileName = FromManaged(fileName);

    local_->LoadVolume(naviteFileName, static_cast<tgt::ProgressCallback>(pointer.ToPointer()));
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

  void Application::SetFirstBgColor(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->SetFirstBgColor(pinned_v);
  }

  void Application::GetFirstBgColor(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->GetFirstBgColor(pinned_v);
  }

  void Application::SetSecondBgColor(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->SetSecondBgColor(pinned_v);
  }

  void Application::GetSecondBgColor(array<float>^ v)
  {
    pin_ptr<float> pinned_v = &v[0];
    local_->GetSecondBgColor(pinned_v);
  }

  void Application::SetBgColorMode(String^ mode)
  {
    local_->SetBgColorMode(FromManaged(mode));
  }

  String^ Application::GetBgColorMode()
  {
    return ToManaged(local_->GetBgColorMode());
  }

  void Application::SaveToImage(String^ filename)
  {
    local_->SaveToImage(FromManaged(filename));
  }

  void Application::SaveToImage()
  {
    local_->SaveToImage();
  }

  void Application::SaveToImage(String^ filename, int width, int height)
  {
    local_->SaveToImage(FromManaged(filename), width, height);
  }

  void Application::SaveToImage(int width, int height)
  {
    local_->SaveToImage(width, height);
  }

  void Application::ChangeClipRight(float val)
  {
    local_->ChangeClipRight(val);
  }

  void Application::ChangeClipLeft(float val)
  {
    local_->ChangeClipLeft(val);
  }

  void Application::ChangeClipBack(float val)
  {
    local_->ChangeClipBack(val);
  }

  void Application::ChangeClipFront(float val)
  {
    local_->ChangeClipFront(val);
  }

  void Application::ChangeClipBottom(float val)
  {
    local_->ChangeClipBottom(val);
  }

  void Application::ChangeClipTop(float val)
  {
    local_->ChangeClipTop(val);
  }

  void Application::resetClipPlanes()
  {
    local_->resetClipPlanes();
  }

  void Application::EnableClip(bool flag)
  {
    local_->EnableClip(flag);
  }

  void Application::getClipMaximum(array<int>^ v)
  {
    pin_ptr<int> pinned_v = &v[0];
    local_->getClipMaximum(pinned_v);
  }

  float Application::GetClipRight()
  {
    return local_->GetClipRight();
  }
  float Application::GetClipLeft()
  {
    return local_->GetClipLeft();
  }
  float Application::GetClipBack()
  {
    return local_->GetClipBack();
  }
  float Application::GetClipFront()
  {
    return local_->GetClipFront();
  }
  float Application::GetClipBottom()
  {
    return local_->GetClipBottom();
  }
  float Application::GetClipTop()
  {
    return local_->GetClipTop();
  }
  bool Application::IsClipEnabled()
  {
    return local_->IsClipEnabled();
  }

  void Application::DoSculpt(array<float>^ polygon)
  {
    pin_ptr<float> pinned_polygon = &polygon[0];
    int count = polygon->Length;
    std::vector<glm::vec2> vec;
    for (int i = 0; i < count / 2; ++i) {
      vec.push_back(glm::round(glm::vec2(pinned_polygon[i * 2], pinned_polygon[i * 2 + 1])));
    }
    local_->DoSculpt(vec);
  }
}

