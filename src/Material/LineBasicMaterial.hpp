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

#ifndef OPENGLCPPWRAPPER_LINES_HPP
#  define OPENGLCPPWRAPPER_LINES_HPP

namespace glwrap
{

DECLARE_CLASS(LineBasicMaterial);

// *****************************************************************************
//! \brief
// *****************************************************************************
class LineBasicMaterial : public Material
{
public:

  LineBasicMaterial(std::string const& name = "line-basic")
    : Material(name, Material::Type::Basic)
  {
    //m_specialization = config.name();
    createLineBasicMaterialShader(m_vertexShader, m_fragmentShader);
    debug();
    m_program.attachShaders(m_vertexShader, m_fragmentShader);

    //width() = 1.0f;
    //color() = Vector3f(1.0f, 0.0f, 0.0f);
  }

  static LineBasicMaterial_SP create(std::string const& name = "line-basic")
  {
    return std::make_shared<LineBasicMaterial>(name);
  }



  //inline float& width()
  //{
  //   return m_program.scalarf("width");
  //}

  //private:

  //GLVertexBuffer<Vector4f>& m_colors;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_LINES_HPP
