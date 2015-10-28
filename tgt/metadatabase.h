#pragma once

#include "serializable.h"
#include <string>

namespace tgt {

  class MetaDataBase : public Serializable {
  public:
    TGT_API virtual MetaDataBase* clone() const { return 0; }
    TGT_API virtual std::string toString() const { return ""; }
    TGT_API virtual std::string toString(const std::string& /*component*/) const { return ""; }
    TGT_API virtual void serialize(XmlSerializer& /*s*/) const {}
    TGT_API virtual void deserialize(XmlDeserializer& /*s*/) {}
  };

} // end namespace tgt