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

#include "OpenGL/Shaders/Program.hpp"
#include "OpenGL/Buffers/iVAO.hpp"

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
    glCheck(glDeleteProgram(m_handle));
    m_uniforms.clear();
    m_uniformLocations.clear();
    m_samplers.clear();
    m_attributes.clear();
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
bool GLProgram::bind(GLVAO& vao)
{
    std::cout << "bind GLProgram " << name() << " to VAO " << vao.name() << std::endl;

    // First compile the GLProgram (compile shaders and load GLSL variables) if
    // this has not been done yet. Indeed GLProgram will populate the bound VAO
    // with VBOs.
    if (unlikely(!compiled()))
    {
        if (!compile())
        {
            std::string err("Tried to bind VAO "  + vao.name()
                            + "' to a GLProgram " + name()
                            + " that failed to compile");
            concatError(err);
            return false;
        }
    }

    // Bind this instance of GLProgram to a never bound VAO. When binding a VAO
    // for the first time: populate it with VBOs and textures.
    if (unlikely(!vao.isBound()))
    {
        vao.m_program = this;
        vao.createVBOsFromAttribs(m_attributes);
        vao.createTexturesFromSamplers(m_samplers);
        vao.m_need_update = true;
        return true;
    }

    // Check if VAO has been previously bound to this instance of GLProgram. If
    // not, this probably an error of the developper trying to bind a VAO to an
    // incompatible GLProgram.
    else if (likely(vao.isBoundTo(m_handle)))
    {
        vao.m_need_update = true; // TBD
        return true;
    }
    else
    {
        std::string err("Tried to bind VAO " + vao.name()
                        + " already bound to another GLProgram than " + name());
        concatError(err);
        return false;
    }
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
                if (!it->compiled())
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
// FIXME glObject::begin() calls onActivate() before onSetup() but for GLProgram
// this should be the inversed.
bool GLProgram::onSetup()
{
    bool success = true;

    // Compile shaders if they have not yet been compiled
    for (auto& it: m_shaders)
    {
        if (it->code().size() == 0u)
        {
            std::string msg = "  - " + it->name() + ":\nhas empty code source\n";
            concatError(msg);
            success = false;
        }
        else if (!it->compile())
        {
            std::string msg = "  - " + it->name() + ":\n" + it->strerror();
            concatError(msg);
            success = false;
        }
    }

    if (success)
    {
        // Attach shaders to program
        for (auto& it: m_shaders)
        {
            glCheck(glAttachShader(m_handle, it->handle()));
        }

        // Link shaders to the program
        glCheck(glLinkProgram(m_handle));
        success = checkLinkageStatus(m_handle);

        // Create the list of attributes and uniforms
        if (success)
        {
            generateLocations();
            m_error.clear();
        }
    }

    // Release shaders stored in GPU.
    detachAllShaders();
    m_need_update = true;

    // FIXME: ugly
    // Activate now because the user may have created uniforms before creating
    // the Program and the next method called after this method is onUpdate().
    glCheck(glUseProgram(m_handle));

    return !success;
}

//--------------------------------------------------------------------------
bool GLProgram::onUpdate()
{
    for (auto const& it: m_uniformLocations)
    {
        it.second->begin();
    }

    return true;
}

//--------------------------------------------------------------------------
// FIXME glObject::begin() calls onActivate() before onSetup() but for GLProgram
// this should be the inversed so need the if(compiled())
void GLProgram::onActivate()
{
    if (compiled()) {
        glCheck(glUseProgram(m_handle));
    }
}

//--------------------------------------------------------------------------
void GLProgram::onDeactivate()
{
    for (auto const& it: m_uniformLocations)
    {
        it.second->end();
    }
    glCheck(glUseProgram(0U));
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
        return false;
    }
    else
    {
        m_error.clear();
        return true;
    }
}

//----------------------------------------------------------------------------
void GLProgram::detachAllShaders()
{
    m_failedShaders.clear();
    for (auto& it: m_shaders)
    {
        if (it->compiled())
        {
            glCheck(glDetachShader(m_handle, it->handle()));
        }
        else
        {
            m_failedShaders.push_back(it->name());
        }
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
        // TODO if (!m_attributes.has<T>(name) {
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
    }
}

//----------------------------------------------------------------------------
void GLProgram::createUniform(GLenum type, const char *name, const GLuint prog)
{
    // TODO: if uniform already bound => do nothing

    switch (type)
    {
    case GL_FLOAT:
        doCreateUniform<float>(1, type, name, prog);
        break;
    case GL_FLOAT_VEC2:
        doCreateUniform<Vector2f>(2, type, name, prog);
        break;
    case GL_FLOAT_VEC3:
        doCreateUniform<Vector3f>(3, type, name, prog);
        break;
    case GL_FLOAT_VEC4:
        doCreateUniform<Vector4f>(4, type, name, prog);
        break;
    case GL_INT:
        doCreateUniform<int>(1, type, name, prog);
        break;
    case GL_INT_VEC2:
        doCreateUniform<Vector2i>(2, type, name, prog);
        break;
    case GL_INT_VEC3:
        doCreateUniform<Vector3i>(3, type, name, prog);
        break;
    case GL_INT_VEC4:
        doCreateUniform<Vector4i>(4, type, name, prog);
        break;
    case GL_FLOAT_MAT2:
        doCreateUniform<Matrix22f>(4, type, name, prog);
        break;
    case GL_FLOAT_MAT3:
        doCreateUniform<Matrix33f>(9, type, name, prog);
        break;
    case GL_FLOAT_MAT4:
        doCreateUniform<Matrix44f>(16, type, name, prog);
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
    }
}

//----------------------------------------------------------------------------
size_t GLProgram::getFailedShaders(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list = m_failedShaders;
    return list.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getUniformNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_uniformLocations.size());
    for (auto const& it: m_uniformLocations)
        list.push_back(it.second->name());
    return list.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getAttributeNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
        list.push_back(it.first);
    return list.size();
}

//----------------------------------------------------------------------------
size_t GLProgram::getSamplerNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_samplers.size());
    for (auto const& it: m_samplers)
        list.push_back(it.first);
    return list.size();
}
