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

#ifndef OPENGLCPPWRAPPER_SOIL_TEXTURES_LOADER_HPP
#  define OPENGLCPPWRAPPER_SOIL_TEXTURES_LOADER_HPP

#  include "external/SOIL/SOIL.h"

namespace glwrap
{

// *****************************************************************************
//! \brief Class wrapping SOIL library for loading and saving textures
// *****************************************************************************
class SOIL
{
public:

  SOIL(CPUPixelFormat const cpuformat)
  {
    switch (cpuformat)
      {
      case CPUPixelFormat::RGBA:
        m_soilformat = SOIL_LOAD_RGBA;
        m_nbpixel = 4_z;
        m_isvalid = true;
        break;
      case CPUPixelFormat::RGB:
        m_soilformat = SOIL_LOAD_RGB;
        m_nbpixel = 3_z;
        m_isvalid = true;
        break;
      case CPUPixelFormat::LUMINANCE:
        ERROR("LUMINANCE_ALPHA not yet managed");
        m_soilformat = SOIL_LOAD_L;
        m_isvalid = false;
        break;
      case CPUPixelFormat::LUMINANCE_ALPHA:
        ERROR("LUMINANCE_ALPHA not yet managed");
        m_soilformat = SOIL_LOAD_LA;
        m_isvalid = false;
        break;
      case CPUPixelFormat::DEPTH_COMPONENT:
      case CPUPixelFormat::RED:
      case CPUPixelFormat::ALPHA:
      case CPUPixelFormat::RG:
      case CPUPixelFormat::DEPTH_STENCIL:
      case CPUPixelFormat::RED_INTEGER:
      case CPUPixelFormat::RGB_INTEGER:
      case CPUPixelFormat::RGBA_INTEGER:
      case CPUPixelFormat::RG_INTEGER:
        ERROR("SOIL does not surport the given pixel format");
        m_soilformat = SOIL_LOAD_AUTO;
        m_isvalid = false;
        break;
      }
  }

  ~SOIL()
  {}

  bool getPixelInfo(GLenum& type, size_t& count)
  {
    type = GL_UNSIGNED_BYTE; // SOIL does not manage other type
    count = m_nbpixel;
    return m_isvalid;
  }

  //----------------------------------------------------------------------------
  //! \brief Load picture file (jpg, png ...)
  //! \param[in] filename the path of the picture file.
  //! \param[inout] data the pending container holding the texture (CPU side)
  //! \param[out] width return the texture width (in pixels)
  //! \param[out] height return the texture height (in pixel)
  //! \return true if the loading ends with success, else return false.
  //----------------------------------------------------------------------------
  bool load(const char *const filename, TextureData& data,
            uint32_t& width, uint32_t& height)
  {
    DEBUG("Loading texture '%s'", filename);

    if (unlikely(nullptr == filename))
      return false;
    if (unlikely(!m_isvalid))
      return false;

    // Load the image as a C array.
    int w, h;
    unsigned char* image = SOIL_load_image(filename, &w, &h, 0, m_soilformat);
    if (likely(nullptr != image))
      {
        // Use the max because with framebuffer we can resize texture
        width = std::max(width, static_cast<uint32_t>(w)); // FIXME
        height = std::max(height, static_cast<uint32_t>(h));

        // Convert it as std::vector
        size_t size = static_cast<size_t>(w * h) * m_nbpixel * sizeof(unsigned char);
        data.append(image, size); // FIXME: not working with preallocated size
        SOIL_free_image_data(image);
        DEBUG("Successfully loaded %ux%u texture '%s'", width, height, filename);
        return true;
      }
    else
      {
        ERROR("Failed loading picture file '%s'. Reason was '%s'",
              filename, SOIL_last_result());
        width = height = 0;
        data.clear();
        return false;
      }
  }

  bool save(const char *const filename, TextureData const& data,
            uint32_t const width, uint32_t const height)
  {
    const unsigned char* buffer = data.to_array();
    if (unlikely(nullptr == buffer))
      {
        ERROR("Cannot save a texture with no data");
        return false;
      }

    std::string ext = getExtension(filename);
    if (ext == "bmp")
      m_soilformat = SOIL_SAVE_TYPE_BMP;
    else if (ext == "tga")
      m_soilformat = SOIL_SAVE_TYPE_TGA;
    else if (ext == "dds")
      m_soilformat = SOIL_SAVE_TYPE_DDS;
    else
      {
        ERROR("Cannot save a texture into the given file format '%s'",
              ext.c_str());
        return false;
      }

    bool res = !!SOIL_save_image(filename, m_soilformat,
                                 static_cast<int>(width),
                                 static_cast<int>(height),
                                 static_cast<int>(m_nbpixel), buffer);
    if (unlikely(!res))
      ERROR("Failed saving the texture in the file '%s'. Reason was '%s'",
            filename, SOIL_last_result());

    return res;
  }

private:

  std::string getExtension(std::string const& path)
  {
    std::string::size_type pos = path.find_last_of(".");
    if (pos != std::string::npos)
      {
        std::string ext = path.substr(pos + 1, std::string::npos);

        // Ignore the ~ in the extension (ie. foo.txt~)
        if ('~' == ext.back())
          ext.pop_back();

        // Get the file extension in lower case
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
      }
    return "";
  }

private:

  size_t m_nbpixel;
  int    m_soilformat;
  bool   m_isvalid;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_SOIL_TEXTURES_LOADER_HPP
