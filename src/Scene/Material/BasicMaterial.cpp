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

void BasicMaterial::generate(GLVertexShader& vertexShader,
                             GLFragmentShader& fragmentShader)
{
    shaders::materials::basic::code(vertexShader, m_config);
    shaders::materials::basic::code(fragmentShader, m_config);
}



void BasicMaterial::init()
{std::cout << "BasicMaterial::init()" << std::endl;
    if (!program.hasUniform<Vector3f>("diffuse"))
    {std::cout << "diffuse" << std::endl;
        diffuse() = Vector3f(1.0f, 1.0f, 1.0f);
    }

    if (!program.hasUniform<float>("opacity"))
    {
        opacity() = 1.0f;
    }

    if (m_config.useColor)
    {
        std::cout << "usecolor" << std::endl;
        if (!program.hasUniform<Vector3f>("color"))
        {
            std::cout << "color" << std::endl;
            color() = diffuse();
        }
    }

    if (m_config.useAlphaTest)
    {
        if (!program.hasUniform<float>("ALPHATEST"))
            alphaTest() = 0.5f;
    }

    if ((m_config.useMap) || (m_config.useBumpMap) || (m_config.useSpecularMap))
    {
        if (!program.hasUniform<Vector4f>("offsetRepeat"))
            offsetTexture() = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);
    }

    // Material flags useExpFog and useFog are exclusive. Disable useFog
    if ((m_config.useExpFog) && (m_config.useFog))
    {
        if (!program.hasUniform<Vector3f>("color"))
            m_config.useFog = false;
    }

    if (m_config.useFog)
    {
        if (!program.hasUniform<Vector3f>("fogColor"))
            fogColor() = Vector3f(0.5f, 0.5f, 0.5f);
        if (!program.hasUniform<float>("fogNear"))
            fogNear() = 1.0f;
        if (!program.hasUniform<float>("fogFar"))
            fogFar() = 10.0f;
    }

    if (m_config.useExpFog)
    {
        if (!program.hasUniform<float>("fogDensity"))
            fogDensity() = 0.00025f;
    }
}
