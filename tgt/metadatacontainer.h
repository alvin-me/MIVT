#pragma once

#include "metadatabase.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"

#include <vector>
#include <map>

namespace tgt {

  class MetaDataContainer : public Serializable {

    friend class XmlSerializer;
    friend class XmlDeserializer;

  public:
    TGT_API MetaDataContainer() {}
    TGT_API MetaDataContainer(const MetaDataContainer& mdc);
    TGT_API MetaDataContainer& operator=(const MetaDataContainer& mdc);
    TGT_API virtual ~MetaDataContainer();

    /**
    * Adds the given @c key/meta-data pair to the meta data associated with the @c Processor.
    *
    * @attention Already existing meta data with the same key is overwritten.
    *
    * @param key the key
    * @param meta the meta-data to add
    */
    TGT_API virtual void addMetaData(const std::string& key, MetaDataBase* meta);

    /**
    * Returns if meta data to the given key exists.
    *
    * @param key the key
    * @returns @c true if meta data exists, otherwise @c false.
    */
    TGT_API virtual bool hasMetaData(const std::string& key) const;

    TGT_API virtual std::vector<std::string> getKeys() const;

    /**
    * Returns meta data to the given key.
    *
    * @param key the key
    * @returns the meta data to the given key if it exists, otherwise @c 0.
    */
    TGT_API virtual MetaDataBase* getMetaData(const std::string& key);

    ///@overload
    TGT_API virtual const MetaDataBase* getMetaData(const std::string& key) const;

    /**
    * Removes meta data with the given key. If no meta data exists, nothing happens
    *
    * @param key the key
    */
    TGT_API virtual void removeMetaData(const std::string& key);

    /**
    * @brief Changes the key of a MetaData entry.
    * Does nothing if the key doesn't exist.
    */
    TGT_API virtual void renameMetaData(const std::string& oldKey, const std::string& newKey);

    /**
    * Deletes all associated meta data.
    */
    TGT_API virtual void clearMetaData();

    /**
    * @see Serializable::serialize
    */
    TGT_API virtual void serialize(XmlSerializer& s) const;

    /**
    * @see Serializable::deserialize
    */
    TGT_API virtual void deserialize(XmlDeserializer& s);

  private:
    /**
    * Stored meta data.
    */
    std::map<std::string, MetaDataBase*> metaData_;
  };

} // end namespace tgt