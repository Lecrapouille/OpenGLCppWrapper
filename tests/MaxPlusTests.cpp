//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

#define protected public
#define private public
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wfloat-equal"
#    include "Math/Matrix.hpp"
#  pragma GCC diagnostic pop
#undef protected
#undef private

template<class T>
class MaxPlus
{
public:

  MaxPlus() : val() {};
  MaxPlus(const MaxPlus & d) : val(d.val){}
  MaxPlus(const T t) : val(t){}
  inline bool operator==(const MaxPlus &rhs) const { return val == rhs.val; }
  inline bool operator==(const T &rhs) const { return val == rhs; }
  inline MaxPlus & operator=(const MaxPlus & rhs) { val = rhs.val; return *this;}
  inline MaxPlus & operator=(const T rhs) { val = rhs; return *this;}
  inline T operator*=(const MaxPlus & rhs) { val += rhs.val; return val; }
  inline T operator+=(const MaxPlus & rhs) { val = std::max(val, rhs.val); return val; }
  inline T operator*(const MaxPlus & rhs) const { return val + rhs.val; }
  inline T operator+(const MaxPlus & rhs) const { return std::max(val, rhs.val); }
  inline T operator/(const MaxPlus & rhs) const { return val - rhs.val; }
  inline T operator-(const MaxPlus & rhs) const { return val - rhs.val; }
  inline T operator-() const { return -val; }
  inline T operator+() const { return val; }
  //inline operator const T & () const { return val; }
  //inline operator T & () { return val; }

  T val;
};

template<> MaxPlus<float>  maths::zero<MaxPlus<float>>()  { return std::numeric_limits<float>::infinity(); }
template<> MaxPlus<float>  maths::one<MaxPlus<float>>()   { return 0; }

//--------------------------------------------------------------------------
template<typename T>
static void checkVector(Vector<T,4_z> const& v, const T x, const T y, const T z, const T w)
{
    ASSERT_EQ(x, v.x); ASSERT_EQ(y, v.y); ASSERT_EQ(z, v.z); ASSERT_EQ(w, v.w);
    ASSERT_EQ(x, v.r); ASSERT_EQ(y, v.b); ASSERT_EQ(z, v.g); ASSERT_EQ(w, v.a);
    ASSERT_EQ(x, v[0]); ASSERT_EQ(y, v[1]); ASSERT_EQ(z, v[2]); ASSERT_EQ(w, v[3]);
}

//--------------------------------------------------------------------------
TEST(TestMaxPlus, testConstructor)
{
    Matrix<MaxPlus<float>, 4u, 4u> A(matrix::Zero);
    Matrix<MaxPlus<float>, 4u, 4u> B(matrix::Once);
    Matrix<MaxPlus<float>, 4u, 4u> C(matrix::Identity);

    const float infinity = std::numeric_limits<float>::infinity();
    ASSERT_EQ(zero<MaxPlus<float>>, infinity);

    checkVector(A[0], infinity, infinity, infinity, infinity);
    checkVector(A[1], infinity, infinity, infinity, infinity);
    checkVector(A[2], infinity, infinity, infinity, infinity);
    checkVector(A[3], infinity, infinity, infinity, infinity);

    checkVector(B[0], 0.0f, 0.0f, 0.0f, 0.0f);
    checkVector(B[1], 0.0f, 0.0f, 0.0f, 0.0f);
    checkVector(B[2], 0.0f, 0.0f, 0.0f, 0.0f);
    checkVector(B[3], 0.0f, 0.0f, 0.0f, 0.0f);

    checkVector(C[0], 0.0f, infinity, infinity, infinity);
    checkVector(C[1], infinity, 0.0f, infinity, infinity);
    checkVector(C[2], infinity, infinity, 0.0f, infinity);
    checkVector(C[3], infinity, infinity, infinity, 0.0f);
}
