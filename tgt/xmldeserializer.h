#pragma once

#include "xmlserializerbase.h"
#include "serializable.h"
#include "xmlserializationconstants.h"

#include <algorithm>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <iostream>
#include <typeinfo>
#include <type_traits>

namespace tgt {

  /**
  * XmlDeserializer is responsible for deserializing XML documents to memory data.
  */
  class XmlDeserializer : public XmlSerializerBase
  {
  public:
    TGT_API XmlDeserializer();

    TGT_API ~XmlDeserializer();

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, bool& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, char& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, signed char& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, unsigned char& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, uint16_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, int16_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, uint32_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, int32_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, uint64_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, int64_t& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, float& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, double& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, long double& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, std::string& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::vec2& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::vec3& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::vec4& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::dvec2& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::dvec3& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::dvec4& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::ivec2& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::ivec3& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::ivec4& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::col3& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::col4& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::mat2& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, glm::mat3& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c key/data pair.
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API  void deserialize(const std::string& key, glm::mat4& data)
      throw (SerializationException);

    /**
    * Deserializes the given @c Serializable interface realization.
    *
    * @note All user defined classes must realize the @c Serializable
    *       interface to be deserializable.
    *
    * @param key the XML node key
    * @param data variable to store deserialized @c Serializable realization
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    TGT_API void deserialize(const std::string& key, Serializable& data)
      throw (SerializationException);

    /**
    * Deserializes the given pointer reference.
    *
    * @tparam type of referenced data
    *
    * @param key the XML node key
    * @param data variable to store deserialized pointer reference
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    */
    template<typename T>
    void deserialize(const std::string& key, T*& data)
      throw (SerializationException);

    /**
    * Deserializes a std::pair.
    *
    * @tparam S data type of first pair item
    * @tparam T data type of second pair item
    *
    * @param key the XML node key
    * @param data variable to store deserialized pair
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename S, typename T>
    void deserialize(const std::string& key, std::pair<S, T>& data)
      throw (SerializationException);

    /**
    * Deserializes a data vector.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data variable to store deserialized data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename T>
    void deserialize(const std::string& key, 
      std::vector<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Deserializes a data deque.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data variable to store deserialized data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename T>
    void deserialize(const std::string& key, 
      std::deque<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Deserializes a data list.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of vector items
    *
    * @param key the XML node key
    * @param data variable to store deserialized data vector
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename T>
    void deserialize(const std::string& key, 
      std::list<T>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Deserializes a data set.
    *
    * @note Element order of set items are not guaranteed to remains constant
    *       during serialization and deserialization due to limits of
    *       some STL containers like @c std::set.
    *
    * @tparam T data type of set items
    * @tparam C comparison class @see std::set
    *
    * @param key the XML node key
    * @param data variable to store deserialized data set
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    * @throws XmlSerializationInvalidOperationException
    *         if pointer content serialization is enabled,
    *         because of possible hash problems on deserialization
    */
    template<typename T, typename C>
    void deserialize(const std::string& key, 
      std::set<T, C>& data,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Deserializes a data map.
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
    * @param data variable to store deserialized data map
    * @param valueKey XML node key for each value node
    * @param keyKey key for each XML key node or attribute
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<class T, class U, class C>
    void deserialize(
      const std::string& key,
      std::map<T, U, C>& data,
      const std::string& valueKey = XmlSerializationConstants::VALUENODE,
      const std::string& keyKey = XmlSerializationConstants::KEYNODE)
      throw (SerializationException);

    /**
    * Reads the XML document from the given input stream after an optional XML preprocessor is applied.
    *
    * @param stream the input stream
    * @param xmlProcessor XML preprocessor
    *
    * @throws XmlSerializationFormatException if the XML document is incorrect formatted.
    * @throws XmlSerializationVersionMismatchException if the XML document serialization version
    *     does not match with the serialization version of this class.
    * @throws XmlSerializationReferenceResolvingException if there are references in
    *     the XML document which cannot be resolved.
    */
    TGT_API void read(std::istream& stream)
      throw (SerializationException);

  private:
    /**
    * Returns the next XML element with given @c key as name, which is not deserialized yet.
    *
    * @param key XML node name
    *
    * @returns the next XML element with given @c key as name
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    */
    TiXmlElement* getNextXmlElement(const std::string& key)
      throw (SerializationException);

    /**
    * Helper function for deserializing @c key/data pairs with simple data types.
    *
    * @tparam T data type
    *
    * @param key the XML node key
    * @param data variable to store deserialized data
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    */
    template<typename T>
    inline void deserializeSimpleTypes(const std::string& key, T& data)
      throw (SerializationException);

    /**
    * Helper function for deserializing glm vectors.
    *
    * @tparam T data type of vector
    *
    * @param key the XML node key
    * @param vector variable to store deserialized vector
    * @param isColor flag determine if given vector is a color
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    */
    template<typename T>
    inline void deserializeGlmVector(const std::string& key, T& vector, const bool& isColor = false)
      throw (SerializationException);

    /**
    * Helper function deserializes the given @c key/data pair from XML attribute named @c key
    * contained in the XML node @c nodeKey.
    *
    * @tparam T data type
    *
    * @param nodeKey the XML node key
    * @param key the XML attribute key
    * @param data variable to store deserialized data
    * @param markVisited if @c true the XML node with given @c nodeKey is marked as visited
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename T>
    inline void deserializeAttributeFromNode(const std::string& nodeKey, 
      const std::string& key, T& data, 
      const bool& markVisited = true)
      throw (SerializationException);

    /**
    * Helper function for deserializing data collections like STL container.
    *
    * Serialization using pointer content does not make sense for constant iterator items,
    * that is why this function ignores the setting concerning usage of pointer content
    * serialization.
    *
    * @note Element order of collection items remains constant during
    *       serialization and deserialization.
    *
    * @tparam T data type of collection
    *
    * @param key the XML node key
    * @param collection variable to store deserialized data collection
    * @param itemKey XML node key for each XML child node
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<typename T>
    inline void deserializeCollection(
      const std::string& key,
      T& collection,
      const std::string& itemKey = XmlSerializationConstants::ITEMNODE)
      throw (SerializationException);

    /**
    * Helper function for deserializing data maps like STL maps.
    *
    * @note Element order of map items are not guaranteed to remains constant
    *       during serialization and deserialization due to limits of
    *       some STL containers like @c std::map.
    *
    * @tparam T data type of map
    *
    * @param key the XML node key
    * @param map varaible to store deserialized data map
    * @param valueKey XML node key for each value node
    * @param keyKey key for each XML key node or attribute
    *
    * @throws XmlSerializationNoSuchDataException if no data with the given key can be found.
    * @throws XmlSerializationFormatException if a XML node is incorrect formatted.
    * @throws XmlSerializationDuplicateIdException if multiple XML nodes share same id attribute
    * @throws XmlSerializationMemoryAllocationException
    *         in case of trying to allocate memory for an @c AbstractSerializable
    *         or if there are not enough allocate items if pointer content serialization is enabled
    */
    template<class T>
    inline void deserializeMap(
      const std::string& key,
      T& map,
      const std::string& valueKey = XmlSerializationConstants::VALUENODE,
      const std::string& keyKey = XmlSerializationConstants::KEYNODE)
      throw (SerializationException);

    /**
    * Free memory of given type.
    */
    template<typename T>
    inline void freeMemory(T*& data);

    template<typename T>
    inline void freeMemory(T& data);

    /**
    * Allocates new memory of given type.
    */
    template<typename T>
    inline void allocateMemory(T*& data);

    template<typename T>
    inline void allocateMemory(T& data);

    /**
    * Type definition for set of visited XML nodes.
    */
    typedef std::set<TiXmlNode*> VisitedNodesSetType;

    /**
    * Set of already visited XML nodes.
    */
    VisitedNodesSetType visitedNodes_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  template<typename T>
  inline void XmlDeserializer::deserializeSimpleTypes(const std::string& key, T& data)
    throw (SerializationException)
  {
    TiXmlElement* element;
    if (useAttributes_)
      element = node_->ToElement();
    else
      element = getNextXmlElement(key);

    const char* value;

    if (useAttributes_) {
      value = element->Attribute(key.c_str());
      if (!value)
        raise(XmlSerializationNoSuchDataException("No attribute with key '" + key + "' found."));
    }
    else {
      // Is there no value attribute in XML element?
      value = element->Attribute(XmlSerializationConstants::VALUEATTRIBUTE);
      if (!value)
        raise(XmlSerializationFormatException("XML node '" + key + "' does not contain value attribute."));
    }

    convertStringToData(std::string(value), data);
  }

  template<>
  inline void XmlDeserializer::deserializeSimpleTypes<std::string>(const std::string& key, std::string& data)
    throw (SerializationException)
  {
    TiXmlElement* element;
    // Deserialize from XML attributes?
    if (useAttributes_)
      element = node_->ToElement();
    else
      element = getNextXmlElement(key);

    const char* value;
    // Deserialize from XML attributes?
    if (useAttributes_)
      value = element->Attribute(key.c_str());
    else
      value = element->Attribute(XmlSerializationConstants::VALUEATTRIBUTE);

    // Is there no matching attribute in XML element?
    if (value) {
      // ATTENTION: No special handling of the given string is needed that is why this block
      //            has to correspond with the not specialized serializeSimpleTypes method.
      data = std::string(value);
    }
    else {
      // Deserialize from XML attributes?
      if (useAttributes_)
        element = getNextXmlElement(key);

      // Is there no text in the XML element?
      if (!element->FirstChild() || element->FirstChild()->Type() != TiXmlElement::TINYXML_TEXT) {
        // Deserialize from XML attributes?
        if (useAttributes_)
          raise(XmlSerializationNoSuchDataException("Neither a '" + key + "' attribute nor text content data exists."));
        else
          raise(XmlSerializationFormatException("XML node '" + key + "' neither has a value attribute nor contains text data."));
      }

      data = std::string(element->FirstChild()->ToText()->Value());
    }
  }

  template<>
  inline void XmlDeserializer::deserializeSimpleTypes<Serializable>(const std::string& key, Serializable& data)
    throw (SerializationException)
  {
    TiXmlElement* element = getNextXmlElement(key);

    TemporaryNodeChanger nodeChanger(*this, element);

    data.deserialize(*this);
  }

  template<typename T>
  void XmlDeserializer::deserialize(const std::string& key, T*& data)
    throw (SerializationException)
  {
    deserialize(key, *data);
  }

  template<typename S, typename T>
  void XmlDeserializer::deserialize(const std::string& key, std::pair<S, T>& data)
    throw (SerializationException)
  {
    TiXmlElement* element = getNextXmlElement(key);

    TemporaryNodeChanger nodeChanger(*this, element);

    // first item
    if (useAttributes_ && isPrimitiveType(typeid(S))) {
      // deserialize primitive type from XML attribute
      deserializeAttributeFromNode("First", XmlSerializationConstants::VALUEATTRIBUTE, data.first);
    }
    else {
      deserialize("First", data.first);
    }

    // second item
    if (useAttributes_ && isPrimitiveType(typeid(T))) {
      // deserialize primitive type from XML attribute
      deserializeAttributeFromNode("Second", XmlSerializationConstants::VALUEATTRIBUTE, data.second);
    }
    else {
      deserialize("Second", data.second);
    }
  }

  template<typename T>
  void XmlDeserializer::deserialize(const std::string& key, std::vector<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    deserializeCollection(key, data, itemKey);
  }

  template<typename T>
  void XmlDeserializer::deserialize(const std::string& key, std::deque<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    deserializeCollection(key, data, itemKey);
  }

  template<typename T>
  void XmlDeserializer::deserialize(const std::string& key, std::list<T>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    deserializeCollection(key, data, itemKey);
  }

  template<typename T, typename C>
  void XmlDeserializer::deserialize(const std::string& key, std::set<T, C>& data, const std::string& itemKey)
    throw (SerializationException)
  {
    deserializeCollection(key, data, itemKey);
  }

  template<class T, class U, class C>
  void XmlDeserializer::deserialize(const std::string& key,
    std::map<T, U, C>& data,
    const std::string& valueKey,
    const std::string& keyKey)
    throw (SerializationException)
  {
    deserializeMap(key, data, valueKey, keyKey);
  }

  template<typename T>
  inline void XmlDeserializer::deserializeGlmVector(const std::string& key, T& vector, const bool& isColor)
    throw (SerializationException)
  {
    TiXmlElement* element = getNextXmlElement(key);

    const char* x;
    const char* y;
    const char* z;
    const char* w;
    if (isColor) {
      x = element->Attribute(XmlSerializationConstants::COLORRATTRIBUTE);
      y = element->Attribute(XmlSerializationConstants::COLORGATTRIBUTE);
      z = element->Attribute(XmlSerializationConstants::COLORBATTRIBUTE);
      w = element->Attribute(XmlSerializationConstants::COLORAATTRIBUTE);
    }
    else {
      x = element->Attribute(XmlSerializationConstants::VECTORXATTRIBUTE);
      y = element->Attribute(XmlSerializationConstants::VECTORYATTRIBUTE);
      z = element->Attribute(XmlSerializationConstants::VECTORZATTRIBUTE);
      w = element->Attribute(XmlSerializationConstants::VECTORWATTRIBUTE);
    }

    // Is there no x attribute?
    if (!x)
      raise(XmlSerializationFormatException("Xml node '" + key + "' has no x-attribute."));

    if (isColor) {
      // ATTENTION: The string to int to type conversion is mandatory,
      //            since glm::col3, glm::col4 are vector of UCHARs.
      //            Otherwise, strings like "153" are converted to '1' instead of 153.
      int value;
      convertStringToData(std::string(x), value);
      vector[0] = static_cast<typename T::value_type>(value);
    }
    else
      convertStringToData(std::string(x), vector[0]);

    // Y attribute expected?
    if (vector.length() >= 2) {
      // Is there no y attribute?
      if (!y)
        raise(XmlSerializationFormatException("Xml node '" + key + "' has no y-attribute."));

      if (isColor) {
        // ATTENTION: The string to int to type conversion is mandatory,
        //            since glm::col3, glm::col4 are vector of UCHARs.
        //            Otherwise, strings like "153" are converted to '1' instead of 153.
        int value;
        convertStringToData(std::string(y), value);
        vector[1] = static_cast<typename T::value_type>(value);
      }
      else
        convertStringToData(std::string(y), vector[1]);
    }

    // Z attribute expected?
    if (vector.length() >= 3) {
      // Is there no z attribute?
      if (!z)
        raise(XmlSerializationFormatException("Xml node '" + key + "' has no z-attribute."));

      if (isColor) {
        // ATTENTION: The string to int to type conversion is mandatory,
        //            since glm::col3, glm::col4 are vector of UCHARs.
        //            Otherwise, strings like "153" are converted to '1' instead of 153.
        int value;
        convertStringToData(std::string(z), value);
        vector[2] = static_cast<typename T::value_type>(value);
      }
      else
        convertStringToData(std::string(z), vector[2]);
    }

    // W attribute expected?
    if (vector.length() >= 4) {
      // Is there no w attribute?
      if (!w)
        raise(XmlSerializationFormatException("Xml node '" + key + "' has no w-attribute."));

      if (isColor) {
        // ATTENTION: The string to int to type conversion is mandatory,
        //            since glm::col3, glm::col4 are vector of UCHARs.
        //            Otherwise, strings like "153" are converted to '1' instead of 153.
        int value;
        convertStringToData(std::string(w), value);
        vector[3] = static_cast<typename T::value_type>(value);
      }
      else
        convertStringToData(std::string(w), vector[3]);
    }
  }

  template<typename T>
  inline void XmlDeserializer::deserializeAttributeFromNode(const std::string& nodeKey, const std::string& key, T& data, const bool& markVisited)
    throw (SerializationException)
  {
    TemporaryNodeChanger nodeChanger(*this, getNextXmlElement(nodeKey));

    deserialize(key, data);

    if (!markVisited)
      visitedNodes_.erase(node_);
  }

  template<typename T>
  inline void XmlDeserializer::deserializeCollection(const std::string& key, T& collection, const std::string& itemKey)
    throw (SerializationException)
  {
    TiXmlElement* element = getNextXmlElement(key);

    TemporaryNodeChanger nodeChanger(*this, element);

    typename T::value_type item;

    try {
      for (typename T::iterator it = collection.begin(); it != collection.end(); ++it) {
        freeMemory(*it);
      }
      collection.clear();

      while (true) {
        allocateMemory(item);

        try {
          // Deserialize primitive type from XML attribute?
          if (useAttributes_ && isPrimitiveType(typeid(typename T::value_type))) {
            deserializeAttributeFromNode(itemKey, XmlSerializationConstants::VALUEATTRIBUTE, item);
          }
          else
            deserialize(itemKey, item);

          collection.insert(collection.end(), item);
        }
        catch (XmlSerializationMemoryAllocationException&) {
          // Just ignore items for which no memory was allocatable...
        }
      }
    }
    catch (XmlSerializationNoSuchDataException&) {
      // There are just no more collection items, just ignore the exception...
      freeMemory(item);
    }
  }

  template<class T>
  inline void XmlDeserializer::deserializeMap(const std::string& keystr,
    T& map,
    const std::string& valueKey,
    const std::string& keyKey)
    throw (SerializationException)
  {
    TiXmlElement* element = getNextXmlElement(keystr);

    TemporaryNodeChanger nodeChanger(*this, element);

    typename T::key_type key;
    typename T::mapped_type value;

    try {
      for (typename T::iterator it = map.begin(); it != map.end(); ++it) {
        freeMemory(it->first);
        freeMemory(it->second);
      }
      map.clear();

      while (true) {
        allocateMemory(key);
        allocateMemory(value);

        bool memoryAllocationExceptionCatched = false;

        try {
          // Deserialize primitive key type as XML attribute?
          if (useAttributes_ && isPrimitiveType(typeid(typename T::key_type))) {
            // ATTENTION: We have to set element unvisited in order to deserialize its content later.
            deserializeAttributeFromNode(valueKey, keyKey, key, false);
          }
          else
            deserialize(keyKey, key);
        }
        catch (XmlSerializationMemoryAllocationException&) {
          memoryAllocationExceptionCatched = true;
        }

        // Always deserialize pointer from content?
        try {
          // Deserialize primitive mapped type as XML attribute?
          if (useAttributes_ && isPrimitiveType(typeid(typename T::mapped_type)))
            deserializeAttributeFromNode(valueKey, XmlSerializationConstants::VALUEATTRIBUTE, value);
          // Deserialize primitive mapped type from XML node
          else
            deserialize(valueKey, value);
        }
        catch (XmlSerializationMemoryAllocationException&) {
          memoryAllocationExceptionCatched = true;
        }

        if (!memoryAllocationExceptionCatched)
          map[key] = value;
      }
    }
    catch (XmlSerializationNoSuchDataException&) {
      // There are just no more collection items, just ignore the exception...
      freeMemory(key);
      freeMemory(value);
    }
  }

  template<typename T> inline void XmlDeserializer::freeMemory(T& data) {}
  template<typename T> inline void XmlDeserializer::freeMemory(T*& data) { delete data; }

  template<typename T> inline void XmlDeserializer::allocateMemory(T& data) {}
  template<typename T> inline void XmlDeserializer::allocateMemory(T*& data) { data = new T(); }

} // end namespace tgt