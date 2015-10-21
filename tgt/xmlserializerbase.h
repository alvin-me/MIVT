#pragma once

#include "serializationexceptions.h"
#include "tgt_math.h"

#include <string>
#include <stdint.h>
#include <sstream>
#include <tinyxml/tinyxml.h>

namespace tgt {

  /**
  * The XmlSerializerBase class encapsulates functions that are common to XmlSerializer
  * and XmlDeserializer.
  */
  class XmlSerializerBase
  {
  public:
    TGT_API XmlSerializerBase();
    
    TGT_API ~XmlSerializerBase();

    /**
    * Returns whether primitive data is serialized as XML attributes or XML nodes.
    *
    * @return true if primitive data is serialized as XML attributes and false otherwise
    */
    TGT_API bool getUseAttributes() const;

    /**
    * Sets whether to serialize primitive data as XML attributes or XML nodes.
    * @param useAttributes if true serialize primitive data as XML attributes,
    *                      otherwise as XML nodes
    */
    TGT_API void setUseAttributes(const bool& useAttributes);

  protected:
    /**
    * Returns if given type is a primitive data type.
    *
    * @return @c true if it is a primitive data type and @c false otherwise
    */
    bool isPrimitiveType(const std::type_info& type) const;

    /**
    * Returns if given type is a primitive data pointer type.
    *
    * @return @c true if it is a primitive data pointer type and @c false otherwise.
    */
    bool isPrimitivePointerType(const std::type_info& type) const;

    /**
    * Converts the given data to std::string.
    *
    * @tparam T type of data to convert
    * @param data data to convert
    * @return the string
    */
    template<class T>
    inline std::string convertDataToString(const T& data);

    std::string convertDataToString(const char* data);

    std::string convertDataToString(const float& data);

    /**
    * Converts the given std::string to data.
    *
    * @tparam T type of data to convert
    * @param str the string
    * @param data variable to store converted data
    */
    template<class T>
    inline void convertStringToData(const std::string& str, T& data);

    /**
    * Adds the error message from the given exception to the error list
    * and raise the exception afterwards.
    *
    * @tparam T exception type
    * @param exception the exception
    * @throws SerializationException the exception is always thrown
    */
    template<class T>
    void raise(const T& exception)
      throw (SerializationException);

    /**
    * This is a helper class to ensure correct state of the XML node
    * for inserting or reading data.
    *
    * @note As C++ does not support a finally block statement, we need this
    *       class to ensure that cleanup code concerning the XML node
    *       for inserting or reading data is executed.
    */
    class TemporaryNodeChanger {
    public:
      /**
      * Creates a TemporaryNodeChange, which changes the actual XML node for
      * inserting or reading data.
      *
      * @param serializer serializer whose XML node should be changed
      * @param node the new node
      */
      TemporaryNodeChanger(XmlSerializerBase& serializer, TiXmlNode* node);

      /**
      * Destructor ensures restoring the XML node for inserting or reading data
      * which was set before this instance was created.
      */
      ~TemporaryNodeChanger();

      TemporaryNodeChanger & operator=(const TemporaryNodeChanger &) { return *this; }

    private:
      /**
      * Serializer whose XML node is changed.
      */
      XmlSerializerBase& serializer_;

      /**
      * XML node which was set before this TemporaryNodeChanger was created.
      */
      TiXmlNode* storedNode_;
    };

    /**
    * XML document that contains the already serialized or deserialized data.
    * @attention Keep in mind that may not all pointer references are already resolved.
    */
    TiXmlDocument document_;

    /**
    * XML node for inserting or reading data.
    */
    friend class TemporaryNodeChanger;
    TiXmlNode* node_;

    /**
    * If true all primitive data is serialized as XML attributes, otherwise as XML nodes.
    */
    bool useAttributes_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  template<class T>
  inline std::string XmlSerializerBase::convertDataToString(const T& data) {
    std::stringstream stream;
    stream << data;
    return stream.str();
  }

  template<class T>
  inline void XmlSerializerBase::convertStringToData(const std::string& str, T& data) {
    std::stringstream stream;
    stream << str;
    stream >> data;
  }

  template<class T>
  void XmlSerializerBase::raise(const T& exception)
    throw (SerializationException)
  {
    throw exception;
  }

} // end namespace tgt