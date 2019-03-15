// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef MOVABLE_TPP_
#  define MOVABLE_TPP_

#  include "Transformation.tpp"

// *************************************************************************************************
//! \brief a Movable defines a 4x4 transformation matrix from a translation, a rotation and a scale.
//! This class allow an object to move in a 2D-world (if n == 2U) or 3D-world (if n == 3U).
//! Not a thread safe class.
// *************************************************************************************************
template <typename T, uint32_t n>
class Movable
{
public:

  //! \brief Empty construction. Set an identity 4x4 transformation matrix.
  Movable()
    : m_origin(0),
      m_position(0),
      m_scale(1),
      m_rot_axis(1),
      m_rot_angle(0),
      m_transform(matrix::Identity),
      m_inverse_transform(matrix::Identity),
      m_to_update(false),
      m_inv_to_update(false)
  {
  }

  //! \brief Reset states. Set an identity 4x4 transformation matrix.
  inline void reset()
  {
    const Vector<T, n> zero(0);
    const Vector<T, n> one(1);

    m_origin = zero;
    m_position = zero;
    m_rot_axis = one;
    m_rot_angle = T(0);
    m_scale = one;
    matrix::identity(m_transform);
    matrix::identity(m_inverse_transform);
    m_to_update = false;
    m_inv_to_update = false;
  }

  //! \brief Set the position of the object.
  inline void position(Vector<T, n> const &position)
  {
    m_position = position;
    m_to_update = true;
  }

  //! \brief Get the position of the object.
  inline Vector<T, n> const &position() const
  {
    return m_position;
  }

  //! \brief Set the local origin of the object.
  inline void origin(Vector<T, n> const &origin)
  {
    m_origin = origin;
    m_to_update = true;
  }

  //! \brief Get the local origin of the object.
  inline Vector<T, n> const &origin() const
  {
    return m_origin;
  }

  //! \brief Set scale factors of the object.
  inline void scale(Vector<T, n> const &scale)
  {
    m_scale = scale;
    m_to_update = true;
  }

  //! \brief Get scale factors of the object.
  inline Vector<T, n> const &scale() const
  {
    return m_scale;
  }

  //! \brief Set the orientation of the object.
  inline void rotate(T const angle, Vector<T, n> const &v)
  {
    m_rot_angle = angle;
    m_rot_axis = v;
    m_to_update = true;
  }

  inline Vector<T, n + 1u> rotation()
  {
    // FIXME Make x, y, z implicit like Vector<T, n + 1u> r(m_rot_axis, m_rot_angle);
    Vector<T, n + 1u> r(m_rot_axis.x, m_rot_axis.y, m_rot_axis.z, m_rot_angle);
    return r;
  }

  //! \brief Set the orientation of the object.
  inline void rotate(T const angle)
  {
    m_rot_angle += angle;
    // TODO m_rot_angle = maths::wrapTo2PI(m_rot_angle);
    m_to_update = true;
  }

  //! \brief Move the object by a given offset.
  inline void move(Vector<T, n> const &offset) //FIXME: renommer translate
  {
    m_position += offset;
    m_to_update = true;
  }

  inline void scalefactor(Vector<T, n> const &factor)
  {
    m_scale *= factor;
    m_to_update = true;
  }

  //! \brief Return the 4x4 transform matrix combining the
  //! position/rotation/scale/origin of the object.
  //!
  //! Operation:
  //!   Transform = TranslationMatrix * RotationMatrix * ScaleMatrix;
  //!   TransformedVector = Transform * OriginalVector;
  //! Note:
  //!   Be careful of operation order: we apply scale first, then
  //!   the rotation then the translation.
  Matrix<T, n + 1U, n + 1U> const &transform()
  {
    if (m_to_update)
      {
        Matrix<T,n+1U,n+1U> I(matrix::Identity);
        m_transform = matrix::translate(I, m_position - m_origin);
        m_transform = matrix::rotate(m_transform, m_rot_angle, m_rot_axis);
        m_transform = matrix::scale(m_transform, m_scale);
        m_to_update = false;
        m_inv_to_update = true;
      }

    return m_transform;
  }

  //! \brief Return the 4x4 inverse transform matrix.
  Matrix<T, n + 1U, n + 1U> const &invTransform()
  {
    if (m_inv_to_update)
      {
        // TODO m_inverse_transform = matrix::inverse(transform());
        m_inv_to_update = false;
      }
    return m_inverse_transform;
  }

protected:

  Vector<T, n> m_origin;
  Vector<T, n> m_position;
  Vector<T, n> m_scale;
  Vector<T, n> m_rot_axis;
  T m_rot_angle;
  Matrix<T, n + 1U, n + 1U> m_transform;
  Matrix<T, n + 1U, n + 1U> m_inverse_transform;
  bool m_to_update;
  bool m_inv_to_update;
};

#endif
