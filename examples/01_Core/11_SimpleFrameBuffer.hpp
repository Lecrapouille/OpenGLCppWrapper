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

#ifndef EXAMPLE_11_SIMPLE_FRAMEBUFFER_HPP
#  define EXAMPLE_11_SIMPLE_FRAMEBUFFER_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;

//------------------------------------------------------------------------------
//! \file This example draw a quad in a framebuffer.
//!
//! \note This example takes its inspiration from
//! the Glumpy project gloo-framebuffer.py example.
//------------------------------------------------------------------------------
class SimpleFrameBuffer: public IGLWindow
{
public:

  SimpleFrameBuffer();
  ~SimpleFrameBuffer();

private:

  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader      m_vs_plane;
  GLFragmentShader    m_fs_plane;
  GLProgram           m_prog_plane;
  GLVAO               m_plane;

  GLVertexShader      m_vs_screen;
  GLFragmentShader    m_fs_screen;
  GLProgram           m_prog_screen;
  GLVAO               m_screen;

  GLFrameBuffer       m_fbo;
};

#endif // EXAMPLE_11_SIMPLE_FRAMEBUFFER_HPP
