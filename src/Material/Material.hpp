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

#ifndef OPENGLCPPWRAPPER_MATERIAL_HPP
#  define OPENGLCPPWRAPPER_MATERIAL_HPP

#  include "OpenGL/Program.hpp"
#  include "Material/ShaderLib.hpp"
#  include "Material/Color.hpp"
#  include "Common/Verbose.hpp"

namespace glwrap
{

DECLARE_CLASS(Material)
DECLARE_CLASS(MaterialDepth)
DECLARE_CLASS(MaterialNormal)
DECLARE_CLASS(MaterialBasic)

// *****************************************************************************
//! \brief
// *****************************************************************************
class Material //TBD :public GLObject or Program ???
{
public:

   // ---------------------------------------------------------------------------
   //! \brief
   // ---------------------------------------------------------------------------
   enum class Type { Depth, Normal, Basic };//, Phong, Particle };

  // ---------------------------------------------------------------------------
  //! \brief
  // ---------------------------------------------------------------------------
  Material(std::string const& name, Type const type)
    : m_name("material_" + name),
      m_type(type),
      vertexShader("VS_material_" + name),
      fragmentShader("FS_material_" + name),
      m_program("prog_material_" + name)
  {
    DEBUG("Create material '%s'", m_name.c_str());
  }

  virtual ~Material()
  {
    DEBUG("Destroy material '%s'", m_name.c_str());
  }

  virtual void init() = 0;

  inline GLProgram& program()
  {
    return m_program;
  }

  inline std::string const& name() const
  {
    return m_name;
  }

  inline Type type() const
  {
    return m_type;
  }

protected:

  void debug(GLVertexShader const& vertexShader, GLFragmentShader const& fragmentShader)
  {
    std::cerr
      << "Shaders for Material '" << m_name << "'" << std::endl
      << "---------------------" << std::endl
      << "Vertex Shader:" << std::endl
      << vertexShader << std::endl
      << "---------------------" << std::endl
      << "Fragment Shader:" << std::endl
      << fragmentShader << std::endl
      << "---------------------" << std::endl;
  }

protected:

  static std::map<std::string, Material_SP> materials;

protected:

  std::string      m_name;
  Type             m_type;
  GLVertexShader   vertexShader;
  GLFragmentShader fragmentShader;
  GLProgram        m_program;
};

// *****************************************************************************
//! \brief
// *****************************************************************************
class MaterialDepth : public Material
{
public:

  MaterialDepth()
    : Material("depth", Material::Type::Depth)
  {
    createDepthMaterialShader(vertexShader, fragmentShader);
    debug(vertexShader, fragmentShader);
    m_program.attachShaders(vertexShader, fragmentShader);
  }

  virtual void init() override // FIXME because uniforms cannot be reached while program is not yet compiled
  {
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

// *****************************************************************************
//! \brief
// *****************************************************************************
class MaterialNormal : public Material
{
public:

  MaterialNormal()
    : Material("normals", Material::Type::Normal)
  {
    createNormalMaterialShader(vertexShader, fragmentShader);
    debug(vertexShader, fragmentShader);
    m_program.attachShaders(vertexShader, fragmentShader);
  }

  virtual void init() override // FIXME because uniforms cannot be reached while program is not yet compiled
  {
    opacity() = 1.0f;
    normalMatrix() = Matrix33f(matrix::Identity);
  }

  static MaterialNormal_SP create()
  {
    return std::make_shared<MaterialNormal>();
    /*
    auto& it = materials.find(type());
    if (it == materials.end())
      {
        MaterialNormal_SP m = std::make_shared<MaterialNormal>();
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
    createBasicMaterialShader(vertexShader, fragmentShader, config);
    debug(vertexShader, fragmentShader);
    m_program.attachShaders(vertexShader, fragmentShader);
  }

  virtual void init() override // FIXME because uniforms cannot be reached while program is not yet compiled
  {
    diffuse() = Color().toVector3f();
    opacity() = 1.0f;

    if (m_config.useColor)
      color() = diffuse();

    if ((m_config.useMap) || (m_config.useBumpMap) || (m_config.useSpecularMap))
      offsetTexture() = Vector4f(0.0f, 0.0f, 1.0f, 1.0f);

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

  MaterialConfig m_config;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIAL_HPP
