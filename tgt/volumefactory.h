#pragma once

#include "volumeram.h"
#include "volumeatomic.h"
#include "volumeelement.h"
#include "logmanager.h"
#include <string>
#include <vector>

namespace tgt {

  class VolumeGeneratorBase {
  public:
    TGT_API virtual ~VolumeGeneratorBase() {}
    TGT_API virtual std::string getFormat() const = 0;
    TGT_API virtual std::string getBaseType() const = 0;
    TGT_API virtual VolumeRAM* create(glm::ivec3 dimensions) const throw (std::bad_alloc) = 0;
    TGT_API virtual bool isType(const VolumeRAM* v) const = 0;
    TGT_API virtual int getBytesPerVoxel() const = 0;
  };

  //------------------------------------------------------------------------------

  class VolumeFactory {
  public:
    TGT_API VolumeFactory();
    TGT_API ~VolumeFactory();

    TGT_API std::string getFormat(const VolumeRAM* v) const {
      for (size_t i = 0; i < generators_.size(); ++i) {
        if (generators_[i]->isType(v))
          return generators_[i]->getFormat();
      }
      return "";
    }

    TGT_API VolumeRAM* create(const std::string& format, glm::ivec3 dimensions) const
      throw (std::bad_alloc)
    {
      for (size_t i = 0; i < generators_.size(); ++i) {
        if (generators_[i]->getFormat() == format)
          return generators_[i]->create(dimensions);
      }
      LERROR("Failed to create volume of format '" << format << "'");
      return 0;
    }

    /// Returns the base-format for a format
    TGT_API std::string getBaseType(const std::string& format) const {
      for (size_t i = 0; i < generators_.size(); ++i) {
        if (generators_[i]->getFormat() == format)
          return generators_[i]->getBaseType();
      }
      LERROR("Failed to determine base-type of format '" << format << "'");
      return "";
    }

    TGT_API int getBytesPerVoxel(const std::string& format) const {
      for (size_t i = 0; i < generators_.size(); ++i) {
        if (generators_[i]->getFormat() == format)
          return generators_[i]->getBytesPerVoxel();
      }
      LERROR("Failed to get bytes per voxel for '" << format << "'");
      return 0;
    }

  private:
    std::vector<VolumeGeneratorBase*> generators_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  template<class T>
  class VolumeGeneratorGeneric : public VolumeGeneratorBase {
  public:
    virtual std::string getFormat() const = 0;
    virtual std::string getBaseType() const = 0;

    virtual VolumeRAM* create(glm::ivec3 dimensions) const throw (std::bad_alloc) {
      return new VolumeAtomic<T>(dimensions);
    }

    virtual bool isType(const VolumeRAM* v) const {
      if (typeid(*v) == typeid(VolumeAtomic<T>))
        return true;
      else
        return false;
    }

    virtual int getBytesPerVoxel() const {
      return sizeof(T);
    }
  };

  //------------------------------------------------------------------------------

  class VolumeGeneratorUInt8 : public VolumeGeneratorGeneric<uint8_t> {
  public:
    TGT_API std::string getFormat() const { return "uint8"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorInt8 : public VolumeGeneratorGeneric<int8_t> {
  public:
    TGT_API std::string getFormat() const { return "int8"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorUInt16 : public VolumeGeneratorGeneric<uint16_t> {
  public:
    TGT_API std::string getFormat() const { return "uint16"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorInt16 : public VolumeGeneratorGeneric<int16_t> {
  public:
    TGT_API std::string getFormat() const { return "int16"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorUInt32 : public VolumeGeneratorGeneric<uint32_t> {
  public:
    TGT_API std::string getFormat() const { return "uint32"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorInt32 : public VolumeGeneratorGeneric<int32_t> {
  public:
    TGT_API std::string getFormat() const { return "int32"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorUInt64 : public VolumeGeneratorGeneric<uint64_t> {
  public:
    TGT_API std::string getFormat() const { return "uint64"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorInt64 : public VolumeGeneratorGeneric<int64_t> {
  public:
    TGT_API std::string getFormat() const { return "int64"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorFloat : public VolumeGeneratorGeneric<float> {
  public:
    TGT_API std::string getFormat() const { return "float"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

  class VolumeGeneratorDouble : public VolumeGeneratorGeneric<double> {
  public:
    TGT_API std::string getFormat() const { return "double"; }
    TGT_API std::string getBaseType() const { return getFormat(); }
  };

} // end namespace tgt
