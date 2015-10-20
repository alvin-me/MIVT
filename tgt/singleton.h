#pragma once

#include "config.h"
#include <cassert>

namespace tgt {

  /**
  This class helps to build the singleton design pattern.
  Here you have full control over construction and deconstruction
  of the object.
  */
  template<class T>
  class Singleton {
  public:
    /**
    * Init the actual singleton.
    * Must be called BEFORE the class is used, like this:
    */
    static void init() {
      assert(!singletonClass_);
      singletonClass_ = new T;
    }

    /**
    * Deinit the actual singleton.
    * Must be done at last.
    */
    static void deinit() {
      assert(singletonClass_ != 0);
      delete singletonClass_;
      singletonClass_ = 0;
    }

    /**
    * Get Pointer of the actual class
    * @return Pointer of the actual class
    */
    static T* getPtr() {
      assert(singletonClass_ != 0);
      return singletonClass_;
    }

    /**
    * Get reference of the actual class
    * @return reference of the actual class
    */
    static T& getRef() {
      assert(singletonClass_ != 0);
      return *singletonClass_;
    }

    /**
    * Has the actual class been inited?
    */
    static bool isInited() {
      return (singletonClass_ != 0);
    }

  private:
  protected:
    static T* singletonClass_;
  };

  /// init static pointers with 0
  template<class T>
  T* Singleton<T>::singletonClass_ = 0;

} // end namespace tgt
