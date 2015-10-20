#include "volumereader.h"

#include "logmanager.h"
#include "volumeram.h"

namespace tgt {

  const std::string VolumeReader::loggerCat_("VolumeReader");

  VolumeReader::VolumeReader(ProgressBar* progress /*= 0*/)
    : progress_(progress)
  {}

  void VolumeReader::read(VolumeRAM* volume, FILE* fin) {
    if (fread(reinterpret_cast<char*>(volume->getData()), 1, volume->getNumBytes(), fin) == 0)
      LWARNING("fread() failed");
  }

  void VolumeReader::reverseXSliceOrder(VolumeRAM* const volume) const {
    if (volume == 0)
      return;

    size_t bytesPerVoxel = volume->getBytesPerVoxel();
    const glm::ivec3 dim = volume->getDimensions();

    typedef unsigned char BYTE;
    BYTE* const data = reinterpret_cast<BYTE* const>(volume->getData());
    BYTE* aux = new BYTE[bytesPerVoxel];

    for (int x = 0; x < (dim.x / 2); ++x) {
      int _x = (dim.x - (x + 1));
      for (int z = 0; z < dim.z; ++z) {
        int a = (z * (dim.x * dim.y));
        for (int y = 0; y < dim.y; ++y) {
          int b = (y * dim.x);
          int l = a + b + x;
          int r = a + b + _x;
          memcpy(aux, (data + (l * bytesPerVoxel)), bytesPerVoxel);
          memcpy((data + (l * bytesPerVoxel)), (data + (r * bytesPerVoxel)), bytesPerVoxel);
          memcpy((data + (r * bytesPerVoxel)), aux, bytesPerVoxel);
        }
      }
    }
    delete[] aux;
  }

  void VolumeReader::reverseYSliceOrder(VolumeRAM* const volume) const {
    if (volume == 0)
      return;

    size_t bytesPerVoxel = volume->getBytesPerVoxel();
    const glm::ivec3 dim = volume->getDimensions();

    typedef unsigned char BYTE;
    BYTE* const data = reinterpret_cast<BYTE* const>(volume->getData());
    BYTE* aux = new BYTE[bytesPerVoxel];

    for (int y = 0; y < (dim.y / 2); ++y) {
      int _y = (dim.y - (y + 1));
      int b = (y * dim.x);
      int _b = (_y * dim.x);
      for (int z = 0; z < dim.z; ++z) {
        int a = (z * (dim.x * dim.y));
        for (int x = 0; x < dim.x; ++x) {
          int l = a + b + x;
          int r = a + _b + x;
          memcpy(aux, (data + (l * bytesPerVoxel)), bytesPerVoxel);
          memcpy((data + (l * bytesPerVoxel)), (data + (r * bytesPerVoxel)), bytesPerVoxel);
          memcpy((data + (r * bytesPerVoxel)), aux, bytesPerVoxel);
        }
      }
    }
    delete[] aux;
  }

  void VolumeReader::reverseZSliceOrder(VolumeRAM* const volume) const {
    if (volume == 0)
      return;

    size_t bytesPerVoxel = volume->getBytesPerVoxel();
    const glm::ivec3 dim = volume->getDimensions();
    size_t sliceSize = dim.x * dim.y * bytesPerVoxel;

    typedef unsigned char BYTE;
    BYTE* const data = reinterpret_cast<BYTE* const>(volume->getData());
    BYTE* aux = new BYTE[sliceSize];
    for (int z = 0; z < dim.z / 2; ++z) {
      // copy slice i to auxiliary buffer
      //
      memcpy(aux, (data + (z * sliceSize)), sliceSize);

      // copy slice r = (dim.z - 1 - i) to position i
      //
      int r = (dim.z - (z + 1));
      memcpy((data + (z * sliceSize)), (data + (r * sliceSize)), sliceSize);

      // copy auxiliary buffer back to position r
      //
      memcpy((data + (r * sliceSize)), aux, sliceSize);
    }
    delete[] aux;
  }

  const std::vector<std::string>& VolumeReader::getSupportedProtocols() const {
    return protocols_;
  }

  void VolumeReader::setProgressBar(ProgressBar* progressBar) {
    progress_ = progressBar;
  }

  ProgressBar* VolumeReader::getProgressBar() const {
    return progress_;
  }

} // end namespace tgt