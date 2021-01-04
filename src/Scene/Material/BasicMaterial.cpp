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

#include "Scene/Material/BasicMaterial.hpp"
#include "Scene/Material/ShaderLib.hpp"
#include "Scene/Material/Color.hpp"

void BasicMaterial::createShaders(GLVertexShader& vertexShader,
                                  GLFragmentShader& fragmentShader)
{
    shaders::materials::basic::code(vertexShader, m_config);
    shaders::materials::basic::code(fragmentShader, m_config);
}

void BasicMaterial::init()
{
    diffuse() = Color().toVector3f();
    opacity() = 1.0f;

    if (m_config.useColor)
        color() = diffuse();

    if (m_config.useAlphaTest)
        alphaTest() = 0.5f;

    if ((m_config.useMap) || (m_config.useBumpMap) || (m_config.useSpecularMap))
        offsetTexture() = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

    // Material flags useExpFog and useFog are exclusive. Disable useFog
    if ((m_config.useExpFog) && (m_config.useFog))
        m_config.useFog = false;

    if (m_config.useFog)
    {
        fogColor() = Vector3f(0.5f, 0.5f, 0.5f);
        fogNear() = 1.0f;
        fogFar() = 10.0f;
    }

    if (m_config.useExpFog)
        fogDensity() = 0.00025f;
}
