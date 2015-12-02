#include "cubeproxygeometry.h"
#include "volume.h"
#include "trianglemeshgeometry.h"
#include "logmanager.h"

namespace mivt {

  const std::string CubeProxyGeometry::loggerCat_("CubeProxyGeometry");

  CubeProxyGeometry::CubeProxyGeometry()
    : enableClipping_(true)
    , clipRight_(0.f)
    , clipLeft_(1e5f)
    , clipFront_(0.f)
    , clipBack_(1e5f)
    , clipBottom_(0.f)
    , clipTop_(1e5f)
    , volume_(0)
    , geometry_(0)
  {
  }


  CubeProxyGeometry::~CubeProxyGeometry()
  {
    DELPTR(geometry_);
  }

  void CubeProxyGeometry::EnableClip(bool flag)
  {
    enableClipping_ = flag;
  }

  void CubeProxyGeometry::SetVolume(tgt::Volume *volume)
  {
    volume_ = volume;
    adjustClipPropertiesRanges();
  }

  void CubeProxyGeometry::adjustClipPropertiesRanges()
  {
    if (!volume_) {
      LERROR("No input volume.");
      return;
    }

    glm::ivec3 numSlices = volume_->getDimensions();

    clipRight_ = 0;
    clipLeft_ = static_cast<float>(numSlices.x - 1) / 2.0f;
    clipFront_ = 0;
    clipBack_ = static_cast<float>(numSlices.y - 1);
    clipBottom_ = 0;
    clipTop_ = static_cast<float>(numSlices.z - 1);
  }

  void CubeProxyGeometry::resetClipPlanes()
  {
    adjustClipPropertiesRanges();
  }

  void CubeProxyGeometry::ChangeClipRight(float val)
  {
    if (val < 0 || val > volume_->getDimensions().x)
      return;

    clipRight_ = val;
    if (clipRight_ > clipLeft_)
      clipLeft_ = clipRight_;
  }

  void CubeProxyGeometry::ChangeClipLeft(float val)
  {
    if (val < 0 || val > volume_->getDimensions().x)
      return;

    clipLeft_ = val;
    if (clipRight_ > clipLeft_)
      clipRight_ = clipLeft_;
  }

  void CubeProxyGeometry::ChangeClipBack(float val)
  {
    if (val < 0 || val > volume_->getDimensions().y)
      return;

    clipBack_ = val;
    if (clipFront_> clipBack_)
      clipFront_ = clipBack_;
  }

  void CubeProxyGeometry::ChangeClipFront(float val)
  {
    if (val < 0 || val > volume_->getDimensions().y)
      return;

    clipFront_ = val;
    if (clipFront_> clipBack_)
      clipBack_ = clipFront_;
  }

  void CubeProxyGeometry::ChangeClipBottom(float val)
  {
    if (val < 0 || val > volume_->getDimensions().z)
      return;

    clipBottom_ = val;
    if (clipBottom_ > clipTop_)
      clipTop_ = clipBottom_;
  }

  void CubeProxyGeometry::ChangeClipTop(float val)
  {
    if (val < 0 || val > volume_->getDimensions().z)
      return;

    clipTop_ = val;
    if (clipBottom_ > clipTop_)
      clipBottom_ = clipTop_;
  }

  void CubeProxyGeometry::Process()
  {
    glm::vec3 volumeSize = volume_->getCubeSize();
    glm::ivec3 numSlices = volume_->getDimensions();

    // vertex and tex coords of bounding box without clipping
    glm::vec3 coordLlf = volume_->getLLF();
    glm::vec3 coordUrb = volume_->getURB();
    const glm::vec3 noClippingTexLlf(0, 0, 0);
    const glm::vec3 noClippingTexUrb(1, 1, 1);

    glm::vec3 texLlf;
    glm::vec3 texUrb;
    if (enableClipping_) {
      // adjust vertex and tex coords to clipping
      texLlf = glm::vec3(
        clipRight_ / static_cast<float>(numSlices.x),
        clipFront_ / static_cast<float>(numSlices.y),
        clipBottom_ / static_cast<float>(numSlices.z));
      texUrb = glm::vec3(
        (clipLeft_ + 1.0f) / static_cast<float>(numSlices.x),
        (clipBack_ + 1.0f) / static_cast<float>(numSlices.y),
        (clipTop_ + 1.0f) / static_cast<float>(numSlices.z));

      coordLlf -= volumeSize * (noClippingTexLlf - texLlf);
      coordUrb -= volumeSize * (noClippingTexUrb - texUrb);
    }
    else {
      texLlf = noClippingTexLlf;
      texUrb = noClippingTexUrb;
    }

    DELPTR(geometry_);
    geometry_ = tgt::TriangleMeshGeometryVec4Vec3::createCube(texLlf, texUrb, texLlf, texUrb, 1.0f);
    geometry_->transform(volume_->getTextureToWorldMatrix());
  }

  tgt::Geometry* CubeProxyGeometry::GetGeometry()
  {
    return geometry_;
  }

  float CubeProxyGeometry::GetClipRight()
  {
    return clipRight_;
  }

  float CubeProxyGeometry::GetClipLeft()
  {
    return clipLeft_;
  }

  float CubeProxyGeometry::GetClipBack()
  {
    return clipBack_;
  }

  float CubeProxyGeometry::GetClipFront()
  {
    return clipFront_;
  }

  float CubeProxyGeometry::GetClipBottom()
  {
    return clipBottom_;
  }

  float CubeProxyGeometry::GetClipTop()
  {
    return clipTop_;
  }

  bool CubeProxyGeometry::IsClipEnabled()
  {
    return enableClipping_;
  }

}