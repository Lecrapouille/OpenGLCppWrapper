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

#include "00_ColorfulTriangle.hpp"
#include <iostream>

//------------------------------------------------------------------------------
// \brief GLObjects instance have a name. This name is usually used for logs and
// debug purpose such as GLShader, GLProgram, GLVAO, GLTexture but some
// GLObjects have name used as key for hash maps. In this case these instance
// are internally (privately used) and their name managed automatically such
// GLVBO, GLAttributes, GLSamplers.
//------------------------------------------------------------------------------
ColorfulTriangle::ColorfulTriangle(uint32_t const width, uint32_t const height,
                               const char *title)
    : GLWindow(width, height, title),
      m_prog("Prog"),
      m_triangle("triangle")
{
    std::cout << "Hello ColorfulTriangle: " << info() << std::endl;
}

//------------------------------------------------------------------------------
ColorfulTriangle::~ColorfulTriangle()
{
    std::cout << "Bye ColorfulTriangle" << std::endl;
}

//------------------------------------------------------------------------------
void ColorfulTriangle::onWindowResized()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------------------
//! \brief Load a vertex shader and a fragment shader from string. Compile them.
//! Bind a VAO object (our 3D shape, here a triangle) to the GLProgram in the
//! aim to create VBOs (position, color). Fill these VBOs with data (vertex
//! positions and vertex colors).
//------------------------------------------------------------------------------
bool ColorfulTriangle::onSetup()
{
    // Load vertex and fragment shaders (GLSL code) from strings. There are two
    // type of strings: -- classic string but you need to add explicitly chars
    // such as '"' '\n' -- C++11 raw string litterals avoid to add '\n'
    // chars. In this function, we will show the two ways. In next examples you
    // will see how to load shaders from files.

    // Shaders have operators to insert text.
    std::string v2f("struct v2f_s {                       \n"
                    "  vec3      color;                   \n"
                    "} v2f;                               \n");

    // Load vertex shader from a classic string.
    m_vertex_shader <<
            "in vec2      position;                   \n"
            "in vec3      color;                      \n"
            "out " << v2f <<
            "void main() {                            \n"
            "  v2f.color = color;                     \n"
            "  gl_Position = vec4(position, 0.0, 1.0);\n"
            "}";

    // Example: Insert string from the begining
    "#version 330 core\n" >> m_vertex_shader;

    // Load fragment shader from C++11 raw string.
    m_fragment_shader <<
            R"GLSL(#version 330 core
                   in struct v2f_s {
                     vec3 color;
                   } v2f;
                   out vec4 fragColor;
                   void main() {
                       fragColor = vec4(v2f.color, 1);
                   })GLSL";

    // Shaders can be displayed with:
    std::cout << "Shader program " << m_vertex_shader.name()
              << ":" << std::endl;
    std::cout << m_vertex_shader << std::endl << std::endl;
    std::cout << "Fragment program " << m_fragment_shader.name()
              << ":" << std::endl;
    std::cout << m_fragment_shader << std::endl << std::endl;

    // You have to compile shaders into the class GLProgram. If shaders
    // contained syntax errors the compile() will return false and errors can be
    // shown with strerror(). If your OpenGL has been compiled with success, the
    // GLProgram will create its own list of shader uniforms, attributes, and
    // samplers from shaders. In this example, shader only use the attribute
    // position and color.
    if (!m_prog.compile(m_vertex_shader, m_fragment_shader))
    {
        std::cerr << "Failed compiling OpenGL program. Reason was '"
                  << m_prog.strerror() << "'" << std::endl;
        return false;
    }

    // Helper for debugging states of your program.
    debug(m_prog);

    // To initialize your 3D model, you have to use a VAO and bind it to the
    // desired GLProgram. This last, helped with its internal list of shader
    // attributes and samplers, will populate VBOs and textures inside the VAO
    // instance. Shader attributes and samplers give their name to VBOs and
    // textures. In our example, the VAO will have a single VBO named position.
    m_prog.bind(m_triangle);

    // Helper for debugging states of your program.
    debug(m_triangle);

    // Fill VBOs of the VAO: init triangle vertex positions. Note "position"
    // shall refer to the variable position inside the GLSL code. If you rename
    // it in the shader you will have to rename here too. Beware name are case
    // sensitive.
    m_triangle.vector2f("position") =
    {
        Vector2f(-1.0f, -1.0f),
        Vector2f(1.0f, -1.0f),
        Vector2f(0.0f, 1.0f)
    };

    // Fill VBOs of the VAO: init triangle vertex colors. Note "color" shall
    // refer to the variable color inside the GLSL code. If you rename it in the
    // shader you will have to rename here too. Beware name are case sensitive.
    m_triangle.vector3f("color") =
    {
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
    };

    return true;
}

//------------------------------------------------------------------------------
bool ColorfulTriangle::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    // Render a triangle from 3 vertices, starting from the first (0th).
    m_prog.draw(m_triangle, Mode::TRIANGLES, 0, 3);

    return true;
}

//------------------------------------------------------------------------------
void ColorfulTriangle::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
void ColorfulTriangle::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
