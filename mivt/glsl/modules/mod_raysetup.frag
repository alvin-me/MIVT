/**
* This module contains all functions which are used for the raysetup
* as well as the ray traversal within a raycaster.
*/

/**
* This parameter defines the minimum opacity saturation
* a ray has to reach before it is terminated.
* Setting this value to 1.0 disables early ray termination.
*/
const float EARLY_RAY_TERMINATION_OPACITY = 0.95;

/***
* Calculates the direction of the ray and returns the number
* of steps and the direction.
***/
void raySetup(in vec3 first, in vec3 last, in float samplingStepSize, out vec3 rayDirection, out float tIncr, out float tEnd) {
  // calculate ray parameters
  tIncr = samplingStepSize;

  rayDirection = last - first;
  tEnd = length(rayDirection);
  rayDirection = normalize(rayDirection);
}

/***
* Applies early ray termination. The current opacity is compared to
* the maximum opacity. In case it is greater, the opacity is set to
* 1.0 and true is returned, otherwise false is returned.
***/
bool earlyRayTermination(inout float opacity, in float maxOpacity) {
  if (opacity >= maxOpacity) {
    opacity = 1.0;
    return true;
  }
  else {
    return false;
  }
}


// Nvidia seems to support a loop count > 256 only for
// for-loops and not for while-loops on Geforce 8800 GTX.
// On GTX280 a for-loop is still slightly faster than a while-loop. joerg
// Reduced loop count to 255*255 due to NVIDIA hang-up bug with driver version > 275.33 (jsp)
#define RAYCASTING_LOOP_COUNT 255*255
#define WHILE(keepGoing) for (int loop=0; keepGoing && loop<RAYCASTING_LOOP_COUNT; loop++) {
#define END_WHILE }