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

#ifndef OPENGLCPPWRAPPER_GLENUM_HPP
#  define OPENGLCPPWRAPPER_GLENUM_HPP

// **************************************************************
//! \file GLEnum.hpp defines OpenGL constants used in public API
//! as C++ class enum. Indeed C++ clas enum are strongly typed
//! and this will prevent the developper to pass the wrong enum
//! in a method.
// **************************************************************

//! \brief Textures Minification Filter.
enum class TextureMinFilter : GLenum
  {
    /* 0x2600 */ NEAREST = GL_NEAREST,
    /* 0x2601 */ LINEAR = GL_LINEAR,
    /* 0x2700 */ NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    /* 0x2701 */ LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    /* 0x2702 */ NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    /* 0x2703 */ LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
  };

//! \brief Textures Magnification Filter.
enum class TextureMagFilter : GLenum
  {
    /* 0x2600 */ NEAREST = GL_NEAREST,
    /* 0x2601 */ LINEAR = GL_LINEAR,
  };

//! \brief Textures Wrap.
enum class TextureWrap : GLenum
  {
    /* 0x2901 */ REPEAT = GL_REPEAT,
    /* 0x812D */ CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
    /* 0x812F */ CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    /* 0x8370 */ MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  };

//! \brief Cubic Texture.
enum class CubeMap : GLenum
  {
    /* 0x8515 */ POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    /* 0x8516 */ NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    /* 0x8517 */ POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    /* 0x8518 */ NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    /* 0x8519 */ POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    /* 0x851A */ NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  };

//! \brief Texture Pixel Format.
enum class PixelFormat : GLenum
  {
    /* 0x1901 */ STENCIL_INDEX = GL_STENCIL_INDEX,
    /* 0x1902 */ DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    /* 0x1903 */ RED = GL_RED,
    /* 0x1904 */ GREEN = GL_GREEN,
    /* 0x1905 */ BLUE = GL_BLUE,
    /* 0x1906 */ ALPHA = GL_ALPHA,
    /* 0x1907 */ RGB = GL_RGB,
    /* 0x1908 */ RGBA = GL_RGBA,
    /* 0x1909 */ LUMINANCE = GL_LUMINANCE,
    /* 0x190A */ LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    /* 0x80E0 */ BGR = GL_BGR,
    /* 0x84F9 */ DEPTH_STENCIL = GL_DEPTH_STENCIL,
  };

//! \brief Texture Pixel Type.
enum class PixelType : GLenum
  {
    /* 0x1401 */ UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  };

// **************************************************************
//! GLBuffer
// **************************************************************

enum class BufferUsage : GLenum
  {
    /* 0x88E0 */ STREAM_DRAW = GL_STREAM_DRAW,
    /* 0x88E4 */ STATIC_DRAW = GL_STATIC_DRAW,
    /* 0x88E8 */ DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  };

enum class FrameBuffer : GLenum
  {
    /* 0x1800 */ COLOR = GL_COLOR_BUFFER_BIT,
    /* 0x1801 */ DEPTH = GL_DEPTH_BUFFER_BIT,
    /* 0x1802 */ STENCIL = GL_STENCIL_BUFFER_BIT,
  };

enum class Primitive : GLenum
  {
    /* 0x0000 */ POINTS = GL_POINTS,
    /* 0x0001 */ LINES = GL_LINES,
    /* 0x0002 */ LINE_LOOP = GL_LINE_LOOP,
    /* 0x0003 */ LINE_STRIP = GL_LINE_STRIP,
    /* 0x0004 */ TRIANGLES = GL_TRIANGLES,
    /* 0x0005 */ TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    /* 0x0006 */ TRIANGLE_FAN = GL_TRIANGLE_FAN,
    /* 0x000A */ LINES_ADJACENCY = GL_LINES_ADJACENCY,
    /* 0x000B */ LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    /* 0x000C */ TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    /* 0x000D */ TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    /* 0x000E */ PATCHES = GL_PATCHES,
  };

#endif // OPENGLCPPWRAPPER_GLENUM_HPP
