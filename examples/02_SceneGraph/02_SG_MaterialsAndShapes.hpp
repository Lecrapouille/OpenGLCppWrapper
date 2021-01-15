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

#ifndef EXAMPLE_02_SG_MATERIALS_AND_SHAPES_HPP
#  define EXAMPLE_02_SG_MATERIALS_AND_SHAPES_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include "../debug.hpp"

// *****************************************************************************
//! \brief This example shows how to apply the depth material to a 3D shape.
// *****************************************************************************
class SGMatAndShape: public GLWindow
{
public:

    class MyShape: public Shape<Model, BasicMaterial>
    {
    public:

        MyShape(std::string const& name, std::string const& path)
            : Shape<Model, BasicMaterial>(name)
        {
            std::cout << "Create MyShape " << name << std::endl;

            Shape<Model, BasicMaterial>::geometry.configure(path);
            Shape<Model, BasicMaterial>::material.diffuse() = Color(1.0f, 0.0f, 0.0f).toVector3f();
            if (!Shape<Model, BasicMaterial>::compile())
            {
                throw GL::Exception("Failed create renderable");
            }
        }

        inline std::string const& name() const
        {
            return Shape<Model, BasicMaterial>::name();
        }
    };

    SGMatAndShape(uint32_t const width, uint32_t const height, const char *title);
    ~SGMatAndShape();

    static std::string info()
    {
        return "Apply a depth material to a 3D shape";
    }

private:

    virtual void onWindowResized() override;
    virtual bool onSetup() override;
    virtual void onSetupFailed(std::string const& reason) override;
    virtual bool onPaint() override;
    virtual void onPaintFailed(std::string const& reason) override;

    SceneTree     m_scene;
};

#endif // EXAMPLE_02_SG_MATERIALS_AND_SHAPES_HPP
