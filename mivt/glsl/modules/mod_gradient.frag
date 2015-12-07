/**
* This module contains all functions which can be used for calculating
* gradients on the fly within a raycaster.
* The functions below are referenced by CALC_GRADIENT which is
* used in the raycaster fragment shaders.
*/


/**
* Calculates a voxel's gradient in volume object space based on the alpha
* channel using central differences.
*
* @param volume the voxel's volume
* @param volumeStruct additional information about the passed volume
* @param samplePos the sample's position in texture space
*/
vec3 calcGradient(sampler3D volume, VolumeParameters volumeStruct, vec3 samplePos) {
  vec3 offset = volumeStruct.datasetDimensionsRCP_;
  float v0 = textureLookup3D(volume, volumeStruct, samplePos + vec3(offset.x, 0.0, 0.0)).r;
  float v1 = textureLookup3D(volume, volumeStruct, samplePos + vec3(0, offset.y, 0)).r;
  float v2 = textureLookup3D(volume, volumeStruct, samplePos + vec3(0, 0, offset.z)).r;
  float v3 = textureLookup3D(volume, volumeStruct, samplePos + vec3(-offset.x, 0, 0)).r;
  float v4 = textureLookup3D(volume, volumeStruct, samplePos + vec3(0, -offset.y, 0)).r;
  float v5 = textureLookup3D(volume, volumeStruct, samplePos + vec3(0, 0, -offset.z)).r;
  vec3 gradient = vec3(v3 - v0, v4 - v1, v5 - v2);
  gradient *= volumeStruct.datasetSpacingRCP_ * 0.5;  // divide by 2*spacing, the length between front and back points.
  gradient *= volumeStruct.vmScale_;                  // rescale to HU
  return gradient;
}

