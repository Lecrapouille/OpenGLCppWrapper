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

#include "OpenGL/VAO.hpp"
#include "OpenGL/Program.hpp"

//uint32_t count() { return static_cast<uint32_t>(m_vbos.begin()->second->size())); }

//----------------------------------------------------------------------------
//! \brief Populate the VAO with a list of VBOs and textures. The
//! number depends on the number of shader attributes and uniform
//! texture samplers.
//----------------------------------------------------------------------------
void GLVAO::init(GLProgram& prog, BufferUsage const usage, size_t const vbo_size)
{
    // Create a list of VBOs. TODO: manage integers
    for (auto& it: prog.attributes())
    {
        const char *name = it.first.c_str();
        switch (it.second->size())
        {
        case 1:
            m_vbos.add(name, std::make_shared<GLVertexBuffer<float>>
                       (name, vbo_size, usage));
            break;
        case 2:
            m_vbos.add(name, std::make_shared<GLVertexBuffer<Vector2f>>
                       (name, vbo_size, usage));
            break;
        case 3:
            m_vbos.add(name, std::make_shared<GLVertexBuffer<Vector3f>>
                       (name, vbo_size, usage));
            break;
        case 4:
            m_vbos.add(name, std::make_shared<GLVertexBuffer<Vector4f>>
                       (name, vbo_size, usage));
            break;
        default:
            throw GL::Exception("Attribute with dimension > 4 is not managed");
            break;
        }
    }

#if 0
    // Create a list of textures
    for (auto& it: m_samplers)
    {
        const char *name = it.first.c_str();
        const GLenum gltype = it.second->target();
        switch (gltype)
        {
        case GL_SAMPLER_1D:
            createTexture<GLTexture1D>(name);
            break;
        case GL_SAMPLER_2D:
            createTexture<GLTexture2D>(name);
            //FIXME createTexture<GLTextureDepth2D>(name);
            break;
        case GL_SAMPLER_3D:
            createTexture<GLTexture3D>(name);
            break;
        case GL_SAMPLER_CUBE:
            createTexture<GLTextureCube>(name);
            break;
        default:
            ERROR("This kind of sampler is not yet managed: %u", gltype);
            break;
        }
    }
#endif

    // Make VAO and GLProgram be coupled.
    m_prog_id = prog.handle();
}
