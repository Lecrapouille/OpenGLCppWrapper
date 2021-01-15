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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP

#  include "Scene/Geometry/Geometry.hpp"

// *****************************************************************************
//! \brief Create a tube shape. Tube is a generic shape that can create Cylinder
//! Cone and Pyramid.
//! \ingroup Geometry
// *****************************************************************************
class Tube: public Geometry
{
public:

    struct Config
    {
        Config() {}
        float top_radius = 1.0f;
        float base_radius = 1.0f;
        float height = 1.0f;
        uint32_t slices = 8.0f;
    };

    //--------------------------------------------------------------------------
    //! \brief Constructor. Z-axis aligned tube centered at origin.
    //! \param[in] m_top_radius The radius at the top of the tube.
    //! \param[in] m_base_radius The radius at the base of the tube.
    //! \param[in] m_height The m_height of the tube.
    //! \param[in] m_slices The number of subdivisions around the Z axis.
    //--------------------------------------------------------------------------
    virtual bool generate(GLVertexBuffer<Vector3f>& vertices,
                          GLVertexBuffer<Vector3f>& normals,
                          GLVertexBuffer<Vector2f>& uv,
                          GLIndex32&        index) override;

public:

    Config config;
};

// *****************************************************************************
//! \brief Class holding meshes of a cylinder. A cylinder is a Tube where top
//! radius is equal to the the base radius.
//! \ingroup Geometry
// *****************************************************************************
class Cylinder: protected Geometry
{
public:

    struct Config
    {
        Config() {}
        float radius = 1.0f;
        float height = 1.0f;
        uint32_t slices = 8.0f;
    };

    //--------------------------------------------------------------------------
    //! \brief Constructor. Z-axis aligned tube centered at origin.
    //! \param[in] m_top_radius The radius at the top of the tube.
    //! \param[in] m_base_radius The radius at the base of the tube.
    //! \param[in] m_height The m_height of the tube.
    //! \param[in] m_slices The number of subdivisions around the Z axis.
    //--------------------------------------------------------------------------
    virtual bool generate(GLVertexBuffer<Vector3f>& vertices,
                          GLVertexBuffer<Vector3f>& normals,
                          GLVertexBuffer<Vector2f>& uv,
                          GLIndex32&        index) override;

public:

    Config config;
};

// *****************************************************************************
//! \brief Class holding meshes of a cone. A cone is a Tube with no top radius.
//! \ingroup Geometry
// *****************************************************************************
class Cone: protected Geometry
{
public:

    struct Config
    {
        Config() {}
        float radius = 1.0f;
        float height = 1.0f;
        uint32_t slices = 8.0f;
    };

    //--------------------------------------------------------------------------
    //! \brief Constructor. Z-axis aligned tube centered at origin.
    //! \param[in] m_top_radius The radius at the top of the tube.
    //! \param[in] m_base_radius The radius at the base of the tube.
    //! \param[in] m_height The m_height of the tube.
    //! \param[in] m_slices The number of subdivisions around the Z axis.
    //--------------------------------------------------------------------------
    virtual bool generate(GLVertexBuffer<Vector3f>& vertices,
                          GLVertexBuffer<Vector3f>& normals,
                          GLVertexBuffer<Vector2f>& uv,
                          GLIndex32&        index) override;

public:

    Config config;
};

// *****************************************************************************
//! \brief Class holding meshes of a pyramid. A pyramid is a cone with 4 slices.
//! \ingroup Geometry
// *****************************************************************************
class Pyramid: protected Geometry
{
public:

    struct Config
    {
        Config() {}
        float radius = 1.0f;
        float height = 1.0f;
    };

    //--------------------------------------------------------------------------
    //! \brief Constructor. Z-axis aligned tube centered at origin.
    //! \param[in] m_top_radius The radius at the top of the tube.
    //! \param[in] m_base_radius The radius at the base of the tube.
    //! \param[in] m_height The m_height of the tube.
    //! \param[in] m_slices The number of subdivisions around the Z axis.
    //--------------------------------------------------------------------------
    virtual bool generate(GLVertexBuffer<Vector3f>& vertices,
                          GLVertexBuffer<Vector3f>& normals,
                          GLVertexBuffer<Vector2f>& uv,
                          GLIndex32&        index) override;

public:

    Config config;
};

#endif // OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP
