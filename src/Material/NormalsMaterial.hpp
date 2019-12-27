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

#ifndef OPENGLCPPWRAPPER_MATERIALNORMALS_HPP
#  define OPENGLCPPWRAPPER_MATERIALNORMALS_HPP

#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(NormalsMaterial);

// *****************************************************************************
//! \brief
// *****************************************************************************
class NormalsMaterial : public Material
{
public:

  NormalsMaterial(std::string const& name = "normals")
    : Material(name, Material::Type::Normals)
  {
    createNormalMaterialShader(m_vertexShader, m_fragmentShader);
    debug();
    m_program.attachShaders(m_vertexShader, m_fragmentShader);

    opacity() = 1.0f;
    normalMatrix() = Matrix33f(matrix::Identity);
  }

  static NormalsMaterial_SP create(std::string const& name = "normals")
  {
    return std::make_shared<NormalsMaterial>(name);
    /*
    auto& it = materials.find(type());
    if (it == materials.end())
      {
        NormalsMaterial_SP m = std::make_shared<NormalsMaterial>();
        materials[type()] = m;
        return m;
      }
    return it->second;
    */
  }

  inline float& opacity()
  {
    return m_program.scalarf("opacity");
  }

  inline Matrix33f& normalMatrix()
  {
    return m_program.matrix33f("normalMatrix");
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIALNORMALS_HPP
