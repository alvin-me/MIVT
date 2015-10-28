#pragma once

#include "serializable.h"
#include "dicomdictentry.h"
#include "exception.h"
#include <vector>
#include <map>

namespace tgt {

  class DicomDict
  {
  public:
    TGT_API DicomDict();

    /**
    * Serializes all member variables using the given XmlSerializer
    *
    * @param s the XmlSerializer to be used
    */
    TGT_API virtual void serialize(XmlSerializer& s) const;

    /**
    * Deserializes all member variables using the given XmlDeserializer.
    * If a keyword occurs more than once, a Exception is thrown
    *
    * @param s the XmlDeserializer to be used
    */
    TGT_API virtual void deserialize(XmlDeserializer& s) throw (Exception);

    /**
    * Returns the DicomDictEntry associated with the keyword.
    * If no entry with this keyword can be found, a FileException will be thrown
    *
    * @param keyword the keyword by which the entry is identified
    */
    TGT_API const DicomDictEntry getDictEntryByKeyword(const std::string &keyword) const throw (FileException);

    /**
    * Loads a DicomDict from an XML file and returns a pointer to it.
    * If the file is corrupt, can not be found, or a keyword occurs more than once, a FileException is thrown
    *
    * @param fileName the file to be loaded
    */
    TGT_API static DicomDict* loadFromFile(const std::string &fileName) throw (FileException);

    /**
    * returns true if the Dict contains an entry with the given keyword
    *
    * @param key the keyword to be lokked for
    */
    TGT_API bool containsKeyword(const std::string &key) const;

    /**
    * returns a std::vector containing all keywords in the DicomDict
    */
    TGT_API const std::vector<std::string> getKeywordVector() const;

  private:
    std::map<std::string, DicomDictEntry> entries_; ///< the actual map containing the entries with their corresponding keywords as keys
  };

} // end namespace tgt
