//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (target your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_PERSPECTIVECAMERA_HPP
#  define OPENGLCPPWRAPPER_PERSPECTIVECAMERA_HPP

#  include "Camera/Camera.hpp"

namespace glwrap
{
  //! \brief Used by CameraController and SceneGraph
  DECLARE_CLASS(PerspectiveCamera3D);

  // *****************************************************************************
  //! \brief
  // *****************************************************************************
  class PerspectiveCamera3D: public Camera3D
  {
    friend class CameraController;

  public:

    inline friend std::ostream& operator<<(std::ostream& os, PerspectiveCamera3D const& c)
    {
      return os << "Camera Pos: " << c.m_position
                << " LookAt: " << c.m_direction
                << " Up: " << c.m_up;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    PerspectiveCamera3D(float const fov = 45.0f,
                        float const near = 0.1f,
                        float const far = 1000.0f)
      : Camera3D(Camera3D::Type::PERSPECTIVE),
        m_fov(maths::toRadian(fov)), m_near(near), m_far(far)
    {
      lookAt(Vector3f::UNIT_SCALE, Vector3f::ZERO, Vector3f::UNIT_Y);
      updateProjectionMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    ~PerspectiveCamera3D()
    {}

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    static PerspectiveCamera3D_SP create(float const fov = 45.0f,
                                         float const near = 0.1f,
                                         float const far = 1000.0f)
    {
      return std::make_shared<PerspectiveCamera3D>(fov, near, far);
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    void init(float const fov = 45.0f, float const near = 0.1f, float const far = 1000.0f)
    {
      m_fov = maths::toRadian(fov);
      m_near = near;
      m_far = far;
      updateProjectionMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //! \note: height is never zero !
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix(float const width, float const height) override
    {
      m_aspect = width / height;
      m_projection = matrix::perspective(m_fov, m_aspect, m_near, m_far);

      return m_projection;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix() override
    {
      GLint viewport[4];

      glCheck(glGetIntegerv(GL_VIEWPORT, viewport));

      float const width  = static_cast<float>(viewport[2] - viewport[0]);
      float const height = static_cast<float>(viewport[3] - viewport[1]);
      m_aspect = width / height;

      m_projection = matrix::perspective(m_fov, m_aspect, m_near, m_far);

      return m_projection;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateViewMatrix() override
    {
      m_view = matrix::lookAt(m_position, m_position + m_front, m_up);
      return m_view;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual void lookAt(Vector3f const &target) override
    {
      m_front = vector::normalize(target - m_position);
      m_right = vector::normalize(vector::cross(m_front, m_worldUp));
      m_up    = vector::normalize(vector::cross(m_right, m_front));
      initAngles();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    void lookAt(Vector3f const &position, Vector3f const &target)
    {
      m_position = position;
      m_front = vector::normalize(target - m_position);
      m_right = vector::normalize(vector::cross(m_front, m_worldUp));
      m_up    = vector::normalize(vector::cross(m_right, m_front));
      initAngles();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    void lookAt(Vector3f const &position, Vector3f const &target, Vector3f const &up)
    {
      m_position = position;
      m_worldUp = up;
      m_front = vector::normalize(target - m_position);
      m_right = vector::normalize(vector::cross(m_front, m_worldUp));
      m_up    = vector::normalize(vector::cross(m_right, m_front));
      initAngles();
    }

    void incrAngles(float const yaw, float const pitch) // radians
    {
      m_pitch += pitch;
      m_pitch = maths::clamp(m_pitch, -89.0f, 89.0f);

      m_yaw += yaw;
      m_yaw = maths::wrapTo360(m_yaw);

      updateStates();
    }

  private:

    void initAngles()
    {
      Vector3f &f = m_front;
      float length = vector::norm(m_front);
      m_yaw = 360 + maths::toDegree(atan2f(f.z, f.x));
      m_pitch = maths::toDegree(asinf(f.y / length));

      updateViewMatrix();
    }

    void updateStates()
    {
      m_direction.x = cosf(maths::toRadian(m_yaw)) * cosf(maths::toRadian(m_pitch));
      m_direction.y = sinf(maths::toRadian(m_pitch));
      m_direction.z = sinf(maths::toRadian(m_yaw)) * cosf(maths::toRadian(m_pitch));
      m_front = vector::normalize(m_direction);
      m_right = vector::normalize(vector::cross(m_front, m_worldUp));
      m_up    = vector::normalize(vector::cross(m_right, m_front));

      updateViewMatrix();
    }

  private:

    Vector3f m_front;
    Vector3f m_right;
    Vector3f m_up;
    Vector3f m_worldUp;
    Vector3f m_direction;

    float m_fov; // radians
    float m_aspect;
    float m_near;
    float m_far;

    float m_yaw; // degrees FIXME: radians
    float m_pitch;
    //float m_roll;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PERSPECTIVECAMERA_HPP
