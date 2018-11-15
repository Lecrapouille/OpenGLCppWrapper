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

#include "Logger.hpp"
#define protected public
#define private public
#include "OpenGL.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

TESTSUITE(VBOs)
{
  TEST(TestVBOs)
    {
      //
      GLVertexBuffer<int> vbo("vbo");
      ASSERT_EQ(true, vbo.name() == "vbo");
      ASSERT_EQ(GL_ARRAY_BUFFER, vbo.target());
      ASSERT_EQ(GL_DYNAMIC_DRAW, vbo.usage());
      ASSERT_EQ(false, vbo.needUpdate());
    }
}
