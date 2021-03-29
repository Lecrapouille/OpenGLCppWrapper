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
//#define protected public
//#define private public
//#  pragma GCC diagnostic push
//#  pragma GCC diagnostic ignored "-Wfloat-equal"
#    include "Math/Transformation.hpp"
//#  pragma GCC diagnostic pop
//#undef protected
//#undef private

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void check_matrix(Matrix<T,r,c> const &a, Matrix<T,r,c> const &b)
{
    for (size_t i = 0_z; i < r * c; ++i)
    {
std::cout << a.m_data[i] << " vs " << b.m_data[i] << std::endl;
        ASSERT_EQ(true, std::abs(a.m_data[i] - b.m_data[i]) < 0.0001f);
    }
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testTranslate)
{
    Matrix44f I(matrix::Identity);
    Matrix44f M = matrix::translate(I, Vector3f(2.0f, 3.0f, 4.0f));

    check_matrix(M, Matrix44f({1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f,
                               2.0f, 3.0f, 4.0f, 1.0f}));
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testScale)
{
    Matrix44f I(matrix::Identity);
    Matrix44f M = matrix::scale(I, Vector3f(2.0f, 3.0f, 4.0f));

    check_matrix(M, Matrix44f({2.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 3.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 4.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f}));
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testRotation)
{
    const units::angle::degree_t angle(45.0f);
    const float a = angle.to<float>();
    const float c = cos(a);
    const float s = cos(a);
    const float Rz = 0.371391f;
    const float Ry = 0.557086f;
    const float Rx = 0.742781f;

    Matrix44f I(matrix::Identity);
    Matrix44f M = matrix::rotate(I, angle, Vector3f(Rx, Ry, Rz));

Vector3f q(Rx, Ry, Rz);
std::cout << vector::normalize(q) << std::endl;
std::cout << "M: " << M << std::endl;
std::cout << "QQ: " << Matrix44f({
                //
                Rx * Rx * (1.0f - c) + c,
                Rx * Ry * (1.0f - c) + Rz * s,
                Rx * Ry * (1.0f - c) - Ry * s,
                0.0f,
                //
                Rx * Ry * (1.0f - c) - Rz * s,
                Ry * Ry * (1.0f - c) + c,
                Ry * Rz * (1.0f - c) + Rx * s,
                0.0f,
                //
                Rx * Rz * (1.0f - c) + Ry * s,
                Ry * Rz * (1.0f - c) - Rx * s,
                Rz * Rz * (1.0f - c) + c,
                0.0f,
                //
                0.0f, 0.0f, 0.0f, 1.0f}) << std::endl;

    check_matrix(M, Matrix44f({
                //
                c + Rx * Rx * (1.0f - c),
                Rx * Ry * (1.0f - c) - Rz * c,
                Rx * Ry * (1.0f - c) + Ry * s,
                0.0f,
                //
                Ry * Rx * (1.0f - c) + Rz * s,
                c + Ry * Ry * (1.0f - c),
                Ry * Rz * (1.0f - c) - Rx * s,
                0.0f,
                //
                Rz * Rx * (1.0f - c) - Ry * s,
                Rz * Ry * (1.0f - c) + Rx * s,
                c + Rz * Rz * (1.0f - c),
                0.0f,
                //
                0.0f, 0.0f, 0.0f, 1.0f}));
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testOrtho)
{
    const float left = 1.0f; const float right = 2.0f;
    const float bottom = 3.0f; const float top = 4.0f;
    const float near = 5.0f; const float far = 6.0f;

    Matrix44f M = matrix::ortho(left, right, bottom, top, near, far);

std::cout << "M: " << M << std::endl;
std::cout << "QQ: " << Matrix44f({
                //
                2.0f / (right - left),
                0.0f,
                0.0f,
                -(right + left) / (right - left),
                //
                0.0f,
                2.0f / (top - bottom),
                0.0f,
                -(top + bottom) / (top - bottom),
                //
                0.0f,
                0.0f,
                2.0f / (far - near),
                -(far + near) / (far - near),
                //
                0.0f, 0.0f, 0.0f, 1.0f}) << std::endl;

    check_matrix(M, Matrix44f({
                //
                2.0f / (right - left),
                0.0f,
                0.0f,
                -(right + left) / (right - left),
                //
                0.0f,
                2.0f / (top - bottom),
                0.0f,
                -(top + bottom) / (top - bottom),
                //
                0.0f,
                0.0f,
                2.0f / (far - near),
                -(far + near) / (far - near),
                //
                0.0f, 0.0f, 0.0f, 1.0f}));
}
