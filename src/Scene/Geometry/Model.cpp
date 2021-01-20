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

#include "Scene/Geometry/Model.hpp"
#include "Loaders/3D/OBJ.hpp"
//#include "Loaders/3D/MD5mesh.hpp"
#include "Common/NonCppStd.hpp"

static std::string extension(std::string const& path)
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

//--------------------------------------------------------------------------
static std::map<std::string, std::unique_ptr<ShapeLoader>> loaders()
{
    std::map<std::string, std::unique_ptr<ShapeLoader>> mp;

    mp["obj"] = std::make_unique<OBJFileLoader>();
    //mp["md5mesh"] = std::make_unique<MD5MeshFileLoader>();

    return mp;
}

//--------------------------------------------------------------------------
bool Model::generate(GLVertexBuffer<Vector3f>& vertices,
                     GLVertexBuffer<Vector3f>& normals,
                     GLVertexBuffer<Vector2f>& uv,
                     GLIndex32& index)
{
    static std::map<std::string, std::unique_ptr<ShapeLoader>>
            s_loaders = loaders();

    if (config.path.size() == 0u)
    {
        std::cerr << "ERROR: Model::doGenerate: no input file given"
                  << std::endl;
        return false;
    }

    std::string ext = extension(config.path);
    auto loader = s_loaders.find(ext);
    if (loader == s_loaders.end())
    {
        std::cerr << "This file extension '" << ext << "' is not managed"
                  << std::endl;
        return false;
    }
    return loader->second->load(config.path, vertices, normals, uv, index);
}
