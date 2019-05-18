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

#ifndef EXAMPLE_08_TERRAIN_TEXTURE3D_HPP
#  define EXAMPLE_08_TERRAIN_TEXTURE3D_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>

using namespace glwrap;

class GLExample08: public IGLWindow
{
public:

  GLExample08()
    : m_vao("VAO_terrain"),
      m_prog("prog")
  {}

  ~GLExample08()
  {}

private:

  bool createTerrain();
  void generateAltitudes(const uint32_t dim);
  void loadTerrain(const uint32_t dim);
  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader      m_vertex_shader;
  GLFragmentShader    m_fragment_shader;
  GLVAO               m_vao;
  GLProgram           m_prog;
  std::vector<float>  m_altitudes;
  uint32_t            m_nbVertices = 0;
};

#endif // EXAMPLE_08_TERRAIN_TEXTURE3D_HPP
