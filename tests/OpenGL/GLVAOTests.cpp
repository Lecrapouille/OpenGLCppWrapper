//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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
#define protected public
#define private public
#  include "OpenGL/Shaders/Program.hpp"
#  include "OpenGL/Buffers/VAO.hpp"
#undef protected
#undef private

// Check initial states
TEST(TestGLVAO, TestCreators)
{
    // No OpenGL context
    {
        GLVAO vao("vao");

        // Check GLObject states
        ASSERT_STREQ("vao", vao.cname());
        ASSERT_EQ(0, vao.m_handle);
        ASSERT_EQ(GL_ARRAY_BUFFER, vao.m_target);
        ASSERT_EQ(true, vao.m_need_setup);
        ASSERT_EQ(true, vao.m_need_create);
        ASSERT_EQ(false, vao.m_need_update);

        // Check GLVAO states
        ASSERT_EQ(0_z, vao.m_vbos.size());
        ASSERT_EQ(0_z, vao.m_textures.size());
        ASSERT_EQ(nullptr, vao.m_program);
        ASSERT_EQ(0_z, vao.m_count);
        ASSERT_EQ(3_z, vao.m_reserve);
        ASSERT_EQ(BufferUsage::DYNAMIC_DRAW, vao.m_usage);
    }

    // With OpenGL context
    OpenGLContext context([]()
    {
        GLVAO vao("vao");

        // Check GLObject states
        ASSERT_STREQ("vao", vao.cname());
        ASSERT_EQ(0, vao.m_handle);
        ASSERT_EQ(GL_ARRAY_BUFFER, vao.m_target);
        ASSERT_EQ(true, vao.m_need_setup);
        ASSERT_EQ(true, vao.m_need_create);
        ASSERT_EQ(false, vao.m_need_update);

        // Check GLVAO states
        ASSERT_EQ(0_z, vao.m_vbos.size());
        ASSERT_EQ(0_z, vao.m_textures.size());
        ASSERT_EQ(nullptr, vao.m_program);
        ASSERT_EQ(0_z, vao.m_count);
        ASSERT_EQ(3_z, vao.m_reserve);
        ASSERT_EQ(BufferUsage::DYNAMIC_DRAW, vao.m_usage);
    });
}
