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

#ifndef GL_VERTEX_ARRAY_HPP
#  define GL_VERTEX_ARRAY_HPP

#  include "GLVBO.tpp"
#  include "GLTextures.hpp"
#  include <unordered_map>
#  include <vector>

class GLVAO: public IGLObject<GLenum>
{
  friend class GLProgram;

public:

  //! \brief Empty constructor without name
  GLVAO()
    : IGLObject()
  {
    m_target = GL_ARRAY_BUFFER;
  }

  //! \brief Constructor with the object name
  GLVAO(const char *name)
    : IGLObject(name)
  {
    m_target = GL_ARRAY_BUFFER;
  }

  virtual ~GLVAO() override
  {
    destroy();
  }

  //------------------------------------------------------------------
  //! \brief Return the list of VBO names
  //------------------------------------------------------------------
  std::vector<std::string> VBONames()
  {
    std::vector<std::string> list;
    list.reserve(m_vbos.size());
    for (auto& it: m_vbos)
      {
        list.push_back(it.first);
      }
    return list;
  }

  std::vector<std::string> TextureNames()
  {
    std::vector<std::string> list;
    list.reserve(m_textures.size());
    for (auto& it: m_textures)
      {
        list.push_back(it.first);
      }
    return list;
  }

  inline bool hasVBOs() const
  {
    return 0_z != m_vbos.size();
  }

  inline bool hasVBO(const char *name) const
  {
    return m_vbos.end() != m_vbos.find(name);
  }

  inline bool hasTextures() const
  {
    return 0_z != m_textures.size();
  }

  inline bool hasTexture(const char *name) const
  {
    return m_textures.end() != m_textures.find(name);
  }

  template<typename T>
  GLVertexBuffer<T>& VBO(const char *name)
  {
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to VBO()");
      }

    auto ptr = m_vbos[name].get();
    if (unlikely(nullptr == ptr))
      {
        throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                              "' does not exist");
      }

    GLVertexBuffer<T> *vbo = dynamic_cast<GLVertexBuffer<T>*>(ptr);
    if (unlikely(nullptr == vbo))
      {
        throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    DEBUG("VAO::GetVBO '%s' %p", name, vbo);
    return *vbo;
  }

  template<typename T>
  T& texture(const char *name)
  {
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to texture()");
      }

    auto ptr = m_textures[name].get();
    if (unlikely(nullptr == ptr))
      {
        throw OpenGLException("GLTexture '" + std::string(name) +
                              "' does not exist");
      }

    T* texture = dynamic_cast<T*>(ptr);
    if (unlikely(nullptr == texture))
      {
        throw OpenGLException("GLTexture '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    DEBUG("VAO::GetTexture '%s' %p", name, texture);
    return *texture;
  }

  //------------------------------------------------------------------
  //! \brief Check if all VBOs have the same size.
  //! \return true if all VBO have the same size, else return false.
  //------------------------------------------------------------------
  bool checkVBOSizes()
  {
    if (m_vbos.empty())
      {
        return true;
      }
    /* FIXME
    size_t size = m_vbos.begin().second->size();
    for (auto const& it: m_vbos)
      {
        if (size != it.second->size())
          {
            ERROR("VAO %s does not have all of its VBOs with the same size",
                  name().c_str());
            return false;
          }
      }
    */
    return true;
  }

private:

  //! \note: name duplicata is not managed because this case should
  //! never happen.
  template<typename T>
  bool createVBO(const char *name, size_t const vbo_init_size, BufferUsage const usage)
  {
    if (unlikely(hasVBO(name)))
      {
        ERROR("Try to create a VBO with name '%s' already used", name);
        return false;
      }
    m_vbos[name] = std::make_unique<GLVertexBuffer<T>>(name, vbo_init_size, usage);
    DEBUG("allocate new VBO '%s' %p", name, m_vbos[name].get());
    return true;
  }

  template<typename T>
  bool createTexture(const char *name)
  {
    if (unlikely(hasTexture(name)))
      {
        ERROR("Try to create a texture with name '%s' already used", name);
        return false;
      }
    m_textures[name] = std::make_unique<T>(name);
    DEBUG("allocate new Texture '%s' %p", name, m_textures[name].get());
    return true;
  }

  virtual bool create() override
  {
    DEBUG("VAO '%s' create", name().c_str());
    glCheck(glGenVertexArrays(1, &m_handle));
    return false;
  }

  virtual void release() override
  {
    DEBUG("VAO '%s' release", name().c_str());
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

  virtual void activate() override
  {
    DEBUG("VAO '%s' activate", name().c_str());
    glCheck(glBindVertexArray(m_handle));
  }

  virtual void deactivate() override
  {
    DEBUG("VAO '%s' deactivate", name().c_str());
    glCheck(glBindVertexArray(0U));
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

  //Callable by GLProgram
  //static void unbind()
  //{
  //  glCheck(glBindVertexArray(0U));
  //}

private:

  std::unordered_map<std::string, std::unique_ptr<IGLObject>> m_vbos;
  std::unordered_map<std::string, std::unique_ptr<IGLTexture>> m_textures;
  GLenum prog = 0; // unbinded prog
};

#endif /* GL_VERTEX_ARRAY_HPP */
