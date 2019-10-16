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

#ifndef OPENGLCPPWRAPPER_CLASSES_HPP
#  define OPENGLCPPWRAPPER_CLASSES_HPP

#  include "private/NonCppStd.hpp"

namespace glwrap
{

DECLARE_CLASS(GLRenderBuffer)
DECLARE_CLASS(GLTextureBuffer)
DECLARE_CLASS(GLColorBuffer)
DECLARE_CLASS(GLDepthBuffer)
DECLARE_CLASS(GLStencilBuffer)
DECLARE_CLASS(GLFrameBuffer)

DECLARE_CLASS(GLLocation) // Used by GLProgram
DECLARE_CLASS(GLAttribute)
//DECLARE_CLASS(GLUniform)
DECLARE_CLASS(GLSampler) // Used by GLProgram
DECLARE_CLASS(GLSampler1D)
DECLARE_CLASS(GLSampler2D)
DECLARE_CLASS(GLSampler3D)

DECLARE_CLASS(GLProgram)
DECLARE_CLASS(GLShader)
DECLARE_CLASS(GLVertexShader)
DECLARE_CLASS(GLFragmentShader)
DECLARE_CLASS(GLGeometryShader)

DECLARE_CLASS(IGLTexture) // Used by GLVAO
DECLARE_CLASS(GLTexture1D)
DECLARE_CLASS(GLTexture2D)
DECLARE_CLASS(GLTextureDepth2D)
DECLARE_CLASS(GLTexture3D)
DECLARE_CLASS(GLTextureCube)

DECLARE_CLASS(GLVAO)
DECLARE_CLASS(IGLBuffer) // Used by GLVAO
//DECLARE_CLASS(GLBuffer)
//DECLARE_CLASS(GLVertexBuffer)
//DECLARE_CLASS(GLIndexBuffer)

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_CLASSES_HPP
