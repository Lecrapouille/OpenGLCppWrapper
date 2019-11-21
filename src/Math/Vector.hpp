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

#ifndef OPENGLCPPWRAPPER_VECTOR_HPP
#  define OPENGLCPPWRAPPER_VECTOR_HPP

// *************************************************************************************************
// Inspired from https://github.com/Reedbeta/reed-util
// and http://www.reedbeta.com/blog/on-vector-math-libraries/
// and http://www.ogre3d.org
// *************************************************************************************************

#  include <initializer_list>
#  include <algorithm>
#  include "Common/NonCppStd.hpp"
#  include <cstdint>
#  include <cassert>
#  include <cmath>
#  include <fstream>
#  include "Math/Maths.hpp"

namespace glwrap
{

// *************************************************************************************************
//! \brief Macro for building constructors
// *************************************************************************************************
#define VECTOR_DIM(N)                                                   \
  /*! \brief Empty constructor */                                       \
  Vector()                                                              \
  {                                                                     \
    static_assert(N >= 2_z, "Minimun dimension for a vector is 2");     \
  }                                                                     \
                                                                        \
  /*! \brief Constructor with initialization list */                    \
  Vector(std::initializer_list<T> initList)                             \
  {                                                                     \
    const size_t m = std::min(static_cast<size_t>(N), initList.size());  /* FIXME cast */                    \
    auto iter = initList.begin();                                       \
    for (size_t i = 0_z; i < m; ++i)                                    \
      {                                                                 \
        m_data[i] = *iter;                                              \
        ++iter;                                                         \
      }                                                                 \
                                                                        \
    /* Zero-fill any remaining elements */                              \
    for (size_t i = m; i < N; ++i)                                      \
      {                                                                 \
        m_data[i] = maths::zero<T>();                                   \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Constructor with uniform value */                          \
  explicit Vector(T const scalar)                                       \
  {                                                                     \
    size_t i = N;                                                       \
    while (i--)                                                         \
      {                                                                 \
        m_data[i] = scalar;                                             \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Constructor by copy */                                     \
  template <typename U, size_t nOther>                                  \
  explicit Vector(Vector<U, nOther> const &other)                       \
  {                                                                     \
    const size_t m = std::min(static_cast<size_t>(N), nOther);  /* FIXME cast */              \
    size_t i = m;                                                       \
    while (i--)                                                         \
      {                                                                 \
        m_data[i] = T(other[i]);                                        \
      }                                                                 \
                                                                        \
    /* Zero-fill any remaining elements */                              \
    for (i = m; i < N; ++i)                                             \
      {                                                                 \
        m_data[i] = maths::zero<T>();                                   \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Return the dimension */                                    \
  size_t size() const { return N; }                                     \
                                                                        \
  /* Accessors */                                                       \
  T& operator[](size_t const i)          { return m_data[i]; }          \
  const T& operator[](size_t const i) const { return m_data[i]; }       \
  T& operator[](int const i)             { return m_data[i]; }          \
  const T& operator[](int const i) const { return m_data[i]; }          \
                                                                        \
  /* C array conversions */                                             \
  typedef T(&array_t)[N];                                               \
  typedef const T(&const_array_t)[N];                                   \
  operator array_t ()                   { return m_data; }              \
  operator const_array_t () const       { return m_data; }              \
                                                                        \
  private:                                                              \
                                                                        \
  /* Disallow bool conversions (without this, they'd happen implicitly*/\
  /* via the array conversions) */                                      \
  operator bool();

// *************************************************************************************************
//! \brief Generic mathematic vector: T for the type (float, int) and n the vector dimension
// *************************************************************************************************
template <typename T, size_t n>
class Vector
{
public:

  VECTOR_DIM(n)

protected:

  T m_data[n];
};

// *************************************************************************************************
//! \brief Specialization for n = 2
// *************************************************************************************************
template <typename T>
class Vector<T, 2_z>
{
public:

  Vector(const T scalar_x, const T scalar_y)
  {
    x = scalar_x;
    y = scalar_y;
  }

  VECTOR_DIM(2_z)

public:

  union
  {
    T m_data[2_z];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    struct { T x; T y; };
    struct { T u; T v; };
#pragma GCC diagnostic pop
  };

  const static Vector<T, 2_z> DUMMY;
  const static Vector<T, 2_z> ZERO;
  const static Vector<T, 2_z> UNIT_SCALE;
  const static Vector<T, 2_z> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 2_z> UNIT_X;
  const static Vector<T, 2_z> UNIT_Y;
  const static Vector<T, 2_z> NEGATIVE_UNIT_X;
  const static Vector<T, 2_z> NEGATIVE_UNIT_Y;
};

// Predifined vectors
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::DUMMY(T(NAN));
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::ZERO(maths::zero<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::UNIT_SCALE(maths::one<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::NEGATIVE_UNIT_SCALE(-maths::one<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::UNIT_X(maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::UNIT_Y(maths::zero<T>(), maths::one<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::NEGATIVE_UNIT_X(-maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 2_z> Vector<T, 2_z>::NEGATIVE_UNIT_Y(maths::zero<T>(), -maths::one<T>());

// *************************************************************************************************
//! \brief Specialization for n = 3
// *************************************************************************************************
template <typename T>
class Vector<T, 3_z>
{
public:

  Vector(Vector<T, 2_z> const &v, const T scalar_z = maths::zero<T>())
  {
    x = v.x;
    y = v.y;
    z = scalar_z;
  }

  Vector(const T scalar_x, const T scalar_y, const T scalar_z = maths::zero<T>())
  {
    x = scalar_x;
    y = scalar_y;
    z = scalar_z;
  }

  VECTOR_DIM(3_z)

public:

  union
  {
    T m_data[3_z];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    struct { T x; T y; T z; };
    struct { T r; T g; T b; };
#pragma GCC diagnostic pop
  };

  const static Vector<T, 3_z> DUMMY;
  const static Vector<T, 3_z> ZERO;
  const static Vector<T, 3_z> UNIT_SCALE;
  const static Vector<T, 3_z> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 3_z> UNIT_X;
  const static Vector<T, 3_z> UNIT_Y;
  const static Vector<T, 3_z> UNIT_Z;
  const static Vector<T, 3_z> NEGATIVE_UNIT_X;
  const static Vector<T, 3_z> NEGATIVE_UNIT_Y;
  const static Vector<T, 3_z> NEGATIVE_UNIT_Z;
};

template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::DUMMY(T(NAN));
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::ZERO(maths::zero<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::UNIT_SCALE(maths::one<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::NEGATIVE_UNIT_SCALE(-maths::one<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::UNIT_X(maths::one<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::UNIT_Y(maths::zero<T>(), maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::UNIT_Z(maths::zero<T>(), maths::zero<T>(), maths::one<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::NEGATIVE_UNIT_X(-maths::one<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::NEGATIVE_UNIT_Y(maths::zero<T>(), -maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 3_z> Vector<T, 3_z>::NEGATIVE_UNIT_Z(maths::zero<T>(), maths::zero<T>(), -maths::one<T>());

// *************************************************************************************************
// Specializations for n = 4
// *************************************************************************************************
template <typename T>
class Vector<T, 4_z>
{
public:

  Vector(Vector<T, 3_z> const &v, const T scalar_w = maths::zero<T>())
  {
    x = v.x;
    y = v.y;
    z = v.z;
    w = scalar_w;
  }

  Vector(const T scalar_x, const T scalar_y, const T scalar_z, const T scalar_w)
  {
    x = scalar_x;
    y = scalar_y;
    z = scalar_z;
    w = scalar_w;
  }

  VECTOR_DIM(4_z)

public:

  union
  {
    T m_data[4_z];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    struct { T x; T y; T z; T w; };
    struct { T r; T g; T b; T a; };
#pragma GCC diagnostic pop
  };

  const static Vector<T, 4_z> DUMMY;
  const static Vector<T, 4_z> ZERO;
  const static Vector<T, 4_z> UNIT_SCALE;
  const static Vector<T, 4_z> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 4_z> UNIT_X;
  const static Vector<T, 4_z> UNIT_Y;
  const static Vector<T, 4_z> UNIT_Z;
  const static Vector<T, 4_z> UNIT_W;
  const static Vector<T, 4_z> NEGATIVE_UNIT_X;
  const static Vector<T, 4_z> NEGATIVE_UNIT_Y;
  const static Vector<T, 4_z> NEGATIVE_UNIT_Z;
  const static Vector<T, 4_z> NEGATIVE_UNIT_W;
};

template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::DUMMY(T(NAN));
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::ZERO(maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::UNIT_SCALE(maths::one<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::NEGATIVE_UNIT_SCALE(-maths::one<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::UNIT_X(maths::one<T>(), maths::zero<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::UNIT_Y(maths::zero<T>(), maths::one<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::UNIT_Z(maths::zero<T>(), maths::zero<T>(), maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::UNIT_W(maths::zero<T>(), maths::zero<T>(), maths::zero<T>(), maths::one<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::NEGATIVE_UNIT_X(-maths::one<T>(), maths::zero<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::NEGATIVE_UNIT_Y(maths::zero<T>(), -maths::one<T>(), maths::zero<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::NEGATIVE_UNIT_Z(maths::zero<T>(), maths::zero<T>(), -maths::one<T>(), maths::zero<T>());
template <typename T> const Vector<T, 4_z> Vector<T, 4_z>::NEGATIVE_UNIT_W(maths::zero<T>(), maths::zero<T>(), maths::zero<T>(), -maths::one<T>());

// *************************************************************************************************
// Overloaded math operators
// *************************************************************************************************

#define DEFINE_UNARY_OPERATOR(op)                       \
  template <typename T, size_t n>                       \
  Vector<T, n> operator op (Vector<T, n> const &a)      \
  {                                                     \
    Vector<T, n> result;                                \
    size_t i = n;                                       \
    while (i--)                                         \
      result[i] = op a[i];                              \
    return result;                                      \
  }

#define DEFINE_BINARY_OPERATORS(op)                                     \
  /* Vector-Vector op */                                                \
  template <typename T, size_t n>                                       \
  Vector<T, n> operator op (Vector<T, n> const &a, Vector<T, n> const &b) \
  {                                                                     \
    Vector<T, n> result;                                                \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a[i] op b[i];                                         \
    return result;                                                      \
  }                                                                     \
  /* Scalar-Vector op */                                                \
  template <typename T, size_t n>                                       \
  Vector<T, n> operator op (T const a, Vector<T, n> const &b)           \
  {                                                                     \
    Vector<T, n> result;                                                \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a op b[i];                                            \
    return result;                                                      \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, size_t n>                                       \
  Vector<T, n> operator op (Vector<T, n> const &a, T const b)           \
  {                                                                     \
    Vector<T, n> result;                                                \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a[i] op b;                                            \
    return result;                                                      \
  }

#define DEFINE_INPLACE_OPERATORS(op)                                    \
  /* Vector-Vector op */                                                \
  template <typename T, size_t n>                                       \
  Vector<T, n>& operator op (Vector<T, n> &a, Vector<T, n> const &b)    \
  {                                                                     \
    size_t i = n;                                                       \
    while (i--)                                                         \
      a[i] op b[i];                                                     \
    return a;                                                           \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, size_t n>                                       \
  Vector<T, n>& operator op (Vector<T, n> &a, T const b)                \
  {                                                                     \
    size_t i = n;                                                       \
    while (i--)                                                         \
      a[i] op b;                                                        \
    return a;                                                           \
  }

#define DEFINE_RELATIONAL_OPERATORS(op)                                 \
  /* Vector-Vector op */                                                \
  template <typename T, typename U, size_t n>                           \
  Vector<bool, n> operator op (Vector<T, n> const &a, Vector<U, n> const &b) \
  {                                                                     \
    Vector<bool, n> result;                                             \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a[i] op b[i];                                         \
    return result;                                                      \
  }                                                                     \
  /* Scalar-Vector op */                                                \
  template <typename T, typename U, size_t n>                           \
  Vector<bool, n> operator op (T const &a, Vector<U, n> const &b)       \
  {                                                                     \
    Vector<bool, n> result;                                             \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a op b[i];                                            \
    return result;                                                      \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, typename U, size_t n>                           \
  Vector<bool, n> operator op (Vector<T, n> const &a, U const &b)       \
  {                                                                     \
    Vector<bool, n> result;                                             \
    size_t i = n;                                                       \
    while (i--)                                                         \
      result[i] = a[i] op b;                                            \
    return result;                                                      \
  }

#define DEFINE_FUN1_OPERATOR(name, op)                          \
  template <typename T, size_t n>                               \
  Vector<T, n> name(Vector<T, n> const &a)                      \
  {                                                             \
    Vector<T, n> result;                                        \
    size_t i = n;                                               \
                                                                \
    while (i--)                                                 \
      result[i] = op(a[i]);                                     \
    return result;                                              \
  }

#define DEFINE_FUN2_OPERATOR(name, op)                          \
  template <typename T, size_t n>                               \
  Vector<T, n> name(Vector<T, n> const &a, Vector<T, n> const &b) \
  {                                                             \
    Vector<T, n> result;                                        \
    size_t i = n;                                               \
                                                                \
    while (i--)                                                 \
      result[i] = op(a[i], b[i]);                               \
    return result;                                              \
  }

#define DEFINE_BOOL_OPERATOR(name, op)                          \
  template <typename T, size_t n>                               \
  bool name(Vector<T, n> const &a, Vector<T, n> const &b)       \
  {                                                             \
    size_t i = n;                                               \
                                                                \
    while (i--)                                                 \
      {                                                         \
        if (!(a[i] op b[i]))                                    \
          return false;                                         \
      }                                                         \
    return true;                                                \
  }

DEFINE_BINARY_OPERATORS(+)
DEFINE_BINARY_OPERATORS(-)
DEFINE_UNARY_OPERATOR(-)
DEFINE_UNARY_OPERATOR(+)
DEFINE_BINARY_OPERATORS(*)
DEFINE_BINARY_OPERATORS(/)
DEFINE_BINARY_OPERATORS(&)
DEFINE_BINARY_OPERATORS(|)
DEFINE_BINARY_OPERATORS(^)
DEFINE_UNARY_OPERATOR(!)
DEFINE_UNARY_OPERATOR(~)

DEFINE_INPLACE_OPERATORS(+=)
DEFINE_INPLACE_OPERATORS(-=)
DEFINE_INPLACE_OPERATORS(*=)
DEFINE_INPLACE_OPERATORS(/=)
DEFINE_INPLACE_OPERATORS(&=)
DEFINE_INPLACE_OPERATORS(|=)
DEFINE_INPLACE_OPERATORS(^=)

DEFINE_RELATIONAL_OPERATORS(==)
DEFINE_RELATIONAL_OPERATORS(!=)
DEFINE_RELATIONAL_OPERATORS(<)
DEFINE_RELATIONAL_OPERATORS(>)
DEFINE_RELATIONAL_OPERATORS(<=)
DEFINE_RELATIONAL_OPERATORS(>=)

namespace vector
{
  //! \brief Return a matrix where each e
  DEFINE_FUN2_OPERATOR(min, std::min)
  DEFINE_FUN2_OPERATOR(max, std::max)
  DEFINE_FUN1_OPERATOR(abs, maths::abs)
  DEFINE_BOOL_OPERATOR(ge, >=)
  DEFINE_BOOL_OPERATOR(gt, >)
  DEFINE_BOOL_OPERATOR(le, <=)
  DEFINE_BOOL_OPERATOR(lt, <)

  //! \brief Compare the integer values for each elements of vectors.
  //! \note Do not confuse this function with operator==() or the
  //! function equivalent() which do not have the same behavior.
  //! \return true if all elements have the same value.
  template <typename T, size_t n>
  typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
  eq(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    if (&a != &b)
      {
        size_t i = n;
        while (i--)
          {
            if (a[i] != b[i])
              return false;
          }
      }
    return true;
  }

  //! \brief Compare the float values for each elements of vectors.
  //! \note Do not confuse this function with operator==() or the
  //! function equivalent() which do not have the same behavior.
  //! \return true if all elements have the same value.
  template <typename T, size_t n>
  typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  eq(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    if (&a != &b)
      {
        size_t i = n;
        while (i--)
          {
            if (!maths::almostEqual(a[i], b[i]))
              return false;
          }
      }
    return true;
  }

  template <typename T, size_t n>
  bool ne(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return !vector::eq(a, b);
  }

  template <typename T, size_t n>
  void swap(Vector<T, n> &a, Vector<T, n> &b)
  {
    if (&a != &b)
      {
        size_t i = n;
        while (i--)
          {
            std::swap(a[i], b[i]);
          }
      }
  }

  //! \brief Get the coeficient of collinearity (k) of two vectors (u and v).
  //!
  //! Two non-null vectors are collinear if and only it exists a
  //! scalar k != 0, where: u = k v (note: if u is a null vector, any
  //! vector v is collinear to u because k = 0).
  //!
  //! \return Nan if vectors are not collinear. Return 0 if Return k if they are collinear.
  //!
  //! \note Use this function for T a familly of float and not for integers.
  template <typename T, size_t n>
  T collinearity(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    // Null vector ?
    if (maths::almostZero(u[0]) || maths::almostZero(v[0]))
      return maths::zero<T>();

    const T k = u[0] / v[0];
    for (size_t i = 1_z; i < n; ++i)
      {
        if (!maths::almostEqual(k * v[i], u[i]))
          return T(NAN);
      }
    return k;
  }

  //! \brief Check if two vectors are parallels.
  template <typename T, size_t n>
  bool collinear(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    T k = collinearity(u, v);
    return !isnan(k);
  }

  //! \brief Check if two vectors are mathematicaly equivalent: same
  //! norm (length), same direction (parallel) and same sign.
  template <typename T, size_t n>
  bool equivalent(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    T k = collinearity(u, v);
    return maths::almostEqual(k, maths::one<T>());
  }

  //! \brief Check if three points A, B, C are aligned.
  template <typename T, size_t n>
  bool arePointsAligned(Vector<T, n> const &a, Vector<T, n> const &b, Vector<T, n> const &c)
  {
    return collinear(b - a, c - a);
  }

// collinear
// http://www.educastream.com/vecteurs-colineaires-seconde

// FIXME u et v colineaires s'il existe k: v = ku
// FIXME bool colineaire(v1, v2) // [2 -3] et [4 -6]
// ==> x1y2 == x2y1
// x1/x2 == y1/y2 == z1/z2 == k

// colineaire: [3 -2] et [-15 10]
// Non col (6 4) (4 2); (3 -2) (12 -5)

// ==> paralles: ssi colineaire
// aligned(A, B, C): colineare(AB, AC)

  //! \brief Constrain each value of the vectorto lower and upper bounds.
  template <typename T, size_t n>
  Vector<T, n> clamp(Vector<T, n> const &a, T const lower, T const upper)
  {
    Vector<T, n> result;
    size_t i = n;

    while (i--)
      result[i] = maths::clamp(a[i], lower, upper);
    return result;
  }

  //! \brief Dot product.
  template <typename T, size_t n>
  T dot(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    T result(maths::zero<T>());
    size_t i = n;

    while (i--)
      result += a[i] * b[i];
    return result;
  }

  template <typename T, size_t n>
  T squaredLength(Vector<T, n> const &a)
  {
    return dot(a, a);
  }

  template <typename T, size_t n>
  T length(Vector<T, n> const &a)
  {
    return T(std::sqrt(squaredLength(a)));
  }

  template <typename T, size_t n>
  T norm(Vector<T, n> const &a)
  {
    return length(a);
  }

  template <typename T, size_t n>
  T squaredDistance(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return squaredLength(a - b);
  }

  template <typename T, size_t n>
  T distance(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return std::sqrt(squaredDistance(a, b));
  }

  template <typename T, size_t n>
  Vector<T, n> normalize(Vector<T, n> const &a)
  {
    // FIXME: throw exception
    return a / length(a);
  }

  template <typename T, size_t n>
  Vector<T, n> middle(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    Vector<T, n> result;
    size_t i = n;

    while (i--)
      result[i] = (a[i] + b[i]) / T(2);
    return result;
  }

  template <typename T>
  Vector<T, 3_z> cross(Vector<T, 3_z> const &a, Vector<T, 3> const &b)
  {
    return
      {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
      };
  }

  //! \brief Perpendicular
  template <typename T>
  Vector<T, 2_z> orthogonal(Vector<T, 2_z> const &a)
  {
    return { -a.y, a.x };
  }

  template <typename T>
  Vector<T, 3_z> orthogonal(Vector<T, 3_z> const &a)
  {
    // Implementation due to Sam Hocevar - see blog post:
    // http://lolengine.net/blog/2013/09/21/picking-orthogonal-Vector-combing-coconuts
    if (maths::abs(a.x) > maths::abs(a.z))
      return { -a.y, a.x,maths::zero<T>() };
    else
      return { maths::zero<T>(), -a.z, a.y };
  }

  template <typename T, size_t n>
  typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
  orthogonal(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return maths::zero<T>() == dot(a, b);
  }

  template <typename T, size_t n>
  typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  orthogonal(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return maths::almostZero(dot(a, b));
  }

  template <typename T, size_t n>
  T angleBetween(Vector<T, n> const &org, Vector<T, n> const &dest)
  {
    T lenProduct = norm(org) * norm(dest);

    // Divide by zero check
    //if (lenProduct < 1e-6f)
    //  lenProduct = 1e-6f;

    T f = dot(org, dest) / lenProduct;
    f = std::min(std::max(f, -maths::one<T>()), maths::one<T>());
    return T(std::acos(f) * 180.0 / 3.14159265);
  }

  template <typename T, size_t n>
  Vector<T, n> reflect(Vector<T, n> const &v, Vector<T, n> const &normal)
  {
    return v - (T(2) * dot(v, normal) * normal);
  }
}

template <typename T, size_t n>
std::ostream& operator<<(std::ostream& os, Vector<T, n> const& v)
{
  os << "Vector(" << v[0];
  for (size_t i = 1; i < n; ++i)
    {
      os << ", " << v[i];
    }
  os << ')';
  return os;
}

// *************************************************************************************************
// Typedefs for the most common types and dimensions
// *************************************************************************************************

typedef Vector<bool, 2_z> Vector2b;
typedef Vector<bool, 3_z> Vector3b;
typedef Vector<bool, 4_z> Vector4b;

typedef Vector<int32_t, 2_z> Vector2i;
typedef Vector<int32_t, 3_z> Vector3i;
typedef Vector<int32_t, 4_z> Vector4i;

typedef Vector<float, 2_z> Vector2f;
typedef Vector<float, 3_z> Vector3f;
typedef Vector<float, 4_z> Vector4f;

typedef Vector<double, 2_z> Vector2g;
typedef Vector<double, 3_z> Vector3g;
typedef Vector<double, 4_z> Vector4g;

#  undef DEFINE_UNARY_OPERATOR
#  undef DEFINE_BINARY_OPERATORS
#  undef DEFINE_INPLACE_OPERATORS
#  undef DEFINE_RELATIONAL_OPERATORS
#  undef DEFINE_FUN1_OPERATOR
#  undef DEFINE_FUN2_OPERATOR
#  undef DEFINE_BOOL_OPERATOR

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_VECTOR_HPP
