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

#ifndef OPENGLCPPWRAPPER_GLBUFFERS_HPP
#  define OPENGLCPPWRAPPER_GLBUFFERS_HPP

#  include "OpenGL/Buffer.hpp"

// *****************************************************************************
//! \brief Buffer for vertex attribute data.
// *****************************************************************************
template<typename T>
class GLVertexBuffer: public GLBuffer<T>
{
public:

    //--------------------------------------------------------------------------
    //! \brief Allows:
    //! \code
    //!   GLVertexBuffer<float> vbo;
    //!   vbo = { 1.0f, 2.0f };
    //! \endcode
    //! Else GLVertexBuffer define a couple of operator=() that hide the
    //! operators inherited from GLBuffer. See
    //! https://stackoverflow.com/questions/64633899/no-inheritance-found-with-
    //! operator-and-initializer-list
    //--------------------------------------------------------------------------
    using GLBuffer<T>::operator=;

    //--------------------------------------------------------------------------
    //! \brief Constructor with the object name and reserved number of
    //! elements.
    //--------------------------------------------------------------------------
    explicit GLVertexBuffer(std::string const& name, size_t const size,
                            BufferUsage const usage)
        : GLBuffer<T>(name, GL_ARRAY_BUFFER, size, usage)
    {}
};

// *****************************************************************************
//! \brief Buffer for index data.
// *****************************************************************************
template<typename T>
class GLIndexBuffer: public GLBuffer<T>
{
public:

    //--------------------------------------------------------------------------
    //! \brief Constructor with the object name
    //--------------------------------------------------------------------------
    explicit GLIndexBuffer(std::string const& name, BufferUsage const usage)
        : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
    {}

    //--------------------------------------------------------------------------
    //! \brief Constructor with the object name and reserved number of
    //! elements.
    //--------------------------------------------------------------------------
    explicit GLIndexBuffer(std::string const& name, const size_t size,
                           BufferUsage const usage)
        : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, size, usage)
    {}

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline GLenum type() const;
};

template<>
inline GLenum GLIndexBuffer<uint32_t>::type() const { return GL_UNSIGNED_INT; }

template<>
inline GLenum GLIndexBuffer<uint16_t>::type() const { return GL_UNSIGNED_SHORT; }

template<>
inline GLenum GLIndexBuffer<uint8_t>::type() const { return GL_UNSIGNED_BYTE; }

// *****************************************************************************
//! \brief
// *****************************************************************************

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer32 = GLIndexBuffer<uint32_t>;

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer16 = GLIndexBuffer<uint16_t>;

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer8 = GLIndexBuffer<uint8_t>;

#endif // OPENGLCPPWRAPPER_GLBUFFERS_HPP
