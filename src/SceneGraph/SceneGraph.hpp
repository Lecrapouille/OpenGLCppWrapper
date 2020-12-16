#ifndef SCENEGRAPH_HPP
#  define SCENEGRAPH_HPP

#  include "Common/Tree.hpp"
#  include "Math/Transformable.hpp"
#  include "SceneGraph/Object.hpp"
#  include "SceneGraph/Behavior.hpp"

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
class GameObject : public Object, public Behavior
{
    friend class SceneGraph;

public:

    GameObject(std::string const& name)
        : Object(name)
    {}

    Matrix44f const& worldTransform() const
    {
        return m_world_transform;
    }

public:

    Transformable3D transform;

private:

    Matrix44f m_world_transform = Identity44f;
};

std::ostream& operator<<(std::ostream& os, GameObject const& o)
{
    //Matrix44f const& mat = o.transform.matrix();
    return os << "GameObject "
              << o.name() << " " << o.id();
            //<< o.transform.matrix();
}

class SceneGraph: public Tree<GameObject>
{
public:

    /*GameObject& attach(GameObject& parent, std::string const& name)
    {
        SceneGraph::Node& node = insert(parent, name);
        node.data.node = &node;
        node.data.onSetup();
        return obj;
        }*/

    void update(float const dt)
    {
        traverse([dt](Node& node)
        {
            // Derived class may override this function for animating nodes.
            node.data.onUpdate(dt);

            // Update the matrix transform from the parent matrix.
            node.data.m_world_transform = node.data.transform.matrix();
            if (node.parent != nullptr)
                node.data.m_world_transform *= node.parent->data.m_world_transform;
        });
    }
};

#endif
