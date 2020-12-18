#ifndef SCENEGRAPH_HPP
#  define SCENEGRAPH_HPP

#  include "Common/Tree.hpp"
#  include "Math/Transformable.hpp"
#  include "SceneGraph/Object.hpp"
#  include "SceneGraph/Behavior.hpp"

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
class GameObject : public Tree<GameObject>, public Object, public Behavior
{
public:

    GameObject(std::string const& name)
        : Object(name)
    {}

    virtual ~GameObject() = default;

    Matrix44f const& worldTransform() const
    {
        return m_world_transform;
    }

    void update(float const dt)
    {
        traverse([dt](GameObject& node)
        {
            // Derived class may override this function for animating nodes.
            node.onUpdate(dt);

            // Update the matrix transform from the parent matrix.
            node.m_world_transform = node.transform.matrix();
            if (node.parent() != nullptr)
                node.m_world_transform *= node.parent()->m_world_transform;
        });
    }

    void debug()
    {
        traverse([](GameObject& node)
        {
            std::cout << "Node: " << node.name();
            std::cout << " has " << node.children().size()
                      << " children:";
            for (auto const& it: node.children())
                std::cout << " " <<  it->name();
            std::cout << std::endl;
        });
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

#endif
