#pragma once

#include "tgt_gl.h"
#include <vector>

namespace tgt {

  /**
  * OpenGL Texture Unit
  */
  class TextureUnit {
  public:
    TGT_API TextureUnit(bool keep = false);
    TGT_API ~TextureUnit();

    TGT_API void activate() const;

    TGT_API GLint getEnum() const;
    TGT_API GLint getUnitNumber() const;

    TGT_API static void setZeroUnit();
    TGT_API static void cleanup();

    TGT_API static unsigned short numLocalActive();

    /**
    * Returns true if no texture units are currently assigned.
    */
    TGT_API static bool unused();

  protected:
    void assignUnit() const;
    static void init();

    mutable GLint number_;
    mutable GLint glEnum_;
    mutable bool assigned_;
    bool keep_;

    static bool initialized_;
    static unsigned short totalActive_;
    static unsigned short maxTexUnits_;
    static unsigned short numKeptUnits_;
    static std::vector<bool> busyUnits_;
  };

} // end namespace tgt