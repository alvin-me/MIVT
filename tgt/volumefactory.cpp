#include "volumefactory.h"

namespace tgt {

  const std::string VolumeFactory::loggerCat_("VolumeFactory");

  VolumeFactory::VolumeFactory() {
    generators_.push_back(new VolumeGeneratorUInt8());
    generators_.push_back(new VolumeGeneratorInt8());

    generators_.push_back(new VolumeGeneratorUInt16());
    generators_.push_back(new VolumeGeneratorInt16());

    generators_.push_back(new VolumeGeneratorUInt32());
    generators_.push_back(new VolumeGeneratorInt32());

    generators_.push_back(new VolumeGeneratorUInt64());
    generators_.push_back(new VolumeGeneratorInt64());

    generators_.push_back(new VolumeGeneratorFloat());
    generators_.push_back(new VolumeGeneratorDouble());
  }

  VolumeFactory::~VolumeFactory() {
    while (!generators_.empty()) {
      delete generators_.back();
      generators_.pop_back();
    }
  }

} // end namespace tgt