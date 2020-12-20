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

#ifndef EXAMPLE_04_INDEXED_BOX_HPP
#  define EXAMPLE_04_INDEXED_BOX_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include "../debug.hpp"

// *****************************************************************************
//! \brief This example shows how to create a simple textured triangle.
// *****************************************************************************
class IndexedQuad: public GLWindow
{
public:

    IndexedQuad(uint32_t const width, uint32_t const height, const char *title);
    ~IndexedQuad();

    static std::string info()
    {
        return "Render an indexed box";
    }

private:

    virtual bool onSetup() override;
    virtual bool onPaint() override;
    virtual void onSetupFailed(std::string const& reason) override;
    virtual void onPaintFailed(std::string const& reason) override;
    virtual void onWindowResized() override;

private:

    GLVertexShader     m_vertex_shader;
    GLFragmentShader   m_fragment_shader;
    GLProgram          m_prog;

    //! \brief Indexed VAO where indices are uint32_t.
    //! You can use instead GLVAO16 where indices are uint16_t.
    //! You can use instead GLVAO8 where indices are uint8_t.
    GLVAO32            m_box;
    GLVAO32            m_box2;
};

#endif // EXAMPLE_04_INDEXED_BOX_HPP
