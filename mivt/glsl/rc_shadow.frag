#include "modules/mod_sampler2d.frag"       // for entry & exit texture lookup
#include "modules/mod_sampler3d.frag"       // for value texture lookup
#include "modules/mod_raysetup.frag"        // for ray setup, terminate, etc.
#include "modules/mod_compositing.frag"     // for ray compositing

uniform sampler2D entryPoints_;             // ray entry points
uniform TextureParameters entryParameters_; // ray entry texture parameters
uniform sampler2D exitPoints_;              // ray exit points
uniform TextureParameters exitParameters_;  // ray exit texture parameters

vec4 rayTraversal(in vec3 first, in vec3 last) {

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

    

    t += tIncr;
    finished = finished || (t > tEnd);
  } END_WHILE

  return result;
}

void main() {
  vec2 p = gl_FragCoord.xy * screenDimRCP_;
  vec3 frontPos = textureLookup2D(entryPoints_, entryParameters_, p).rgb;
  vec3 backPos = textureLookup2D(exitPoints_, exitParameters_, p).rgb;

  if (frontPos == backPos)
    discard;
  else
    FragData0 = rayTraversal(frontPos, backPos);
}