#ifndef MATHS_HPP
#define MATHS_HPP

namespace maths
{

//! \brief Normalize the angle given in degrees to [-180 +180] degrees.
template<typename T>
static inline T wrapTo180(T const degrees)
{
  T angle = degrees;
  while (angle <= -T(180))
    angle += T(360);
  while (angle > T(180))
      angle -= T(360);
  return angle;
}

//! \brief Converts radians to degrees and returns the result.
template<typename T>
static inline T radians(T const degrees)
{
  double rad = degrees * 0.01745329251994329576923690768489;
  return T(rad);
}

//! \brief Converts degrees to radians and returns the result.
template<typename T>
static inline T degrees(T const radians)
{
  double deg = radians * 57.295779513082320876798154814105;
  return T(deg);
}

}

#endif
