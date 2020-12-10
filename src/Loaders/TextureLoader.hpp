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

#ifndef TEXTURES_LOADER_HPP
#  define TEXTURES_LOADER_HPP

#  include "OpenGL/Textures/Texture.hpp"
//#  include <algorithm>

class TextureLoader
{
public:

    virtual bool setPixelFormat(GLTexture::PixelFormat const cpuformat) = 0;

    virtual bool load(std::string const& filename, GLTexture::Buffer& buffer,
                      size_t& width, size_t& height) = 0;

    virtual bool save(std::string const& filename, GLTexture::Buffer const& buffer,
                      size_t const width, size_t const height) = 0;

    virtual GLenum getPixelType() const = 0;

    virtual size_t getPixelCount() const = 0;

    static std::string getExtension(std::string const& path)
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
};

#endif