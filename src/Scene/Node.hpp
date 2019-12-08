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

#ifndef OPENGLCPPWRAPPER_Node_HPP
#  define OPENGLCPPWRAPPER_Node_HPP

#  include "Scene/Transformable.hpp"
#  include <iostream>

namespace glwrap
{
  DECLARE_CLASS(Node3D)

  // ***************************************************************************
  //! \brief Class defining a node of a Scene Graph.
  //!
  //! A scene (aka scene graph) is a spatial representation of a graphical
  //! scene. Instead of storing 3D models in an array where their positions are
  //! related to the world origin, a scene graph classifies entities inside a
  //! tree structure. The parent-children relation between nodes of the tree
  //! allow to define relative transformation of their positions. Each children
  //! has a relative position from its parent given by a transform matrix
  //! (scaling, translation and rotation). The aim of the scene graph is to
  //! simplify placement and movement of entities because moving a node will
  //! automaticaly affect child nodes and their descendants.
  //!
  //! For more information you can read this introduction document:
  //! https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf
  //! The code source of this file largely get inspired by this document.
  // ***************************************************************************
  class Node3D
    : public Transformable3D
  {
    // FIXME protected: to force using Node3D::create
  public:

    //--------------------------------------------------------------------------
    //! \brief Create a scene node with a given name.
    //!
    //! \param name the name of the node. It is used mainly for the debug.
    //! \param renderable not all nodes are made for displaying 3D models but
    //! can be used as intermediate for doing specific computations. Set this
    //! parameter to false when the node is not made for being rendered.
    //--------------------------------------------------------------------------
    Node3D(std::string const& name, bool const renderable)
      : m_name(name),
        m_renderable(renderable)
    {
      DEBUG("Create Node3D named '%s'", name.c_str());
    }

  public:

    //--------------------------------------------------------------------------
    //! \brief Release the node and all its descenants automatically done thanks
    //! to internal smart pionters.
    //--------------------------------------------------------------------------
    virtual ~Node3D()
    {
      DEBUG("Destroy Node3D %s", name().c_str());
      m_children.clear();
    }

    //--------------------------------------------------------------------------
    //! \brief Static method allowing to create a scene node.
    //--------------------------------------------------------------------------
    static Node3D_SP create(std::string const& name, bool const renderable)
    {
      return std::make_shared<Node3D>(name, renderable);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the name for the log and debug usages.
    //--------------------------------------------------------------------------
    std::string const& name() const
    {
      return m_name;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the name for the log and debug usages.
    //--------------------------------------------------------------------------
    const char* cname() const
    {
      return m_name.c_str();
    }

    //--------------------------------------------------------------------------
    //! \brief Display on the console information about children nodes.
    //! \note this function is not recursive and does not show information on
    //! descenants of children nodes.
    //--------------------------------------------------------------------------
    virtual void debug() const
    {
      std::cout << "Scene Node '" << m_name << "':" << std::endl;
      std::cout << "  Traversable: " << isTraversable()
                << " Renderable: " << isRenderable() << std::endl;
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

    //--------------------------------------------------------------------------
    //! \brief Check if the node holds a 3D model for the render.
    //! \return Return true if the node is made for being rendered, else false.
    //--------------------------------------------------------------------------
    inline bool isRenderable() const
    {
      return m_renderable;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the information if the node and all its descendants can be
    //! visited.
    //!
    //! \return true if the node can be visited, else return false.
    //--------------------------------------------------------------------------
    inline bool isTraversable() const
    {
      return m_traversable;
    }

    //--------------------------------------------------------------------------
    //! \brief Make the node can be (or not) visited. A not travsersable node
    //! and its descendants are ignored by functions such as traverse().
    //--------------------------------------------------------------------------
    inline void setTraversable(bool const traversable = true)
    {
      m_traversable = traversable;
    }

    //--------------------------------------------------------------------------
    //! \brief Traverse recursively the scene for computing all world
    //! transformation matrices for traversable nodes. Non traversable nodes and
    //! their descendant are ignored.
    //!
    //! \note no traversable nodes are ignored as well as their descendants.
    //!
    //! \param dt: delta time from the previous update.
    //--------------------------------------------------------------------------
    void update(float const dt)
    {
      if (unlikely(!isTraversable()))
        return;

      doUpdate(dt);
      m_world_transform = Transformable3D::transform();
      if (unlikely(nullptr != m_parent))
        m_world_transform *= m_parent->m_world_transform;

      for (auto& i: m_children)
        i->update(dt);
    }

    //--------------------------------------------------------------------------
    //! \brief Callback triggered when visiting the scene for updating
    //! transformation matrices. This method does nothing by default but can be
    //! overrided by Node3D's derived classes for animating/moving the node.
    //!
    //! \note: https://github.com/vahlers/scg3 traverse the tree with the vistor
    //! design pattern but I am not found of visitors, they make the code harder
    //! to understand. In
    //! https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf
    //! they let the user overriding the method update() and suppose the
    //! developper will call the the method update() of the parent. I think this
    //! is too error prone. I think doUpdate() does the same thing in a more
    //! simpler way.
    //!
    //! \param dt: delta time from the previous update.
    //--------------------------------------------------------------------------
    virtual void doUpdate(float const /*dt*/)
    {}

    //--------------------------------------------------------------------------
    //! \brief Traverse recursively the scene for drawing 3D models.
    //!
    //! \note no traversable nodes are ignored as well as their descendants.
    //!
    //! \fixme Can we compile/compute the scene once and only update some parts ?
    //--------------------------------------------------------------------------
    virtual void renderer(/* RendererState */)
    {
      if (unlikely(!isTraversable()))
        return;

      if (likely(m_renderable))
        {
          // TODO: is not better to create an node like OpenInventor separator
          // instead of this computation made everytime even if
          // When scaling a node it will also scale descendants. Sometimes you
          // just want to scale the node not its descendants.
          draw(matrix::scale(m_world_transform, Transformable3D::localScale()));
        }

      for (auto& c: m_children)
        c->renderer();
    }

    //--------------------------------------------------------------------------
    //! \brief Callback triggered for drawing the 3D model. This method does
    //! nothing by default but can be overrided by Node3D's derived classes for
    //! rendering the object.
    //!
    //! \param modelMatrix the transformation matrix placing the model in the
    //! space. Pass this matrix to the shader.
    //--------------------------------------------------------------------------
    virtual void draw(Matrix44f const& /*modelMatrix*/)
    {}

    //--------------------------------------------------------------------------
    //! \brief Return the local transformation matrix
    //--------------------------------------------------------------------------
    Matrix44f const& localTransform() //FIXME const
    {
      return Transformable3D::transform();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the global transformation matrix
    //--------------------------------------------------------------------------
    Matrix44f const& worldTransform() const
    {
      return m_world_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the global transformation matrix
    //--------------------------------------------------------------------------
    Matrix44f& worldTransform()
    {
      return m_world_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief Add a descendant node. Return *this for combining several attachements.
    //--------------------------------------------------------------------------
    Node3D& add(Node3D_SP node)
    {
      if (likely(this == node.get()))
        {
          ERROR("Cannot attach a node to itself");
          return *this;
        }

      if (unlikely(node->m_attached))
        {
          ERROR("Node already attached");
          return *this;
        }

      node->m_parent = this;
      m_children.push_back(node);
      onNodeAdded(node);
      return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Remove a node and its descendants from the scene.
    //--------------------------------------------------------------------------
    Node3D& remove(Node3D_SP node)
    {
      auto it = std::find_if(m_children.begin(), m_children.end(),
        [node](Node3D_SP i){ return i.get() == node.get(); });

      if (it != m_children.end())
        m_children.erase(it);

      onNodeRemoved();
      return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Execute the function on the node and each of its descendants.
    //--------------------------------------------------------------------------
    template<typename Functor>
    static void traverse(Node3D_SP& node, Functor functor)
    {
      if (unlikely(!node->isTraversable()))
        return;

      functor(node);
      for (auto& c: node->children())
        traverse(c, functor);
    }

    //--------------------------------------------------------------------------
    //! \brief Execute the function on ancestors of the given node.
    //! \note the function is not called on the node passed as parameter.
    //--------------------------------------------------------------------------
    template<typename Functor>
    static void traverseAncestors(Node3D_SP node, Functor functor)
    {
      if (likely(node->hasParent()))
        {
          functor(node->parent());
          traverseAncestors(node->parent(), functor);
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Find the first child matching positively with the given functor.
    //!
    //! \note the depth of the search is 0: this method only focus on direct
    //! children of the node. It does not search descendant of children.
    //!
    //! \return the node if found any, else return nullptr.
    //--------------------------------------------------------------------------
    template<typename Functor>
    Node3D_SP findChild(Functor predicate)
    {
      auto it = std::find_if(m_children.begin(), m_children.end(), predicate);
      return (it != m_children.end()) ? *it : nullptr;
    }

    //--------------------------------------------------------------------------
    //! \brief Find the first child matching positively with the given name.
    //!
    //! \note the depth of the search is 0: this method only focus on direct
    //! children of the node. It does not search descendant of children.
    //!
    //! \return the node if found any, else return nullptr.
    //--------------------------------------------------------------------------
    Node3D_SP getChild(std::string const& name)
    {
      return findChild([name](Node3D_SP i){ return name == i->name(); });
    }

    //--------------------------------------------------------------------------
    //! \brief Sugar syntax for getChild() but does not check if the node really
    //! exists or not.
    //--------------------------------------------------------------------------
    //Node3D_SP operator[](std::string const& name)
    //{
      // //return getChild(name);
      //return getNodeFromPath(std::shared_from_this(), name);
    //}

    //--------------------------------------------------------------------------
    //! \brief Sugar syntax for getChildFromPath() but does not check if the node
    //! really exists or not.
    //--------------------------------------------------------------------------
    //Node3D_SP operator()(std::string const& name)
    //{
    //  return getNodeFromPath(shared_from_this(), name);
    //}

    //--------------------------------------------------------------------------
    //! \brief Find recursively the first node matching the predicate. Halt on
    //! the first match.
    //!
    //! \param root the node to start searching.
    //! \param predicate the functor taking a scene node as parameter and
    //! returning a bool whome the value depends on the matching result.
    //!
    //! \return the node on success, else return nullptr.
    //--------------------------------------------------------------------------
    template<typename Functor>
    static Node3D_SP findNode(Node3D_SP root, Functor predicate)
    {
      if (predicate(root))
        return root;

      for (auto& c: root->m_children)
        {
          Node3D_SP node = findNode(c, predicate);
          if (node != nullptr)
            return node;
        }

      return nullptr;
    }

    //--------------------------------------------------------------------------
    //! \brief Find recursively the first node matching the given name. Halt on
    //! the first match.
    //!
    //! \param root the node to start searching.
    //! \param predicate the functor taking a scene node as parameter and
    //! returning a bool whome the value depends on the matching result.
    //!
    //! \return the node on success, else return nullptr.
    //--------------------------------------------------------------------------
    static Node3D_SP getNode(Node3D_SP root, std::string const& name)
    {
      return Node3D::findNode(root, [name](Node3D_SP i){ return name == i->name(); });
    }

    //--------------------------------------------------------------------------
    //! \brief Return the node following the given tree path.
    //!
    //! \param root the node to start searching.
    //! \param the tree path: a set of node names separating by '/'. ie "/root/node"
    //!
    //! \return the node on success, else return nullptr.
    //--------------------------------------------------------------------------
    static Node3D_SP getNodeFromPath(Node3D_SP root, std::string const& path)
    {
      std::stringstream sspath(path);
      std::string name;
      Node3D_SP child = nullptr;
      Node3D_SP node = root;

      while (std::getline(sspath, name, '/'))
        {
          if (name == "")
            continue;

          child = node->getChild(name);
          if (child == nullptr)
            return nullptr;

          node = child;
        }
      return child;
    }

    //--------------------------------------------------------------------------
    //! \brief Check the existence of child nodes.
    //! \return Return true if at least one child node exits.
    //--------------------------------------------------------------------------
    inline bool hasChildren() const
    {
      return m_children.size() != 0_z;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the container holding child nodes.
    //--------------------------------------------------------------------------
    inline std::vector<Node3D_SP> const& children() const
    {
      return m_children;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the container holding child nodes.
    //--------------------------------------------------------------------------
    inline std::vector<Node3D_SP>& children()
    {
      return m_children;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the begin iterator on the child nodes.
    //--------------------------------------------------------------------------
    std::vector<Node3D_SP>::iterator begin()
    {
      return m_children.begin();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the end iterator on the child nodes.
    //--------------------------------------------------------------------------
    std::vector<Node3D_SP>::iterator end()
    {
      return m_children.end();
    }

    //--------------------------------------------------------------------------
    //! \brief Check the existence of the parent node.
    //! \return Return true if the parent node exists else return false.
    //--------------------------------------------------------------------------
    inline bool hasParent() const
    {
      return m_parent != nullptr;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the parent node.
    //! \note No check is made to check the existence of the parent node. The
    //! parent is supposed to be known.
    //--------------------------------------------------------------------------
    inline Node3D const& parent() const
    {
      return *m_parent;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the parent node.
    //! \note No check is made to check the existence of the parent node. The
    //! parent is supposed to be known.
    //--------------------------------------------------------------------------
    inline Node3D& parent()
    {
      return *m_parent;
    }

  protected:

    //--------------------------------------------------------------------------
    //! \brief Callback when a node has been added. By default do nothing but
    //! derived class can override this method.
    //!
    //! \param node newly inserted node.
    //--------------------------------------------------------------------------
    virtual void onNodeAdded(Node3D_SP& /*node*/)
    {}

    //--------------------------------------------------------------------------
    //! \brief Callback when a node has been removed. By default do nothing but
    //! derived class can override this method.
    //--------------------------------------------------------------------------
    virtual void onNodeRemoved()
    {}

  protected:

    //! \brief Descendant nodes
    std::vector<Node3D_SP> m_children;
    //! \brief Ancestor of node. The root of the scene has no ancestor and
    //! therefore set to nullptr. TODO std::weak_ptr ?
    Node3D*               m_parent = nullptr;
    //! \brief the matrix transform to apply on the child. Transformations are
    //! relative to the parent node.
    Matrix44f             m_world_transform;
    //! \brief Scene node name. For debug and log usage.
    std::string           m_name;
    //! \brief When not traversable, ignore the node and its descendant.
    bool                  m_traversable = true;
    //! \brief A Scene is a tree. This bool avoids to create a graph avoiding to
    //! produce circuits making infinite loop or making the algorithm of
    //! traverse more complex.
    bool                  m_attached = false;
    //! \brief Not all nodes are made for displaying 3D models but can be used
    //! as intermediate for doing specific computations. Set this parameter to
    //! false when the node is not made for being rendered.
    bool                  m_renderable;
  };
} // namespace glwrap

#endif // OPENGLCPPWRAPPER_Node_HPP
