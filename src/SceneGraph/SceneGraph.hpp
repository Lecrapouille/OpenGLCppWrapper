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

// *****************************************************************************
//! \brief Class container fo holding a scene.
//!
//! A scene is a spatial representation of a graphical scene using a tree
//! structure. Instead of storing 3D models (entities) in an array, where
//! their positions are related to the world origin, making more difficult to
//! move these entites inside the world, a scene classifies entities inside a
//! tree structure where parent-children relations between nodes allow to
//! define relative transformations of their positions given by a transform
//! matrix (translation, rotation and scaling). This way, the animation of
//! entities are easier because modifying the matrix of one done will
//! automatically impact the position of the descendant nodes. Computed
//! matrices are usually passed to the GLSL shaders through a 4x4 uniform
//! matrix with the name of "model matrix".
//!
//! For more information you can read this document "Scene Graphs" at
//!   https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/
// *****************************************************************************
class SceneGraph
{
public:

    //--------------------------------------------------------------------------
    //! \brief Extend the Tree node class by adding data (transform matrices ...)
    //! while keep knowing the parent node and child nodes.
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
        //! \brief Return the const reference of the world transformation
        //! matrix.
        //----------------------------------------------------------------------
        inline Matrix44f const& worldTransform() const
        {
            return m_world_transform;
        }

    public:

        //! \brief Relative transformation to parent node. This allow the
        //! current node to have local movement.
        Transformable3D transform;

    private:

        //! \brief The matrix transform to apply on the child. Transformations
        //! are relative to the parent node.
        Matrix44f m_world_transform = Identity44f;
    };

    //--------------------------------------------------------------------------
    //! \brief Find an element by its name. If the name uses '/'
    //--------------------------------------------------------------------------
    Node* get(std::string const& name);

    void getByTag(std::string const& name, std::vector<Node*> found);

    //--------------------------------------------------------------------------
    //! \brief Release all node attached to this scene. Each node before its
    //! \brief destruction has its method onDisable() called.
    //--------------------------------------------------------------------------
    ~SceneGraph();

    //--------------------------------------------------------------------------
    //! \brief Traverse the scene and print information on each node to the
    //! console.
    //--------------------------------------------------------------------------
    void debug();

    //--------------------------------------------------------------------------
    //! \brief Traverse the scene and call onSetup() on each node.
    //--------------------------------------------------------------------------
    void setup();

    //--------------------------------------------------------------------------
    //! \brief Traverse the scene and call onUpdate() on each node.
    //! \param[in] dt: delta time from the previous frame.
    //--------------------------------------------------------------------------
    void update(float const dt);

    //--------------------------------------------------------------------------
    //! \brief Traverse the scene and call the draw() method.
    //--------------------------------------------------------------------------
    void draw();

    //--------------------------------------------------------------------------
    //! \brief Traverse the scene and call onDisable() on each node. Then all
    //! nodes are destroyed. The root node is set to nullptr.
    //--------------------------------------------------------------------------
    void release();

private:

    Node* findChild(Node* node, std::string const& path);

public:

    // Root of the scene
    Node::Ptr root = nullptr;

    // TODO
    // OverrideMaterial
    // active camera
    // fog
};

//------------------------------------------------------------------------------
//! \brief Alias name
//------------------------------------------------------------------------------
using SceneObject = SceneGraph::Node;

//------------------------------------------------------------------------------
//! \brief Print on the console internal states of a node.
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, SceneObject& node)
{
    return os << "Node: " << node.name() << std::endl
              << " transform: " << node.transform.matrix() << std::endl
              << " enabled: " << node.enabled() << std::endl
              << " has " << node.children.size()
              << " children:" << std::endl << "  ";
    for (auto const& it: node.children)
        std::cout << " " <<  it->name();
}

#endif
