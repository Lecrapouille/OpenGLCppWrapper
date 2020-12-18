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

#ifndef SCENEGRAPH_HPP
#  define SCENEGRAPH_HPP

#  include "Math/Transformable.hpp"
#  include "Common/Tree.hpp"
#  include "SceneGraph/GameObject.hpp"

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
class SceneGraph
{
public:

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    class Node : public GameObject, public Tree<Node>
    {
        friend class SceneGraph;

    public:

        //----------------------------------------------------------------------
        //! \brief Create a Node with a name
        //----------------------------------------------------------------------
        Node(std::string const& name)
            : GameObject(name)
        {}

        //----------------------------------------------------------------------
        //! \brief
        //----------------------------------------------------------------------
        inline Matrix44f const& worldTransform() const
        {
            return m_world_transform;
        }

    public:

        //----------------------------------------------------------------------
        //! \brief
        //----------------------------------------------------------------------
        Transformable3D transform;

    private:

        //----------------------------------------------------------------------
        //! \brief
        //----------------------------------------------------------------------
        Matrix44f m_world_transform = Identity44f;
    };

    ~SceneGraph();
    void debug();
    void setup();
    void update(float const dt);
    void draw();
    void release();

public:

    Node::Ptr root = nullptr;
};

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, SceneGraph::Node const& o)
{
    //Matrix44f const& mat = o.transform.matrix();
    return os << "Node "
              << o.name() << " " << o.id();
            //<< o.transform.matrix();
}

#endif
