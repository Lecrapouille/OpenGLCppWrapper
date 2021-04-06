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
#define ASSERT_MATRIX_NEAR(expected, actual, thresh)                    \
    for (size_t idx = 0; idx < 16u; ++idx)                              \
    {                                                                   \
        ASSERT_NEAR(expected.data()[idx], actual.data()[idx], thresh) << "at index: " << idx; \
    }

//--------------------------------------------------------------------------
#define ASSERT_MATRIX(expected, actual)                                 \
    for (size_t idx = 0; idx < 16u; ++idx)                              \
    {                                                                   \
        ASSERT_EQ(expected.data()[idx], actual.data()[idx]) << "at index: " << idx; \
    }

//--------------------------------------------------------------------------
TEST(TestTransformation, testTranslate)
{
    Matrix44f I(matrix::Identity);
    Matrix44f M = matrix::translate(I, Vector3f(2.0f, 3.0f, 4.0f));

    ASSERT_MATRIX(M, Matrix44f({1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f,
                                2.0f, 3.0f, 4.0f, 1.0f}));
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testScale)
{
    Matrix44f I(matrix::Identity);
    Matrix44f M = matrix::scale(I, Vector3f(2.0f, 3.0f, 4.0f));

    ASSERT_MATRIX(M, Matrix44f({2.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 3.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 4.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f}));
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testRotation)
{
    const units::angle::radian_t angle(units::angle::degree_t(45.0f));
    const float c = cos(angle.to<float>());
    const float s = cos(angle.to<float>());
    const float oc = 1.0f - c;
    const Vector3f axis(0.371391f, 0.557086f, 0.742781f);
    Matrix44f I(matrix::Identity);

    // Actual
    Matrix44f A = matrix::rotate(I, angle, axis);

    // Expected https://fr.wikipedia.org/wiki/Matrice_de_rotation
    Matrix44f E = {
        //
        axis[0] * axis[0] * oc + c,
        axis[0] * axis[1] * oc + axis[2] * s,
        axis[0] * axis[2] * oc - axis[1] * s,
        0.0f,

        //
        axis[0] * axis[1] * oc - axis[2] * s,
        axis[1] * axis[1] * oc + c,
        axis[1] * axis[2] * oc + axis[0] * s,
        0.0f,

        //
        axis[0] * axis[2] * oc + axis[1] * s,
        axis[1] * axis[2] * oc - axis[0] * s,
        axis[2] * axis[2] * oc + c,
        0.0f,

        //
        0.0f, 0.0f, 0.0f, 1.0f
    };

    ASSERT_MATRIX_NEAR(A, E, 0.0001f);
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testOrtho)
{
    const float left = 1.0f; const float right = 2.0f;
    const float bottom = 3.0f; const float top = 4.0f;
    const float near = 5.0f; const float far = 6.0f;

    // Actual
    Matrix44f A = matrix::ortho(left, right, bottom, top, near, far);

    // Expected (glm)
    Matrix44f E(matrix::Identity);
    E[0][0] = 2.0f / (right - left);
    E[1][1] = 2.0f / (top - bottom);
    E[2][2] = 2.0f / (far - near);
    E[3][0] = -(right + left) / (right - left);
    E[3][1] = -(top + bottom) / (top - bottom);
    E[3][2] = -(far + near) / (far - near);

    ASSERT_MATRIX_NEAR(A, E, 0.0001f);
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testPersp)
{
    units::angle::radian_t const fov(units::angle::degree_t(45.0f));
    float const aspect = 800.0f / 600.0f;
    float const near = 0.1f;
    float const far = 100.0f;
    float const tanHalfFovY = std::tan(fov.to<float>() / 2.0f);

    // Actual
    Matrix44f A = matrix::perspective(fov, aspect, near, far);

    // Expected (glm)
    Matrix44f E(0.0f);
    E[0][0] = 1.0f / (aspect * tanHalfFovY);
    E[1][1] = 1.0f / (tanHalfFovY);
    E[2][3] = -1.0f;
    E[2][2] = -(far + near) / (far - near);
    E[3][2] = -(2.0f * far * near) / (far - near);

    ASSERT_MATRIX_NEAR(A, E, 0.0001f);
}

//--------------------------------------------------------------------------
TEST(TestTransformation, testLookAt)
{
    const Vector3f position(1.0f, 1.0f, 1.0f);
    const Vector3f target(0.0f, 0.0f, 0.0f);
    const Vector3f upwards(0.0f, 0.0f, 1.0f);

    // Actual
    Matrix44f A = matrix::lookAt(position, target, upwards);

    // Expected
    Matrix44f E(matrix::Identity);
    Vector3f const direction(vector::normalize(target - position));
    Vector3f const right(vector::normalize(vector::cross(direction, upwards)));
    Vector3f const up(vector::cross(right, direction));

    E[0][0] = right.x;
    E[1][0] = right.y;
    E[2][0] = right.z;
    E[0][1] = up.x;
    E[1][1] = up.y;
    E[2][1] = up.z;
    E[0][2] = -direction.x;
    E[1][2] = -direction.y;
    E[2][2] = -direction.z;
    E[3][0] = -(vector::dot(right, position));
    E[3][1] = -(vector::dot(up, position));
    E[3][2] = vector::dot(direction, position);

    ASSERT_MATRIX_NEAR(A, E, 0.0001f);
}
