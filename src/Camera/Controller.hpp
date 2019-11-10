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

#ifndef OPENGLCPPWRAPPER_CAMERACONTROLLER_HPP
#  define OPENGLCPPWRAPPER_CAMERACONTROLLER_HPP

// TODO
#include <iostream>
// TODO: control any camera
// TODO: other type of control: like 3th person, trackball,
// TODO: follow a path
#  include "Camera/Perspective.hpp"
#  include "Camera/Orthographic.hpp"

namespace glwrap
{

  // *****************************************************************************
  //! \brief
  // *****************************************************************************
  class CameraController
  {
  public:

    enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    CameraController(Camera3D::Type const type)
    {
      setCamera(type);
    }

    CameraController(PerspectiveCamera3D_SP camera) // Camera3D_SP camera)
    {
      setCamera(camera);
    }

    Camera3D& setCamera(Camera3D::Type const type)
    {
      switch (type)
        {
        case Camera3D::Type::ORTHOGRAPHIC:
          setCamera(PerspectiveCamera3D::create());//FIXME OrthographicCamera3D::create();
          break;
        case Camera3D::Type::PERSPECTIVE:
        default:
          setCamera(PerspectiveCamera3D::create());
          break;
        }

      return camera();
    }

    void setCamera(PerspectiveCamera3D_SP camera) // Camera3D_SP camera)
    {
      m_camera = camera;
      m_camera->updateStates();
      m_camera->updateProjectionMatrix();
    }

    PerspectiveCamera3D& camera()
    {
      return *m_camera;
    }

    void setMoveVelocity(float const moveVelocity)
    {
      m_moveVelocity = moveVelocity;
    }

    void setrotateVelocity(float const rotateVelocity)
    {
      m_rotateVelocity = rotateVelocity;
    }

    void setFlightVelocity(float const flightVelocity)
    {
      m_flightVelocity = flightVelocity;
    }

    void setFlightVelocityStep(float const flightVelocityStep)
    {
      m_flightVelocityStep = flightVelocityStep;
    }

    void processKeyboard(Movement const direction, float const deltaTime)
    {
      doProcessKeyboard(direction, m_moveVelocity * deltaTime);
      m_camera->updateViewMatrix();
    }

    void processMouseMovement(float const xoffset, float const yoffset, float const deltaTime)
    {
      doProcessMouseMovement(xoffset, yoffset, m_rotateVelocity * deltaTime);
      m_camera->updateViewMatrix();

      //std::cout << "Yaw: " << m_camera->m_yaw << " Pitch: " << m_camera->m_pitch << std::endl;
      //std::cout << "Tar: " << m_camera->m_position + m_camera->m_front << std::endl;
    }

  private:

    /*virtual void doProcessKeyboard(Movement const direction, float const velocity)
    {}

    virtual void doProcessMouseMovement(float const xoffset, float const yoffset, float const deltaTime)
    {}*/

  protected:

    PerspectiveCamera3D_SP m_camera; // TODO Camera3D_SP m_camera;
    float m_moveVelocity = 2.5f;
    float m_rotateVelocity = 2.5f; // deg / s
    float m_flightVelocity = 2.5f;
    float m_flightVelocityStep = 2.5f;
    //};

  // *****************************************************************************
  //! \brief
  // *****************************************************************************
    //TODO class FPSCameraController : public CameraController
    //{
  public:

    virtual void doProcessKeyboard(Movement const direction, float const velocity)
    {
      if (direction == Movement::FORWARD)
        displace(velocity);
      if (direction == Movement::BACKWARD)
        displace(-velocity);
      if (direction == Movement::LEFT)
        strafe(-velocity);
      if (direction == Movement::RIGHT)
        strafe(velocity);
    }

    virtual void doProcessMouseMovement(float const xoffset, float const yoffset, float const velocity)
    {
      m_camera->incrAngles(xoffset * velocity, yoffset * velocity);
    }

    //! \brief change Field Of View (FOV).
    //! \param delta FOV in degree
    Matrix44f const& zoom(float const delta)
    {
      const float minAngle = maths::toRadian(1.0f); // radian
      const float maxAngle = maths::toRadian(179.0f); // radian

      m_camera->m_fov -= maths::toRadian(delta);
      m_camera->m_fov = maths::clamp(m_camera->m_fov, minAngle, maxAngle);

      return m_camera->updateProjectionMatrix();
    }

  private:

    void displace(float const velocity)
    {
      m_camera->m_position += (velocity * m_camera->m_front);
    }

    void strafe(float const velocity)
    {
      m_camera->m_position += (velocity * m_camera->m_right);
    }
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_CAMERACONTROLLER_HPP
