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
#  include "../debug.hpp"

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
    class MyModel: public Shape<Model, BasicMaterial>
    {
    public:

        MyModel(std::string const& name, Model& model, BasicMaterial& mat)
            : Shape<Model, BasicMaterial>(name, model, mat)
        {
            std::cout << "Create MyModel " << name << std::endl;
        }
    };

    /*
    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    class MySphere: public Shape<Sphere, NormalsMaterial>
    {
    public:

        MySphere(std::string const& name, float const radius, uint32_t const slices,
                 uint32_t const stacks)
            : Shape<Sphere, NormalsMaterial>(name)
        {
            std::cout << "Create MySphere " << name << std::endl;
            geometry.configure(radius, slices, stacks);
            //return Shape<Sphere, NormalsMaterial>::onCreate();
        }

        virtual bool onSetup() override
        {
            std::cout << "MySphere::onSetup()" << std::endl;
            //material.near() = 1.0f;
            //material.far() = 10.0f;
            //material.opacity() = 1.0f;
            return true;
        }
    };
    */

private:

    virtual void onWindowResized() override;
    virtual bool onSetup() override;
    virtual void onSetupFailed(std::string const& reason) override;
    virtual bool onPaint() override;
    virtual void onPaintFailed(std::string const& reason) override;

    SceneTree     m_scene;
    Model         m_model;
    BasicMaterial m_material;
};

#endif // EXAMPLE_01_SG_MATERIALS_AND_SHAPES_HPP
