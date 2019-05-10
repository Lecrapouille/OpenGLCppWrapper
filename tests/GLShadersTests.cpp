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
#include "GLShaders.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

TESTSUITE(Shaders)
{
  TEST(TestCreators)
    {
      GLVertexShader shaderNoName1;
      ASSERT_EQ(true, shaderNoName1.name() == "VertexShader");
      GLFragmentShader shaderNoName2;
      ASSERT_EQ(true, shaderNoName2.name() == "FragmentShader");
      GLGeometryShader shaderNoName3;
      ASSERT_EQ(true, shaderNoName3.name() == "GeometryShader");

      GLVertexShader shader1("vert");
      ASSERT_EQ(true, shader1.name() == "vert");
      ASSERT_EQ(GL_VERTEX_SHADER, shader1.target());

      GLFragmentShader shader2("frag");
      ASSERT_EQ(true, shader2.name() == "frag");
      ASSERT_EQ(GL_FRAGMENT_SHADER, shader2.target());

      GLGeometryShader shader3;
      ASSERT_EQ(true, shader3.name() == "GeometryShader");
      ASSERT_EQ(GL_GEOMETRY_SHADER, shader3.target());

      //--- Check initial state
      ASSERT_EQ(true, shader1.m_shader_code == "");
      ASSERT_EQ(true, shader1.m_error_msg == "");
      ASSERT_EQ(false, shader1.m_compiled);
      ASSERT_EQ(0, shader1.m_attached);

      //--- Check initial state
      ASSERT_EQ(true, shader2.code() == "");
      ASSERT_EQ(true, shader2.getError() == "");
      ASSERT_EQ(false, shader2.hasErrored());
      ASSERT_EQ(false, shader2.isCompiled());
      ASSERT_EQ(0, shader2.m_attached);

      //--- Load from string
      shader1.fromString("foobar");
      ASSERT_EQ(false, shader1.hasErrored());
      ASSERT_EQ(true, shader1.code() == "foobar");
      ASSERT_EQ(true, shader1.loaded());
      ASSERT_EQ(false, shader1.isCompiled());
      try {
        shader1.throw_if_not_loaded();
      } catch(...) { ASSERT_TRUE("Exception should have not occured"); }

      try {
        shader1.throw_if_already_compiled();
        ASSERT_TRUE("Exception should have occured");
      } catch(...) { }

      shader1.fromString("");
      ASSERT_EQ(false, shader1.hasErrored());
      ASSERT_EQ(true, shader1.code() == "");
      ASSERT_EQ(false, shader1.loaded());
      ASSERT_EQ(false, shader1.isCompiled());

      //--- Load from wrong file
      ASSERT_EQ(true, shader3.code() == "");
      ASSERT_EQ(false, shader3.fromFile("this_file_does_not_exist"));
      ASSERT_EQ(false, shader3.isCompiled());
      ASSERT_EQ(true, shader3.hasErrored());
      ASSERT_EQ(true, shader3.code() == "");
      ASSERT_EQ(false, shader3.isCompiled());
      ASSERT_EQ(true, shader3.getError() != "");
      ASSERT_EQ(false, shader3.hasErrored());
      try {
        shader3.throw_if_not_loaded();
        ASSERT_TRUE("Exception should have occured");
      } catch(...) {  }
      try {
        shader3.throw_if_already_compiled();
      } catch(...) { ASSERT_TRUE("Exception should have not occured"); }
      shader3.m_need_setup = false;
      try {
        shader3.throw_if_already_compiled();
        ASSERT_TRUE("Exception should have occured");
      } catch(...) { }

      //---
      shader2.attachProg(0);
      ASSERT_EQ(0, shader2.attached());
      shader2.attachProg(42);
      ASSERT_EQ(42, shader2.attached());

      //--- Try setup
      shader1.m_error_msg = "";
      shader1.fromString(""); shader1.m_compiled = true;
      shader1.setup();
      ASSERT_EQ(true, shader1.hasErrored());
      ASSERT_EQ(true, shader1.getError() != "");
      shader1.setup();

      //--- Try setup
      shader1.m_error_msg = "";
      shader1.fromString("ff"); shader1.m_compiled = true;
      shader1.setup();
      ASSERT_EQ(true, shader1.hasErrored());
      ASSERT_EQ(true, shader1.getError() != "");
      shader1.setup();


      // TODO shader1.begin();
    }
}
