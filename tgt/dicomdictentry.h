#pragma once

#include "serializable.h"
#include <string>
#include <stdint.h>

namespace tgt {

  class DicomDictEntry : public Serializable
  {
  public:
    TGT_API DicomDictEntry();

    TGT_API DicomDictEntry(std::string keyword, std::string name, std::string vm, std::string vr, bool isMetaData);

    /**
    * Serializes all member variables using the given XmlSerializer
    *
    * @param s the XmlSerializer to be used
    */
    TGT_API  virtual void serialize(XmlSerializer& s) const;

    /**
    * Deserializes all member variables using the given XmlDeserializer
    *
    * @param s the XmlDeserializer to be used
    */
    TGT_API virtual void deserialize(XmlDeserializer& s);

    /**
    * Sets the group number.
    *
    * @param group must be of the following form: gggg (where g is a hexadecimal digit), otherwise it is set to 0
    */
    TGT_API void setGroupNumber(std::string group);

    /**
    * Sets the element number.
    *
    * @param element must be of the following form: eeee (where e is a hexadecimal digit), otherwise it is set to 0
    */
    TGT_API void setElementNumber(std::string element);

    /**
    * Returns a std::string representation of the DictEntry
    */
    TGT_API std::string toString() const;

    /**
    * get the keyword as a string
    */
    TGT_API std::string getKeyword() const;

    /**
    * get VR as string
    */
    TGT_API std::string getValueRepresentation() const;

    /**
    * get VM as string
    */
    TGT_API std::string getValueMultiplicity() const;

    /**
    * get name as string
    */
    TGT_API std::string getName() const;

    /**
    * get group number as (hex-)string
    */
    TGT_API std::string getGroupNumber() const;

    /**
    * get element number as (hex-)string
    */
    TGT_API std::string getElementNumber() const;

    /**
    * sould the value of this Entry be added to the Meta Information of the Volume?
    */
    TGT_API bool isMetaData() const;

    /**
    * get group number as uint16_t
    */
    TGT_API uint16_t getGroupTagUint16() const;

    /**
    * get group number as uint16_t
    */
    TGT_API uint16_t getElementTagUint16() const;

  private:
    std::string keyword_; ///< The keyword to find the entry
    std::string vm_;      ///< The Value Multiplicity
    std::string vr_;      ///< The Value Representation
    std::string name_;    ///< The Name of the Entry
    std::string groupNumber_; ///< The Tag Group Number
    std::string elementNumber_; ///< The Tag Element Number
    bool metaData_; ///< implies if this entry (ie. it's value in the loaded Files) should be added to the Volume's MetaDataContainer
  };

} // end namespace tgt
