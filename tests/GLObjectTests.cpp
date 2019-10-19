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
#include "OpenGL/GLObject.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

class FakeObject : public GLObject<GLint>
{
public:

  FakeObject()
    : GLObject<GLint>("FakeObject")
  {
    reset();
  }

  virtual ~FakeObject() { destroy(); }

  void reset()
  {
    GLObject<GLint>::init();
    m_position = 0;
    m_setup_position = 0;
    m_create_position = 0;
    m_activate_position = 0;
    m_update_position = 0;
    m_deactivate_position = 0;
    m_release_position = 0;
  }

private:

  virtual bool setup() override
  {
    m_setup_position = m_position++;
    return false;
  }

  virtual bool create() override
  {
    m_handle = 0; // fake creation
    m_create_position = m_position++;
    return false;
  }

  virtual void activate() override
  {
    m_activate_position = m_position++;
  }

  virtual bool update() override
  {
    m_update_position = m_position++;
    return false;
  }

  virtual void deactivate() override
  {
    m_deactivate_position = m_position++;
  }

  virtual void release() override
  {
    m_release_position = m_position++;
  }

public:

  uint32_t m_position;
  uint32_t m_setup_position;
  uint32_t m_create_position;
  uint32_t m_activate_position;
  uint32_t m_update_position;
  uint32_t m_deactivate_position;
  uint32_t m_release_position;
};

TESTSUITE(TS_GLObject)
{
  TEST(T_GLObject)
    {
      //opengl::hasCreatedContext() = true;
      FakeObject obj;

      // --- Check init
      ASSERT_EQ(obj.m_handle, -1);
      ASSERT_EQ(obj.gpuID(), -1);
      ASSERT_EQ(obj.m_target, 0U);
      ASSERT_EQ(obj.target(), 0U);
      ASSERT_EQ(obj.m_need_setup, true);
      ASSERT_EQ(obj.m_need_create, true);
      ASSERT_EQ(obj.m_need_update, false);
      ASSERT_EQ(obj.m_position, 0);
      ASSERT_EQ(obj.m_setup_position, 0);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 0);
      ASSERT_EQ(obj.m_update_position, 0);
      ASSERT_EQ(obj.m_deactivate_position, 0);
      ASSERT_EQ(obj.m_release_position, 0);

      // --- glBegin(FakeObject)
      obj.begin();
      ASSERT_EQ(obj.m_handle, 0);
      ASSERT_EQ(obj.gpuID(), 0);
      ASSERT_EQ(obj.m_target, 0U);
      ASSERT_EQ(obj.m_need_setup, false);
      ASSERT_EQ(obj.m_need_create, false);
      ASSERT_EQ(obj.m_need_update, false);
      ASSERT_EQ(obj.m_position, 3);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 1);
      ASSERT_EQ(obj.m_setup_position, 2);
      ASSERT_EQ(obj.m_deactivate_position, 0);
      ASSERT_EQ(obj.m_update_position, 0);
      ASSERT_EQ(obj.m_release_position, 0);

      // --- glEnd(FakeObject)
      obj.end();
      ASSERT_EQ(obj.m_handle, 0);
      ASSERT_EQ(obj.m_target, 0U);
      ASSERT_EQ(obj.m_need_setup, false);
      ASSERT_EQ(obj.m_need_create, false);
      ASSERT_EQ(obj.m_need_update, false);
      ASSERT_EQ(obj.m_position, 4);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 1);
      ASSERT_EQ(obj.m_setup_position, 2);
      ASSERT_EQ(obj.m_deactivate_position, 3);
      ASSERT_EQ(obj.m_update_position, 0);
      ASSERT_EQ(obj.m_release_position, 0);

      // --- Force update fake object
      obj.forceUpdate();
      ASSERT_EQ(obj.m_handle, 0);
      ASSERT_EQ(obj.m_target, 0U);
      ASSERT_EQ(obj.m_need_setup, false);
      ASSERT_EQ(obj.m_need_create, false);
      ASSERT_EQ(obj.m_need_update, true);
      obj.begin();
      ASSERT_EQ(obj.m_position, 6);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 4);
      ASSERT_EQ(obj.m_setup_position, 2);
      ASSERT_EQ(obj.m_deactivate_position, 3);
      ASSERT_EQ(obj.m_update_position, 5);
      ASSERT_EQ(obj.m_release_position, 0);
      obj.end();
      ASSERT_EQ(obj.m_position, 7);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 4);
      ASSERT_EQ(obj.m_setup_position, 2);
      ASSERT_EQ(obj.m_deactivate_position, 6);
      ASSERT_EQ(obj.m_update_position, 5);
      ASSERT_EQ(obj.m_release_position, 0);

      // --- Destroy object
      obj.destroy();
      ASSERT_EQ(obj.m_handle, -1);
      ASSERT_EQ(obj.m_target, 0U);
      ASSERT_EQ(obj.m_need_setup, true);
      ASSERT_EQ(obj.m_need_create, true);
      ASSERT_EQ(obj.m_need_update, false);
      ASSERT_EQ(obj.m_position, 7);
      ASSERT_EQ(obj.m_create_position, 0);
      ASSERT_EQ(obj.m_activate_position, 4);
      ASSERT_EQ(obj.m_setup_position, 2);
      ASSERT_EQ(obj.m_deactivate_position, 6);
      ASSERT_EQ(obj.m_update_position, 5);
      ASSERT_EQ(obj.m_release_position, 0);
    }
}
