//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef MATERIAL_HPP
#  define MATERIAL_HPP

#  include "OpenGL/Shaders/Program.hpp"

// *****************************************************************************
//! \brief Interface class for defining the reaction of an object to the light.
// *****************************************************************************
class Material
{
public:

    //--------------------------------------------------------------------------
    //! \brief Give a name to the material. It will be passed to program and
    //! shaders.
    //--------------------------------------------------------------------------
    Material(std::string const& name)
        : program("Prog_" + name),
          m_vert_shader("VS_" + name),
          m_frag_shader("FS_" + name),
          m_name(name)
    {}

    //--------------------------------------------------------------------------
    //! \brief Needed because of pure virtual method.
    //--------------------------------------------------------------------------
    virtual ~Material() = default;

    //--------------------------------------------------------------------------
    //! \brief Generate vertex and fragement shaders (virtual mathod to be
    //! implemented by derived class). Compile shaders. Initialize materials
    //! (uniform glsl variables) with default values.
    //! \return true if shader have been compiled, else return false.
    //--------------------------------------------------------------------------
    bool create()
    {
        createShaders(m_vert_shader, m_frag_shader);
        if (!program.compile(m_vert_shader, m_frag_shader))
        {
            std::cerr << "Failed compiling Material " << name()
                      << ". Reason was '" << program.strerror()
                      << "'" << std::endl;
            return false;
        }

        init();
        return true;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the material name.
    //--------------------------------------------------------------------------
    inline std::string const& name() const
    {
        return m_name;
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Initialize uniform glsl variables with default values.
    //--------------------------------------------------------------------------
    virtual void init() = 0;

    //--------------------------------------------------------------------------
    //! \brief Generate the code for vertex and fragment shaders.
    //--------------------------------------------------------------------------
    virtual void createShaders(GLVertexShader& vert, GLFragmentShader& frag) = 0;

public:

    // FIXME should be static to avoid loosing memory with duplicated things.
    // Or tell the user to make them static but not compatible with Shape<Geometry, Material>
    GLProgram        program;

protected:

    GLVertexShader   m_vert_shader; // FIXME should be static
    GLFragmentShader m_frag_shader; // FIXME should be static
    std::string      m_name; // FIXME should be static
};

#endif
