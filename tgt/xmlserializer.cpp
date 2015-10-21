#include "xmlserializer.h"

namespace tgt {

  const std::string XmlSerializer::loggerCat_ = "XmlSerializer";

  XmlSerializer::XmlSerializer()
    : XmlSerializerBase()
  {
    // Insert XML declaration...
    document_.LinkEndChild(new TiXmlDeclaration(
      XmlSerializationConstants::XMLVERSION,
      XmlSerializationConstants::XMLENCODING,
      XmlSerializationConstants::XMLSTANDALONE));

    // Create XML root node...
    TiXmlElement* root = new TiXmlElement(XmlSerializationConstants::ROOTNODE);
    root->SetAttribute(
      XmlSerializationConstants::VERSIONATTRIBUTE,
      XmlSerializationConstants::VERSION);
    document_.LinkEndChild(root);

    node_ = root;
  }


  XmlSerializer::~XmlSerializer(){
  }

  void XmlSerializer::checkAttributeKey(const std::string& key)
    throw (SerializationException)
  {
    // Is key a reserved XML attribute name?
    if (key == XmlSerializationConstants::IDATTRIBUTE
      || key == XmlSerializationConstants::REFERENCEATTRIBUTE
      || key == XmlSerializationConstants::TYPEATTRIBUTE)
    {
      raise(XmlSerializationAttributeNamingException("'" + key + "' is a reserved XML attribute name."));
    }

    // Does key already exists?
    if (node_->ToElement()->Attribute(key.c_str()))
      raise(XmlSerializationAttributeNamingException("XML attribute with name '" + key + "' already exists."));
  }

  void XmlSerializer::serialize(const std::string& key, const bool& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, (data ? "true" : "false"));
  }

  void XmlSerializer::serialize(const std::string& key, const char& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const signed char& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const unsigned char& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const uint16_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const int16_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const uint32_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const int32_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const uint64_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const int64_t& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const float& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const double& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const long double& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const std::string& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::vec2& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::vec3& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::vec4& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::dvec2& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::dvec3& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::dvec4& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::ivec2& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::ivec3& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::ivec4& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::col3& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data, true);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::col4& data)
    throw (SerializationException)
  {
    serializeGlmVector(key, data, true);
  }

  void XmlSerializer::serialize(const std::string& key, const glm::mat2& data)
    throw (SerializationException)
  {
    serializeGlmVector(key + ".row0", glm::vec2(data[0][0], data[1][0]));
    serializeGlmVector(key + ".row1", glm::vec2(data[0][1], data[1][1]));
  }

  void XmlSerializer::serialize(const std::string& key, const glm::mat3& data)
    throw (SerializationException)
  {
    serializeGlmVector(key + ".row0", glm::vec3(data[0][0], data[1][0], data[2][0]));
    serializeGlmVector(key + ".row1", glm::vec3(data[0][1], data[1][1], data[2][1]));
    serializeGlmVector(key + ".row2", glm::vec3(data[0][2], data[1][2], data[2][2]));
  }

  void XmlSerializer::serialize(const std::string& key, const glm::mat4& data)
    throw (SerializationException)
  {
    serializeGlmVector(key + ".row0", glm::vec4(data[0][0], data[1][0], data[2][0], data[3][0]));
    serializeGlmVector(key + ".row1", glm::vec4(data[0][1], data[1][1], data[2][1], data[3][1]));
    serializeGlmVector(key + ".row2", glm::vec4(data[0][2], data[1][2], data[2][2], data[3][2]));
    serializeGlmVector(key + ".row3", glm::vec4(data[0][3], data[1][3], data[2][3], data[3][3]));
  }

  void XmlSerializer::serialize(const std::string& key, const Serializable& data)
    throw (SerializationException)
  {
    serializeSimpleTypes(key, data);
  }

  void XmlSerializer::write(std::ostream& stream) {
    TiXmlPrinter printer;
    document_.Accept(&printer);
    stream << printer.CStr();
  }

} // end namespace tgt