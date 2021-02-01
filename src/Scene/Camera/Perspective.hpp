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

#ifndef SCENE_CAMERA_PERSPECTIVE_HPP
#  define SCENE_CAMERA_PERSPECTIVE_HPP

#  include "Scene/Components.hpp"
#  include "Math/Transformable.hpp"

// *****************************************************************************
//! \brief
// *****************************************************************************
class CameraPerspective : public Component
{
public:

    //----------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------
    CameraPerspective(float const fov = 60.0f, float const near = 0.1f,
                      float const far = 1000.0f);

    //----------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------
    void setFieldOfView(float const fov = 45.0f);

    //----------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------
    void setPlanes(float const near = 0.1f, float const far = 1000.0f);

    //----------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------
    Matrix44f updateProjectionMatrix(Vector4f const& viewport,
                                     float const width, float const height);

    //----------------------------------------------------------------------
    //! \brief
    //----------------------------------------------------------------------
    Matrix44f updateProjectionMatrix();

private:

    float m_fov;
    float m_near;
    float m_far;
};

#endif
