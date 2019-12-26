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

#  include "OpenGL/Shaders.hpp"
#  include "OpenGL/Attribute.hpp"
#  include "OpenGL/Uniform.hpp"
#  include "OpenGL/Samplers.hpp"
#  include "OpenGL/VAO.hpp"
#  include <unordered_map>

namespace glwrap
{

// *****************************************************************************
//! \brief This class allows to do three things:
//! - Manage a list of GLSL shaders, compile them and extract from their code
//!   source lists of shader variables (uniforms, samplers and attributes).
//! - Manage the creation of VBOs and textures when bound for the first time to
//!   a VAO (in accordance of the list of shader variables).
//! - Allow to render a VAO if bound to it.
// *****************************************************************************
class GLProgram: public GLObject<GLenum>
{

  //! \brief Memorize GLAttributes and GLUniforms.
  //! \note Unordered map = hash table = O(1) access time.
  using mapGLLocation = std::map<std::string, GLLocation_SP>;

  //! \brief Memorize GLSamplers.
  //! \note Unordered map = hash table = O(1) access time.
  using mapGLSampler = std::map<std::string, GLSampler_SP>;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Give a name to the instance. Set the number of
  //! elements to reserve when creating VBOs (0 by default). This constructor
  //! makes no other actions.
  //!
  //! \param[in] name the name of this instance (used for debug and traces).
  //! \param[in] nb_vertices number of elements to allocate when creating VBOs.
  //!
  //! \note VBO size can be resized as long as they have not been allocated
  //! inside the GPU memory.
  //----------------------------------------------------------------------------
  GLProgram(std::string const& name, size_t const vbo_size = 0_z)
    : GLObject(name),
      m_vbo_reserve(vbo_size)
  {}

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements from CPU and GPU.
  //----------------------------------------------------------------------------
  virtual ~GLProgram() override
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Attach a vertex shader or fragment shader or geometry shader to
  //! this instance. No action is immediatly made: attached shaders will be
  //! compiled on delegated moment.
  //!
  //! \note the number of shaders elements is not managed here but before the
  //! compilation.
  //!
  //! \param[in] shader: a reference to the instance holding either a vertex or
  //! fragment or geometry shader.
  //! \param[in] clear if the list has to be cleared before being filled.
  //! Set it if shaders have previously failed compiling.
  //!
  //! \return the reference of this instance (*this).
  //----------------------------------------------------------------------------
  GLProgram& attachShader(GLShader& shader, bool const clear = false)
  {
    DEBUG("Prog '%s' attaching Shader '%s'", cname(), shader.cname());
    if (clear) { m_shaders.clear(); }
    m_shaders.push_back(&shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Attach a vertex shader and a fragment shader and a geometry shader
  //! to this class. No action is immediatly made but attached shaders will be
  //! compiled on delegated moment.
  //!
  //! \note the number of shaders elements is not managed here but before the
  //! compilation.
  //!
  //! \param[in] vertex_shader: an instance of the class holding a vertex shader.
  //! \param[in] fragment_shader: an instance of the class holding a fragment shader.
  //! \param[in] geometry_shader: an instance of the class holding a geometry shader.
  //! \param[in] clear if the list has to be cleared before being filled. Set it
  //! if shaders have previously failed compiling.
  //!
  //! \return the reference of this instance (*this).
  //----------------------------------------------------------------------------
  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           GLGeometryShader& geometry_shader,
                           bool const clear = false)
  {
    DEBUG("Prog '%s' attaching Shaders '%s, %s, %s'", cname(),
          vertex_shader.cname(), fragment_shader.cname(), geometry_shader.cname());
    if (clear) { m_shaders.clear(); }
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    m_shaders.push_back(&geometry_shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Attach a vertex shader and a fragment shader to this class. No
  //! action is immediatly made but attached shaders will be compiled on
  //! delegated moment.
  //!
  //! \note the number of shaders elements is not managed here but before the
  //! compilation.
  //!
  //! \param[in] vertex_shader: an instance of the class holding a vertex shader.
  //! \param[in] fragment_shader: an instance of the class holding a fragment shader.
  //! \param[in] clear if the list has to be cleared before being filled. Set it
  //! if shaders have previously failed compiling.
  //!
  //! \return the reference of this instance (*this).
  //----------------------------------------------------------------------------
  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           bool const clear = false)
  {
    DEBUG("Prog '%s' attaching Shaders '%s, %s'", cname(),
          vertex_shader.cname(), fragment_shader.cname());
    if (clear) { m_shaders.clear(); }
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind VAO with this GLProgram instance.
  //!
  //! Two cases:
  //!   - If it's the first time that the VAO is bound to the GLProgram then the
  //!     VAO has its internal list of VBOs and textures populated. They are
  //!     refered by the variable name used in shaders (string). VBOs are
  //!     pre-allocated with the number of elements (either set from the by
  //!     GLProgram constructor or by the method bind()) but no data are filled,
  //!     this is the job of the developper to do it explicitly.
  //!   - Else, if the VAO was bound previously, nothing is made (list are not
  //!     created/updated).
  //!
  //! \note A VAO bound to a different GLProgram cannot be bind to a another
  //! GLProgram. This is automatically detected and will produce an error. This
  //! case is refused to avoid to the developer to have a silent error with an
  //! unexpected behavior.
  //!
  //! \param[in] vao the VAO to be bound with this instance of GLProgram.
  //!
  //! \return true if the GLVAO has been bound. If bounding cannot be made
  //! returns false (for one of the following reasons: the GLProgram cannot be
  //! compiled (syntax errors in shader code) or if the VOA has already been
  //! bound by another GLProgram (incompatibility).
  //----------------------------------------------------------------------------
  inline bool bind(GLVAO& vao)
  {
    DEBUG("Binding VAO '%s' to Prog '%s':", vao.cname(), cname());

    // Try compile the GLProgram if not previously compiled
    if (unlikely(!isCompiled()))
      {
        if (!compile())
          {
            ERROR("  Tried to bind VAO '%s' on a non compilable GLProgram '%s'",
                  vao.cname(), cname());
            return false;
          }
      }

    if (unlikely(0 == vao.prog))
      {
        // When binding the VAO to GLProgram for the first time:
        // populate VBOs and textures in the VAO.
        DEBUG("%s", "  First binding. VBOs will be populated {");
        initVAO(vao);
        DEBUG("%s", "  } First binding. VBOs will be populated");
      }
    else if (unlikely(m_handle != vao.prog))
      {
        // Check if VAO has been previously bind by this GLProgram. If not
        // This is probably an error of the developper.
        ERROR("  Tried to bind VAO '%s' already bound to another Prog than '%s'",
              vao.cname(), cname());
        return false;
      }

    // Bind the VAO to the GLProgram. This relationship is now unbreakable
    m_vao = &vao;
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Change how many elements shall be pre-allocated when creating VBOs
  //! and bind the VAO.
  //!
  //! \note This method is interesting when VAO has never been bound, else use
  //! this method without the parameter vbo_size.
  //!
  //! \param[in] vao the VAO to be bound with this instance of GLProgram.
  //! \param[in] vbo_size number of elements to allocate when creating VBOs.
  //! \return true if the GLVAO has been bound with success.
  //----------------------------------------------------------------------------
  inline bool bind(GLVAO& vao, size_t const vbo_size)
  {
    m_vbo_reserve = vbo_size;
    return bind(vao);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if a VAO is bound with this GLProgram.
  //!
  //! \return true if VAO and this GLProgram are already bound, else return false.
  //----------------------------------------------------------------------------
  inline bool isBound() const
  {
    return nullptr != m_vao;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if attached shaders (passed to attachShaders() methods) have
  //! been compiled and linked with success.
  //!
  //! \return true if shaders have been compiled and linked successfuly, else
  //! return false.
  //----------------------------------------------------------------------------
  inline bool isCompiled() const
  {
    return m_compiled;
  }

  //----------------------------------------------------------------------------
  //! \brief Compile attached shaders in the case it has not already been done.
  //!
  //! \note: this method is equivalent to the method begin() but with a more
  //! explicit name and return the status of the compilation.
  //!
  //! \return true if the program has been successfully compiled, else return
  //! false. A internal error message is created in case of error.
  //----------------------------------------------------------------------------
  inline bool compile()
  {
    begin();
    bool res = isCompiled();
    if (!isContextCreated())
      concatError(" OpenGL context is not yet created");
    return res;
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
  //! \brief Return all error messages (concated by '\\n' char) produced either
  //! during the shader compilation or by an other event.
  //!
  //! \note Once this method as been called the error message is
  //! automatically cleared.
  //!
  //! \return the error message (the message is empty if no error have occured).
  //----------------------------------------------------------------------------
  inline std::string getError()
  {
    std::string msg;

    if (hasErrored())
      {
        std::vector<GLShader*> shaders;

        msg += "The following shaders failed to compile:\n";
        getFailedShaders(shaders);
        for (auto& it: shaders)
          {
            msg += ' ';
            msg += it->name();
          }
        msg += "\nReason was:\n";
        msg += m_error_msg;
      }

    m_error_msg.clear();
    return msg;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of names of attached shaders. This is method is
  //! mainly used for debug purpose.
  //!
  //! \note if no shader have never been attached to a GLProgram this method
  //! will return an empty list.
  //!
  //! \param[in,out] list the list where to insert shader names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getShaderNames(std::vector<std::string>& list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_shaders.size());
    for (auto const& it: m_shaders)
      {
        list.push_back(it->name());
      }
    return list.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of shaders that failed to be compiled (if happened).
  //! This is method is mainly used for debug purpose.
  //!
  //! \note if no shader have been attached to a GLProgram or no compilation
  //! error happened or no compilation have been made this method will return an
  //! empty list.
  //!
  //! \param[in,out] list the list where to insert shaders.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getFailedShaders(std::vector<GLShader*>& list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_shaders.size());
    for (auto const& it: m_shaders)
      {
        if (!it->isCompiled())
          {
            list.push_back(it);
          }
      }
    return m_shaders.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of uniform names. This is method is mainly used for
  //! debug purpose.
  //!
  //! \note if no shader have been attached to a GLProgram or compilation error
  //! happened or no compilation have been made or the shader code did not use
  //! uniforms this method will return an empty list.
  //!
  //! \param[in,out] list the list where to insert attributes names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getUniformNames(std::vector<std::string>& list, bool const clear = true)
  {
    // TODO list = m_uniform_names.to_list();
    // TODO for (MapType::iterator it = m.begin(); it != m.end(); ++it)
    // TODO { v.push_back( it->second ); }
    if (clear) { list.clear(); }
    list.reserve(m_uniforms.size());
    for (auto const& it: m_uniforms)
      {
        list.push_back(it.first);
      }
    return m_uniforms.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of attributes names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note if no shader have been attached to a GLProgram or compilation error
  //! happened or no compilation have been made this method will return an empty
  //! list.
  //!
  //! \param[in,out] list the list where to insert attributes names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getAttributeNames(std::vector<std::string>& list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
      {
        list.push_back(it.first);
      }
    return m_attributes.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of texture names. This is method is mainly
  //! used for debug purpose.
  //!
  //! \note Do not confuse samplers with textures. In this method, we do not
  //! refer to the file name with jpeg, png or bmp ... extensions but to the
  //! GLSL uniform name (used inside shader code sources).
  //!
  //! \note if no shader have been attached to a GLProgram or compilation error
  //! happened or no compilation have been made or the shader code did not use
  //! sampler, this method will return an empty list.
  //!
  //! \param[in,out] list the list where to insert sampler names.
  //! \param[in] if the list has to be cleared before being filled.
  //!
  //! \return the number of inserted elements.
  //----------------------------------------------------------------------------
  size_t getSamplersNames(std::vector<std::string>& list, bool const clear = true)
  {
    if (clear) { list.clear(); }
    list.reserve(m_samplers.size());
    for (auto const& it: m_samplers)
      {
        list.push_back(it.first);
      }
    return m_samplers.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the name passed as parameter matches the name of an
  //! uniform variable used in GLSL shaders.
  //!
  //! \return true if the GLProgram contains it, else return false.
  //----------------------------------------------------------------------------
  inline bool hasUniform(const char *name) const
  {
// TODO: m_uniform_names[name]
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
// TODO: 0_z != m_uniform_names.size();
    return 0_z != m_uniforms.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the uniform variable refered by its name and by
  //! its type T.
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
    return getUniform<T>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the uniform variable refered by its name and by
  //! its type T.
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
    return getUniform<T>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 4x4 matrix. This method
  //! wraps the \a uniform() method hidding the misery of the template.
  //----------------------------------------------------------------------------
  inline Matrix44f& matrix44f(const char *name)
  {
    return uniform<Matrix44f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 3x3 matrix. This method
  //! wraps the \a uniform() method hidding the misery of the template.
  //----------------------------------------------------------------------------
  inline Matrix33f& matrix33f(const char *name)
  {
    return uniform<Matrix33f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 2x2 matrix.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Matrix22f& matrix22f(const char *name)
  {
    return uniform<Matrix22f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 4D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector4f& vector4f(const char *name)
  {
    return uniform<Vector4f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 3D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector3f& vector3f(const char *name)
  {
    return uniform<Vector3f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float 2D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector2f& vector2f(const char *name)
  {
    return uniform<Vector2f>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform float scalar.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline float& scalarf(const char *name)
  {
    return uniform<float>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform integer 4D vector.
  //!  \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector4i& vector4i(const char *name)
  {
    return uniform<Vector4i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform integer 3D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector3i& vector3i(const char *name)
  {
    return uniform<Vector3i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform integer 2D vector.
  //! This method wraps the \a uniform() method hidding the misery of
  //! the template.
  //----------------------------------------------------------------------------
  inline Vector2i& vector2i(const char *name)
  {
    return uniform<Vector2i>(name);
  }

  //----------------------------------------------------------------------------
  //! \brief Locate and return the shader uniform integer scalar.
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
    likely(0_z != index.size()) ? draw(mode, index) : draw(mode);
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
  //! \brief
  //----------------------------------------------------------------------------
  void throw_if_incompatible_number_of_shaders()
  {
    size_t no_vertex_shaders = 0_z;
    size_t no_fragment_shaders = 0_z;

    for (auto const& it: m_shaders)
      {
        switch (it->target())
          {
          case GL_VERTEX_SHADER:
            ++no_vertex_shaders;
            if (no_vertex_shaders > 1_z)
              throw OpenGLException("has too many vertex shaders. One is needed");
            if (!it->loaded())
              throw OpenGLException("Vertex shader has empty GLSL code");
            break;
          case GL_FRAGMENT_SHADER:
            ++no_fragment_shaders;
            if (no_fragment_shaders > 1_z)
              throw OpenGLException("Prog '%s' has too many fragment shaders. One is needed");
            if (!it->loaded())
              throw OpenGLException("Fragment shader has empty GLSL code");
            break;
          case GL_GEOMETRY_SHADER:
            break;
          }
      }
  }

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
    if (!m_vao->checkLoadTextures())
      {
        throw OpenGLException("Failed OpenGL textures have not all been loaded");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Populate the VAO with a list of VBOs and textures. The
  //! number depends on the number of shader attributes and uniform
  //! texture samplers.
  //----------------------------------------------------------------------------
  void initVAO(GLVAO& vao)
  {
    // Create a list of VBOs. TODO: manage integers
    for (auto& it: m_attributes)
      {
        const char *name = it.first.c_str();
        switch (it.second->dim())
          {
          case 1:
            // FIXME: simply vao.VBO<T> =  ?
            // FIXME: complexity O(n^2) tests
            vao.createVBO<float>(name, m_vbo_reserve, m_vbo_usage);
            break;
          case 2:
            vao.createVBO<Vector2f>(name, m_vbo_reserve, m_vbo_usage);
            break;
          case 3:
            vao.createVBO<Vector3f>(name, m_vbo_reserve, m_vbo_usage);
            break;
          case 4:
            vao.createVBO<Vector4f>(name, m_vbo_reserve, m_vbo_usage);
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

    if (likely(m_shaders.size() >= 2_z))
      {
        throw_if_incompatible_number_of_shaders();

        // Compile shaders if they have not yet compiled
        DEBUG("Prog '%s' compile shaders {", cname());
        for (auto& it: m_shaders)
          {
            DEBUG("Prog '%s' compile shader '%s'", cname(), it->cname());
            it->begin();
            if (it->hasErrored())
              {
                std::string msg = " " + it->name() + ":\n   " + it->getError();
                concatError(msg);
                failure = true;
              }
          }
        DEBUG("} Prog '%s' compile shaders. Failure? %u", cname(), failure);
      }
    else
      {
        ERROR("Prog '%s' has not enough shaders. Need >= 2 but got %zu",
              cname(), m_shaders.size());
        DEBUG("} Prog '%s' setup. Linked? %u", cname(), m_compiled);
        failure = true;
      }

    if (!failure)
      {
        // Attach shaders to program
        for (auto& it: m_shaders)
          {
            glCheck(glAttachShader(m_handle, it->handle()));
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
    GLuint location;
    GLenum type;

    // Create the list of uniforms
    DEBUG("Prog '%s' populating shader uniforms and samplers {", cname());
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    location = static_cast<GLuint>(count);
    while (location--)
      {
        glCheck(glGetActiveUniform(m_handle, location, bufSize, &length,
                                   &size, &type, name));
        DEBUG("  Uniform #%u Type: %u Name: %s", location, type, name);
        if (m_uniforms.find(name) == m_uniforms.end())
          {
            // Uniform has never been refered before the compilation of the
            // GLProgram: create a new instance.
            addNewUniform(type, name);
          }
        else
          {
            // Uniform has never refered before the compilation of the
            // GLProgram: update it by setting the OpenGL id of the program.
            m_uniforms[name]->setProgram(m_handle);
          }
      }
    DEBUG("} Prog '%s' populating shader uniforms and samplers", cname());

    // Create the list of attributes and list of VBOs
    DEBUG("Prog '%s' populating shader attributes {", cname());
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    location = static_cast<GLuint>(count);
    while (location--)
      {
        glCheck(glGetActiveAttrib(m_handle, location, bufSize, &length,
                                  &size, &type, name));
        DEBUG("  Attribute #%u Type: %u Name: %s", location, type, name);
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
        m_attributes[name] = std::make_unique<GLAttribute>(name, 1, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC2:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 2, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC3:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 3, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC4:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 4, GL_FLOAT, handle());
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
        m_uniforms[name] = std::make_unique<GLUniform<float>>(name, 1, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2f>>(name, 2, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3f>>(name, 3, GL_FLOAT, handle());
        break;
      case GL_FLOAT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4f>>(name, 4, GL_FLOAT, handle());
        break;
      case GL_INT:
        m_uniforms[name] = std::make_unique<GLUniform<int>>(name, 1, GL_INT, handle());
        break;
      case GL_INT_VEC2:
        m_uniforms[name] = std::make_unique<GLUniform<Vector2i>>(name, 2, GL_INT, handle());
        break;
      case GL_INT_VEC3:
        m_uniforms[name] = std::make_unique<GLUniform<Vector3i>>(name, 3, GL_INT, handle());
        break;
      case GL_INT_VEC4:
        m_uniforms[name] = std::make_unique<GLUniform<Vector4i>>(name, 4, GL_INT, handle());
        break;
      case GL_FLOAT_MAT2:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix22f>>(name, 4, GL_FLOAT, handle());
        break;
      case GL_FLOAT_MAT3:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix33f>>(name, 9, GL_FLOAT, handle());
        break;
      case GL_FLOAT_MAT4:
        m_uniforms[name] = std::make_unique<GLUniform<Matrix44f>>(name, 16, GL_FLOAT, handle());
        break;
      case GL_SAMPLER_1D:
        m_samplers[name] = std::make_unique<GLSampler1D>(name, m_sampler_count, handle());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_2D:
        m_samplers[name] = std::make_unique<GLSampler2D>(name, m_sampler_count, handle());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_3D:
        m_samplers[name] = std::make_unique<GLSampler3D>(name, m_sampler_count, handle());
        m_sampler_count += 1u;
        break;
      case GL_SAMPLER_CUBE:
        m_samplers[name] = std::make_unique<GLSamplerCube>(name, m_sampler_count, handle());
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
  GLUniform<T>& getUniform(const char *name);

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
            glCheck(glDetachShader(m_handle, it->handle()));
            it->attachProg(0);
          }
      }
    m_shaders.clear();
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
        glCheck(glGetProgramInfoLog(obj, length - 1, &length, &log[0U]));
        concatError(&log[0U]);
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

protected:

  //----------------------------------------------------------------------------
  //! \brief Reset states.
  //----------------------------------------------------------------------------
  virtual void onReset() override
  {
    m_attributes.clear();
    m_uniforms.clear();
    m_samplers.clear();
    m_shaders.clear();
    m_vao = nullptr;
    m_error_msg.clear();
    m_sampler_count = 0u;
    m_compiled = false;
    m_vbo_usage = BufferUsage::STATIC_DRAW;
    m_vbo_reserve = 0_z;
  }

  template<class T>
  void createUniform(const char *name);

private:

  //! \brief Hold the localization of shader attributes.
  mapGLLocation          m_attributes;
  //! \brief Hold the localization of shader uniforms.
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
  BufferUsage            m_vbo_usage = BufferUsage::STATIC_DRAW;
  //! \brief Reserve memory when creating VBOs.
  size_t                 m_vbo_reserve = 0_z;
};

template<>
inline void GLProgram::createUniform<float>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<float>>(name, 1, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Vector2f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector2f>>(name, 2, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Vector3f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector3f>>(name, 3, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Vector4f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector4f>>(name, 4, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Matrix22f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Matrix22f>>(name, 4, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Matrix33f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Matrix33f>>(name, 9, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<Matrix44f>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Matrix44f>>(name, 16, GL_FLOAT, handle());
}

template<>
inline void GLProgram::createUniform<int>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<int>>(name, 1, GL_INT, handle());
}

template<>
inline void GLProgram::createUniform<Vector2i>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector2i>>(name, 2, GL_INT, handle());
}

template<>
inline void GLProgram::createUniform<Vector3i>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector3i>>(name, 3, GL_INT, handle());
}

template<>
inline void GLProgram::createUniform<Vector4i>(const char *name)
{
  m_uniforms[name] = std::make_unique<GLUniform<Vector4i>>(name, 4, GL_INT, handle());
}

template<>
inline void GLProgram::createUniform<GLSampler1D>(const char *name)
{
  m_samplers[name] = std::make_unique<GLSampler1D>(name, m_sampler_count, handle());
  m_sampler_count += 1u;
}

template<>
inline void GLProgram::createUniform<GLSampler2D>(const char *name)
{
  m_samplers[name] = std::make_unique<GLSampler2D>(name, m_sampler_count, handle());
  m_sampler_count += 1u;
}

template<>
inline void GLProgram::createUniform<GLSampler3D>(const char *name)
{
  m_samplers[name] = std::make_unique<GLSampler3D>(name, m_sampler_count, handle());
  m_sampler_count += 1u;
}

template<>
inline void GLProgram::createUniform<GLSamplerCube>(const char *name)
{
  m_samplers[name] = std::make_unique<GLSamplerCube>(name, m_sampler_count, handle());
  m_sampler_count += 1u;
}

template<class T>
GLUniform<T>& GLProgram::getUniform(const char *name)
{
  if (unlikely(nullptr == name))
    throw OpenGLException("nullptr passed to getUniform");

  if (likely(isCompiled()))
    {
      auto it = m_uniforms.find(name);
      if (likely(m_uniforms.end() != it))
        {
          GLUniform<T> *uniform = dynamic_cast<GLUniform<T>*>(it->second.get());
          if (likely(nullptr != uniform))
            return *uniform;

          throw std::invalid_argument("GLUniform '" + std::string(name) +
                                      "' exists but has wrong template type");
        }

      throw OpenGLException("GLUniform '" + std::string(name) + "' does not exist");
    }
  else
    {
      DEBUG("GLProgram '%s' not compiled: find or create uniform '%s'", cname(), name);
      if (likely(m_uniforms.end() == m_uniforms.find(name)))
        createUniform<T>(name);

      GLUniform<T> *uniform = dynamic_cast<GLUniform<T>*>(m_uniforms[name].get());
      return *uniform;
    }
}

//----------------------------------------------------------------------------
//! \class GLProgram Program.hpp
//! \ingroup OpenGL
//!
//! GLProgram is a class managing a list of GLSL GLShader (vertex shader,
//! fragment shader and geometry shader), compile them and extract:
//!   - their uniform variables (GLUniform) and allow to change their values.
//!   - their attribute variables GLAttribute which are used for creating the
//!     list of Vertex Buffer Objects (GLVBO) to the GLVAO (Vertex Array
//!     Object) bound to this instance of GLProgram (note that filling VBO is
//!     made from GLVAO's methods).
//!   - their sampler variables GLSamplers which are used for creating the list
//!     of textures (GLTexture1D, GLTexture2D, GLTexture3D, GLTextureCube) to
//!     the GLVAO (Vertex Array Object) bound to this instance of GLProgram
//!     (note that filling VBO is made from GLVAO's methods).
//! Finally, a GLProgram can render (draw) the GLVAO bound to it (note that
//! multiple VAO can be bound to the same GLProgram).
//!
//! Usage example (beware, this example does not show shaders initialization or VAO
//! initialization!)
//!
//! \code
//! GLVertexShader vs;
//! GLFragmentShader fs;
//! GLProgram prog("prog");
//!
//! if (!m_prog.attachShaders(vs, fs).compile()) {
//!     std::cerr << "failed compiling OpenGL program. Reason was '"
//!               << m_prog.getError() << "'" << std::endl;
//!     exit();
//! }
//!
//! GLVAO vao1("VAO1");
//! GLVAO vao2("VAO2");
//!
//! prog.bind(vao1);
//! prog.draw(vao1, Mode::TRIANGLES);
//! prog.bind(vao2);
//! prog.draw(Mode::TRIANGLES); // vao2 is painted
//! \endcode
//----------------------------------------------------------------------------

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLPROGRAM_HPP
