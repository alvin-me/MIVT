#include "rawvolumereader.h"
#include "logmanager.h"
#include "volume.h"
#include "volumeatomic.h"
#include "volumegl.h"

#pragma warning(disable:4996)

namespace tgt {

  const std::string RawVolumeReader::loggerCat_ = "RawVolumeReader";

  RawVolumeReader::ReadHints::ReadHints(glm::ivec3 dimensions, glm::vec3 spacing,
    const std::string& format, size_t headerskip, bool bigEndian,
    float rescaleIntercept, float rescaleSlope, float windowCenter, float windowWidth)
    : dimensions_(dimensions), spacing_(spacing),
    format_(format), headerskip_(headerskip),
    bigEndianByteOrder_(bigEndian),
    rescaleIntercept_(rescaleIntercept),
    rescaleSlope_(rescaleSlope),
    windowCenter_(windowCenter),
    windowWidth_(windowWidth),
    transformation_(glm::mat4(1)),
    sliceOrder_("+z")
  {}

  RawVolumeReader::RawVolumeReader()
    : VolumeReader()
  {
    protocols_.push_back("raw");
  }

  void RawVolumeReader::setReadHints(glm::ivec3 dimensions, glm::vec3 spacing,
    const std::string& format, int headerskip, bool bigEndian,
    float rescaleIntercept, float rescaleSlope, float windowCenter, float windowWidth)
  {
    hints_ = ReadHints(dimensions, spacing, format, headerskip, bigEndian, rescaleIntercept, rescaleSlope, windowCenter, windowWidth);
  }

  void RawVolumeReader::setReadHints(const ReadHints& hints) {
    hints_ = hints;
  }

  Volume* RawVolumeReader::read(const std::string& fileName)
    throw (IOException, CorruptedFileException, std::bad_alloc)
  {
    ReadHints& h = hints_;

    // check dimensions
    if (glm::any(glm::lessThanEqual(h.dimensions_, glm::ivec3(0))) || glm::any(glm::greaterThan(h.dimensions_, glm::ivec3(10000)))) {
      LERROR("Invalid volume dimensions: " << glm::to_string(h.dimensions_));
      return 0;
    }

    std::string info = "Loading raw file " + fileName + " ";

    FILE* fin;
    fin = fopen(fileName.c_str(), "rb");

    if (fin == 0)
      throw IOException("Unable to open raw file for reading", fileName);

    VolumeRAM* volumeRAM;

    if (h.format_ == "UCHAR") {
      LINFO(info << "(8 bit dataset)");
      VolumeRAM_UInt8* v = new VolumeRAM_UInt8(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "CHAR") {
      LINFO(info << "(8 bit signed dataset)");
      VolumeRAM_Int8* v = new VolumeRAM_Int8(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "USHORT" || h.format_ == "USHORT_12") {
      LINFO(info << "(16 bit dataset)");
      VolumeRAM_UInt16* v = new VolumeRAM_UInt16(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "SHORT") {
      LINFO(info << "(16 bit signed dataset)");
      VolumeRAM_Int16* v = new VolumeRAM_Int16(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "UINT") {
      LINFO(info << "(32 bit dataset)");
      VolumeRAM_UInt32* v = new VolumeRAM_UInt32(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "INT") {
      LINFO(info << "(32 bit signed dataset)");
      VolumeRAM_Int32* v = new VolumeRAM_Int32(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "UINT64") {
      LINFO(info << "(64 bit dataset)");
      VolumeRAM_UInt64* v = new VolumeRAM_UInt64(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "INT64") {
      LINFO(info << "(64 bit signed dataset)");
      VolumeRAM_Int64* v = new VolumeRAM_Int64(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "FLOAT") {
      LINFO(info << "(32 bit float dataset)");
      VolumeRAM_Float* v = new VolumeRAM_Float(h.dimensions_);
      volumeRAM = v;
    }
    else if (h.format_ == "DOUBLE") {
      LINFO(info << "(64 bit double dataset)");
      VolumeRAM_Double* v = new VolumeRAM_Double(h.dimensions_);
      volumeRAM = v;
    }
    else {
      fclose(fin);
      throw CorruptedFileException("Format '" + h.format_ + "' not supported", fileName);
    }

    // now add that to the headerskip we might have received
    uint64_t offset = h.headerskip_;

#ifdef _MSC_VER
    _fseeki64(fin, offset, SEEK_SET);
#else
    fseek(fin, offset, SEEK_SET);
#endif

    volumeRAM->clear();

    VolumeReader::read(volumeRAM, fin);

    fclose(fin);

    if (h.sliceOrder_ == "-x") {
      LINFO("slice order is -x, reversing order to +x...\n");
      reverseXSliceOrder(volumeRAM);
    }
    else if (h.sliceOrder_ == "-y") {
      LINFO("slice order is -y, reversing order to +y...\n");
      reverseYSliceOrder(volumeRAM);
    }
    else if (h.sliceOrder_ == "-z") {
      LINFO("slice order is -z, reversing order to +z...\n");
      reverseZSliceOrder(volumeRAM);
    }

    if (h.bigEndianByteOrder_) {
      volumeRAM->swapEndianness();
    }

    glm::vec3 offs(0.0f);
    Volume* volumeHandle = new Volume(volumeRAM, h.spacing_, offs);
    volumeHandle->setOrigin(fileName);
    volumeHandle->setPhysicalToWorldMatrix(h.transformation_);
    volumeHandle->setRescaleIntercept(h.rescaleIntercept_);
    volumeHandle->setRescaleSlope(h.rescaleSlope_);
    volumeHandle->setWindowCenter(h.windowCenter_);
    volumeHandle->setWindowWidth(h.windowWidth_);

    //VolumeGL* vgl = new VolumeGL(volumeRAM);
    //volumeHandle->addRepresentation(vgl);

    return volumeHandle;
  }

} // end namespace tgt