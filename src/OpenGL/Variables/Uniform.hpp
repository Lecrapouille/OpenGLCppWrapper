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

#ifndef OPENGLCPPWRAPPER_GLUNIFORM_HPP
#  define OPENGLCPPWRAPPER_GLUNIFORM_HPP

// *****************************************************************************
//! \file Uniform.hpp file implements:
//!   - GLUniform:
// *****************************************************************************

#  include "OpenGL/Variables/Location.hpp"
#  include "Math/Matrix.hpp"

// *****************************************************************************
//! \brief GLUniform represents a program uniform variable.
//! \tparam T float or int or VectorXf or VectorXi or MatrixXXf with X = [2 .. 4]
//! or GLSamplerXD with X = [1 .. 3] or GLSamplerCube.
// *****************************************************************************
template<class T>
class GLUniform: public GLLocation
{
public:

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    GLUniform(const char *name, const GLint dim, const GLint gltype, const GLuint prog)
        : GLLocation(name, dim, static_cast<GLenum>(gltype), prog)
    {}

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release elements from CPU and GPU.
    //--------------------------------------------------------------------------
    virtual ~GLUniform() override
    {
        release();
    }

    //--------------------------------------------------------------------------
    //! \brief Change the CPU data. It will be automatically transfered to the GPU.
    //--------------------------------------------------------------------------
    template<class U>
    GLUniform<T>& operator=(const U& val)
    {
        GLUniform<T>::m_data = T(val);
        m_need_update = false;
        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Getter. Return the reference of the CPU data in read only mode.
    //--------------------------------------------------------------------------
    inline operator const T&() const
    {
        return m_data;
    }

    //--------------------------------------------------------------------------
    //! \brief Setter. Return the reference of CPU data in write mode. New value
    //! will be transfered to GPU memory.
    //! \fixme Could be nice to avoid transfering to the GPU if we set the same val
    //--------------------------------------------------------------------------
    inline operator T&()
    {
        m_need_update = true;
        return m_data;
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Create a new OpenGL Uniform.
    //--------------------------------------------------------------------------
    virtual bool onCreate() override
    {
        m_handle = glCheck(glGetUniformLocation(m_program, cname()));
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Bind the OpenGL Uniform. This is a dummy method. No
    //! action is made.
    //--------------------------------------------------------------------------
    virtual void onActivate() override
    {}

    //--------------------------------------------------------------------------
    //! \brief Setup the behavior of the instance. This is a dummy
    //! method. No action is made.
    //--------------------------------------------------------------------------
    virtual bool onSetup() override
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Transfer the CPU data to the GPU data.
    //--------------------------------------------------------------------------
    virtual bool onUpdate() override
    {
        apply(GLUniform<T>::m_data);
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Unbind the OpenGL Uniform. This is a dummy method. No
    //! action is made.
    //--------------------------------------------------------------------------
    virtual void onDeactivate() override
    {}

    //--------------------------------------------------------------------------
    //! \brief Destroy the OpenGL Uniform. This is a dummy method. No
    //! action is made.
    //--------------------------------------------------------------------------
    virtual void onRelease() override
    {
        //GLLocation::onRelease();
    }

    //--------------------------------------------------------------------------
    //! \brief Transfer the CPU data to the GPU data.
    //--------------------------------------------------------------------------
    inline void apply(const T& value) const;

protected:

    T m_data {};
};

template<>
inline void GLUniform<int>::apply(const int& value) const
{
    glCheck(glUniform1i(m_handle, value));
}

template<>
inline void GLUniform<float>::apply(const float& value) const
{
    glCheck(glUniform1f(m_handle, value));
}

template<>
inline void GLUniform<Vector2f>::apply(const Vector2f& value) const
{
    glCheck(glUniform2f(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3f>::apply(const Vector3f& value) const
{
    glCheck(glUniform3f(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4f>::apply(const Vector4f& value) const
{
    glCheck(glUniform4f(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Vector2i>::apply(const Vector2i& value) const
{
    glCheck(glUniform2i(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3i>::apply(const Vector3i& value) const
{
    glCheck(glUniform3i(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4i>::apply(const Vector4i& value) const
{
    glCheck(glUniform4i(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Matrix22f>::apply(const Matrix22f& m) const
{
    glCheck(glUniformMatrix2fv(m_handle, 1, GL_FALSE, m));
}

template<>
inline void GLUniform<Matrix33f>::apply(const Matrix33f& m) const
{
    glCheck(glUniformMatrix3fv(m_handle, 1, GL_FALSE, m));
}

template<>
inline void GLUniform<Matrix44f>::apply(const Matrix44f& m) const
{
    glCheck(glUniformMatrix4fv(m_handle, 1, GL_FALSE, m));
}

#endif // OPENGLCPPWRAPPER_GLUNIFORM_HPP