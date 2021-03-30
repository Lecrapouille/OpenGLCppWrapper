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
#    include "Math/Vector.hpp"
#  pragma GCC diagnostic pop
#undef protected
#undef private

//--------------------------------------------------------------------------
// Check union [0,1,2,3] and [x,y,z,w] and [r,g,b,a]
#define ASSERT_NEAR_VECTOR4(vect, a, b, c, d, thresh)                        \
    ASSERT_NEAR(vect[0], a, thresh);                                    \
    ASSERT_NEAR(vect[1], b, thresh);                                    \
    ASSERT_NEAR(vect[2], c, thresh);                                    \
    ASSERT_NEAR(vect[3], d, thresh);                                    \
    ASSERT_NEAR(vect.x, a, thresh);                                     \
    ASSERT_NEAR(vect.y, b, thresh);                                     \
    ASSERT_NEAR(vect.z, c, thresh);                                     \
    ASSERT_NEAR(vect.w, d, thresh);                                     \
    ASSERT_NEAR(vect.r, a, thresh);                                     \
    ASSERT_NEAR(vect.g, b, thresh);                                     \
    ASSERT_NEAR(vect.b_, c, thresh);                                     \
    ASSERT_NEAR(vect.a_, d, thresh)

//--------------------------------------------------------------------------
#define ASSERT_VECTOR4_NAN(vect)                                        \
    ASSERT_EQ(true, std::isnan(vect[0]));                               \
    ASSERT_EQ(true, std::isnan(vect[1]));                               \
    ASSERT_EQ(true, std::isnan(vect[2]));                               \
    ASSERT_EQ(true, std::isnan(vect[3]));                               \
    ASSERT_EQ(true, std::isnan(vect.x));                                \
    ASSERT_EQ(true, std::isnan(vect.y));                                \
    ASSERT_EQ(true, std::isnan(vect.z));                                \
    ASSERT_EQ(true, std::isnan(vect.w));                                \
    ASSERT_EQ(true, std::isnan(vect.r));                                \
    ASSERT_EQ(true, std::isnan(vect.g));                                \
    ASSERT_EQ(true, std::isnan(vect.b_));                               \
    ASSERT_EQ(true, std::isnan(vect.a_))

//--------------------------------------------------------------------------
// Check union [0,1,2] and [x,y,z] and [r,g,b]
#define ASSERT_NEAR_VECTOR3(vect, a, b, c, thresh)                           \
    ASSERT_NEAR(vect[0], a, thresh);                                    \
    ASSERT_NEAR(vect[1], b, thresh);                                    \
    ASSERT_NEAR(vect[2], c, thresh);                                    \
    ASSERT_NEAR(vect.x, a, thresh);                                     \
    ASSERT_NEAR(vect.y, b, thresh);                                     \
    ASSERT_NEAR(vect.z, c, thresh);                                     \
    ASSERT_NEAR(vect.r, a, thresh);                                     \
    ASSERT_NEAR(vect.g, b, thresh);                                     \
    ASSERT_NEAR(vect.b_, c, thresh)

//--------------------------------------------------------------------------
#define ASSERT_THAT_VECTOR3(vect, a, b, c)                      \
    ASSERT_THAT(vect[0], a);                                    \
    ASSERT_THAT(vect[1], b);                                    \
    ASSERT_THAT(vect[2], c);                                    \
    ASSERT_THAT(vect.x, a);                                     \
    ASSERT_THAT(vect.y, b);                                     \
    ASSERT_THAT(vect.z, c);                                     \
    ASSERT_THAT(vect.r, a);                                     \
    ASSERT_THAT(vect.g, b);                                     \
    ASSERT_THAT(vect.b_, c)

//--------------------------------------------------------------------------
#define ASSERT_VECTOR3_NAN(vect)                                   \
    ASSERT_EQ(true, std::isnan(vect[0]));                               \
    ASSERT_EQ(true, std::isnan(vect[1]));                               \
    ASSERT_EQ(true, std::isnan(vect[2]));                               \
    ASSERT_EQ(true, std::isnan(vect.x));                                \
    ASSERT_EQ(true, std::isnan(vect.y));                                \
    ASSERT_EQ(true, std::isnan(vect.z));                                \
    ASSERT_EQ(true, std::isnan(vect.r));                                \
    ASSERT_EQ(true, std::isnan(vect.g));                                \
    ASSERT_EQ(true, std::isnan(vect.b_))

//--------------------------------------------------------------------------
#define ASSERT_NEAR_VECTOR3_INF(vect)                                        \
    ASSERT_EQ(true, std::isinf(vect[0]));                               \
    ASSERT_EQ(true, std::isinf(vect[1]));                               \
    ASSERT_EQ(true, std::isinf(vect[2]));                               \
    ASSERT_EQ(true, std::isinf(vect.x));                                \
    ASSERT_EQ(true, std::isinf(vect.y));                                \
    ASSERT_EQ(true, std::isinf(vect.z));                                \
    ASSERT_EQ(true, std::isinf(vect.r));                                \
    ASSERT_EQ(true, std::isinf(vect.g));                                \
    ASSERT_EQ(true, std::isinf(vect.b_))

//--------------------------------------------------------------------------
// Check union [0,1] and [x,y] and [u,v]
#define ASSERT_NEAR_VECTOR2(vect, a, b, thresh)                              \
    ASSERT_NEAR(vect[0], a, thresh);                                    \
    ASSERT_NEAR(vect[1], b, thresh);                                    \
    ASSERT_NEAR(vect.x, a, thresh);                                     \
    ASSERT_NEAR(vect.y, b, thresh);                                     \
    ASSERT_NEAR(vect.u, a, thresh);                                     \
    ASSERT_NEAR(vect.v, b, thresh)

//--------------------------------------------------------------------------
#define ASSERT_VECTOR2_NAN(vect)                                        \
    ASSERT_EQ(true, std::isnan(vect[0]));                               \
    ASSERT_EQ(true, std::isnan(vect[1]));                               \
    ASSERT_EQ(true, std::isnan(vect.x));                                \
    ASSERT_EQ(true, std::isnan(vect.y));                                \
    ASSERT_EQ(true, std::isnan(vect.u));                                \
    ASSERT_EQ(true, std::isnan(vect.v))

//--------------------------------------------------------------------------
TEST(TestVectors, testSizeof)
{
    ASSERT_EQ(2u * sizeof (float), sizeof (Vector2f));
    ASSERT_EQ(2u * sizeof (double), sizeof (Vector2g));
    ASSERT_EQ(2u * sizeof (int), sizeof (Vector2i));

    ASSERT_EQ(3u * sizeof (float), sizeof (Vector3f));
    ASSERT_EQ(3u * sizeof (double), sizeof (Vector3g));
    ASSERT_EQ(3u * sizeof (int), sizeof (Vector3i));

    ASSERT_EQ(4u * sizeof (float), sizeof (Vector4f));
    ASSERT_EQ(4u * sizeof (double), sizeof (Vector4g));
    ASSERT_EQ(4u * sizeof (int), sizeof (Vector4i));
}

//--------------------------------------------------------------------------
TEST(TestVectors, testConstructorVec4)
{
    Vector4f v1;
    Vector4f v2(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f v3(Vector3f(1.0f, 2.0f, 3.0f));
    Vector4f v4 = { 4.0f, 5.0f, 6.0f, 7.0f };
    Vector4f v5(1, 2, 3, 4);
    Vector4f v6 = { -4, 5, -6 };
    Vector4f v7(Vector2f(1, 2));
    Vector4f v8(42);
    Vector4f dummy = Vector4f::DUMMY;

    // Check size
    ASSERT_EQ(4_z, v1.size());
    ASSERT_EQ(4_z, v2.size());
    ASSERT_EQ(4_z, v3.size());
    ASSERT_EQ(4_z, v4.size());
    ASSERT_EQ(4_z, v5.size());
    ASSERT_EQ(4_z, v6.size());
    ASSERT_EQ(4_z, v7.size());
    ASSERT_EQ(4_z, v8.size());
    ASSERT_EQ(4_z, dummy.size());

    // Check values passed to the constructor
    ASSERT_NEAR_VECTOR4(v2, 1.0f, 2.0f, 3.0f, 4.0f,   0.001f);
    ASSERT_NEAR_VECTOR4(v3, 1.0f, 2.0f, 3.0f, 0.0f,   0.001f);
    ASSERT_NEAR_VECTOR4(v4, 4.0f, 5.0f, 6.0f, 7.0f,   0.001f);
    ASSERT_NEAR_VECTOR4(v5, 1.0f, 2.0f, 3.0f, 4.0f,   0.001f);
    ASSERT_NEAR_VECTOR4(v6, -4.0f, 5.0f, -6.0f, 0.0f, 0.001f);
    ASSERT_NEAR_VECTOR4(v7, 1.0f, 2.0f, 0.0f, 0.0f,   0.001f);
    ASSERT_NEAR_VECTOR4(v8, 42.0f, 42.0f, 42.0f, 42.0f, 0.001f);
    ASSERT_VECTOR4_NAN(dummy);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testConstructorVec3)
{
    Vector3f v1;
    Vector3f v2(1.0f, 2.0f, 3.0f);
    Vector3f v3(1.0f, 2.0f);
    Vector3f v4 = { 4.0f, 5.0f, 6.0f };
    Vector3f v5(1, 2, 3);
    Vector3f v6 = { -4, 5, -6 };
    Vector3f v7(Vector2f(1, 2));
    Vector3f v8(42);
    Vector3f dummy = Vector3f::DUMMY;

    // Check size
    ASSERT_EQ(3_z, v1.size());
    ASSERT_EQ(3_z, v2.size());
    ASSERT_EQ(3_z, v3.size());
    ASSERT_EQ(3_z, v4.size());
    ASSERT_EQ(3_z, v5.size());
    ASSERT_EQ(3_z, v6.size());
    ASSERT_EQ(3_z, v7.size());
    ASSERT_EQ(3_z, v8.size());
    ASSERT_EQ(3_z, dummy.size());

    // Check values passed to the constructor
    ASSERT_NEAR_VECTOR3(v2, 1.0f, 2.0f, 3.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v3, 1.0f, 2.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v4, 4.0f, 5.0f, 6.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v5, 1.0f, 2.0f, 3.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v6, -4.0f, 5.0f, -6.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v7, 1.0f, 2.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(v8, 42.0f, 42.0f, 42.0f,  0.001f);
    ASSERT_VECTOR3_NAN(dummy);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testConstructorVec2)
{
    Vector2f v1;
    Vector2f v2(1.0f, 2.0f);
    Vector2f v3(1.0f);
    Vector2f v4 = { 4.0f, 5.0f, 6.0f };
    Vector2f v5(1, 2);
    Vector2f v6 = { -4, 5, -6 };
    Vector2f v7(Vector2f(1, 2));
    Vector2f v8(42);
    Vector2f dummy = Vector2f::DUMMY;

    // Check size
    ASSERT_EQ(2_z, v1.size());
    ASSERT_EQ(2_z, v2.size());
    ASSERT_EQ(2_z, v3.size());
    ASSERT_EQ(2_z, v4.size());
    ASSERT_EQ(2_z, v5.size());
    ASSERT_EQ(2_z, v6.size());
    ASSERT_EQ(2_z, v7.size());
    ASSERT_EQ(2_z, v8.size());
    ASSERT_EQ(2_z, dummy.size());

    // Check values passed to the constructor
    ASSERT_NEAR_VECTOR2(v2, 1.0f, 2.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v3, 1.0f, 1.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v4, 4.0f, 5.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v5, 1.0f, 2.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v6, -4.0f, 5.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v7, 1.0f, 2.0f,  0.001f);
    ASSERT_NEAR_VECTOR2(v8, 42.0f, 42.0f,  0.001f);
    ASSERT_VECTOR2_NAN(dummy);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testPredefined)
{
    ASSERT_VECTOR3_NAN(Vector3f::DUMMY);
    ASSERT_NEAR_VECTOR3(Vector3f::POSITIVE_INFINITY, maths::max<float>(), maths::max<float>(), maths::max<float>(), 0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::NEGATIVE_INFINITY, -maths::max<float>(), -maths::max<float>(), -maths::max<float>(), 0.001f);

    ASSERT_NEAR_VECTOR3(Vector3f::ZERO, 0.0f, 0.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::ONE, 1.0f, 1.0f, 1.0f,  0.001f);

    ASSERT_NEAR_VECTOR3(Vector3f::UNIT_SCALE, 1.0f, 1.0f, 1.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::NEGATIVE_UNIT_SCALE, -1.0f, -1.0f, -1.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::UNIT_X, 1.0f, 0.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::UNIT_Y, 0.0f, 1.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::UNIT_Z, 0.0f, 0.0f, 1.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::NEGATIVE_UNIT_X, -1.0f, 0.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::NEGATIVE_UNIT_Y, 0.0f, -1.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::NEGATIVE_UNIT_Z, 0.0f, 0.0f, -1.0f,  0.001f);

    ASSERT_NEAR_VECTOR3(Vector3f::LEFT, -1.0f, 0.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::RIGHT, 1.0f, 0.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::BACK, 0.0f, 0.0f, -1.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::FORWARD, 0.0f, 0.0f, 1.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::DOWN, 0.0f, -1.0f, 0.0f,  0.001f);
    ASSERT_NEAR_VECTOR3(Vector3f::UP, 0.0f, 1.0f, 0.0f,  0.001f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testPrint)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    std::cout << Vector4f::ZERO << std::endl;
    std::cout.rdbuf(old);

    ASSERT_THAT(buffer.str().c_str(), HasSubstr("[0, 0, 0, 0]"));
}

//--------------------------------------------------------------------------
TEST(TestVectors, testSwap)
{
    Vector3f v2(1.0f, 2.0f, 3.0f);
    Vector3f v4 = { 4.0f, 5.0f, 6.0f };

    vector::swap(v2, v4);
    ASSERT_NEAR_VECTOR3(v2, 4.0f, 5.0f, 6.0f,  0.001f);
    vector::swap(v2, v4);
    ASSERT_NEAR_VECTOR3(v2, 1.0f, 2.0f, 3.0f,  0.001f);
    vector::swap(v4, v2);
    ASSERT_NEAR_VECTOR3(v2, 4.0f, 5.0f, 6.0f,  0.001f);
    vector::swap(v4, v2);
    ASSERT_NEAR_VECTOR3(v2, 1.0f, 2.0f, 3.0f,  0.001f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testComparaisons)
{
    Vector3f one(1.0f);
    Vector3f two(2.0f);

    // Operator <
    {
        Vector3b A = (one < two);
        Vector3b B = (two < one);
        Vector3b C = (one < one);

        ASSERT_THAT_VECTOR3(A, true, true, true);
        ASSERT_THAT_VECTOR3(B, false, false, false);
        ASSERT_THAT_VECTOR3(C, false, false, false);
    }
    /*

    Vector3b v = (v3 == v7);
    CHECK_VECTOR3B(v, true, true, true);
    v = (v2 == v5);
    CHECK_VECTOR3B(v, true, true, true);
    v = (v3 == v2);
    CHECK_VECTOR3B(v, true, true, false);
    v = (v2 != v5);
    CHECK_VECTOR3B(v, false, false, false);
    v = (v3 != v2);
    CHECK_VECTOR3B(v, false, false, true);
    v = (v3 < v2);
    CHECK_VECTOR3B(v, false, false, true);
    v = (v3 <= v2);
    CHECK_VECTOR3B(v, true, true, true);
    v = (v3 > v2);
    CHECK_VECTOR3B(v, false, false, false);
    v = (v3 >= v2);
    CHECK_VECTOR3B(v, true, true, false);
    v = (Vector3f::UNIT_SCALE == -Vector3f::NEGATIVE_UNIT_SCALE);
    CHECK_VECTOR3B(v, true, true, true);
    v = (Vector3f::NEGATIVE_UNIT_SCALE ==
         (Vector3f::NEGATIVE_UNIT_X + Vector3f::NEGATIVE_UNIT_Y + Vector3f::NEGATIVE_UNIT_Z));
    CHECK_VECTOR3B(v, true, true, true);
    */
}

#if 0

static Vector3f v1;
static Vector3f v2(1.0f, 2.0f, 3.0f);
static Vector3f v3(1.0f, 2.0f);
static Vector3f v4 = { 4.0f, 5.0f, 6.0f };
static Vector3f v5(1, 2, 3);
static Vector3f v6 = { -4, 5, -6 };
static Vector3f v7(Vector2f(1, 2));
static Vector3f v8(42);
static Vector3f dummy = Vector3f::DUMMY;

#define ASSERT_DOUBLES_EQUAL(a,b,c) ASSERT_EQ(true, maths::abs(a-b) < c)




//--------------------------------------------------------------------------
TEST(TestVectors, testArithmetic)
{
    const float c_scalar = -2.0f;
    float scalar = -2.0f;

    // Addition, substraction
    ASSERT_NEAR_VECTOR3((v2 + v5 + v3), 3.0f, 6.0f, 6.0f);
    ASSERT_NEAR_VECTOR3(Vector3f::ZERO + 4.0f, 4.0f, 4.0f, 4.0f);
    ASSERT_NEAR_VECTOR3(Vector3f::UNIT_X - Vector3f::UNIT_X, 0.0f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(-Vector3f::UNIT_X, -1.0f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(+Vector3f::UNIT_X, +1.0f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(-v2, -1.0f, -2.0f, -3.0f);

    // Multiplication
    ASSERT_NEAR_VECTOR3((v5 * 2.0f), 2.0f, 4.0f, 6.0f);
    ASSERT_NEAR_VECTOR3((v5 * -2.0f), -2.0f, -4.0f, -6.0f);
    ASSERT_NEAR_VECTOR3((-2.0f * v5), -2.0f, -4.0f, -6.0f);
    ASSERT_NEAR_VECTOR3((c_scalar * v5), -2.0f, -4.0f, -6.0f);
    ASSERT_NEAR_VECTOR3((-v5 * 2.0f), -2.0f, -4.0f, -6.0f);
    ASSERT_NEAR_VECTOR3((-v5 * c_scalar), 2.0f, 4.0f, 6.0f);
    ASSERT_NEAR_VECTOR3((-v5 * scalar), 2.0f, 4.0f, 6.0f);

    // Division
    ASSERT_NEAR_VECTOR3((v5 / 2.0f), 0.5f, 1.0f, 3.0f/2.0f);
    ASSERT_NEAR_VECTOR3((v5 / -2.0f), -0.5f, -1.0f, -3.0f/2.0f);
    ASSERT_NEAR_VECTOR3((-2.0f / v5), -2.0f, -1.0f, -2.0f/3.0f);
    ASSERT_NEAR_VECTOR3((c_scalar / v5), -2.0f, -1.0f, -2.0f/3.0f);
    ASSERT_NEAR_VECTOR3((scalar / v5), -2.0f, -1.0f, -2.0f/3.0f);
    ASSERT_NEAR_VECTOR3((-v5 / 2.0f), -0.5f, -1.0f, -3.0f/2.0f);
    ASSERT_NEAR_VECTOR3((-v5 / c_scalar), 0.5f, 1.0f, 3.0f/2.0f);
    ASSERT_NEAR_VECTOR3((-v5 / scalar), 0.5f, 1.0f, 3.0f/2.0f);

    //
    static Vector3f v(v2);
    v += 1.0f;
    ASSERT_NEAR_VECTOR3(v, 2.0f, 3.0f, 4.0f);
    v += 1.0f;
    ASSERT_NEAR_VECTOR3(v, 3.0f, 4.0f, 5.0f);
    v -= 2.0f;
    ASSERT_NEAR_VECTOR3(v, 1.0f, 2.0f, 3.0f);
    v /= 2.0f;
    ASSERT_NEAR_VECTOR3(v, 1.0f / 2.0f, 1.0f, 3.0f / 2.0f);
    v *= 2.0f;
    ASSERT_NEAR_VECTOR3(v, 1.0f, 2.0f, 3.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testCopy)
{
    v1 = v5;
    ASSERT_NEAR_VECTOR3(v1, 1.0f, 2.0f, 3.0f);
    //v1 = 42.0f;
    //ASSERT_NEAR_VECTOR(v1, 42.0f, 42.0f, 42.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testOperations)
{
    // Min, max, clamp, abs
    ASSERT_NEAR_VECTOR3(vector::abs(v6), 4.0f, 5.0f, 6.0f);
    ASSERT_NEAR_VECTOR3(vector::min(v2, dummy), v2.x, v2.y, v2.z);
    ASSERT_NEAR_VECTOR3(vector::max(v2, dummy), v2.x, v2.y, v2.z);
    ASSERT_NEAR_VECTOR3(vector::min(v2, Vector3f::UNIT_Z + 1.0f), 1.0f, 1.0f, 2.0f);
    ASSERT_NEAR_VECTOR3(vector::max(v2, Vector3f::UNIT_Y + 2.0f), 2.0f, 3.0f, 3.0f);
    ASSERT_NEAR_VECTOR3(vector::clamp(v6, -5.0f, 3.0f), -4.0f, 3.0f, -5.0f);
    ASSERT_NEAR_VECTOR3(vector::clamp(vector::abs(v6), -3.0f, 5.0f), 4.0f, 5.0f, 5.0f);

    // Middle Point
    ASSERT_NEAR_VECTOR3(vector::middle(Vector3f::ZERO, Vector3f::UNIT_Z), 0.0f, 0.0f, 0.5f);
    ASSERT_NEAR_VECTOR3(vector::middle(Vector3f::ZERO, Vector3f::UNIT_Y), 0.0f, 0.5f, 0.0f);
    ASSERT_NEAR_VECTOR3(vector::middle(Vector3f::ZERO, Vector3f::UNIT_X), 0.5f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(vector::middle(Vector3f::UNIT_X, -Vector3f::UNIT_X), 0.0f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(vector::middle(-Vector3f::UNIT_X, Vector3f::UNIT_X), 0.0f, 0.0f, 0.0f);
    ASSERT_NEAR_VECTOR3(-vector::middle(Vector3f::UNIT_X, Vector3f::UNIT_X), -1.0f, 0.0f, 0.0f);

    // Add scaled
    v1 = Vector3f::UNIT_SCALE;
    v1.addScaled(Vector3f::UNIT_X, 2.0f);
    CHECK_VECTOR3B((v1 == Vector3f(3.0f, 1.0f, 1.0f)), true, true, true);

    // Distance
    v1 = Vector3f::UNIT_SCALE * 2.0f;
    ASSERT_EQ(maths::sqrt(12.0f), vector::distance(v1, Vector3f::ZERO));
    v1 = Vector3f::ZERO;
    ASSERT_EQ(0.0f, vector::distance(v1, Vector3f::ZERO));

    // Norm
    ASSERT_EQ(5.0f, vector::norm(Vector2i(-3, 4)));
    ASSERT_EQ(5.0f, vector::magnitude(Vector2i(-3, 4)));
    ASSERT_EQ(5.0f, Vector2f(-3, 4).norm());
    ASSERT_EQ(5.0f, Vector2f(-3, 4).norm());
    ASSERT_EQ(5.0f, maths::sqrt(vector::squaredMagnitude(Vector2f(-3, 4))));
    ASSERT_EQ(1.0f, vector::magnitude(Vector3f::UNIT_X));
    ASSERT_EQ(maths::sqrt(3.0f), vector::magnitude(Vector3f::UNIT_SCALE));
    ASSERT_EQ(maths::sqrt(3.0f), vector::magnitude(-Vector3f::UNIT_SCALE));
    ASSERT_EQ(3.0f, vector::squaredMagnitude(Vector3f::UNIT_SCALE));
    ASSERT_EQ(0.0f, vector::dot(Vector3f::UNIT_X, Vector3f::UNIT_Y));
    ASSERT_EQ(3.0f, vector::dot(Vector3f::UNIT_SCALE, Vector3f::UNIT_SCALE));

    // Normalize
    v1 = vector::normalize(Vector3f::UNIT_SCALE * 2.0f);
    v2 = Vector3f::UNIT_SCALE / maths::sqrt(3.0f);
    CHECK_VECTOR3B(v2 == v1, true, true, true);

    // Perpendicular 2D
    Vector2f a = Vector2f(2.0f, 4.0f);
    Vector2f o = vector::orthogonal(Vector2f(2.0f, 4.0f));
    ASSERT_EQ(true, vector::areOrthogonal(o, a));
    ASSERT_EQ(true, vector::eq(o, Vector2f(-4.0f, 2.0f)));
    ASSERT_EQ(false, vector::areOrthogonal(a, Vector2f(3.0f, 2.0f)));

    // Perpendicular 3D

    // Colinear 2D
    ASSERT_EQ(true, vector::areCollinear(Vector2f(3.0f, -2.0f), Vector2f(-15.0f, 10.0f)));
    ASSERT_EQ(false, vector::areCollinear(Vector2f(6.0f, 4.0f), Vector2f(4.0f, 2.0f)));

    // Cross product 3D
    Vector3f v = vector::cross(Vector3f::UNIT_X, Vector3f::UNIT_Y);
    CHECK_VECTOR3B((v == Vector3f::UNIT_Z), true, true, true);
    v = vector::cross(Vector3f::UNIT_Y, Vector3f::UNIT_X);
    CHECK_VECTOR3B((v == -Vector3f::UNIT_Z), true, true, true);

    // Cross product 3D
    v = Vector3f::UNIT_X % Vector3f::UNIT_Y;
    CHECK_VECTOR3B((v == Vector3f::UNIT_Z), true, true, true);
    v = Vector3f::UNIT_Y % Vector3f::UNIT_X;
    CHECK_VECTOR3B((v == -Vector3f::UNIT_Z), true, true, true);

    // Cross product 2D
    float b = vector::cross(Vector2f::UNIT_X, Vector2f::UNIT_Y);
    ASSERT_EQ(1.0f, b);
    b = vector::cross(Vector2f::UNIT_Y, Vector2f::UNIT_X);
    ASSERT_EQ(-1.0f, b);

    // Scalar product
    b = Vector3f::UNIT_X * Vector3f::UNIT_X;
    ASSERT_EQ(1.0f, b);
    b = Vector3f::UNIT_X * Vector3f::UNIT_Y;
    ASSERT_EQ(0.0f, b);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testComplexMath)
{
    ASSERT_EQ(true, vector::eq(Vector3f::NEGATIVE_UNIT_X, vector::reflect(Vector3f::UNIT_X, Vector3f::UNIT_X)));
    ASSERT_EQ(true, vector::eq(Vector3f::NEGATIVE_UNIT_Y, vector::reflect(Vector3f::UNIT_Y, Vector3f::UNIT_Y)));
    ASSERT_EQ(true, vector::eq(Vector3f::NEGATIVE_UNIT_Z, vector::reflect(Vector3f::UNIT_Z, Vector3f::UNIT_Z)));
    ASSERT_EQ(true, vector::eq(Vector3f::UNIT_X, vector::reflect(Vector3f::UNIT_X, Vector3f::UNIT_Y)));

    ASSERT_DOUBLES_EQUAL(00.0f, vector::angleBetween(Vector3f::UNIT_X, Vector3f::UNIT_X), 0.0001f);
    ASSERT_DOUBLES_EQUAL(90.0f, vector::angleBetween(Vector3f::UNIT_X, Vector3f::UNIT_Y), 0.0001f);
    ASSERT_DOUBLES_EQUAL(90.0f, vector::angleBetween(Vector3f::UNIT_Y, Vector3f::UNIT_X), 0.0001f);
    ASSERT_DOUBLES_EQUAL(180.0f, vector::angleBetween(Vector3f::UNIT_X, Vector3f::NEGATIVE_UNIT_X), 0.0001f);
    ASSERT_DOUBLES_EQUAL(180.0f, vector::angleBetween(Vector3f::NEGATIVE_UNIT_X, Vector3f::UNIT_X), 0.0001f);
    ASSERT_DOUBLES_EQUAL(90.0f, vector::angleBetween(Vector3f::UNIT_X, Vector3f::NEGATIVE_UNIT_Y), 0.0001f);
    ASSERT_DOUBLES_EQUAL(90.0f, vector::angleBetween(Vector3f::NEGATIVE_UNIT_X, Vector3f::UNIT_Y), 0.0001f);
    ASSERT_DOUBLES_EQUAL(90.0f, vector::angleBetween(Vector3f::NEGATIVE_UNIT_X, Vector3f::UNIT_Z), 0.0001f);

    Vector2f a(1.0, 2.0);
    ASSERT_EQ(1.5f, vector::mean(a));
    Vector3f b(1.0, 2.0, 3.0);
    ASSERT_EQ(2.0f, vector::mean(b));
    Vector4f c(1.0, 2.0, 3.0, 4.0);
    ASSERT_EQ(2.5f, vector::mean(c));

    Vector<float, 5_z> measurements({2.0f, 4.0f, 5.0f, 7.0f, 7.0f});
    float m = vector::mean(measurements); // (2+4+5+7+7)/5 = 5
    ASSERT_EQ(5.0f, m);
    // Deviation from average = mean - x[i]
    // = [5-2, 5-4, 5-5, 5-7, 5-7]
    // = [3 1 0 2 2]
    Vector<float, 5_z> deviation = m - measurements;
    // Square of the deviation: (mean - x[i])^2
    // = [3^2 1^2 0^2 2^2 2^2]
    Vector<float, 5_z> deviation2 = vector::componentProduct(deviation, deviation);
    // Variance: sum((mean - x[i])^2) / size()
    // = (3^2 + 1^2 + 0^2 + 2^2 + 2^2) / 5
    ASSERT_EQ(3.6f, vector::mean(deviation2));
}

//--------------------------------------------------------------------------
TEST(TestVectors, testDisplay)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    std::cout << Vector3f(1.0f, 2.0f, 3.0f) << std::endl;
    ASSERT_STREQ(buffer.str().c_str(), "[1, 2, 3]\n");
    std::cout.rdbuf(old);
}
#endif
