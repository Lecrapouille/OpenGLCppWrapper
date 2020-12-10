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

#ifndef OPENGLCPPWRAPPER_GLATTRIBUTES_HPP
#  define OPENGLCPPWRAPPER_GLATTRIBUTES_HPP

#  include "OpenGL/Variables/Location.hpp"
#  include <cassert>

// *****************************************************************************
//! \class GLAttribute Attribute.hpp
//! \ingroup OpenGL
//!
//! \brief Represent an attribute variable used in a GLSL shader program.
//!
//! This class only stores information about the attribute variable (dimension,
//! type). These information are used by GLProgam when a VAO is bind to it for
//! creating GLVBO inside the VAO. GLAttribute should be used directly by the
//! user, it is an internal class for GLProgram.
// *****************************************************************************
class GLAttribute: public GLLocation
{
public:

    //--------------------------------------------------------------------------
    //! \brief See GLLocation constructor.
    //! \param[in] name Give a name to the instance. GLProgram uses these names
    //! in their internal hash table.
    //! \param[in] size set the dimension of variable (1 for scalar else the
    //! dimension for vector)
    //! \param[in] gltype set the OpenGL type of data (GL_FLOAT ...)
    //! \param[in] prog the handle of the GLProgram (which is the owner of this
    //! instance).
    //--------------------------------------------------------------------------
    GLAttribute(const char *name, const GLint size, const GLint gltype, const GLuint prog)
        : GLLocation(name, size, static_cast<GLenum>(gltype), prog)
    {
        assert((size >= 1) && (size <= 4));
    }

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release elements in CPU and GPU memories.
    //--------------------------------------------------------------------------
    virtual ~GLAttribute() override
    {
        release();
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Create a new OpenGL Attribute.
    //--------------------------------------------------------------------------
    virtual bool onCreate() override
    {
        m_handle = glCheck(glGetAttribLocation(m_program, cname()));
        m_index = static_cast<GLuint>(m_handle);

#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
        glCheck(glVertexAttribPointer(m_index, m_size, m_target, GL_FALSE,
                                      static_cast<GLsizei>(m_stride),
                                      (void*) m_offset));
#  pragma GCC diagnostic pop

        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Bind the OpenGL Attribute.
    //--------------------------------------------------------------------------
    virtual void onActivate() override
    {
        glCheck(glEnableVertexAttribArray(m_index));
    }

    //--------------------------------------------------------------------------
    //! \brief Setup the behavior of the instance. This is a dummy
    //! method. No action is made.
    //--------------------------------------------------------------------------
    virtual bool onSetup() override
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief This is a dummy method. No action is made.
    //--------------------------------------------------------------------------
    virtual bool onUpdate() override
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Unbind the OpenGL Attribute.
    //--------------------------------------------------------------------------
    virtual void onDeactivate() override
    {
        glCheck(glDisableVertexAttribArray(m_index));
    }

    //--------------------------------------------------------------------------
    //! \brief Destroy the OpenGL Attribute. This is a dummy method. No
    //! action is made.
    //--------------------------------------------------------------------------
    virtual void onRelease() override
    {
        //GLLocation::onRelease();
        m_index = 0;
        m_stride = 0;
        m_offset = 0;
    }

private:

    //! \brief Hack ! This is an alias for m_handle but with a different type.
    GLuint m_index = 0;
    //! \brief Specifies the byte offset between consecutive generic vertex
    //! attributes. See OpenGL API doc for glVertexAttribPointer().
    size_t m_stride = 0;
    //! \brief Specifies a offset of the first component of the first generic
    //! vertex attribute in the array in the data store. See OpenGL API doc for
    //! glVertexAttribPointer().
    size_t m_offset = 0;
};

#endif // OPENGLCPPWRAPPER_GLATTRIBUTES_HPP