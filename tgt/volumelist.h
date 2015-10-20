#pragma once

#include "volume.h"

namespace tgt {

  /**
  * The volume list does not perform memory management, i.e., added volumes
  * are not deleted on removal or destruction of the list.
  */
  class VolumeList {

  public:

    TGT_API VolumeList();

    TGT_API virtual ~VolumeList();

    /**
    * Adds the passed volume to the list,
    * if it is not already contained.
    *
    * The VolumeList does not take ownership of the
    * added Volume and does therefore not delete it
    * on its own destruction.
    */
    TGT_API virtual void add(Volume* volumeHandle);

    /**
    * Adds volume contained by the passed VolumeList
    * to the list, if they are not already contained.
    *
    * The VolumeList does not take ownership of the
    * added volume and does therefore not delete them
    * on its own destruction.
    */
    TGT_API virtual void add(const VolumeList* volumeList);

    /**
    * Removes the passed volume from the VolumeList
    * without deleting it.
    */
    TGT_API virtual void remove(const Volume* volume);

    /**
    * Removes all volume contained by the passed list
    * without deleting them.
    */
    TGT_API virtual void remove(const VolumeList* volumeList);

    /**
    * Returns whether the passed volume is contained
    * by the list.
    */
    TGT_API virtual bool contains(const Volume* volume) const;

    /**
    * Returns the VolumeElement at a specified index position.
    *
    * @param The index of the volume to return. Must be valid, i.e. i < size().
    */
    TGT_API virtual Volume* at(size_t i) const;

    /**
    * Returns the first volume of the list, or null
    * if the list is empty.
    */
    TGT_API virtual Volume* first() const;

    /**
    * Clears the list without deleting the volumes.
    */
    TGT_API virtual void clear();

    /**
    * Returns the number of volumes contained by the list.
    */
    TGT_API virtual size_t size() const;

    /**
    * Returns whether the list is currently empty.
    */
    TGT_API virtual bool empty() const;

  protected:
    /**
    * Returns an iterator pointing to the position of the passed Volume
    * within the volumeHandles_ vector. Returns volumeHandles_.end(), if
    * the handle is not contained by the list.
    */
    std::vector<Volume*>::iterator find(const Volume* volume);
    std::vector<Volume*>::const_iterator find(const Volume* volume) const;

    /// Vector storing the volumes contained by the list.
    std::vector<Volume*> volumes_;

    /// category for logging.
    static const std::string loggerCat_;
  };

} // end namespace tgt
