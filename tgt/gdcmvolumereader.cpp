#include "gdcmvolumereader.h"
#include "filesystem.h"
#include "volumefactory.h"
#include "volumegl.h"
#include "tgt_string.h"
#include "primitivemetadata.h"

#include <gdcm/gdcmFile.h>
#include <gdcm/gdcmDirectory.h>
#include <gdcm/gdcmReader.h>
#include <gdcm/gdcmImageReader.h>
#include <gdcm/gdcmStringFilter.h>
#include <gdcm/gdcmRescaler.h>

#pragma warning(disable:4702)

namespace tgt {
  const std::string GdcmVolumeReader::loggerCat_ = "GdcmVolumeReader";

  GdcmVolumeReader::GdcmVolumeReader(const std::string& standardDictFileName, ProgressBar* progress)
    throw (FileException)
    : VolumeReader(progress)
    , dict_(0)
  {
    protocols_.push_back("dcm");

    dict_ = DicomDict::loadFromFile(standardDictFileName);
  }

  GdcmVolumeReader::~GdcmVolumeReader()
  {
    DELPTR(dict_);
  }

  Volume* GdcmVolumeReader::read(const std::string& fileName)
    throw (IOException, CorruptedFileException, std::bad_alloc)
  {
    VolumeList* collection = read_2(fileName);
    if (collection->size() > 1) {
      throw FileException("Could not load Volume, found more than one Volume: ", fileName);
    }
    else {
      if (getProgressBar())
        getProgressBar()->hide();
      return collection->first();
    }
    return 0;
  }

  VolumeList* GdcmVolumeReader::read_2(const std::string& fileName)
    throw (IOException, CorruptedFileException, std::bad_alloc)
  {
    //if Standard Dictionary is not yet loaded: try to load
    if (!dict_) {
      LERROR("Error loading Standard Dictionary.");
      return 0;
    }

    VolumeList* collection = 0;

    std::string searchSeriesInstanceUID;

    //check if fileName is a file or a directory
    if (FileSystem::dirExists(fileName)) {
      //fileName is a directory
      collection = selectAndLoadDicomFiles(getFileNamesInDir(fileName), fileName, searchSeriesInstanceUID);
    }
    else if (FileSystem::fileExists(fileName)) {
      //fileName is a file -> check if it is a Dicom file
      if (!isDicomFile(fileName)) {
        throw FileAccessException("Selected file is not a DICOM file or it is broken!", fileName);
      }
      else if (isDicomDir(fileName)) {
        //file is a DICOMDIR
        //collection = readDicomDir(fileName);
      }
      else {
        //file ist NOT a DICOMDIR -> read files in Directory
        LINFO("Loading files in " << FileSystem::dirName(fileName));

        //read SeriesInstanceUID from file if possible
        gdcm::Reader reader;
        reader.SetFileName(fileName.c_str());
        if (!reader.Read())
          throw FileAccessException("Could not read Dicom File", fileName);

        gdcm::StringFilter sf = gdcm::StringFilter();
        sf.SetFile(reader.GetFile());
        std::string seriesInstanceUID = sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("SeriesInstanceUID")));
        seriesInstanceUID = tgt::trim(seriesInstanceUID, " ");

        searchSeriesInstanceUID = seriesInstanceUID;
        collection = selectAndLoadDicomFiles(getFileNamesInDir(FileSystem::dirName(fileName)),
          FileSystem::dirName(fileName), searchSeriesInstanceUID);
      }
    }
    else {
      //url is neither a file nor a directory -> error
      throw FileNotFoundException("GdcmVolumeReader: Unable to find ", fileName);
    }

    if (collection->empty()) {
      if (getProgressBar())
        getProgressBar()->hide();
      throw FileException("Found no volume to load!", fileName);
    }

    if (getProgressBar())
      getProgressBar()->hide();
    return collection;
  }

  std::vector<std::string> GdcmVolumeReader::getFileNamesInDir(const std::string& dirName) const
  {
    gdcm::Directory dir;
    dir.Load(dirName);
    return dir.GetFilenames();
  }

  bool GdcmVolumeReader::isDicomFile(const std::string &path) const {
    gdcm::Reader reader;
    reader.SetFileName(path.c_str());
    return reader.Read();
  }

  bool GdcmVolumeReader::isDicomDir(const std::string &path) const {
    if (!isDicomFile(path)) return false;

    gdcm::Reader reader;
    reader.SetFileName(path.c_str());
    reader.Read();

    gdcm::MediaStorage ms;
    ms.SetFromFile(reader.GetFile());

    return (ms == gdcm::MediaStorage::MediaStorageDirectoryStorage);
  }

  std::vector<std::string> GdcmVolumeReader::getFilesInSeries(std::vector<std::string> filenames,
    std::string seriesInstanceUID)
  {
    //show Progress Bar
    if (getProgressBar() && !filenames.empty())
      getProgressBar()->setTitle("Assigning files to SeriesInstanceUID: " + seriesInstanceUID);

    int progress = 1;

    std::vector<std::string> result;

    //iterate over files
    std::vector<std::string>::iterator fileIterator;
    for (fileIterator = filenames.begin(); fileIterator != filenames.end(); fileIterator++) {
      if (getProgressBar()) {
        getProgressBar()->setProgressMessage("Reading files...");
        getProgressBar()->setProgress(static_cast<float>(progress) / static_cast<float>(filenames.size()));
        progress++;
      }

      //check SeriesInstanceUID of the file
      //if it fits: add this file to result
      try {
        std::string series = getMetaDataFromFile(*fileIterator, *dict_, "SeriesInstanceUID");
        if (series == seriesInstanceUID)
          result.push_back(*fileIterator);
      }
      catch (FileException) {
        //file is not a DICOM file... just ignore it
        LINFO("Ignoring file " + (*fileIterator) + " because it is not a DICOM file...");
      }
    }

    if (getProgressBar())
      getProgressBar()->hide();

    return result;
  }

  std::string GdcmVolumeReader::getMetaDataFromFile(const std::string& filename,
    const DicomDict& dict,
    const std::string& keyword)
    throw (FileException)
  {
    std::map<std::string, MetaDataContainer>::iterator mapIterator;
    mapIterator = fileInfoBuffer_.find(filename);

    //if the file is not already present in the file buffer: create a new entry and add the meta information from the DicomDict
    if (mapIterator == fileInfoBuffer_.end()) {
      //put file and corresponding dict meta info into the buffer
      std::pair<std::string, MetaDataContainer> newPair;
      newPair.first = filename;
      //MetaDataContainer c;
      //newPair.second = c;
      setMetaDataFromDict(&newPair.second, &dict, filename, true);
      fileInfoBuffer_.insert(newPair);
      mapIterator = fileInfoBuffer_.find(filename);
    }
    else if (!(mapIterator->second.hasMetaData(keyword))) {
      //if the filename is present, but does not have the right meta data: add the meta information from the DicomDict
      setMetaDataFromDict(&mapIterator->second, &dict, filename, true);
    }

    //now the filename is present, the meta information is added and the iterator is set to the right position
    //extract the meta data and return it
    if (mapIterator->second.hasMetaData(keyword))
      return mapIterator->second.getMetaData(keyword)->toString();
    else
      return "";
  }

  void GdcmVolumeReader::setMetaDataFromDict(MetaDataContainer* container,
    const DicomDict* dict,
    const std::string& file,
    bool setAll)
    throw (FileException)
  {
    if ((!container) || (!dict))
      return;

    gdcm::Reader reader;
    reader.SetFileName(file.c_str());
    if ((!reader.Read()) || (!reader.GetFile().GetHeader().IsValid()))
      throw FileException("Cannot extract meta data from file " + file, file);

    gdcm::StringFilter sf;
    sf.SetFile(reader.GetFile());

    const std::vector<std::string> keys = dict->getKeywordVector();

    std::vector<std::string>::const_iterator keywordIterator;

    for (keywordIterator = keys.begin(); keywordIterator != keys.end(); ++keywordIterator) {
      //get entry and check if it should be set as MetaData
      DicomDictEntry entry = dict->getDictEntryByKeyword(*keywordIterator);
      if (entry.isMetaData() || setAll) {
        //get the value
        std::string s = trim(sf.ToString(getTagFromDictEntry(entry)), " ");
        if (!s.empty()) {
          //if value is not empty: construct MetaData and add to MetaDataContainer
          MetaDataBase* m = constructMetaData(entry, s);

          if (container->hasMetaData(*keywordIterator))
            container->removeMetaData(*keywordIterator);
          container->addMetaData(*keywordIterator, m);
        }
      }
    }
  }

  gdcm::Tag GdcmVolumeReader::getTagFromDictEntry(const DicomDictEntry &entry) {
    return gdcm::Tag(entry.getGroupTagUint16(), entry.getElementTagUint16());
  }

  MetaDataBase* GdcmVolumeReader::constructMetaData(const DicomDictEntry &entry, const std::string &valueString) {

    std::string vr = entry.getValueRepresentation();
    std::string vm = entry.getValueMultiplicity();
    std::string value = valueString;
    value = trim(value, " ");
    vr = trim(vr, " ");
    vm = trim(vm, " ");

    //select by ValueRepresentation and ValueMultiplicity
    if ((vr == "IS") && (vm == "1")) {
      //string to int
      IntMetaData* m = new IntMetaData();
      int i = stoi(value);
      m->setValue(i);
      return m;
    }
    else if ((vr == "IS") && (vm == "2")) {
      //string to glm::ivec2
      IVec2MetaData* m = new IVec2MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::ivec2 v;
      v.x = stoi(values[0]);
      v.y = stoi(values[1]);
      m->setValue(v);
      return m;
    }
    else if ((vr == "IS") && (vm == "3")) {
      //string to glm::ivec3
      IVec3MetaData* m = new IVec3MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::ivec3 v;
      v.x = stoi(values[0]);
      v.y = stoi(values[1]);
      v.z = stoi(values[2]);
      m->setValue(v);
      return m;
    }
    else if (((vr == "FL") || (vr == "DS") || (vr == "OF")) && (vm == "1")) {
      //string to float
      FloatMetaData* m = new FloatMetaData();
      float f = stof(value);
      m->setValue(f);
      return m;
    }
    else if (((vr == "FL") || (vr == "DS") || (vr == "OF")) && (vm == "2")) {
      //string to glm::vec2
      Vec2MetaData* m = new Vec2MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::vec2 v;
      v.x = stof(values[0]);
      v.y = stof(values[1]);
      m->setValue(v);
      return m;
    }
    else if (((vr == "FL") || (vr == "DS") || (vr == "OF")) && (vm == "3")) {
      //string to glm::vec3
      Vec3MetaData* m = new Vec3MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::vec3 v;
      v.x = stof(values[0]);
      v.y = stof(values[1]);
      v.z = stof(values[2]);
      m->setValue(v);
      return m;
    }
    else if ((vr == "FD") && (vm == "1")) {
      //string to double
      DoubleMetaData* m = new DoubleMetaData();
      value = trim(value, " ");
      double d = stof(value);
      m->setValue(d);
      return m;
    }
    else if ((vr == "FD") && (vm == "2")) {
      //string to glm::dvec2
      DVec2MetaData* m = new DVec2MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::dvec2 v;
      v.x = stod(values[0]);
      v.y = stod(values[1]);
      m->setValue(v);
      return m;
    }
    else if ((vr == "FD") && (vm == "3")) {
      //string to glm::dvec3
      DVec3MetaData* m = new DVec3MetaData();
      std::vector<std::string> values = strSplit(value, '\\');
      glm::dvec3 v;
      v.x = stod(values[0]);
      v.y = stod(values[1]);
      v.z = stod(values[2]);
      m->setValue(v);
      return m;
    }
    else {
      //other type: just use the string
      StringMetaData* m = new StringMetaData();
      m->setValue(value);
      return m;
    }
  }

  VolumeList* GdcmVolumeReader::selectAndLoadDicomFiles(
    const std::vector<std::string> &fileNames,
    const std::string& origin,
    std::string& searchSeriesInstanceUID)
    throw (FileException, std::bad_alloc)
  {
    if (fileNames.empty())
      throw FileNotFoundException("Directory is empty!", origin);

    //filter the given files, so that only readable DICOM files with one SeriesInstanceUID remain
    bool filter;
    if (searchSeriesInstanceUID.empty())
      filter = false;
    else
      filter = true;

    std::vector<std::string> useFiles; //selected files to be used

    std::vector<std::string>::const_iterator i;
    int itemused = 1;

    if (getProgressBar())
      getProgressBar()->setTitle("Preselecting files");

    for (i = fileNames.begin(); i != fileNames.end(); i++) {

      if (getProgressBar()) {
        getProgressBar()->setProgressMessage("Checking SeriesInstanceUID of files...");
        getProgressBar()->setProgress(static_cast<float>(itemused) / static_cast<float>(fileNames.size()));
        itemused++;
      }

      if (!isDicomFile(*i)) {
        LINFO("Skipping file: " + (*i));
      }
      else {
        gdcm::Reader iReader;

        if (!filter) {
          //if there is no SeriesInstanceUID until now, get it from the first file
          //Read file
          iReader.SetFileName((*i).c_str());
          iReader.Read();

          //check if meta information is valid
          if (!iReader.GetFile().GetHeader().IsValid())
            throw FileAccessException("GdcmVolumeReader: File Header not valid! ", (*i));

          //get SeriesInstanceUID
          std::string seriesInstanceUID = getMetaDataFromFile(*i, *dict_, "SeriesInstanceUID");

          if (seriesInstanceUID.empty())
            LERROR("File has no SeriesInstanceUID: " << (*i));
          else {
            seriesInstanceUID = trim(seriesInstanceUID, " ");
            searchSeriesInstanceUID = seriesInstanceUID;
            filter = true;
            useFiles.push_back(*i);
            LINFO("Using first SeriesInstanceUID found: " + seriesInstanceUID);
          }
        }
        else {
          //check if this file belongs to the given series
          std::string seriesInstanceUID = getMetaDataFromFile(*i, *dict_, "SeriesInstanceUID");

          if (seriesInstanceUID == searchSeriesInstanceUID)
            useFiles.push_back(*i);
        }
      }
    }

    if (getProgressBar())
      getProgressBar()->hide();

    //now all files in useFiles belong to the same SeriesInstanceUID
    if (useFiles.empty())
      throw FileNotFoundException("No file with corresponding SeriesInstanceUID could be found!", origin);

    return subdivideAndLoadDicomFiles(useFiles, origin);
  }

  VolumeList* GdcmVolumeReader::subdivideAndLoadDicomFiles(
    const std::vector<std::string> &fileNames,
    const std::string& origin)
    throw (FileException, std::bad_alloc)
  {
    VolumeList* vc = new VolumeList(); //the VolumeCollection to be returned

    Volume* vh = readDicomFiles(fileNames, origin);
    if (vh) {
      vh->setOrigin(origin);
      vc->add(vh);
    }

    return vc;
  }

  /*
  * Sorts strings according to a double value interpreted as the distance of the Image Origin to the Volume Origin
  */
  bool slices_cmp_dist(std::pair<std::string, double> a, std::pair<std::string, double> b) {
    return a.second < b.second;
  }

  Volume* GdcmVolumeReader::readDicomFiles(
    const std::vector<std::string> &fileNames,
    const std::string& origin)
    throw (FileException, std::bad_alloc)
  {
    std::vector<std::pair<std::string, double> > slices; //filename, distance

    //take first file as reference, since all files belong to the same SeriesInstanceUID
    gdcm::ImageReader reader;
    reader.SetFileName(fileNames.begin()->c_str());
    reader.Read();
    gdcm::StringFilter sf;
    sf.SetFile(reader.GetFile());

    //Get Meta Information
    info_.setSeriesInstanceUID(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("SeriesInstanceUID"))));
    info_.setStudyInstanceUID(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("StudyInstanceUID"))));
    info_.setSeriesDescription(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("SeriesDescription"))));
    info_.setStudyDescription(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("StudyDescription"))));
    info_.setModality(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("Modality"))));
    info_.setPatientName(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("PatientName"))));
    info_.setPatientId(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("PatientID"))));

    info_.setIntercept(static_cast<float>(reader.GetImage().GetIntercept()));
    info_.setSlope(static_cast<float>(reader.GetImage().GetSlope()));
    info_.setRescaleType(sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("RescaleType"))));

    gdcm::PixelFormat::ScalarType scalarType = reader.GetImage().GetPixelFormat().GetScalarType();

    //for checks, if pixel data needs to be rescaled
    bool slopeDiffers = false;
    bool interceptDiffers = false;

    //get image related information
    //get image dimensions
    const unsigned int* dimensions = reader.GetImage().GetDimensions();

    if (dimensions) {
      info_.setDx(dimensions[0]);
      info_.setDy(dimensions[1]);
    }
    else
      throw FileException("No Dimensions found in Image File.", *fileNames.begin());

    //get bits stored
    info_.setBitsStored(reader.GetImage().GetPixelFormat().GetBitsStored());
    //get samples per pixel
    info_.setSamplesPerPixel(reader.GetImage().GetPixelFormat().GetSamplesPerPixel());

    LINFO("    Size: " << info_.getDx() << "x" << info_.getDy() << ", " << info_.getBitsStored()* info_.getSamplesPerPixel() << " bits");

    //get spacing for x and y
    const double* spacing = reader.GetImage().GetSpacing();
    info_.setXSpacing(spacing[0]);
    info_.setYSpacing(spacing[1]);

    //get ImageOrientation
    glm::dvec3 xOrientationPatient;
    glm::dvec3 yOrientationPatient;
    const double* directions = reader.GetImage().GetDirectionCosines();
    xOrientationPatient.x = directions[0];
    xOrientationPatient.y = directions[1];
    xOrientationPatient.z = directions[2];
    yOrientationPatient.x = directions[3];
    yOrientationPatient.y = directions[4];
    yOrientationPatient.z = directions[5];

    info_.setXOrientationPatient(xOrientationPatient);
    info_.setYOrientationPatient(yOrientationPatient);

    //calculate slice normal
    glm::dvec3 sliceNormal = glm::cross(xOrientationPatient, yOrientationPatient);

    info_.setSliceNormal(sliceNormal);

    //Read ImagePositionPatient from all files and calculate distance along slice normal
    std::vector<std::string>::const_iterator it_files = fileNames.begin();
    int i = 0;
    if (getProgressBar() && !fileNames.empty())
      getProgressBar()->setTitle("Loading DICOM Data Set");

    while (it_files != fileNames.end()) {
      if (getProgressBar()) {
        getProgressBar()->setProgressMessage("Reading slice '" + FileSystem::fileName(*it_files) + "' ...");
        getProgressBar()->setProgress(static_cast<float>(i) / static_cast<float>(fileNames.size()));
      }
      i++;

      //Read file
      gdcm::ImageReader reader;
      reader.SetFileName((*it_files).c_str());

      //check if file could be read -> should always be the case due to earlier checks
      if (!reader.Read()){
        if (getProgressBar())
          getProgressBar()->hide();
        throw FileAccessException("Could not read File which should be readable!", (*it_files));
      }

      //check if meta information is valid -> should always be the case due to earlier checks
      if (!reader.GetFile().GetHeader().IsValid()){
        if (getProgressBar())
          getProgressBar()->hide();
        throw FileAccessException("GdcmVolumeReader: File Header not valid! ", (*it_files));
      }

      gdcm::StringFilter sf = gdcm::StringFilter();
      sf.SetFile(reader.GetFile());

      //get position of image
      glm::dvec3 imagePositionPatient;
      const double* origin = reader.GetImage().GetOrigin();
      imagePositionPatient.x = origin[0];
      imagePositionPatient.y = origin[1];
      imagePositionPatient.z = origin[2];

      //calculate distance
      double dist = glm::dot(info_.getSliceNormal(), imagePositionPatient);

      //add image
      slices.push_back(make_pair(*it_files, dist));

      it_files++;
    }

    if (slices.size() == 0) {
      if (getProgressBar())
        getProgressBar()->hide();
      throw CorruptedFileException("Path does not contain any DICOM slices");
    }
    else if (slices.size() == 1) {
      //check, if this file is a multiframe image
      if (reader.GetImage().GetNumberOfDimensions() == 3) {
        //get number of frames in this image
        std::string frames = sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("NumberOfFrames")));
        if (!frames.empty())
          info_.setDz(stoi(frames));
        else {
          info_.setDz(1);
        }

        info_.setNumberOfFrames(info_.getDz());

        //get Z Spacing
        std::string sliceSpacing = sf.ToString(getTagFromDictEntry(dict_->getDictEntryByKeyword("SpacingBetweenSlices")));
        if (!sliceSpacing.empty())
          info_.setZSpacing(stod(sliceSpacing));
        else {
          info_.setZSpacing(1.0);
        }

      }
      else if (reader.GetImage().GetNumberOfDimensions() == 2){
        //not a multiframe image
        info_.setDz(1);
        info_.setZSpacing(1.0);
        info_.setNumberOfFrames(1);
      }
      else {
        if (getProgressBar())
          getProgressBar()->hide();
        throw FileException("Unexpected Number of Dimensions in Image File (Not supported): " + reader.GetImage().GetNumberOfDimensions(), slices.begin()->first);
      }
    }
    else
      info_.setNumberOfFrames(1);

    //sort slices by their distance from the origin, calculate Z spacing and do some additional checks
    if (slices.size() > 1) {
      //sort slices by distance from origin
      std::sort(slices.begin(), slices.end(), slices_cmp_dist);

      //check, if all images files are of dimension 2
      //also check if rescale intercept and slope are uniform and samples per pixel = 1 in all images
      int samplesPerPixel = 1;
      for (unsigned int i = 0; i < slices.size(); i++) {
        gdcm::ImageReader imR;
        imR.SetFileName(slices[i].first.c_str());
        imR.Read();
        if (imR.GetImage().GetNumberOfDimensions() != 2) {
          if (getProgressBar())
            getProgressBar()->hide();
          throw FileException("Image file has unexpected Dimensions (Multiple slices are required to have Dimension 2): " + imR.GetImage().GetNumberOfDimensions(), slices[i].first);
        }

        //check if PixelRepresentation is uniform
        if (imR.GetImage().GetPixelFormat().GetScalarType() != scalarType) {
          if (getProgressBar())
            getProgressBar()->hide();
          throw FileException("Image files do not have uniform scalar type!");
        }

        //check if rescale slope and intercept are the same for alle images, otherwise: warning
        if (info_.getIntercept() != static_cast<float>(imR.GetImage().GetIntercept())) {
          interceptDiffers = true;
          //LERROR("Intercept: " + ftos(static_cast<float>(imR.GetImage().GetIntercept())));
        }
        if (info_.getSlope() != static_cast<float>(imR.GetImage().GetSlope())) {
          slopeDiffers = true;
          //LERROR("Slope: " + ftos(static_cast<float>(imR.GetImage().GetSlope())));
        }

        //check if samples per pixel are uniformly = 1
        if ((imR.GetImage().GetPixelFormat().GetSamplesPerPixel() != samplesPerPixel) && (imR.GetImage().GetPixelFormat().GetSamplesPerPixel() != 1)) {
          samplesPerPixel = imR.GetImage().GetPixelFormat().GetSamplesPerPixel();
          LWARNING("Found image files with unsupported Pixel Format: " + itos(samplesPerPixel) + " Samples per Pixel instead of 1! Might lead to unexpected results.");
          info_.setSamplesPerPixel(samplesPerPixel);
        }
      }

      if (interceptDiffers)
        LWARNING("Rescale Intercept differs within the image files!");

      if (slopeDiffers)
        LWARNING("Rescale Slope differs within the image files!");

      //calculate Z-Spacing
      info_.setZSpacing(slices[1].second - slices[0].second);
      if (info_.getZSpacing() == 0){
        if (getProgressBar())
          getProgressBar()->hide();
        throw FileException("Slice Spacing is 0: Found two or more Slices with the same Position! (Either not a Volume or Slices have to be subdivided by additional Attributes)");
      }

      //check, if slice spacing remains constant (with 10% tolerance)
      for (unsigned int i = 0; i < slices.size() - 1; i++) {
        if (((slices[i + 1].second - slices[i].second) < 0.9*info_.getZSpacing()) || ((slices[i + 1].second - slices[i].second) > 1.1*info_.getZSpacing())) {
          if ((slices[i + 1].second - slices[i].second) == 0) {
            if (getProgressBar())
              getProgressBar()->hide();
            throw FileException("Slice Spacing is 0: Found two or more Slices with the same Position! (Either not a Volume or Slices have to be subdivided by additional Attributes)");
          }
          if (getProgressBar())
            getProgressBar()->hide();

          // display warning
          LWARNING("Slice Spacing is not steady (differs > 10% Tolerance)! The data set might be missing one or more slices!");
          break;
        }
      }

      info_.setDz(static_cast<int>(slices.size()));
    }

    //get position of first image to calculate offset
    gdcm::ImageReader yar;
    yar.SetFileName(slices[0].first.c_str());
    yar.Read();
    sf.SetFile(yar.GetFile()); //set file to gdcm::StringFilter for later use

    glm::dvec3 offset;
    const double* volumeOrigin = yar.GetImage().GetOrigin();
    offset.x = volumeOrigin[0];
    offset.y = volumeOrigin[1];
    offset.z = volumeOrigin[2];

    info_.setOffset(offset);

    //get pixel representation
    info_.setPixelRepresentation(yar.GetImage().GetPixelFormat().GetPixelRepresentation());

    LINFO("We have " << info_.getDz() << " slices. [" << info_.getDx() << "x" << info_.getDy() << "]");
    LINFO("Spacing: (" << info_.getXSpacing() << "; " << info_.getYSpacing() << "; " << info_.getZSpacing() << ")");

    //Determine scalar base type of the data set if not already known (and if possible)
    if (scalarType == gdcm::PixelFormat::UNKNOWN) {

      LWARNING("Pixel Format: Scalar Type in DICOM files is UNKNOWN or format is not well defined... trying to compute the right format.");

      if (info_.getPixelRepresentation()) { //signed
        switch (info_.getBitsStored()) {
        case 8:
          scalarType = gdcm::PixelFormat::INT8;
          break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          scalarType = gdcm::PixelFormat::INT16;
          break;
        case 24:
        case 32:
          scalarType = gdcm::PixelFormat::INT32;
          break;
        default:
          break;
        }
      }
      else {  //unsigned
        switch (info_.getBitsStored()) {
        case 8:
          scalarType = gdcm::PixelFormat::UINT8;
          break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
          scalarType = gdcm::PixelFormat::UINT16;
          break;
        case 24:
        case 32:
          scalarType = gdcm::PixelFormat::UINT32;
          break;
        default:
          break;
        }
      }
    }

    bool rwmDiffers = slopeDiffers || interceptDiffers;
    info_.setRwmDiffers(rwmDiffers);

    if (rwmDiffers) {
      if ((scalarType != gdcm::PixelFormat::UNKNOWN) && (info_.getSamplesPerPixel() == 1)) {
        LWARNING("Rescaling of pixel data necessary due to different scaling");
        //compute correct rescale slope and intercept values
        computeCorrectRescaleValues(slices);
      }
      else
        LWARNING("Rescaling of pixel data not possible due to unknown data type. Result may be broken.");
    }

    //write data base type as string to info object
    switch (scalarType) {
    case gdcm::PixelFormat::UINT8:
      info_.setBaseType("uint8"); break;
    case gdcm::PixelFormat::INT8:
      info_.setBaseType("int8"); break;
    case gdcm::PixelFormat::UINT12:
      info_.setBaseType("uint16"); LWARNING("unsigned int (12 bit) is interpreted as 16 bit - might lead to unexpected results"); break;
    case gdcm::PixelFormat::INT12:
      info_.setBaseType("int16"); LWARNING("signed int (12 bit) is interpreted as 16 bit - might lead to unexpected results"); break;
    case gdcm::PixelFormat::UINT16:
      info_.setBaseType("uint16"); break;
    case gdcm::PixelFormat::INT16:
      info_.setBaseType("int16"); break;
    case gdcm::PixelFormat::UINT32:
      info_.setBaseType("uint32"); break;
    case gdcm::PixelFormat::INT32:
      info_.setBaseType("int32"); break;
    case gdcm::PixelFormat::FLOAT32:
      info_.setBaseType("float"); break;
    case gdcm::PixelFormat::FLOAT64:
      info_.setBaseType("double"); break;
    case gdcm::PixelFormat::FLOAT16:
      throw Exception("Data format: 16 bit float not supported");
    case gdcm::PixelFormat::UNKNOWN:
      throw Exception("Unknown data format!");
    default:
      throw Exception("Data format could not be computed!");
    }

    //determine format by using samples per pixel and base type
    if (info_.getSamplesPerPixel() == 1)
      info_.setFormat(info_.getBaseType());
    else if (info_.getSamplesPerPixel() < 1 || info_.getSamplesPerPixel() > 4)
      throw Exception("Unsupported format: " + itos(info_.getSamplesPerPixel()) + " samples per pixel");
    else {
      //build format as vector type
      std::stringstream s;
      s << "Vector" << info_.getSamplesPerPixel() << "(" << info_.getBaseType() << ")";
      info_.setFormat(s.str());
    }

    //determine bytes per voxel
    VolumeFactory volumeFac;
    info_.setBytesPerVoxel(volumeFac.getBytesPerVoxel(info_.getFormat()));

    //copy slices into new vector for disk representation
    std::vector<std::string> sliceFilenamesOnly(slices.size());
    for (size_t i = 0; i < slices.size(); ++i)
      sliceFilenamesOnly.at(i) = slices.at(i).first;

    Volume* vh = 0;

    if (info_.getNumberOfFrames() > 1) {
      throw Exception("Multiframe DICOM file not supported yet");

    }
    else {
      //build volume raw representation
      VolumeRAM* volumeRAM = loadDicomSlices(info_, sliceFilenamesOnly);

      //build: VolumeDisk -> Volume
      vh = new Volume(volumeRAM,
        glm::vec3(static_cast<float>(info_.getXSpacing()), static_cast<float>(info_.getYSpacing()), static_cast<float>(info_.getZSpacing())), glm::vec3(0.f));
    }

    //get volume origin in world coordinates:
    glm::vec3 os(static_cast<float>(info_.getOffset().x), static_cast<float>(info_.getOffset().y), static_cast<float>(info_.getOffset().z));

    //construct PhysicalToWorld-Matrix for correct positioning of Volume
    glm::mat4 ptw(glm::dvec4(info_.getXOrientationPatient(), os.x),
      glm::dvec4(info_.getYOrientationPatient(), os.y), glm::dvec4(info_.getSliceNormal(), os.z), glm::dvec4(0, 0, 0, 1));
    vh->setPhysicalToWorldMatrix(ptw);

    //set Modality and VolumeURL
    //vh->setModality(Modality(info_.getModality()));
    vh->setOrigin(origin);
    vh->setRescaleIntercept(info_.getIntercept());
    vh->setRescaleSlope(info_.getSlope());
    vh->setWindowCenter(0);
    vh->setWindowWidth(0);

    //VolumeGL* vgl = new VolumeGL(vh->getRepresentation<VolumeRAM>());
    //vh->addRepresentation(vgl);

    if (getProgressBar())
      getProgressBar()->hide();

    return vh;
  }

  void GdcmVolumeReader::computeCorrectRescaleValues(std::vector<std::pair<std::string, double> > slices) {
    //for every slice, it is assumed that the whole domain of the scalar type values is used
    float rwmMin = std::numeric_limits<float>::max();
    float rwmMax = std::numeric_limits<float>::min();

    float dataTypeMin = 0, dataTypeMax = 0;

    //get max and min real world values
    std::vector<std::pair<std::string, double> >::iterator it_slices = slices.begin();

    int i = 0;
    while (it_slices != slices.end()) {
      if (getProgressBar()) {
        getProgressBar()->setProgressMessage("Calculating pixel rescaling...");
        getProgressBar()->setProgress(static_cast<float>(i) / static_cast<float>(slices.size()));
      }
      i++;

      gdcm::ImageReader reader;
      reader.SetFileName(it_slices->first.c_str());
      reader.Read();
      float slope = static_cast<float>(reader.GetImage().GetSlope());
      float intercept = static_cast<float>(reader.GetImage().GetIntercept());

      dataTypeMin = static_cast<float>(reader.GetImage().GetPixelFormat().GetMin());
      dataTypeMax = static_cast<float>(reader.GetImage().GetPixelFormat().GetMax());
      float sliceMin = dataTypeMin * slope + intercept;
      float sliceMax = dataTypeMax * slope + intercept;

      rwmMin = std::min(rwmMin, sliceMin);
      rwmMax = std::max(rwmMax, sliceMax);

      it_slices++;
    }

    //Calculate correct global slope and intercept values
    float globalSlope = (rwmMax - rwmMin) / (dataTypeMax - dataTypeMin + 1);
    float globalIntercept = rwmMin - dataTypeMin;

    info_.setSlope(globalSlope);
    info_.setIntercept(globalIntercept);

    if (getProgressBar())
      getProgressBar()->hide();
  }

  VolumeRAM* GdcmVolumeReader::loadDicomSlices(DicomInfo info, std::vector<std::string> sliceFiles)
    throw (FileException)
  {

    if (sliceFiles.size() < 1)
      throw FileException("No slice files to build volume!");

    LINFO("Building volume...");

    //build the actual volume
    VolumeFactory volumeFac;
    VolumeRAM* dataset = 0;
    try {
      //create data set for the slices
      dataset = volumeFac.create(info.getFormat(), glm::ivec3(info.getDx(), info.getDy(), sliceFiles.size()));
    }
    catch (std::exception& e) {
      LERROR(e.what());
      if (getProgressBar())
        getProgressBar()->hide();
      throw e;
    }

    LINFO("Reading slice data from " << sliceFiles.size() << " files...");

    size_t posScalar = 0;

    std::vector<std::string>::iterator it_slices = sliceFiles.begin();
    size_t i = 0;
    while (it_slices != sliceFiles.end()) {
      if (getProgressBar()) {
        getProgressBar()->setProgressMessage("Loading slice '" + FileSystem::fileName(*it_slices) + "' ...");
        getProgressBar()->setProgress(static_cast<float>(i) / static_cast<float>(sliceFiles.size()));
        i++;
      }

      int slicesize = loadSlice(reinterpret_cast<char*>(dataset->getData()), *it_slices, posScalar, info);

      if (slicesize == 0) {
        //obviously an error in loadSlice method
        delete[] dataset;
        if (getProgressBar())
          getProgressBar()->hide();
        throw FileException("Failed to read Pixel data.", *it_slices);
      }

      posScalar += slicesize;
      it_slices++;
    }
    if (getProgressBar())
      getProgressBar()->hide();

    LINFO("Building volume complete.");

    return dataset;
  }

  int GdcmVolumeReader::loadSlice(char* dataStorage, const std::string& fileName, size_t posScalar, DicomInfo info)
  {

    gdcm::ImageReader reader;
    reader.SetFileName(fileName.c_str());

    if (!reader.Read()){
      LERROR("Error loading file " << fileName);
      return 0;
    }

    size_t dataLength = (static_cast<size_t>(info.getDx()) * static_cast<size_t>(info.getDy())
      * static_cast<size_t>(info.getBytesPerVoxel()) * static_cast<size_t>(info.getNumberOfFrames()));

    if (reader.GetImage().GetBufferLength() != dataLength){
      LERROR("Failed to read Pixel data from file " << fileName << " because of unexpected Buffer Length!");
      return 0;
    }

    gdcm::PixelFormat scalarType = baseTypeStringToGdcm(info.getBaseType());

    //get pixel data
    if (info.rwmDiffers() && (scalarType != gdcm::PixelFormat::UNKNOWN) && (info.getSamplesPerPixel() == 1)) {
      //if rescale intercept and slope differ: recalculate the scalar values so that these fit the correct rescaling
      float slope = static_cast<float>(reader.GetImage().GetSlope());
      float intercept = static_cast<float>(reader.GetImage().GetIntercept());

      float nSlope = slope / info.getSlope();
      float nIntercept = intercept - info.getIntercept();

      //save the original buffer temporarily
      char* sliceScalars = new char[dataLength];
      reader.GetImage().GetBuffer(sliceScalars);


      //rescale and write buffer: use either gdcm rescaling or naive implementation (slower but might work for some data sets that the other method might not)
      bool useGdcmRescaling = true;
      if (useGdcmRescaling) {
        gdcm::Rescaler ir;
        ir.SetSlope(static_cast<double>(nSlope));
        ir.SetIntercept(static_cast<double>(nIntercept));
        ir.SetPixelFormat(scalarType);
        ir.SetUseTargetPixelType(true);
        ir.SetTargetPixelType(scalarType);
        ir.SetMinMaxForPixelType(static_cast<double>(gdcm::PixelFormat(scalarType).GetMin()), static_cast<double>(gdcm::PixelFormat(scalarType).GetMax()));
        ir.Rescale(&dataStorage[posScalar * info.getBytesPerVoxel()], sliceScalars, dataLength);
      }
      else {
        //do a dynamic cast depending on the data type and rescale every value
        switch (scalarType) {
        case gdcm::PixelFormat::UINT8:
        {
                                       uint8_t* typeScalars = reinterpret_cast<uint8_t*>(sliceScalars);
                                       size_t length = dataLength;

                                       for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                         float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                         float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                         typeScalars[voxelIndex] = static_cast<uint8_t>(rescaledValue);
                                       }
        }
          break;
        case gdcm::PixelFormat::INT8:
        {
                                      int8_t* typeScalars = reinterpret_cast<int8_t*>(sliceScalars);
                                      size_t length = dataLength;

                                      for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                        float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                        float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                        typeScalars[voxelIndex] = static_cast<int8_t>(rescaledValue);
                                      }
        }
          break;
        case gdcm::PixelFormat::UINT12:
        case gdcm::PixelFormat::UINT16:
        {
                                        uint16_t* typeScalars = reinterpret_cast<uint16_t*>(sliceScalars);
                                        size_t length = dataLength / 2;

                                        for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                          float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                          float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                          typeScalars[voxelIndex] = static_cast<uint16_t>(rescaledValue);
                                        }
        }
          break;
        case gdcm::PixelFormat::INT12:
        case gdcm::PixelFormat::INT16:
        {
                                       int16_t* typeScalars = reinterpret_cast<int16_t*>(sliceScalars);
                                       size_t length = dataLength / 2;

                                       for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                         float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                         float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                         typeScalars[voxelIndex] = static_cast<int16_t>(rescaledValue);
                                       }
        }
          break;
        case gdcm::PixelFormat::UINT32:
        {
                                        uint32_t* typeScalars = reinterpret_cast<uint32_t*>(sliceScalars);
                                        size_t length = dataLength / 4;

                                        for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                          float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                          float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                          typeScalars[voxelIndex] = static_cast<uint32_t>(rescaledValue);
                                        }
        }
          break;
        case gdcm::PixelFormat::INT32:
        {
                                       int32_t* typeScalars = reinterpret_cast<int32_t*>(sliceScalars);
                                       size_t length = dataLength / 4;

                                       for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                         float valueToRescale = static_cast<float>(typeScalars[voxelIndex]);
                                         float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                         typeScalars[voxelIndex] = static_cast<int32_t>(rescaledValue);
                                       }
        }
          break;
        case gdcm::PixelFormat::FLOAT16:
        case gdcm::PixelFormat::FLOAT32:
        {
                                         float* typeScalars = reinterpret_cast<float*>(sliceScalars);
                                         size_t length = dataLength / 4;

                                         for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                           float valueToRescale = typeScalars[voxelIndex];
                                           float rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                           typeScalars[voxelIndex] = rescaledValue;
                                         }
        }
          break;
        case gdcm::PixelFormat::FLOAT64:
        {
                                         double* typeScalars = reinterpret_cast<double*>(sliceScalars);
                                         size_t length = dataLength / 8;

                                         for (size_t voxelIndex = 0; voxelIndex < length; ++voxelIndex) {
                                           double valueToRescale = typeScalars[voxelIndex];
                                           double rescaledValue = ((valueToRescale * slope + intercept) - info.getIntercept()) / info.getSlope();
                                           typeScalars[voxelIndex] = rescaledValue;
                                         }
        }
          break;
        default:
          LERROR("Unexpected datatype while rescaling... no rescaling applied!");
        }

        //copy the rescaled values into the scalar buffer
        std::memcpy(&dataStorage[posScalar * info.getBytesPerVoxel()], sliceScalars, dataLength);
      }
      //delete temporary data
      delete[] sliceScalars;
    }
    else {
      reader.GetImage().GetBuffer(&dataStorage[posScalar * info.getBytesPerVoxel()]);
    }

    // Return number of voxels rendered
    return info.getDx() * info.getDy() * info.getNumberOfFrames();
  }

  gdcm::PixelFormat GdcmVolumeReader::baseTypeStringToGdcm(const std::string& type) {
    if (type == "uint8")
      return gdcm::PixelFormat::UINT8;
    else if (type == "int8")
      return gdcm::PixelFormat::INT8;
    else if (type == "uint16")
      return gdcm::PixelFormat::UINT16;
    else if (type == "int16")
      return gdcm::PixelFormat::INT16;
    else if (type == "uint32")
      return gdcm::PixelFormat::UINT32;
    else if (type == "int32")
      return gdcm::PixelFormat::INT32;
    else if (type == "float")
      return gdcm::PixelFormat::FLOAT32;
    else if (type == "double")
      return gdcm::PixelFormat::FLOAT64;
    else
      return gdcm::PixelFormat::UNKNOWN;
  }
}

