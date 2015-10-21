#include "xmlserializerbase.h"

namespace tgt {

  const std::string XmlSerializerBase::loggerCat_ = "XmlSerializerBase";

  XmlSerializerBase::TemporaryNodeChanger::TemporaryNodeChanger(XmlSerializerBase& serializer, TiXmlNode* node)
    : serializer_(serializer)
    , storedNode_(serializer_.node_)
  {
    serializer_.node_ = node;
  }

  XmlSerializerBase::TemporaryNodeChanger::~TemporaryNodeChanger() {
    serializer_.node_ = storedNode_;
  }

  //------------------------------------------------------------------------------

  XmlSerializerBase::XmlSerializerBase()
    : node_(0)
    , useAttributes_(false)
  {
  }

  XmlSerializerBase::~XmlSerializerBase()
  {
  }

  void XmlSerializerBase::setUseAttributes(const bool& useAttributes) {
    useAttributes_ = useAttributes;
  }

  bool XmlSerializerBase::getUseAttributes() const {
    return useAttributes_;
  }

  bool XmlSerializerBase::isPrimitiveType(const std::type_info& type) const {
    if (type == typeid(bool)
      || type == typeid(char)
      || type == typeid(signed char)
      || type == typeid(unsigned char)
      || type == typeid(signed short)
      || type == typeid(unsigned short)
      || type == typeid(signed int)
      || type == typeid(unsigned int)
      || type == typeid(signed long)
      || type == typeid(unsigned long)
      || type == typeid(float)
      || type == typeid(double)
      || type == typeid(long double)
      || type == typeid(std::string)
      || type == typeid(uint8_t)
      || type == typeid(int8_t)
      || type == typeid(uint16_t)
      || type == typeid(int16_t)
      || type == typeid(uint32_t)
      || type == typeid(int32_t)
      || type == typeid(int64_t)
      || type == typeid(uint64_t))
    {
      return true;
    }

    return false;
  }

  bool XmlSerializerBase::isPrimitivePointerType(const std::type_info& type) const {
    if (type == typeid(bool*)
      || type == typeid(char*)
      || type == typeid(signed char*)
      || type == typeid(unsigned char*)
      || type == typeid(signed short*)
      || type == typeid(unsigned short*)
      || type == typeid(signed int*)
      || type == typeid(unsigned int*)
      || type == typeid(signed long*)
      || type == typeid(unsigned long*)
      || type == typeid(float*)
      || type == typeid(double*)
      || type == typeid(long double*)
      || type == typeid(std::string*)
      || type == typeid(uint8_t*)
      || type == typeid(int8_t*)
      || type == typeid(uint16_t*)
      || type == typeid(int16_t*)
      || type == typeid(uint32_t*)
      || type == typeid(int32_t*)
      || type == typeid(int64_t*)
      || type == typeid(uint64_t*))
    {
      return true;
    }

    return false;
  }

  std::string XmlSerializerBase::convertDataToString(const float& data) {
    std::stringstream stream;
    stream.precision(8);
    stream.setf(std::ios::fixed);

    stream << data;
    std::string s = stream.str();

    // remove trailing zeros and decimal point
    size_t point_pos = s.find('.');
    if (point_pos != std::string::npos) {
      size_t new_size = s.size();
      for (size_t i = s.size() - 1; i >= point_pos; i--) {
        if (s[i] == '0' || s[i] == '.')
          new_size--;
        else
          break;
      }
      s.resize(new_size);
    }

    return s;
  }

  std::string XmlSerializerBase::convertDataToString(const char* data) {
    return std::string(data);
  }

} // end namespace tgt