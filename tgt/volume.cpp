#include "volume.h"
#include "volumeram.h"
#include "logmanager.h"
#include "volumepreview.h"
#include "volumegl.h"
#include "volumeminmax.h"
#include "volumehistogram.h"

namespace tgt {

  Volume::Volume()
    : spacing_(0), offset_(0), origin_("")
  {}

  Volume::Volume(VolumeRepresentation* const volume, 
    const glm::vec3& spacing, 
    const glm::vec3& offset, 
    const glm::mat4& physical2world, 
    const std::string origin,
    float rescaleIntercept, 
    float rescaleSlope, 
    float windowCenter, 
    float windowWidth)
    : spacing_(spacing)
    , offset_(offset)
    , physical2world_(physical2world)
    , origin_(origin)
    , rescaleIntercept_(rescaleIntercept)
    , rescaleSlope_(rescaleSlope)
    , windowCenter_(windowCenter)
    , windowWidth_(windowWidth)
    , ready_(false)
  {
    addRepresentationInternal(volume);
  }

  Volume::~Volume() {
    clearRepresentation();
    clearDerivedData();
  }

  void Volume::setSpacing(const glm::vec3 spacing) {
    spacing_ = spacing;
  }

  void Volume::setOffset(const glm::vec3 offset) {
    offset_ = offset;
  }

  void Volume::setRescaleIntercept(float value) {
    rescaleIntercept_ = value;
  }

  void Volume::setRescaleSlope(float value) {
    rescaleSlope_ = value;
  }

  void Volume::setWindowCenter(float value) {
    windowCenter_ = value;
  }

  void Volume::setWindowWidth(float value) {
    windowWidth_ = value;
  }

  void Volume::setOrigin(const std::string& origin) {
    origin_ = origin;
  }

  void Volume::setPhysicalToWorldMatrix(const glm::mat4& m) {
    physical2world_ = m;
  }

  std::string Volume::getFormat() {
    VolumeRAM* volumeRam = getRepresentation<VolumeRAM>();
    assert(volumeRam);

    return volumeRam->getFormat();
  }

  glm::ivec3 Volume::getDimensions() {
    const VolumeRepresentation* rep = representations_[0];
    if (rep)
      return rep->getDimensions();
    else {
      assert(false);
      return glm::ivec3(0, 0, 0);
    }
  }

  size_t Volume::getNumVoxels() {
    const VolumeRepresentation* rep = representations_[0];
    if (rep)
      return rep->getNumVoxels();
    else {
      assert(false);
      return 0;
    }
  }

  size_t Volume::getBytesPerVoxel() {
    const VolumeRepresentation* rep = representations_[0];
    if (rep)
      return rep->getBytesPerVoxel();
    else {
      assert(false);
      return 0;
    }
  }

  glm::vec3 Volume::getSpacing() {
    return spacing_;
  }

  glm::vec3 Volume::getOffset() {
    return offset_;
  }

  float Volume::getRescaleIntercept() {
    return rescaleIntercept_;
  }

  float Volume::getRescaleSlope() {
    return rescaleSlope_;
  }

  ValueMapping Volume::getRescaleMapping() {
    return ValueMapping(getRescaleSlope(), getRescaleIntercept());
  }

  float Volume::getWindowCenter() {
    return windowCenter_;
  }

  float Volume::getWindowWidth() {
    return windowWidth_;
  }

  //glm::vec2 Volume::defaultWindowing() {
  //  if (windowWidth_ <= 0) {
  //    if (getDerivedData<VolumeMinMax>()) {
  //      float lower = getDerivedData<VolumeMinMax>()->getMinHu();
  //      float upper = getDerivedData<VolumeMinMax>()->getMaxHu();
  //      return glm::vec2(upper - lower, lower + (upper - lower) * 0.5f);
  //    }
  //  }
  //  else {
  //    return glm::vec2(windowWidth_, windowCenter_);
  //  }

  //  LWARNINGC("Volume", "defaultWindowingDomain(): cannot.");
  //  return glm::vec2(0);
  //}

  std::string& Volume::getOrigin() {
    return origin_;
  }


  glm::vec3 Volume::getCubeSize() {
    glm::vec3 cubeSize = glm::vec3(getDimensions()) * getSpacing();
    return cubeSize;
  }

  std::vector<glm::vec3> Volume::getCubeVertices() {
    std::vector<glm::vec3> cubeVertices;
    glm::vec3 llf = getLLF();
    glm::vec3 urb = getURB();

    cubeVertices.push_back(glm::vec3(llf.x, llf.y, urb.z));// llb 0
    cubeVertices.push_back(glm::vec3(urb.x, llf.y, urb.z));// lrb 1
    cubeVertices.push_back(glm::vec3(urb.x, urb.y, urb.z));// urb 2
    cubeVertices.push_back(glm::vec3(llf.x, urb.y, urb.z));// ulb 3

    cubeVertices.push_back(glm::vec3(llf.x, llf.y, llf.z));// llf 4
    cubeVertices.push_back(glm::vec3(urb.x, llf.y, llf.z));// lrf 5
    cubeVertices.push_back(glm::vec3(urb.x, urb.y, llf.z));// urf 6
    cubeVertices.push_back(glm::vec3(llf.x, urb.y, llf.z));// ulf 7
    return cubeVertices;
  }

  glm::vec3 Volume::getLLF() {
    return getOffset();
  }

  glm::vec3 Volume::getURB() {
    return getOffset() + getCubeSize();
  }

  glm::mat4 Volume::getTextureToVoxelMatrix() {
    return glm::scale(glm::vec3(getDimensions()));
  }

  glm::mat4 Volume::getVoxelToTextureMatrix() {
    return glm::scale(1.0f / glm::vec3(getDimensions()));
  }

  glm::mat4 Volume::getVoxelToPhysicalMatrix() {
    // 1. Multiply by spacing 2. Apply offset
    return glm::translate(getOffset()) * glm::scale(getSpacing());
  }

  glm::mat4 Volume::getPhysicalToVoxelMatrix() {
    return glm::scale(1.0f / getSpacing()) * glm::translate(-getOffset());
  }

  glm::mat4 Volume::getPhysicalToWorldMatrix() {
    return physical2world_;
  }

  glm::mat4 Volume::getWorldToPhysicalMatrix() {
    return glm::inverse(getPhysicalToWorldMatrix());
  }

  glm::mat4 Volume::getVoxelToWorldMatrix() {
    return getPhysicalToWorldMatrix() * getVoxelToPhysicalMatrix();
  }

  glm::mat4 Volume::getWorldToVoxelMatrix() {
    return glm::inverse(getVoxelToWorldMatrix());
  }

  glm::mat4 Volume::getTextureToWorldMatrix() {
    return getVoxelToWorldMatrix() * getTextureToVoxelMatrix();
  }

  glm::mat4 Volume::getWorldToTextureMatrix() {
    return glm::inverse(getTextureToWorldMatrix());
  }

  glm::mat4 Volume::getTextureToPhysicalMatrix() {
    return getVoxelToPhysicalMatrix() * getTextureToVoxelMatrix();
  }

  glm::mat4 Volume::getPhysicalToTextureMatrix() {
    return getVoxelToTextureMatrix() * getPhysicalToVoxelMatrix();
  }

  template <class T>
  T* Volume::getRepresentation()
  {
    T* test = hasRepresentation<T>();
    if (test)
      return test;

    // if not have, create a new one.
    VolumeRepresentation* result = 0;
    // now we only have one Representation data created auto, that's VolumeGL.
    if (typeid(T) == typeid(VolumeGL)) {
      result = new VolumeGL(getRepresentation<VolumeRAM>());
    }
    else if (typeid(T) == typeid(VolumeRAM)) {
      LERRORC("Volume", "VolumeRAM data should read first");
      throw std::invalid_argument("VolumeRAM data should read first");
    }

    if (result)
      addRepresentationInternal<T>(dynamic_cast<T*>(result));

    return hasRepresentation<T>();
  }

  template<class T>
  T* Volume::hasRepresentation() const
  {
    for (std::vector<VolumeRepresentation*>::const_iterator it = representations_.begin(); 
      it != representations_.end(); ++it) {
      if (dynamic_cast<T*>(*it)) {
        return dynamic_cast<T*>(*it);
      }
    }
    return 0;
  }

  template<class T>
  void Volume::addRepresentationInternal(T* data)
  {
    if (!dynamic_cast<VolumeRepresentation*>(data)) {
      LERRORC("Volume", "VolumeRepresentation data item is not of type VolumeRepresentation");
      throw std::invalid_argument("passed data item is not of type VolumeRepresentation");
    }

    if (hasRepresentation<T>())
      removeRepresentationInternal<T>();

    representations_.push_back(dynamic_cast<VolumeRepresentation*>(data));
  }

  template<class T>
  void Volume::removeRepresentationInternal()
  {
    if (!hasRepresentation<T>())
      return;

    for (std::vector<VolumeRepresentation*>::iterator it = representations_.begin(); 
      it != representations_.end(); ++it) {
      if (dynamic_cast<T*>(*it)) {
        delete *it;
        representations_.erase(it);
        return;
      }
    }
  }

  void Volume::clearRepresentation()
  {
      while (!representations_.empty()) {
        delete representations_.back();
        representations_.pop_back();
      }
  }

  /// derived data

  template <class T>
  T* Volume::getDerivedData() {
    T* test = hasDerivedData<T>();
    if (test)
      return test;

    // if not have, create a new one.
    T dummy;
    VolumeDerivedData* result = 0;
    result = dummy.createFrom(this);

    if (result)
      addDerivedDataInternal<T>(dynamic_cast<T*>(result));

    return hasDerivedData<T>();
  }

  template<class T>
  T* Volume::hasDerivedData() const {
    for (std::vector<VolumeDerivedData*>::const_iterator it = derivedData_.begin(); it != derivedData_.end(); ++it) {
      if (typeid(**it) == typeid(T)) {
        T* tmp = dynamic_cast<T*>(*it);
        return tmp;
      }
    }
    return 0;
  }

  template<class T>
  void Volume::addDerivedDataInternal(T* data) {
    if (!dynamic_cast<VolumeDerivedData*>(data)) {
      LERRORC("Volume", "derived data item is not of type VolumeDerivedData");
      throw std::invalid_argument("passed data item is not of type VolumeDerivedData");
    }

    if (hasDerivedData<T>())
      removeDerivedDataInternal<T>();

    derivedData_.push_back(dynamic_cast<VolumeDerivedData*>(data));

  }

  template<class T>
  void Volume::removeDerivedDataInternal() {
    if (!hasDerivedData<T>())
      return;

    for (std::vector<VolumeDerivedData*>::iterator it = derivedData_.begin(); it != derivedData_.end(); ++it) {
      if (dynamic_cast<T*>(*it)) {
        delete *it;
        derivedData_.erase(it);
        return;
      }
    }
  }

  void Volume::clearDerivedData() {
    while (!derivedData_.empty()) {
      delete derivedData_.back();
      derivedData_.pop_back();
    }
  }

  void Volume::SetReady(bool flag)
  {
    ready_ = flag;
  }

  bool Volume::IsReady()
  {
    return ready_;
  }

  void Volume::SyncData()
  {
    VolumeRAM* volumeRam = getRepresentation<VolumeRAM>();
    assert(volumeRam);
    VolumeGL* volumeGl = getRepresentation<VolumeGL>();
    assert(volumeGl);
    volumeGl->getTexture()->setPixelData(reinterpret_cast<GLubyte*>(volumeRam->getData()));
    volumeGl->getTexture()->uploadTexture();
    volumeGl->getTexture()->setPixelData(0);
  }

  //------------------------------------------------------------------------------

  void oldVolumePosition(Volume* vh) {
    glm::vec3 cubeSize = glm::vec3(vh->getDimensions()) * vh->getSpacing();
    glm::mat4 matrix = glm::translate(-0.5f * cubeSize);
    vh->setPhysicalToWorldMatrix(matrix);
  }

} // end namespace tgt
