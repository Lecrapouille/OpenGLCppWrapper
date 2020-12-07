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

#include "OpenGL/Buffers/VAO.hpp"
#include "OpenGL/Shaders/Program.hpp"

//uint32_t count() { return static_cast<uint32_t>(m_VBOs.begin()->second->size())); }

//--------------------------------------------------------------------------
bool GLVAO::bound(GLenum const prog_id) const
{
    return (m_program != nullptr) && (m_program->handle() == prog_id);
}

//--------------------------------------------------------------------------
bool GLVAO::bound() const
{
    return (m_program != nullptr) && (m_program->handle() != 0u);
}

//----------------------------------------------------------------------------
//! \brief Populate the VAO with a list of VBOs and textures. The
//! number depends on the number of shader attributes and uniform
//! texture samplers.
//----------------------------------------------------------------------------
void GLVAO::init(GLProgram& program)
{
    // Create a list of VBOs. TODO: manage integers
    for (auto& it: program.attributes())
    {
        const char *name = it.first.c_str();
        switch (it.second->size())
        {
        case 1:
            if (!m_VBOs.has<std::shared_ptr<GLVertexBuffer<float>>>(name))
            {
                auto ptr = std::make_shared<GLVertexBuffer<float>>(name, m_reserve, m_usage);
                m_VBOs.add(name, ptr);
                m_listBuffers[name] = ptr;
            }
            break;
        case 2:
            if (!m_VBOs.has<std::shared_ptr<GLVertexBuffer<Vector2f>>>(name))
            {
                auto ptr = std::make_shared<GLVertexBuffer<Vector2f>>(name, m_reserve, m_usage);
                m_VBOs.add(name, ptr);
                m_listBuffers[name] = ptr;
            }
            break;
        case 3:
            if (!m_VBOs.has<std::shared_ptr<GLVertexBuffer<Vector3f>>>(name))
            {
                auto ptr = std::make_shared<GLVertexBuffer<Vector3f>>(name, m_reserve, m_usage);
                m_VBOs.add(name, ptr);
                m_listBuffers[name] = ptr;
            }
            break;
        case 4:
            if (!m_VBOs.has<std::shared_ptr<GLVertexBuffer<Vector4f>>>(name))
            {
                auto ptr = std::make_shared<GLVertexBuffer<Vector4f>>(name, m_reserve, m_usage);
                m_VBOs.add(name, ptr);
                m_listBuffers[name] = ptr;
            }
            break;
        default:
            throw GL::Exception("Attribute with dimension > 4 is not managed");
        }
    }

    // Create a list of textures.
    //m_listTextures.reserve(program.samplers().size());
    for (auto& it: program.samplers())
    {
        const char *name = it.first.c_str();
        const GLenum gltype = it.second->target();
        switch (gltype)
        {
        case GL_SAMPLER_1D:
            {
                std::shared_ptr<GLTexture1D> ptr = std::make_shared<GLTexture1D>(name);
                m_textures.add(name, ptr);
                m_listTextures[name] = ptr;
            }
            break;
        case GL_SAMPLER_2D: // TODO GLTextureDepth2D
            {
                std::shared_ptr<GLTexture2D> ptr = std::make_shared<GLTexture2D>(name);
                m_textures.add(name, ptr);
                m_listTextures[name] = ptr;
            }
            break;
        case GL_SAMPLER_3D:
            {
                std::shared_ptr<GLTexture3D> ptr = std::make_shared<GLTexture3D>(name);
                m_textures.add(name, ptr);
                m_listTextures[name] = ptr;
            }
            break;
        case GL_SAMPLER_CUBE:
            {
                std::shared_ptr<GLTextureCube> ptr = std::make_shared<GLTextureCube>(name);
                m_textures.add(name, ptr);
                m_listTextures[name] = ptr;
            }
            break;
        default:
            throw GL::Exception("This kind of sampler is not managed: "
                                + std::to_string(gltype));
        }
    }

    // Make VAO and GLProgram be coupled.
    m_program = &program;
}
