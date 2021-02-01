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

#include "Scene/Camera/Perspective.hpp"
#include "OpenGL/Context/OpenGL.hpp"
#include <iostream>

//------------------------------------------------------------------------------
CameraPerspective::CameraPerspective(float const fov, float const near, float const far)
    : m_fov(maths::toRadian(fov)),
      m_near(near),
      m_far(far)
{}

//------------------------------------------------------------------------------
void CameraPerspective::setFieldOfView(float const fov)
{
    m_fov = maths::toRadian(fov);
    updateProjectionMatrix();
}

//------------------------------------------------------------------------------
void CameraPerspective::setPlanes(float const near, float const far)
{
    m_near = near;
    m_far = far;
    updateProjectionMatrix();
}

//------------------------------------------------------------------------------
Matrix44f CameraPerspective::updateProjectionMatrix(Vector4f const& viewport,
                                                    float const width,
                                                    float const height)
{
    glCheck(glViewport(int(viewport[0] * width),
                       int(viewport[1] * height),
                       int(viewport[2] * width),
                       int(viewport[3] * height)));
    float const aspect = width / height;
    return matrix::perspective(m_fov, aspect, m_near, m_far);
}

//------------------------------------------------------------------------------
Matrix44f CameraPerspective::updateProjectionMatrix()
{
    GLint viewport[4];

    glCheck(glGetIntegerv(GL_VIEWPORT, viewport));

    float const width  = static_cast<float>(viewport[2] - viewport[0]);
    float const height = static_cast<float>(viewport[3] - viewport[1]);
    float const aspect = width / height;

    return matrix::perspective(m_fov, aspect, m_near, m_far);
}
