/**
* This module contains all functions which can be used for performing
* classifications of a voxel within a raycaster.
* The functions below are referenced by APPLY_CLASSIFICATION which
* is used in the raycaster fragment shaders.
*/

struct TransFuncParameters {
  float windowingLowerHu_;
  float windowingUpperHu_;
};


/// normalize HU value with windowing
float realWorldToTexture(TransFuncParameters tf, float hu) {
  // clamp auto by with GL_TEXTURE_BORDER_COLOR setting.
  return (hu - tf.windowingLowerHu_) / (tf.windowingUpperHu_ - tf.windowingLowerHu_);
}

vec4 applyTF(TransFuncParameters transfunc, sampler1D tex, float intensity) {
  intensity = realWorldToTexture(transfunc, intensity);
  return texture(tex, intensity);
}

//pre-integrated TF:
vec4 applyTFpi(TransFuncParameters transfunc, sampler2D tex, float intensity, float prev) {
  intensity = realWorldToTexture(transfunc, intensity);
  prev = realWorldToTexture(transfunc, prev);
  return texture(tex, vec2(intensity, prev));
}