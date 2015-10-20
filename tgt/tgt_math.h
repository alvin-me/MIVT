#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS

#pragma warning(disable:4201)
#pragma warning(disable:4310)
#pragma warning(disable:4245)
#pragma warning(disable:4127)

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define DELARR(a) if(a) { delete []a; a = 0; }
#define DELPTR(p) if(p) { delete p; p = 0; }

namespace glm {

  const float PIf = 3.14159265358979323846f;

  typedef u8vec3 col3;
  typedef u8vec4 col4;

  inline float rad2deg(float f) {
    return (180.f / PIf) * f;
  }

  inline float deg2rad(float f) {
    return (PIf / 180.f) * f;
  }

  inline size_t hmul(ivec3 const & v) {
    return (size_t)v[0] * (size_t)v[1] * (size_t)v[2];
  }

  template <typename T, precision P>
  T hmax(detail::tvec3<T, P> const & v)
  {
    return glm::max(glm::max(v[0], v[1]), v[2]);
  }

  template <typename T, precision P>
  T hmax(detail::tvec2<T, P> const & v)
  {
    return glm::max(v[0], v[1]);
  }

  template <typename T, precision P>
  T hmin(detail::tvec3<T, P> const & v)
  {
    return glm::min(glm::min(v[0], v[1]), v[2]);
  }

  template <typename T, precision P>
  T hmin(detail::tvec2<T, P> const & v)
  {
    return glm::min(v[0], v[1]);
  }

  /**
  * Return the upper left 3x3-submatrix in a 4x4 matrix
  */
  template <typename T, precision P>
  detail::tmat4x4<T, P> getRotationalPart(detail::tmat4x4<T, P> const & m)
  {
    detail::tmat4x4<T, P> result(0);
    result[0][0] = m[0][0]; result[0][1] = m[0][1]; result[0][2] = m[0][2];
    result[1][0] = m[1][0]; result[1][1] = m[1][1]; result[1][2] = m[1][2];
    result[2][0] = m[2][0]; result[2][1] = m[2][1]; result[2][2] = m[2][2];
    result[3][3] = 1;
    return result;
  }

  /**
  * Maps point by multiplying this matrix by point.
  */
  template <typename T, precision P>
  detail::tvec3<T, P> mapPoint(detail::tmat4x4<T, P> const & m, detail::tvec3<T, P> const & p)
  {
    detail::tvec4<T, P> point(p, 1);
    return detail::tvec3<T, P>(m * point);
  }

  /**
  * Maps vector by multiplying the top 3x3 portion of this matrix by vector.
  * The translation and projection components of this matrix are ignored.
  */
  template <typename T, precision P>
  detail::tvec3<T, P> mapVector(detail::tmat4x4<T, P> const & mat, detail::tvec3<T, P> const & v)
  {
    detail::tvec4<T, P> vec(v, 0);
    return detail::tvec3<T, P>(mat * vec);
  }

  /**
  * Given a value and an input range, map the value to an output range.
  *
  * @param value The number to be mapped.
  * @param inputMin The lower bound of the input range.
  * @param inputMax The upper bound of the input range.
  * @param outputMin The lower bound of the output range.
  * @param outputMax The upper bound of the output range.
  * @param clamp True if the value should be clamped to [outputMin, outputMax).
  * @return a mapped floating point number.
  */
  inline float mapValue(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false)
  {
    assert(inputMin < inputMax);
    assert(outputMin < outputMax);

    if (std::fabs(inputMin - inputMax) < FLT_EPSILON) {
      return outputMin;
    }
    else {
      float outValue = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
      if (clamp) {
        outValue = glm::clamp(outValue, outputMin, outputMax);
      }
      return outValue;
    }
  }

} // end namespace glm