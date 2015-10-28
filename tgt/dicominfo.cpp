#include "dicominfo.h"

namespace tgt {

  DicomInfo::DicomInfo()
  {
  }

  void DicomInfo::setDx(int dx){
    dx_ = dx;
  }

  void DicomInfo::setDy(int dy){
    dy_ = dy;
  }

  void DicomInfo::setDz(int dz){
    dz_ = dz;
  }

  void DicomInfo::setNumberOfFrames(int frames) {
    numberOfFrames_ = frames;
  }

  void DicomInfo::setXSpacing(double x){
    xSpacing_ = x;
  }

  void DicomInfo::setYSpacing(double y){
    ySpacing_ = y;
  }

  void DicomInfo::setZSpacing(double z){
    zSpacing_ = z;
  }

  void DicomInfo::setXOrientationPatient(glm::dvec3 orientation){
    xOrientationPatient_ = orientation;
  }

  void DicomInfo::setYOrientationPatient(glm::dvec3 orientation){
    yOrientationPatient_ = orientation;
  }

  void DicomInfo::setSliceNormal(glm::dvec3 normal){
    sliceNormal_ = normal;
  }

  void DicomInfo::setOffset(glm::dvec3 offset){
    offset_ = offset;
  }

  void DicomInfo::setBitsStored(int bits){
    bitsStored_ = bits;
  }

  void DicomInfo::setSamplesPerPixel(int spp){
    samplesPerPixel_ = spp;
  }

  void DicomInfo::setBytesPerVoxel(int bytes){
    bytesPerVoxel_ = bytes;
  }

  void DicomInfo::setPixelRepresentation(unsigned short representation) {
    pixelRepresentation_ = representation;
  }

  void DicomInfo::setIntercept(float intercept){
    intercept_ = intercept;
  }

  void DicomInfo::setSlope(float slope){
    slope_ = slope;
  }

  void DicomInfo::setRescaleType(std::string type){
    rescaleType_ = type;
  }

  void DicomInfo::setModality(std::string modality){
    modality_ = modality;
  }

  void DicomInfo::setSeriesInstanceUID(std::string uid){
    seriesInstanceUID_ = uid;
  }

  void DicomInfo::setStudyInstanceUID(std::string uid){
    studyInstanceUID_ = uid;
  }

  void DicomInfo::setSeriesDescription(std::string description){
    seriesDescription_ = description;
  }

  void DicomInfo::setStudyDescription(std::string description){
    studyDescription_ = description;
  }

  void DicomInfo::setPatientName(std::string name){
    patientName_ = name;
  }

  void DicomInfo::setPatientId(std::string id){
    patientId_ = id;
  }

  void DicomInfo::setRwmDiffers(bool d) {
    rwmDiffers_ = d;
  }

  void DicomInfo::setFormat(std::string format) {
    format_ = format;
  }

  void DicomInfo::setBaseType(std::string type) {
    baseType_ = type;
  }

  int DicomInfo::getDx() const {
    return dx_;
  }

  int DicomInfo::getDy() const {
    return dy_;
  }


  int DicomInfo::getDz() const {
    return dz_;
  }

  int DicomInfo::getNumberOfFrames() const {
    return numberOfFrames_;
  }

  double DicomInfo::getXSpacing() const {
    return xSpacing_;
  }

  double DicomInfo::getYSpacing() const {
    return ySpacing_;
  }

  double DicomInfo::getZSpacing() const {
    return zSpacing_;
  }

  glm::dvec3 DicomInfo::getXOrientationPatient() const {
    return xOrientationPatient_;
  }

  glm::dvec3 DicomInfo::getYOrientationPatient() const {
    return yOrientationPatient_;
  }

  glm::dvec3 DicomInfo::getSliceNormal() const {
    return sliceNormal_;
  }

  glm::dvec3 DicomInfo::getOffset() const {
    return offset_;
  }

  int DicomInfo::getBitsStored() const {
    return bitsStored_;
  }

  int DicomInfo::getSamplesPerPixel() const {
    return samplesPerPixel_;
  }

  unsigned short DicomInfo::getPixelRepresentation() const {
    return pixelRepresentation_;
  }

  int DicomInfo::getBytesPerVoxel() const {
    return bytesPerVoxel_;
  }

  float DicomInfo::getIntercept() const {
    return intercept_;
  }

  float DicomInfo::getSlope() const {
    return slope_;
  }

  std::string DicomInfo::getRescaleType() const {
    return rescaleType_;
  }

  std::string DicomInfo::getModality() const {
    return modality_;
  }

  std::string DicomInfo::getSeriesInstanceUID() const {
    return seriesInstanceUID_;
  }

  std::string DicomInfo::getStudyInstanceUID() const {
    return studyInstanceUID_;
  }

  std::string DicomInfo::getSeriesDescription() const {
    return seriesDescription_;
  }

  std::string DicomInfo::getStudyDescription() const {
    return studyDescription_;
  }

  std::string DicomInfo::getPatientName() const {
    return patientName_;
  }

  std::string DicomInfo::getPatientId() const {
    return patientId_;
  }

  bool DicomInfo::rwmDiffers() const {
    return rwmDiffers_;
  }

  std::string DicomInfo::getFormat() const {
    return format_;
  }

  std::string DicomInfo::getBaseType() const {
    return baseType_;
  }

} // end namespace tgt
