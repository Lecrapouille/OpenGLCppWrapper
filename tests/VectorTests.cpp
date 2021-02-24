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
static void CHECK_VECTOR3B(Vector3b const& v, const bool x, const bool y, const bool z)
{
    ASSERT_EQ(x, v.x);
    ASSERT_EQ(y, v.y);
    ASSERT_EQ(z, v.z);
}

//--------------------------------------------------------------------------
static void CHECK_VECTOR3F(Vector3f const& v, const float x, const float y, const float z)
{
    ASSERT_EQ(x, v.x);
    ASSERT_EQ(y, v.y);
    ASSERT_EQ(z, v.z);
}

//--------------------------------------------------------------------------
static void CHECK_NAN_VECTOR3F(Vector3f const& v)
{
    ASSERT_EQ(true, std::isnan(v.x));
    ASSERT_EQ(true, std::isnan(v.y));
    ASSERT_EQ(true, std::isnan(v.z));
}

//--------------------------------------------------------------------------
TEST(TestVectors, testCreator)
{
    // Check sizeof
    ASSERT_EQ(2u * sizeof (float), sizeof (Vector2f));
    ASSERT_EQ(2u * sizeof (double), sizeof (Vector2g));
    ASSERT_EQ(2u * sizeof (int), sizeof (Vector2i));

    ASSERT_EQ(3u * sizeof (float), sizeof (Vector3f));
    ASSERT_EQ(3u * sizeof (double), sizeof (Vector3g));
    ASSERT_EQ(3u * sizeof (int), sizeof (Vector3i));

    ASSERT_EQ(4u * sizeof (float), sizeof (Vector4f));
    ASSERT_EQ(4u * sizeof (double), sizeof (Vector4g));
    ASSERT_EQ(4u * sizeof (int), sizeof (Vector4i));

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
    CHECK_VECTOR3F(v2, 1.0f, 2.0f, 3.0f);
    CHECK_VECTOR3F(v3, 1.0f, 2.0f, 0.0f);
    CHECK_VECTOR3F(v4, 4.0f, 5.0f, 6.0f);
    CHECK_VECTOR3F(v5, 1.0f, 2.0f, 3.0f);
    CHECK_VECTOR3F(v6, -4.0f, 5.0f, -6.0f);
    CHECK_VECTOR3F(v7, 1.0f, 2.0f, 0.0f);
    CHECK_VECTOR3F(v8, 42.0f, 42.0f, 42.0f);
    CHECK_NAN_VECTOR3F(dummy);

    // Check the union [x,y,z] == [r,g,b]
    ASSERT_EQ(v1.r, v1.x);
    ASSERT_EQ(v1.g, v1.y);
    ASSERT_EQ(v1.b, v1.z);
    CHECK_VECTOR3F(v2, v2[0], v2[1], v2[2]);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testPredefined)
{
    CHECK_NAN_VECTOR3F(Vector3f::DUMMY);
    //CHECK_VECTOR3F(Vector3f::POSITIVE_INFINITY, maths::inf<float>(), maths::inf<float>(), maths::inf<float>());
    //CHECK_VECTOR3F(Vector3f::NEGATIVE_INFINITY, -maths::inf<float>(), -maths::inf<float>(), -maths::inf<float>());

    CHECK_VECTOR3F(Vector3f::ZERO, 0.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::ONE, 1.0f, 1.0f, 1.0f);

    CHECK_VECTOR3F(Vector3f::UNIT_SCALE, 1.0f, 1.0f, 1.0f);
    CHECK_VECTOR3F(Vector3f::NEGATIVE_UNIT_SCALE, -1.0f, -1.0f, -1.0f);
    CHECK_VECTOR3F(Vector3f::UNIT_X, 1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::UNIT_Y, 0.0f, 1.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::UNIT_Z, 0.0f, 0.0f, 1.0f);
    CHECK_VECTOR3F(Vector3f::NEGATIVE_UNIT_X, -1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::NEGATIVE_UNIT_Y, 0.0f, -1.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::NEGATIVE_UNIT_Z, 0.0f, 0.0f, -1.0f);

    CHECK_VECTOR3F(Vector3f::LEFT, -1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::RIGHT, 1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::BACK, 0.0f, 0.0f, -1.0f);
    CHECK_VECTOR3F(Vector3f::FORWARD, 0.0f, 0.0f, 1.0f);
    CHECK_VECTOR3F(Vector3f::DOWN, 0.0f, -1.0f, 0.0f);
    CHECK_VECTOR3F(Vector3f::UP, 0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testSwap)
{
    vector::swap(v2, v4);
    CHECK_VECTOR3F(v2, 4.0f, 5.0f, 6.0f);
    vector::swap(v2, v4);
    CHECK_VECTOR3F(v2, 1.0f, 2.0f, 3.0f);
    vector::swap(v4, v2);
    CHECK_VECTOR3F(v2, 4.0f, 5.0f, 6.0f);
    vector::swap(v4, v2);
    CHECK_VECTOR3F(v2, 1.0f, 2.0f, 3.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testEquality)
{
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
}

//--------------------------------------------------------------------------
TEST(TestVectors, testArithmetic)
{
    const float c_scalar = -2.0f;
    float scalar = -2.0f;

    // Addition, substraction
    CHECK_VECTOR3F((v2 + v5 + v3), 3.0f, 6.0f, 6.0f);
    CHECK_VECTOR3F(Vector3f::ZERO + 4.0f, 4.0f, 4.0f, 4.0f);
    CHECK_VECTOR3F(Vector3f::UNIT_X - Vector3f::UNIT_X, 0.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(-Vector3f::UNIT_X, -1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(+Vector3f::UNIT_X, +1.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(-v2, -1.0f, -2.0f, -3.0f);

    // Multiplication
    CHECK_VECTOR3F((v5 * 2.0f), 2.0f, 4.0f, 6.0f);
    CHECK_VECTOR3F((v5 * -2.0f), -2.0f, -4.0f, -6.0f);
    CHECK_VECTOR3F((-2.0f * v5), -2.0f, -4.0f, -6.0f);
    CHECK_VECTOR3F((c_scalar * v5), -2.0f, -4.0f, -6.0f);
    CHECK_VECTOR3F((-v5 * 2.0f), -2.0f, -4.0f, -6.0f);
    CHECK_VECTOR3F((-v5 * c_scalar), 2.0f, 4.0f, 6.0f);
    CHECK_VECTOR3F((-v5 * scalar), 2.0f, 4.0f, 6.0f);

    // Division
    CHECK_VECTOR3F((v5 / 2.0f), 0.5f, 1.0f, 3.0f/2.0f);
    CHECK_VECTOR3F((v5 / -2.0f), -0.5f, -1.0f, -3.0f/2.0f);
    CHECK_VECTOR3F((-2.0f / v5), -2.0f, -1.0f, -2.0f/3.0f);
    CHECK_VECTOR3F((c_scalar / v5), -2.0f, -1.0f, -2.0f/3.0f);
    CHECK_VECTOR3F((scalar / v5), -2.0f, -1.0f, -2.0f/3.0f);
    CHECK_VECTOR3F((-v5 / 2.0f), -0.5f, -1.0f, -3.0f/2.0f);
    CHECK_VECTOR3F((-v5 / c_scalar), 0.5f, 1.0f, 3.0f/2.0f);
    CHECK_VECTOR3F((-v5 / scalar), 0.5f, 1.0f, 3.0f/2.0f);

    //
    static Vector3f v(v2);
    v += 1.0f;
    CHECK_VECTOR3F(v, 2.0f, 3.0f, 4.0f);
    v += 1.0f;
    CHECK_VECTOR3F(v, 3.0f, 4.0f, 5.0f);
    v -= 2.0f;
    CHECK_VECTOR3F(v, 1.0f, 2.0f, 3.0f);
    v /= 2.0f;
    CHECK_VECTOR3F(v, 1.0f / 2.0f, 1.0f, 3.0f / 2.0f);
    v *= 2.0f;
    CHECK_VECTOR3F(v, 1.0f, 2.0f, 3.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testCopy)
{
    v1 = v5;
    CHECK_VECTOR3F(v1, 1.0f, 2.0f, 3.0f);
    //v1 = 42.0f;
    //CHECK_VECTOR3F(v1, 42.0f, 42.0f, 42.0f);
}

//--------------------------------------------------------------------------
TEST(TestVectors, testOperations)
{
    // Min, max, clamp, abs
    CHECK_VECTOR3F(vector::abs(v6), 4.0f, 5.0f, 6.0f);
    CHECK_VECTOR3F(vector::min(v2, dummy), v2.x, v2.y, v2.z);
    CHECK_VECTOR3F(vector::max(v2, dummy), v2.x, v2.y, v2.z);
    CHECK_VECTOR3F(vector::min(v2, Vector3f::UNIT_Z + 1.0f), 1.0f, 1.0f, 2.0f);
    CHECK_VECTOR3F(vector::max(v2, Vector3f::UNIT_Y + 2.0f), 2.0f, 3.0f, 3.0f);
    CHECK_VECTOR3F(vector::clamp(v6, -5.0f, 3.0f), -4.0f, 3.0f, -5.0f);
    CHECK_VECTOR3F(vector::clamp(vector::abs(v6), -3.0f, 5.0f), 4.0f, 5.0f, 5.0f);

    // Middle Point
    CHECK_VECTOR3F(vector::middle(Vector3f::ZERO, Vector3f::UNIT_Z), 0.0f, 0.0f, 0.5f);
    CHECK_VECTOR3F(vector::middle(Vector3f::ZERO, Vector3f::UNIT_Y), 0.0f, 0.5f, 0.0f);
    CHECK_VECTOR3F(vector::middle(Vector3f::ZERO, Vector3f::UNIT_X), 0.5f, 0.0f, 0.0f);
    CHECK_VECTOR3F(vector::middle(Vector3f::UNIT_X, -Vector3f::UNIT_X), 0.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(vector::middle(-Vector3f::UNIT_X, Vector3f::UNIT_X), 0.0f, 0.0f, 0.0f);
    CHECK_VECTOR3F(-vector::middle(Vector3f::UNIT_X, Vector3f::UNIT_X), -1.0f, 0.0f, 0.0f);

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
