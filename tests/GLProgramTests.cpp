//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#define protected public
#define private public
#include "OpenGL/Program.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

TESTSUITE(Programs)
{
  TEST(TestCreators)
    {
      GLProgram prog("prog");

      ASSERT_EQ(false, prog.isCompiled());
      ASSERT_EQ(false, prog.isBound());
      ASSERT_EQ(false, prog.hasErrored());
      ASSERT_EQ(true,  prog.getError() == "");
      ASSERT_EQ(0_z, prog.getShaderNames().size());
      ASSERT_EQ(0_z, prog.getFailedShaders().size());
      ASSERT_EQ(0_z, prog.getAttributeNames().size());
      ASSERT_EQ(0_z, prog.getUniformNames().size());

      ASSERT_EQ(0_z, prog.getUniformNames().size());
      ASSERT_EQ(false, prog.hasUniforms());
      ASSERT_EQ(false, prog.hasAttributes());
      ASSERT_EQ(false, prog.hasUniform(""));
      ASSERT_EQ(0_z, prog.getUniformNames().size());
      ASSERT_EQ(false, prog.hasAttribute(""));
      ASSERT_EQ(false, prog.hasUniform(nullptr));
      ASSERT_EQ(false, prog.hasAttribute(nullptr));
      ASSERT_EQ(0_z, prog.getUniformNames().size());

      //---
      try {
        prog.throw_if_not_compiled();
        ASSERT_TRUE("Exception should have occured");
      } catch(...) { }

      prog.m_compiled = true;
      try {
        prog.throw_if_not_compiled();
      } catch(...) { ASSERT_TRUE("Exception should have not occured"); }

      // ---
      try {
        prog.uniform<int>("");
        ASSERT_TRUE("Exception should have occured");
      } catch(...) { }

      try {
        prog.uniform<int>("aaa");
        ASSERT_TRUE("Exception should have occured");
      } catch(OpenGLException&) { }

      try {
        prog.uniform<int>(nullptr);
        ASSERT_TRUE("Exception should have occured");
      } catch(OpenGLException&) { }

      // TODO: try add name conflict wit different types
      prog.addNewUniform(GL_FLOAT, "u1");
      prog.addNewUniform(GL_FLOAT_VEC2, "u2");
      prog.addNewUniform(GL_FLOAT_VEC3, "u3");
      prog.addNewUniform(GL_FLOAT_VEC4, "u4");
      ASSERT_EQ(4_z, prog.getUniformNames().size());
      ASSERT_EQ(true, prog.hasUniform("u1"));
      ASSERT_EQ(true, prog.hasUniform("u2"));
      ASSERT_EQ(true, prog.hasUniform("u3"));
      ASSERT_EQ(true, prog.hasUniform("u4"));

      //Old API
      //try {
      //  prog.getVBO<int>("");
      //  ASSERT_TRUE("Exception should have occured");
      //} catch(OpenGLException) { }

      GLVAO vao("VAO");
      ASSERT_EQ(0, vao.prog);
      prog.m_handle = 42;
      ASSERT_EQ(true, prog.bind(vao));
      ASSERT_EQ(true, prog.isBound());
      ASSERT_EQ(42, vao.prog);
      ASSERT_EQ(true, prog.m_vao == &vao);

      //Old API
      //try {
      //  prog.getVBO<int>("");
      //  ASSERT_TRUE("Exception should have occured");
      //} catch(OpenGLException) { }

      // TODO: try add name conflict wit different types
      ASSERT_EQ(0_z, prog.getAttributeNames().size());
      prog.addNewAttribute(GL_FLOAT, "a1");
      prog.addNewAttribute(GL_FLOAT_VEC2, "a2");
      prog.addNewAttribute(GL_FLOAT_VEC3, "a3");
      prog.addNewAttribute(GL_FLOAT_VEC4, "a4");
      ASSERT_EQ(4_z, prog.getAttributeNames().size());
      ASSERT_EQ(true, prog.hasAttribute("a1"));
      ASSERT_EQ(true, prog.hasAttribute("a2"));
      ASSERT_EQ(true, prog.hasAttribute("a3"));
      ASSERT_EQ(true, prog.hasAttribute("a4"));

      GLVertexShader shader("foo");
      prog.attachShader(shader);
      auto getShaderNames = prog.getShaderNames();
      ASSERT_EQ(1_z, getShaderNames.size());
      ASSERT_EQ(true, getShaderNames.end() !=
                std::find(getShaderNames.begin(), getShaderNames.end(), "foo"));

      GLVAO vao2("VAO2");
      ASSERT_EQ(0, vao2.prog);
      prog.initVAO(vao2);
      ASSERT_EQ(42, vao2.prog);
      ASSERT_EQ(true, vao2.hasVBOs());
      ASSERT_EQ(true, vao2.hasVBO("a1"));
      ASSERT_EQ(true, vao2.hasVBO("a2"));
      ASSERT_EQ(true, vao2.hasVBO("a3"));
      ASSERT_EQ(true, vao2.hasVBO("a4"));
      ASSERT_EQ(true, prog.m_vao != &vao2);
      ASSERT_EQ(true, prog.m_vao == &vao);

      // Restore uninitialized prog else OpenGL will segfault
      prog.m_handle = 0;
    }

    TEST(bindVAOtoWrongGLProg)
    {
      GLProgram prog1("prog1");
      prog1.m_handle = 42;
      prog1.m_compiled = true;
      GLProgram prog2("prog2");
      prog2.m_handle = 43;
      prog2.m_compiled = true;
      GLVAO vao1("VAO1");
      GLVAO vao2("VAO2");

      ASSERT_EQ(0, vao1.prog);
      ASSERT_EQ(0, vao2.prog);
      ASSERT_EQ(true, prog1.bind(vao1));
      ASSERT_EQ(true, prog2.bind(vao2));
      ASSERT_EQ(42, vao1.prog);
      ASSERT_EQ(43, vao2.prog);
      // Try binding VAO to the wrong GLProg
      ASSERT_EQ(false, prog1.bind(vao2));
      ASSERT_EQ(false, prog2.bind(vao1));
    }
}
