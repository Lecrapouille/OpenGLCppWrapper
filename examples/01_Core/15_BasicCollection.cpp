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

#include "15_BasicCollection.hpp"
#include <iostream>

BasicCollection::BasicCollection()
  : m_collection("Collection"),
    m_prog("prog")
{}

BasicCollection::~BasicCollection()
{}

// -----------------------------------------------------------------------------
//! \brief Window event
// -----------------------------------------------------------------------------
void BasicCollection::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Initialize shader and create a quad.
//------------------------------------------------------------------
bool BasicCollection::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));

  m_collection.setup(512u,
   {
     DType("Atranslate", LocType::SHARED, Vector2f(1.0f, 0.0f)),
     DType("color", LocType::SHARED, Vector4f(1.0f, 0.0f, 0.0f, 0.0f)),
     DType("QQA", LocType::SHARED, Vector3f(1.0f, 0.0f, 0.0f)),
     DType("translate", LocType::SHARED, Vector2f(1.0f, 0.0f)),
     DType("scale", LocType::SHARED, 1.0f)
   });
  createCollectionShader(m_vs, m_fs, m_collection);
  std::cout << m_vs.code() << std::endl;

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool BasicCollection::draw()
{
  static float time = 0.0f;
  time += dt();

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  return true;
}
