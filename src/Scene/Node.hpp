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
  DECLARE_CLASS(Node3D);

  // ***************************************************************************
  //! \brief Class defining a Scene node.
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
  //! For more information you can read this introduction document:
  //! https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf
  //! The code source of this file largely get inspired by this document.
  // ***************************************************************************
  class Node3D
    : public Transformable3D
  {
  public:

    //--------------------------------------------------------------------------
    //! \brief Create a scene node with a given name.
    //!
    //! \param name the name of the node. It is used mainly for the debug.
    //! \param renderable not all nodes are made for displaying 3D models but
    //! can be used as intermediate for doing additional transformational
    //! computations. So, set this parameter to false when the node is not made
    //! for being rendered.
    //--------------------------------------------------------------------------
    Node3D(std::string const& name, bool const renderable)
      : m_name(name),
        m_renderable(renderable)
    {
      DEBUG("Create Node3D named '%s'", name.c_str());
    }

    //--------------------------------------------------------------------------
    //! \brief Create a not renderable scene node having an empty name.
    //--------------------------------------------------------------------------
    Node3D()
      : Node3D("", false)
    {}

  public:

    //--------------------------------------------------------------------------
    //! \brief Release the node and all its descenants automatically done thanks
    //! to internal smart pionters.
    //--------------------------------------------------------------------------
    virtual ~Node3D()
    {
      DEBUG("Destroy Node3D '%s'", name().c_str());
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
        std::cout << "  Parent '" << m_parent->name() << std::endl;
      else
        std::cout << "  is root" << std::endl;
      std::cout << "  " << m_children.size() << " Child:";
      for (auto it: m_children)
        {
          std::cout << " " << it->name();
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

      // Derived class may override this function for animating nodes.
      doUpdate(dt);

      // Update the matrix transform from the parent matrix.
      m_world_transform = Transformable3D::transform();
      if (likely(hasParent()))
        m_world_transform *= parent().m_world_transform;

      for (auto& i: m_children)
        i->update(dt);
    }

    //--------------------------------------------------------------------------
    //! \brief Callback triggered when visiting the scene for updating
    //! transformation matrices. This method does nothing by default but can be
    //! overrided by Node3D's derived classes for animating/moving the current
    //! node (which will impact on position of descendant nodes).
    //!
    //! \note: the project https://github.com/vahlers/scg3 traverse the tree
    //! using the vistor design pattern. I did not follow because I am a big fan
    //! of visitors: in my personal opinion they make the code harder to
    //! understand. In the course https://research.ncl.ac.uk/game/mastersdegree/
    //! graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf
    //! they let derived classes override the method update() but suppose the
    //! developper will call the method update() of the parent class. I think this
    //! is an error prone pratice. I think doUpdate() does the same thing in a more
    //! simpler way for the developper.
    //!
    //! \param dt: delta time from the previous update.
    //--------------------------------------------------------------------------
    virtual void doUpdate(float const /*dt*/)
    {}

    //--------------------------------------------------------------------------
    //! \brief Traverse recursively the scene for drawing all renderable 3D models.
    //!
    //! \note non-traversable nodes are ignored as well as their descendants.
    //!
    //! \fixme Can we compile/compute the scene once and only update some parts ?
    //--------------------------------------------------------------------------
    virtual void render(/* RendererState */)
    {
      if (unlikely(!isTraversable()))
        return;

      if (likely(m_renderable))
        {
          // TODO: is this not better to create an node like OpenInventor
          // separator instead of this computation made everytime (even if
          // scaling a node it will also scale descendants)? Sometimes you just
          // want to scale the node not its descendants.
          draw(matrix::scale(m_world_transform, Transformable3D::localScale()));
        }

      for (auto& c: m_children)
        c->render();
    }

    //--------------------------------------------------------------------------
    //! \brief Callback triggered for drawing the 3D model. This method does
    //! nothing by default but can be overrided by Node3D's derived classes for
    //! rendering the object.
    //!
    //! \param modelMatrix the transformation matrix placing the model in the
    //! space. Usually pass this matrix to the uniform shader named 'model
    //! matrix'.
    //--------------------------------------------------------------------------
    virtual void draw(Matrix44f const& /*modelMatrix*/)
    {}

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the local transformation matrix.
    //--------------------------------------------------------------------------
    Matrix44f const& localTransform()
    {
      return Transformable3D::transform();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the global transformation matrix.
    //--------------------------------------------------------------------------
    Matrix44f const& worldTransform() const
    {
      return m_world_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference to the global transformation matrix.
    //--------------------------------------------------------------------------
    Matrix44f& worldTransform()
    {
      return m_world_transform;
    }

    //--------------------------------------------------------------------------
    //! \brief Attach the given node as child to this instance and trig the
    //! callback onNodeAdded().
    //!
    //! \note A node cannot be attached to itself or on an already attached
    //! node. This constrain avoid to create a cycle making the traversal of
    //! the scene more difficult.
    //!
    //! \param node the node which will be the child node. It will placed as the
    //! last child.
    //!
    //! \note Child hierarchy order is important because the tree is traversed
    //! using a depth-first traversal with pre-order.
    //!
    //! \return *this for combining several adds.
    //--------------------------------------------------------------------------
    Node3D& attach(Node3D_SP node)
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

      node->m_parent = this; // TODO: shared_from_this();
      m_children.push_back(node);
      onNodeAdded(node);
      return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Remove a node and its descendants from the scene. Memory is
    //! automatically released if nobody is refering the deleted node.
    //--------------------------------------------------------------------------
    Node3D& detach(Node3D_SP node)
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
    //! \note depth-first traversal with pre-order.
    //! \param node the node to start with.
    //! \param functor the function to call for each traversed node.
    //--------------------------------------------------------------------------
    template<typename Functor>
    static void traverse(Node3D_SP node, Functor functor)
    {
      if (unlikely(!node->isTraversable()))
        return;

      functor(node);
      for (auto& c: node->children())
        traverse(c, functor);
    }

    //--------------------------------------------------------------------------
    //! \brief Execute the function on ancestors of the given node.
    //! \param node the node to start with.
    //! \note the function is not called on the node passed as parameter.
    //! \param functor the function to call for each traversed node.
    //--------------------------------------------------------------------------
    template<typename Functor>
    static void traverseAncestors(Node3D_SP node, Functor functor)
    {
      Node3D& n = *(node.get());
      while (likely(n.hasParent()))
        {
          n = n.parent();
          functor(n);
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Return the root of the scene graph holding the node passed as
    //! parameter.
    //--------------------------------------------------------------------------
    static Node3D& root(Node3D_SP node)
    {
      Node3D& n = *(node.get());
      while (likely(n.hasParent()))
        n = n.parent();
      return n;
    }

    //--------------------------------------------------------------------------
    //! \brief Find the first child matching positively with the given functor.
    //!
    //! \param predicate the function to call for each traversed node.
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
    //Node3D_SP operator[](const char* name)
    //{
    //  return getNode(std::make_shared<Node3D>(this), std::string(name));
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
    //! \param root the node to start searching with.
    //! \param predicate the functor taking a scene node as parameter and
    //! returning a bool whome the value depends on the matching result.
    //!
    //! \return the node on success, else return nullptr.
    //--------------------------------------------------------------------------
    static Node3D_SP findNode(Node3D_SP root, std::string const& name)
    {
      return Node3D::findNode(root, [name](Node3D_SP i){ return name == i->name(); });
    }

    //--------------------------------------------------------------------------
    //! \brief Return the node following the given child path.
    //!
    //! \param root the node to start searching with.
    //!
    //! \param path the child path: a set of node names separating by '/'. i.e.
    //! "foo/bar". Note that traillings '/' are ignored i.e."foo/bar/" or
    //! "foo/bar/////". TODO If the path starts by '/' so the search is made from the
    //! root of the scene graph (ie "/root/foo/bar").
    //!
    //! \return the node on success, else return nullptr.
    //--------------------------------------------------------------------------
    static Node3D_SP getNode(Node3D_SP node, std::string const& path)
    {
      std::stringstream sspath(path);
      std::string name;
      Node3D_SP child = nullptr;
      Node3D_SP n = node; // FIXME: (path[0] == '/') ? root(node) : node;

      while (std::getline(sspath, name, '/'))
        {
          if (name == "")
            continue;

          child = n->getChild(name);
          if (child == nullptr)
            return nullptr;

          n = child;
        }
      return child;
    }

    //--------------------------------------------------------------------------
    //! \brief Check if the instance has child nodes.
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
      return m_parent != nullptr; // TODO return !m_parent.expired();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the parent node.
    //! \note No check is made to check the existence of the parent node. The
    //! parent is supposed to be known.
    //--------------------------------------------------------------------------
    inline Node3D const& parent() const // FIXME shall return Node3D_SP
    {
      return *m_parent; // TODO m_parent.lock();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the parent node.
    //! \note No check is made to check the existence of the parent node. The
    //! parent is supposed to be known.
    //--------------------------------------------------------------------------
    inline Node3D& parent() // FIXME shall return Node3D_SP
    {
      return *m_parent; // TODO m_parent.lock();
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
    //! therefore set to nullptr.
    Node3D*               m_parent = nullptr; // TODO Node3D_WP m_parent;
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
