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

#include "Scene/Camera/CameraNode.hpp"

//--------------------------------------------------------------------------
Camera::Camera(std::string const& name, float x, float y, float w, float h,
               Mode const mode)
    : SceneObject(name),
      m_viewport(x, y, w, h),
      m_perspective(components.addComponent<CameraPerspective>()),
      //m_orthographic(components.addComponent<Orthographic>("Orthographic"))
      m_mode(mode)
{
    transform.lookAt(Vector3f::UNIT_SCALE, Vector3f::ZERO, Vector3f::UNIT_Y);
}

//--------------------------------------------------------------------------
Matrix44f const& Camera::updateProjectionMatrix(float const width, float const height)
{
    //if (m_mode = PERSPECTIVE)
    m_projection = m_perspective.updateProjectionMatrix(m_viewport, width, height);
    //else

    return m_projection;
}

//--------------------------------------------------------------------------
Matrix44f const& Camera::updateProjectionMatrix()
{
    //if (m_mode = PERSPECTIVE)
    m_projection = m_perspective.updateProjectionMatrix();
    //else

    return m_projection;
}

//--------------------------------------------------------------------------
Matrix44f const& Camera::updateViewMatrix()
{
    // TODO transform.translation()
    // negative translation:
    // https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/
    // Chapiter The View Matrix
    Matrix44f I(matrix::Identity);
    Matrix44f transl = matrix::translate(I, -transform.position());

    std::cout << "Trans: " << transl << std::endl;
    std::cout << "Rot: " << transform.rotation() << std::endl;

    // FIXME shall be rotation * translation but glm seems to inverse matrix prodcut: A * B is made as B * A
    // https://stackoverflow.com/questions/18151845/converting-glmlookat-matrix-to-quaternion-and-back
    m_view = transl * transform.rotation();
    //std::cout << "updateViewMatrix " << m_view << std::endl;


    return m_view;
}
