//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef MATHS_HPP_
#  define MATHS_HPP_

#  include "NonCppStd.hpp"
#  include <cmath>
#  include <limits>
#  include <cassert>
#  include <iostream>
#  include <cstdint>

// TODO ajouter un fast_cos

namespace maths
{
  //! \brief
  extern uint32_t maxUlps;
  extern bool fastSqrt;

  template<typename T>
  inline T sqrt(T const val)
  {
    return std::sqrt(val);
  }

  //! \brief Linear interpolation. T = int/float, U = float
  template<typename T, typename U>
  inline T lerp(T const from, T const to, float const f)
  {
    return static_cast<T>(U(from) + (U(to - from) * f));
  }

  //! \brief Absolute value
  template<typename T>
  inline T abs(T const val)
  {
    if (val < 0) return -val;
    return val;
  }

  template<typename T>
  const T& min(const T& a, const T& b)
  {
    return (b < a) ? b : a;
  }

  template<typename T>
  const T& max(const T& a, const T& b)
  {
    return (a < b) ? b : a;
  }

  //! \brief Converts radians to degrees and returns the result.
  template<typename T>
  inline T radians(T const degrees)
  {
    double rad = degrees * 0.01745329251994329576923690768489;
    return T(rad);
  }

  //! \brief Converts degrees to radians and returns the result.
  template<typename T>
  inline T degrees(T const radians)
  {
    double deg = radians * 57.295779513082320876798154814105;
    return T(deg);
  }

  //! \brief Normalize the angle given in degrees to [-180 +180] degrees.
  template<typename T>
  inline T wrapTo180(T const degrees)
  {
    T angle = degrees;
    while (angle <= -T(180))
      angle += T(360);
    while (angle > T(180))
      angle -= T(360);
    return angle;
  }

  //! \brief Normalize the angle given in degrees to [0 +360] degrees.
  template<typename T>
  inline T wrapTo360(T const degrees)
  {
    T angle = degrees;
    while (angle < T(0))
      angle += T(360);
    while (angle >= T(360))
      angle -= T(360);
    return angle;
  }

  // Fast inversed square root. Code from Doom/Quake code source
  float fastInvSqrt(const float number);

  //! \brief Constrain value: std::min(std::max(a[i], lower), upper)
  template<typename T>
  inline T clamp(T const value, T const lower, T const upper)
  {
    if (value < lower)
      return lower;

    if (value > upper)
      return upper;

    return value;
  }

  /*
  template<typename T>
  typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  almostEqual(T const x, T const y)
  {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * maths::maxUlps
    // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
  }
  */
  // http://bitbashing.io/comparing-floats.html
  // https://www.working-software.com/cpp-float-comparisons
  // http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm#_Toc135149455
  // Usable AlmostEqual function
  //template<>

  bool almostEqual(float const A, float const B);
  bool almostZero(float const A);

}

#endif
