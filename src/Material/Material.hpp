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

DECLARE_CLASS(Material);

// *****************************************************************************
//! \brief
// *****************************************************************************
class Material //TBD :public GLObject or Program ???
{
public:

   // ---------------------------------------------------------------------------
   //! \brief
   // ---------------------------------------------------------------------------
   enum class Type { Depth, Normals, Basic };//, Phong, Particle };

  // ---------------------------------------------------------------------------
  //! \brief
  // ---------------------------------------------------------------------------
  Material(std::string const& name, Type const type)
    : m_name("material_" + name),
      m_type(type),
      m_vertexShader("VS_material_" + name),
      m_fragmentShader("FS_material_" + name),
      m_program("prog_material_" + name)
  {
    DEBUG("Create material '%s'", m_name.c_str());
  }

  virtual ~Material()
  {
    DEBUG("Destroy material '%s'", m_name.c_str());
  }

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

  void debug()
  {
    std::cerr
      << "Shaders for Material '" << m_name << "'" << std::endl
      << "---------------------" << std::endl
      << "Vertex Shader:" << std::endl
      << m_vertexShader << std::endl
      << "---------------------" << std::endl
      << "Fragment Shader:" << std::endl
      << m_fragmentShader << std::endl
      << "---------------------" << std::endl;
  }

protected:

  static std::map<std::string, Material_SP> materials;

protected:

  std::string      m_name;
  Type             m_type;
  GLVertexShader   m_vertexShader;
  GLFragmentShader m_fragmentShader;
  GLProgram        m_program;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIAL_HPP
