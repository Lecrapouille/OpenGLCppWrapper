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

// inspired from https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf

#  include "Logger.hpp"
#  include "Movable.tpp"
#  include <memory>
#  include <vector>

// *************************************************************************************************
//! This class defines a node of the scene graph. It holds a
//! transformation matrix (scaling, rotating, translating) that can be
//! modified by the user through methods (Movable). It can also holds
//! an 3D entity (Mesh class) that can be drawn (Drawable) while it's
//! not mandatory (in this case the node is used for transforming children).
// *************************************************************************************************
template <typename Mesh, typename Type, uint32_t dim>
class SceneNode
  : public Movable<Type, dim>
{
public:

  //! \brief Constructor for anonymous node with no 3D entity attached
  //! to it. The transform matrix is the identity matrix.
  SceneNode()
    : m_local_scaling(1)
  {
    m_mesh = nullptr;
    m_parent = nullptr;
  }

  //! \brief Constructor for a node with name and holding a 3D entity.
  //! mesh shall be allocated because will be free by the destructor.
  SceneNode(Mesh &mesh, const char *name)
    : m_local_scaling(1)
  {
    m_mesh = &mesh;
    m_parent = nullptr;
    m_name = name;
  }

  //! \brief Constructor for a node with name and holding a 3D entity.
  //! mesh shall be allocated because will be free by the destructor.
  //! mesh can be nullptr.
  SceneNode(Mesh *mesh, const char *name)
    : m_local_scaling(1)
  {
    m_mesh = mesh;
    m_parent = nullptr;
    m_name = name;
  }

  //! \brief Constructor for anonymous node with name and holding a 3D
  //! entity. mesh shall be allocated because will be free by the destructor.
  SceneNode(Mesh &mesh)
    : m_local_scaling(1)
  {
    m_mesh = &mesh;
    m_parent = nullptr;
  }

  //! \brief Constructor for a node with name and holding a 3D entity.
  //! mesh shall be allocated because will be free by the destructor.
  //! mesh can be nullptr.
  SceneNode(const char *name)
  {
    m_mesh = nullptr;
    m_parent = nullptr;
    m_name = name;
  }

  //! \brief Destructor. Free all 3D entites.
  virtual ~SceneNode()
  {
    LOGI("SceneGraph delete node '%s'", m_name.c_str());
    size_t i = m_children.size();
    while (i--)
      {
        delete m_children[i];
      }
  }

  inline void mesh(Mesh &mesh)
  {
    m_mesh = &mesh;
  }

  inline Mesh *mesh() const
  {
    return m_mesh;
  }

  //! \brief Attach a scene node as a child.
  //!
  //! \note Be careful of not adding a node already present as
  //! ancestor. This will create a graph and a circular reference.
  //! This pathologic case is not checked for faster runtime.
  void add(SceneNode &node)
  {
    node.m_parent = this;
    m_children.push_back(&node);
  }

  //! \brief Return the world transform matrix.
  //! \note the local transform matrix is given by Movable::transform().
  inline Matrix<Type, dim + 1U, dim + 1U> const& worldTransform() const
  {
    return m_world_transform;
  }

  //! \brief Compute recursively transformation matrices of all
  //! children.
  //! \param dt: is the delta time (the FPS) that can be used by
  //! classes that override this method.
  virtual void update(float const dt)
  {
    LOGI("ScenGraph::updating '%s'", m_name.c_str());
    if (nullptr != m_parent)
      {
        // This  node  has a parent
        m_world_transform = Movable<Type, dim>::transform()
          * m_parent->m_world_transform;
      }
    else
      {
        // Root node, world transform is local transform
        m_world_transform = Movable<Type, dim>::transform();
      }

    // Update all children
    for (auto i: m_children)
      {
        i->update(dt);
      }
  }

  //! \brief Set a local scaling of the object. By local we mean that
  //! will not impact children (they will not be scaled).
  //! \note for scaling children use methods Movable::scale or
  //! Movable::scaleFactor.
  inline void localScale(Vector<Type, dim> const &scale)
  {
    m_local_scaling = scale;
  }

  //! \brief Get the local scaling.
  inline Vector<Type, dim> const &localScale() const
  {
    return m_local_scaling;
  }

  inline std::vector<SceneNode<Mesh, Type, dim>*> const &children() const
  {
    return m_children;
  }

  //! \brief Give a name to the node.
  std::string                        m_name;

protected:

  //! \brief holds a 3D entity. If no entity has to be hold set it to nullptr.
  Mesh                              *m_mesh;
  //! \brief Pointer on the parent : nullptr for the root, for children it's never nullptr.
  SceneNode                         *m_parent;
  //! \brief World transformation matrix. Stored faor avoiding to compute it every times when
  //! traversing the scene graph.
  Matrix<Type, dim + 1U, dim + 1U>   m_world_transform;
  //! List of SceneNode as children. Pointers are never nullptr.
  std::vector<SceneNode<Mesh, Type, dim>*> m_children;
  //! Scale factors for the current 3D entity
  Vector<Type, dim>                  m_local_scaling;
};

// *************************************************************************************************
//! \brief A SceneGraph is a method for organizing 3D entities inside
//! a 3D world. Instead of storing entities in an array and where
//! entities have its position related to the world origin, a scene
//! graph classifies entities as a tree whith a parent-children
//! relation. Each children has a relative position from its
//! parent. The relative position is given by a transform matrix
//! (scaling, translation and rotation). The aim of the scene graph is
//! too simplify movement of entities: for example translating a
//! parent will automaticly translate all children (same thing for
//! scaling and rotating). An example of scen graph could be a car,
//! its stearing wheel and its four wheels.
//!
//! This class only manage the root of the scene graph.
// *************************************************************************************************
template <typename Mesh, typename Type, uint32_t dim>
class SceneGraph
{
public:

  SceneGraph()
    : m_root(nullptr)
  {
  }

  ~SceneGraph()
  {
    if (nullptr != m_root)
      delete m_root;
  }

  inline SceneNode<Mesh, Type, dim> *root()
  {
    return m_root;
  }

  inline void root(SceneNode<Mesh, Type, dim> *node)
  {
    if (nullptr != m_root)
      delete m_root;
    m_root = node;
  }

private:

  SceneNode<Mesh, Type, dim> *m_root;
};

#endif /* SCENEGRAPH_TPP_ */
