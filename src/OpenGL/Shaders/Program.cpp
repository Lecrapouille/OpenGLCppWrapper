//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

//------------------------------------------------------------------------------
GLProgram::GLProgram(std::string const& name)
    : GLObject(name, 0)
{}

//------------------------------------------------------------------------------
GLProgram::~GLProgram()
{
    release();
}

//------------------------------------------------------------------------------
void GLProgram::onRelease()
{
    glCheck(glDeleteProgram(m_handle));
    m_uniforms.clear();
    m_samplers.clear();
    m_attributes.clear();
    m_error.clear();
}

//------------------------------------------------------------------------------
void GLProgram::concatError(std::string const& msg)
{
    if (!m_error.empty())
        m_error += '\n';
    m_error += msg;
}

//------------------------------------------------------------------------------
bool GLProgram::bind(GLVAO& vao)
{
    std::cout << "bind GLVAO " << vao.name() << " to GLProgram " << name() << std::endl;

    // First compile the GLProgram (compile shaders and load GLSL variables) if
    // this has not been done yet. Indeed GLProgram will populate the bound VAO
    // with VBOs.
    if (unlikely(!compiled()))
    {
        if (!compile())
        {
            std::string err("Tried to bind GLVAO "  + vao.name() +
                            " to the GLProgram " + name() +
                            " which has failed to compile");
            concatError(err);
            return false;
        }
    }

    // Bind this instance of GLProgram to a never bound VAO. When a VAO is bound
    // for the first time, we create list of VBOs and textures. The list of VBOs
    // depends on the list of shader attributes and the list of textures depends
    // on the list of shader samplers. Shader attributes and samplers are the
    // "variables" declared in the vertex, fragment shaders.
    if (unlikely(!vao.isBound()))
    {
        vao.m_program = this;
        vao.createVBOsFromAttribs(m_attributes);
        vao.createTexturesFromSamplers(m_samplers);
        vao.m_need_update = true;
        return true;
    }

    // Check if the VAO was already bound to this instance of GLProgram. If not,
    // this is probably an error made by the developper trying to bind a VAO to
    // an incompatible GLProgram. If yes, nothing has to be done (since lists of
    // VBO and textures have already been made during the first binding).
    else if (likely(vao.isBoundTo(m_handle)))
    {
        vao.m_need_update = true; // FIXME TBD ?
        return true;
    }
    else
    {
        assert(vao.m_program != nullptr);
        std::string err("You have tried to bind the GLVAO " + vao.name() +
                        "to GLProgram " + name() + " but the GLVAO was previously"
                        " bound to GLProgram " + vao.m_program->name());
        concatError(err);
        return false;
    }
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
bool GLProgram::compile(GLVertexShader& vertex, GLFragmentShader& fragment)
{
    m_shaders.clear();
    m_shaders.push_back(&vertex);
    m_shaders.push_back(&fragment);
    return compile();
}

//------------------------------------------------------------------------------
bool GLProgram::compile(GLVertexShader& vertex, GLFragmentShader& fragment,
                        GLGeometryShader& geometry)
{
    m_shaders.clear();
    m_shaders.push_back(&vertex);
    m_shaders.push_back(&fragment);
    m_shaders.push_back(&geometry);
    return compile();
}

//------------------------------------------------------------------------------
bool GLProgram::compile()
{
    begin();
    return compiled();
}

//------------------------------------------------------------------------------
bool GLProgram::onCreate()
{
    m_handle = glCheck(glCreateProgram());
    return false;
}

//------------------------------------------------------------------------------
// GLObject::begin() calls onActivate() before onSetup() but for GLProgram
// this should be the inversed.
bool GLProgram::onSetup()
{
    bool success = true;
    std::cout << "Linking shaders into GLProgram named "
              << name() << " ..." << std::endl;

    // Compile shaders if they have not yet been compiled. Keep iterating even
    // if one has failed in the aim to display the most errors.
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

        // Create the list of attributes, samplers and uniforms that will be
        // used for populating list of VBOs (attibutes) and textures (samplers)
        // when a VAO will be bind to the GLProgram.
        if (success)
        {
            std::cout << "Generating attributes and uniforms for GLProgram named "
                      << name() << "..." << std::endl;
            generateAttributesAndUniforms();
            m_error.clear();

            // Force calling onActivate() without checks since GLObject::begin()
            // calls onActivate() before onSetup() but for GLProgram this should
            // be the inversed. So onActivate() was called before this method
            // and has failed because this GLProgram was not yet compiled. So
            // now, activate the GLProgram.
            glCheck(glUseProgram(m_handle));

            // Force calling onUpdate() allowing to update uniforms since the
            // API allows the user to define uniforms before compiling the
            // GLProgram. This allows for example to create 3d object with
            // predefined materials before compiling shaders
            m_need_update = true;
        }
    }

    // Release shaders stored in GPU: they are no longer needed.
    detachAllShaders();
    return !success;
}

//------------------------------------------------------------------------------
bool GLProgram::onUpdate()
{
    for (auto const& it: m_uniforms)
    {
        it.second->begin();
    }

    // Always force calling GLProgram::onUpdate()
    return true;
}

//------------------------------------------------------------------------------
// GLObject::begin() calls onActivate() before onSetup() but for GLProgram this
// should be the inversed so need the if(compiled())
void GLProgram::onActivate()
{
    if (compiled())
    {
        glCheck(glUseProgram(m_handle));
    }
}

//------------------------------------------------------------------------------
void GLProgram::onDeactivate()
{
    for (auto const& it: m_uniforms)
    {
        it.second->end();
    }
    glCheck(glUseProgram(0U));
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void GLProgram::generateAttributesAndUniforms()
{
    const GLsizei BUFFER_SIZE = 64;
    GLchar name[BUFFER_SIZE];
    GLint size = 0;
    GLint count = 0;
    GLuint location;
    GLenum type;

    // Create the list of uniforms and samplers. Uniforms are used to modify
    // values of uniform variables in shaders. Samplers are for texture they act
    // like uniform but are used to populate textures when a VAO is bound to the
    // first time to this GLProgram.
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));
    location = static_cast<GLuint>(count);
    while (location--)
    {
        glCheck(glGetActiveUniform(m_handle, location, BUFFER_SIZE, nullptr,
                                   &size, &type, name));
        storeUniformOrSampler(type, name);
    }

    // Create the list of attributes. Attributes are used to populate VBOs when
    // a VAO is bound to the first time to this GLProgram.
    glCheck(glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count));
    location = static_cast<GLuint>(count);
    while (location--)
    {
        glCheck(glGetActiveAttrib(m_handle, location, BUFFER_SIZE, nullptr,
                                  &size, &type, name));
        storeAttribute(type, name);
    }
}

//------------------------------------------------------------------------------
void GLProgram::storeAttribute(GLenum type, const char *name)
{
    // TODO: kill unused uniforms ? Meaning uniforms created by the user but
    // not used inside shaders.
    auto it = m_attributes.find(name);
    if (it != m_attributes.end())
        return ;

    switch (type)
    {
        // TODO: manage integers
    case GL_FLOAT:
        createAttribute<float>(name);
        return;
    case GL_FLOAT_VEC2:
        createAttribute<Vector2f>(name);
        return;
    case GL_FLOAT_VEC3:
        createAttribute<Vector3f>(name);
        return;
    case GL_FLOAT_VEC4:
        createAttribute<Vector4f>(name);
        return;
    default:
        std::string msg =
                "The type of Attribute for " + std::string(name) +
                " is not managed";
        throw GL::Exception(msg);
    }
}
//------------------------------------------------------------------------------
void GLProgram::storeUniformOrSampler(GLenum type, const char *name)
{
    if (!storeUniform(type, name))
    {
        if (!storeSampler(type, name))
        {
            std::string msg =
                    "The type of Uniform for " + std::string(name) +
                    " is not managed";
            throw GL::Exception(msg);
        }
    }
}

//------------------------------------------------------------------------------
bool GLProgram::storeUniform(GLenum type, const char *name)
{
    // The API allows the user to define uniforms before compiling the
    // GLProgram. Avoid replacing the uniform to avoid loosing its value.
    // TODO: kill unused uniforms ? Meaning uniforms created by the user but
    // not used inside shaders.
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
        return true;

    // Store new uniform
    switch (type)
    {
    case GL_FLOAT:
        createUniform<float>(name);
        return true;
    case GL_FLOAT_VEC2:
        createUniform<Vector2f>(name);
        return true;
    case GL_FLOAT_VEC3:
        createUniform<Vector3f>(name);
        return true;
    case GL_FLOAT_VEC4:
        createUniform<Vector4f>(name);
        return true;
    case GL_INT:
        createUniform<int>(name);
        return true;
    case GL_INT_VEC2:
        createUniform<Vector2i>(name);
        return true;
    case GL_INT_VEC3:
        createUniform<Vector3i>(name);
        return true;
    case GL_INT_VEC4:
        createUniform<Vector4i>(name);
        return true;
    case GL_FLOAT_MAT2:
        createUniform<Matrix22f>(name);
        return true;
    case GL_FLOAT_MAT3:
        createUniform<Matrix33f>(name);
        return true;
    case GL_FLOAT_MAT4:
        createUniform<Matrix44f>(name);
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
bool GLProgram::storeSampler(GLenum type, const char *name)
{
    // The API allows the user to define uniforms before compiling the
    // GLProgram. Avoid replacing the uniform to avoid loosing its value.
    // TODO: kill unused uniforms ? Meaning uniforms created by the user but
    // not used inside shaders.
    auto it = m_samplers.find(name);
    if (it != m_samplers.end())
        return true;

    // Store new sampler
    switch (type)
    {
    case GL_SAMPLER_1D:
        createSampler<GLSampler1D>(name);
        return true;
    case GL_SAMPLER_2D:
        createSampler<GLSampler2D>(name);
        return true;
    case GL_SAMPLER_3D:
        createSampler<GLSampler3D>(name);
        return true;
    case GL_SAMPLER_CUBE:
        createSampler<GLSamplerCube>(name);
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
size_t GLProgram::getFailedShaders(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list = m_failedShaders;
    return list.size();
}

//------------------------------------------------------------------------------
size_t GLProgram::getUniformNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_uniforms.size());
    for (auto const& it: m_uniforms)
        list.push_back(it.first);
    return list.size();
}

//------------------------------------------------------------------------------
size_t GLProgram::getAttributeNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_attributes.size());
    for (auto const& it: m_attributes)
        list.push_back(it.first);
    return list.size();
}

//------------------------------------------------------------------------------
size_t GLProgram::getSamplerNames(std::vector<std::string>& list, bool const clear) const
{
    if (clear) { list.clear(); }
    list.reserve(m_samplers.size());
    for (auto const& it: m_samplers)
        list.push_back(it.first);
    return list.size();
}
