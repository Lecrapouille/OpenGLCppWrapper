//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "SceneGraph/Material/DepthMaterial.hpp"
#include "SceneGraph/Material/ShaderLib.hpp"

void DepthMaterial::createShaders(GLVertexShader& vertexShader,
                                  GLFragmentShader& fragmentShader)
{
    shaders::materials::depth::code(vertexShader);
    shaders::materials::depth::code(fragmentShader);
}

void DepthMaterial::init()
{
    near() = 1.0f;
    far() = 100.0f;
    opacity() = 1.0f;
}
