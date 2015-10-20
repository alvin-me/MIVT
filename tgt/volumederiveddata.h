#pragma once
#include "config.h"

namespace tgt {

  class Volume;

  /**
  * Marker interface for data that is derived from a volume dataset,
  * such as histograms or preview images.
  *
  * @note each concrete subclass must provide a default constructor
  */
  class VolumeDerivedData {
  public:
    TGT_API virtual ~VolumeDerivedData() {}

    /**
    * Virtual constructor.
    * Check for boo
    *
    * @param handle the volume to derive from
    * @return the constructed derived data object, or null if construction is not possible
    */
    TGT_API virtual VolumeDerivedData* createFrom(Volume *handle) const = 0;

  protected:
    // protected default constructor
    VolumeDerivedData() {}
  };

} // end namespace tgt