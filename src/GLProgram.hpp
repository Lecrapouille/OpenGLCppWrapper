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

#ifndef GLPROGRAM_HPP
#  define GLPROGRAM_HPP

//! \brief This file contains the class managing OpenGL shaders :
//! read, compile, load into the GPU.

#  include "GLShaders.hpp"
#  include "GLLocation.hpp"
#  include <unordered_map>

// TODO: verifier les GLVariables non init dans le GPU

// **************************************************************
//! \class GLProgram GLProgram.hpp
//!
//! \brief
// **************************************************************
class GLProgram: public IGLObject<GLenum>
{
  using GLLocationPtr = std::unique_ptr<GLLocation>;
  using mapGLLocation = std::unordered_map<std::string, GLLocationPtr>;

public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  GLProgram(std::string const& name)
    : IGLObject(name)
  {
  }

  //------------------------------------------------------------------
  //! \brief Empty constructor and set the number of elements to
  //! reserve when allocating VBOs to binded VOA.
  //------------------------------------------------------------------
  GLProgram(std::string const& name, size_t const nb_vertices)
    : IGLObject(name),
      m_vbo_init_size(nb_vertices)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. Release GPU memory
  //------------------------------------------------------------------
  virtual ~GLProgram() override
  {
    destroy();
  }

  GLProgram& attachShader(GLShader& shader)
  {
    DEBUG("Prog::attachShader");
    m_shaders.push_back(&shader);
    return *this;
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader,
                           GLGeometryShader& geometry_shader)
  {
    DEBUG("Prog::attachShaders");
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    m_shaders.push_back(&geometry_shader);
    return *this;
  }

  GLProgram& attachShaders(GLVertexShader&   vertex_shader,
                           GLFragmentShader& fragment_shader)
  {
    DEBUG("Prog::attachShaders");
    m_shaders.push_back(&vertex_shader);
    m_shaders.push_back(&fragment_shader);
    return *this;
  }

  inline bool bind(GLVAO& vao)
  {
    DEBUG("Gonna bind Prog '%s' with VAO named '%s'", name().c_str(), vao.name().c_str());

    // Try compile the GLProgram
    if (unlikely(!compiled()))
      {
        if (!compile())
          {
            ERROR("Tried binding VAO on a non compilable GLProgram");
            return false;
          }
      }

    if (unlikely(0 == vao.prog))
      {
        // When binding the VAO to GLProgram for the first time:
        // create VBOs to the VAO.
        DEBUG("Prog '%s' will init VAO named '%s'", name().c_str(), vao.name().c_str());
        initVAO(vao);
      }
    else if (unlikely(m_handle != vao.prog))
      {
        // Check if VAO has been previously binded by this GLProgram. If not
        // This is probably an error of the developper.
        ERROR("You tried to bind VAO %s which never been binded by GLProg %s",
              vao.name().c_str(), name().c_str());
        return false;
      }

    // Bind the VAO to the GLProgram
    m_vao = &vao;
    return true;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline bool binded() const
  {
    return nullptr != m_vao;
  }

  //------------------------------------------------------------------
  //! \brief Check if the program has been linked with success.
  //! Note: the method name is compiled() but should be linked() because
  //! compile is reserved for shaders.
  //------------------------------------------------------------------
  inline bool compiled() const
  {
    return m_compiled;
  }

  //------------------------------------------------------------------
  //! \brief Compile the program (aka for begin() method but with a
  //! more explicit name).
  //! \return true if the progam has been succesfully compiled, else
  //! return false.
  //------------------------------------------------------------------
  inline bool compile()
  {
    begin();
    return compiled();
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message
  //------------------------------------------------------------------
  inline bool hasErrored() const
  {
    return !m_error_msg.empty();
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message. member variable is then
  //! cleared.
  //------------------------------------------------------------------
  inline std::string error()
  {
    std::string tmp(m_error_msg);
    m_error_msg.clear();
    return tmp;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of shader names
  //------------------------------------------------------------------
  std::vector<std::string> shaderNames()
  {
    std::vector<std::string> list;

    list.reserve(m_shaders.size());
    for (auto const& it: m_shaders)
      {
        list.push_back(it->name());
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of failed shaders
  //------------------------------------------------------------------
  std::vector<std::string> failedShaders()
  {
    std::vector<std::string> list;

    list.reserve(4_z);
    for (auto const& it: m_shaders)
      {
        if (!it->compiled())
          {
            list.push_back(it->name());
          }
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of unifom names
  //------------------------------------------------------------------
  std::vector<std::string> uniformNames()
  {
    std::vector<std::string> list;

    list.reserve(m_uniforms.size());
    for (auto const& it: m_uniforms)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Return the list of unifom names
  //------------------------------------------------------------------
  std::vector<std::string> attributeNames()
  {
    std::vector<std::string> list;

    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
      {
        list.push_back(it.first);
      }
    return list;
  }

  std::vector<std::string> texturesNames()
  {
    std::vector<std::string> list;

    list.reserve(m_textures.size());
    for (auto const& it: m_textures)
      {
        list.push_back(it.first);
      }
    return list;
  }

  //------------------------------------------------------------------
  //! \brief Check if the uniform variable exists in the shader code
  //------------------------------------------------------------------
  inline bool hasUniform(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_uniforms.end() != m_uniforms.find(name);
  }

  //------------------------------------------------------------------
  //! \brief Check program has uniforms.
  //------------------------------------------------------------------
  inline bool hasUniforms() const
  {
    return 0_z != m_uniforms.size();
  }

  //------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! Return the reference of the data.
  //------------------------------------------------------------------
  template<class T>
  inline T& uniform(const char *name)
  {
    return getUniform<T>(name).data();
  }

  template<class T>
  inline const T& uniform(const char *name) const
  {
    return getUniform<T>(name).data();
  }

  inline Matrix44f& matrix44f(const char *name)
  {
    return uniform<Matrix44f>(name);
  }

  inline Matrix33f& matrix33f(const char *name)
  {
    return uniform<Matrix33f>(name);
  }

  inline Matrix22f& matrix22f(const char *name)
  {
    return uniform<Matrix22f>(name);
  }

  inline Vector4f& vector4f(const char *name)
  {
    return uniform<Vector4f>(name);
  }

  inline Vector3f& vector3f(const char *name)
  {
    return uniform<Vector3f>(name);
  }

  inline Vector2f& vector2f(const char *name)
  {
    return uniform<Vector2f>(name);
  }

  inline float& scalarf(const char *name)
  {
    return uniform<float>(name);
  }

  inline Vector4i& vector4i(const char *name)
  {
    return uniform<Vector4i>(name);
  }

  inline Vector3i& vector3i(const char *name)
  {
    return uniform<Vector3i>(name);
  }

  inline Vector2i& vector2i(const char *name)
  {
    return uniform<Vector2i>(name);
  }

  inline int& scalar(const char *name)
  {
    return uniform<int>(name);
  }

  //------------------------------------------------------------------
   //! \brief Check if the attribute variable exists in the shader code
  // FIXME if begin() a reussi
  // FIXME: peut on avoir un nom doublon attrib / unifo ??
  //------------------------------------------------------------------
  inline bool hasAttribute(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_attributes.end() != m_attributes.find(name);
  }

  //------------------------------------------------------------------
  //! \brief Check if program has attributes.
  //------------------------------------------------------------------
  inline bool hasAttributes() const
  {
    return 0_z != m_attributes.size();
  }

  inline bool hasTexture(const char *name) const
  {
    if (unlikely(nullptr == name)) return false;
    return m_textures.end() != m_textures.find(name);
  }

  inline bool hasTextures() const
  {
    return 0_z != m_textures.size();
  }

  //------------------------------------------------------------------
  //! \brief Render the binded VAO. Use as params the first and count
  //! vertices (see glDrawArrays OpenGL official documentation).
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  void draw(Primitive const mode, GLint first, GLsizei count)
  {
    DEBUG("Prog '%s' draw {", name().c_str());
    throw_if_not_compiled();
    throw_if_vao_not_binded();
    throw_if_inconsitency_attrib_sizes();

    // FIXME: A optimiser car ca prend 43 appels OpenGL alors qu'avant
    // il suffisait entre 16 et 35
    begin();
    //m_vao->begin();
    glCheck(glDrawArrays(static_cast<GLenum>(mode), first, count));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    //m_vao->end();
    end();
    DEBUG("} Prog '%s' draw", name().c_str());
  }

  //------------------------------------------------------------------
  //! \brief Bind the VAO and render it. Use as params the first and
  //! count vertices (see glDrawArrays OpenGL official documentation).
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  void draw(GLVAO& vao, Primitive const mode, GLint first, GLsizei count)
  {
    throw_if_vao_cannot_be_binded(vao);
    draw(mode, first, count);
  }

  //------------------------------------------------------------------
  //! \brief Render the binded VAO. Use implicit first and count vertices
  //! (see glDrawArrays OpenGL official documentation).
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  inline void draw(Primitive const /*mode*/)
  {
    ERROR("Draw with implicit number of vertices is not yet implemented");
    //throw_if_not_compiled();
    //throw_if_inconsitency_attrib_sizes();
    //draw(static_cast<GLenum>(mode), 0, m_attributes.begin()->second->size());
  }

  //------------------------------------------------------------------
  //! \brief Bind a VAO and render it with implicit first and count
  //! vertices (see glDrawArrays OpenGL official documentation).
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  inline void draw(GLVAO& vao, Primitive const mode)
  {
    throw_if_vao_cannot_be_binded(vao);
    draw(mode);
  }

  //------------------------------------------------------------------
  //! \brief Render a VAO with the help of an vertices index.
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  template<class T>
  void draw(Primitive const mode, GLIndexBuffer<T>& index)
  {
    DEBUG("Prog::drawIndex %d elements", index.size());

    throw_if_not_compiled();
    throw_if_vao_not_binded();
    throw_if_inconsitency_attrib_sizes();

    //m_vao->begin();
    begin();
    index.begin();
    glCheck(glDrawElements(static_cast<GLenum>(mode), index.size(), index.type(), 0));
    index.end();
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
    //m_vao->end();
  }

  //------------------------------------------------------------------
  //! \brief bind a VAO and render it with the help of an vertices index.
  //! \throw if the program has not been compiled or if the VAO has not
  //! been binded or if VBOs have not all the same sizes.
  //------------------------------------------------------------------
  template<class T>
  void draw(GLVAO& vao, Primitive const mode, GLIndexBuffer<T>& index)
  {
    throw_if_vao_cannot_be_binded(vao);
    draw(mode, index);
  }

  //------------------------------------------------------------------
  //! \brief Choose if the usage of coming VBO created will be
  //! GL_DYNAMIC_DRAW or GL_STATIC_DRAW or GL_STREAM_DRAW. If this
  //! method is not called default usage will be GL_DYNAMIC_DRAW
  //------------------------------------------------------------------
  void setBufferUsage(BufferUsage const usage)
  {
    m_vbo_usage = usage;
  }

  //------------------------------------------------------------------
  //! \brief Change how many elements are pre-allocated when creating
  //! VBOs.
  //------------------------------------------------------------------
  void setInitVBOSize(size_t const size)
  {
    m_vbo_init_size = size;
  }

private:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  /*inline virtual bool isValid() const override
  {
    DEBUG("Prog::isValid %d", compiled());
    return compiled();
    }*/

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void throw_if_not_compiled()
  {
    if (unlikely(!compiled()))
      {
        throw OpenGLException("Failed OpenGL program has not been compiled");
      }
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void throw_if_vao_cannot_be_binded(GLVAO& vao)
  {
    if (unlikely(!bind(vao)))
      {
        throw OpenGLException("Failed binding VAO to OpenGL program");
      }
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void throw_if_vao_not_binded()
  {
    if (unlikely(!binded()))
      {
        throw OpenGLException("Failed OpenGL program has not been binded to a VAO");
      }
  }

  //------------------------------------------------------------------
  //! \brief check if the binded VAO has all its VBO with the
  //! same size.
  //! \note this function does not check if VAO is binded call before
  //! throw_if_vao_not_binded().
  //------------------------------------------------------------------
  void throw_if_inconsitency_attrib_sizes()
  {
    if (!m_vao->checkVBOSizes())
      {
        throw OpenGLException("Failed OpenGL attributes have not the same size");
      }
  }

  //------------------------------------------------------------------
  //! \brief Create the VAO which will contain the list of VBOs. The
  //! list of VBOs is created in accordance of the list of attributes.
  //------------------------------------------------------------------
  void initVAO(GLVAO& vao)
  {
    throw_if_not_compiled();

    // Create VBOs and attach them in the VAO.
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
          }
      }

    // Create all texture samplers
    for (auto& it: m_textures)
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
            break;
            //FIXME
            //case GL_SAMPLER_2D_DEPTH:
            //vao.createTexture<GLTextureDepth2D>(name);
            //break;
          case GL_SAMPLER_3D:
            vao.createTexture<GLTexture3D>(name);
            break;
          default:
            ERROR("This kind of sampler is not yet managed: %u", gltype);
            break;
          }
      }

    vao.prog = m_handle;
  }

  //------------------------------------------------------------------
  //! \brief
  //! \note Contrary to VBO, GLProgram has to perform its setup() before
  //! calling activate()
  //------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Prog '%s' create", name().c_str());
    m_handle = glCheck(glCreateProgram());
    return false;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual bool setup() override
  {
    bool failure = false;

    // Compile shaders if they have not yet compiled
    DEBUG("Prog '%s' setup: compile shaders", name().c_str());
    for (auto& it: m_shaders)
      {
        it->begin();
        if (it->hasErrored())
          {
            std::string msg =
              "Shader '" + it->name() +
              "' has not been compiled: reason was '" +
              it->error() + "'";
            ERROR("%s", msg.c_str());
            m_error_msg += '\n' + msg;
            failure = true;
          }
      }

    if (!failure)
      {
        // Attach shaders to program
        DEBUG("Prog '%s' setup: attach shaders", name().c_str());
        for (auto& it: m_shaders)
          {
            glCheck(glAttachShader(m_handle, it->gpuID()));
            it->attachProg(m_handle);
          }

        // Compile the program
        DEBUG("Prog '%s' setup: compile prog", name().c_str());
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

    return !m_compiled;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("Prog '%s' activate", name().c_str());

    if (unlikely(!compiled()))
      return ;
    if (unlikely(!binded()))
      return ;

    glCheck(glUseProgram(m_handle));
    m_vao->begin();
    for (auto& it: m_attributes)
      {
        m_vao->m_vbos[it.first]->begin();
        it.second->begin();
      }
    for (auto& it: m_uniforms)
      {
        it.second->begin();
      }
    for (auto& it: m_textures)
      {
        // Important: activate the texture unit first before binding
        // texture
        it.second->begin();
        m_vao->m_textures[it.first]->begin();
      }
  }

  //------------------------------------------------------------------
  //! \brief Dummy method.
  //------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //------------------------------------------------------------------
  //! \brief A program can be desactivated if and only if shaders have
  //! been loaded into a program (else nothing is done).
  //------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("Prog '%s' deactivate", name().c_str());
    glCheck(glUseProgram(0U));

    for (auto& it: m_uniforms)
      {
        it.second->end();
      }
    for (auto& it: m_textures)
      {
        it.second->end();
      }
    for (auto& it: m_attributes)
      {
        it.second->end();
      }

    m_vao = nullptr;
  }

  //------------------------------------------------------------------
  //! \brief Once program is no longer used, release it from the GPU
  //! memory. Can be used to abort the shader.
  //------------------------------------------------------------------
  virtual void release() override
  {
    DEBUG("Prog '%s' release", name().c_str());
    detachAllShaders();
    glCheck(glDeleteProgram(m_handle));
  }

  //------------------------------------------------------------------
  //! \brief Create Attribute and Uniform instances
  //------------------------------------------------------------------
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
    DEBUG("Prog::get all attrib and uniform");
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveUniform(m_handle, i, bufSize, &length,
                                   &size, &type, name));
        DEBUG("Uniform #%u Type: %u Name: %s", i, type, name);
        addNewUniform(type, name);
      }

    // Create the list of attributes and list of VBOs
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    i = static_cast<GLuint>(count);
    while (i--)
      {
        glCheck(glGetActiveAttrib(m_handle, i, bufSize, &length,
                                  &size, &type, name));
        DEBUG("Attribute #%u Type: %u Name: %s", i, type, name);
        addNewAttribute(type, name);
      }
  }

  //------------------------------------------------------------------
  //! \brief Create Attribute instances
  //------------------------------------------------------------------
  void addNewAttribute(GLenum type, const char *name)
  {
    assert(nullptr != name);
    GLenum EGL_FLOAT = static_cast<GLenum>(GL_FLOAT);

    switch (type)
      {
      case GL_FLOAT:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 1, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC2:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 2, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC3:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 3, EGL_FLOAT, gpuID());
        break;
      case GL_FLOAT_VEC4:
        m_attributes[name] = std::make_unique<GLAttribute>(name, 4, EGL_FLOAT, gpuID());
        break;
      default:
        std::string msg = "Attribute '" + std::string(name) + "' type is not managed";
        ERROR("%s", msg.c_str());
        m_error_msg += '\n' + msg;
        break;
      }
  }

  //------------------------------------------------------------------
  //! \brief Create Uniform instances
  //------------------------------------------------------------------
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
        m_uniforms[name] = std::make_unique<GLSampler1D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;
      case GL_SAMPLER_2D:
        m_textures[name] = std::make_unique<GLSampler2D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;
      case GL_SAMPLER_CUBE:
        m_uniforms[name] = std::make_unique<GLSampler3D>(name, m_textures_count, gpuID());
        m_textures_count += 1u;
        break;
      default:
        std::string msg = "Uniform '" + std::string(name) + "' type is not managed";
        ERROR("%s", msg.c_str());
        m_error_msg += '\n' + msg;
        break;
      }
  }

  //------------------------------------------------------------------
  //! \brief Locate the uniform variable by its name and its type T.
  //! \return the uniform instance if found else throw the exception
  //! std::out_of_range
  //------------------------------------------------------------------
  template<class T>
  IGLUniform<T>& getUniform(const char *name)
  {
    if (unlikely(!compiled()))
      {
        begin();
        // TODO: check if now compiled() == true
      }

    if (unlikely(nullptr == name))
      {
        throw OpenGLException("nullptr passed to getUniform");
      }

    if (unlikely(false == hasUniform(name)))
      {
        // TODO: create the variable: call addNewUniform
        // TODO: http://www.cplusplus.com/forum/general/21246/#msg112085
        throw OpenGLException("GLUniform '" + std::string(name) +
                              "' does not exist");
      }

    auto ptr = m_uniforms[name].get();
    IGLUniform<T> *uniform_ptr = dynamic_cast<IGLUniform<T>*>(ptr);
    if (unlikely(nullptr == uniform_ptr))
      {
        throw std::invalid_argument("GLUniform '" + std::string(name) +
                                    "' exists but has wrong template type");
      }
    return *uniform_ptr;
  }

private:

  void detachAllShaders()
  {
    DEBUG("Prog::detachAllshaders");
    for (auto& it: m_shaders)
      {
        if (m_handle == it->attached())
          {
            glCheck(glDetachShader(m_handle, it->gpuID()));
            it->attachProg(0);
          }
      }
  }

  //------------------------------------------------------------------
  //! \return true if case of success, else return false.
  //------------------------------------------------------------------
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
        m_error_msg += '\n';
        m_error_msg += &log[0U];
        ERROR("%s", m_error_msg.c_str());
      }
    else
      {
        m_error_msg.clear();
      }
    return status;
  }

private:

  mapGLLocation          m_attributes;
  mapGLLocation          m_uniforms;
  std::unordered_map<std::string, std::unique_ptr<GLSampler>> m_textures;
  std::vector<GLShader*> m_shaders; // Note: pointer is to avoid copy
  GLVAO                 *m_vao = nullptr;
  std::string            m_error_msg;
  uint32_t               m_textures_count = 0u;
  bool                   m_compiled = false;
  BufferUsage            m_vbo_usage = BufferUsage::DYNAMIC_DRAW;
  size_t                 m_vbo_init_size = 0_z;
};

#endif /* GLPROGRAM_HPP */
