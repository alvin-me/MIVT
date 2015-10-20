#include "volumelist.h"
#include "logmanager.h"

namespace tgt {

  const std::string VolumeList::loggerCat_ = "VolumeList";

  VolumeList::VolumeList()
  {
  }

  VolumeList::~VolumeList() {
    clear();
  }

  void VolumeList::add(Volume* volume) {

    if (!volume) {
      LERROR("Null pointer as Volume passed");
      return;
    }
    if (!contains(volume)) {
      volumes_.push_back(volume);
    }
  }

  void VolumeList::add(const VolumeList* volumeList) {

    if (!volumeList) {
      LERROR("Unexpected null pointer");
      return;
    }
    for (size_t i = 0; i<volumeList->size(); ++i) {
      add(volumeList->at(i));
    }
  }

  void VolumeList::remove(const Volume* volume){
    std::vector<Volume*>::iterator handleIter = find(volume);
    if (handleIter != volumes_.end()) {
      volumes_.erase(handleIter);
    }
  }

  void VolumeList::remove(const VolumeList* volumeList) {
    if (!volumeList) {
      LERROR("Unexpected null pointer");
      return;
    }
    for (size_t i = 0; i<volumeList->size(); ++i) {
      remove(volumeList->at(i));
    }
  }

  std::vector<Volume*>::iterator VolumeList::find(const Volume* volume) {
    return std::find(volumes_.begin(), volumes_.end(), volume);
  }

  std::vector<Volume*>::const_iterator VolumeList::find(const Volume* volume) const {
    return std::find(volumes_.begin(), volumes_.end(), volume);
  }

  bool VolumeList::contains(const Volume* volume) const {
    return (find(volume) != volumes_.end());
  }

  Volume* VolumeList::at(size_t i) const {
    if (i >= volumes_.size()) {
      LERROR("Invalid index");
      return 0;
    }
    return volumes_.at(i);
  }

  Volume* VolumeList::first() const {
    return (!empty() ? volumes_.front() : 0);
  }

  void VolumeList::clear() {
    while (!empty())
      remove(first());
  }

  size_t VolumeList::size() const {
    return volumes_.size();
  }

  bool VolumeList::empty() const {
    return (size() == 0);
  }

} // end namespace tgt
