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

#  include "OpenGL/Shaders.hpp"
#  include "Material/Config.hpp"
#  include "Texturing/Collection.hpp"

namespace glwrap
{
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-function"

  namespace shaders
  {
    //--------------------------------------------------------------------------
    namespace common
    {
      static const char* version(/*BasicMaterialConfig const& config*/)
      {
#if defined(GLES)
        return "precision highp float;\n#version 330\n\n";
#elif defined(__APPLE__)
        return "#version 330\n\n";
#else
        return "#version 330\n\n";
#endif
      }

      static const char* constants()
      {
        return
          "const float LOG2 = 1.442695;\n\n";
      }

      namespace vertex
      {
        static const char* variables(/*BasicMaterialConfig const& config*/)
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

      namespace fragment
      {
        static const char* variables(/*BasicMaterialConfig const& config*/)
        {
          return
            "uniform mat4 viewMatrix;\n"
            "uniform vec3 cameraPosition;\n";
        }
      } // namespace fragment
    } // namespace common

    //--------------------------------------------------------------------------
    namespace color
    {
      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if (config.useColor)
            return
              "// Color\n"
              "in vec3 vColor;\n";
          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
        {
          if (config.useColor)
            return
              "  // Color\n"
              "  gl_FragColor = gl_FragColor * vec4(vColor, opacity);\n";
          return "";
        }
      } // namespace fragment

      namespace vertex
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if (config.useColor)
            return
              "// Color\n"
              "uniform vec3 color;\n"
              "out vec3 vColor;\n";
          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
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

    //--------------------------------------------------------------------------
    namespace texture
    {
      namespace vertex
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
            return
              "// Texture\n"
              "uniform vec4 offsetRepeat;\n"
              "in vec2 UV;\n"
              "out vec2 vUV;\n";
          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
        {
          if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
            return
              "  // Texture\n"
              "  vUV = UV * offsetRepeat.zw + offsetRepeat.xy;\n";
          return "";
        }
      } // namespace vertex

      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if ((config.useMap) || (config.useBumpMap) || (config.useSpecularMap))
            return
              "// Texture\n"
              "in vec2 vUV;\n"
              "uniform sampler2D texture;\n";
          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
        {
          if (config.useMap) {
            if (config.useGammaInput)
              return
                "  // Texture + Gamma input\n"
                "  vec4 texelColor = texture2D(texture, vUV);\n"
                "  texelColor.xyz *= texelColor.xyz;\n"
                "  gl_FragColor = gl_FragColor * texelColor;\n";
            return
              "  // Texture\n"
              "  gl_FragColor = gl_FragColor * texture2D(texture, vUV);\n";
          }
          return "";
        }
      } // namespace fragment
    } // namespace texture

    //--------------------------------------------------------------------------
    namespace light
    {
      namespace vertex
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace vertex

      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace fragment
    } // namespace light

    //--------------------------------------------------------------------------
    namespace fog
    {
      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if (config.useFog)
            return
              "// Fog\n"
              "uniform vec3 fogColor;\n"
              "uniform float fogNear;\n"
              "uniform float fogFar;\n";

          if (config.useExpFog)
            return
              "// Fog\n"
              "uniform vec3 fogColor;\n"
              "uniform float fogDensity;\n";

          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
        {
          if (config.useFog)
            return
              "  // Fog\n"
              "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
              "  float fogFactor = smoothstep(fogNear, fogFar, depth);\n"
              "  gl_FragColor = mix(gl_FragColor, vec4(fogColor, gl_FragColor.w), fogFactor);\n";

          if (config.useExpFog)
            return
              "  // Exponential Fog\n"
              "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
              "  float fogFactor = exp2(-fogDensity * fogDensity * depth * depth * LOG2);\n"
              "  fogFactor = 1.0 - clamp(fogFactor, 0.0, 1.0);\n"
              "  gl_FragColor = mix(gl_FragColor, vec4(fogColor, gl_FragColor.w), fogFactor);\n";

          return "";
        }
      } // namespace fragment
    } // namespace fog

    //--------------------------------------------------------------------------
    namespace shadow
    {
      namespace vertex
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace vertex

      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace fragment
    } // namespace shadow

    //--------------------------------------------------------------------------
    namespace specular
    {
      namespace vertex
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace vertex

      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }

        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return // TODO
            "";
        }
      } // namespace fragment
    } // namespace specular

    //--------------------------------------------------------------------------
    namespace base
    {
      namespace vertex
      {
        static const char* code(BasicMaterialConfig const& /*config*/)
        {
          return
            "  // Base vertex\n"
            "  gl_Position = projectionMatrix * mvPosition;\n"
            ;
        }
      } // namespace vertex
    } // namespace base

    //--------------------------------------------------------------------------
    namespace alpha
    {
      namespace fragment
      {
        static const char* variables(BasicMaterialConfig const& config)
        {
          if (config.useAlphaTest)
            return
              "// Alpha test\n"
              "uniform float ALPHATEST;";
          return "";
        }

        static const char* code(BasicMaterialConfig const& config)
        {
          if (config.useAlphaTest)
            return
              "  // Alpha test\n"
              "  if (gl_FragColor.a < ALPHATEST) discard;\n";
          return "";
        }
      } // namespace fragment
    } // namespace alpha

    //--------------------------------------------------------------------------
    namespace gamma
    {
      namespace fragment
      {
        static const char* code(BasicMaterialConfig const& config)
        {
          if (config.useGammaOutput)
            return
              "  // Gamma\n"
              "  gl_FragColor.xyz = sqrt(gl_FragColor.xyz);\n";
          return "";
        }
      } // namespace fragment
    } // namespace gamma

    //--------------------------------------------------------------------------
    namespace collection
    {
      namespace vertex
      {
        static const char* variables()
        {
          return
            "// Collections: variables for fetch_collection()\n"
            "#define GET_FIELD(i) texture2D(collection, vec2(float(i)/size_x,ty))\n"
            "uniform   sampler2D collection;\n"
            "uniform   vec3      collection_shape;\n"
            "attribute float     collection_index;\n";
        }

        static const char* code() // TODO add Collection const& col*/) to generate a full function
        {
          return
            "  float rows   = collection_shape.x;\n"
            "  float cols   = collection_shape.y;\n"
            "  float count  = collection_shape.z;\n"
            "  int index_x  = int(mod(collection_index, (floor(cols/(count/4.0))))) * int(count/4.0);\n"
            "  int index_y  = int(floor(collection_index / (floor(cols/(count/4.0)))));\n"
            "  float size_x = cols - 1.0;\n"
            "  float size_y = rows - 1.0;\n"
            "  float ty     = 0.0;\n"
            "  if (size_y > 0.0)\n"
            "    ty = float(index_y) / size_y;\n";
        }
      } // namespace vertex
    } // namespace collection
  } // namespace shaders

  // -----------------------------------------------------------------------------
  //! \brief
  // -----------------------------------------------------------------------------
  static void createCollectionShader(GLVertexShader& vertexShader,
                                     GLFragmentShader& fragmentShader,
                                     Collection const& col)
  {
    vertexShader
      << shaders::common::version()
      << shaders::collection::vertex::variables()
      << "\n"
      << col.toVariableShaders()
      << "\nvoid fetch_collection()\n{\n"
      << shaders::collection::vertex::code()
      << "\n  // Init fields\n"
      << "  int i = index_x;\n"
      << "  m_vec4 field;\n"
      << col.toVertexShaders()
      << "}\n\nvoid main()\n{\n"
      << "  fetch_uniforms();\n"
      << "  gl_Position = modelMatrix * viewMatrix * projectionMatrix * position;"
      << "\n}";

    fragmentShader
      << shaders::common::version()
    //  << col.fragmentVariables()
    //  << "void main()\n{\n"
      // << col.fragmentCode()
    //   << "\n}"
      ;
  }

  // -----------------------------------------------------------------------------
  //! \brief
  // -----------------------------------------------------------------------------
  static void createDepthMaterialShader(GLVertexShader& vertexShader, GLFragmentShader& fragmentShader)
  {
    vertexShader
      << shaders::common::version()
      << shaders::common::vertex::variables()
      << "\nvoid main()\n{\n"
      << "  gl_Position = projectionMatrix * modelMatrix * viewMatrix * vec4(position, 1.0);\n"
      << "}\n";

    fragmentShader
      << shaders::common::version()
      << shaders::common::fragment::variables()
      << "uniform float near;\n"
      << "uniform float far;\n"
      << "uniform float opacity;\n"
      << "\nvoid main()\n{\n"
      << "  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
      << "  float color = 1.0 - smoothstep(near, far, depth);\n"
      << "  gl_FragColor = vec4(vec3(color), opacity);\n"
      << "}\n";
  }

  // -----------------------------------------------------------------------------
  //! \brief
  // -----------------------------------------------------------------------------
  static void createNormalMaterialShader(GLVertexShader& vertexShader, GLFragmentShader& fragmentShader)
  {
    vertexShader
      << shaders::common::version()
      << shaders::common::vertex::variables()
      << "uniform mat3 normalMatrix; // = mat3(transpose(inverse(modelMatrix * viewMatrix)));\n"
      << "out vec3 vNormal;\n"
      << "\nvoid main()\n{\n"
      << "  vec4 mvPosition = modelMatrix * viewMatrix * vec4(position, 1.0);\n"
      << "  vNormal = normalMatrix * normal;\n"
      << "  gl_Position = projectionMatrix * mvPosition;\n"
      << "}\n";

    fragmentShader
      << shaders::common::version()
      << shaders::common::fragment::variables()
      << "uniform float opacity;\n"
      << "in vec3 vNormal;\n"
      << "\nvoid main()\n{\n"
      << "  gl_FragColor = vec4(0.5 * normalize(vNormal) + 0.5, opacity);\n"
      << "}\n";
  }

  // -----------------------------------------------------------------------------
  //! \note Be sure GLVertexShader::m_shader_code and GLFragmentShader::m_shader_code
  //! are empty
  // -----------------------------------------------------------------------------
  static void createBasicMaterialShader(GLVertexShader& vertexShader,
                                        GLFragmentShader& fragmentShader,
                                        BasicMaterialConfig const& config)
  {
    vertexShader
      << shaders::common::version()
      << shaders::common::vertex::variables()
      << shaders::texture::vertex::variables(config)
      << shaders::light::vertex::variables(config)
      << shaders::color::vertex::variables(config)
      << "\nvoid main()\n{\n"
      << "  vec4 mvPosition = modelMatrix * viewMatrix * vec4(position, 1.0);\n"
      << shaders::texture::vertex::code(config)
      << shaders::light::vertex::code(config)
      << shaders::color::vertex::code(config)
      << shaders::base::vertex::code(config) << "}";

    fragmentShader
      << shaders::common::version()
      << shaders::common::constants()
      << shaders::alpha::fragment::variables(config)
      << shaders::common::fragment::variables()
      << "uniform vec3 diffuse;\n"
      << "uniform float opacity;\n"
      << shaders::color::fragment::variables(config)
      << shaders::texture::fragment::variables(config)
      << shaders::light::fragment::variables(config)
      << shaders::fog::fragment::variables(config)
      << shaders::shadow::fragment::variables(config)
      << shaders::specular::fragment::variables(config)
      << "\nvoid main()\n{\n"
      << "  gl_FragColor = vec4(diffuse, opacity);\n"
      << shaders::texture::fragment::code(config)
      << shaders::alpha::fragment::code(config)
      << shaders::light::fragment::code(config)
      << shaders::color::fragment::code(config)
      << shaders::shadow::fragment::code(config)
      << shaders::gamma::fragment::code(config)
      << shaders::fog::fragment::code(config) << "}";
  }

  // -----------------------------------------------------------------------------
  //! \brief
  // -----------------------------------------------------------------------------
  static void createLineBasicMaterialShader(GLVertexShader& vertexShader, GLFragmentShader& fragmentShader)
  {
    vertexShader
      << shaders::common::version()
      << "uniform mat4 modelMatrix;\n"
      << "uniform mat4 viewMatrix;\n"
      << "uniform mat4 projectionMatrix;\n"
      << "in vec3 position;\n"
      << "in vec4 colors;\n"
      << "out vec4 vColors;\n"
      << "\nvoid main()\n{\n"
      << "  vec4 mvPosition = modelMatrix * viewMatrix * vec4(position, 1.0);\n"
      << "  gl_Position = projectionMatrix * mvPosition;\n"
      << "  vColors = colors;\n"
      << "}\n";

    fragmentShader
      << shaders::common::version()
      << "in vec4 vColors;\n"
      << "\nvoid main()\n{\n"
      << "  gl_FragColor = vColors;\n"
      << "}\n";
  }

#   pragma GCC diagnostic pop

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_SHADERLIB_HPP
