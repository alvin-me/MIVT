#include "xmldeserializer.h"

namespace tgt {

  const std::string XmlDeserializer::loggerCat_ = "XmlDeserializer";

  XmlDeserializer::XmlDeserializer()
    : XmlSerializerBase()
  {
  }

  XmlDeserializer::~XmlDeserializer() {
  }

  TiXmlElement* XmlDeserializer::getNextXmlElement(const std::string& key)
    throw (SerializationException)
  {
    TiXmlElement* element = node_->FirstChildElement(key.c_str());
    while (element) {
      // Was node not visited before?
      if (visitedNodes_.find(element) == visitedNodes_.end())
      {
        visitedNodes_.insert(element);
        return element;
      }

      element = element->NextSiblingElement(key.c_str());
    }

    raise(XmlSerializationNoSuchDataException("No further XML node with key '" + key + "' found."));
    return 0;
  }

  void XmlDeserializer::deserialize(const std::string& key, bool& data)
    throw (SerializationException)
  {
    std::string boolValue;
    deserializeSimpleTypes(key, boolValue);

    std::transform(boolValue.begin(), boolValue.end(), boolValue.begin(), tolower);

    if (boolValue == "true" || boolValue == "1")
      data = true;
    else if (boolValue == "false" || boolValue == "0")
      data = false;
    else
      raise(XmlSerializationFormatException("XML node with key '" + key + "' contains unknown bool value."));
  }

  void XmlDeserializer::deserialize(const std::string& key, char& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, signed char& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, unsigned char& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, uint16_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, int16_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, uint32_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, int32_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, uint64_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, int64_t& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, float& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, double& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, long double& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, std::string& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::vec2& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::vec3& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::vec4& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::dvec2& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::dvec3& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::dvec4& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::ivec2& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::ivec3& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::ivec4& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::col3& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data, true);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::col4& data)
    throw (SerializationException)
  {
    deserializeGlmVector(key, data, true);
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::mat2& data)
    throw (SerializationException)
  {
    glm::vec2 row0, row1;
    deserializeGlmVector(key + ".row0", row0);
    deserializeGlmVector(key + ".row1", row1);
    data = glm::mat2(glm::vec2(row0.x, row1.x),
      glm::vec2(row0.y, row1.y));
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::mat3& data)
    throw (SerializationException)
  {
    glm::vec3 row0, row1, row2;
    deserializeGlmVector(key + ".row0", row0);
    deserializeGlmVector(key + ".row1", row1);
    deserializeGlmVector(key + ".row2", row2);
    data = glm::mat3(glm::vec3(row0.x, row1.x, row2.x),
      glm::vec3(row0.y, row1.y, row2.y),
      glm::vec3(row0.z, row1.z, row2.z));
  }

  void XmlDeserializer::deserialize(const std::string& key, glm::mat4& data)
    throw (SerializationException)
  {
    glm::vec4 row0, row1, row2, row3;
    deserializeGlmVector(key + ".row0", row0);
    deserializeGlmVector(key + ".row1", row1);
    deserializeGlmVector(key + ".row2", row2);
    deserializeGlmVector(key + ".row3", row3);
    data = glm::mat4(glm::vec4(row0.x, row1.x, row2.x, row3.x),
      glm::vec4(row0.y, row1.y, row2.y, row3.y),
      glm::vec4(row0.z, row1.z, row2.z, row3.z),
      glm::vec4(row0.w, row1.w, row2.w, row3.w));
  }

  void XmlDeserializer::deserialize(const std::string& key, Serializable& data)
    throw (SerializationException)
  {
    deserializeSimpleTypes(key, data);
  }

  void XmlDeserializer::read(std::istream& stream)
    throw (SerializationException)
  {
    // Read input stream...
    std::stringbuf buffer;
    do
    {
      // Use 0 character instead of '\n' to minimize the number of get-calls...
      stream.get(buffer, 0);
    } while (stream.good() && !stream.eof()
      && (buffer.sputc((char)stream.get()) != std::stringbuf::traits_type::eof()));

    // Parse input...
    document_.Parse(buffer.str().c_str());

    TiXmlElement* root = document_.RootElement();

    // Is there no root element?
    if (!root)
      raise(XmlSerializationFormatException(std::string("No root node found.")));

    // Has root node incorrect name?
    if (strcmp(root->Value(), XmlSerializationConstants::ROOTNODE) != 0) {
      raise(XmlSerializationFormatException("XML root node name is '" + std::string(root->Value())
        + "' instead of '" + XmlSerializationConstants::ROOTNODE + "'."));
    }

    const char* version = root->Attribute(XmlSerializationConstants::VERSIONATTRIBUTE);
    // Is serialization version not set?
    if (!version)
      raise(XmlSerializationFormatException("XML root node has no version attribute."));
    // Does XmlSerializer and XmlDeserializer version not match the XML document version?
    if (strcmp(version, XmlSerializationConstants::VERSION) != 0) {
      raise(XmlSerializationVersionMismatchException("XML document has version " + std::string(version)
        + " instead of " + XmlSerializationConstants::VERSION + "."));
    }

    node_ = root;
  }

} // end namespace tgt