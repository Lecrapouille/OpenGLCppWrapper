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


#ifndef OPENGLCPPWRAPPER_ORTHOGRAPHICCAMERA_HPP
#  define OPENGLCPPWRAPPER_ORTHOGRAPHICCAMERA_HPP

#  include "Camera/Camera.hpp"

namespace glwrap
{

  //! \brief Used by CameraController and SceneGraph
  DECLARE_CLASS(OrthographicCamera3D);

  // *****************************************************************************
  //! \brief
  // *****************************************************************************
  class OrthographicCamera3D: public Camera3D
  {
    friend class CameraController;

  public:

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    OrthographicCamera3D(float const left = -1.0f,
                         float const right = 1.0f,
                         float const bottom = -1.0f,
                         float const top = 1.0f,
                         float const near = -1.0f,
                         float const far = 1.0f)
      : Camera3D(Camera3D::Type::ORTHOGRAPHIC),
        m_left(left), m_right(right), m_bottom(bottom),
        m_top(top), m_near(near), m_far(far)
    {
      updateProjectionMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    ~OrthographicCamera3D()
    {}

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    static OrthographicCamera3D_SP create(float const left = -1.0f,
                                          float const right = 1.0f,
                                          float const bottom = -1.0f,
                                          float const top = 1.0f,
                                          float const near = -1.0f,
                                          float const far = 1.0f)
    {
      return std::make_shared<OrthographicCamera3D>(left, right, bottom,
                                                    top, near, far);
    }

    //----------------------------------------------------------------------------
    //! \brief Initialize orthographic projection.
    //! Aspect ratio is determined in updateProjection() from current viewport dimensions,
    //! left and right borders are adjusted to comply with m_aspect ratio.
    //!
    //! \param left left border
    //! \param right right border
    //! \param bottom bottom border
    //! \param top top border
    //! \param near near distance
    //! \param far far distance
    //----------------------------------------------------------------------------
    void init(float const left = -1.0f,
              float const right = 1.0f,
              float const bottom = -1.0f,
              float const top = 1.0f,
              float const near = -1.0f,
              float const far = 1.0f)
    {
      m_left = left;
      m_right = right;
      m_bottom = bottom;
      m_top = top;
      m_near = near;
      m_far = far;
      updateProjectionMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief Update projection matrix from current viewport dimensions, should
    //! be called if window has been resized.
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix() override
    {
      GLint viewport[4];

      glCheck(glGetIntegerv(GL_VIEWPORT, viewport));

      float const width  = static_cast<float>(viewport[2] - viewport[0]);
      float const height = static_cast<float>(viewport[3] - viewport[1]);
      float const aspect = width / height;
      m_projection = matrix::ortho(-aspect, aspect, m_bottom, m_top
                                  /* TODO , m_near, m_far */);

      return m_projection;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //! \note: height is never zero !
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateProjectionMatrix(const float width, const float height) override
    {
      float const aspect = width / height;
      m_projection = matrix::ortho(-aspect, aspect, m_bottom, m_top
                                  /* TODO , m_near, m_far */);
      return m_projection;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual Matrix44f const& updateViewMatrix() override
    {
      m_view = matrix::lookAt(m_position, m_target, m_up);
      return m_view;
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    virtual void lookAt(Vector3f const &target) override
    {
      m_target = target;
      updateViewMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    void lookAt(Vector3f const &eye, Vector3f const &center)
    {
      m_position = eye;
      m_target = center;
      updateViewMatrix();
    }

    //----------------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------------
    void lookAt(Vector3f const &eye, Vector3f const &center, Vector3f const &up)
    {
      m_position = eye;
      m_target = center;
      m_up = up;
      updateViewMatrix();
    }

  private:

    Vector3f m_target = Vector3f::ZERO;
    Vector3f m_up = Vector3f::UNIT_Y;
    float m_left;
    float m_right;
    float m_bottom;
    float m_top;
    float m_near;
    float m_far;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_ORTHOGRAPHICCAMERA_HPP
