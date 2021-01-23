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

#  include "Scene/Material/BasicMaterial.hpp"
#  include <string>

#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wunused-function"
#    pragma GCC diagnostic ignored "-Wunused-variable"

#  define UV "uv"
#  define POSITION "position"
#  define NORMAL "normal"
#  define COLOR "color"

namespace shaders
{
    //==========================================================================
    namespace name
    {
        static const char* uv = UV;
        static const char* position = POSITION;
        static const char* normal = NORMAL;
        static const char* color = COLOR;
    } // namespace name

    //==========================================================================
    namespace common
    {
        static const char* version()
        {
#if defined(GLES)
            return
                    "precision highp float;\n"
                    "#version 330\n\n";
#elif defined(__APPLE__)
            return
                    "#version 330\n\n";
#else
            return
                    "#version 330\n\n";
#endif
        }

        static const char* constants()
        {
            return
                    "// Constants\n"
                    "const float LOG2 = 1.442695;\n\n";
        }

        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params()
            {
                return
                        "// Inputs\n"
                        "in vec3 " POSITION ";\n"
                        "in vec3 " NORMAL ";\n\n";
            }
        } // namespace vertex


        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params()
            {
                return
                        "// Outputs\n"
                        "out vec4 FragColor;\n\n";
            }
        } // namespace vertex
    } // namespace common

    //==========================================================================
    namespace mvp
    {
        namespace vertex
        {
            static const char* params()
            {
                return
                        "// Model-View-Projection matrices\n"
                        "uniform mat4 modelMatrix;\n"
                        "uniform mat4 viewMatrix;\n"
                        "uniform mat4 projectionMatrix;\n"
                        "uniform vec3 cameraPosition;\n\n";
            }
        } // namespace vertex

        static const char* code()
        {
            return
                    "projectionMatrix * viewMatrix * modelMatrix"
                    " * vec4(" POSITION ", 1.0);\n";
        }

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params()
            {
                return
                        "// Model-View-Projection matrices\n"
                        "uniform mat4 viewMatrix;\n"
                        "uniform vec3 cameraPosition;\n\n";
            }
        } // namespace fragment
    } // namespace mvp

    //==========================================================================
    namespace color
    {
        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& config)
            {
                if (config.useColor)
                    return
                            "// Color\n"
                            "in vec3 vColor;\n\n";
                return "";
            }

            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useColor)
                    return
                            "  // Color\n"
                            "  FragColor = FragColor * vec4(vColor, opacity);\n";
                return "";
            }
        } // namespace fragment

        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params(BasicMaterial::Config const& config)
            {
                if (config.useColor)
                    return
                            "// Color\n"
                            "uniform vec3 color;\n"
                            "out vec3 vColor;\n\n";
                return "";
            }

            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useColor) {
                    if (config.useGammaInput)
                        return
                                "  // Color + Gamma input\n"
                                "  vColor = color * color;\n";
                    return
                            "  // Color\n"
                            "  vColor = color;\n";
                }
                return "";
            }
        } // namespace vertex
    } // namespace color

    //==========================================================================
    namespace texture
    {
        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params(BasicMaterial::Config const& config)
            {
                if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
                    return
                            "// Texture\n"
                            "uniform vec4 offsetRepeat;\n"
                            "in vec2 " UV ";\n"
                            "out vec2 vUV;\n\n";
                return "";
            }

            static const char* code(BasicMaterial::Config const& config)
            {
                if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
                    return
                            "  // Texture\n"
                            "  vUV = " UV "* offsetRepeat.zw + offsetRepeat.xy;\n";
                return "";
            }
        } // namespace vertex

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& config)
            {
                if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
                    return
                            "// Texture\n"
                            "in vec2 vUV;\n"
                            "uniform sampler2D texture;\n\n";
                return "";
            }

            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useMap) {
                    if (config.useGammaInput)
                        return
                                "  // Texture + Gamma input\n"
                                "  vec4 texelColor = texture2D(texture, vUV);\n"
                                "  texelColor.xyz *= texelColor.xyz;\n"
                                "  FragColor = FragColor * texelColor;\n";
                    return
                            "  // Texture\n"
                            "  FragColor = FragColor * texture2D(texture, vUV);\n";
                }
                return "";
            }
        } // namespace fragment
    } // namespace texture

    //==========================================================================
    namespace light
    {
        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Light\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Light\n\n";
            }
        } // namespace vertex

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Light\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Light\n\n";
            }
        } // namespace fragment
    } // namespace light

    //==========================================================================
    namespace fog
    {
        namespace fragment
        {
            //------------------------------------------------------------------
            static const char* params(BasicMaterial::Config const& config)
            {
                if (config.useFog)
                    return
                            "// Fog\n"
                            "uniform vec3 fogColor;\n"
                            "uniform float fogNear;\n"
                            "uniform float fogFar;\n\n";

                if (config.useExpFog)
                    return
                            "// Fog\n"
                            "uniform vec3 fogColor;\n"
                            "uniform float fogDensity;\n\n";

                return "";
            }

            //------------------------------------------------------------------
            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useFog)
                    return
                            "  // Fog\n"
                            "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
                            "  float fogFactor = smoothstep(fogNear, fogFar, depth);\n"
                            "  FragColor = mix(FragColor, vec4(fogColor, FragColor.w), fogFactor);\n";

                if (config.useExpFog)
                    return
                            "  // Exponential Fog\n"
                            "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
                            "  float fogFactor = exp2(-fogDensity * fogDensity * depth * depth * LOG2);\n"
                            "  fogFactor = 1.0 - clamp(fogFactor, 0.0, 1.0);\n"
                            "  FragColor = mix(FragColor, vec4(fogColor, FragColor.w), fogFactor);\n";

                return "";
            }
        } // namespace fragment
    } // namespace fog

    //==========================================================================
    namespace shadow
    {
        namespace vertex
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Shadow\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Shadow\n\n";
            }
        } // namespace vertex

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Shadow\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Shadow\n\n";
            }
        } // namespace fragment
    } // namespace shadow

    //==========================================================================
    namespace specular
    {
        //----------------------------------------------------------------------
        namespace vertex
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Specular\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Specular\n";
            }
        } // namespace vertex

        //----------------------------------------------------------------------
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "// Specular\n\n";
            }

            static const char* code(BasicMaterial::Config const& /*config*/)
            {
                return // TODO
                        "  // Specular\n\n";
            }
        } // namespace fragment
    } // namespace specular

    //==========================================================================
    namespace alpha
    {
        namespace fragment
        {
            static const char* params(BasicMaterial::Config const& config)
            {
                if (config.useAlphaTest)
                    return
                            "// Alpha Test\n"
                            "uniform float ALPHATEST;\n\n";
                return "";
            }

            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useAlphaTest)
                    return
                            "  // Alpha Test\n"
                            "  if (FragColor.a < ALPHATEST) discard;\n";
                return "";
            }
        } // namespace fragment
    } // namespace alpha

    //==========================================================================
    namespace gamma
    {
        namespace fragment
        {
            static const char* code(BasicMaterial::Config const& config)
            {
                if (config.useGammaOutput)
                    return
                            "  // Gamma\n"
                            "  FragColor.xyz = sqrt(FragColor.xyz);\n\n";
                return "";
            }
        } // namespace fragment
    } // namespace gamma

    //==========================================================================
    namespace materials
    {
        //----------------------------------------------------------------------
        namespace depth
        {
            static void code(GLVertexShader& vertexShader)
            {
                vertexShader
                        << "// shaders::materials::depth::vertex\n"
                        << shaders::common::version()
                        << shaders::common::vertex::params()
                        << shaders::mvp::vertex::params()
                        << "void main()\n{\n"
                        << "  // Final position\n"
                        << "  gl_Position = " << shaders::mvp::code()
                        << "}\n";
            }

            static void code(GLFragmentShader& fragmentShader)
            {
                fragmentShader
                        << "// shaders::materials::depth::fragment\n"
                        << shaders::common::version()
                        << shaders::common::fragment::params()
                        << "uniform float near;\n"
                        << "uniform float far;\n"
                        << "uniform float opacity;\n"
                        << "void main()\n{\n"
                        << "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
                        << "  float color = 1.0 - smoothstep(near, far, depth);\n"
                        << "  FragColor = vec4(vec3(color), opacity);\n"
                        << "}\n";
            }
        } // namespace depth

        //----------------------------------------------------------------------
        namespace normals
        {
            static void code(GLVertexShader& vertexShader)
            {
                vertexShader
                        << "// shaders::materials::normals::vertex\n"
                        << shaders::common::version()
                        << shaders::mvp::vertex::params()
                        << shaders::common::vertex::params()
                        << "uniform mat3 normalMatrix; // = mat3(transpose(inverse(modelMatrix * viewMatrix)));\n"
                        << "out vec3 vNormal;\n"
                        << "void main()\n{\n"
                        << "  vNormal = normalMatrix * " NORMAL ";\n"
                        << "  // Final position\n"
                        << "  gl_Position = " << shaders::mvp::code()
                        << "}\n";
            }

            static void code(GLFragmentShader& fragmentShader)
            {
                fragmentShader
                        << "// shaders::materials::normals::fragment\n"
                        << shaders::common::version()
                        << shaders::common::fragment::params()
                        << "uniform float opacity;\n"
                        << "in vec3 vNormal;\n"
                        << "void main()\n{\n"
                        << "  FragColor = vec4(0.5 * normalize(vNormal) + 0.5, opacity);\n"
                        << "}\n";
            }
        } // namespace normals

        //----------------------------------------------------------------------
        namespace basic
        {
            //************
            namespace line
            {
                static void code(GLVertexShader& vertexShader)
                {
                    vertexShader
                            << "// shaders::materials::basic::line::vertex\n"
                            << shaders::common::version()
                            << shaders::common::vertex::params()
                            << shaders::mvp::vertex::params()
                            << "in vec4 colors;\n"
                            << "out vec4 vColors;\n"
                            << "void main()\n{\n"
                            << "  vColors = colors;\n"
                            << "  // Final position\n"
                            << "  gl_Position = " << shaders::mvp::code()
                            << "}\n";
                }

                static void code(GLFragmentShader& fragmentShader)
                {
                    fragmentShader
                            << "// shaders::materials::basic::line::fragment\n"
                            << shaders::common::version()
                            << shaders::common::fragment::params()
                            << "in vec4 vColors;\n"
                            << "void main()\n{\n"
                            << "  FragColor = vColors;\n"
                            << "}\n";
                }
            } // namespace line

            //************
            namespace mesh
            {
                static void code(GLVertexShader& vertexShader, BasicMaterial::Config const& config)
                {
                    vertexShader
                            << "// shaders::materials::basic::mesh::vertex\n"
                            << shaders::common::version()
                            << shaders::common::vertex::params()
                            << shaders::mvp::vertex::params()
                            << shaders::texture::vertex::params(config)
                            << shaders::light::vertex::params(config)
                            << shaders::color::vertex::params(config)
                            << "void main()\n{\n"
                            << shaders::texture::vertex::code(config)
                            << shaders::light::vertex::code(config)
                            << shaders::color::vertex::code(config)
                            << "  // Final position\n"
                            << "  gl_Position = " << shaders::mvp::code()
                            << "}\n";
                }

                static void code(GLFragmentShader& fragmentShader, BasicMaterial::Config const& config)
                {
                    fragmentShader
                            << "// shaders::materials::basic::mesh::fragment\n"
                            << shaders::common::version()
                            << shaders::common::fragment::params()
                            << shaders::common::constants()
                            << shaders::alpha::fragment::params(config)
                            << shaders::color::fragment::params(config)
                            << shaders::texture::fragment::params(config)
                            << shaders::light::fragment::params(config)
                            << shaders::fog::fragment::params(config)
                            << shaders::shadow::fragment::params(config)
                            << shaders::specular::fragment::params(config)
                            << "// Diffuse color\n"
                            << "uniform vec3 diffuse;\n"
                            << "uniform float opacity;\n\n"
                            << "void main()\n{\n"
                            << "  FragColor = vec4(diffuse, opacity);\n"
                            << shaders::texture::fragment::code(config)
                            << shaders::alpha::fragment::code(config)
                            << shaders::light::fragment::code(config)
                            << shaders::color::fragment::code(config)
                            << shaders::shadow::fragment::code(config)
                            << shaders::gamma::fragment::code(config)
                            << shaders::fog::fragment::code(config)
                            << "}\n";
                }
            } // namespace mesh
        } // namespace basic
    } // namespace materials
} // namespace shaders

#endif // OPENGLCPPWRAPPER_SHADERLIB_HPP
