#include "modules/mod_sampler2d.frag"       // for entry & exit texture lookup
#include "modules/mod_sampler3d.frag"       // for value texture lookup
#include "modules/mod_raysetup.frag"        // for ray setup, terminate, etc.
#include "modules/mod_compositing.frag"     // for ray compositing
#include "modules/mod_transfunc.frag"       // for transfer function mapping
#include "modules/mod_gradient.frag"        // for gradient calculation
#include "modules/mod_shading.frag"         // for shading
#include "modules/mod_depth.frag"           // for depth

out vec4 FragData0;

uniform sampler2D entryPoints_;             // ray entry points
uniform sampler2D entryPointsDepth_;        // ray entry points depth
uniform TextureParameters entryParameters_; // ray entry texture parameters
uniform sampler2D exitPoints_;              // ray exit points
uniform sampler2D exitPointsDepth_;         // ray exit points depth
uniform TextureParameters exitParameters_;  // ray exit texture parameters

uniform sampler3D volume_;                  // volume texture
uniform VolumeParameters volumeStruct_;     // volume texture parameters

uniform TF_SAMPLER_TYPE transFuncTex_;        // transfunc texture
uniform TransFuncParameters transFuncStruct_; // transfunc texture parameters

uniform float samplingStepSize_;            // sampling step


vec4 rayTraversal(in vec3 first, in vec3 last, float entryDepth, float exitDepth) {

  vec4 result = vec4(0.0);
  float t = 0.0;
  float tIncr = 0.0;
  float tEnd = 1.0;
  float tDepth = -1.0;
  vec3 rayDirection;
  bool finished = false;
  float lastIntensity = 0.0f; //used for pre-integrated transfer-functions

  // calculate the required ray parameters
  raySetup(first, last, samplingStepSize_, rayDirection, tIncr, tEnd);

  WHILE(!finished) {
    vec3 samplePos = first + t * rayDirection;
    float intensity = textureLookup3DMapped(volume_, volumeStruct_, samplePos);

    // apply classification
    vec4 color = RC_APPLY_CLASSIFICATION(transFuncStruct_, transFuncTex_, intensity, lastIntensity);

    // if opacity greater zero, apply compositing
    if (color.a > 0.0) {
      // calculate gradient
      vec3 n = CALC_GRADIENT(volume_, volumeStruct_, samplePos);

      // apply shading
      color.rgb = APPLY_SHADING(n,
        texToPhysical(samplePos, volumeStruct_),
        volumeStruct_.cameraPositionPhysical_,  // lightPositionPhysical_
        volumeStruct_.cameraPositionPhysical_,
        color.rgb,
        color.rgb,
        vec3(1.0f));

      compositeDVR(result, color, t, samplingStepSize_, tDepth);
    }

    finished = earlyRayTermination(result.a, EARLY_RAY_TERMINATION_OPACITY);
    lastIntensity = intensity;

    t += tIncr;
    finished = finished || (t > tEnd);
  } END_WHILE


    gl_FragDepth = getDepthValue(tDepth, tEnd, entryDepth, exitDepth);;
  return result;
}

void main() {
  // fetch entry/exit points
  vec2 p = gl_FragCoord.xy * screenDimRCP_;
  vec3 frontPos = textureLookup2D(entryPoints_, entryParameters_, p).rgb;
  vec3 backPos = textureLookup2D(exitPoints_, exitParameters_, p).rgb;
  float entryDepth = textureLookup2D(entryPointsDepth_, entryParameters_, p).z;
  float exitDepth = textureLookup2D(exitPointsDepth_, exitParameters_, p).z;

  // determine whether the ray has to be casted
  if (frontPos == backPos)
    // background needs no raycasting
    discard;
  else {
    // fragCoords are lying inside the bounding box
    FragData0 = rayTraversal(frontPos, backPos, entryDepth, exitDepth);
  }
}