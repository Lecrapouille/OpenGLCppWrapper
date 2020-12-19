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

#include "00_SG_Base.hpp"
#include <iostream>

//------------------------------------------------------------------------------
SGBase::SGBase(uint32_t const width, uint32_t const height,
                             const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello SGBase: " << info() << std::endl;
}

//------------------------------------------------------------------------------
SGBase::~SGBase()
{
    std::cout << "Bye SGBase" << std::endl;
}

//------------------------------------------------------------------------------
bool SGBase::onSetup()
{
    //TODO
    // + debug with DearImGUI

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool SGBase::onPaint()
{
    // TODO

    return true;
}

void SGBase::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

void SGBase::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
