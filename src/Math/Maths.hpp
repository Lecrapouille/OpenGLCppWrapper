//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_MATHS_HPP
#  define OPENGLCPPWRAPPER_MATHS_HPP

#  include <cstdint>
#  include <vector>
#  include <cassert>
#  include <cmath>
#  include <limits>

namespace glwrap
{
  namespace maths
  {
    //! \brief Allow to redefine neutral and/or absorbing element in algebra.
    template<typename T> T one() { return T(1); }

    //! \brief Allow to redefine neutral and/or absorbing element in algebra.
    template<typename T> T zero() { return T(0); }

    //! \brief PI number
    static double const PI = 3.14159265358979323846;
    static double const TWO_PI = 2.0 * PI;

    //! \brief
    static uint32_t maxUlps = 4U;

    template<typename T>
    static inline T abs(T const x)
    {
      return (x >= zero<T>()) ? x : -x;
    }

    // TODO https://gist.github.com/MikimotoH/282dca62e08b90b9b673
    static inline float sqrt(int const x)
    {
      return ::sqrtf(static_cast<float>(x));
    }

    static inline float sqrt(size_t const x)
    {
      return ::sqrtf(static_cast<float>(x));
    }

    static inline float sqrt(float const x)
    {
      return ::sqrtf(x);
    }

    static inline long double sqrt(long double const x)
    {
      return ::sqrtl(x);
    }

    static inline double sqrt(double const x)
    {
      return ::sqrt(x);
    }

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wstrict-aliasing"
#   pragma GCC diagnostic ignored "-Wfloat-equal"
#   pragma GCC diagnostic ignored "-Wold-style-cast"
#   pragma GCC diagnostic ignored "-Wcast-qual"
#   pragma GCC diagnostic ignored "-Wsign-conversion"

    static inline bool almostEqual(float const A, float const B)
    {
      if (A == B)
        return true;

      // Make sure maxUlps is non-negative and small enough that the
      // default NAN won't compare as equal to anything.
      assert(maths::maxUlps < 4U * 1024U * 1024U);

      int aInt = *(int*) &A;

      // Make aInt lexicographically ordered as a twos-complement int
      if (aInt < 0)
        aInt = 0x80000000 - aInt;

      // Make bInt lexicographically ordered as a twos-complement int
      int bInt = *(int*) &B;

      if (bInt < 0)
        bInt = 0x80000000 - bInt;

      int intDiff = maths::abs(aInt - bInt);
      if (intDiff <= (int) maths::maxUlps)
        return true;
      return false;
    }

#   pragma GCC diagnostic pop

    static inline bool almostZero(float const A)
    {
      return almostEqual(A, zero<float>());
    }

    template<typename T, class =
             typename std::enable_if<std::is_integral<T>::value>::type>
    bool inline isPowerOfTwo(T const value)
    {
      return ((value & (value - one<T>())) == zero<T>()) &&
        (value != zero<T>());
    }

    //! \brief Constrain value: std::min(std::max(x, lower), upper)
    template<typename T>
    static inline T clamp(T const x, T const lower, T const upper)
    {
      if (x < lower)
        return lower;

      if (x > upper)
        return upper;

      return x;
    }

    //! \brief Return the sign of the number: -1 or 0 or +1.
    template<typename T>
    static inline int sign(T const val)
    {
      return (zero<T>() < val) - (val < zero<T>);
    }

    //! \brief Converts radians to degrees and returns the result.
    template<typename T>
    static inline T toRadian(T const degrees)
    {
      return static_cast<T>(degrees * 0.01745329251994329576923690768489);
    }

    //! \brief Converts degrees to radians and returns the result.
    template<typename T>
    static inline T toDegree(T const radians)
    {
      return static_cast<T>(radians * 57.295779513082320876798154814105);
    }

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

    //! \brief Normalize the angle given in degrees to [0 +360] degrees.
    template<typename T>
    static inline T wrapTo360(T const degrees)
    {
      T angle = degrees;
      while (angle < T(0))
        angle += T(360);
      while (angle >= T(360))
        angle -= T(360);
      return angle;
    }

    //! \brief Normalize the angle given in radians to [-PI +PI] radians.
    template<typename T>
    static inline T wrapToPI(T const radians)
    {
      T angle = radians;
      while (angle <= -T(PI))
        angle += T(2.0 * PI);
      while (angle > T(PI))
        angle -= T(2.0 * PI);
      return angle;
    }

    //! \brief Normalize the angle given in radians to [0 2*PI] radians.
    template<typename T>
    static inline T wrapTo2PI(T const radians)
    {
      T angle = radians;
      while (angle < T(0))
        angle += T(2.0 * PI);
      while (angle >= T(2.0 * PI))
        angle -= T(2.0 * PI);
      return angle;
    }

    //! \brief Linear mapping of x from range [start1 stop1] to range [start2 stop2].
    template<typename T>
    static inline T lmap(T const x, T const start1, T const stop1, T const start2, T const stop2)
    {
      T mapped = start2 + (stop2 - start2) * ((x - start1) / (stop1 - start1));
      return mapped;
    }

    //! \brief Linear interpolation between a and b for the parameter t with t
    //! inside the range [0,1].
    template<typename T>
    static inline T lerp(T const a, T const b, T const t)
    {
      static_assert((t >= zero<T>()) && (t <= one<T>()), "param t shall be [0 1]");
      return (one<T>() - t) * a + t * b;
    }

    // *****************************************************************************
    //! \brief Return evenly spaced numbers over a specified interval.
    //! \param[in] start The starting scalar value of the sequence.
    //! \param[in] end The end value of the sequence, unless endpoint is set to false.
    //!    In that case, the sequence consists of all but the last of num + 1
    //!    evenly spaced samples, so that stop is excluded.
    //! \param[in] N Number of samples to generate. Should be > 1.
    //! \param[out] The vector of equally spaced samples in the closed interval [start, stop]
    //!   or the half-open interval [start, stop) (depending on whether endpoint is true or false).
    //! \return Size of spacing between samples. Return NaN if this value cannot be
    //! computed.
    //! \note: This code has been inpsired by the Numpy.linspace function.
    // *****************************************************************************
    template<typename T>
    static T linspace(T const start, T const end, size_t const N, std::vector<T>& result, const bool endpoint)
    {
      const T not_a_number = std::numeric_limits<T>::quiet_NaN();
      result.clear();

      if (0 == N) return not_a_number;
      if (1 == N)
        {
          result.push_back(start);
          return not_a_number;
        }

      result.reserve(N);
      T delta = (end - start) / static_cast<T>(N-1);
      for (size_t i = 0; i < N-1; ++i)
        {
          result.push_back(start + delta * T(i));
        }

      if (endpoint)
        {
          result.push_back(end);
        }
      return delta;
    }
  } // namespace maths
} // namespace glwrap

#endif
