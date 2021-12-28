//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

#  include "Loaders/TextureLoader.hpp"
#  include "SOIL/SOIL.h"
#  include <iostream>

// *****************************************************************************
//! \brief Class wrapping SOIL library for loading and saving textures
// *****************************************************************************
class SOIL: public TextureLoader
{
public:

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual bool setPixelFormat(GLTexture::PixelFormat const cpuformat) override;

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual GLenum getPixelType() const override
    {
        return m_pixelType;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual size_t getPixelCount() const override
    {
        return m_pixelCount;
    }

    //--------------------------------------------------------------------------
    //! \brief Load picture file (jpg, png ...)
    //! \param[in] filename the path of the picture file.
    //! \param[inout] buffer the pending container holding the texture (CPU side)
    //! \param[out] width return the texture width (in pixels)
    //! \param[out] height return the texture height (in pixel)
    //! \return true if the loading ends with success, else return false.
    //--------------------------------------------------------------------------
    virtual bool load(std::string const&, GLTexture::Buffer& buffer,
                      size_t& width, size_t& height) override;


    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual bool save(std::string const&, GLTexture::Buffer const& texture_buffer,
                      size_t const width, size_t const height) override;

private:

    size_t m_pixelCount;
    int    m_soilFormat;
    GLenum m_pixelType = GL_UNSIGNED_BYTE; // Only managed by SOIL
    bool   m_isValid;
};

#endif // OPENGLCPPWRAPPER_SOIL_TEXTURES_LOADER_HPP
