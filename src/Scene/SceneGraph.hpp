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

#ifndef OPENGLCPPWRAPPER_SCENEGRAPH_HPP
#  define OPENGLCPPWRAPPER_SCENEGRAPH_HPP

#  include "Scene/Transformable.hpp"
#  include <iostream>

namespace glwrap
{
  DECLARE_CLASS(Node3D)

  class Node3D
    : public Transformable3D
  {
  public:

    Node3D(std::string const& name)
      : m_name(name)
    {
      DEBUG("Create Node3D named '%s'", name.c_str());
    }

    Node3D(const char* name)
      : m_name(name)
    {
      DEBUG("Create Node3D named '%s'", name);
    }

    virtual ~Node3D()
    {
      m_children.clear();
    }

    static std::shared_ptr<Node3D> create(std::string const& name)
    {
      return std::make_shared<Node3D>(name);
    }

    static std::shared_ptr<Node3D> create(const char* name)
    {
      return std::make_shared<Node3D>(name);
    }

    std::string const& name() const
    {
      return m_name;
    }

    virtual void update(float const dt)
    {
      m_world_transform = Transformable3D::transform();
      if (unlikely(nullptr != m_parent))
        {
          m_world_transform *= m_parent->m_world_transform;
        }

      for (auto& i: m_children)
        {
          i->update(dt);
        }
    }

    virtual bool renderable() const { return false; }

    virtual void renderer(/* RendererState */)
    {
      if (unlikely(!isTraversable()))
        return;

      for (auto& c: m_children)
        {
          c->renderer();
        }
    }

    void debug() const
    {
      std::cout << "Scene Node '" << m_name << "':" << std::endl;
      if (hasParent())
        std::cout << "  Parent '" << static_cast<Node3D*>(m_parent)->name() << std::endl;
      else
        std::cout << "  is root" << std::endl;
      std::cout << "  " << m_children.size() << " Child:";
      for (auto it: m_children)
        {
          std::cout << " " << static_cast<Node3D*>(it.get())->name();
        }
      std::cout << std::endl;
    }

    Matrix44f const& localTransform()
    {
      return Transformable3D::transform();
    }

    Matrix44f const& worldTransform() const
    {
      return m_world_transform;
    }

    Matrix44f& worldTransform()
    {
      return m_world_transform;
    }

    inline bool isTraversable() const
    {
      return m_traversable;
    }

    inline void setTraversable(bool const traversable = true)
    {
      m_traversable = traversable;
    }

    Node3D& add(Node3D_SP node)
    {
      if (likely(this == node.get())) {
        ERROR("Cannot attach a node to itself");
        return *this;
      }

      if (unlikely(node->m_attached)) {
        ERROR("Node already attached");
        return *this;
      }

      node->m_parent = this;
      m_children.push_back(node);
      return *this;
    }

    Node3D& remove(Node3D_SP node)
    {
      auto it = std::find_if(m_children.begin(), m_children.end(),
        [node](Node3D_SP i){ return i.get() == node.get(); });

      if (it != m_children.end())
        m_children.erase(it);

      return *this;
    }

    template<typename Functor>
    static Node3D_SP find(Node3D_SP root, Functor predicate, bool const forceTraverse)
    {
      //if ((!forceTraverse) && (unlikely(!root->isTraversable())))
      //  return m_children.end();

      if (predicate(root))
        return root;

      for (auto& c: root->m_children)
        {
          return find(c, predicate, forceTraverse);
        }

      return nullptr;
    }

    // TODO get("/node1/subnode/etc");
    static Node3D_SP get(Node3D_SP root, std::string const& name, bool const forceTraverse = false)
    {
      return find(root, [name](Node3D_SP i){ return name == i->name(); }, forceTraverse);
    }

    template<typename Functor>
    Node3D_SP findSibling(Functor predicate)
    {
      auto it = std::find_if(m_children.begin(), m_children.end(), predicate);
      return (it != m_children.end()) ? *it : nullptr;
    }

    Node3D_SP getSibling(std::string const& name)
    {
      return findSibling([name](Node3D_SP i){ return name == i->name(); });
      //auto it = std::find_if(m_children.begin(), m_children.end(),
      //   [name](Node3D_SP i){ return name == i->name(); });
      //return (it != m_children.end()) ? *it : nullptr;
    }

    template<typename Functor>
    void traverse(Functor functor)
    {
      if (unlikely(!isTraversable()))
        return;

      functor(/**this*/);
      for (auto& c: m_children)
        {
          c->traverse(functor);
        }
    }

    template<typename Functor>
    void traverseAncestors(Functor functor)
    {
      if (likely(hasParent()))
        {
          functor(m_parent);
          m_parent->traverseAncestors(functor);
        }
    }

    inline bool hasChildren() const
    {
      return m_children.size() != 0_z;
    }

    inline std::vector<Node3D_SP> const& children() const
    {
      return m_children;
    }

    inline std::vector<Node3D_SP>& children()
    {
      return m_children;
    }

    std::vector<Node3D_SP>::iterator begin()
    {
      return m_children.begin();
    }

    std::vector<Node3D_SP>::iterator end()
    {
      return m_children.end();
    }

    inline bool hasParent() const
    {
      return m_parent != nullptr;
    }

    inline Node3D const& parent() const
    {
      return *m_parent;
    }

    inline Node3D& parent()
    {
      return *m_parent;
    }

  protected:

    std::vector<Node3D_SP> m_children;
    Node3D*  m_parent = nullptr;
    Matrix44f   m_world_transform;
    std::string m_name;
    bool m_traversable = true;
    bool m_attached = false;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_SCENEGRAPH_HPP
