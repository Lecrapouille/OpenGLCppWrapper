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
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_GLFONT_HPP
#  define OPENGLCPPWRAPPER_GLFONT_HPP

#  include "IGLObject.hpp"
#  include <ft2build.h>
#  include FT_FREETYPE_H
#  include <map>

// *****************************************************************************
//! \file GLFont.hpp Define fonts for OpenGL
// *****************************************************************************

namespace glwrap
{

// *****************************************************************************
//! \brief FreeType Font Wrapper.
//!
//! Wrap FreeType routines.
// *****************************************************************************
class FTFont
{
public:

  FTFont()
  {
    // Note: lazy management of failure
    m_error = FT_Init_FreeType(&m_library);
  }

  FTFont(const char* const filename)
    : FTFont()
  {
    load(filename);
  }

  ~FTFont()
  {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
  }

  bool load(const char* const filename)
  {
    if (unlikely(m_error))
      {
        ERROR("Could not init FreeType Library");
        return false;
      }

    //TODO: FT_Done_Face(m_face);

    m_error = FT_New_Face(m_library, filename, 0, &m_face);
    if (unlikely(FT_Err_Unknown_File_Format == m_error))
      {
        ERROR("Failed to load ttf font '%s'", filename);
        return false;
      }
    else if (unlikely(m_error))
      {
        ERROR("Failed to open ttf font '%s'", filename);
        return false;
      }
    return true;
  }

  inline bool isLoaded() const
  {
    return FT_Err_Ok == m_error;
  }

  //! \note call isLoaded() before calling this method to be sure to get a valid
  //! FT_Face.
  inline FT_Face face()
  {
    return m_face;
  }

private:

  FT_Library m_library;
  FT_Face    m_face;
  FT_Error   m_error = 1;
};

// *****************************************************************************
//! \brief Define an OpenGL atlas texture holding FreeType fonts.
// *****************************************************************************
class FreeTypeFontAtlas
  : public IGLObject<GLenum>
{
public:

  struct Character
  {
    float advanceX;
    float advanceY;

    float bitmapWidth;
    float bitmapHeight;

    float bitmapLeft;
    float bitmapTop;

    float xOffset;
  };

  FreeTypeFontAtlas(std::string const& name)
    : IGLObject(name)
  {
    m_target = GL_TEXTURE_2D;
  }

  inline bool loaded() const
  {
    return ((0 != m_width) && (0 != m_height));
  }

  bool load(FT_Face face, uint32_t const pixelSize)
  {
    m_width = m_height = 0;
    m_face = face;

    FT_GlyphSlot slot = m_face->glyph;
    FT_Set_Pixel_Sizes(m_face, 0, pixelSize);

    // Main char set (0 - 128)
    for (FT_ULong i = 0; i < 128; ++i)
      {
        if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
          {
            ERROR("Loading character ttf %lu failed!", i);
            continue; // try next character
          }

        // Add the width of this glyph to our texture width.
        // Note: We add 2 pixels of blank space between glyphs for padding this
        // helps reduce texture bleeding that can occur with antialiasing.
        m_width += slot->bitmap.width + 2;
        m_height = std::max(m_height, slot->bitmap.rows);

        // Store glyph info in our char array for this pixel size
        m_chars[i].advanceX = static_cast<float>(slot->advance.x >> 6);
        m_chars[i].advanceY = static_cast<float>(slot->advance.y >> 6);

        m_chars[i].bitmapWidth = static_cast<float>(slot->bitmap.width);
        m_chars[i].bitmapHeight = static_cast<float>(slot->bitmap.rows);

        m_chars[i].bitmapLeft = static_cast<float>(slot->bitmap_left);
        m_chars[i].bitmapTop = static_cast<float>(slot->bitmap_top);
      }

    float texPos = 0.0f; // texture offset
    for (int i = 0; i < 128; ++i)
      {
        m_chars[i].xOffset = texPos / static_cast<float>(m_width);
        texPos += static_cast<float>(slot->bitmap.width) + 2.0f;
      }

    return true; // FIXME
  }

  //inline GLuint getTexId() { return _tex; }
  inline uint32_t width() { return m_width; }
  inline uint32_t height() { return m_height; }
  inline const Character* character() const { return m_chars; }

private:

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL texture.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the Texture to OpenGL.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
  }

  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", cname());

    // Note: m_buffer can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", cname());
        return true;
      }

    applyTextureParam();
    specifyTexture();
    populateAtlas();

    return false;
  }

  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the Texture to OpenGL.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL Texture.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
    m_width = m_height = 0;
  }

  inline void applyTextureParam() const
  {
    glCheck(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  }

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a two-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture() const
  {
    glCheck(glTexImage2D(m_target, 0, GL_RED,
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0, GL_RED, GL_UNSIGNED_BYTE, nullptr));
  }

  void populateAtlas() const
  {
    GLint texPos = 0; // texture offset
    FT_GlyphSlot slot = m_face->glyph;

    for (FT_ULong i = 0; i < 128; ++i)
      {
        if (FT_Load_Char(m_face, i, FT_LOAD_RENDER))
          continue;

        // Add this character glyph to our texture
        glTexSubImage2D(m_target, 0, texPos, 0, 1, static_cast<GLsizei>(slot->bitmap.rows), GL_RED, GL_UNSIGNED_BYTE, nullptr);
        glTexSubImage2D(m_target, 0, texPos, 0, static_cast<GLsizei>(slot->bitmap.width), static_cast<GLsizei>(slot->bitmap.rows), GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
        glTexSubImage2D(m_target, 0, texPos, 0, 1, static_cast<GLsizei>(slot->bitmap.rows), GL_RED, GL_UNSIGNED_BYTE, nullptr);
        texPos += static_cast<GLint>(slot->bitmap.width + 2);
      }
  }

private:

  Character    m_chars[128];
  FT_Face      m_face;
  uint32_t     m_width = 0;
  uint32_t     m_height = 0;
};

// *****************************************************************************
//! \brief Class using a TrueType font and drawing strings.
// *****************************************************************************
class GLString
{
public:

  GLString(const char* const fontFile) // FIXME a placer ailleurs
    : m_font(fontFile),
      m_vertex_shader("fs_TTF"),
      m_fragment_shader("fs_TTF"),
      m_prog("prog_TTF")
  {
    // Create default font size
    setFontSize(m_font_size);

    m_vertex_shader.fromString(R"GLSL(
      #version 330 core
      in vec4 vertex;
      out vec2 TexCoords;
      uniform mat4 projection;

      void main() {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
      })GLSL");

    m_fragment_shader.fromString(R"GLSL(
      #version 330 core
      in vec2 TexCoords;
      out vec4 color;
      uniform sampler2D tex;
      uniform vec3 textColor;

      void main() {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
      })GLSL");

    if (m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
      {
        //m_prog.matrix44f("projection") = ;
        //m_prog.vector3f("textColor") = Vector3f(1.0f, 0.0f, 0.0f);
        //m_prog.texture2D("tex").load();
        //m_prog.bind(m_vao);
      }
    else
      {
        ERROR("Failed compiling OpenGL program. Reason was '%s'", m_prog.getError().c_str());
      }

    // Unbind current program by security
    //m_prog.end();
  }

  void setColor(float const r, float const b, float const g)
  {
    m_prog.vector3f("textColor") = Vector3f({r, g, b});
  }

  void setColor(Vector3f const& color)
  {
    m_prog.vector3f("textColor") = color;
  }

  void setFontSize(uint16_t const size)
  {
    m_font_size = size;
    auto it = m_font_atlas.find(size);
    if (it != m_font_atlas.end())
      {
        m_font_atlas.insert(new FreeTypeFontAtlas(m_font.face(), size));
      }
  }

  void print(std::string const& text, float const x, float const y)
  {
    /*if (m_text != text)
      {
        m_text = text;
        recalculateVertices(m_text.c_str(), x, y);
        m_font_atlas[m_font_size].draw();
        }*/
  }

private:

  void recalculateVertices(const char* const text, float const x, float const y)
  {
  }

private:

  FTFont                            m_font;
  GLVertexShader                    m_vertex_shader; // TODO local variables
  GLFragmentShader                  m_fragment_shader;
  GLProgram                         m_prog;
  std::map<int, FreeTypeFontAtlas*> m_font_atlas;
  std::string                       m_text;
  uint16_t                          m_font_size = 48u;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLFONT_HPP
