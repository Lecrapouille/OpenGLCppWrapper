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

#ifndef GEOMETRY_MODEL_HPP
#  define GEOMETRY_MODEL_HPP

#  include "SceneGraph/Geometry/Geometry.hpp"

// *****************************************************************************
//! \brief Generate the geometry from a file
// *****************************************************************************
class Model: public Geometry
{
public:

    //--------------------------------------------------------------------------
    //! \brief Select the loader
    //--------------------------------------------------------------------------
    Model& select(std::string const& filename);

private:

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual bool doGenerate(GLVertexBuffer<Vector3f>& vertices,
                            GLVertexBuffer<Vector3f>& normals,
                            GLVertexBuffer<Vector2f>& uv,
                            GLIndex32& index) override;

private:

    std::string m_filename;
};

#endif // GEOMETRY_MODEL_HPP
