/**
* This module contains all functions which can be used for compositing
* voxels within a raycaster.
* The functions are referenced by APPLY_COMPOSITING as used in the
* raycaster fragment shaders.
*/

/**
* Reciprocal value of the reference sampling interval used
* for the opacity correction that is necessary to compensate
* variable sampling rates.
*
* See Engel et. al.: "Real-Time Volume Graphics" - Ch 9.1.3
*/
const float SAMPLING_BASE_INTERVAL_RCP = 200.0;


/**
* Performs regular DVR compositing. Expects the current voxels color
* and the intermediate result. Returns the result after compositing.
*/
void compositeDVR(inout vec4 curResult, 
  in vec4 color, 
  in float t, 
  in float samplingStepSize, 
  inout float tDepth) 
{
  vec4 result = curResult;

  // apply opacity correction to accomodate for variable sampling intervals
  color.a = 1.0 - pow(1.0 - color.a, samplingStepSize * SAMPLING_BASE_INTERVAL_RCP);

  result.rgb = result.rgb + (1.0 - result.a) * color.a * color.rgb;
  result.a = result.a + (1.0 - result.a) * color.a;
  // save first hit ray parameter for depth value calculation
  if (tDepth < 0.0)
    tDepth = t;
  curResult = result;
}


/**
* Performs MIP (maximum intensity projection) compositing. Expects the current
* voxels color and the intermediate result. Returns the result after compositing.
*/
void compositeMIP(inout vec4 curResult, 
  in vec4 color, 
  in float t, 
  inout float tDepth) 
{
  vec4 result;
  if (color.a > curResult.a) {
    result = color;
    // save ray parameter for depth value calculation
    tDepth = t;
  }
  else result = curResult;
  curResult = result;
}

///**
//* Performs MIP compositing.
//*/
//void compositeMIP(float intensity,
//  TransFuncParameters transfuncStruct,
//  inout vec4 curResult,
//  in float t, 
//  inout float tDepth)
//{
//  float value = realWorldToTexture(transfuncStruct, intensity);
//  vec4 color = vec4(value);
//
//  if (value > 0.0) {
//    compositeMIP_internal(curResult, color, t, tDepth);
//  }
//}