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

#ifndef OPENGLCPPWRAPPER_CAMERA_HPP
#  define OPENGLCPPWRAPPER_CAMERA_HPP

#  include "Scene/Transformable.hpp"

namespace glwrap
{

  //! \brief Used by CameraController and SceneGraph
  DECLARE_CLASS(Camera3D)

  // *****************************************************************************
  //! \brief
  // *****************************************************************************
  class Camera3D: public Transformable3D
  {
    friend class CameraController;

  public:

    enum class Type { ORTHOGRAPHIC, PERSPECTIVE };

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    Camera3D(Type const type)
      : m_type(type)
    {
      m_position = Vector3f::UNIT_SCALE;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual ~Camera3D()
    {}

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    inline Type type() const
    {
      return m_type;
    }

    //----------------------------------------------------------------------------
    //! \brief Get projection matrix.
    //----------------------------------------------------------------------------
    inline Matrix44f const& projectionMatrix() const
    {
      return m_projection;
    }

    //----------------------------------------------------------------------------
    //! \brief Get view transformation matrix according to camera position and
    //! orientation.
    //----------------------------------------------------------------------------
    inline Matrix44f const& viewMatrix() const
    {
      return m_view;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix(float const width, float const height) = 0;

    //----------------------------------------------------------------------------
    //! \brief Update projection matrix from current viewport dimensions, should
    //! be called if window has been resized.
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix() = 0;

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateViewMatrix() = 0;

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual void lookAt(Vector3f const &target) = 0;

    //protected:

    Matrix44f m_projection;
    Matrix44f m_view;
    Type      m_type;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_CAMERA_HPP
