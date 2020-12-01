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

#ifndef IVAO_HPP
#  define IVAO_HPP

#  include "OpenGL/Buffers/VAO.hpp"
#  include "OpenGL/Buffers/EBO.hpp"

// *****************************************************************************
//! \brief Indexed VAO.
// *****************************************************************************
template<class T>
class GLVAOi: public GLVAO
{
public:

    GLVAOi(std::string const& name, BufferUsage const usage = BufferUsage::DYNAMIC_DRAW,
           size_t const reserve = 3u)
        : GLVAO(name, usage, reserve),
          m_index("index", usage)
    {}

    inline GLElementBuffer<T>& index()
    {
        return m_index;
    }

    inline GLElementBuffer<T> const& index() const
    {
        return m_index;
    }

private:

    GLElementBuffer<T> m_index;
};

using GLVAO8 = GLVAOi<uint8_t>;
using GLVAO16 = GLVAOi<uint16_t>;
using GLVAO32 = GLVAOi<uint32_t>;

#endif
