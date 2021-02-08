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

#ifndef OPENGLCPPWRAPPER_TRANSFORMATION_HPP
#  define OPENGLCPPWRAPPER_TRANSFORMATION_HPP

// *****************************************************************************
//! \file Collection of Matrix transformation routines adaptated from the OpenGL
//! Mathematic library (GLM).
//!
//! \warning Note that all matrices are transposed to be directly usable by
//!   OpenGL (since OpenGL matrices are column-major. Beware of option passed
//!   to glUniformMatrix4fv that should be set to GL_FALSE). As consequence:
//!   transpose(A * B) = transpose(B) * transpose(A). But to confuse more things
//!   OpenGL convention is still M * x.
//!
//! \note The original code can be found here:
//!   https://github.com/g-truc/glm/blob/master/glm/ext/matrix_transform.inl
//! \see https://learnopengl.com/Getting-started/Transformations
//! \see https://antongerdelan.net/opengl/raycasting.html
// *****************************************************************************

#  include "Math/Matrix.hpp"

namespace matrix
{
    //--------------------------------------------------------------------------
    //! \brief Build a translation 4 * 4 matrix created from a vector of 3
    //! components.
    //!
    //! \param[in] M Input matrix multiplied by this translation matrix.
    //! \param[in] t Coordinates of a translation vector.
    //! \return A transposed matrix.
    //!
    //! \code
    //! If \c M is the identity matrix, the output matrix O will be:
    //!                | 1 0 0 Tx |
    //!                | 0 1 0 Ty |
    //! O = transpose( | 0 0 1 Tz | )
    //!                | 0 0 0 1  |
    //! \endcode
    //--------------------------------------------------------------------------

    template<typename T>
    Matrix<T, 4_z, 4_z> translate(Matrix<T, 4_z, 4_z> const& M,
                                  Vector<T, 3_z> const& t)
    {
        Matrix<T, 4_z, 4_z> O(M);

        O[3] = M[0] * t.x + M[1] * t.y + M[2] * t.z + M[3];

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief Build a scale 4 * 4 matrix created from 3 scalars.
    //!
    //! \param[in] M Input matrix multiplied by this scale matrix.
    //! \param[in] s Ratio of scaling for each axis.
    //! \return A transposed matrix.
    //!
    //! \code
    //! If \c M is the identity matrix, the output matrix O will be:
    //!                 | Sx 0  0  0 |
    //!                 | 0  Sy 0  0 |
    //! O =  transpose( | 0  0  Sz 0 | )
    //!                 | 0  0  0  1 |
    //! \endcode
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> scale(Matrix<T, 4_z, 4_z> const& M,
                              Vector<T, 3_z> const& s)
    {
        Matrix<T, 4_z, 4_z> O;

        O[0] = M[0] * s.x;
        O[1] = M[1] * s.y;
        O[2] = M[2] * s.z;
        O[3] = M[3];

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief Build a rotation 4 * 4 matrix created from an axis vector and an
    //! angle. With positive angle the rotation direction is clockwise due to the
    //! fact that OpenGL matrices are transposed.
    //!
    //! \param[in] M Input matrix multiplied by this rotation matrix.
    //! \param[in] angle Rotation angle expressed in radians.
    //! \param[in] r Rotation axis, recommended to be normalized.
    //! \return A transposed matrix.
    //!
    //! \code
    //! If \c M is the identity matrix, the output matrix O will be:
    //!
    //!                | c+RxRx(1-c)      RxRy(1-c)-Rz.c   RxRy(1-c)+Ry.s   0 |
    //!                | RyRx(1-c)+Rz.s   c+RyRy(1-c)      RyRz(1-c)-Rx.s   0 |
    //! O = transpose( | RzRx(1-c)-Ry.s   RzRy(1-c)+Rx.s   c+RzRz(1-c)      0 | )
    //!                | 0                0                0                1 |
    //!
    //! Where: c = cosinus and s = sinus.
    //! \endcode
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> rotate(Matrix<T, 4_z, 4_z> const& M,
                               units::angle::radian_t const angle,
                               Vector<T, 3_z> const& r)
    {
        T const a = angle.to<T>();
        T const c = std::cos(a);
        T const s = std::sin(a);

        Vector<T, 3_z> axis(vector::normalize(r));
        Vector<T, 3_z> temp((maths::one<T>() - c) * axis);
        Matrix<T, 4_z, 4_z> rotate;

        rotate[0][0] = c + temp[0] * axis[0];
        rotate[0][1] = temp[0] * axis[1] + s * axis[2];
        rotate[0][2] = temp[0] * axis[2] - s * axis[1];

        rotate[1][0] = temp[1] * axis[0] - s * axis[2];
        rotate[1][1] = c + temp[1] * axis[1];
        rotate[1][2] = temp[1] * axis[2] + s * axis[0];

        rotate[2][0] = temp[2] * axis[0] + s * axis[1];
        rotate[2][1] = temp[2] * axis[1] - s * axis[0];
        rotate[2][2] = c + temp[2] * axis[2];

        Matrix<T, 4_z, 4_z> O;
        O[0] = M[0] * rotate[0][0] + M[1] * rotate[0][1] + M[2] * rotate[0][2];
        O[1] = M[0] * rotate[1][0] + M[1] * rotate[1][1] + M[2] * rotate[1][2];
        O[2] = M[0] * rotate[2][0] + M[1] * rotate[2][1] + M[2] * rotate[2][2];
        O[3] = M[3];

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> ortho(T const left, T const right,
                              T const bottom, T const top,
                              T const near, T const far)
    {
        Matrix<T, 4_z, 4_z> O(matrix::Identity);

        O[0][0] = T(2) / (right - left);
        O[1][1] = T(2) / (top - bottom);
        O[2][2] = T(2) / (far - near);
        O[3][0] = -(right + left) / (right - left);
        O[3][1] = -(top + bottom) / (top - bottom);
        O[3][2] = -(far + near) / (far - near);

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief Replace gluPerspective(). Set the frustum to perspective mode.
    //! \param[in] fovY Field of vision in radians in the y direction.
    //! \param[in] aspect Aspect ratio of the viewport.
    //! \param[in] zNear The near clipping distance.
    //! \param[in] zFar The far clipping distance.
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> perspective(T const fovY, T const aspect, T const zNear, T const zFar)
    {
        assert(maths::abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

        T const tanHalfFovY = std::tan(fovY / T(2));
        Matrix<T, 4_z, 4_z> O(0);

        O[0][0] = maths::one<T>() / (aspect * tanHalfFovY);
        O[1][1] = maths::one<T>() / (tanHalfFovY);
        O[2][3] = -maths::one<T>();
        O[2][2] = -(zFar + zNear) / (zFar - zNear);
        O[3][2] = -(T(2) * zFar * zNear) / (zFar - zNear);

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief Build a look at view matrix based on the default handedness.
    //!
    //! \param[in] position Position of the camera
    //! \param[in] target Position where the camera is looking at
    //! \param[in] upwards Normalized up vector, how the camera is oriented.
    //!   Typically (0, 0, 1)
    //! \return A transposed matrix.
    //!
    //! \code
    //!                     | Rx Ry Rz 0 |   | 1 0 0 -Px |
    //! LookAt = transpose( | Ux Uy Uz 0 | x | 0 1 0 -Py | )
    //!                     | Dx Dy Dz 0 |   | 0 0 1 -Pz |
    //!                     |  0  0  0 1 |   | 0 0 0 1   |
    //!
    //! Where:
    //!   - U is the up vector,
    //!   - D is the direction vector,
    //!   - R is the right vector (= cross product between U and D).
    //!   - P is the camera's position vector.
    //! \endcode
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> lookAt(Vector<T, 3_z> const& position,
                               Vector<T, 3_z> const& target,
                               Vector<T, 3_z> const& upwards)
    {
        Vector<T, 3_z> const direction(vector::normalize(target - position));
        Vector<T, 3_z> const right(vector::normalize(vector::cross(direction, upwards)));
        Vector<T, 3_z> const up(vector::cross(right, direction));
        Matrix<T, 4_z, 4_z> O(matrix::Identity);

        O[0][0] = right.x;
        O[1][0] = right.y;
        O[2][0] = right.z;
        O[0][1] = up.x;
        O[1][1] = up.y;
        O[2][1] = up.z;
        O[0][2] = -direction.x;
        O[1][2] = -direction.y;
        O[2][2] = -direction.z;
        O[3][0] = -(vector::dot(right, position));
        O[3][1] = -(vector::dot(up, position));
        O[3][2] = vector::dot(direction, position);

        return O;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 3_z, 3_z> normalMatrix(Matrix<T, 4_z, 4_z> const& modelViewMatrix)
    {
        return Matrix<T, 3_z, 3_z>(transpose(inverse(modelViewMatrix)));
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 3_z, 3_z> normalMatrix(Matrix<T, 4_z, 4_z> const& modelMatrix,
                                     Matrix<T, 4_z, 4_z> const& viewMatrix)
    {
        return normalMatrix(modelMatrix * viewMatrix);
    }

} // namespace matrix

#endif // OPENGLCPPWRAPPER_TRANSFORMATION_HPP
