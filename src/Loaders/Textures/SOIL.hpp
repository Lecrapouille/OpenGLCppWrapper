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

#  include "Loaders/Textures.hpp"
#  include "external/SOIL/SOIL.h"
#  include <iostream>

// *****************************************************************************
//! \brief Class wrapping SOIL library for loading and saving textures
// *****************************************************************************
class SOIL: public TextureLoader
{
public:

    SOIL(GLTexture::PixelFormat const cpuformat)
    {
        m_isvalid = setPixelFormat(cpuformat);
    }

    virtual bool setPixelFormat(GLTexture::PixelFormat const cpuformat) override
    {
        switch (cpuformat)
        {
        case GLTexture::PixelFormat::RGBA:
            m_soilformat = SOIL_LOAD_RGBA;
            m_nbpixel = 4_z;
            return true;
        case GLTexture::PixelFormat::RGB:
            m_soilformat = SOIL_LOAD_RGB;
            m_nbpixel = 3_z;
            return true;
        case GLTexture::PixelFormat::LUMINANCE:
            std::cerr << "LUMINANCE_ALPHA not managed" << std::endl;
            m_soilformat = SOIL_LOAD_L;
            return false;
        case GLTexture::PixelFormat::LUMINANCE_ALPHA:
            std::cerr << "LUMINANCE_ALPHA not yet managed" << std::endl;
            m_soilformat = SOIL_LOAD_LA;
            return false;
        case GLTexture::PixelFormat::BGR:
        case GLTexture::PixelFormat::STENCIL_INDEX:
        case GLTexture::PixelFormat::DEPTH_COMPONENT:
        case GLTexture::PixelFormat::RED:
        case GLTexture::PixelFormat::GREEN:
        case GLTexture::PixelFormat::BLUE:
        case GLTexture::PixelFormat::ALPHA:
        case GLTexture::PixelFormat::DEPTH_STENCIL:
        default:
            std::cerr << "SOIL does not surport the given pixel format"
                      << std::endl;
            m_soilformat = SOIL_LOAD_AUTO;
            return false;
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Load picture file (jpg, png ...)
    //! \param[in] filename the path of the picture file.
    //! \param[inout] buffer the pending container holding the texture (CPU side)
    //! \param[out] width return the texture width (in pixels)
    //! \param[out] height return the texture height (in pixel)
    //! \return true if the loading ends with success, else return false.
    //--------------------------------------------------------------------------
    virtual bool load(const char *const filename, GLTexture::Buffer& buffer,
                      size_t& width, size_t& height) override
    {
        if (unlikely(nullptr == filename))
            return false;
        if (unlikely(!m_isvalid))
            return false;

        // Load the image as a C array.
        int w, h;
        unsigned char* image = SOIL_load_image(filename, &w, &h, 0,
                                               static_cast<int>(m_soilformat));
        if (likely(nullptr != image))
        {
            // Use the max because with framebuffer we can resize texture
            width = std::max(width, static_cast<size_t>(w)); // FIXME
            height = std::max(height, static_cast<size_t>(h));

            // Convert it as std::vector
            size_t size = static_cast<size_t>(w * h) * m_nbpixel * sizeof(unsigned char);
            buffer.append(image, size); // FIXME: not working with preallocated size
            SOIL_free_image_data(image);
            return true;
        }
        else
        {
            std::cerr << "Failed loading picture file '"
                      << filename << "'. Reason was '"
                      << SOIL_last_result()
                      << "'" << std::endl;
            width = height = 0;
            buffer.clear();
            return false;
        }
    }

    virtual bool save(const char *const filename, GLTexture::Buffer const& texture_buffer,
                      size_t const width, size_t const height) override
    {
        const unsigned char* buffer = texture_buffer.to_array();
        if (unlikely(nullptr == buffer))
        {
            std::cerr << "Cannot save a texture with no buffer"
                      << std::endl;
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
            std::cerr << "Cannot save a texture into the given file format '"
                      << ext << std::endl;
            return false;
        }

        bool res = !!SOIL_save_image(filename, m_soilformat,
                                     static_cast<int>(width),
                                     static_cast<int>(height),
                                     static_cast<int>(m_nbpixel), buffer);
        if (unlikely(!res))
        {
            std::cerr << "Failed saving the texture in the file '"
                      << filename << "'. Reason was '"
                      << SOIL_last_result()
                      << "'" << std::endl;
        }
        return res;
    }

private:

    size_t m_nbpixel;
    int    m_soilformat;
    bool   m_isvalid;
};

#endif // OPENGLCPPWRAPPER_SOIL_TEXTURES_LOADER_HPP
