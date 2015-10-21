#pragma once

#include "exception.h"

namespace tgt {

  /**
  * Base class of all serialization exceptions.
  */
  class SerializationException : public Exception {
  public:
    /**
    * @see VoreenException::VoreenException
    */
    TGT_API SerializationException(const std::string& what = "") : Exception(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * Base class of all serialization exceptions concerning reading and writing XML.
  * @see SerializationException
  */
  class XmlSerializationException : public SerializationException {
  public:
    /**
    * @see SerializationException::SerializationException
    */
    TGT_API XmlSerializationException(const std::string& what = "") : SerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationInvalidOperationException is raised in case of a method-call in an
  * invalid, respectively unsupported way.
  */
  class XmlSerializationInvalidOperationException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationInvalidOperationException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationFormatException is raised when XML nodes does not fulfill expected format.
  */
  class XmlSerializationFormatException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationFormatException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationVersionMismatchException is raised in case of a version mismatch among
  * the XML document and the used @c XmlSerializer or @c XmlDeserializer.
  */
  class XmlSerializationVersionMismatchException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationVersionMismatchException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationNoSuchDataException is raised in case of searching for a XML node
  * by key that does not exists.
  */
  class XmlSerializationNoSuchDataException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationNoSuchDataException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationDuplicateIdException is raised in case of multiple XML nodes
  * share the same id attribute.
  */
  class XmlSerializationDuplicateIdException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationDuplicateIdException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationAttributeNamingException is raised in case of multiple XML attributes
  * sharing the same name or using a reserved attribute name.
  */
  class XmlSerializationAttributeNamingException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationAttributeNamingException(const std::string& what = "") : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationReferenceResolvingException is raised in case of problems
  * concerning the reference resolving process.
  */
  class XmlSerializationReferenceResolvingException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationReferenceResolvingException(const std::string& what = "")
      : XmlSerializationException(what) {}
  };

  //----------------------------------------------------------------------------

  /**
  * A @c XmlSerializationMemoryAllocationException is raised in case of trying to allocate memory
  * for an @c AbstractSerializable.
  */
  class XmlSerializationMemoryAllocationException : public XmlSerializationException {
  public:
    TGT_API XmlSerializationMemoryAllocationException(const std::string& what = "") : XmlSerializationException(what) {}
  };

} // end namespace tgt