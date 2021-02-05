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

// Modifed from the OpenGL Mathematic library (GLM).
// Original code: https://github.com/g-truc/glm/blob/master/glm/gtc/matrix_transform.inl

#  include "Math/Matrix.hpp"

#  pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wold-style-cast"
#    pragma GCC diagnostic ignored "-Wfloat-equal"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#      include "units/units.hpp"
#  pragma GCC diagnostic pop

namespace matrix
{
    template<typename T>
    Matrix<T, 4_z, 4_z> translate(Matrix<T, 4_z, 4_z> const& m, Vector<T, 3_z> const& v)
    {
        Matrix<T, 4_z, 4_z> M(m);

        M[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

        return M;
    }

    template<typename T>
    Matrix<T, 4_z, 4_z> scale(Matrix<T, 4_z, 4_z> const& m, Vector<T, 3_z> const& v)
    {
        Matrix<T, 4_z, 4_z> M;

        M[0] = m[0] * v[0];
        M[1] = m[1] * v[1];
        M[2] = m[2] * v[2];
        M[3] = m[3];

        return M;
    }

    //! \brief angle in radian
    template<typename T>
    Matrix<T, 4_z, 4_z> rotate(Matrix<T, 4_z, 4_z> const& m, units::angle::radian_t const angle, Vector<T, 3_z> const& v)
    {
        T const c = std::cos(angle.to<T>());
        T const s = std::sin(angle.to<T>());

        Vector<T, 3_z> axis(vector::normalize(v));
        Vector<T, 3_z> temp((maths::one<T>() - c) * axis);
        Matrix<T, 4_z, 4_z> rotate;

        rotate(0, 0) = c + temp[0] * axis[0];
        rotate(0, 1) = temp[0] * axis[1] + s * axis[2];
        rotate(0, 2) = temp[0] * axis[2] - s * axis[1];

        rotate(1, 0) = temp[1] * axis[0] - s * axis[2];
        rotate(1, 1) = c + temp[1] * axis[1];
        rotate(1, 2) = temp[1] * axis[2] + s * axis[0];

        rotate(2, 0) = temp[2] * axis[0] + s * axis[1];
        rotate(2, 1) = temp[2] * axis[1] - s * axis[0];
        rotate(2, 2) = c + temp[2] * axis[2];

        Matrix<T, 4_z, 4_z> M;
        M[0] = m[0] * rotate(0, 0) + m[1] * rotate(0, 1) + m[2] * rotate(0, 2);
        M[1] = m[0] * rotate(1, 0) + m[1] * rotate(1, 1) + m[2] * rotate(1, 2);
        M[2] = m[0] * rotate(2, 0) + m[1] * rotate(2, 1) + m[2] * rotate(2, 2);
        M[3] = m[3];

        return M;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> ortho(T const left, T const right,
                              T const bottom, T const top,
                              T const near, T const far)
    {
        Matrix<T, 4_z, 4_z> M(matrix::Identity);

        M(0, 0) = T(2) / (right - left);
        M(1, 1) = T(2) / (top - bottom);
        M(2, 2) = T(2) / (far - near);
        M(3, 0) = -(right + left) / (right - left);
        M(3, 1) = -(top + bottom) / (top - bottom);
        M(3, 2) = -(far + near) / (far - near);

        return M;
    }

    //--------------------------------------------------------------------------
    //! \brief Replace gluPerspective(). Set the frustum to perspective mode.
    //! \param fovY     - Field of vision in radians in the y direction.
    //! \param aspect   - Aspect ratio of the viewport.
    //! \param zNear    - The near clipping distance.
    //! \param zFar     - The far clipping distance.
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> perspective(T const fovY, T const aspect, T const zNear, T const zFar)
    {
        assert(maths::abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

        T const tanHalfFovY = std::tan(fovY / T(2));
        Matrix<T, 4_z, 4_z> M(0);

        M(0, 0) = maths::one<T>() / (aspect * tanHalfFovY);
        M(1, 1) = maths::one<T>() / (tanHalfFovY);
        M(2, 3) = -maths::one<T>();
        M(2, 2) = -(zFar + zNear) / (zFar - zNear);
        M(3, 2) = -(T(2) * zFar * zNear) / (zFar - zNear);

        return M;
    }

    //--------------------------------------------------------------------------
    //! \brief Camera looks at.
    //! \param position camera position.
    //! \param target the target position the camera is looking at.
    //! \param up upwards direction of the camera.
    //!
    //!          | Rx Ry Rz 0 |   | 1 0 0 -Px |
    //! LookAt = | Ux Uy Uz 0 | x | 0 1 0 -Py |
    //!          | Dx Dy Dz 0 |   | 0 0 1 -Pz |
    //!          |  0  0  0 1 |   | 0 0 0 1   |
    //!
    //! Where U is the up vector, D is the direction vector, P is the camera's
    //! position vector and R is the right vector (= cross product between U and D).
    //--------------------------------------------------------------------------
    template<typename T>
    Matrix<T, 4_z, 4_z> lookAt(Vector<T, 3_z> const& position,
                               Vector<T, 3_z> const& target,
                               Vector<T, 3_z> const& upwards)
    {
        Vector<T, 3_z> const direction(vector::normalize(target - position));
        Vector<T, 3_z> const right(vector::normalize(vector::cross(direction, upwards)));
        Vector<T, 3_z> const up(vector::cross(right, direction));
        Matrix<T, 4_z, 4_z> M(matrix::Identity);

        M(0, 0) = right.x;
        M(1, 0) = right.y;
        M(2, 0) = right.z;
        M(0, 1) = up.x;
        M(1, 1) = up.y;
        M(2, 1) = up.z;
        M(0, 2) = -direction.x;
        M(1, 2) = -direction.y;
        M(2, 2) = -direction.z;
        M(3, 0) = -(vector::dot(right, position));
        M(3, 1) = -(vector::dot(up, position));
        M(3, 2) = vector::dot(direction, position);

        return M;
    }

    template<typename T>
    Matrix<T, 3_z, 3_z> normalMatrix(Matrix<T, 4_z, 4_z> const& modelViewMatrix)
    {
        return Matrix<T, 3_z, 3_z>(transpose(inverse(modelViewMatrix)));
    }

    template<typename T>
    Matrix<T, 3_z, 3_z> normalMatrix(Matrix<T, 4_z, 4_z> const& modelMatrix,
                                     Matrix<T, 4_z, 4_z> const& viewMatrix)
    {
        return normalMatrix(modelMatrix * viewMatrix);
    }

} // namespace matrix

#endif // OPENGLCPPWRAPPER_TRANSFORMATION_HPP
