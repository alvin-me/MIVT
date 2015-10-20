uniform float const_to_z_w_1;
uniform float const_to_z_w_2;
uniform float const_to_z_e_1;
uniform float const_to_z_e_2;


/**
* Calculates the depth value for the current sample specified by the parameter t.
**/
float calculateDepthValue(float t, float entryPointsDepth, float exitPointsDepth) {
  /*
  Converting eye coordinate depth values to windows coordinate depth values:
  (see http://www.opengl.org/resources/faq/technical/depthbuffer.htm 12.050, assuming w_e = 1)

  z_w = (1.0/z_e)*((f*n)/(f-n)) + 0.5*((f+n)/(f-n))+0.5; (f=far plane, n=near plane)

  We calculate constant terms outside:
  const_to_z_w_1 = ((f*n)/(f-n))
  const_to_z_w_2 = 0.5*((f+n)/(f-n))+0.5

  Converting windows coordinates to eye coordinates:

  z_e = 1.0/([z_w - 0.5 - 0.5*((f+n)/(f-n))]*((f-n)/(f*n)));

  with constant terms
  const_to_z_e_1 = 0.5 + 0.5*((f+n)/(f-n))
  const_to_z_e_2 = ((f-n)/(f*n))
  */

  // assign front value given in windows coordinates
  float zw_front = entryPointsDepth;
  // and convert it into eye coordinates
  float ze_front = 1.0 / ((zw_front - const_to_z_e_1)*const_to_z_e_2);

  // assign back value given in windows coordinates
  float zw_back = exitPointsDepth;
  // and convert it into eye coordinates
  float ze_back = 1.0 / ((zw_back - const_to_z_e_1)*const_to_z_e_2);

  // interpolate in eye coordinates
  float ze_current = ze_front + t*(ze_back - ze_front);

  // convert back to window coordinates
  float zw_current = (1.0 / ze_current)*const_to_z_w_1 + const_to_z_w_2;

  return zw_current;
}

/**
* Returns the depth value for a given t by considering the ray
* length as well as the depth of the entry and exit points.
*/
float getDepthValue(float t, float tEnd, float entryPointsDepth, float exitPointsDepth) {
  if (t >= 0.0)
    return calculateDepthValue(t / tEnd, entryPointsDepth, exitPointsDepth);
  else
    return 1.0;
}