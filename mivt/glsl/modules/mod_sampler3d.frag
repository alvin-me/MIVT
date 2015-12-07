

/*
* This struct contains a volume and information about it, as
* its dimensions and spacing. Additionally, the reciprocal
* values of all parameters are available (suffix RCP) .
* The values are set automatically by
* VolumeRenderer::bindVolumes() if necessary.
*/
struct VolumeParameters {
  vec3 datasetDimensions_;        // the dataset's resolution, e.g. [ 256.0, 128.0, 128.0]
  vec3 datasetDimensionsRCP_;

  vec3 datasetSpacing_;           // set dataset's voxel size, e.g. [ 0.02, 0.05, 0.05]
  vec3 datasetSpacingRCP_;

  vec3 volumeCubeSize_;           // the volume's size in physical coordinates, e.g. [ 1.0, 0.5, 0.5]
  vec3 volumeCubeSizeRCP_;

  vec3 volumeOffset_;             // see VolumeHandle::getOffset()

  int bitDepth_;                  // the volume's bit depth

  float vmScale_;                // ValueMapping slope
  float vmOffset_;               // ValueMapping intercept

  mat4 physicalToWorldMatrix_;     // (see Volume)
  mat4 worldToPhysicalMatrix_;

  mat4 worldToTextureMatrix_;
  mat4 textureToWorldMatrix_;

  vec3 cameraPositionPhysical_;        // camera position in volume object coordinates (see mod_shading.frag)
  vec3 lightPositionPhysical_;         // light position in volume object coordinates (see mod_shading.frag)
};

vec4 textureLookup3D(sampler3D volume, VolumeParameters volumeStruct, vec3 texCoords) {
  return texture(volume, texCoords);
}

float textureLookup3DMapped(sampler3D volume, VolumeParameters volumeStruct, vec3 texCoords) {
  vec4 result;
  result = texture(volume, texCoords);
  result.r *= volumeStruct.vmScale_;
  result.r += volumeStruct.vmOffset_;
  return result.r;
}

vec3 texToPhysical(vec3 samplePos, VolumeParameters volumeParams) {
  return ((samplePos*volumeParams.datasetDimensions_)*volumeParams.datasetSpacing_) + volumeParams.volumeOffset_;
}

vec3 physicalToTex(vec3 samplePos, VolumeParameters volumeStruct) {
  return ((samplePos - volumeStruct.volumeOffset_) * volumeStruct.datasetSpacingRCP_) * volumeStruct.datasetDimensionsRCP_;
}

vec3 worldToTex(vec3 worldSamplePos, VolumeParameters volumeStruct) {
  return (volumeStruct.worldToTextureMatrix_* vec4(worldSamplePos, 1.0)).xyz;
}

vec3 texToWorld(vec3 texSamplePos, VolumeParameters volumeStruct) {
  return (volumeStruct.textureToWorldMatrix_* vec4(texSamplePos, 1.0)).xyz;
}