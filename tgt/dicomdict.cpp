#include "dicomdict.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"
#include "filesystem.h"

namespace tgt {

  DicomDict::DicomDict()
  {
  }

  void DicomDict::serialize(XmlSerializer &s) const {
    //put DictEntries into a Vector
    std::vector<DicomDictEntry> entryVector;

    std::map<std::string, DicomDictEntry>::const_iterator it;
    for (it = entries_.begin(); it != entries_.end(); ++it) {
      entryVector.push_back(it->second);
    }

    //serialize this vector
    s.setUseAttributes(false);
    s.serialize("Dict", entryVector, "entry");
  }

  void DicomDict::deserialize(XmlDeserializer &d) throw (Exception) {
    //get Vector of DictEntries
    std::vector<DicomDictEntry> entryVector;

    //deserialize Vector
    d.setUseAttributes(false);
    d.deserialize("Dict", entryVector, "entry");

    //iterate over Vector and put in map
    std::vector<DicomDictEntry>::iterator it;
    for (it = entryVector.begin(); it != entryVector.end(); ++it) {
      //check if keyword is already in entries_
      if (entries_.find(it->getKeyword()) != entries_.end()) {
        throw Exception("Keyword in Dictionary must be unique! Found keyword more than once: " + it->getKeyword());
      }

      entries_[it->getKeyword()] = *it;
    }
  }

  const DicomDictEntry DicomDict::getDictEntryByKeyword(const std::string &keyword) const throw (FileException){
    if (entries_.find(keyword) == entries_.end())
      throw FileException("Keyword could not be found in Dictionary: " + keyword);
    else
      return entries_.find(keyword)->second;
  }

  DicomDict* DicomDict::loadFromFile(const std::string &fileName) throw (FileException) {
    //Check the given file
    if (!(FileSystem::fileExists(fileName)))
      throw FileNotFoundException("Dictionary not found", fileName);

    if (!(FileSystem::fileExtension(fileName) == "xml"))
      throw FileAccessException("Dictionary is not an XML file", fileName);

    //get Data from the file
    std::string xmlData;
    FileSystem sys;
    File* file = sys.open(fileName);

    if (!file->good())
      throw FileAccessException("Dictionary file cannot be accessed", fileName);

    xmlData = file->getAsString();

    if (file->isOpen())
      file->close();

    delete file;
    file = 0;

    DicomDict* dict = new DicomDict();

    //Put Data into a Stream to be read by XmlDeserializer and try to deserialize it
    std::stringstream stream;
    stream << xmlData;

    XmlDeserializer d;
    d.read(stream);

    try {
      dict->deserialize(d);
    }
    catch (Exception e) {
      throw FileAccessException("Could not load Dictionary: " + std::string(e.what()), fileName);
    }

    return dict;
  }

  bool DicomDict::containsKeyword(const std::string &key) const {
    return (entries_.count(key) == 1);
  }

  const std::vector<std::string> DicomDict::getKeywordVector() const {
    std::vector<std::string> v;

    std::map<std::string, DicomDictEntry>::const_iterator it;
    for (it = entries_.begin(); it != entries_.end(); ++it) {
      v.push_back(it->first);
    }

    return v;
  }

} // end namespace tgt
