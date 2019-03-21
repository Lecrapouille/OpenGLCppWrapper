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

#ifndef TRANSFORMATION_HPP
#  define TRANSFORMATION_HPP

// Modifed from the OpenGL Mathematic library (GLM).
// Original code: https://github.com/g-truc/glm/blob/master/glm/gtc/matrix_transform.inl

#  include "Matrix.hpp"
#  include <cassert>

namespace matrix
{
  template<typename T>
  Matrix<T, 4_z, 4_z> translate(Matrix<T, 4_z, 4_z> const &m, Vector<T, 3_z> const &v)
  {
    Matrix<T, 4_z, 4_z> M(m);

    M[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4_z, 4_z> scale(Matrix<T, 4_z, 4_z> const &m, Vector<T, 3_z> const &v)
  {
    Matrix<T, 4_z, 4_z> M;

    M[0] = m[0] * v[0];
    M[1] = m[1] * v[1];
    M[2] = m[2] * v[2];
    M[3] = m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4_z, 4_z> rotate(Matrix<T, 4_z, 4_z> const &m, T const angle, Vector<T, 3_z> const &v)
  {
    T const a = angle;
    T const c = std::cos(a);
    T const s = std::sin(a);

    Vector<T, 3_z> axis(vector::normalize(v));
    Vector<T, 3_z> temp((T(1) - c) * axis);
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

    Matrix<T, 4_z, 4_z> M;
    M[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
    M[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
    M[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
    M[3] = m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4_z, 4_z> ortho(T const left, T const right, T const bottom, T const top)
  {
    Matrix<T, 4_z, 4_z> M(matrix::Identity);

    M[0][0] = T(2) / (right - left);
    M[1][1] = T(2) / (top - bottom);
    M[2][2] = T(-1);
    M[3][0] = -(right + left) / (right - left);
    M[3][1] = -(top + bottom) / (top - bottom);

    return M;
  }

  // *************************************************************************************************
  //! \brief Replace gluPerspective(). Set the frustum to perspective mode.
  //! \param fovY     - Field of vision in degrees in the y direction.
  //! \param aspect   - Aspect ratio of the viewport.
  //! \param zNear    - The near clipping distance.
  //! \param zFar     - The far clipping distance.
  // *************************************************************************************************
  template<typename T>
  Matrix<T, 4_z, 4_z> perspective(T const fovY, T const aspect, T const zNear, T const zFar)
  {
    assert(std::abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

    T const tanHalfFovY = std::tan(fovY / T(2));
    Matrix<T, 4_z, 4_z> M(0);

    M[0][0] = T(1) / (aspect * tanHalfFovY);
    M[1][1] = T(1) / (tanHalfFovY);
    M[2][3] = T(-1);
    M[2][2] = -(zFar + zNear) / (zFar - zNear);
    M[3][2] = -(T(2) * zFar * zNear) / (zFar - zNear);

    return M;
}

  template<typename T>
  Matrix<T, 4_z, 4_z> lookAt(Vector<T, 3_z> const &eye,
                             Vector<T, 3_z> const &center,
                             Vector<T, 3_z> const &up)
  {
    Vector<T, 3_z> const f(vector::normalize(center - eye));
    Vector<T, 3_z> const s(vector::normalize(vector::cross(f, up)));
    Vector<T, 3_z> const u(vector::cross(s, f));
    Matrix<T, 4_z, 4_z> M(matrix::Identity);

    M[0][0] = s.x;
    M[1][0] = s.y;
    M[2][0] = s.z;
    M[0][1] = u.x;
    M[1][1] = u.y;
    M[2][1] = u.z;
    M[0][2] = -f.x;
    M[1][2] = -f.y;
    M[2][2] = -f.z;
    M[3][0] = -(vector::dot(s, eye));
    M[3][1] = -(vector::dot(u, eye));
    M[3][2] = vector::dot(f, eye);

    return M;
  }
}

#endif /* TRANSFORMATION_HPP */
