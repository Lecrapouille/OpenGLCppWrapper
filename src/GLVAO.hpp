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

#ifndef OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
#  define OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP

// *****************************************************************************
//! \file GLVAO.hpp handles the OpenGL representation of 3D models.
// *****************************************************************************

#  include "Matrix.hpp"
#  include "GLVBO.hpp"
#  include "GLTextures.hpp"
#  include <unordered_map>
#  include <vector>

namespace glwrap
{

// *****************************************************************************
//! \class GLVAO GLVAO.hpp
//!
//! \brief Class handling the OpenGL representation of a 3D model.
//!
//! GLVAO has its list of VBOs and textures populated by a GLProgram once
//! bind. GLVAO is drawn by a GLProgram.
// *****************************************************************************
class GLVAO: public IGLObject<GLenum>
{
  //! \brief GLProgram directly modifies GLVAO states.
  friend class GLProgram;

  // Unordered map = hash table = O(1) access time.
  using IGLObjectPtr = std::unique_ptr<IGLObject>;
  using IGLTexturePtr = std::unique_ptr<IGLTexture>;
  using mapIGLObject = std::unordered_map<std::string, IGLObjectPtr>;
  using mapGLTexture = std::unordered_map<std::string, IGLTexturePtr>;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance (used for debug and traces).
  //----------------------------------------------------------------------------
  GLVAO(std::string const& name)
    : IGLObject(name)
  {
    m_target = GL_ARRAY_BUFFER;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLVAO() override
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of VBO names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram the return
  //! lit will be empty.
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //! \brief Return the list of textures names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram the return
  //! lit will be empty.
  //!
  //! \note Do not be confused with meaning of texture name. We do not
  //! refer to the jpeg, png or bmp file name but to the shader
  //! uniform name (used in shaders).
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //! \brief Check if program has a non empty list of VBOs.
  //!
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasVBOs() const
  {
    return 0_z != m_vbos.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of
  //! a VBO.
  //!
  //! \return true if contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasVBO(const char *name) const
  {
    return m_vbos.end() != m_vbos.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if program has a non empty list of textures.
  //!
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasTextures() const
  {
    return 0_z != m_textures.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of
  //! a texture.
  //!
  //! \return true if contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasTexture(const char *name) const
  {
    return m_textures.end() != m_textures.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the VBO refered by the name and its type T.
  //!
  //! \return the reference of the VBO if it exists.
  //!
  //! \throw OpenGLException if the VBO is not in the list
  //! (probably due to a typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
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

    DEBUG("VAO::GetVBO '%s'", name);
    return *vbo;
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 4D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector4f>& vector4f(const char *name)
  {
    return VBO<Vector4f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 3D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vector3f(const char *name)
  {
    return VBO<Vector3f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float 2D vector.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector2f>& vector2f(const char *name)
  {
    return VBO<Vector2f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the VBO float scalar.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<float>& scalarf(const char *name)
  {
    return VBO<float>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the texture refered by the name and its type T.
  //!
  //! \return the reference of the texture if it exists.
  //!
  //! \throw OpenGLException if the VBO is not in the list
  //! (probably due to a typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
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

    DEBUG("VAO::GetTexture '%s'", name);
    return *texture;
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 1D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture1D& texture1D(const char *name)
  {
    return texture<GLTexture1D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 2D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture2D& texture2D(const char *name)
  {
    return texture<GLTexture2D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the 3D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTexture3D& texture3D(const char *name)
  {
    return texture<GLTexture3D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the cubic texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLTextureCube& textureCube(const char *name)
  {
    return texture<GLTextureCube>(name);
  }

  //------------------------------------------------------------------
  //! \brief Check if all VBOs have the same size.
  //! \return true if all VBO have the same size, else return false.
  //------------------------------------------------------------------
  bool checkVBOSizes()
  {
    if (prog == 0 || m_vbos.empty())
      {
        return false;
      }
    /* FIXME
    size_t size = m_vbos.begin().second->size();
    for (auto const& it: m_vbos)
      {
        if (size != it.second->size())
          {
            ERROR("VAO %s does not have all of its VBOs with the same size",
                  cname());
            return false;
          }
      }
    */
    return true;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief
  //!
  //! \note: name duplicata is not managed because this case should
  //! never happen.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createVBO(const char *name, size_t const vbo_init_size, BufferUsage const usage)
  {
    if (unlikely(hasVBO(name)))
      {
        ERROR("Try to create a VBO with name '%s' already used", name);
        return false;
      }
    m_vbos[name] = std::make_unique<GLVertexBuffer<T>>(name, vbo_init_size, usage);
    DEBUG("allocate new VBO '%s'", name);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //!
  //! \note: name duplicata is not managed because this case should
  //! never happen.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createTexture(const char *name)
  {
    if (unlikely(hasTexture(name)))
      {
        ERROR("Try to create a texture with name '%s' already used", name);
        return false;
      }
    m_textures[name] = std::make_unique<T>(name);
    DEBUG("allocate new Texture '%s'", name);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL VAO.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("VAO '%s' create", cname());
    glCheck(glGenVertexArrays(1, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the VAO to OpenGL.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("VAO '%s' activate", cname());
    glCheck(glBindVertexArray(m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method. Nothing is made.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method. Nothing is made.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the VAO from OpenGL.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("VAO '%s' deactivate", cname());
    glCheck(glBindVertexArray(0U));
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL VAO.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    DEBUG("VAO '%s' release", cname());
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

  //Callable by GLProgram
  //static void unbind()
  //{
  //  glCheck(glBindVertexArray(0U));
  //}

private:

  //! \brief Hold VBOs.
  mapIGLObject m_vbos;
  //! \brief Hold textures.
  mapGLTexture m_textures;
  //! \brief Hold the ID of the bound GLProgam.
  GLenum prog = 0;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
