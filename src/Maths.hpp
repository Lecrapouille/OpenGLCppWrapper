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

namespace glwrap
{
  namespace maths
  {
    //! \brief Allow to redefine neutral and/or absorbing element in algebra.
    template<class T> T one() { return T(1); }

    //! \brief Allow to redefine neutral and/or absorbing element in algebra.
    template<class T> T zero() { return T(0); }

    //! \brief PI number
    static double PI = 3.14159265358979323846;

    //! \brief
    static uint32_t maxUlps = 4U;

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wstrict-aliasing"
#   pragma GCC diagnostic ignored "-Wfloat-equal"
#   pragma GCC diagnostic ignored "-Wold-style-cast"
#   pragma GCC diagnostic ignored "-Wcast-qual"
#   pragma GCC diagnostic ignored "-Wsign-conversion"

    static bool almostEqual(float const A, float const B)
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

      int intDiff = std::abs(aInt - bInt);
      if (intDiff <= (int) maths::maxUlps)
        return true;
      return false;
    }

#   pragma GCC diagnostic pop

    static inline bool almostZero(float const A)
    {
      return almostEqual(A, zero<float>());
    }

    //! \brief Constrain value: std::min(std::max(a[i], lower), upper)
    template<typename T>
    static T clamp(T const value, T const lower, T const upper)
    {
      if (value < lower)
        return lower;

      if (value > upper)
        return upper;

      return value;
    }

    //! \brief Converts radians to degrees and returns the result.
    template<typename T>
    inline T toRadian(T const degrees)
    {
      double rad = degrees * 0.01745329251994329576923690768489;
      return T(rad);
    }

    //! \brief Converts degrees to radians and returns the result.
    template<typename T>
    inline T toDegree(T const radians)
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

    //! \brief Normalize the angle given in radians to [-PI +PI] radians.
    template<typename T>
    inline T wrapToPI(T const radians)
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
    inline T wrapTo2PI(T const radians)
    {
      T angle = radians;
      while (angle < T(0))
        angle += T(2.0 * PI);
      while (angle >= T(2.0 * PI))
        angle -= T(2.0 * PI);
      return angle;
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
    template <typename T>
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
