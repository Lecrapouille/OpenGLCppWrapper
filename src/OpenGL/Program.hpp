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
// OpenGLCppWrapper is distributed in the hope that it will be useful, but
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

#ifndef OPENGLCPPWRAPPER_GLPROGRAM_HPP
#  define OPENGLCPPWRAPPER_GLPROGRAM_HPP

// *****************************************************************************
//! \file GLProgram.hpp manages a list of GLSL shader codes and lists of shader
//! variables (uniforms, samplers and attributes), can initialize VAO and paint
//! them.
// *****************************************************************************

#  include "OpenGL/ShaderVertex.hpp"
#  include "OpenGL/ShaderFragment.hpp"
#  include "OpenGL/ShaderGeometry.hpp"
#  include "OpenGL/LocationAttribute.hpp"
#  include "OpenGL/LocationUniform.hpp"
#  include "OpenGL/LocationSamplers.hpp"
#  include "OpenGL/VAO.hpp"

namespace glwrap
{

// *****************************************************************************
//! \brief GLProgram manages a list of shaders and lists of shader
//! variables (Uniforms, Samplers and Attributes). A GLProgram can
//! render (draw) a Vertex Array Object (VAO) bound to it.
// *****************************************************************************
class GLProgram: public GLObject<GLenum>
{
  // Unordered map = hash table = O(1) access time.
  using mapGLLocation = std::unordered_map<std::string, GLLocation_SP>;
  using mapGLSampler = std::unordered_map<std::string, GLSampler_SP>;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. Give the preferred number of
  //! elements to create when populating VBOs (0 by default). This
  //! constructor makes no other actions.
  //!
  //! \param name the name of this instance (used for debug and traces).
  //! \param nb_vertices the preferred number of elements to create
  //! when populating VBOs.
  //----------------------------------------------------------------------------
  GLProgram(std::string const& name, size_t const nb_vertices = 0_z)
    : GLObject(name),
      m_vbo_init_size(nb_vertices)
  {}

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements from CPU and GPU.
  //----------------------------------------------------------------------------
  virtual ~GLProgram() override
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Attach a shader program (vertex or fragment or geometry) to this
  //! class. No action is immediatly made (delegated).
  //!
  //! \param shader: an instance of the class holding a vertex or
  //! fragment or geometry shader.
  //!
  //! \return the reference of this instance (*this).
  //!
  //! \note the number of shaders elements is not managed.
  //----------------------------------------------------------------------------
  GLProgram& attachShader(GLShader& shader)
  {
    DEBUG("Prog '%s' attaching Shader '%s'", cname(), shader.cname());
    m_shaders.push_back(&shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Attach shader programs (vertex and fragment and geometry) to this
  //! class. No action is immediatly made (delegated).
  //!
  //! \param vertex_shader: an instance of the class holding a vertex shader.
  //! \param fragment_shader: an instance of the class holding a fragment shader.
  //! \param geometry_shader: an instance of the class holding a geometry shader.
  //!
  //! \return the reference of this instance (*this).
  //!
  //! \note the number of shaders elements is not managed.
  //----------------------------------------------------------------------------
  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           GLGeometryShader& geometry_shader)
  {
    DEBUG("Prog '%s' attaching Shaders '%s, %s, %s'", cname(),
          vertex_shader.cname(), fragment_shader.cname(), geometry_shader.cname());
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    m_shaders.push_back(&geometry_shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Attach shader programs (vertex and fragment) to this
  //! class. No action is immediatly made (delegated).
  //!
  //! \param vertex_shader: an instance of the class holding a vertex shader.
  //! \param fragment_shader: an instance of the class holding a fragment shader.
  //!
  //! \return the reference of this instance (*this).
  //!
  //! \note the number of shaders elements is not managed.
  //----------------------------------------------------------------------------
  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader)
  {
    DEBUG("Prog '%s' attaching Shaders '%s, %s'", cname(),
          vertex_shader.cname(), fragment_shader.cname());
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind a VAO instance to this instance of GLProgram.
  //!
  //! If its the first time that the VAO is bound to the GLProgram
  //! then the VAO gets its list of VBOs and textures created. The
  //! number of elements of VBOs are reserved through the parameter
  //! \p nb_vertices passed in constructor GLProgram or set through
  //! setInitVBOSize(size_t const). No data will be filled (this is
  //! the job of the developper to do it explicitly).
  //!
  //! Else (if the VAO was bound previously) nothing is made.
  //!
  //! \note A VAO bound to a different GLProgram cannot be bind to
  //! this GLProgram.  This will produce an error. This case is
  //! refused to avoid to the developer to have a silent error with an
  //! unexpected behavior.
  //!
  //! \param vao the VAO instance to be bound with.
  //!
  //! \return false if the GLVAO cannot be bound. This case occurs for
  //! one of the following reasons: the GLProgram cannot be compiled
  //! (syntax errors in shader code) or if the VOA has been bound by
  //! another GLProgram (incompatibility).
  //----------------------------------------------------------------------------
  inline bool bind(GLVAO& vao)
  {
    DEBUG("Binding VAO '%s' to Prog '%s'", vao.cname(), cname());

    // Try compile the GLProgram
    if (unlikely(!isCompiled()))
      {
        if (!compile())
          {
            ERROR("Tried to bind VAO '%s' on a non compilable GLProgram '%s'",
                  vao.cname(), cname());
            return false;
          }
      }

    if (unlikely(0 == vao.prog))
      {
        // When binding the VAO to GLProgram for the first time:
        // create VBOs to the VAO.
        DEBUG("%s", "  First binding. VBOs will be populated {");
        initVAO(vao);
        DEBUG("%s", "  } First binding. VBOs will be populated");
      }
    else if (unlikely(m_handle != vao.prog))
      {
        // Check if VAO has been previously bind by this GLProgram. If not
        // This is probably an error of the developper.
        ERROR("Tried to bind VAO '%s' already bound to another Prog than '%s'",
              vao.cname(), cname());
        return false;
      }

    // Bind the VAO to the GLProgram
    m_vao = &vao;
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if a VAO is bound with this GLProgram.
  //!
  //! \return true if VAO and this GLProgram are coupled, else return false.
  //----------------------------------------------------------------------------
  inline bool isBound() const
  {
    return nullptr != m_vao;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if attached shaders (passed to \a attachShaders
  //! methods) have been compiled and linked with success.
  //!
  //! \return true if shaders have been linked, else return false.
  //----------------------------------------------------------------------------
  inline bool isCompiled() const
  {
    return m_compiled;
  }

  //----------------------------------------------------------------------------
  //! \brief Compile shaders if this was not already made.
  //!
  //! \note: this method is equivalent to the method \a begin but has a more
  //! explicit name.
  //!
  //! \return true if the program has been successfully compiled, else
  //! return false.
  //----------------------------------------------------------------------------
  inline bool compile()
  {
    begin();
    return isCompiled();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if this instance contains an error message (produced during
  //! the compilation or linkage of shaders).
  //!
  //! \return true if an error message is present, else return false.
  //----------------------------------------------------------------------------
  inline bool hasErrored() const
  {
    return !m_error_msg.empty();
  }

  //----------------------------------------------------------------------------
  //! \brief Return all error messages (concated by '\\n' char) produced
  //! either during the shader compilation or by an other event.
  //!
  //! \note Once this method as been called the error message is
  //! automatically cleared.
  //!
  //! \return the error message (the message can be empty).
  //----------------------------------------------------------------------------
  inline std::string getError()
  {
    std::string tmp(m_error_msg);
    m_error_msg.clear();
    return tmp;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of shader names used. This is method is
  //! mainly used for debug purpose.
  //----------------------------------------------------------------------------
  std::vector<std::string> getShaderNames()
  {
    std::vector<std::string> list;

    list.reserve(m_shaders.size());
    for (auto const& it: m_shaders)
      {
        list.push_back(it->name());
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of shaders that failed to be
  //! compiled. This is method is mainly used for debug purpose.
  //----------------------------------------------------------------------------
  std::vector<GLShader*> getFailedShaders()
  {
    std::vector<GLShader*> list;

    list.reserve(4_z);
    for (auto const& it: m_shaders)
      {
        if (!it->isCompiled())
          {
            list.push_back(it);
          }
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of uniform names. This is method is mainly
  //! used for debug purpose.
  //----------------------------------------------------------------------------
  std::vector<std::string> getUniformNames()
  {
    std::vector<std::string> list;

    list.reserve(m_uniforms.size());
    for (auto const& it: m_uniforms)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of attributes names. This is method is mainly
  //! used for debug purpose.
  //----------------------------------------------------------------------------
  std::vector<std::string> getAttributeNames()
  {
    std::vector<std::string> list;

    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of texture names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note Do not be confused with meaning of texture name. We do not refer to
  //! the jpeg, png or bmp file name but to the shader uniform name (used in
  //! shaders).
  //----------------------------------------------------------------------------
  std::vector<std::string> getSamplersNames()
  {
    std::vector<std::string> list;

    list.reserve(m_samplers.size());
    for (auto const& it: m_samplers)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of an uniform.
  //!
  //! \return true if the GLProgram contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasUniform(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_uniforms.end() != m_uniforms.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if program has a non empty list of shader uniforms.
  //!
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasUniforms() const
  {
    return 0_z != m_uniforms.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the uniform variable refered by the name
  //! and its type T.
  //!
  //! \return the reference of the shader uniform if it exists.
  //!
  //! \throw OpenGLException if the uniform is not in the list
  //! (probably due to a typo in the name) or if the type T does not match.
  //----------------------------------------------------------------------------
  template<class T>
  inline T& uniform(const char *name)
  {
    DEBUG("Prog '%s' get uniform '%s'", cname(), name);
    return getUniform<T>(name).data();
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and get the uniform variable refered by the name
  //! and its type T.
  //!
  //! \return the reference of the shader uniform if it exists.
  //!
  //! \throw OpenGLException if the uniform is not in the list
  //! (probably due to a wrong name) or if the type T does not match.
  //----------------------------------------------------------------------------
  template<class T>
  inline const T& uniform(const char *name) const
  {
    DEBUG("Prog '%s' const get uniform '%s'", cname(), name);
    return getUniform<T>(name).data();
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 4x4 matrix.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Matrix44f& matrix44f(const char *name)
  {
    return uniform<Matrix44f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 3x3 matrix.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Matrix33f& matrix33f(const char *name)
  {
    return uniform<Matrix33f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 2x2 matrix.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Matrix22f& matrix22f(const char *name)
  {
    return uniform<Matrix22f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 4D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector4f& vector4f(const char *name)
  {
    return uniform<Vector4f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 3D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector3f& vector3f(const char *name)
  {
    return uniform<Vector3f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float 2D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector2f& vector2f(const char *name)
  {
    return uniform<Vector2f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform float scalar.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline float& scalarf(const char *name)
  {
    return uniform<float>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform integer 4D vector.
  //!  \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector4i& vector4i(const char *name)
  {
    return uniform<Vector4i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform integer 3D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector3i& vector3i(const char *name)
  {
    return uniform<Vector3i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform integer 2D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector2i& vector2i(const char *name)
  {
    return uniform<Vector2i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Get the shader uniform integer scalar.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline int& scalar(const char *name)
  {
    return uniform<int>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of an attribute.
  //! \return true if the GLProgram contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasAttribute(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_attributes.end() != m_attributes.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if program has a non empty list of shader attributes.
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasAttributes() const
  {
    return 0_z != m_attributes.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of
  //! uniform texture sampler.
  //!
  //! \return true if the GLProgram contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasSampler(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_samplers.end() != m_samplers.find(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if program has a non empty list of uniform texture sampler.
  //! \return true if the list is non empty.
  //----------------------------------------------------------------------------
  inline bool hasSamplers() const
  {
    return 0_z != m_samplers.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Draw the currently bound VAO. Give the first vertex and
  //! count vertices (see glDrawArrays OpenGL official documentation
  //! for more info).
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if no VAO is bound or if VBOs have not all the same sizes.
  //----------------------------------------------------------------------------
  void draw(Mode const mode, size_t const first, size_t const count)
  {
    DEBUG("Prog '%s' draw {", cname());
    throw_if_not_compiled();
    throw_if_vao_not_bound();
    throw_if_inconsitency_attrib_sizes();

    doDraw(mode, first, count);
    DEBUG("} Prog '%s' draw", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the VAO and render it. Use as params the first and
  //! count vertices (see glDrawArrays OpenGL official documentation).
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if the VAO cannot be bound or if VBOs have not all the same
  //! sizes.
  //----------------------------------------------------------------------------
  inline void draw(GLVAO& vao, Mode const mode, size_t const first, size_t const count)
  {
    DEBUG("Prog '%s' draw VAO '%s' {", cname(), vao.cname());
    throw_if_vao_cannot_be_bound(vao);
    throw_if_inconsitency_attrib_sizes();

    doDraw(mode, first, count);
    DEBUG("} Prog '%s' draw VAO '%s'", cname(), vao.cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Render the bound VAO. Use implicit first and count vertices
  //! (see glDrawArrays OpenGL official documentation).
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if the VAO has not been bound or if VBOs have not all the same
  //! sizes.
  //----------------------------------------------------------------------------
  inline void draw(Mode const mode)
  {
    DEBUG("Prog '%s' draw %zu elements {", cname(), m_vao->m_vbos.begin()->second->size());
    throw_if_not_compiled();
    throw_if_vao_not_bound();
    throw_if_inconsitency_attrib_sizes();

    doDraw(mode, 0, static_cast<uint32_t>(m_vao->m_vbos.begin()->second->size()));
    DEBUG("} Prog '%s' draw", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Bind a VAO and render it with implicit first and count
  //! vertices (see glDrawArrays OpenGL official documentation).
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if the VAO has not been bound or if VBOs have not all the same
  //! sizes.
  //----------------------------------------------------------------------------
  inline void draw(GLVAO& vao, Mode const mode)
  {
    throw_if_vao_cannot_be_bound(vao);
    draw(mode);
  }

  //----------------------------------------------------------------------------
  //! \brief Render a VAO with the help of an vertices index.
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if the VAO has not been bound or if VBOs have not all the same
  //! sizes.
  //----------------------------------------------------------------------------
  template<class T>
  void draw(Mode const mode, GLIndexBuffer<T>& index)
  {
    DEBUG("Prog '%s' drawIndex %zu elements {", cname(), index.size());

    throw_if_not_compiled();
    throw_if_vao_not_bound();
    throw_if_inconsitency_attrib_sizes();

    DEBUG("Prog '%s' begin {", cname());
    begin();
    DEBUG("} Prog '%s' begin", cname());

    DEBUG("Prog '%s' index begin {", cname());
    index.begin();
    DEBUG("} Prog '%s' index begin", cname());

    DEBUG("%s", "Draw Elements");
    glCheck(glDrawElements(static_cast<GLenum>(mode),
                           static_cast<GLsizei>(index.size()),
                           index.type(), 0));
    DEBUG("Prog '%s' end index, VAO", cname());
    index.end();
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    DEBUG("} Prog '%s' drawIndex ", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief bind a VAO and render it with the help of an vertices index.
  //!
  //! \throw OpenGLException if the program has not been compiled or
  //! if the VAO has not been bound or if VBOs have not all the same
  //! sizes.
  //----------------------------------------------------------------------------
  template<class T>
  void draw(GLVAO& vao, Mode const mode, GLIndexBuffer<T>& index)
  {
    throw_if_vao_cannot_be_bound(vao);
    draw(mode, index);
  }

  //----------------------------------------------------------------------------
  //! \brief Choose if the usage of coming VBO created will be
  //! GL_DYNAMIC_DRAW or GL_STATIC_DRAW or GL_STREAM_DRAW. If this
  //! method is not called default usage will be GL_DYNAMIC_DRAW
  //----------------------------------------------------------------------------
  void setBufferUsage(BufferUsage const usage)
  {
    m_vbo_usage = usage;
  }

  //----------------------------------------------------------------------------
  //! \brief Change how many elements are pre-allocated when creating
  //! VBOs. If this method is not called default usage will be 0.
  //! \fixme bind cannot replace this method ?
  //----------------------------------------------------------------------------
  void setInitVBOSize(size_t const size)
  {
    m_vbo_init_size = size;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Do the draw call. This method is only made for factorizing code.
  //----------------------------------------------------------------------------
  void doDraw(Mode const mode, size_t const first, size_t const count)
  {
    DEBUG("<<<<<<<<<<<< Prog '%s' draw VAO '%s' {", cname(), m_vao->cname());

    // FIXME: A optimiser car ca prend 43 appels OpenGL alors qu'avant
    // il suffisait entre 16 et 35

    DEBUG("Prog '%s' begin {", cname());
    begin();
    DEBUG("} Prog '%s' begin", cname());

    DEBUG("Draw Arrays %u %zu %zu", static_cast<uint32_t>(mode), first, count);
    glCheck(glDrawArrays(static_cast<GLenum>(mode),
                         static_cast<GLint>(first),
                         static_cast<GLsizei>(count)));

    DEBUG("Prog '%s' end VAO", cname());
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    DEBUG(">>>>>>>>>>>> } Prog '%s'", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  /*inline virtual bool isValid() const override
  {
    DEBUG("Prog::isValid %d", isCompiled());
    return isCompiled();
    }*/

  //----------------------------------------------------------------------------
  //! \brief Throw OpenGLException if GLProgram cannot be compiled (due to
  //! errors in shaders code source).
  //----------------------------------------------------------------------------
  void throw_if_not_compiled()
  {
    if (unlikely(!isCompiled()))
      {
        throw OpenGLException("Failed OpenGL program has not been compiled");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Throw OpenGLException if VAO cannot be bound to this GLProgram
  //! (reasons are: errors in shaders code source or GLProgram already bind
  //! to another VAO).
  //----------------------------------------------------------------------------
  void throw_if_vao_cannot_be_bound(GLVAO& vao)
  {
    if (unlikely(!bind(vao)))
      {
        throw OpenGLException("Failed binding VAO to OpenGL program");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Throw OpenGLException if VAO is not bind to this GLProgram
  //! (reason: the developer forget to call \a bind.
  //----------------------------------------------------------------------------
  void throw_if_vao_not_bound()
  {
    if (unlikely(!isBound()))
      {
        throw OpenGLException("Failed OpenGL program has not been bound to a VAO");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Throw OpenGLException if the bound VAO has not all its VBO
  //! with the same size.
  //! \note this function does not check if VAO is bind. Call
  //! throw_if_vao_not_bound() before this method.
  //----------------------------------------------------------------------------
  void throw_if_inconsitency_attrib_sizes()
  {
    if (!m_vao->checkVBOSizes())
      {
        throw OpenGLException("Failed OpenGL attributes have not the same size");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Populate the VAO with a list of VBOs and textures. The
  //! number depends on the number of shader attributes and uniform
  //! texture samplers.
  //----------------------------------------------------------------------------
  void initVAO(GLVAO& vao)
  {
    // Create a list of VBOs
    for (auto& it: m_attributes)
      {
        const char *name = it.first.c_str();
        switch (it.second->dim())
          {
          case 1:
            vao.createVBO<float>(name, m_vbo_init_size, m_vbo_usage);
            break;
          case 2:
            vao.createVBO<Vector2f>(name, m_vbo_init_size, m_vbo_usage);
            break;
          case 3:
            vao.createVBO<Vector3f>(name, m_vbo_init_size, m_vbo_usage);
            break;
          case 4:
            vao.createVBO<Vector4f>(name, m_vbo_init_size, m_vbo_usage);
            break;
          default:
            throw OpenGLException("Attribute with dimension > 4 is not managed");
            break;
          }
      }

    // Create a list of textures
    for (auto& it: m_samplers)
      {
        const char *name = it.first.c_str();
        const GLenum gltype = it.second->target();
        switch (gltype)
          {
          case GL_SAMPLER_1D:
            vao.createTexture<GLTexture1D>(name);
            break;
          case GL_SAMPLER_2D:
            vao.createTexture<GLTexture2D>(name);
            //FIXME vao.createTexture<GLTextureDepth2D>(name);
            break;
          case GL_SAMPLER_3D:
            vao.createTexture<GLTexture3D>(name);
            break;
          case GL_SAMPLER_CUBE:
            vao.createTexture<GLTextureCube>(name);
            break;
          default:
            ERROR("This kind of sampler is not yet managed: %u", gltype);
            break;
          }
      }

    // Make VAO and GLprogram be coupled.
    vao.prog = m_handle;
  }

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL program.
  //!
  //! \note Contrary to VBO, GLProgram has to perform its setup() before
  //! calling activate()
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Prog '%s' create", cname());
    m_handle = glCheck(glCreateProgram());
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Activate in OpenGL the program, its attributes, its uniforms and
  //! samplers. A VAO shall be bound else nothing is made.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("Prog '%s' activate {", cname());

    if (unlikely(!isCompiled()))
      return ;
    if (unlikely(!isBound()))
      return ;

    glCheck(glUseProgram(m_handle));

    DEBUG("Prog '%s' -> VAO '%s' begin {", cname(), m_vao->cname());
    m_vao->begin();
    DEBUG("} Prog '%s' -> VAO '%s' begin", cname(), m_vao->cname());

    for (auto& it: m_attributes)
      {
        DEBUG("Prog '%s' -> VAO '%s' -> VBO '%s' begin {",
              cname(), m_vao->cname(), it.first.c_str());
        m_vao->m_vbos[it.first]->begin();
        DEBUG("} Prog '%s' -> VAO '%s' -> VBO '%s' begin",
              cname(), m_vao->cname(), it.first.c_str());

        DEBUG("Prog '%s' -> VAO '%s' -> attribute '%s' begin {",
              cname(), m_vao->cname(), it.second->cname());
        it.second->begin();
        DEBUG("} Prog '%s' -> VAO '%s' -> attribute '%s' begin {",
              cname(), m_vao->cname(), it.second->cname());
      }
    for (auto& it: m_uniforms)
      {
        DEBUG("Prog '%s' -> uniform '%s' begin {", cname(), it.second->cname());
        it.second->begin();
        DEBUG("} Prog '%s' -> uniform '%s' begin", cname(), it.second->cname());
      }
    for (auto& it: m_samplers)
      {
        // Important: activate the texture unit first before binding
        // texture.
        DEBUG("Prog '%s' -> sampler '%s' begin {", cname(), it.second->cname());
        it.second->begin();
        DEBUG("} Prog '%s' -> sample '%s' begin", cname(), it.second->cname());

        DEBUG("Prog '%s' -> VAO '%s' -> attribute '%s' begin {",
              cname(), m_vao->cname(), it.second->cname());
        m_vao->m_textures[it.first]->begin();
        DEBUG("} Prog '%s' -> VAO '%s' -> attribute '%s' begin {",
              cname(), m_vao->cname(), it.second->cname());
      }
    DEBUG("} Prog '%s' activate", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Compile and link shaders attached to this GLProgram.
  //!
  //! \return false is compilation succeeded (indicating setup has not to be
  //! redone). Return true if an error occurred (errors in the code source of
  //! shaders).
  //!
  //! \note Contrary to other GLObject, GLProgram has to perform its
  //! setup() before calling activate().
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Prog '%s' setup {", cname());
    bool failure = false;

    // Compile shaders if they have not yet compiled
    DEBUG("Prog '%s' compile shaders {", cname());
    for (auto& it: m_shaders)
      {
        it->begin();
        if (it->hasErrored())
          {
            std::string msg =
              "Shader '" + it->name() +
              "' has not been compiled: reason was '" +
              it->getError() + "'";
            ERROR("%s", msg.c_str());
            concatError(msg);
            failure = true;
          }
      }
    DEBUG("} Prog '%s' compile shaders. Failure? %u", cname(), failure);

    if (!failure)
      {
        // Attach shaders to program
        for (auto& it: m_shaders)
          {
            glCheck(glAttachShader(m_handle, it->gpuID()));
            it->attachProg(m_handle);
          }

        // Link shaders to the program
        DEBUG("Prog '%s' link shaders", cname());
        glCheck(glLinkProgram(m_handle));
        m_compiled = checkLinkageStatus(m_handle);
        if (m_compiled)
          {
            m_error_msg.clear();
            // Create the list of attributes and uniforms
            createAllLists();
            // Release shaders stored in GPU.
            detachAllShaders();
          }
      }

    DEBUG("} Prog '%s' setup. Linked? %u", cname(), m_compiled);
    return !m_compiled;
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method. Nothing is made.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Deactivate the OpenGL program, its uniform, attributes, samplers.
  //!
  //! \note A program can be deactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("Prog '%s' deactivate", cname());
    glCheck(glUseProgram(0U));

    for (auto& it: m_uniforms)
      {
        it.second->end();
      }
    for (auto& it: m_samplers)
      {
        it.second->end();
      }
    for (auto& it: m_attributes)
      {
        it.second->end();
      }

    m_vao = nullptr;
  }

  //----------------------------------------------------------------------------
  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    DEBUG("Prog '%s' release", cname());
    detachAllShaders();
    glCheck(glDeleteProgram(m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Create lists of instances holding Attribute and Uniform.
  //----------------------------------------------------------------------------
  void createAllLists()
  {
    const GLsizei bufSize = 64;
    GLchar name[bufSize];
    GLsizei length = 0;
    GLint size = 0;
    GLint count = 0;
    GLuint i;
    GLenum type;

    // Create the list of uniforms
    DEBUG("Prog '%s' populating shader uniforms and samplers {", cname());
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveUniform(m_handle, i, bufSize, &length,
                                   &size, &type, name));
        DEBUG("  Uniform #%u Type: %u Name: %s", i, type, name);
        addNewUniform(type, name);
      }
    DEBUG("} Prog '%s' populating shader uniforms and samplers", cname());

    // Create the list of attributes and list of VBOs
    DEBUG("Prog '%s' populating shader attributes {", cname());
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveAttrib(m_handle, i, bufSize, &length,
                                  &size, &type, name));
        DEBUG("  Attribute #%u Type: %u Name: %s", i, type, name);
        addNewAttribute(type, name);
      }
    DEBUG("} Prog '%s' populating shader attributes", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Create Attribute instances
  //----------------------------------------------------------------------------
  void addNewAttribute(GLenum type, const char *name)
  {
    assert(nullptr != name);

    switch (type)
      {
      case GL_FLOAT:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 1, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC2:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 2, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC3:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 3, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC4:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 4, GL_FLOAT, gpuID());
        break;
      default:
        std::string msg = "Attribute '" + std::string(name) + "' type is not managed";
        ERROR("%s", msg.c_str());
        concatError(msg);
        break;
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Create uniform instances or texture sampler instances.
  //----------------------------------------------------------------------------
  void addNewUniform(GLenum type, const char *name)
  {
    assert(nullptr != name);
    switch (type)
      {
      case GL_FLOAT:
        m_uniforms[name] = std::make_unique<GLUniform<float>>(name, 1, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2f>>(name, 2, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3f>>(name, 3, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4f>>(name, 4, GL_FLOAT, gpuID());
        break;
      case GL_INT:
        m_uniforms[name] = std::make_unique<GLUniform<int>>(name, 1, GL_INT, gpuID());
        break;
      case GL_INT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2i>>(name, 2, GL_INT, gpuID());
        break;
      case GL_INT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3i>>(name, 3, GL_INT, gpuID());
        break;
      case GL_INT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4i>>(name, 4, GL_INT, gpuID());
        break;
      case GL_FLOAT_MAT2:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix22f>>(name, 4, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_MAT3:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix33f>>(name, 9, GL_FLOAT, gpuID());
        break;
      case GL_FLOAT_MAT4:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix44f>>(name, 16, GL_FLOAT, gpuID());
        break;
      case GL_SAMPLER_1D:
        m_samplers[name] = std::make_unique<GLSampler1D>(name, m_sampler_count, gpuID());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_2D:
        m_samplers[name] = std::make_unique<GLSampler2D>(name, m_sampler_count, gpuID());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_3D:
        m_samplers[name] = std::make_unique<GLSampler3D>(name, m_sampler_count, gpuID());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_CUBE:
        m_samplers[name] = std::make_unique<GLSamplerCube>(name, m_sampler_count, gpuID());
        m_sampler_count += 1u;
        break;
      default:
        std::string msg = "Uniform '" + std::string(name) + "' type is not managed";
        ERROR("%s", msg.c_str());
        concatError(msg);
        break;
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! \return the uniform instance if found else throw an exception.
  //! \throw OpenGLException if the uniform does not exist or bad T type param.
  //----------------------------------------------------------------------------
  template<class T>
  GLUniform<T>& getUniform(const char *name)
  {
    if (unlikely(!isCompiled()))
      {
        begin();
        // TODO: check if now isCompiled() == true
      }

    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to getUniform");
      }

    if (unlikely(false == hasUniform(name)))
      {
        // TODO: create the variable: call addNewUniform
        // TODO: http://www.cplusplus.com/forum/general/21246/#msg112085
        if (isBound())
          {
            throw OpenGLException("GLUniform '" + std::string(name) +
                                  "' does not exist");
          }
        else
          {
            throw OpenGLException("GLUniform '" + std::string(name) +
                                  "' does not exist because no VAO has been bound");
          }
      }

    auto ptr = m_uniforms[name].get();
    GLUniform<T> *uniform_ptr = dynamic_cast<GLUniform<T>*>(ptr);
    if (unlikely(nullptr == uniform_ptr))
      {
        throw std::invalid_argument("GLUniform '" + std::string(name) +
                                    "' exists but has wrong template type");
      }
    return *uniform_ptr;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Release the GPU memory by removing compiled shaders.
  //----------------------------------------------------------------------------
  void detachAllShaders()
  {
    DEBUG("%s", "Prog::detachAllshaders");
    for (auto& it: m_shaders)
      {
        if (m_handle == it->attached())
          {
            glCheck(glDetachShader(m_handle, it->gpuID()));
            it->attachProg(0);
          }
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the shaders have been successfully linked.
  //!
  //! \return true if case of success, else return false.
  //----------------------------------------------------------------------------
  bool checkLinkageStatus(GLuint obj)
  {
    GLint status;

    glCheck(glGetProgramiv(obj, GL_LINK_STATUS, &status));
    if (GL_FALSE == status)
      {
        GLint length;
        glCheck(glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetProgramInfoLog(obj, length, &length, &log[0U]));
        concatError(&log[0U]);
        m_error_msg.pop_back();
        ERROR("Failed linking '%s'. Reason was '%s'", cname(), m_error_msg.c_str());
      }
    else
      {
        m_error_msg.clear();
      }
    return !!status;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat the last error to the list of errors
  //----------------------------------------------------------------------------
  void concatError(std::string const& msg)
  {
    if (!m_error_msg.empty())
      {
        m_error_msg += '\n';
      }
    m_error_msg += msg;
  }

private:

  //! \brief Hold the localiation of shader attributes.
  mapGLLocation          m_attributes;
  //! \brief Hold the localiation of shader uniforms.
  mapGLLocation          m_uniforms;
  //! \brief Hold the localisation of uniform texture sampler.
  mapGLSampler           m_samplers;
  //! \brief Hold class holding shader code source.
  //! \note pointer is to avoid copy because of deleted copy constructor.
  std::vector<GLShader*> m_shaders;
  //! \brief Bind VAO (when not null)
  GLVAO                 *m_vao = nullptr;
  //! \brief Save error messages of shader compilations.
  std::string            m_error_msg;
  //! \brief Count textures for giving them their GL_TEXTURE0 + id
  uint32_t               m_sampler_count = 0u;
  //! \brief Know if shaders have been compiled and linked.
  bool                   m_compiled = false;
  //! \brief Preferred VBO storage inside GPU (fast, low).
  BufferUsage            m_vbo_usage = BufferUsage::DYNAMIC_DRAW;
  //! \brief Reserve memory when creating VBOs.
  size_t                 m_vbo_init_size = 0_z;
};

//----------------------------------------------------------------------------
//! \class GLProgram GLProgram.hpp
//!
//! A \ref GLProgram takes as input a set of GLShaders, compile and
//! link them. As result a list of GLLocations (\ref GLUniform, \ref
//! GLSampler and \ref GLAttribute) are internaly created in the
//! GLProgram. GLLocations allow the GLProgram to populate in the
//! bound VAO (\ref GLVAO) a list of VBOs (\ref GLVertexBuffer) and a
//! list of textures (GLTexture). Finally a GLProgram can draw a VAO
//! bound to it.
//!
//! Usage example:
//! \code
//! GLVertexShader     vs;
//! GLFragmentShader   fs;
//! GLProgram          prog("prog");
//!
//! if (!m_prog.attachShaders(vs, fs).compile()) {
//!     std::cerr << "failed compiling OpenGL program. Reason was '"
//!               << m_prog.getError() << "'" << std::endl;
//!     exit();
//! }
//!
//! GLVAO              vao1("VAO1");
//! GLVAO              vao2("VAO2");
//!
//! prog.bind(vao1);
//! prog.draw(vao1, Mode::TRIANGLES);
//! prog.bind(vao2);
//! prog.draw(Mode::TRIANGLES); // vao2 is painted
//! \endcode
//!
//! Beware: this example does not show shaders or VAO initialization.
//! \see GLVertexShader, GLFragmentShader, GLVAO.
//----------------------------------------------------------------------------

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLPROGRAM_HPP
