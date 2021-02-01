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

#ifndef CAMERA_NODE_HPP
#  define CAMERA_NODE_HPP

#  include "Scene/SceneTree.hpp"
#  include "Scene/Camera/Perspective.hpp"

// *****************************************************************************
//! \brief
// *****************************************************************************
class Camera: public SceneObject
{
public:

    enum class Mode { ORTHOGRAPHIC, PERSPECTIVE };

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Camera(std::string const& name,
           float x, float y, float w, float h,
           Mode const mode = Mode::PERSPECTIVE);

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    void setMode(Mode const mode)
    {
        m_mode = mode;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix.
    //--------------------------------------------------------------------------
    inline Matrix44f const& projection() const
    {
        return m_projection;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the view transformation matrix according to camera
    //! position and orientation.
    //--------------------------------------------------------------------------
    inline Matrix44f const& view() const
    {
        return m_view;
    }

    //--------------------------------------------------------------------------
    //! \brief Update the projection matrix from the width and height of the
    //! view port.
    //! \fixme width and height should be normalized / screen.width ...
    //--------------------------------------------------------------------------
    Matrix44f const& updateProjectionMatrix(float const width, float const height);

    //--------------------------------------------------------------------------
    //! \brief Update projection matrix from current viewport dimensions, should
    //! be called if window has been resized.
    //--------------------------------------------------------------------------
    Matrix44f const& updateProjectionMatrix();

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    Matrix44f const& updateViewMatrix();

protected:

    Vector4f m_viewport;
    CameraPerspective& m_perspective;
    Mode m_mode;
    Matrix44f m_projection;
    Matrix44f m_view;
};

#endif
