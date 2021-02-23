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
// This code has been inspired by the Youtube channel Game Dev Guide
// Please watch this video "Building a Camera Controller for a Strategy Game"
// https://www.youtube.com/watch?v=rnqF6S7PfFA
// *****************************************************************************

#ifndef RIG_CAMERA_CONTROLLER_HPP
#  define RIG_CAMERA_CONTROLLER_HPP

#  include "Scene/RigNode.hpp"
#  include "Scene/Camera/CameraNode.hpp"

// *****************************************************************************
//! \brief Camera rig reactive to keyboard IO for strategy games
// *****************************************************************************
class RigNode: public SceneObject
{
public:

    //--------------------------------------------------------------------------
    RigNode(std::string const& name, Camera& camera);

    //--------------------------------------------------------------------------
    void control(Camera& camera);

private:

    //--------------------------------------------------------------------------
    virtual void onUpdate(float const /*dt*/) override;

    //--------------------------------------------------------------------------
    virtual void handleMouseInput();
    virtual void handleKeyBoardInput();
    virtual void handleMovementInput(float const dt);

public:

    float normalSpeed = 0.1f;
    float fastSpeed = 1.0f;
    float movementTime = 0.1f;
    float zoomAmount = 0.1f;

private:

    Camera* m_camera;
    Transformable3D m_newTransform;
    float m_fov, m_newZoom;
};

#endif // RIG_CAMERA_CONTROLLER_HPP
