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

#include "main.hpp"
#include <iostream>

Example::Example(uint32_t const width, uint32_t const height, const char *title)
    : GLWindow(width, height, title),
      m_prog("Prog"),
      m_mesh("VAO_triangle")
{
    std::cout << "Hello Example" << std::endl;
}

Example::~Example()
{
    std::cout << "Bye Example" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void Example::onResized()
{
    // Make the viewport matches the new window dimensions.
    glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Make the instropection of VAO and program shader.
//! \note From the previous example we add sampler lists.
//------------------------------------------------------------------
void Example::onDebug()
{
    // Display the list of VBOs hold by the VAO
    {
        std::vector<std::string> vbo_names;
        size_t count = m_mesh.getVBONames(vbo_names);
        std::cout << "VAO '" << m_mesh.name() << "' has "
                  << count << " VBO: " << std::endl;
        for (auto& it: vbo_names)
        {
            std::cout << "  '" << it << "'" << std::endl;
        }
    }

    // Display the list of Uniforms hold by the program
    {
        std::vector<std::string> uniform_names;
        size_t count = m_prog.getUniformNames(uniform_names);
        std::cout << "Prog '" << m_prog.name() << "' has "
                  << count << " uniforms: " << std::endl;
        for (auto& it: uniform_names)
        {
            std::cout << "  '" << it << "'" << std::endl;
        }
    }

    // Display the list of Samplers hold by the program
    {
        std::vector<std::string> sampler_names;
        size_t count = m_prog.getSamplerNames(sampler_names);
        std::cout << "Prog '" << m_prog.name() << "' has "
                  << count << " samplers: " << std::endl;
        for (auto& it: sampler_names)
        {
            std::cout << "  '" << it << "'" << std::endl;
        }
    }
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
bool Example::loadTextures()
{
    GLTexture2D& texture = m_mesh.texture2D("texID");
    texture.interpolation(GLTexture::Minification::LINEAR,
                          GLTexture::Magnification::LINEAR);
    texture.wrap(GLTexture::Wrap::MIRRORED_REPEAT);
    return texture.load("/home/qq/MyGitHub/OpenGLCppWrapper/examples/textures/hazard.png");
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBOs (vertex and texture position). Load all textures.
//------------------------------------------------------------------
bool Example::onSetup()
{
    GLVertexShader     vertex_shader;
    GLFragmentShader   fragment_shader; // FIXME: GLdeleteShaders() apres compile()

    // Load vertex and fragment shaders with GLSL code.
    vertex_shader.read("/home/qq/MyGitHub/OpenGLCppWrapper/src/shaders/02_TexturedTriangle.vs");
    fragment_shader.read("/home/qq/MyGitHub/OpenGLCppWrapper/src/shaders/02_TexturedTriangle.fs");

    // Compile the shader program
    if (!m_prog.compile(vertex_shader, fragment_shader))
    {
        std::cerr << "Failed compiling OpenGL program. Reason was '"
                  << m_prog.strerror() << "'" << std::endl;
        return false;
    }

    // Create VBOs of the VAO.
    m_prog.bind(m_mesh);

    // Fill VBOs of the VAO: init triangle vertex positions.
    m_mesh.vector3f("position") =
    {
        Vector3f(-1.0f, -1.0f, 0.0f),
        Vector3f(1.0f, -1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f)
    };

    // Fill VBOs of the VAO: init triangle texture positions.
    m_mesh.vector2f("UV") =
    {
        Vector2f(0.0f, 0.0f),
        Vector2f(1.0f, 0.0f),
        Vector2f(0.5f, 1.0f)
    };

    // Repeat the texture motif
    m_mesh.vector2f("UV") *= 4.0f;

    // Fill Load texture files
    if (!loadTextures())
    {
        // In case of failure show which textures has failed.
        std::vector<std::string> list;
        size_t count = m_mesh.getFailedSamplers(list);
        std::cerr << "Failed loading " << count << " textures:" << std::endl;
        for (auto& it: list)
        {
            std::cerr << " " << it;
        }
        std::cerr << std::endl;
        return false;
    }

    // Helper for debugging states of your program
    //onDebug();

    //std::cout << m_mesh.vector3f("position") << std::endl;
    //std::cout << m_mesh.vector2f("UV") << std::endl;

    return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool Example::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    m_prog.draw(m_mesh, Mode::TRIANGLES, 0, 3);
    //m_mesh.draw(Mode::TRIANGLES, 0, 3);

    return true;
}

void Example::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

void Example::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}

int main()
{
    bool res;

    try
    {
        GLApplication app;
        app.create<Example>(800u, 600u, "example1");
        app.create<Example>(800u, 600u, "example2");
        res = app.start() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch (const GL::Exception& e)
    {
        std::cerr << e.message() << std::endl;
        res = EXIT_FAILURE;
    }

    return res;
}
