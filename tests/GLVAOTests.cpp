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
#include "GLVAO.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

TESTSUITE(VAOs)
{
  TEST(TestVBOs)
    {
      // TODO
      // GLVAO vao0(nullptr);

      // VAO with no name
      GLVAO vao0("");
      ASSERT_EQ(true, vao0.name() == "");
      ASSERT_EQ(GL_ARRAY_BUFFER, vao0.target());

      // VAO with name
      GLVAO vao("vao");
      ASSERT_EQ(true, vao.name() == "vao");
      ASSERT_EQ(GL_ARRAY_BUFFER, vao.target());

      // Check there is no VBOs
      std::vector<std::string> vbo_names = vao.VBONames();
      ASSERT_EQ(0_z, vbo_names.size());
      ASSERT_EQ(false, vao.hasVBOs());
      ASSERT_EQ(false, vao.hasVBO(""));

      // Add the first VBO. Check it has been inserted.
      ASSERT_EQ(true, vao.createVBO<float>("vbo1", 0_z, BufferUsage::DYNAMIC_DRAW));
      vbo_names = vao.VBONames();
      ASSERT_EQ(1_z, vbo_names.size());
      ASSERT_EQ(true, vbo_names[0] == "vbo1");
      ASSERT_EQ(true, vao.hasVBO("vbo1"));
      ASSERT_EQ(false, vao.hasVBO(""));

      // FIXME: is it a wanted behavior ?
      // Add another VBO: same name but different type.
      // Check it has not been inserted.
      ASSERT_EQ(false, vao.createVBO<int>("vbo1", 0_z, BufferUsage::DYNAMIC_DRAW));
      vbo_names = vao.VBONames();
      ASSERT_EQ(1_z, vbo_names.size());
      ASSERT_EQ(true, vbo_names[0] == "vbo1");
      ASSERT_EQ(true, vao.hasVBO("vbo1"));
      ASSERT_EQ(false, vao.hasVBO("vbo2"));

      // Add the second VBO. Check it has been inserted.
      ASSERT_EQ(true, vao.createVBO<float>("vbo2", 0_z, BufferUsage::DYNAMIC_DRAW));
      vbo_names = vao.VBONames();
      std::sort(vbo_names.begin(), vbo_names.end());
      ASSERT_EQ(2_z, vbo_names.size());
      ASSERT_EQ(true, vbo_names[0] == "vbo1");
      ASSERT_EQ(true, vbo_names[1] == "vbo2");
      ASSERT_EQ(true, vao.hasVBO("vbo1"));
      ASSERT_EQ(true, vao.hasVBO("vbo2"));
      ASSERT_EQ(false, vao.hasVBO(""));

      // Get a VBO
      GLVertexBuffer<float>& vbof = vao.VBO<float>("vbo2");
      ASSERT_EQ(true, vbof.name() == "vbo2");
      ASSERT_EQ(true, vbof.needCreate());
      ASSERT_EQ(true, vbof.needSetup());
      ASSERT_EQ(false, vbof.needUpdate());
      ASSERT_EQ(false, vbof.canBeReleased());

      // Get a VBO with the type not inserted
      try {
        GLVertexBuffer<int>& vboi = vao.VBO<int>("vbo1");
        std::cout << vboi.name() << std::endl;
        ASSERT_FALSE("Unknown VBO Exception not caught");
      }
      catch (OpenGLException) { }

      // Nullptr exception
      try
        {
          GLVertexBuffer<int>& vbo = vao.VBO<int>(nullptr);
          std::cout << vbo.name() << std::endl;
          ASSERT_FALSE("Nullptr Exception not caught");
        }
      catch (OpenGLException) { }

      // Unknown VBO
      try
        {
          GLVertexBuffer<int>& vbo = vao.VBO<int>("");
          std::cout << vbo.name() << std::endl;
          ASSERT_FALSE("Unknown VBO Exception not caught");
        }
      catch (OpenGLException) { }

      // Bad cast VBO
       try
        {
          GLVertexBuffer<double>& vbo = vao.VBO<double>("vbo1");
          std::cout << vbo.name() << std::endl;
          ASSERT_FALSE("Bad casted VBO Exception not caught");
        }
        catch (OpenGLException) { }
    }
}
