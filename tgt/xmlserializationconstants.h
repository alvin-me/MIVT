#pragma once

#include <string>

namespace tgt {

  /**
  * The XmlSerializationConstants class defines serialization constants, which are shared
  * by XmlSerializer and XmlDeserializer.
  */
  class XmlSerializationConstants {
  public:
    /**
    * XML version attribute in XML declaration.
    */
    static const char* XMLVERSION;

    /**
    * XML encoding attribute in XML declaration.
    */
    static const char* XMLENCODING;

    /**
    * XML standalone attribute in XML declaration.
    */
    static const char* XMLSTANDALONE;

    /**
    * XML root node name of serialized XML document.
    */
    static const char* ROOTNODE;

    /**
    * Version attribute name.
    *
    * @note To obtain finer version control, you can set a version attribute for every XML node.
    */
    static const char* VERSIONATTRIBUTE;

    /**
    * Version of XmlSerializer and XmlDeserializer.
    */
    static const char* VERSION;

    /**
    * Id attribute name for reference resolving purposes.
    */
    static const char* IDATTRIBUTE;

    /**
    * Reference attribute name for reference resolving purposes.
    */
    static const char* REFERENCEATTRIBUTE;

    /**
    * Type attribute name for polymorphism purposes.
    */
    static const char* TYPEATTRIBUTE;

    /**
    * Name of the value attribute which contains the content of simple types.
    */
    static const char* VALUEATTRIBUTE;

    /**
    * Id prefix for reference resolving purposes.
    */
    static const char* IDPREFIX;

    /**
    * Name of item nodes for collection serialization.
    */
    static const char* ITEMNODE;

    /**
    * Name of key nodes for key/value pair serialization of maps.
    */
    static const char* KEYNODE;

    /**
    * Name of value nodes for key/value pair serialization of maps.
    */
    static const char* VALUENODE;

    /**
    * Name of temporary XML nodes.
    */
    static const char* TEMPNODE;

    /**
    * Attribute name of vector's x element.
    */
    static const char* VECTORXATTRIBUTE;

    /**
    * Attribute name of vector's y element.
    */
    static const char* VECTORYATTRIBUTE;

    /**
    * Attribute name of vector's z element.
    */
    static const char* VECTORZATTRIBUTE;

    /**
    * Attribute name of vector's w element.
    */
    static const char* VECTORWATTRIBUTE;

    /**
    * Attribute name of color's r element.
    */
    static const char* COLORRATTRIBUTE;

    /**
    * Attribute name of color's g element.
    */
    static const char* COLORGATTRIBUTE;

    /**
    * Attribute name of color's b element.
    */
    static const char* COLORBATTRIBUTE;

    /**
    * Attribute name of color's a element.
    */
    static const char* COLORAATTRIBUTE;
  };

} // end namespace tgt


