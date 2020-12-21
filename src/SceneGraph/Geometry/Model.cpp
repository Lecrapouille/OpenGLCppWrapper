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

#include "SceneGraph/Geometry/Model.hpp"
#include "Loaders/3D/OBJ.hpp"

//--------------------------------------------------------------------------
Model& Model::select(std::string const& filename)
{
    m_filename = filename;
    return *this;
}

//--------------------------------------------------------------------------
bool Model::doGenerate(GLVertexBuffer<Vector3f>& vertices,
                       GLVertexBuffer<Vector3f>& normals,
                       GLVertexBuffer<Vector2f>& uv,
                       GLIndex32& index)
{
    if (m_filename.size() == 0u)
    {
        std::cerr << "ERROR: Model::doGenerate: no input file given"
                  << std::endl;
        return false;
    }

    OBJFileLoader loader;
    return loader.load(m_filename, vertices, normals, uv, index);
}
