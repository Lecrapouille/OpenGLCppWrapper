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

#include "main.hpp"
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

    MaxPlus() = default;
    inline explicit MaxPlus(const MaxPlus & d) : val(d.val) {}
    inline explicit MaxPlus(const T t) : val(t) {}
    inline MaxPlus(MaxPlus&& o) noexcept : val(std::move(o.val)) {}
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
    inline operator const T & () const { return val; }
    inline operator T & () { return val; }

    T val;
};

inline std::ostream& operator<<(std::ostream& os, MaxPlus<float> const& m)
{
  std::cout << m.val;
  return os;
}

template<>
MaxPlus<float> maths::zero<MaxPlus<float>>()
{
    return MaxPlus<float>(std::numeric_limits<float>::infinity());
}

template<>
MaxPlus<float> maths::one<MaxPlus<float>>()
{
    return MaxPlus<float>(0.0f);
}

//--------------------------------------------------------------------------
#define ASSERT_ARR_FLOATS_NEARLY_EQ(expected, actual, size, thresh)     \
    for (size_t idx = 0; idx < size; ++idx)                             \
    {                                                                   \
        ASSERT_NEAR(expected[idx], actual[idx], thresh) << "at index: " << idx; \
    }

//--------------------------------------------------------------------------
#define ASSERT_VECT_FLOATS_NEARLY_EQ(expected, actual, thresh)          \
    ASSERT_EQ(expected.size(), actual.size()) << "Array sizes differ."; \
    for (size_t idx = 0; idx < std::min(expected.size(), actual.size()); ++idx) \
    {                                                                   \
        ASSERT_NEAR(expected[idx], actual[idx], thresh) << "at index: " << idx; \
    }

//--------------------------------------------------------------------------
TEST(TestMaxPlus, testConstructor)
{
    Matrix<MaxPlus<float>, 4u, 4u> A(matrix::Zero);
    Matrix<MaxPlus<float>, 4u, 4u> B(matrix::One);
    Matrix<MaxPlus<float>, 4u, 4u> C(matrix::Identity);

    const float infinity = std::numeric_limits<float>::infinity();
    ASSERT_EQ(maths::zero<MaxPlus<float>>(), infinity);

    ASSERT_THAT(A[0].data(), ElementsAre(infinity, infinity, infinity, infinity));
    ASSERT_THAT(A[1].data(), ElementsAre(infinity, infinity, infinity, infinity));
    ASSERT_THAT(A[2].data(), ElementsAre(infinity, infinity, infinity, infinity));
    ASSERT_THAT(A[3].data(), ElementsAre(infinity, infinity, infinity, infinity));

    ASSERT_THAT(B[0].data(), ElementsAre(0.0f, 0.0f, 0.0f, 0.0f));
    ASSERT_THAT(B[1].data(), ElementsAre(0.0f, 0.0f, 0.0f, 0.0f));
    ASSERT_THAT(B[2].data(), ElementsAre(0.0f, 0.0f, 0.0f, 0.0f));
    ASSERT_THAT(B[3].data(), ElementsAre(0.0f, 0.0f, 0.0f, 0.0f));

    ASSERT_THAT(C[0].data(), ElementsAre(0.0f, infinity, infinity, infinity));
    ASSERT_THAT(C[1].data(), ElementsAre(infinity, 0.0f, infinity, infinity));
    ASSERT_THAT(C[2].data(), ElementsAre(infinity, infinity, 0.0f, infinity));
    ASSERT_THAT(C[3].data(), ElementsAre(infinity, infinity, infinity, 0.0f));
}
