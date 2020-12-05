//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef OPENGLCPPWRAPPER_SHADERLIB_HPP
#  define OPENGLCPPWRAPPER_SHADERLIB_HPP

//----------------------------------------------------------------------------
//! \file This code source is largely inspired by the file
//! three_cpp/three/renderers/impl/gl_shaders.ipp of the project
//! https://github.com/jdduke/three_cpp
//----------------------------------------------------------------------------

#  include "OpenGLCppWrapper/OpenGL.hpp"

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-function"

namespace shaders
{
    //==========================================================================
    namespace common
    {
        static const char* version(/*BasicMaterialConfig const& config*/)
        {
#if defined(GLES)
            return "precision highp float;\n#version 330\n\n";
#elif defined(__APPLE__)
            return
                "#version 330\n\n"
                "out vec4 FragColor;\n";
#else
            return
                "#version 330\n\n"
                "out vec4 FragColor;\n";
#endif
        }

        static const char* constants()
        {
            return
                "const float LOG2 = 1.442695;\n\n";
        }

        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params()
            {
                return
                    "uniform mat4 modelMatrix;\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    "uniform vec3 cameraPosition;\n"
                    "in vec3 position;\n"
                    "in vec3 normal;\n";
            }
        } // namespace vertex

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params()
            {
                return
                    "uniform mat4 viewMatrix;\n"
                    "uniform vec3 cameraPosition;\n";
            }
        } // namespace fragmen
    } // namespace common
} // namespace shaders

#endif // OPENGLCPPWRAPPER_SHADERLIB_HPP
