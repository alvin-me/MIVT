#pragma once

#include "volumereader.h"
#include "volumelist.h"
#include "dicomdict.h"
#include "dicominfo.h"
#include "metadatacontainer.h"

#include <gdcm/gdcmTag.h>
#include <gdcm/gdcmPixelFormat.h>

namespace tgt {

  class GdcmVolumeReader : public VolumeReader
  {
  public:
    TGT_API GdcmVolumeReader(const std::string& standardDictFileName, ProgressBar* progress = 0)
      throw (FileException);
    TGT_API ~GdcmVolumeReader();

    TGT_API virtual Volume* read(const std::string& fileName)
      throw (IOException, CorruptedFileException, std::bad_alloc);

    TGT_API virtual VolumeList* read_2(const std::string& fileName)
      throw (IOException, CorruptedFileException, std::bad_alloc);

  private:
    /**
    * Helper method that returns all filenames contained in a given directory.
    */
    std::vector<std::string> getFileNamesInDir(const std::string& dirName) const;

    /**
    * Helper method that determines if a file exists and is a readable Dicom file.
    *
    * @param path the path to the file that should be tested
    * @return true, if the file is a Dicom File, false else
    */
    bool isDicomFile(const std::string &path) const;

    /**
    * Helper method that determines if a file is a DICOMDIR.
    *
    * @param path the path to the file that should be tested
    * @return true, if the file is a DICOMDIR, false else
    */
    bool isDicomDir(const std::string &path) const;

    /**
    * Helper method that filters the files by the given SeriesInstanceUID.
    * All files have to be readable DICOM files and StandardDictionary has to be already loaded!
    */
    std::vector<std::string> getFilesInSeries(std::vector<std::string> filenames,
      std::string seriesInstanceUID);

    /**
    * Returns meta data from a DICOM file, which is specified by a DicomDict and a keyword for the DictEntry.
    * If the file and the corresponding tag are already found in the internal file info buffer, it is read from there.
    * Else all meta information specified in the DicomDict for this file is put into the buffer first.
    * If there is no such MetaData, an empty string is returned.
    * If the file is not a valid DICOM file, a FileException is thrown.
    */
    std::string getMetaDataFromFile(const std::string& filename,
      const DicomDict& dict,
      const std::string& keyword)
      throw (FileException);

    /**
    * Extracts the meta data of the file to the MetaDataContainer.
    * The DicomDict specifies, which meta data is extracted.
    * If the file is not a valid DICOM file, a FileException is thrown
    * If setAll is true, also dict entries which are not marked as meta data are set
    */
    static void setMetaDataFromDict(MetaDataContainer* container,
      const DicomDict* dict,
      const std::string& file,
      bool setAll = false)
      throw (FileException);

    /**
    * Helper function that gets the filenames and a VolumeURL.
    * If there is no SeriesInstanceUID-searchParameter in the VolumeURL, the SeriesInstanceUID of the first file is used.
    * The given files are checked and files with different SeriesInstanceUID or files that could not be read are ignored.
    * -> calls method subdivideAndLoadDicomFiles
    *
    * @param fileNames the files that should be selected and loaded
    * @param searchSeriesInstanceUID filtered series instance uid
    *
    * @return a collection of Volumes constructed out of the given files
    */
    virtual VolumeList* selectAndLoadDicomFiles(
      const std::vector<std::string> &fileNames,
      const std::string& origin,
      std::string& searchSeriesInstanceUID)
      throw (FileException, std::bad_alloc);

    /**
    * Helper function before the actual loading (using readDicomFiles): Checks, if any available CustomDicomDict fits the given files (that already should all have the same SeriesInstanceUID).
    * Subdivides and selects the files according to that CustomDicomDict and loads every group by calling readDicomFiles.
    *
    * @param fileNames the files that should be loaded
    * @param origin VolumeURL containing at least the path and also the SeriesInstanceUID of the files
    *
    * @return a collection of Volumes constructed out of the given files
    */
    virtual VolumeList* subdivideAndLoadDicomFiles(
      const std::vector<std::string> &fileNames,
      const std::string& origin)
      throw (FileException, std::bad_alloc);

    /**
    * Helper function that does the actual loading of all files given.
    * Awaits files of one SeriesInstanceUID that belong to one Volume (should only be called by subdivideAndLoadDicomFiles).
    *
    * @param fileNames vector with the fileNames of the files to load
    * @param origin the VolumeURL of the Volume to be loaded
    *
    * @return if reading was succesful, a Volume* constructed of the DICOM slices will be returned
    */
    virtual Volume* readDicomFiles(
      const std::vector<std::string> &fileNames,
      const std::string& origin)
      throw (FileException, std::bad_alloc);

    /**
    * Helper method that finds the correct rescale slope and intercept values for a list of slices where these differ.
    * The correct values are set to info_
    *
    * @param slices a vector of pairs of a filename and the distance from the origin position of the volume
    */
    void computeCorrectRescaleValues(std::vector<std::pair<std::string, double> > slices);

    /**
    * Load several dicom slices of a volume.
    * Does not support multiframe files, only one slice per file.
    *
    * @param info the DicomInfo object containing the necessary meta information (e.g. what the GdcmVolumeReader returns in a VolumeDiskDicom object)
    * @param sliceFiles the list of (correctly ordered!) slices (e.g. what the GdcmVolumeReader returns in a VolumeDiskDicom object)
    */
    VolumeRAM* loadDicomSlices(DicomInfo info, std::vector<std::string> sliceFiles) throw (FileException);

    /**
    * Helper function that reads a single slice.
    *
    * @param dataStorage pointer to an array in which the data should be stored
    * @param fileName name of the file to be loaded
    * @param posScalar offset into the dataStorage array where this particular slice's pixel data should begin
    * @param info DicomInfo object containig meta information about the volume (e.g. for rescaling)
    *
    * @return returns the number of voxels rendered
    */
    virtual int loadSlice(char* dataStorage, const std::string& fileName, size_t posScalar, DicomInfo info);

    /**
    * Helper function that returns a Gdcm::Tag constructed by the information of the DicomDictEntry given.
    */
    static gdcm::Tag getTagFromDictEntry(const DicomDictEntry& entry);

    /**
    * Helper function that constructs the correct type of MetaData Object by knowing its type from a DictEntry and its value
    */
    static MetaDataBase* constructMetaData(const DicomDictEntry &entry, const std::string &value);

    /**
    * Helper method that takes a data type string and converts it to the gdcm representation of the pixel format.
    * If the type cannot be converted, gdcm::PixelFormat::UNKNOWN is returned.
    */
    static gdcm::PixelFormat baseTypeStringToGdcm(const std::string& format);

    DicomDict* dict_; ///< pointer to the Standard Dictionary (when loaded)

    DicomInfo info_; ///< Object containing all relevant meta information about the volume

    ///< used to buffer information about files to reduce file I/Os, buffer is cleared when reading a new dataset
    std::map<std::string, MetaDataContainer> fileInfoBuffer_;

    static const std::string loggerCat_;
  };
}


