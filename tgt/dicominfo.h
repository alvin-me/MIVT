#pragma once

#include "tgt_math.h"
#include "config.h"

namespace tgt {

  /**
  * This class is used by GdcmVolumeReader to store some information about a DICOM volume that is loaded.
  * It contains basic Meta Information like StudyInstanceUID, SeriesInstanceUID, Patient's Name, Modality, etc.
  * as well as Volume-related information such as information about spacing.
  */
  class DicomInfo
  {
  public:
    TGT_API DicomInfo();

    TGT_API void setDx(int dx);
    TGT_API void setDy(int dy);
    TGT_API void setDz(int dz);
    TGT_API void setNumberOfFrames(int frames);
    TGT_API void setXSpacing(double x);
    TGT_API void setYSpacing(double y);
    TGT_API void setZSpacing(double z);
    TGT_API void setXOrientationPatient(glm::dvec3 orientation);
    TGT_API void setYOrientationPatient(glm::dvec3 orientation);
    TGT_API void setSliceNormal(glm::dvec3 normal);
    TGT_API void setOffset(glm::dvec3 offset);
    TGT_API void setPixelRepresentation(unsigned short representation);
    TGT_API void setBitsStored(int bits);
    TGT_API void setSamplesPerPixel(int spp);
    TGT_API void setBytesPerVoxel(int bytes);
    TGT_API void setIntercept(float intercept);
    TGT_API void setSlope(float slope);
    TGT_API void setRescaleType(std::string type);
    TGT_API void setModality(std::string modality);
    TGT_API void setSeriesInstanceUID(std::string uid);
    TGT_API void setStudyInstanceUID(std::string uid);
    TGT_API void setSeriesDescription(std::string description);
    TGT_API void setStudyDescription(std::string description);
    TGT_API void setPatientName(std::string name);
    TGT_API void setPatientId(std::string id);
    TGT_API void setRwmDiffers(bool d);
    TGT_API void setBaseType(std::string type);
    TGT_API void setFormat(std::string format);


    TGT_API int getDx() const;
    TGT_API int getDy() const;
    TGT_API int getDz() const;
    TGT_API int getNumberOfFrames() const;
    TGT_API int getBitsStored() const;
    TGT_API int getSamplesPerPixel() const;
    TGT_API int getBytesPerVoxel() const;
    TGT_API unsigned short getPixelRepresentation() const;
    TGT_API float getIntercept() const;
    TGT_API float getSlope() const;
    TGT_API double getXSpacing() const;
    TGT_API double getYSpacing() const;
    TGT_API double getZSpacing() const;
    TGT_API glm::dvec3 getXOrientationPatient() const;
    TGT_API glm::dvec3 getYOrientationPatient() const;
    TGT_API glm::dvec3 getSliceNormal() const;
    TGT_API glm::dvec3 getOffset() const;
    TGT_API std::string getRescaleType() const;
    TGT_API std::string getModality() const;
    TGT_API std::string getSeriesInstanceUID() const;
    TGT_API std::string getStudyInstanceUID() const;
    TGT_API std::string getSeriesDescription() const;
    TGT_API std::string getStudyDescription() const;
    TGT_API std::string getPatientName() const;
    TGT_API std::string getPatientId() const;
    TGT_API bool rwmDiffers() const;
    TGT_API std::string getBaseType() const;
    TGT_API std::string getFormat() const;

  private:
    int dx_, dy_, dz_;          ///< information about the size of the volume in x-, y- and z-direction
    int numberOfFrames_;        ///< for multiframe DICOM files

    //information about patient, study, modality etc.
    std::string patientId_;                 ///< Patient ID of the DICOM Meta Data
    std::string patientName_;               ///< Patient's Name
    std::string studyInstanceUID_;          ///< StudyInstanceUID of the DICOM Meta Data
    std::string studyDescription_;          ///< Study Description of the DICOM Meta Data
    std::string seriesInstanceUID_;         ///< SeriesInstanceUID of the DICOM Meta Data
    std::string seriesDescription_;         ///< Series Description of the DICOM Meta Data
    std::string modality_;                  ///< the Modality of the DICOM file(s)

    //spatial information about the volume
    double xSpacing_, ySpacing_, zSpacing_;     ///< spacing of the volume in x-, y- and z-direction
    glm::dvec3 xOrientationPatient_;            ///< first vector of ImageOrientationPatient
    glm::dvec3 yOrientationPatient_;            ///< second vector of ImageOrientationPatient
    glm::dvec3 sliceNormal_;                    ///< direction in which the slices are arranged
    glm::dvec3 offset_;                         ///< offset of the Volume's origin

    //pixel storage
    unsigned short pixelRepresentation_;    ///< 0 = unsigned, 1 = signed
    int bitsStored_;                        ///< bits stored per sample
    int bytesPerVoxel_;                     ///< bytes stored per Voxel (samplesPerPixel_ * bits per sample in bytes)

    //voxel format
    std::string baseType_;      ///< base type for format (e.g. "uint16")
    int samplesPerPixel_;       ///< samples stored per pixel (-> together with base type determines format)
    std::string format_;        ///< format string (e.g. "float", "Vector3(uint8)")

    //pixel rescale information
    bool rwmDiffers_;           ///< does the rescale slope and intercept of the image files differ?
    float intercept_;           ///< Rescale Intercept
    float slope_;               ///< Rescale Slope
    std::string rescaleType_;   ///< Rescale Type
  };

} // end namespace tgt
