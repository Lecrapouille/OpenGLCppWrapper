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

#ifndef EXAMPLE_01_SG_MATERIALS_AND_SHAPES_HPP
#  define EXAMPLE_01_SG_MATERIALS_AND_SHAPES_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include "SceneGraph/SceneGraph.hpp"
#  include "SceneGraph/Shape.hpp"
#  include "../debug.hpp"

//#  include "Material/Shape.hpp"
//#  include "Material/Tube.hpp"
//#  include "Material/DepthMaterial.hpp"

// *****************************************************************************
//! \brief This example shows how to apply the depth material to a 3D shape.
// *****************************************************************************
class SGMatAndShape: public GLWindow
{
public:

    SGMatAndShape(uint32_t const width, uint32_t const height, const char *title);
    ~SGMatAndShape();

    static std::string info()
    {
        return "Apply a depth material to a 3D shape";
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    class MyObject: public Shape<Exported, DepthMaterial>
    {
    public:

        using Ptr = std::unique_ptr<MyObject>;

        MyObject(std::string const& name)
            : Shape<Exported, DepthMaterial>(name + std::to_string(id()))
        {}

        virtual bool onCreate() override
        {
            std::cout << "MyObject::onCreate()" << std::endl;
            geometry.select("/home/qq/MyGitHub/OpenGLCppWrapper/examples/textures/qq.obj");
            Shape<Exported, DepthMaterial>::onCreate();
            return true;
        }

        virtual bool onSetup() override
        {
            std::cout << "MyObject::onSetup()" << std::endl;
            material.near() = 1.0f;
            material.far() = 10.0f;
            material.opacity() = 1.0f;
            return true;
        }
    };

private:

    virtual void onWindowResized() override;
    virtual bool onSetup() override;
    virtual void onSetupFailed(std::string const& reason) override;
    virtual bool onPaint() override;
    virtual void onPaintFailed(std::string const& reason) override;

    SceneGraph m_scene;
};

#endif // EXAMPLE_01_SG_MATERIALS_AND_SHAPES_HPP
