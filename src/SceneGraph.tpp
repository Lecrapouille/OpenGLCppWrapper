// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef SCENEGRAPH_TPP_
#  define SCENEGRAPH_TPP_

// The code source of this file has been inspired by the following document
// New Castle University, Tutorial 6: Scene Graphs
// https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf

#  include "Logger.hpp"
#  include "Movable.tpp"
#  include <memory>
#  include <vector>

template <typename R, typename T, uint32_t D>
class ISceneGraphRenderer
{
public:

  virtual ~ISceneGraphRenderer() {}
  virtual void drawNode(R& renderable, Matrix<T, D + 1u, D + 1u> const& transformation) = 0;
};

// *************************************************************************************************
//! \brief A Scene Graph is a spatial representation of a graphical
//! scene. Instead of storing entities in an array and where entities
//! have its position related to the world origin, a scene graph
//! classifies entities as a tree whith a parent-children relation (we
//! do not manage graph here). Each children has a relative position
//! from its parent. The relative position is given by a transform
//! matrix (scaling, translation and rotation). The aim of the scene
//! graph is to simplify placement and movement of entities. Moving a
//! node will automaticaly affect child nodes.
//!
//! I = node identifier (for searching a particuliar node).
//! R = renderable class( VAO, Mesh ...)
//! T, D = transformation matrix of type T and D its dimension (ie a 4x4 matrix of floats).
// *************************************************************************************************
template <typename I, typename R, typename T, uint32_t D>
class SceneGraph_t
{
  using ObjPtr = std::shared_ptr<R>;

public:

  // ***********************************************************************************************
  //! \brief A Scene Node is made of a transformation matrix (defined by its type T and dimension D
  //! mostly: T = float and D = 4) and an optional renderable object (implementing a draw() method).
  // ***********************************************************************************************
  class Node : public Movable<T, D>
  {
    friend SceneGraph_t;
    using NodePtr = std::shared_ptr<Node>;

  public:

    //-----------------------------------------------------------------
    //! \brief Constructor for named scene node with a renderable attached to
    //! it. The transform matrix is the identity matrix. nullptr is
    //! accepted. renderable shall inherite from the Renderable class.
    //-----------------------------------------------------------------
    Node(ObjPtr renderable, I const& id)
    {
      m_renderable = renderable;
      m_id = id;
    }

    //-----------------------------------------------------------------
    //! \brief Constructor anonymous scene node with a renderable attached to
    //! it. The transform matrix is the identity matrix. nullptr is
    //! accepted. renderable shall inherite from the Renderable class.
    //-----------------------------------------------------------------
    Node(ObjPtr renderable = nullptr)
    {
      m_renderable = renderable;
    }

    //-----------------------------------------------------------------
    //! \brief Constructor for named scene node with no renderable attached to
    //! it. The transform matrix is the identity matrix.
    //-----------------------------------------------------------------
    Node(I const& id)
    {
      m_id = id;
    }

    //-----------------------------------------------------------------
    //! \brief Destructor. Smart pointers will free automaticaly stuffs.
    //-----------------------------------------------------------------
    virtual ~Node()
    {
      CPP_LOG(logger::Info) << "SceneGraph delete node '" << m_id << "'\n";
      m_children.clear();
    }

    //-----------------------------------------------------------------
    //! \brief Replace the renderable attached to the scene node. Smart
    //! pointers will do their job if needed. nullptr is accepted.
    //-----------------------------------------------------------------
    inline void renderable(ObjPtr renderable)
    {
      m_renderable = renderable;
    }

    //-----------------------------------------------------------------
    //! \brief Get the renderable attached to the scene node. nullptr can be
    //! returned.
    //-----------------------------------------------------------------
    inline ObjPtr renderable()
    {
      return m_renderable;
    }

    //-----------------------------------------------------------------
    //! \brief Get the renderable attached to the scene node. nullptr can be
    //! returned.
    //-----------------------------------------------------------------
    inline ObjPtr const renderable() const
    {
      return m_renderable;
    }

    //-----------------------------------------------------------------
    //! \brief Create an anonymous scene node, attach it on the scene
    //! graph as a child of this class.
    //! \return the created smart pointer of the scene node.
    //-----------------------------------------------------------------
    NodePtr attach()
    {
      NodePtr node = std::make_shared<Node>();
      node->m_parent = this;
      m_children.push_back(node);
      return node;
    }

    //-----------------------------------------------------------------
    //! \brief Create a named scene node, attach it on the scene graph
    //! as a child of this class. Attach a renderable element given by
    //! its smart pointer.
    //! \return the created smart pointer of the scene node.
    //-----------------------------------------------------------------
    NodePtr attach(ObjPtr renderable, I const& id)
    {
      NodePtr node = attach();
      node->renderable(renderable);
      node->m_id = id;
      return node;
    }

    //-----------------------------------------------------------------
    //! \brief Attach on the scene graph a created node refered by its
    //! smart pointer.
    //-----------------------------------------------------------------
    void attach(NodePtr node)
    {
      m_children.push_back(node);
    }

    //-----------------------------------------------------------------
    //! \brief Return the world transform matrix.
    //! \note the local transform matrix is given by Movable::transform().
    //-----------------------------------------------------------------
    inline Matrix<T, D + 1u, D + 1u> const& worldTransform() const
    {
      return m_world_transform;
    }

    //-----------------------------------------------------------------
    //! \brief Compute recursively transformation matrices of all
    //! children.
    //! \param dt: is the delta time (the FPS) that can be used by
    //! classes that override this method.
    //-----------------------------------------------------------------
    virtual void update(float const dt)
    {
      CPP_LOG(logger::Info) << "ScenGraph::updating '" << m_id << "'\n";
      if (nullptr != m_parent)
        {
          // This  node  has a parent
          m_world_transform = Movable<T, D>::transform()
            * m_parent->m_world_transform;
        }
      else
        {
          // Root node, world transform is local transform
          m_world_transform = Movable<T, D>::transform();
        }

      // Update all children
      for (auto& i: m_children)
        {
          i->update(dt);
        }
    }

    //-----------------------------------------------------------------
    //! \brief Draw the whole scene graph. Each nodes shall implement
    //! a draw() method.
    //-----------------------------------------------------------------
    virtual void draw(ISceneGraphRenderer<R, T, D>& renderer) //const
    {
      CPP_LOG(logger::Info) << "ScenGraph::draw '" << m_id << "'\n";
      // Sheets are optional, so do not forget to check against nullptr
      if (nullptr != m_renderable)
        {
          Matrix<T, D + 1u, D + 1u> transform =
            matrix::scale(m_world_transform, m_local_scaling);
          renderer.drawNode(*m_renderable, transform);
        }

      // Recursive iteration of the graph for drawing other node
      for (auto const& i: m_children)
        {
          i->draw(renderer);
        }
    }

    //-----------------------------------------------------------------
    //! \brief Set a local scaling of the object. By local we mean that
    //! will not impact children (they will not be scaled).
    //! \note for scaling children use methods Movable::scale or
    //! Movable::scaleFactor.
    //-----------------------------------------------------------------
    inline void localScale(Vector<T, D> const &scale)
    {
      m_local_scaling = scale;
    }

    //-----------------------------------------------------------------
    //! \brief Get the local scaling.
    //-----------------------------------------------------------------
    inline Vector<T, D> const &localScale() const
    {
      return m_local_scaling;
    }

    //-----------------------------------------------------------------
    //! \brief Get the list of child nodes.
    //-----------------------------------------------------------------
    inline std::vector<NodePtr> const &children() const
    {
      return m_children;
    }

    //-----------------------------------------------------------------
    //! \brief Return the scene node identifer
    //-----------------------------------------------------------------
    inline I const& id() const
    {
      return m_id;
    }

  private:

    //! \brief Identifier for searching elements.
    I                      m_id;
    //! \brief holds a 3D entity. If no entity has to be hold set it to nullptr.
    ObjPtr                 m_renderable;
    //! \brief Pointer on the parent : nullptr for the root, for children it's never nullptr.
    /*NodePtr */ Node*     m_parent = nullptr;
    //! \brief World transformation matrix. Stored faor avoiding to compute it every times when
    //! traversing the scene graph.
    Matrix<T, D + 1u, D + 1u>  m_world_transform;
    //! List of Node as children. Pointers are never nullptr.
    std::vector<NodePtr>   m_children;
    //! Scale factors for the current 3D entity
    Vector<T, D>           m_local_scaling = Vector<T, D>(1);
  };

private:

  using NodePtr = std::shared_ptr<Node>;

public:

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  ~SceneGraph_t()
  {
    std::cout << "Destroy SceneGraph" << std::endl;
    m_root = nullptr;
  }

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  void draw(ISceneGraphRenderer<R, T, D>& renderer) // const
  {
    if (nullptr != m_root)
      m_root->draw(renderer);
  }

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  void update(float const dt)
  {
    if (nullptr != m_root)
      m_root->update(dt);
  }

  //-----------------------------------------------------------------
  //! \brief Search for a node from the given identifier. Algorithm
  //! complexity is O(n) iterations because nodes are not organized
  //! for beeing indexed by index but have a parent-child world
  //! transformation.
  //-----------------------------------------------------------------
  NodePtr findNode(I const& id)
  {
    return findNode(id, m_root);
  }

  //-----------------------------------------------------------------
  //! \brief Wrapper for findNode but return the renderable object.
  //-----------------------------------------------------------------
  ObjPtr findRenderable(I const& id)
  {
    ObjPtr o = find(id, m_root);
    if (nullptr != o)
      return o->renderable();
    return nullptr;
  }

  //-----------------------------------------------------------------
  //! \brief Attach a scene node to te
  //-----------------------------------------------------------------
  void attach(NodePtr node)
  {
    if (nullptr == m_root)
      {
        m_root = node;
        m_root->m_parent = nullptr;
      }
    else
      {
        m_root->attach(node);
        node->m_parent = &(*m_root);
      }
  }

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  NodePtr attach()
  {
    if (nullptr == m_root)
      {
        createRoot();
        return m_root;
      }

    return m_root->attach();
  }

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  NodePtr attach(ObjPtr renderable, I const& id)
  {
    if (nullptr == m_root)
      {
        createRoot();
        m_root->renderable(renderable);
        m_root->m_id = id;
        return m_root;
      }

    return m_root->attach(renderable, id);
  }

  //-----------------------------------------------------------------
  //! \brief Return the root of the scene graph.
  //-----------------------------------------------------------------
  inline NodePtr root()
  {
    return m_root;
  }

private:

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  void createRoot()
  {
    m_root = std::make_shared<Node>();
    m_root->m_parent = nullptr;
  }

  //-----------------------------------------------------------------
  //! \brief Find recursively a node by its id.
  //! \note this method has been placed here and not in the scene node
  //! class because to avoid inherting from std::enable_shared_from_this.
  //-----------------------------------------------------------------
  NodePtr findNode(I const& id, NodePtr res)
  {
    // This case is suppose to never happen
    if (nullptr == res)
      {
        std::cout << "nullptr error" << std::endl;
        return nullptr;
      }

    std::cout << "Compare " << res->m_id << " " << id << std::endl;
    if (res->m_id == id)
      {
        std::cout << "Found " << res->m_id << std::endl;
        return res;
      }

    for (auto i: res->m_children)
      {
        std::cout << "Finding child" << std::endl;
        NodePtr n = findNode(id, i);
        if (nullptr != n)
          return n;
      }

    std::cout << "Finding nothing" << std::endl;
    return nullptr;
  }

private:

  //! \brief the root node of the tree.
  NodePtr m_root = nullptr;
};

#endif /* SCENEGRAPH_TPP_ */
