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

#ifndef EXAMPLE_00_WINDOW_00_BASIC_WINDOW_HPP
#  define EXAMPLE_00_WINDOW_00_BASIC_WINDOW_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

//------------------------------------------------------------------------------
//! \brief This example shows how to create a basic window instanciating an
//! OpenGL context. No OpenGL objects are rendered here for the moment. The
//! single OpenGL code called here is to clear the window ith a different color
//! depending the time. No input/ouput events are observed for this basic
//! example.
//------------------------------------------------------------------------------
class BasicWindow: public GLWindow
{
public:

    BasicWindow(uint32_t const width, uint32_t const height, const char *title);
    ~BasicWindow();

    static std::string info()
    {
        return "Dummy window clearing background color over time";
    }

private:

    virtual bool onSetup() override;
    virtual bool onPaint() override;
    virtual void onSetupFailed(std::string const& reason) override;
    virtual void onPaintFailed(std::string const& reason) override;
};

#endif // EXAMPLE_00_WINDOW_00_BASIC_WINDOW_HPP
