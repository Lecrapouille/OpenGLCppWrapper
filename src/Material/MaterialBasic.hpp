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

#ifndef OPENGLCPPWRAPPER_MATERIALBASIC_HPP
#  define OPENGLCPPWRAPPER_MATERIALBASIC_HPP

#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(MaterialBasic);

// *****************************************************************************
//! \brief
// *****************************************************************************
class MaterialBasic : public Material
{
public:

  MaterialBasic()
    : MaterialBasic(MaterialConfig())
  {}

  MaterialBasic(MaterialConfig const config)
    : Material("basic", Material::Type::Basic),
      m_config(config)
  {
    //m_specialization = config.name();
    createBasicMaterialShader(m_vertexShader, m_fragmentShader, config);
    debug();
    m_program.attachShaders(m_vertexShader, m_fragmentShader);

    diffuse() = Color().toVector3f();
    opacity() = 1.0f;

    if (m_config.useColor)
      color() = diffuse();

    if ((m_config.useMap) || (m_config.useBumpMap) || (m_config.useSpecularMap))
      offsetTexture() = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

    // Material flags useExpFog and useFog are exclusive. Disable useFog
    if ((m_config.useExpFog) && (m_config.useFog))
      m_config.useFog = false;

    if (m_config.useFog)
      {
        fogColor() = Color().toVector3f();
        fogNear() = 0.1f;
        fogFar() = 2000.0f;
      }

    if (m_config.useExpFog)
      fogDensity() = 0.00025f;
  }


  static MaterialBasic_SP create(MaterialConfig const config)
  {
    return glwrap::make_shared<MaterialBasic>(config);
  }

  static MaterialBasic_SP create()
  {
    return std::make_shared<MaterialBasic>();
    /*auto& it = materials.find(m_specialization);
    if (it == materials.end())
      {
        MaterialBasic_SP m = std::make_shared<MaterialBasic>();
        materials[m_specialization] = m;
        return m;
      }
    return it->second;
    */
  }

  //FIXME "Color3 et m_program.color3 .color4"
  inline Vector3f& diffuse()
  {
    return m_program.vector3f("diffuse");
  }

  inline float& opacity()
  {
    return m_program.scalarf("opacity");
  }

  inline Vector3f& color() // alias to diffuse() ???
  {
    return m_program.vector3f("color");
  }

  inline Vector4f& offsetTexture()
  {
    return m_program.vector4f("offsetRepeat");
  }

  inline float& fogDensity()
  {
    return m_program.scalarf("fogDensity");
  }

  inline float& fogNear()
  {
    return m_program.scalarf("fogNear");
  }

  inline float& fogFar()
  {
    return m_program.scalarf("fogFar");
  }

  inline Vector3f& fogColor()
  {
    return m_program.vector3f("fogColor");
  }

protected:

    MaterialConfig m_config; // FIXME: necessaire ?
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIALBASIC_HPP
