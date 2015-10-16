#pragma once

#include "filesystem.h"
#include "logmanager.h"

#include <vector>
#include <map>
#include <list>

namespace tgt {

  template <class T>
  class ResourceManager {
  public:
    /// Constructor
    ResourceManager(bool cacheResources = true);

    /// Destroy all resources in memory
    virtual ~ResourceManager();

    /// Check if resource is loaded
    bool isLoaded(const std::string& filename);

    /// Mark resource as unused
    virtual void dispose(T* ptr);

    // getter - setter
    void addPath(std::string path);
    void removePath(std::string path);

    /**
    * Searches in all paths for file and returns valid filename including complete path.
    * If file is not found in search path, an empty string is returned.
    */
    std::string completePath(std::string filename);

    bool isCaching() const { return cacheResources_; }

    std::vector<std::string> getFilenames();

  protected:
    struct Resource {
      T*          data_;
      uint64_t    usedBy_;
      std::string filename_;
    };

    std::map< std::string, Resource* >  resourcesByFilename_;
    std::map< T*, Resource* >           resourcesByPtr_;
    bool cacheResources_;

    std::list<std::string> pathList_;

    void reg(T *ptr, const std::string& filename);
    void increaseUsage(const std::string& filename);

    static const std::string loggerCat_;

    // getter - setter
    T* get(const std::string& filename);
    //const T* get(const std::string& filename) const;
  };

  //------------------------------------------------------------------------------

  template <class T>
  const std::string ResourceManager<T>::loggerCat_("ResourceManager");

  template <class T>
  T* ResourceManager<T>::get(const std::string& filename) {
    return resourcesByFilename_[filename]->data_;
  }

  template <class T>
  void ResourceManager<T>::reg(T* ptr, const std::string& filename) {
    if (cacheResources_ && isLoaded(filename)) {
      Resource* r = resourcesByFilename_[filename];
      r->data_ = ptr;
      r->usedBy_++;
      resourcesByPtr_[ptr] = r;
    }
    else {
      Resource* r = new Resource();
      r->data_ = ptr;
      r->usedBy_ = 1;
      r->filename_ = filename;
      resourcesByFilename_[filename] = r;
      resourcesByPtr_[ptr] = r;
    }
  }

  template <class T>
  void ResourceManager<T>::increaseUsage(const std::string& filename) {
    assert(cacheResources_);
    resourcesByFilename_[filename]->usedBy_++;
  }

  template <class T>
  ResourceManager<T>::ResourceManager(bool cacheResources) {
    // this should not be changed afterwards
    cacheResources_ = cacheResources;
  }

  template <class T>
  ResourceManager<T>::~ResourceManager() {
    while (!resourcesByFilename_.empty()) {
#ifdef _DEBUG
      LWARNING("Un-disposed Resource: " << (*resourcesByFilename_.begin()).second->filename_
        << " in use by " << (*resourcesByFilename_.begin()).second->usedBy_);
#endif
      delete resourcesByFilename_.begin()->second->data_;
      delete resourcesByFilename_.begin()->second;
      resourcesByFilename_.erase(resourcesByFilename_.begin());
    }
  }

  template <class T>
  bool ResourceManager<T>::isLoaded(const std::string& filename) {
    return(cacheResources_ && (resourcesByFilename_.find(filename) != resourcesByFilename_.end()));
  }

  template <class T>
  void ResourceManager<T>::dispose(T* ptr) {
    if (ptr == 0 || resourcesByPtr_.find(ptr) == resourcesByPtr_.end())
      return;

    Resource* r = resourcesByPtr_[ptr];
    r->usedBy_--;

    // check if resource is still in use
    if (r->usedBy_ == 0) {
      std::string filename = r->filename_;
      delete r->data_;
      resourcesByFilename_.erase(filename);
      resourcesByPtr_.erase(ptr);
      delete r;
    }
    else if (!cacheResources_) {
      resourcesByPtr_.erase(ptr);
    }
    ptr = 0;
  }

  template <class T>
  void ResourceManager<T>::addPath(std::string path) {
    pathList_.push_front(path);
    // remove duplicates
    //TODO: better use std::set<> here
    pathList_.sort();
    pathList_.unique();
  }

  template <class T>
  void ResourceManager<T>::removePath(std::string path) {
    std::list<std::string>::iterator it;
    for (it = pathList_.begin(); it != pathList_.end(); ++it) {
      if (*it == path) {
        pathList_.erase(it);
        break;
      }
    }
  }

  template <class T>
  std::string ResourceManager<T>::completePath(std::string filename) {
    std::string cplFileName = filename;

    if (FileSys.exists(filename))
      return filename;

    bool foundFile = false;
    if (!cplFileName.empty()) {
      std::list<std::string>::iterator iter = pathList_.begin();
      while (iter != pathList_.end() && !foundFile) {
        cplFileName = (!(*iter).empty() ? (*iter) + '/' : "") + filename;
        LDEBUG("Completed file name to " << cplFileName);
        if (FileSys.exists(cplFileName))
          foundFile = true;
        iter++;
      }
    }

    if (foundFile)
      return cplFileName;
    else
      return "";
  }

  template <class T>
  std::vector<std::string> ResourceManager<T>::getFilenames() {
    std::vector<std::string> filenames;
    for (typename std::map<std::string, Resource*>::const_iterator iter = resourcesByFilename_.begin();
      iter != resourcesByFilename_.end(); iter++)
    {
      filenames.push_back((*iter).first);
    }
    return filenames;
  }

} // end namespace tgt