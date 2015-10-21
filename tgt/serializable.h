#pragma once
#include "config.h"

namespace tgt {

  class XmlSerializer;
  class XmlDeserializer;

  class Serializable {
  public:
    TGT_API virtual ~Serializable() {}

    /**
    * Serializes all member varaibles using the given @c XmlSerializer.
    *
    * @note You have to implement the serialization in your @c Serializable realization.
    *
    * @param s the @c XmlSerializer
    */
    TGT_API virtual void serialize(XmlSerializer& s) const = 0;

    /**
    * Deserializes all member varaibles using the given @c XmlDeserializer.
    *
    * @note You have to implement the deserialization in your @c Serializable realization.
    *
    * @param s the @c XmlDeserializer
    */
    TGT_API virtual void deserialize(XmlDeserializer& s) = 0;
  };

} // end namespace tgt