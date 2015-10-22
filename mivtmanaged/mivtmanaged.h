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

    void SetTransfunc(String^ fileName);
    String^ GetTransfunc();

    void SetClassificationMode(String^ mode);
    String^ GetClassificationMode();

  private:
    mivt::Application *local_;
	};
}
