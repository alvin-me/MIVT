#pragma once

#include "tgt_math.h"
#include "valuemapping.h"

#include <vector>
#include <set>

namespace tgt {

  class VolumeRepresentation;
  class VolumeDerivedData;
  class VolumeRAM;

  class Volume {
  public:
    TGT_API Volume();
    TGT_API Volume(VolumeRepresentation* const volume,
      const glm::vec3& spacing, 
      const glm::vec3& offset, 
      const glm::mat4& physical2world = glm::mat4(1), 
      const std::string origin = "",
      float rescaleIntercept = 0.f, 
      float rescaleSlope = 1.f,
      float windowCenter = 0.f, 
      float windowWidth = 0.f);

    TGT_API ~Volume();

    TGT_API void setSpacing(const glm::vec3 spacing);
    TGT_API void setOffset(const glm::vec3 offset);
    TGT_API void setRescaleIntercept(float value);
    TGT_API void setRescaleSlope(float value);
    TGT_API void setWindowCenter(float value);
    TGT_API void setWindowWidth(float value);
    TGT_API void setOrigin(const std::string& origin);
    TGT_API void setPhysicalToWorldMatrix(const glm::mat4& m);

    TGT_API std::string getFormat();
    TGT_API glm::ivec3 getDimensions();
    TGT_API glm::vec3 getSpacing();
    TGT_API glm::vec3 getOffset();
    TGT_API size_t getNumVoxels();
    TGT_API size_t getBytesPerVoxel();
    TGT_API float getRescaleIntercept();
    TGT_API float getRescaleSlope();
    TGT_API ValueMapping getRescaleMapping();
    TGT_API float getWindowCenter();
    TGT_API float getWindowWidth();
    //glm::vec2 defaultWindowing();
    TGT_API std::string& getOrigin();

    /// physical coordinates.
    TGT_API std::vector<glm::vec3> getCubeVertices();
    TGT_API glm::vec3 getCubeSize();
    TGT_API glm::vec3 getLLF();
    TGT_API glm::vec3 getURB();

    /**
    * voxel coordinates (i.e. [0; dim-1]) 
    * texture coordinates (i.e. [0.0; 1.0]).
    * texture <*dim> voxel <*spacing+offset> physical <> world
    */
    TGT_API glm::mat4 getTextureToVoxelMatrix();
    TGT_API glm::mat4 getVoxelToTextureMatrix();

    TGT_API glm::mat4 getVoxelToPhysicalMatrix();
    TGT_API glm::mat4 getPhysicalToVoxelMatrix();

    TGT_API glm::mat4 getPhysicalToWorldMatrix();
    TGT_API glm::mat4 getWorldToPhysicalMatrix();

    TGT_API glm::mat4 getVoxelToWorldMatrix();
    TGT_API glm::mat4 getWorldToVoxelMatrix();

    TGT_API glm::mat4 getTextureToWorldMatrix();
    TGT_API glm::mat4 getWorldToTextureMatrix();
    
    TGT_API glm::mat4 getTextureToPhysicalMatrix();
    TGT_API glm::mat4 getPhysicalToTextureMatrix();


    /// derived data
    template <class T>
    T* getDerivedData();

    template<class T>
    T* hasDerivedData() const;

    template<class T>
    void addDerivedDataInternal(T* data);

    template<class T>
    void removeDerivedDataInternal();

    TGT_API void clearDerivedData();

    /// representation
    template <class T>
    T* getRepresentation();

    template<class T>
    T* hasRepresentation() const;

    template<class T>
    void addRepresentationInternal(T* data);

    template<class T>
    void removeRepresentationInternal();

    TGT_API void clearRepresentation();

    TGT_API void SetReady(bool flag = true);
    TGT_API bool IsReady();

  private:
    std::vector<VolumeRepresentation*> representations_;
    std::vector<VolumeDerivedData*> derivedData_;
    glm::vec3 spacing_;
    glm::vec3 offset_;
    glm::mat4 physical2world_;
    std::string origin_;

    float rescaleIntercept_;
    float rescaleSlope_;
    float windowCenter_;
    float windowWidth_;

    bool ready_;
  };

  class VolumePreview;
  template TGT_API VolumePreview* Volume::getDerivedData<VolumePreview>();

  class VolumeMinMax;
  template TGT_API VolumeMinMax* Volume::getDerivedData<VolumeMinMax>();

  class VolumeHistogramIntensity;
  template TGT_API VolumeHistogramIntensity* Volume::getDerivedData<VolumeHistogramIntensity>();

  class VolumeGL;
  template TGT_API VolumeGL* Volume::getRepresentation<VolumeGL>();

  class VolumeRAM;
  template TGT_API VolumeRAM* Volume::getRepresentation<VolumeRAM>();

  //------------------------------------------------------------------------------

  /*
  * Position volume centered at (0,0,0), max edge length = 1
  */
  TGT_API void oldVolumePosition(Volume* vh);

} // end namespace tgt
