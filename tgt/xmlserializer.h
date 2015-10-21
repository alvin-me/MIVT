#pragma once

#include "xmlserializerbase.h"
#include "serializable.h"
#include "xmlserializationconstants.h"

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <stack>

namespace tgt {

  /**
  * XmlSerializer is responsible for serializing memory data to XML documents.
  */
  class XmlSerializer : public XmlSerializerBase
  {
  public:
    TGT_API XmlSerializer();

    TGT_API ~XmlSerializer();

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const bool& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const char& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const signed char& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const unsigned char& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const uint16_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const int16_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const uint32_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const int32_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const uint64_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const int64_t& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const float& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const double& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const long double& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    TGT_API void serialize(const std::string& key, const std::string& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::vec2& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::vec3& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::vec4& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::dvec2& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::dvec3& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::dvec4& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::ivec2& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::ivec3& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::ivec4& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::col3& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::col4& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::mat2& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::mat3& data)
      throw (SerializationException);

    /**
    * Serializes the given key/data pair.
    *
    * @param key the XML node key
    * @param data the data
    */
    TGT_API void serialize(const std::string& key, const glm::mat4& data)
      throw (SerializationException);

    /**
    * Serializes the given Serializable interface realization.
    *
    * @note All user defined classes must realize the Serializable
    *       interface to be serializable.
    *
    * @param key the XML node key
    * @param data the Serializable realization
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialize method of given Serializable raises this exception
    */
    TGT_API void serialize(const std::string& key, const Serializable& data)
      throw (SerializationException);

    /**
    * Serializes the given pointer reference.
    *
    * @tparam type of referenced data
    *
    * @param key the XML node key
    * @param data the pointer reference
    */
    template<class T>
    void serialize(const std::string& key, const T* const& data)
      throw (SerializationException);

    /**
    * Serializes the given std::pair.
    *
    * @tparam S data type of first pair item
    * @tparam T data type of second pair item
    *
    * @param key the XML node key
    * @param data the pair to serialize
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of pair items raises this exception
    */
    template<class S, class T>
    void serialize(const std::string& key, const std::pair<S, T>& data)
      throw (SerializationException);

    /**
    * Serializes the given data vector.
    *
    * @par
    * Iterates over the given data collection and serialize each collection item.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data the data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of collection items raises this exception
    */
    template<class T>
    void serialize(const std::string& key, 
      const std::vector<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Serializes the given data deque.
    *
    * @par
    * Iterates over the given data collection and serialize each collection item.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data the data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of collection items raises this exception
    */
    template<class T>
    void serialize(const std::string& key, 
      const std::deque<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Serializes the given data list.
    *
    * @par
    * Iterates over the given data collection and serialize each collection item.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data the data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of collection items raises this exception
    */
    template<class T>
    void serialize(const std::string& key, 
      const std::list<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Serializes the given data set.
    *
    * @par
    * Iterates over the given data collection and serialize each collection item.
    *
    * @note Element order of set items are not guaranteed to remains constant
    *       during serialization and deserialization due to limits of
    *       some STL containers like std::set.
    *
    * @tparam T data type of set items
    * @tparam C comparison class @see std::set
    *
    * @param key the XML node key
    * @param data the data set
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of collection items raises this exception
    * @throws XmlSerializationInvalidOperationException
    *     if pointer content serialization is enabled,
    *     because of possible hash problems on deserialization
    */
    template<class T, class C>
    void serialize(const std::string& key, 
      const std::set<T, C>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Serializes the given data map.
    *
    * @par
    * Iterates over the given map and serialize each @c key/value pair item of the map.
    *
    * @note Element order of map items are not guaranteed to remains constant
    *       during serialization and deserialization due to limits of
    *       some STL containers like @c std::map.
    *
    * @tparam T data type of map keys
    * @tparam U data type of map values
    * @tparam C comparison class @see std::map
    *
    * @param key the XML node key
    * @param data the data map
    * @param valueKey XML node key for each value node
    * @param keyKey key for each XML key node or attribute
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of the @c key/value pair item raises this exception
    */
    template<class T, class U, class C>
    void serialize(
      const std::string& key,
      const std::map<T, U, C>& data,
      const std::string& valueKey = XmlSerializationConstants::VALUENODE,
      const std::string& keyKey = XmlSerializationConstants::KEYNODE)
      throw (SerializationException);

    /**
    * Writes the XML document that contains all already serialized data to the given stream.
    *
    * @attention Keep in mind that all memory data must be serialized before the XML document
    *            can be written. Otherwise, it cannot be ensured that all pointer references
    *            are resolved.
    *
    * @param stream the output stream
    */
    TGT_API void write(std::ostream& stream);

  private:
    /**
    * Checks if the given XML attribute key is not a reserved key and unique.
    *
    * @param key the XML attrubte key
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    void checkAttributeKey(const std::string& key)
      throw (SerializationException);

    /**
    * Helper function for serializing key/data pairs with simple data types.
    *
    * @tparam T data type
    *
    * @param key the XML node key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    template<typename T>
    inline void serializeSimpleTypes(const std::string& key, const T& data)
      throw (SerializationException);

    /**
    * Helper function for serializing key/data pairs with glm vectors.
    *
    * @tparam T data type of vector
    *
    * @param key the XML node key
    * @param vector the glm vector
    * @param isColor flag determine if given vector is a color
    */
    template<class T>
    inline void serializeGlmVector(const std::string& key, const T& vector, const bool& isColor = false)
      throw (SerializationException);

    /**
    * Helper function creates a XML node with the given nodeKey and
    * serializes the given key/data pair as an attribute.
    *
    * @tparam T data type
    *
    * @param nodeKey the XML node key
    * @param key the XML attribute key
    * @param data the data
    *
    * @throws XmlSerializationAttributeNamingException
    *     if primitive data is serialized as XML attributes and key is reserved or not unique
    */
    template<class T>
    inline void serializeAttributeInNode(const std::string& nodeKey, const std::string& key, const T& data)
      throw (SerializationException);

    /**
    * Helper function for serializing data collections like STL container.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of collection
    *
    * @param key the XML node key
    * @param collection the data collection
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of collection items raises this exception
    */
    template<class T>
    inline void serializeCollection(const std::string& key, 
      const T& collection, 
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Helper function for serializing data maps like STL maps.
    *
    * @note Element order of map items are not guaranteed to remains constant
    *       during serialization and deserialization due to limits of
    *       some STL containers like @c std::map.
    *
    * @tparam T data type of map
    *
    * @param key the XML node key
    * @param map the map
    * @param valueKey XML node key for each value node
    * @param keyKey key for each XML key node or attribute
    *
    * @throws XmlSerializationAttributeNamingException
    *     if serialization of @c key/value pair items raises the exception
    */
    template<class T>
    inline void serializeMap(
      const std::string& key,
      const T& map,
      const std::string& valueKey = XmlSerializationConstants::VALUENODE,
      const std::string& keyKey = XmlSerializationConstants::KEYNODE)
      throw (SerializationException);

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------


  template<typename T>
  inline void XmlSerializer::serializeSimpleTypes(const std::string& key, const T& data)
    throw (SerializationException)
  {
    // Serialize as XML attribute?
    if (useAttributes_) {
      checkAttributeKey(key);
      node_->ToElement()->SetAttribute(key.c_str(), convertDataToString(data).c_str());
    }
    // Serialize as XML node?
    else {
      TiXmlElement* newNode = new TiXmlElement(key.c_str());
      node_->LinkEndChild(newNode);
      newNode->SetAttribute(XmlSerializationConstants::VALUEATTRIBUTE, convertDataToString(data).c_str());
    }
  }

  template<>
  inline void XmlSerializer::serializeSimpleTypes<std::string>(const std::string& key, const std::string& data)
    throw (SerializationException)
  {
    // check, if we have to serialize the string as CDATA
    bool requireCDATA = false;
    requireCDATA |= data.find("\n") != std::string::npos;
    requireCDATA |= data.find("\r") != std::string::npos;

    // Serialize as XML attribute wanted and possible?
    if (useAttributes_ && !requireCDATA) {
      checkAttributeKey(key);
      node_->ToElement()->SetAttribute(key.c_str(), data.c_str());
      return;
    }

    // Serialize as XML node...
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);
    if (!requireCDATA) {
      // ATTENTION: No special handling of the given string is needed that is why this block
      //            has to correspond with the not specialized serializeSimpleTypes method.
      newNode->SetAttribute(XmlSerializationConstants::VALUEATTRIBUTE, data.c_str());
    }
    else {
      // Serialize string as CDATA to prevent possible conversion or other XML errors...
      TiXmlText* text = new TiXmlText(data.c_str());
      text->SetCDATA(true);
      newNode->LinkEndChild(text);
    }
  }

  template<>
  inline void XmlSerializer::serializeSimpleTypes<Serializable>(const std::string& key, const Serializable& data)
    throw (SerializationException)
  {
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);

    TemporaryNodeChanger nodeChanger(*this, newNode);

    data.serialize(*this);
  }

  template<class T>
  void XmlSerializer::serialize(const std::string& key, const T* const& data)
    throw (SerializationException)
  {
    serialize(key, *data);
  }

  template<class S, class T>
  void XmlSerializer::serialize(const std::string& key, const std::pair<S, T>& data)
    throw (SerializationException)
  {
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);

    TemporaryNodeChanger nodeChanger(*this, newNode);

    // first item
    if (useAttributes_ && isPrimitiveType(typeid(S))) {
      // serialize primitive type as XML attribute
      serializeAttributeInNode("First", XmlSerializationConstants::VALUEATTRIBUTE, data.first);
    }
    else {
      // serialize as XML node
      serialize("First", data.first);
    }

    // second item
    if (useAttributes_ && isPrimitiveType(typeid(T))) {
      // serialize primitive type as XML attribute
      serializeAttributeInNode("Second", XmlSerializationConstants::VALUEATTRIBUTE, data.second);
    }
    else {
      // serialize as XML node
      serialize("Second", data.second);
    }
  }

  template<class T>
  void XmlSerializer::serialize(const std::string& key, const std::vector<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    serializeCollection(key, data, itemKey);
  }

  template<class T>
  void XmlSerializer::serialize(const std::string& key, const std::deque<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    serializeCollection(key, data, itemKey);
  }

  template<class T>
  void XmlSerializer::serialize(const std::string& key, const std::list<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    serializeCollection(key, data, itemKey);
  }

  template<class T, class C>
  void XmlSerializer::serialize(const std::string& key, const std::set<T, C>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    serializeCollection(key, data, itemKey);
  }

  template<class T, class U, class C>
  void XmlSerializer::serialize(const std::string& key,
    const std::map<T, U, C>& data,
    const std::string& valueKey,
    const std::string& keyKey)
    throw (SerializationException)
  {
    serializeMap(key, data, valueKey, keyKey);
  }

  template<class T>
  inline void XmlSerializer::serializeGlmVector(const std::string& key, const T& vector, const bool& isColor)
    throw (SerializationException)
  {
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);

    newNode->SetAttribute(
      isColor ? XmlSerializationConstants::COLORRATTRIBUTE : XmlSerializationConstants::VECTORXATTRIBUTE,
      convertDataToString(isColor ? static_cast<int>(vector[0]) : vector[0]).c_str());

    if (vector.length() >= 2) {
      newNode->SetAttribute(
        isColor ? XmlSerializationConstants::COLORGATTRIBUTE : XmlSerializationConstants::VECTORYATTRIBUTE,
        convertDataToString(isColor ? static_cast<int>(vector[1]) : vector[1]).c_str());
    }

    if (vector.length() >= 3) {
      newNode->SetAttribute(
        isColor ? XmlSerializationConstants::COLORBATTRIBUTE : XmlSerializationConstants::VECTORZATTRIBUTE,
        convertDataToString(isColor ? static_cast<int>(vector[2]) : vector[2]).c_str());
    }

    if (vector.length() >= 4) {
      newNode->SetAttribute(
        isColor ? XmlSerializationConstants::COLORAATTRIBUTE : XmlSerializationConstants::VECTORWATTRIBUTE,
        convertDataToString(isColor ? static_cast<int>(vector[3]) : vector[3]).c_str());
    }
  }

  template<class T>
  inline void XmlSerializer::serializeAttributeInNode(const std::string& nodeKey, const std::string& key, const T& data)
    throw (SerializationException)
  {
    TiXmlNode* newNode = new TiXmlElement(nodeKey.c_str());
    node_->LinkEndChild(newNode);

    TemporaryNodeChanger nodeChanger(*this, newNode);

    serialize(key, data);
  }

  template<class T>
  inline void XmlSerializer::serializeCollection(const std::string& key, const T& collection, const std::string& itemKey)
    throw (SerializationException)
  {
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);

    TemporaryNodeChanger nodeChanger(*this, newNode);

    for (typename T::const_iterator it = collection.begin(); it != collection.end(); ++it) {
      // Serialize primitive type as XML attribute?
      if (useAttributes_ && isPrimitiveType(typeid(typename T::value_type))) {
        serializeAttributeInNode(itemKey, XmlSerializationConstants::VALUEATTRIBUTE, *it);
      }
      // Serialize as XML node...
      else
        serialize(itemKey, *it);
    }
  }

  template<class T>
  inline void XmlSerializer::serializeMap(const std::string& key,
    const T& map,
    const std::string& valueKey,
    const std::string& keyKey)
    throw (SerializationException)
  {
    TiXmlElement* newNode = new TiXmlElement(key.c_str());
    node_->LinkEndChild(newNode);

    TemporaryNodeChanger nodeChanger(*this, newNode);

    for (typename T::const_iterator it = map.begin(); it != map.end(); ++it)
    {
      // Serialize primitive key type as XML node?
      if (!useAttributes_ || !isPrimitiveType(typeid(typename T::key_type)))
        serialize(keyKey, it->first);

      // Serialize primitive mapped type as XML attribute?
      if (useAttributes_ && isPrimitiveType(typeid(typename T::mapped_type))) {
        serializeAttributeInNode(valueKey, XmlSerializationConstants::VALUEATTRIBUTE, it->second);
      }
      // Serialize as XML node...
      else {
        serialize(valueKey, it->second);
      }

      // Serialize primitive key type as XML attribute?
      if (useAttributes_ && isPrimitiveType(typeid(typename T::key_type))) {
        TemporaryNodeChanger innerNodeChanger(*this, node_->LastChild());

        serialize(keyKey, it->first);
      }
    }
  }

} // end namespace tgt