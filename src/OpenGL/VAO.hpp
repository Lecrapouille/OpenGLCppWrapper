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

#  include "Math/Matrix.hpp"
#  include "OpenGL/VBO.hpp"
#  include "OpenGL/Texture1D.hpp"
#  include "OpenGL/Texture2D.hpp"
#  include "OpenGL/Texture3D.hpp"
#  include <map>

namespace glwrap
{

// Used by SceneGraph
DECLARE_CLASS(GLVAO)

// *****************************************************************************
//! \brief Handle the OpenGL representation of 3D models. Hold a list of VBO and
//! textures created by a GLProgram.
// *****************************************************************************
class GLVAO: public GLObject<GLenum>
{
  //! \brief GLProgram directly modifies GLVAO states.
  friend class GLProgram;

  //! \brief Memorize Vertex Buffer Objects (VBOs).

  //! \note Unordered map = hash table = O(1) access time but the last does not
  //! invalidate references and iterator when inserting, which is needed by
  //! Shape3D.
  using mapGLBuffer = std::map<std::string, IGLBuffer_UP>;

  //! \brief Memorize Textures.
  //! \note Unordered map = hash table = O(1) access time but the last does not
  //! invalidate references and iterator when inserting, which is needed by
  //! Shape3D.
  using mapGLTexture = std::map<std::string, IGLTexture_UP>;

public:

  //----------------------------------------------------------------------------

  //! \brief Constructor. Give a name to the instance. This name is mainly used
  //! for logs as debug purpose. This constructor makes no other actions.
  //!
  //! \param[in] name the name of the VAO instance.
  //----------------------------------------------------------------------------
  GLVAO(std::string const& name)
    : GLObject(name)
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
  //! \brief Return the list of VBO names. VBO names come from names of
  //! attributes variables used inside GLSL shaders. This method is mainly
  //! used for debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram this method will
  //! return an empty list.
  //!
  //! \param[in,out] list the list where to insert VBO names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getVBONames(std::vector<std::string> &list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_vbos.size());
    for (auto& it: m_vbos)
      {
        list.push_back(it.first);
      }
    return m_vbos.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of sampler names. Samplers come from names of
  //! uniform variables used in GLSL shaders. This is method is mainly used for
  //! debug purpose.
  //!
  //! \note if the VAO has never been bound to a GLProgram this method will
  //! return an empty list.
  //!
  //! \param[in,out] list the list where to insert sampler names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \note Do not be confused with the sense of 'sampler name'. We do not refer
  //! to the name of the jpeg, png or bmp file. We refer to the shader sampler
  //! name (GLSL code). The GLTexture holds the acces of the file. To access to
  //! it do this VAO.texture2D["sampler_name"].name().
  //!
  //! \todo: get the tuple sampler name and texture name ?
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getSamplerNames(std::vector<std::string>& list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_textures.size());
    for (auto& it: m_textures)
      {
        list.push_back(it.first);
      }
    return m_textures.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance holds a GLIndexBuffer. GLIndexBuffer is an
  //! and index of vertices. Indeed vertices are usually shared by several
  //! meshes of the 3D model. Indexing them allows to mimize the size of the GPU
  //! memory.
  //!
  //! \return true if the instance holds a GLIndexBuffer.
  //----------------------------------------------------------------------------
  inline bool hasIndex()
  {
    return nullptr != m_index.get(); // TODO? && (0_z != m_index->size());
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index of type GL_UNSIGNED_INT.
  //! \throw OpenGLException if the index is not of type GL_UNSIGNED_INT.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint32_t>& index32(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint32_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index of type GL_UNSIGNED_SHORT.
  //! \throw OpenGLException if the index is not of type GL_UNSIGNED_SHORT.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint16_t>& index16(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint16_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the index type of GL_UNSIGNED_BYTE.
  //!
  //! \throw OpenGLException if the index is not of type GL_UNSIGNED_BYTE.
  //!
  //! This method wraps the \a index() method hidding the misery of the
  //! template.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer<uint8_t>& index8(BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
  {
    return index<uint8_t>(usage);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance has VBOs.
  //!
  //! \note Having no VBO generally means that this instance of VAO have not yet
  //! been bound to a GLProgram. See the bind() method.
  //!
  //! \return true if this instance has VBOs.
  //----------------------------------------------------------------------------
  inline bool hasVBOs() const
  {
    return 0_z != m_vbos.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance holds a VBO named with the name passed as
  //! parameter.
  //!
  //! \return true if contains the named VBO, else return false.
  //----------------------------------------------------------------------------
  inline bool hasVBO(const char *name) const
  {
    return m_vbos.end() != m_vbos.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance has textures.
  //!
  //! \note Having no textures generally means that the shader has no sampler or
  //! this instance of VAO have not yet been bound to a GLProgram. See the bind()
  //! method.
  //!
  //! \return true if this instance has textures.
  //----------------------------------------------------------------------------
  inline bool hasTextures() const
  {
    return 0_z != m_textures.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Alias for hasTextures()
  //----------------------------------------------------------------------------
  inline bool hasSamplers() const
  {
    return 0_z != m_textures.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance holds a sampler named with the name passed
  //! as parameter.
  //!
  //! \return true if contains the named sampler, else return false.
  //----------------------------------------------------------------------------
  inline bool hasSampler(const char *name) const
  {
    return m_textures.end() != m_textures.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 4D vector of type
  //! float.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector4f>& vector4f(const char *name)
  {
    return VBO<Vector4f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 3D vector of type
  //! float.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vector3f(const char *name)
  {
    return VBO<Vector3f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 2D vector of type
  //! float.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector2f>& vector2f(const char *name)
  {
    return VBO<Vector2f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a float scalar.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<float>& scalarf(const char *name)
  {
    return VBO<float>(name);
  }

#if 0
  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 4D vector of type
  //! int.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector4f>& vector4i(const char *name)
  {
    return VBO<Vector4i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 3D vector of type
  //! int.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vector3i(const char *name)
  {
    return VBO<Vector3i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 2D vector of type
  //! int.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector2f>& vector2i(const char *name)
  {
    return VBO<Vector2i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a int scalar.
  //!
  //! This method wraps the \a VBO() method hidding the misery of the template.
  //!
  //! \throw OpenGLException if the VBO does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<int>& scalari(const char *name)
  {
    return VBO<int>(name);
  }
#endif

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 1D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //!
  //! \throw OpenGLException if the texture does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLTexture1D& texture1D(const char *name)
  {
    return texture<GLTexture1D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 2D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //!
  //! \throw OpenGLException if the texture does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLTexture2D& texture2D(const char *name)
  {
    return texture<GLTexture2D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a 3D texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //!
  //! \throw OpenGLException if the texture does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLTexture3D& texture3D(const char *name)
  {
    return texture<GLTexture3D>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the named VBO holding a cubic texture.
  //!
  //! This method wraps the \a texture() method hidding the misery of the
  //! template.
  //!
  //! \throw OpenGLException if the texture does not exist or does not have the
  //! correct type.
  //----------------------------------------------------------------------------
  inline GLTextureCube& textureCube(const char *name)
  {
    return texture<GLTextureCube>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if all VBOs have the same size (same number of elements). The
  //! purpose of this method is purely for security and/or debugging. You can
  //! call it after filling a VBO or after the GLWindow::setup() to check if all
  //! VBOs have been init and you did not forget one.
  //!
  //! \return true if all VBO have the same size, else return false.
  //----------------------------------------------------------------------------
  bool checkVBOSizes()
  {
    DEBUG("VAO '%s' check VBO sizes", cname());
    if (likely(m_vbo_size_verified))
      {
        return true;
      }

    if (unlikely(prog == 0 || m_vbos.empty()))
      {
        ERROR("VAO '%s' is not yet bound to a GLProgram", cname());
        return false;
      }

    size_t size = m_vbos.begin()->second->size();
    for (auto& it: m_vbos)
      {
        if (unlikely(size != it.second->size()))
          {
            ERROR("VAO '%s' does not have all of its VBOs with the same size",
                  cname());
            for (auto& itt: m_vbos)
              {
                ERROR("=> VBO '%s' size is %zu", itt.first.c_str(), itt.second->size());
              }
            return false;
          }
      }
    m_vbo_size_verified = true;
    return true;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Locate and get the reference of the VBO refered by the name and by
  //! its type T. If possible cache your VBO reference because this function is
  //! not fast.
  //!
  //! \return the reference of the VBO if it exists.
  //!
  //! \throw OpenGLException if the VBO is not in the list (probably due to a
  //! typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
  template<typename T>
  GLVertexBuffer<T>& VBO(const char *name)
  {
    DEBUG("VAO '%s' get VBO '%s'", cname(), name);
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to VBO()");
      }

    auto ptr = m_vbos[name].get();
    if (unlikely(nullptr == ptr))
      {
        if (prog == 0 || m_vbos.empty())
          {
            throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                                  "' does not exist because VAO '" + cname()
                                  + "' is not bound to a GLProgram");
          }
        else
          {
            throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                                  "' does not exist");
          }
      }

    GLVertexBuffer<T> *vbo = dynamic_cast<GLVertexBuffer<T>*>(ptr);
    if (unlikely(nullptr == vbo))
      {
        throw OpenGLException("GLVertexBuffer '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    m_vbo_size_verified = false;
    return *vbo;
  }

  //----------------------------------------------------------------------------
  //! \brief Get the reference of GLIndexBuffer refered by its type T. If the
  //! GLIndexBuffer did not exist then it is created before returning its
  //! reference.
  //!
  //! \throw OpenGLException if the type T is not correct.
  //----------------------------------------------------------------------------
  template<typename T>
  GLIndexBuffer<T>& index(BufferUsage const usage)
  {
    DEBUG("VAO '%s' get index", cname());
    auto ptr = m_index.get();
    if (unlikely(nullptr == ptr))
      {
        m_index = std::make_unique<GLIndexBuffer<T>>(name() + "_index", usage);
        ptr = m_index.get();
      }

    GLIndexBuffer<T> *ib = dynamic_cast<GLIndexBuffer<T>*>(ptr);
    if (unlikely(nullptr == ib))
      {
        throw OpenGLException("GLIndexBuffer exists but has wrong template type");
      }

    return *ib;
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the reference of ther texture refered by its type T
  //! and by the given the sampler name used in GLSL code.
  //!
  //! \return the reference of the texture if it exists.
  //!
  //! \throw OpenGLException if the texture is not in the list or if the type T
  //! does not match or by a typo in the name of the sampler.
  //----------------------------------------------------------------------------
  template<typename T>
  T& texture(const char *name)
  {
    DEBUG("VAO '%s' get texture '%s'", cname(), name);
    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to texture()");
      }

    auto ptr = m_textures[name].get();
    if (unlikely(nullptr == ptr))
      {
        if (prog == 0 || m_vbos.empty())
          {
            throw OpenGLException("GLTexture '" + std::string(name) +
                                  "' does not exist because VAO '" + cname()
                                  + "' is not bound to a GLProgram");
          }
        else
          {
            throw OpenGLException("GLTexture '" + std::string(name) +
                                  "' does not exist");
          }
      }

    T* tex = dynamic_cast<T*>(ptr);
    if (unlikely(nullptr == tex))
      {
        throw OpenGLException("GLTexture '" + std::string(name) +
                              "' exists but has wrong template type");
      }

    return *tex;
  }

  //----------------------------------------------------------------------------
  //! \brief Create a Vertex Buffer Object. Called by GLProgram when populating
  //! the bound VAO with VBOs from attribute names used in shader GLSL code.
  //!
  //! \param[in] name the attribute name for the VBO.
  //! \param[in] vbo_init_size reserve number of elements for the VBO.
  //! \param[in] usage type of VBO (stream, static or dynamic).
  //! \tparam T float or Vector2f or Vector3f or Vector4f or
  //!             int or Vector2i or Vector3i or Vector4i.
  //!
  //! \note \a name duplicata is not managed because this case should never happen
  //! (meaning two attribute names are used for different type which is not
  //! allowed and detected by the GLSL compiler).
  //!
  //! \return true if the VBO has been created, else return false.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createVBO(const char *name, size_t const vbo_init_size, BufferUsage const usage)
  {
    if (unlikely(hasVBO(name)))
      {
        ERROR("VAO '%s' Tried to create a VBO with name '%s' already used",
              cname(), name);
        return false;
      }
    DEBUG("VAO '%s' creating a new VBO '%s' of %zu elements of %zu bytes",
          cname(), name, vbo_init_size, sizeof (T));
    m_vbos[name] = std::make_unique<GLVertexBuffer<T>>(name, vbo_init_size, usage);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Create a texture. Called by GLProgram when populating the bound VAO
  //! with textures frome sampler names used in shader GLSL code.
  //!
  //! \param[in] name the sampler name for the texture.
  //! \tparam T GLTexture1D or GLTexture2D or GLTexture3D or GLTextureCube.
  //!
  //! \note name duplicata is not managed because this case should never happen
  //! (meaning two attribute names are used for different type which is not
  //! allowed and detected by the GLSL compiler).
  //!
  //! \return true if the texture has been created, else return false.
  //----------------------------------------------------------------------------
  template<typename T>
  bool createTexture(const char *name)
  {
    if (unlikely(hasSampler(name)))
      {
        ERROR("VAO '%s' Tried to create a texture with name '%s' already used",
              cname(), name);
        return false;
      }
    m_textures[name] = std::make_unique<T>(name);
    DEBUG("VAO '%s' has a new texture '%s'", cname(), name);
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

private:

  //! \brief Hold VBOs populated by the a bound GLProgram form attributes in
  //! GLSL shader code.
  //! \note ideally, instead of holding IGLBuffer we should hold GLBuffer<T> but
  //! this is not possible in C++. So I have no idea how to make it correctly.
  mapGLBuffer m_vbos;
  //! \brief Hold textures populated by the a bound GLProgram form samplers in
  //! GLSL shader code.
  mapGLTexture m_textures;
  //! \brief Optionally hold vertex indices.
  IGLBuffer_UP m_index = nullptr;
  //! \brief Hold the ID of the bound GLProgam. Note this varibale is modified
  //! directly by the GLProgam.
  GLenum prog = 0;
  //! \brief boolean avoiding to make uncessary checks on VBO size.
  bool m_vbo_size_verified = false;
};

//----------------------------------------------------------------------------
//! \class GLVAO VAO.hpp
//! \ingroup OpenGL
//!
//! GLVAO is a class handling the OpenGL representation of a 3D model: it holds
//! a list of Vertex Buffer Objects (GLVBO) and textures (GLTexture1D,
//! GLTexture2D, GLTexture3D, GLTextureCube). Theses lists are created by a
//! GLProgram once a GLVAO has been bound to it. Note that once bound, a GLVAO
//! cannot be bound to another GLProgram. Finally, for rendering the 3D model,
//! just get the GLVAO be drawn by the associated GLProgram.
//! Usage example (beware, this example does not show shaders initialization or VAO
//! initialization!)
//!
//! Usage example (beware, this example does not show GLProgram initialization)
//!
//! \code
//! GLProgram prog("prog");
//! GLVAO vao("VAO");
//! prog.bind(vao);
//! vao.vector3f("position") = { Vector3f(1.0, 2.0, 3.0); ... }
//! vao.vector2f("UV") = { Vector2f(1.0, 2.0); ... }
//! GLTexture2D& texture = vao.texture2D("texID");
//! texture.load("foobar.jpg");
//! prog.draw(vao, Mode::TRIANGLES, 0, 36);
//! prog.draw(vao, Mode::TRIANGLES);
//! vao.vector2f("UV") *= 4.0f;
//! prog.draw(vao);
//! \endcode
//----------------------------------------------------------------------------

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
