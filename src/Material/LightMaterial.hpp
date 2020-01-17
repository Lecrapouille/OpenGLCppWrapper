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

#ifndef OPENGLCPPWRAPPER_LIGHTMATERIAL_HPP
#  define OPENGLCPPWRAPPER_LIGHTMATERIAL_HPP


#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(LightMaterial);

// *****************************************************************************
//! \brief
// *****************************************************************************
class LightMaterial : public Material
{
public:

  LightMaterial(std::string const& name = "light-material")
    : LightMaterial(LightMaterialConfig(), name)
  {}

  LightMaterial(LightMaterialConfig const& config, std::string const& name = "basic")
    : Material(name, Material::Type::Basic),
      m_config(config)
  {
    createLightMaterialShader(m_vertexShader, m_fragmentShader, config);
    debug();
    m_program.attachShaders(m_vertexShader, m_fragmentShader);

    diffuse() = Color().toVector3f();
    opacity() = 1.0f;

    if (m_config.useColor)
      color() = diffuse();

    if (m_config.useAlphaTest)
      alphaTest() = 0.5f;

    if ((m_config.useMap) || (m_config.useBumpMap) || (m_config.useSpecularMap))
      offsetTexture() = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

    // Material flags useExpFog and useFog are exclusive. Disable useFog
    if ((m_config.useExpFog) && (m_config.useFog))
      m_config.useFog = false;

    if (m_config.useFog)
      {
        fogColor() = Vector3f(0.5f, 0.5f, 0.5f);
        fogNear() = 1.0f;
        fogFar() = 10.0f;
      }

    if (m_config.useExpFog)
      fogDensity() = 0.00025f;
  }


  static BasicMaterial_SP create(LightMaterialConfig const config, std::string const& name = "basic")
  {
    return glwrap::make_shared<LightMaterial>(config, name);
  }

  static BasicMaterial_SP create(std::string const& name = "basic")
  {
    return std::make_shared<LightMaterial>(name);
    /*auto& it = materials.find(m_specialization);
    if (it == materials.end())
      {
        BasicMaterial_SP m = std::make_shared<LightMaterial>();
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

  inline float& alphaTest()
  {
    return m_program.scalarf("ALPHATEST");
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

  LightMaterialConfig m_config;
};

} // namespace glwrap
#endif // OPENGLCPPWRAPPER_LIGHTMATERIAL_HPP
