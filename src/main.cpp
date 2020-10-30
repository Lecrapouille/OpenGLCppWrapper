#include "OpenGLCppWrapper/UI.hpp"
#include "OpenGLCppWrapper/OpenGL.hpp"
#include <iostream>

class Window: public IGLWindow
{
public:

    Window();
    ~Window();

private:

    virtual bool onSetup() override;
    virtual bool onDraw() override;
    virtual void onSetupFailed() override;
    virtual void onDrawFailed() override;

private:

    GLProgram         m_prog;
    GLVertexShader    m_vertex_shader;
    GLFragmentShader  m_fragment_shader;
};

Window::Window()
    : m_prog("prog")
{
    std::cout << "Hello Window" << std::endl;
}

Window::~Window()
{
    std::cout << "Bye Window" << std::endl;
}

bool Window::onSetup()
{
/*
    m_vertex_shader <<
            "#version 330 core                        \n"
            "in vec2      position;                   \n"
            "uniform vec4    color;                   \n"
            "out vec4       texColor;                 \n"
            "void main() {                            \n"
            "  texColor = color;                      \n"
            "  gl_Position = vec4(position, 0.0, 1.0);\n"
            "}";

    m_fragment_shader
            << R"GLSL(#version 330 core
                      in  vec4 texColor;
                      out vec4 fragColor;
                      void main() {
                         fragColor = texColor;
                      })GLSL";
*/
m_vertex_shader.read("/home/qq/MyGitHub/OpenGLCppWrapper-NewAttempt/examples/01_Core/shaders/03_MultiTexturedTriangle.vs");
m_fragment_shader.read("/home/qq/MyGitHub/OpenGLCppWrapper-NewAttempt/examples/01_Core/shaders/03_MultiTexturedTriangle.fs");

    if (!m_prog.compile(m_vertex_shader, m_fragment_shader))
    {
        std::cerr << "Failed compiling OpenGL program. Reason was '"
                  << m_prog.strerror() << "'" << std::endl;
        return false;
    }

  // Display the list of Attributes hold by the program
  {
    std::vector<std::string> attributes_names;
    size_t count = m_prog.getAttributeNames(attributes_names);
    std::cout << "Prog '" << m_prog.name() << "' has "
              << count << " attributes: " << std::endl;
    for (auto& it: attributes_names)
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


//m_prog.vector4f("color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);

    return true;
}

bool Window::onDraw()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    return true;
}

void Window::onSetupFailed()
{
    std::cerr << "onSetupFailed() called" << std::endl;
}

void Window::onDrawFailed()
{
  std::cerr << "onDrawFailed() called" << std::endl;
}

int main()
{
    Window win;
    win.start();

    return 0;
}
