#pragma once

#include "config.h"

namespace tgt {

  class OffScreenRender
  {
  public:
    TGT_API OffScreenRender();

    TGT_API ~OffScreenRender();

    /**
    * Makes the OpenGL rendering context within this manager the current RC.
    */
    TGT_API bool MakeCurrent();

    /**
    * Makes current OpenGL rendering context within this manager no longer current.
    */
    TGT_API bool MakeUncurrent();

    /*
    * Initializes rendering context.
    * @return: True if successful, false otherwise.
    */
    TGT_API bool InitializeContext();

    /**
    * Creates a hidden window for off-screen rendering.
    * @return: True if successful, false otherwise.
    */
    TGT_API bool CreateHiddenWindow();

    /**
    * Destroys rendering context of this manager.
    * @return: True if successful, false otherwise.
    */
    TGT_API  bool DestroyContext();

    /**
    * Destroys the hidden window.
    * @return: True if the window and related resources are released properly, false otherwise.
    */
    TGT_API bool DestroyHiddenWindow();

  private:

    /** Window handle. */
    void *window_handle_;

    /** DC handle to the window. */
    void *dc_handle_;

    /** Rendering context handle. */
    void *rc_handle_;

    /** Flag indicating if the window handle is passed to this object or owned by it. */
    bool own_window_;
  };
}

