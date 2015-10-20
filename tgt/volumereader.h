#pragma once

#include "exception.h"
#include "progressbar.h"

#include <string>
#include <vector>

namespace tgt {

  class Volume;
  class VolumeRAM;

  /**
  * Reads one volume data sets from a passed location.
  * Implement this class in order to support a new volume data format.
  */
  class VolumeReader {
  public:
    TGT_API VolumeReader(ProgressBar* progress = 0);
    TGT_API virtual ~VolumeReader() {}

    /**
    * Loads a single volume from the passed origin.
    *
    * The default implementation calls the read() function with the
    * origin's path. Override it in order to access the VolumeURL directly.
    *
    * @param origin The origin the data set is to be read from.
    *
    * @return Volume encapsulating the loaded volume. The caller is responsible for freeing the memory.
    */
    TGT_API virtual Volume* read(const std::string& fileName)
      throw (IOException, CorruptedFileException, std::bad_alloc) = 0;

    /**
    * Returns a list of protocols that are supported by this reader.
    */
    TGT_API const std::vector<std::string>& getSupportedProtocols() const;

    /**
    * Assigns a progress bar to the reader. May be null.
    */
    TGT_API void setProgressBar(ProgressBar* progressBar);

    /**
    * Returns the assigned progress bar. May be null.
    */
    TGT_API ProgressBar* getProgressBar() const;

  protected:
    void read(VolumeRAM* volume, FILE* fin);

    /**
    * Reverses the order of the slice in x-direction. This method
    * is called, when the .dat file contains "SliceOrder: -x" in one line.
    */
    void reverseXSliceOrder(VolumeRAM* const volume) const;

    /**
    * Reverses the order of the slice in y-direction. This method
    * is called, when the .dat file contains "SliceOrder: -y" in one line.
    */
    void reverseYSliceOrder(VolumeRAM* const volume) const;

    /**
    * Reverses the order of the slice in z-direction. This method
    * is called, when the .dat file contains "SliceOrder: -z" in one line.
    */
    void reverseZSliceOrder(VolumeRAM* const volume) const;

    /// List of protocols supported by the reader.
    std::vector<std::string> protocols_;

    ProgressBar* progress_;

    static const std::string loggerCat_;
  };

} // end namespace tgt