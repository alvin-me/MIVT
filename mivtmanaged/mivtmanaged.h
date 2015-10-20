// mivtmanaged.h

#pragma once

#pragma unmanaged
#include "application.h"
#pragma managed

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

  private:
    mivt::Application *localApp_;
	};
}
