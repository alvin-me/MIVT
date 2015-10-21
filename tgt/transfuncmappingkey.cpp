#include "transfuncmappingkey.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"

namespace tgt {

  TransFuncColorKey::TransFuncColorKey(float i, const glm::col3& color)
    : intensity_(i)
    , color_(color)
  {}

  TransFuncColorKey::TransFuncColorKey()
    : intensity_(0.f)
    , color_(0)
  {

  }

  TransFuncColorKey::~TransFuncColorKey()
  {

  }

  bool TransFuncColorKey::operator==(const TransFuncColorKey& key)
  {
    return (intensity_ == key.intensity_) && (color_ == key.color_);
  }

  bool TransFuncColorKey::operator!=(const TransFuncColorKey& key)
  {
    return !(*this == key);
  }

  void TransFuncColorKey::setColor(const glm::col3& color)
  {
    color_ = color;
  }

  const glm::col3& TransFuncColorKey::getColor() const
  {
    return color_;
  }

  glm::col3& TransFuncColorKey::getColor()
  {
    return color_;
  }

  float TransFuncColorKey::getIntensity() const
  {
    return intensity_;
  }

  void TransFuncColorKey::setIntensity(float i)
  {
    intensity_ = i;
  }

  void TransFuncColorKey::serialize(XmlSerializer& s) const
  {
    s.serialize("intensity", intensity_);
    s.serialize("color", color_);
  }

  void TransFuncColorKey::deserialize(XmlDeserializer& s) {
    s.deserialize("intensity", intensity_);
    s.deserialize("color", color_);
  }

  //------------------------------------------------------------------------------

  TransFuncAlphaKey::TransFuncAlphaKey(float i, float alpha)
    : intensity_(i)
    , alpha_(alpha)
  {

  }

  TransFuncAlphaKey::TransFuncAlphaKey()
    : intensity_(0.f)
    , alpha_(0.f)
  {

  }

  TransFuncAlphaKey::~TransFuncAlphaKey()
  {

  }

  bool TransFuncAlphaKey::operator==(const TransFuncAlphaKey& key)
  {
    return (intensity_ == key.intensity_) && (alpha_ == key.alpha_);
  }

  bool TransFuncAlphaKey::operator!=(const TransFuncAlphaKey& key)
  {
    return !(*this == key);
  }

  void TransFuncAlphaKey::setAlpha(float a)
  {
    alpha_ = a;
  }

  float TransFuncAlphaKey::getAlpha()
  {
    return alpha_;
  }

  float TransFuncAlphaKey::getIntensity() const
  {
    return intensity_;
  }

  void TransFuncAlphaKey::setIntensity(float i)
  {
    intensity_ = i;
  }

  void TransFuncAlphaKey::serialize(XmlSerializer& s) const
  {
    s.serialize("intensity", intensity_);
    s.serialize("alpha", alpha_);
  }

  void TransFuncAlphaKey::deserialize(XmlDeserializer& s) {
    s.deserialize("intensity", intensity_);
    s.deserialize("alpha", alpha_);
  }

} // end namespace tgt