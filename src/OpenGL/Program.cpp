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

#include "OpenGL/Program.hpp"

//--------------------------------------------------------------------------
GLProgram::GLProgram(std::string const& name)
    : GLObject(name)
{}

//--------------------------------------------------------------------------
GLProgram::~GLProgram()
{
    release();
}

//----------------------------------------------------------------------------
void GLProgram::onRelease()
{
    detachAllShaders();
    glCheck(glDeleteProgram(m_handle));
    m_shaders.clear();
    m_attributes.clear();
    m_uniforms.clear();
    m_samplers.clear();
    m_error.clear();
}

//----------------------------------------------------------------------------
void GLProgram::concatError(std::string const& msg)
{
    if (!m_error.empty())
        m_error += '\n';
    m_error += msg;
}

//----------------------------------------------------------------------------
bool GLProgram::bind(GLVAO& vao, BufferUsage const usage, size_t const vbo_size)
{
    // Try compile the GLProgram if not previously compiled
    if (unlikely(!hasBeenCompiled()))
    {
        if (!compile())
        {
            std::cerr << "Tried to bind VAO '"
                      << vao.name()
                      << "' to a GLProgram '"
                      << name() << " that failed to compile"
                      << std::endl;
            return false;
        }
    }

    if (unlikely(vao.isBound(0u)))
    {
        // When binding the VAO to GLProgram for the first time:
        // populate VBOs and textures in the VAO.
        vao.init(*this, usage, vbo_size);
    }
    else if (unlikely(!vao.isBound(m_handle)))
    {
        // Check if VAO has been previously bind by this GLProgram. If not
        // This is probably an error of the developper.
        std::cerr << "Tried to bind VAO '"
                  << vao.name()
                  << " already bound to another GLProgram than '"
                  << name() << std::endl;
        return false;
    }

    // Bind the VAO to the GLProgram. This relationship is now unbreakable
    m_vao = &vao;
    return true;
}

//----------------------------------------------------------------------------
std::string GLProgram::strerror()
{
    std::string msg;

    if (!m_error.empty())
    {
        if (m_shaders.size() != 0u)
        {
            msg += "The following shaders failed to compile:\n";
            for (auto& it: m_shaders)
            {
                if (!it->hasBeenCompiled())
                {
                    msg += "  - ";
                    msg += it->name();
                    msg += '\n';
                }
            }
        }
        msg += "Reason was:\n";
        msg += m_error;
        m_error.clear();
    }

    return msg;
}

//--------------------------------------------------------------------------
bool GLProgram::compile(GLVertexShader& vertex, GLFragmentShader& fragment)
{
    m_shaders.clear();
    m_shaders.push_back(&vertex);
    m_shaders.push_back(&fragment);
    return compile();
}

//--------------------------------------------------------------------------
bool GLProgram::compile(GLVertexShader& vertex, GLFragmentShader& fragment,
                        GLGeometryShader& geometry)
{
    m_shaders.clear();
    m_shaders.push_back(&vertex);
    m_shaders.push_back(&fragment);
    m_shaders.push_back(&geometry);
    return compile();
}

//--------------------------------------------------------------------------
bool GLProgram::compile()
{
    begin();
    return !needSetup();
}

//--------------------------------------------------------------------------
bool GLProgram::onCreate()
{
    m_handle = glCheck(glCreateProgram());
    return false;
}

//--------------------------------------------------------------------------
bool GLProgram::onUpdate()
{
    return false;
}

//--------------------------------------------------------------------------
bool GLProgram::onSetup()
{
    // Compile shaders if they have not yet compiled
    for (auto& it: m_shaders)
    {
        if (!it->compile())
        {
            std::string msg = "  - " + it->name() + ":\n" + it->strerror();
            concatError(msg);
            return true;
        }
    }

    // Attach shaders to program
    for (auto& it: m_shaders)
    {
        glCheck(glAttachShader(m_handle, it->handle()));
        //it->bind(m_handle);
    }

    // Link shaders to the program
    glCheck(glLinkProgram(m_handle));
    if (checkLinkageStatus(m_handle))
    {
        m_error.clear();
        // Create the list of attributes and uniforms
        generateLocations();
        // Release shaders stored in GPU.
        detachAllShaders();
        // Success
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
bool GLProgram::checkLinkageStatus(GLuint handle)
{
    GLint status;

    glCheck(glGetProgramiv(handle, GL_LINK_STATUS, &status));
    if (GL_FALSE == status)
    {
        GLint length;
        glCheck(glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetProgramInfoLog(handle, length - 1, &length, &log[0U]));
        concatError(&log[0U]);
    }
    else
    {
        m_error.clear();
    }
    return !!status;
}

//----------------------------------------------------------------------------
void GLProgram::detachAllShaders()
{
    for (auto& it: m_shaders)
    {
        glCheck(glDetachShader(m_handle, it->handle()));
    }
    m_shaders.clear();
}

//----------------------------------------------------------------------------
void GLProgram::generateLocations()
{
    const GLsizei BUFFER_SIZE = 64;
    GLchar name[BUFFER_SIZE];
    GLint size = 0;
    GLint count = 0;
    GLuint location;
    GLenum type;

    // Create the list of uniforms and samplers
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    location = static_cast<GLuint>(count);
    while (location--)
    {
        glCheck(glGetActiveUniform(m_handle, location, BUFFER_SIZE, nullptr,
                                   &size, &type, name));
        createUniform(type, name, handle());
        // TODO
    }

    // Create the list of attributes
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    location = static_cast<GLuint>(count);
    while (location--)
    {
        glCheck(glGetActiveAttrib(m_handle, location, BUFFER_SIZE, nullptr,
                                  &size, &type, name));
        createAttribute(type, name, handle());
    }
}

//----------------------------------------------------------------------------
void GLProgram::createAttribute(GLenum type, const char *name, const GLuint prog)
{
    switch (type)
    {
    case GL_FLOAT:
        m_attributes[name] = std::make_shared<GLAttribute>(name, 1, GL_FLOAT, prog);
        break;
    case GL_FLOAT_VEC2:
        m_attributes[name] = std::make_shared<GLAttribute>(name, 2, GL_FLOAT, prog);
        break;
    case GL_FLOAT_VEC3:
        m_attributes[name] = std::make_shared<GLAttribute>(name, 3, GL_FLOAT, prog);
        break;
    case GL_FLOAT_VEC4:
        m_attributes[name] = std::make_shared<GLAttribute>(name, 4, GL_FLOAT, prog);
        break;
    default:
        std::string msg = "The type of Attribute for " + std::string(name)
                          + " is not managed";
        throw GL::Exception(msg);
        break;
    }
}

//----------------------------------------------------------------------------
void GLProgram::createUniform(GLenum type, const char *name, const GLuint prog)
{
    std::cout << "GLProgram::createUniform " << name << std::endl;
    switch (type)
    {
    case GL_FLOAT:
        m_uniforms.add(name,
            std::make_shared<GLUniform<float>>(name, 1, GL_FLOAT, prog));
        break;
    case GL_FLOAT_VEC2:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector2f>>(name, 2, GL_FLOAT, prog));
        break;
    case GL_FLOAT_VEC3:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector3f>>(name, 3, GL_FLOAT, prog));
        break;
    case GL_FLOAT_VEC4:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector4f>>(name, 4, GL_FLOAT, prog));
        break;
    case GL_INT:
        m_uniforms.add(name,
            std::make_shared<GLUniform<int>>(name, 1, GL_INT, prog));
        break;
    case GL_INT_VEC2:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector2i>>(name, 2, GL_INT, prog));
        break;
    case GL_INT_VEC3:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector3i>>(name, 3, GL_INT, prog));
        break;
    case GL_INT_VEC4:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Vector4i>>(name, 4, GL_INT, prog));
        break;
    case GL_FLOAT_MAT2:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Matrix22f>>(name, 4, GL_FLOAT, prog));
        break;
    case GL_FLOAT_MAT3:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Matrix33f>>(name, 9, GL_FLOAT, prog));
        break;
    case GL_FLOAT_MAT4:
        m_uniforms.add(name,
            std::make_shared<GLUniform<Matrix44f>>(name, 16, GL_FLOAT, prog));
        break;
    case GL_SAMPLER_1D:
        m_samplers[name]
                = std::make_shared<GLSampler1D>(name, m_samplers.size(), prog);
        break;
    case GL_SAMPLER_2D:
        m_samplers[name]
                = std::make_shared<GLSampler2D>(name, m_samplers.size(), prog);
        break;
    case GL_SAMPLER_3D:
        m_samplers[name]
                = std::make_shared<GLSampler3D>(name, m_samplers.size(), prog);
        break;
    case GL_SAMPLER_CUBE:
        m_samplers[name]
                = std::make_shared<GLSamplerCube>(name, m_samplers.size(), prog);
        break;
    default:
        std::string msg = "The type of Uniform for " + std::string(name)
                          + " is not managed";
        throw GL::Exception(msg);
        break;
    }
}

//----------------------------------------------------------------------------
size_t GLProgram::failedShaders(std::vector<std::string>& list, bool const clear)
{
    if (clear) { list.clear(); }
    list.reserve(m_shaders.size());
    for (auto const& it: m_shaders)
    {
        if (!it->hasBeenCompiled())
        {
            list.push_back(it->name());
        }
    }
    return m_shaders.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getUniformNames(std::vector<std::string>& list, bool const clear)
{
    if (clear) { list.clear(); }
    list.reserve(m_uniforms.size());
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<float>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector2f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector3f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector4f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Matrix22f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Matrix33f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Matrix44f>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<int>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector2i>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector3i>>>())
        list.push_back(it.first);
    for (auto const& it: m_uniforms.map<std::shared_ptr<GLUniform<Vector4i>>>())
        list.push_back(it.first);
    return list.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getAttributeNames(std::vector<std::string>& list, bool const clear)
{
    if (clear) { list.clear(); }
    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
        list.push_back(it.first);
    return list.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getSamplerNames(std::vector<std::string>& list, bool const clear)
{
    if (clear) { list.clear(); }
    list.reserve(m_samplers.size());
    for (auto const& it: m_samplers)
        list.push_back(it.first);
    return list.size();
}

//--------------------------------------------------------------------------
void GLProgram::onActivate()
{
#if 0
    glCheck(glUseProgram(m_handle));
    if (m_vao->needCreate())
    {
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

        for (auto& it: m_samplers)
        {
            // Important: Contrary to attributes, activate the texture units first
            // before binding textures.
            it.second->begin();
            m_vao->m_textures[it.first]->begin();
        }

        throw_if_inconsitency_attrib_sizes();
    }
    else
    {
        m_vao->activate();
    }
#endif
}

//--------------------------------------------------------------------------
void GLProgram::onDeactivate()
{
    glCheck(glUseProgram(0U));
}

//--------------------------------------------------------------------------
void GLProgram::doDraw(Mode const mode, size_t const first, size_t const count)
{
    begin();
    glCheck(glDrawArrays(static_cast<GLenum>(mode),
                         static_cast<GLint>(first),
                         static_cast<GLsizei>(count)));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    end();
}

//--------------------------------------------------------------------------
void GLProgram::draw(Mode const mode, size_t const first, size_t const count)
{
    throw_if_not_compiled();
    throw_if_vao_not_bound();
    doDraw(mode, first, count);
}

//--------------------------------------------------------------------------
void GLProgram::draw(GLVAO& vao, Mode const mode, size_t const first, size_t const count)
{
    throw_if_vao_cannot_be_bound(vao);
    doDraw(mode, first, count);
}

//--------------------------------------------------------------------------
void GLProgram::draw(GLVAO& /*vao*/, Mode const /*mode*/)
{
    //    throw_if_not_compiled();
    //    throw_if_vao_cannot_be_bound(vao);
    //    doDraw(mode, 0, m_vao->count());
}

//--------------------------------------------------------------------------
void GLProgram::draw(Mode const /*mode*/)
{
    //    throw_if_not_compiled();
    //    throw_if_vao_not_bound();
    //    doDraw(mode, 0, m_vao->count());
}

//----------------------------------------------------------------------------
void GLProgram::throw_if_inconsitency_attrib_sizes()
{
#if 0
    if (!m_vao->checkVBOSizes())
    {
        throw GL::Exception("Failed OpenGL attributes have not the same size");
    }
    if (!m_vao->checkLoadTextures())
    {
        throw GL::Exception("Failed OpenGL textures have not all been loaded");
    }
#endif
}

//----------------------------------------------------------------------------
void GLProgram::throw_if_not_compiled()
{
    if (unlikely(!hasBeenCompiled()))
    {
        throw GL::Exception("Failed OpenGL program has not been compiled");
    }
}

//----------------------------------------------------------------------------
void GLProgram::throw_if_vao_cannot_be_bound(GLVAO& vao)
{
    if (unlikely(!bind(vao)))
    {
        throw GL::Exception("Failed binding VAO to OpenGL program");
    }
}

//----------------------------------------------------------------------------
void GLProgram::throw_if_vao_not_bound()
{
    if (unlikely(m_vao == nullptr))
    {
        throw GL::Exception("Failed OpenGL program has not been bound to a VAO");
    }
}
