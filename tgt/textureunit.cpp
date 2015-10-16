#include "textureunit.h"
#include "gpucapabilities.h"

namespace tgt {

  bool TextureUnit::initialized_ = false;
  unsigned short TextureUnit::totalActive_ = 0;
  unsigned short TextureUnit::maxTexUnits_ = 0;
  unsigned short TextureUnit::numKeptUnits_ = 0;
  std::vector<bool> TextureUnit::busyUnits_ = std::vector<bool>();

  TextureUnit::TextureUnit(bool keep)
    : number_(0)
    , glEnum_(0)
    , assigned_(false)
    , keep_(keep)
  {
    if (!initialized_)
      init();
  }

  TextureUnit::~TextureUnit() {
    if (assigned_ && !keep_) {
      busyUnits_.at(number_) = false;
      totalActive_--;
    }
  }

  void TextureUnit::activate() const {
    if (!assigned_)
      assignUnit();
    glActiveTexture(getEnum());
  }

  GLint TextureUnit::getEnum() const {
    if (!assigned_)
      assignUnit();
    return glEnum_;
  }

  GLint TextureUnit::getUnitNumber() const {
    if (!assigned_)
      assignUnit();
    return number_;
  }

  void TextureUnit::setZeroUnit() {
    glActiveTexture(GL_TEXTURE0);
  }

  void TextureUnit::cleanup() {
    for (size_t i = 0; i < maxTexUnits_; i++) {
      if (busyUnits_.at(i))
        busyUnits_.at(i) = false;
    }
    totalActive_ = 0;
    numKeptUnits_ = 0;
    setZeroUnit();
  }

  bool TextureUnit::unused() {
    return (totalActive_ == 0);
  }

  unsigned short TextureUnit::numLocalActive() {
    return (totalActive_ - numKeptUnits_);
  }

  void TextureUnit::assignUnit() const {
    assert(totalActive_ <= maxTexUnits_);

    assigned_ = true;

    for (size_t i = 0; i < maxTexUnits_; i++) {
      if (!busyUnits_.at(i)) {
        number_ = (GLint)i;
        busyUnits_.at(i) = true;
        totalActive_++;
        if (keep_)
          numKeptUnits_++;
        break;
      }
    }

    glEnum_ = GL_TEXTURE0 + number_;
  }

  void TextureUnit::init() {
    maxTexUnits_ = (unsigned short)GpuCaps.getNumTextureUnits();
    busyUnits_ = std::vector<bool>(maxTexUnits_, false);
    initialized_ = true;
    numKeptUnits_ = 0;
  }

} // end namespace tgt