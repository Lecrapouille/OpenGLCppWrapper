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

#  include <stddef.h>
#  include <GL/glew.h>

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

//! \brief Texture Pixel Format when stored on CPU.
//! \brief Texture Pixel Format when stored on CPU.
enum class CPUPixelFormat : GLenum
  {
   /* 0x1902 */ DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
   /* 0x1903 */ RED = GL_RED,
   /* 0x1906 */ ALPHA = GL_ALPHA,
   /* 0x1907 */ RGB = GL_RGB,
   /* 0x1908 */ RGBA = GL_RGBA,
   /* 0x1909 */ LUMINANCE = GL_LUMINANCE, // Greyscale
   /* 0x190A */ LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA, // Luminance with alpha
   /* 0x8227 */ RG = GL_RG,
   /* 0x84F9 */ DEPTH_STENCIL = GL_DEPTH_STENCIL,
   /* 0x8D94 */ RED_INTEGER = GL_RED_INTEGER,
   /* 0x8D98 */ RGB_INTEGER = GL_RGB_INTEGER,
   /* 0x8D99 */ RGBA_INTEGER = GL_RGBA_INTEGER,
   /* 0x8D9A */ RG_INTEGER = GL_RG_INTEGER,
  };

//! \brief Texture Pixel Format when stored on GPU.
//! The list is not exhaustive:
//! https://www.khronos.org/registry/OpenGL-Refpages/es3.0/
enum class GPUPixelFormat : GLenum
  {
   /* 0x1906 */ ALPHA = GL_ALPHA,
   /* 0x1907 */ RGB = GL_RGB,
   /* 0x1908 */ RGBA = GL_RGBA,
   /* 0x1909 */ LUMINANCE = GL_LUMINANCE, // Greyscale
   /* 0x190A */ LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA, // Luminance with alpha
   /* 0x8815 */ RGB32F = GL_RGB32F,
   /* 0x8814 */ RGBA32F = GL_RGBA32F,
   /* 0x881A */ RGBA16F = GL_RGBA16F,
   /* 0x8D7C */ RGBA8UI = GL_RGBA8UI,
   /* 0x8D8E */ RGBA8I = GL_RGBA8I,
   /* 0x8D76 */ RGBA16UI = GL_RGBA16UI,
   /* 0x8D88 */ RGBA16I = GL_RGBA16I,
   /* 0x8D82 */ RGBA32I = GL_RGBA32I,
   /* 0x8D70 */ RGBA32UI = GL_RGBA32UI,
   /* 0x81A5 */ DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
   /* 0x81A6 */ DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
   /* 0x8CAC */ DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
   /* 0x88F0 */ DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
   /* 0x8CAD */ DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
  };

//! \brief Save texture as.
enum class PictureFile : GLenum
  {
    TGA, BMP, DDS
  };

// **************************************************************
// FrameBuffer internal pixel format
// **************************************************************
enum class RenderBufferFormat : GLenum
  {
   R8 = GL_R8,
   R8UI = GL_R8UI,
   R8I = GL_R8I,
   R16UI = GL_R16UI,
   R16I = GL_R16I,
   R32UI = GL_R32UI,
   R32I = GL_R32I,
   RG8 = GL_RG8,
   RG8UI = GL_RG8UI,
   RG8I = GL_RG8I,
   RG16UI = GL_RG16UI,
   RG16I = GL_RG16I,
   RG32UI = GL_RG32UI,
   RG32I = GL_RG32I,
   RGB8 = GL_RGB8,
   RGB565 = GL_RGB565,
   RGBA8 = GL_RGBA8,
   SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
   RGB5_A1 = GL_RGB5_A1,
   RGBA4 = GL_RGBA4,
   RGB10_A2 = GL_RGB10_A2,
   RGBA8UI = GL_RGBA8UI,
   RGBA8I = GL_RGBA8I,
   RGB10_A2UI = GL_RGB10_A2UI,
   RGBA16UI = GL_RGBA16UI,
   RGBA16I = GL_RGBA16I,
   RGBA32I = GL_RGBA32I,
   RGBA32UI = GL_RGBA32UI,
   DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
   DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
   DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
   DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
   DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
   STENCIL_INDEX8 = GL_STENCIL_INDEX8,
  };

// **************************************************************
// GLBuffer
// **************************************************************

//! \brief Specifies the expected usage pattern of the data store for
//! glBufferData.
enum class BufferUsage : GLenum
  {
    /* 0x88E0 */ STREAM_DRAW = GL_STREAM_DRAW,
    /* 0x88E4 */ STATIC_DRAW = GL_STATIC_DRAW,
    /* 0x88E8 */ DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  };

//! \brief Type of buffer used by frame buffers.
enum class FrameBuffer : GLenum
  {
    /* 0x1800 */ COLOR = GL_COLOR_BUFFER_BIT,
    /* 0x1801 */ DEPTH = GL_DEPTH_BUFFER_BIT,
    /* 0x1802 */ STENCIL = GL_STENCIL_BUFFER_BIT,
  };

//! \brief Mode for drawing primitives (points, lines, triangles ...)
enum class Mode : GLenum
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
