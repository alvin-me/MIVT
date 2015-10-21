#pragma once

#include "tgt_math.h"
#include "serializable.h"

namespace tgt {

  class TransFuncColorKey : public Serializable {
  public:
    TGT_API TransFuncColorKey(float i, const glm::col3& color);
    
    TGT_API TransFuncColorKey();

    TGT_API ~TransFuncColorKey();

    TGT_API bool operator==(const TransFuncColorKey& key);

    TGT_API bool operator!=(const TransFuncColorKey& key);

    TGT_API void setColor(const glm::col3& color);

    TGT_API const glm::col3& getColor() const;

    TGT_API glm::col3& getColor();

    TGT_API float getIntensity() const;

    TGT_API  void setIntensity(float i);

    TGT_API virtual void serialize(XmlSerializer& s) const;

    TGT_API  virtual void deserialize(XmlDeserializer& s);

  private:
    float intensity_;       ///< intensity at which the key is located
    glm::col3 color_;       ///< color of the key
  };

  //------------------------------------------------------------------------------

  class TransFuncAlphaKey : public Serializable {
  public:
    TGT_API TransFuncAlphaKey(float i, float alpha);

    TGT_API TransFuncAlphaKey();

    TGT_API ~TransFuncAlphaKey();

    TGT_API bool operator==(const TransFuncAlphaKey& key);

    TGT_API bool operator!=(const TransFuncAlphaKey& key);

    TGT_API void setAlpha(float a);

    TGT_API  float getAlpha();

    TGT_API float getIntensity() const;

    TGT_API void setIntensity(float i);

    TGT_API virtual void serialize(XmlSerializer& s) const;

    TGT_API virtual void deserialize(XmlDeserializer& s);

  private:
    float intensity_;       ///< intensity at which the key is located
    float alpha_;           ///< alpha of the key
  };
} // end namespace tgt