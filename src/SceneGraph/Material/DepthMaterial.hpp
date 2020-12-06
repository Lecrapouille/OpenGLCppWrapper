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

#  include "SceneGraph/Material/Material.hpp"
#  include "SceneGraph/Material/ShaderLib.hpp"

class DepthMaterial : public Material
{
public:

    DepthMaterial()
        : Material("DepthMaterial")
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
        shaders::materials::depth::code(vertexShader);
        shaders::materials::depth::code(fragmentShader);
    }

    void init() override
    {
        near() = 1.0f;
        far() = 100.0f;
        opacity() = 1.0f;
    }
};

#endif
