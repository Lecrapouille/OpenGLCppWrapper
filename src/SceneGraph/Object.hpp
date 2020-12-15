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

#ifndef OBJECT_HPP
#  define OBJECT_HPP

#  include "Common/ClassCounter.hpp"
#  include "Common/Components.hpp"

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
class Object : private UniqueID<Object>, private InstanceCounter<Object>
{
public:

    Object(std::string const& name /* TODO: components*/)
        : m_id(UniqueID<Object>::getID()),
          m_name(name)
    {}

    std::string const& name() const
    {
        return m_name;
    }

    Key id() const
    {
        return m_id;
    }

    Key instanceCount() const
    {
        return InstanceCounter<Object>::count();
    }

    void enable(bool const v = true)
    {
        m_enabled = v;
    }

    bool enabled() const
    {
        return m_enabled;
    }

public:

    Components components;

private:

    Key m_id;
    std::string m_name;
    bool m_enabled = true;
};


#endif
