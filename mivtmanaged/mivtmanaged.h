// mivtmanaged.h

#pragma once

#pragma unmanaged
#include "application.h"
#pragma managed

#include <msclr/marshal.h>

using namespace System;

namespace mivtmanaged {

	public ref class Application
	{
  public:
    Application();

    ~Application();

    void GetPixels(array<unsigned char>^ buffer, bool downsampling);

    void GetPixels(array<unsigned char>^ buffer);

    void Resize(int width, int height);

    void Rotate(int newPosX, int newPosY, int lastPosX, int lastPosY);

    void Zoom(int newPosX, int newPosY, int lastPosX, int lastPosY);

    void Pan(int newPosX, int newPosY, int lastPosX, int lastPosY);

    void LoadVolume(String^ fileName,
      String^ format,
      array<int>^ dimension,
      array<float>^ spacing,
      float intercept,
      float slope,
      float windowWidth,
      float windowCenter);

    delegate void NativeDelegate(String^);

    void LoadVolume(String^ fileName, NativeDelegate^ callback);

    void SetTransfunc(String^ fileName);
    String^ GetTransfunc();

    void SetClassificationMode(String^ mode);
    String^ GetClassificationMode();

    void SetLightAmbient(array<float>^ v);
    void GetLightAmbient(array<float>^ v);

    void SetLightDiffuse(array<float>^ v);
    void GetLightDiffuse(array<float>^ v);

    void SetLightSpecular(array<float>^ v);
    void GetLightSpecular(array<float>^ v);

    void SetMaterialShininess(float v);
    float GetMaterialShininess();

    void SetFirstBgColor(array<float>^ v);
    void GetFirstBgColor(array<float>^ v);

    void SetSecondBgColor(array<float>^ v);
    void GetSecondBgColor(array<float>^ v);

    void SetBgColorMode(String^ mode);
    String^ GetBgColorMode();

  private:
    mivt::Application *local_;
	};
}
