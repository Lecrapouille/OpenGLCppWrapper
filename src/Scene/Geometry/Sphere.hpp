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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_SPHERE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_SPHERE_HPP

#  include "Scene/Geometry/Geometry.hpp"

// *****************************************************************************
//! \brief
//! \ingroup Geometry
// *****************************************************************************
class Sphere: public Geometry
{
public:

    //--------------------------------------------------------------------------
    //! \brief Configure the shape.
    //! Call it before calling generate() else default parameters will be used.
    //--------------------------------------------------------------------------
    Sphere& configure(float const radius, uint32_t const slices, uint32_t const stacks);

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

private:

    float m_radius = 1.0f;
    uint32_t m_slices = 32.0f;
    uint32_t m_stacks = 32.0f;
};

#endif
