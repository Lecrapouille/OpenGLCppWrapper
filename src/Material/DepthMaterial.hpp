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

#ifndef DEPTH_MATERIAL_HPP
#  define DEPTH_MATERIAL_HPP

#  include "Material/Material.hpp"
#  include "Material/ShaderLib.hpp"

class DepthMaterial : public IMaterial
{
public:

    DepthMaterial()
        : IMaterial("DepthMaterial")
    {}

    float& near()
    {
        return program.scalarf("near");
    }

    float& far()
    {
        return program.scalarf("far");
    }

    float& opacity()
    {
        return program.scalarf("opacity");
    }

private:

    virtual void createShaders(GLVertexShader& vertexShader,
                               GLFragmentShader& fragmentShader) override
    {
        vertexShader
            << shaders::common::version()
            << shaders::common::vertex::params()
            << "\nvoid main()\n{\n"
            << "  gl_Position = projectionMatrix"
            << "              * modelMatrix"
            << "              * viewMatrix"
            << "              * vec4(position, 1.0);\n"
            << "}\n";

        fragmentShader
            << shaders::common::version()
            << shaders::common::fragment::params()
            << "uniform float near;\n"
            << "uniform float far;\n"
            << "uniform float opacity;\n"
            << "\nvoid main()\n{\n"
            << "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
            << "  float color = 1.0 - smoothstep(near, far, depth);\n"
            << "  FragColor = vec4(vec3(color), opacity);\n"
            << "}\n";
    }

    void init() override
    {
        near() = 1.0f;
        far() = 100.0f;
        opacity() = 1.0f;
    }
};

#endif
