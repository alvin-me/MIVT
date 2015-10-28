#include "metadatacontainer.h"

namespace tgt {

  MetaDataContainer::MetaDataContainer(const MetaDataContainer& mdc) {
    for (std::map<std::string, MetaDataBase*>::const_iterator it = mdc.metaData_.begin(); it != mdc.metaData_.end(); ++it) {
      const MetaDataBase* md = it->second;
      addMetaData(it->first, md->clone());
    }
  }

  MetaDataContainer::~MetaDataContainer() {
    clearMetaData();
  }

  void MetaDataContainer::addMetaData(const std::string& key, MetaDataBase* meta) {
    removeMetaData(key);
    metaData_[key] = meta;
  }

  bool MetaDataContainer::hasMetaData(const std::string& key) const {
    return metaData_.find(key) != metaData_.end();
  }

  MetaDataBase* MetaDataContainer::getMetaData(const std::string& key) {
    if (hasMetaData(key))
      return metaData_[key];
    else
      return 0;
  }

  const MetaDataBase* MetaDataContainer::getMetaData(const std::string& key) const {
    if (hasMetaData(key))
      return metaData_.find(key)->second;//metaData_.at(key); the function "at" does not exists under Visual Studio 2008
    else
      return 0;
  }

  void MetaDataContainer::removeMetaData(const std::string& key) {
    if (!hasMetaData(key))
      return;

    delete metaData_[key];
    metaData_.erase(metaData_.find(key));
  }

  void MetaDataContainer::renameMetaData(const std::string& oldKey, const std::string& newKey) {
    if (!hasMetaData(oldKey) || hasMetaData((newKey)))
      return;

    metaData_[newKey] = metaData_[oldKey];
    metaData_.erase(metaData_.find(oldKey));
  }

  void MetaDataContainer::clearMetaData() {
    for (std::map<std::string, MetaDataBase*>::iterator it = metaData_.begin(); it != metaData_.end(); ++it)
      delete it->second;

    metaData_.clear();
  }

  void MetaDataContainer::serialize(XmlSerializer& s) const {
    if (metaData_.empty())
      return;

    s.serialize("MetaData", metaData_, "MetaItem", "name");
  }

  void MetaDataContainer::deserialize(XmlDeserializer& s) {
    clearMetaData();

    try {
      s.deserialize("MetaData", metaData_, "MetaItem", "name");
    }
    catch (XmlSerializationNoSuchDataException&) {
      // There were no meta data during serialization, so we can just ignore the exception...
    }
  }

  std::vector<std::string> MetaDataContainer::getKeys() const {
    std::vector<std::string> keys;
    for (std::map<std::string, MetaDataBase*>::const_iterator it = metaData_.begin(); it != metaData_.end(); ++it) {
      keys.push_back(it->first);
    }
    return keys;
  }

  MetaDataContainer& MetaDataContainer::operator=(const MetaDataContainer& mdc) {
    clearMetaData();
    for (std::map<std::string, MetaDataBase*>::const_iterator it = mdc.metaData_.begin(); it != mdc.metaData_.end(); ++it) {
      const MetaDataBase* md = it->second;
      addMetaData(it->first, md->clone());
    }
    return *this;
  }

} // end namespace tgt