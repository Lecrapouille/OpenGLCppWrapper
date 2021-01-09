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

#  include "Math/Transformation.hpp"
#  include "Math/Quaternion.hpp"
#include <iostream>

// *****************************************************************************
//! \brief a Transformable defines a 4x4 transformation matrix from a given
//! translation, a given rotation and a given scale factor. This class allows an
//! object to move inside a 2D-world (if n == 2U) or 3D-world (if n == 3U).
//! The returned matrix will be given to OpenGL shader for rendering the object
//! inside the world.
// *****************************************************************************
template <typename T, size_t n>
class Transformable
{
public:

    //--------------------------------------------------------------------------
    //! \brief Empty constructor. Init position and origin to 0.
    //! Init scale to 1. Init to transformation to identity matrix.
    //--------------------------------------------------------------------------
    Transformable()
        : m_origin(maths::zero<T>()),
          m_position(maths::zero<T>()),
          m_scale(maths::one<T>()),
          m_local_scaling(maths::one<T>()),
          m_transform(matrix::Identity),
          m_inverse_transform(matrix::Identity),
          m_transform_needs_update(false),
          m_inverse_trans_needs_update(false)
    {}

    //--------------------------------------------------------------------------
    //! \brief Restore states to default. Init position and origin to 0.
    //! Init scale to 1. Init to transformation to identity matrix.
    //--------------------------------------------------------------------------
    void reset()
    {
        m_origin = Vector<T, n>::ZERO;
        m_position = Vector<T, n>::ZERO;
        m_orientation = Quat<T>();
        m_scale = Vector<T, n>::ONE;
        m_local_scaling = Vector<T, n>::ONE;
        matrix::identity(m_transform);
        matrix::identity(m_inverse_transform);
        m_transform_needs_update = false;
        m_inverse_trans_needs_update = false;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the right vector.
    //--------------------------------------------------------------------------
    const Vector<T, n> right() const
    {
        return Vector<T, n>(rotation()[0]);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the up vector.
    //--------------------------------------------------------------------------
    const Vector<T, n> up() const
    {
        return Vector<T, n>(rotation()[1]);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the forward vector.
    //--------------------------------------------------------------------------
    const Vector<T, n> forward() const
    {
        return Vector<T, n>(rotation()[2]);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the direction.
    //--------------------------------------------------------------------------
    Vector<T, n> direction()
    {
        Matrix<T, n + 1_z, n + 1_z> m = matrix();
        Vector<T, n> v(-m[0][2], -m[1][2], -m[2][2]);
        return vector::normalize(v);
    }

    //--------------------------------------------------------------------------
    //! \brief Set the origin of the object (relative to word origin).
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& origin(Vector<T, n> const& origin)
    {
        m_origin = origin;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Get the origin of the object (relative to word origin).
    //--------------------------------------------------------------------------
    inline Vector<T, n> const& origin() const
    {
        return m_origin;
    }

    //--------------------------------------------------------------------------
    //! \brief Set the relative position of the object from its own origin.
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& position(Vector<T, n> const& position)
    {
        m_position = position;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Get the absolute position of the object from its own origin.
    //--------------------------------------------------------------------------
    inline Vector<T, n> const& position() const
    {
        return m_position;
    }

    //--------------------------------------------------------------------------
    //! \brief Move the object by a given offset.
    //! Do the same job than translate() or displace().
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& move(Vector<T, n> const& offset)
    {
        m_position += offset;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Move the object by a given offset.
    //! Do the same job than move() or displace().
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& translate(Vector<T, n> const& offset)
    {
        m_position += offset;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Move the object by a given offset.
    //! Do the same job than translate() or move().
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& displace(Vector<T, n> const& offset)
    {
        m_position += offset;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Move the object by a given offset to the right (positive offset)
    //! or to the left (negative offset).
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& moveRight(T const& offset)
    {
        m_position -= offset * right();
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Move up the object by a given offset (positive offset)
    //! or to move down (negative offset).
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& moveUp(T const& offset)
    {
        m_position -= offset * up();
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Move forward the object by a given offset (positive offset)
    //! or to move backward (negative offset).
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& moveForward(T const& offset)
    {
        m_position -= offset * forward();
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Set the absolute scale factor of the object.
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& scaling(Vector<T, n> const& scale)
    {
        m_scale = scale;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Get the absolute scale factors of the object.
    //--------------------------------------------------------------------------
    inline Vector<T, n> const& scaling() const
    {
        return m_scale;
    }

    //--------------------------------------------------------------------------
    //! \brief Relative scaling of the object.
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& scale(Vector<T, n> const& factor)
    {
        m_scale *= factor;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Set a local scaling of the object. By local we mean that
    //! will not impact children (they will not be scaled).
    //! \note for scaling children use methods Transformable::scale or
    //! Transformable::scaleFactor.
    //--------------------------------------------------------------------------
    inline Transformable<T,n>& localScale(Vector<T, n> const &scale)
    {
        m_local_scaling = scale;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Get the local scaling.
    //--------------------------------------------------------------------------
    inline Vector<T, n> const &localScale() const
    {
        return m_local_scaling;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline Matrix<T, n + 1_z, n + 1_z> rotation() const
    {
        return m_orientation.toMatrix();
    }

    //--------------------------------------------------------------------------
    //! \brief Set the absolute orientation of the object.
    //! \param angle in radian.
    //--------------------------------------------------------------------------
    Transformable<T,n>& rotate(T const angle, Vector<T, n> const& axis)
    {
        Quat<T> rot = angleAxis(angle, axis);
        rot.normalize();
        m_orientation = m_orientation * rot;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Transformable<T,n>& pitch(T const angle)
    {
        Quat<T> rot = angleAxis(angle, right());
        rot.normalize();
        m_orientation = m_orientation * rot;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Transformable<T,n>& yaw(T const angle)
    {
        Quat<T> rot = angleAxis(angle, up());
        rot.normalize();
        m_orientation = m_orientation * rot;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Transformable<T,n>& roll(T const angle)
    {
        Quat<T> rot = angleAxis(angle, forward());
        rot.normalize();
        m_orientation = m_orientation * rot;
        m_transform_needs_update = true;

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the 4x4 transform matrix combining the
    //! position/rotation/scale/origin of the object.
    //!
    //! Operation:
    //!   Transform = TranslationMatrix * RotationMatrix * ScaleMatrix;
    //!   TransformedVector = Transform * OriginalVector;
    //! Note:
    //!   Be careful of operation order: we apply scale first, then
    //!   the rotation then the translation.
    //--------------------------------------------------------------------------
    Matrix<T, n + 1_z, n + 1_z> const& matrix() // FIXME should be const
    {
        if (unlikely(m_transform_needs_update))
        {
            m_transform = matrix::translate(m_transform, m_position - m_origin);
std::cout << "Angle: " << m_orientation.angle() << std::endl;
std::cout << "Axis: " << m_orientation.axis() << std::endl;
            m_transform = matrix::rotate(m_transform, m_orientation.angle(), m_orientation.axis());
            m_transform = matrix::scale(m_transform, m_scale);
            m_transform_needs_update = false;
            m_inverse_trans_needs_update = true;
        }

        return m_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the 4x4 inverse transform matrix.
    //--------------------------------------------------------------------------
    Matrix<T, n + 1_z, n + 1_z> const& invMatrix()
    {
        if (unlikely(m_inverse_trans_needs_update))
        {
            if (unlikely(!matrix::inverse(matrix(), m_inverse_transform)))
            {
                //FIXME throw GL::Exception("Failed to inverse the matrix");
                matrix::identity(m_inverse_transform);
            }
            m_inverse_trans_needs_update = false;
        }
        return m_inverse_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    // TODO void lookAt(Vector3f const &target);

protected:

    //! \brief Position of the origin relative to the origin of the world.
    Vector<T, n> m_origin;
    //! \brief Position of the object relative to its origin.
    Vector<T, n> m_position;
    //! \brief Orientation (quaternion).
    Quat<T> m_orientation;
    //! \brief Relative scaling of the object relative to the world.
    Vector<T, n> m_scale;
    //! \brief Scaling factors for the object.
    Vector<T, n> m_local_scaling;
    //! \brief Local transformation
    Matrix<T, n + 1_z, n + 1_z> m_transform;
    //! \brief Inversed transformation
    Matrix<T, n + 1_z, n + 1_z> m_inverse_transform;
    //! \brief Shall m_transform need to be computed ?
    bool m_transform_needs_update;
    //! \brief Shall m_inverse_transform need to be computed ?
    bool m_inverse_trans_needs_update;
};

using Transformable3D = Transformable<float, 3u>;

#endif // OPENGLCPPWRAPPER_TRANSFORMABLE_HPP
