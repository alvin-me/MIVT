#pragma once

#include "volumereader.h"
#include "tgt_math.h"

namespace tgt {

  /**
  * Reads a raw volume dataset. This requires information hints about dimension, format, etc.,
  * usually given by an accompanying <tt>.dat</tt>. DatVolumeReader reads this information and
  * uses RawVolumeReader to read the actual data.
  */
  class RawVolumeReader : public VolumeReader {
  public:

    /**
    * Contains hints about the volume dataset.
    */
    struct ReadHints {
      ReadHints(glm::ivec3 dimensions = glm::ivec3(0),
      glm::vec3 spacing = glm::vec3(0.f),
      const std::string& format = "UCHAR",
      size_t headerskip = 0,
      bool bigEndianByteOrder = false,
      float rescaleIntercept = 0.f,
      float rescaleSlope = 1.f,
      float windowCenter = 0.f,
      float windowWidth = 0.f);

      glm::ivec3 dimensions_;       ///< number of voxels in x-, y- and z-direction
      glm::vec3 spacing_;           ///< non-uniform voxel scaling
      std::string objectModel_;     ///< \c I (intensity) or \c RGBA
      std::string format_;          ///< voxel data format
      int timeframe_;               ///< zero-based time frame in volume with multiple time frames
      size_t headerskip_;           ///< number of bytes to skip at the beginning of the raw file
      bool bigEndianByteOrder_;     ///< data is saved in big endian format an needs to be converted
      float rescaleIntercept_;      ///< DICOM(0028 1052) rescale intercept
      float rescaleSlope_;          ///< DICOM(0028 1053) rescale slope
      float windowCenter_;          ///< DICOM(0028 1050) window center
      float windowWidth_;           ///< DICOM(0028 1051) window width
      glm::mat4 transformation_;    ///< 4x4-matrix for affine transformation of volume
      std::string sliceOrder_;
    };

    TGT_API RawVolumeReader();

    /**
    * Set hints about the volume dataset. Must be set before read() is called. More options
    * can be specified by using the other overloaded variant.
    *
    * @param dimensions number of voxels in x-, y- and z-direction
    * @param spacing non-uniform voxel scaling
    * @param objectModel \c I (intensity) or \c RGBA
    * @param format voxel data format, one of \c UCHAR, \c USHORT, \c USHORT_12 (for CT datasets), \c FLOAT
    * @param timeframe zero-based time frame in volume with multiple time frames
    * @param headerskip number of bytes to skip at the beginning of the raw file
    * @param bigEndian if set to true, the data is converted from big endian to little endian byte order
    */
    TGT_API void setReadHints(glm::ivec3 dimensions,
      glm::vec3 spacing,
      const std::string& format = "UCHAR",
      int headerskip = 0,
      bool bigEndian = false,
      float rescaleIntercept = 0.f,
      float rescaleSlope = 1.f,
      float windowCenter = 0.f,
      float windowWidth = 0.f);

    /**
    * Set hints about the volume dataset. Must be set before read() is called.
    */
    TGT_API void setReadHints(const ReadHints& hints);

    TGT_API virtual Volume* read(const std::string& fileName)
      throw (IOException, CorruptedFileException, std::bad_alloc);

  private:
    ReadHints hints_;

    static const std::string loggerCat_;
  };

} // end namespace tgt