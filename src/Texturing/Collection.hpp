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
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_TEXTURING_COLLECTION_HPP
#  define OPENGLCPPWRAPPER_TEXTURING_COLLECTION_HPP

#  include "OpenGL/Texture2D.hpp"
#  include <algorithm>

namespace
{
  static const std::string locations[3] = { "attribute ", "varying ", "uniform " };
  static const std::string types[7] = { "mat4 ", "vec4 ", "vec2 ", "mat3 ", "vec3 ", "float ", "float " };
  static const std::string stores[5] = { "", ".w", ".zw", ".yzw", ".xyzw" };
  static const std::string shifts[4] = { ".xyzw", ".yzw", ".zw", ".w" };
}

namespace glwrap
{
  //! \note orderner by order of placement
  enum VarType { MAT4, VEC4, VEC2, MAT3, VEC3, SCALAR, PADDING };
  enum LocType { LOCAL, SHARED, GLOBAL, ATTRIBUTE = LOCAL, VARYING = SHARED, UNIFORM = GLOBAL };

class DType
{
public:

  DType(uint32_t const padding)
    : m_name("padding"), m_vartype(VarType::PADDING),
      m_loctype(LocType::LOCAL), m_count(padding)
  {}

  DType(const char* n, LocType const t, float const f)
    : m_name(n), m_vartype(VarType::SCALAR),
      m_loctype(t), m_scal(f), m_count(1u)
  {}

  DType(const char* n, LocType const t, Vector2f const& v)
    : m_name(n), m_vartype(VarType::VEC2),
      m_loctype(t), m_vec2(v), m_count(2u)
  {}

  DType(const char* n, LocType const t, Vector3f const& v)
    : m_name(n), m_vartype(VarType::VEC3),
      m_loctype(t), m_vec3(v), m_count(3u)
  {}

  DType(const char* n, LocType const t, Vector4f const& v)
    : m_name(n), m_vartype(VarType::VEC4),
      m_loctype(t), m_vec4(v), m_count(4u)
  {}

  DType(const char* n, LocType const t, Matrix33f const& m)
    : m_name(n), m_vartype(VarType::MAT3),
      m_loctype(t), m_mat3(m), m_count(9u)
  {}

  DType(const char* n, LocType const t, Matrix44f const& m)
    : m_name(n), m_vartype(VarType::MAT4),
      m_loctype(t), m_mat4(m), m_count(16u)
  {}

  std::string toVar() const
  {
    return locations[m_loctype] + types[m_vartype] + m_name + ";\n";
  }

  inline uint32_t count() const
  {
    return m_count;
  }

  inline std::string const& name() const
  {
    return m_name;
  }

private:

  std::string m_name;
  VarType     m_vartype;
  LocType     m_loctype;
  // TODO union {
  float       m_scal;
  Vector2f    m_vec2;
  Vector3f    m_vec3;
  Vector4f    m_vec4;
  Matrix33f   m_mat3;
  Matrix44f   m_mat4;
  // }
  uint32_t    m_count;
};

//! \brief
DECLARE_CLASS(Collection);

// *****************************************************************************
//! \brief a 2D Texture storing the depth information of the scene.
// *****************************************************************************
class Collection: public GLFloatTexture2D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //----------------------------------------------------------------------------
  Collection(std::string const& name)
    : GLFloatTexture2D(name)
  {}

  //! \brief
  //! \param size number of items to be stored in the texture)
  //! \param initList initializer list of Item data type.
  void setup(uint32_t const size, std::initializer_list<DType> initList)
  {
    m_dtypes = initList;
    assert(m_dtypes.size() != 0_z);

    // Compute the number of float used by the struct and round it to the first
    // multiple of 4 floats. Add padding.
    uint32_t count = 0u;
    for (auto const& it: m_dtypes)
      count += it.count();
    uint32_t nc = (count + 3u) & ~0x03u; // next multiple of 4 floats
    if (nc > count)
      m_dtypes.push_back({nc - count});
    count = nc;

    // Compute texture dimension
    // TODO OpenGL context pas forcement cree
    uint32_t const maxTextureSize = getMaxTextureSize<uint32_t>();

    m_width = maxTextureSize;
    m_height = 1u + size / m_width;
    m_cols = maxTextureSize / (count / 4u);
    m_rows = size / m_cols + ((size % m_cols) ? 1u : 0u);
  }

  std::string toVariableShaders() const
  {
    std::string code;
    for (auto const& it: m_dtypes)
      {
        code += it.toVar();
      }
    return code;
  }

  std::string toVertexShaders() const
  {
    std::string code;
    uint32_t count, shift;
    uint32_t store = 0u;
    uint32_t size;

    for (auto const& it: m_dtypes)
      {
        shift = 0u;
        count = it.count();
        size = count;

        while (count)
          {
            if (store == 0u)
              {
                store = 4u;
                code += "\n  field = GET_FIELD(i++);\n";
              }
            uint32_t i = std::min(std::min(4u - shift, count), store);
            std::string st = stores[store].substr(0, i+1);
            std::string sh = shifts[shift].substr(0, i+1);
            std::cout << st << " " << sh << " " << i << std::endl;

            if (size > 1_z)
              code += "  v_" + it.name() + sh + " = field" + st + ";\n";
            else
              code += "  v_" + it.name() + " = field" + st + ";\n";

            count -= i;
            shift += i;
            store -= i;
          }
      }

    return code;
  }

private:

  uint32_t m_cols = 0_z;
  uint32_t m_rows = 0_z;
  std::vector<DType> m_dtypes;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_TEXTURING_COLLECTION_HPP
