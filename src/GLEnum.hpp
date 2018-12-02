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

#ifndef GLENUM_HPP
#  define GLENUM_HPP

// **************************************************************
//! GLTextures
// **************************************************************

enum class TextureMinFilter : GLenum
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  };

enum class TextureMagFilter : GLenum
  {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
  };

enum class TextureWrap : GLenum
  {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    REPEAT = GL_REPEAT,
  };

enum class PixelFormat : GLenum
  {
    ALPHA = GL_ALPHA,
    LUMINANCE = GL_LUMINANCE,
    LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
  };

enum class PixelType : GLenum
  {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  };

struct TextureOptions
{
  TextureMinFilter minFilter = TextureMinFilter::LINEAR;
  TextureMagFilter magFilter = TextureMagFilter::LINEAR;
  TextureWrap wrapS = TextureWrap::CLAMP_TO_EDGE;
  TextureWrap wrapT = TextureWrap::CLAMP_TO_EDGE;
  PixelFormat cpuPixelFormat = PixelFormat::RGBA;
  PixelFormat gpuPixelFormat = PixelFormat::RGBA;
  PixelType pixelType = PixelType::UNSIGNED_BYTE;
  bool generateMipmaps = false;
};

// **************************************************************
//! GLBuffer
// **************************************************************

enum class BufferUsage : GLenum
  {
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    STATIC_DRAW = GL_STATIC_DRAW,
    STREAM_DRAW = GL_STREAM_DRAW
  };

enum class FrameBuffer : GLenum
  {
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT
  };

enum class DrawPrimitive : GLenum
  {
    POINTS = GL_POINTS,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    LINES = GL_LINES,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    LINE_STRIP = GL_LINE_STRIP,
    LINE_LOOP = GL_LINE_LOOP,
    PATCHES = GL_PATCHES
  };

#endif
