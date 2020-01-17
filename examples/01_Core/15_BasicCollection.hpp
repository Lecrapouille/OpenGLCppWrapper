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

#ifndef EXAMPLE_15_COLLECTION_HPP
#define EXAMPLE_15_COLLECTION_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;
using namespace glwrap::window;

// *****************************************************************************
//! \brief This example shows how to create a basic light.
// *****************************************************************************
class BasicCollection: public IGLWindow
{
public:

  BasicCollection();
  ~BasicCollection();

private:

  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  Collection         m_collection;
  GLVertexShader     m_vs;
  GLFragmentShader   m_fs;
  GLProgram          m_prog;
};

#endif // EXAMPLE_15_COLLECTION_HPP
