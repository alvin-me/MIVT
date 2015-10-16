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

  private:
    mivt::Application *localApp_;
	};
}
