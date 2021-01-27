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

#include "OpenGL/Shaders/Shader.hpp"

GLShader::GLShader(std::string const& name, const GLenum target)
    : GLObject(name, target)
{}

//--------------------------------------------------------------------------
GLShader::~GLShader()
{
    release();
}

//--------------------------------------------------------------------------
bool GLShader::read(std::string const& path)
{
    throw_if_already_compiled();

    std::string msg;
    std::ifstream infile;

    // Open shader file
    errno = 0;
    infile.open(path);
    if (!infile.fail())
    {
        // Get the size of the shader
        infile.seekg(0, std::ios::end);
        std::streampos pos = infile.tellg();
        infile.seekg(0, std::ios::beg);

        if (pos > 0)
        {
            // Increase the buffer size
            size_t p = m_code.size();
            m_code.resize(p + static_cast<size_t>(pos));

            // Read the shader content
            if (!infile.read(&m_code[p], static_cast<std::streamsize>(pos)))
            {
                msg = "Failed reading file '" + path + "'. Reason was '" + ::strerror(errno) + "'";
                concatError(msg);
                return false;
            }
        }
        else
        {
            msg = "Failed reading file '" + path + "'. Reason was 'cannot get the file size'";
            concatError(msg);
            return false;
        }
    }
    else
    {
        msg = "Failed opening file '" + path + "'. Reason was '" + ::strerror(errno) + "'";
        concatError(msg);
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------
std::string GLShader::strerror()
{
    std::string tmp(m_error);
    m_error.clear();
    return tmp;
}

//--------------------------------------------------------------------------
bool GLShader::compile()
{
    if (!needSetup())
        return true;

    // TODO solveIncludes()

    begin();
    return (m_error.size() == 0u);
}

//--------------------------------------------------------------------------
bool GLShader::onCreate()
{
    m_handle = glCheck(glCreateShader(m_target));
    return false;
}

//--------------------------------------------------------------------------
void GLShader::onActivate()
{}

//--------------------------------------------------------------------------
bool GLShader::onSetup()
{
    bool compiled = false;

    if (!m_code.empty())
    {
        char const *source = m_code.c_str();
        GLint length = static_cast<GLint>(m_code.size());
        glCheck(glShaderSource(m_handle, 1, &source, &length));
        glCheck(glCompileShader(m_handle));
        compiled = checkCompilationStatus(m_handle);
    }
    else
    {
        std::string msg =
                "   Could not compile the shader named '" + name() +
                "'. Reason was 'compilation errored'";
        concatError(msg);
    }

    return !compiled;
}

//--------------------------------------------------------------------------
bool GLShader::onUpdate()
{
    return false;
}

//--------------------------------------------------------------------------
void GLShader::onDeactivate()
{}

//--------------------------------------------------------------------------
void GLShader::onRelease()
{
    glCheck(glDeleteShader(m_handle));
    m_code.clear();
    m_error.clear();
}

//--------------------------------------------------------------------------
bool GLShader::checkCompilationStatus(GLuint obj)
{
    GLint status;

    glCheck(glGetShaderiv(obj, GL_COMPILE_STATUS, &status));
    if (GL_FALSE == status)
    {
        GLint length;
        glCheck(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetShaderInfoLog(obj, length - 1, &length, &log[0U]));
        concatError(&log[0U]);
    }
    else
    {
        m_error.clear();
    }
    return status;
}

//--------------------------------------------------------------------------
void GLShader::throw_if_not_loaded()
{
    if (unlikely(m_code.empty()))
    {
        throw GL::Exception("No code attached to the shader");
    }
}

//--------------------------------------------------------------------------
void GLShader::throw_if_already_compiled()
{
    if (unlikely(!needSetup()))
    {
        throw GL::Exception("Shader already compiled");
    }
}

//--------------------------------------------------------------------------
void GLShader::concatError(std::string const& msg)
{
    if (!m_error.empty())
    {
        m_error += '\n';
    }
    m_error += msg;
}
