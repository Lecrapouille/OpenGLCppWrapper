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

#ifndef CAMERA_NODE_HPP
#  define CAMERA_NODE_HPP

#  include "Scene/SceneTree.hpp"

// *****************************************************************************
//! \brief
// *****************************************************************************
class Camera: public SceneObject
{
public:

    class Perspective : public Component
    {
    public:

        //----------------------------------------------------------------------
        //! \brief
        //----------------------------------------------------------------------
        Perspective(float const fov = 45.0f,
                    float const near = 0.1f,
                    float const far = 1000.0f)
            : m_fov(maths::toRadian(fov)),
              m_near(near),
              m_far(far)
        {
            updateProjectionMatrix();
        }

        void setFieldOfView(float const fov = 45.0f)
        {
            m_fov = maths::toRadian(fov);
            updateProjectionMatrix();
        }

        void setPlanes(float const near = 0.1f, float const far = 1000.0f)
        {
            m_near = near;
            m_far = far;
            updateProjectionMatrix();
        }

        Matrix44f updateProjectionMatrix(float const width,
                                         float const height)
        {
            float const aspect = width / height;

            glCheck(glViewport(0, 0, int(width), int(height)));
            return matrix::perspective(m_fov, aspect, m_near, m_far);
        }

        Matrix44f updateProjectionMatrix()
        {
            GLint viewport[4];

            glCheck(glGetIntegerv(GL_VIEWPORT, viewport));

            float const width  = static_cast<float>(viewport[2] - viewport[0]);
            float const height = static_cast<float>(viewport[3] - viewport[1]);
            float const aspect = width / height;

            return matrix::perspective(m_fov, aspect, m_near, m_far);
        }

    private:

        float m_fov;
        float m_near;
        float m_far;
    };

public:

    enum class Mode { ORTHOGRAPHIC, PERSPECTIVE };

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Camera(std::string const& name, Mode const mode = Mode::PERSPECTIVE)
        : SceneObject(name),
          m_perspective(components.addComponent<Perspective>()),
          //m_orthographic(components.addComponent<Orthographic>("Orthographic"))
          m_mode(mode)
    {
        transform.lookAt(Vector3f::UNIT_SCALE, Vector3f::ZERO, Vector3f::UNIT_Y);
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void setMode(Mode const mode)
    {
        m_mode = mode;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    //virtual void lookAt(Vector3f const &target) = 0;

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix.
    //--------------------------------------------------------------------------
    inline Matrix44f const& projectionMatrix() const
    {
        return m_projection;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the view transformation matrix according to camera
    //! position and orientation.
    //--------------------------------------------------------------------------
    inline Matrix44f const& viewMatrix() const
    {
        return m_view;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void updateProjectionMatrix(float const width, float const height)
    {
        //if (m_mode = PERSPECTIVE)
        m_projection = m_perspective.updateProjectionMatrix(width, height);
        //else
    }

    //--------------------------------------------------------------------------
    //! \brief Update projection matrix from current viewport dimensions, should
    //! be called if window has been resized.
    //--------------------------------------------------------------------------
    void updateProjectionMatrix()
    {
        //if (m_mode = PERSPECTIVE)
        m_projection = m_perspective.updateProjectionMatrix();
        //else
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void updateViewMatrix()
    {
        m_view = matrix::lookAt(transform.position() - transform.origin(),
                                transform.position() + transform.forward(),
                                transform.up());
    }

protected:

    Perspective& m_perspective;
    Mode m_mode;
    Matrix44f m_projection;
    Matrix44f m_view;
};

#endif
