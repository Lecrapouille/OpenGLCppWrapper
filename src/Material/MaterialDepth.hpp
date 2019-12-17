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

#ifndef OPENGLCPPWRAPPER_MATERIALDEPTH_HPP
#  define OPENGLCPPWRAPPER_MATERIALDEPTH_HPP

#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(MaterialDepth);

// *****************************************************************************
//! \brief
// *****************************************************************************
class MaterialDepth : public Material
{
public:

  MaterialDepth()
    : Material("depth", Material::Type::Depth)
  {
    createDepthMaterialShader(m_vertexShader, m_fragmentShader);
    debug();
    m_program.attachShaders(m_vertexShader, m_fragmentShader);

    near() = 1.0f;
    far() = 100.0f;
    opacity() = 1.0f;
  }

  static MaterialDepth_SP create()
  {
    return std::make_shared<MaterialDepth>();
    /* Not working beacuse we do not want to change a uniform for all materials
    auto& it = materials.find(type());
    if (it == materials.end())
      {
        MaterialDepth_SP m = std::make_shared<MaterialDepth>();
        materials[type()] = m;
        return m;
      }
    return it->second;
    */
  }

  inline float& near()
  {
    return m_program.scalarf("near");
  }

  inline float& far()
  {
    return m_program.scalarf("far");
  }

  inline float& opacity()
  {
    return m_program.scalarf("opacity");
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIALDEPTH_HPP
