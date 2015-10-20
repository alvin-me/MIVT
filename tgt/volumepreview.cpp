#include "volumepreview.h"
#include "volume.h"
#include "volumeram.h"

#include <algorithm>

namespace tgt {

  const std::string VolumePreview::loggerCat_("VolumePreview");

  VolumePreview::VolumePreview()
    : height_(0)
  {}

  VolumePreview::VolumePreview(int height, const std::vector<unsigned char>& data)
    : height_(height)
    , prevData_(data)
  {}

  VolumeDerivedData* VolumePreview::createFrom(Volume *handle) const {
    assert(handle);

    int internHeight = 64;

    // gamma correction factor (amplifies low gray values)
    const float GAMMA = 1.8f;

    float xSpacing = handle->getSpacing()[0];
    float ySpacing = handle->getSpacing()[1];
    float xDimension = static_cast<float>(handle->getDimensions()[0]);
    float yDimension = static_cast<float>(handle->getDimensions()[1]);

    // determine offsets and scale factors for non-uniform aspect ratios
    float aspectRatio = (yDimension * ySpacing) / (xDimension * xSpacing);
    float xOffset, yOffset, xScale, yScale;
    if (aspectRatio <= 1.f) {
      xOffset = 0.f;
      yOffset = ((internHeight - 1.f) * (1.f - aspectRatio)) / 2.f;
      xScale = 1.f;
      yScale = 1.f / aspectRatio;
    }
    else {
      xOffset = ((internHeight - 1.f) * (1.f - 1.f / aspectRatio)) / 2.f;
      yOffset = 0.f;
      xScale = aspectRatio;
      yScale = 1.f;
    }

    float maxVal, minVal;
    std::vector<float> prevData = std::vector<float>(internHeight * internHeight);

    const VolumeRAM* volumeRam = 0;
    volumeRam = handle->getRepresentation<VolumeRAM>();
    assert(volumeRam);

    glm::vec3 position;
    position.z = glm::floor((handle->getDimensions().z - 1) / 2.0f);

    // generate preview in float buffer
    minVal = volumeRam->elementRange().y;
    maxVal = volumeRam->elementRange().x;
    for (int y = 0; y < internHeight; y++){
      for (int x = 0; x < internHeight; x++){
        position.x = ((x - xOffset) / (internHeight - 1)) * xScale * (xDimension - 1.f);
        position.y = ((y - yOffset) / (internHeight - 1)) * yScale * (yDimension - 1.f);
        int previewIndex = y*internHeight + x;
        float val = 0.f;
        if (position.x >= 0 && position.y >= 0 && position.x < xDimension && position.y < yDimension)
          val = volumeRam->getVoxelNormalizedLinear(position);
        prevData[previewIndex] = val;
        minVal = std::min(minVal, val);
        maxVal = std::max(maxVal, val);
      }
    }

    float valOffset = minVal;
    float valScale = maxVal - minVal;
    if (valScale > 0.f) {
      for (int y = 0; y<internHeight; y++) {
        for (int x = 0; x<internHeight; x++) {
          int previewIndex = y*internHeight + x;
          prevData[previewIndex] = pow((prevData[previewIndex] - valOffset) / valScale, 1.f / GAMMA);
        }
      }
    }

    std::vector<unsigned char> prevDataUInt8 = std::vector<unsigned char>(internHeight * internHeight);
    for (size_t i = 0; i < prevDataUInt8.size(); i++)
      prevDataUInt8[i] = static_cast<unsigned char>(std::roundf(prevData[i] * 255.f));

    return new VolumePreview(internHeight, prevDataUInt8);
  }

  int VolumePreview::getHeight() const {
    return height_;
  }

  const std::vector<unsigned char>& VolumePreview::getData() const {
    return prevData_;
  }

} // end namespace tgt