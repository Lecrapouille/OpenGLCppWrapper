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

#ifndef OPENGLCPPWRAPPER_TRANSFORMABLE_HPP
#  define OPENGLCPPWRAPPER_TRANSFORMABLE_HPP

#  include "Scene/Transformation.hpp"

namespace glwrap
{

// *************************************************************************************************
//! \brief a Transformable defines a 4x4 transformation matrix from a translation, a rotation and a scale.
//! This class allow an object to move in a 2D-world (if n == 2U) or 3D-world (if n == 3U).
//! Not a thread safe class.
// *************************************************************************************************
template <typename T, size_t n>
class Transformable
{
public:

  //! \brief Empty constructor.
  Transformable()
    : m_origin(maths::zero<T>()),
      m_position(maths::zero<T>()),
      m_scale(maths::one<T>()),
      m_local_scaling(maths::one<T>()),
      m_axis(maths::one<T>()),
      m_transform(matrix::Identity),
      m_inverse_transform(matrix::Identity),
      m_angle(maths::zero<T>()),
      m_transform_needs_update(false),
      m_inverse_trans_needs_update(false)
  {
  }

  //! \brief Restore states to default.
  inline void reset()
  {
    const Vector<T, n> Vz(maths::zero<T>());
    const Vector<T, n> Vo(maths::one<T>());
    const T z = maths::zero<T>();

    m_origin = Vz;
    m_position = Vz;
    m_axis = Vo;
    m_angle = z;
    m_scale = Vo;
    m_local_scaling = Vo;
    matrix::identity(m_transform);
    matrix::identity(m_inverse_transform);
    m_transform_needs_update = false;
    m_inverse_trans_needs_update = false;
  }

  //! \brief Set the origin of the object (relative to word origin).
  inline void origin(Vector<T, n> const& origin)
  {
    m_origin = origin;
    m_transform_needs_update = true;
  }

  //! \brief Get the origin of the object (relative to word origin).
  inline Vector<T, n> const& origin() const
  {
    return m_origin;
  }

  //! \brief Set the relative position of the object from its own origin.
  inline void position(Vector<T, n> const& position)
  {
    m_position = position;
    m_transform_needs_update = true;
  }

  //! \brief Get the absolute position of the object from its own origin.
  inline Vector<T, n> const& position() const
  {
    return m_position;
  }

  //! \brief Move the object by a given offset.
  //! Do the same job than translate() or displace().
  inline void move(Vector<T, n> const& offset)
  {
    m_position += offset;
    m_transform_needs_update = true;
  }

  //! \brief Move the object by a given offset.
  //! Do the same job than move() or displace().
  inline void translate(Vector<T, n> const& offset)
  {
    m_position += offset;
    m_transform_needs_update = true;
  }

  //! \brief Move the object by a given offset.
  //! Do the same job than translate() or move().
  inline void displace(Vector<T, n> const& offset)
  {
    m_position += offset;
    m_transform_needs_update = true;
  }

  //! \brief Set the absolute scale factor of the object.
  inline void scaling(Vector<T, n> const& scale)
  {
    m_scale = scale;
    m_transform_needs_update = true;
  }

  //! \brief Get the absolute scale factors of the object.
  inline Vector<T, n> const& scaling() const
  {
    return m_scale;
  }

  //! \brief Relative scaling of the object.
  inline void scale(Vector<T, n> const& factor)
  {
    m_scale *= factor;
    m_transform_needs_update = true;
  }

  //-----------------------------------------------------------------
  //! \brief Set a local scaling of the object. By local we mean that
  //! will not impact children (they will not be scaled).
  //! \note for scaling children use methods Transformable::scale or
  //! Transformable::scaleFactor.
  //-----------------------------------------------------------------
  inline void localScale(Vector<T, n> const &scale)
  {
    m_local_scaling = scale;
  }

  //-----------------------------------------------------------------
  //! \brief Get the local scaling.
  //-----------------------------------------------------------------
  inline Vector<T, n> const &localScale() const
  {
    return m_local_scaling;
  }

  //! \brief Set the absolute orientation of the object.
  //! \param angle in radian.
  inline void rotation(T const angle, Vector<T, n> const& axis)
  {
    m_angle = angle;
    m_axis = axis;
    m_transform_needs_update = true;
  }

  //! \brief Set the absolute orientation of the object.
  inline void rotation(Vector<T, n> const& axis)
  {
    m_axis = axis;
    m_transform_needs_update = true;
  }

  //! \brief Set the absolute orientation of the object.
  inline Vector<T, n> const& rotation() const
  {
    return m_axis;
  }

  //! \brief Set the relative orientation of the object.
  //! \param angle in radian.
  inline void rotate(T const angle)
  {
    m_angle += angle;
    m_angle = maths::wrapTo2PI(m_angle);
    m_transform_needs_update = true;
  }

  //! \brief Set the relative orientation of the object.
  //! \param angle in radian.
  inline void rotate(T const angle, Vector<T, n> const& axis)
  {
    m_angle += angle;
    m_angle = maths::wrapTo2PI(m_angle);
    m_axis = axis;
    m_transform_needs_update = true;
  }

  //! \brief Set the relative orientation of the object.
  //! \param angle in radian.
  inline void rotateX(T const angle)
  {
    rotate(angle, Vector<T, n>::UNIT_X);
  }

  //! \brief Set the relative orientation of the object.
  //! \param angle in radian.
  inline void rotateY(T const angle)
  {
    rotate(angle, Vector<T, n>::UNIT_Y);
  }

  //! \brief Set the relative orientation of the object.
  //! \param angle in radian.
  inline void rotateZ(T const angle)
  {
    rotate(angle, Vector<T, n>::UNIT_Z);
  }

  //! \brief Set the absolute orientation angle (in radian).
  inline T const& angle() const
  {
    return m_angle;
  }

  Vector3f direction()
  {
    Matrix44f& m = transform();
    Vector3f v(-m[0][2], -m[1][2], -m[2][2]);
    return vector::normalize(v);
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
  Matrix<T, n + 1_z, n + 1_z> const& transform()
  {
    if (unlikely(m_transform_needs_update))
      {
        Matrix<T, n+1_z, n+1_z> I(matrix::Identity);
        m_transform = matrix::translate(I, m_position - m_origin);
        m_transform = matrix::rotate(m_transform, m_angle, m_axis);
        m_transform = matrix::scale(m_transform, m_scale);
        m_transform_needs_update = false;
        m_inverse_trans_needs_update = true;
      }

    return m_transform;
  }

  //! \brief Return the 4x4 inverse transform matrix.
  Matrix<T, n + 1_z, n + 1_z> const& invTransform()
  {
    if (unlikely(m_inverse_trans_needs_update))
      {
        if (unlikely(!matrix::inverse(transform(), m_inverse_transform)))
          {
            ERROR("Failed to inverse the matrix");
            matrix::identity(m_inverse_transform);
          }
        m_inverse_trans_needs_update = false;
      }
    return m_inverse_transform;
  }

protected:

  //! \brief Position of the origin relative to the origin of the world.
  Vector<T, n> m_origin;
  //! \brief Position of the object relative to its origin.
  Vector<T, n> m_position;
  //! \brief Relative scaling of the object relative to the world.
  Vector<T, n> m_scale;
  //! \brief Scaling factors for the object.
  Vector<T, n> m_local_scaling;
  //! \brief Axis rotation of the object.
  Vector<T, n> m_axis;
  //! \brief
  Matrix<T, n + 1_z, n + 1_z> m_transform;
  Matrix<T, n + 1_z, n + 1_z> m_inverse_transform;
  //! \brief Angle of rotation of the object.
  T m_angle;
  bool m_transform_needs_update;
  bool m_inverse_trans_needs_update;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_TRANSFORMABLE_HPP
