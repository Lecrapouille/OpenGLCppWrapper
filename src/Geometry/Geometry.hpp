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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_HPP

#  include "Scene/SceneGraph.hpp"
#  include "OpenGL/Program.hpp"
#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(Shape3D)

// *****************************************************************************
//! \brief Class having predefined GLProgram, GLVBO for holding data of a 3D
//! model.
//! \ingroup OpenGL.
//! \fixme Uuse the idea of GLObject begin() to create the Shape only when
//! OpenGL context is created ?
// *****************************************************************************
class Shape3D: public Node3D
{
public: // TODO BoundingBox, BoundingSphere

  //----------------------------------------------------------------------------
  //! \brief Constructor. Compile the internal GLSL shader.
  //!
  //! \throw OpenGLException is the OpenGL context is not created or if the
  //! GLProgram could not be compiled.
  //!
  //! \note Make sure the OpenGL is created before (from the constructor of
  //! IGLWindow) else an OpenGLException is thrown.
  //!
  //! \note we init reference on VAO not yet existing, this is fine because map
  //!       does not invalidate iterator and references.
  //!
  //! \fixme Attach Material, attach std::function<onUpdate>
  //----------------------------------------------------------------------------
  Shape3D(std::string const& name, Material_SP material = MaterialBasic::create())
    : Node3D(name, true),
      m_material(material),
      m_program(m_material->program()),
      m_vao("VAO_" + name),
      m_vertices(m_vao.vector3f("position")),
      m_normals(m_vao.vector3f("normal")),
      m_uv(m_vao.vector2f("UV")),
      m_texture(m_vao.texture2D("texture")),
      m_indices(m_vao.index32())
  {
    throwIfOpenGLClassCalledBeforeContext();

    DEBUG("%s", "<<<---------- NewGeometry+Material ---------------------------------------------");

    if (unlikely(!m_program.compile()))
      throw OpenGLException(m_program.getError());

    m_program.bind(m_vao);
    m_material->init(); // FIXME: ugly code

    model() = Matrix44f(matrix::Identity);

    DEBUG("%s", "------------- NewGeometry+Material ------------------------------------------>>>");
  }

  // TODO
  //Shape3D(std::string const& name)
  //  : Shape3D(name, Material::create(Material::Type::Basic))
  //{}

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release VBOs.
  //----------------------------------------------------------------------------
  virtual ~Shape3D()
  {}

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  inline Material& material()
  {
    return *m_material;
  }

  //----------------------------------------------------------------------------
  //! \brief Tell if the instance is valid: the OpenGL Program has been compiled
  //! VAO has all its VBOs filled and textures loaded.
  //! \return true if everything is ok, else return false and check logs for
  //! knowing which elements have not been initialized.
  //! \note Use this function for debug purpose.
  //----------------------------------------------------------------------------
  inline bool isValid()
  {
    return m_program.isValid() && m_vao.checkVBOSizes() &&
      m_vao.checkLoadTextures();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the VBO holding vertices positions.
  //! \return VBO holding vertex positions of the model.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vertices()
  {
    return m_vertices;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the nth vertex position. If not present and
  //! the VBO can be resized, the VBO size is updated. The nth element is set
  //! pending to be transfered to the GPU memory.
  //! \param[in] nth the desired vertex.
  //! \return the nth vertex positions of the model.
  //----------------------------------------------------------------------------
  inline Vector3f& vertices(size_t const nth)
  {
    return m_vertices.set(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the VBO holding face normals.
  //! \return VBO holding face normals of the model.
  //! \fixme rename to faceNormals() and implement vertexNormals()
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& normals()
  {
    return m_normals;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the nth face normal. If not present and the
  //! VBO can be resized, the VBO size is updated. The nth element is set
  //! pending to be transfered to the GPU memory.
  //! \param[in] nth the desired normal.
  //! \return the nth face normal of the model.
  //----------------------------------------------------------------------------
  inline Vector3f& normals(size_t const nth)
  {
    return m_normals.set(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the VBO holding texture positions.
  //! \return VBO holding texture position of the model.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector2f>& uv()
  {
    return m_uv;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the nth texture position. If not present
  //! and the VBO can be resized, the VBO size is updated. The nth element is
  //! set pending to be transfered to the GPU memory.
  //! \param[in] nth the desired texture position.
  //! \return the nth texture position of the model.
  //----------------------------------------------------------------------------
  inline Vector2f& uv(size_t const nth)
  {
    return m_uv.set(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the matrix transformation of the model.
  //! \return the reference of the matrix.
  //----------------------------------------------------------------------------
  inline Matrix44f& model()
  {
    return m_program.matrix44f("modelMatrix");
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the matrix transformation of the view.
  //! \return the reference of the matrix.
  //----------------------------------------------------------------------------
  inline Matrix44f& view()
  {
    return m_program.matrix44f("viewMatrix");
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the matrix projection.
  //! \return the reference of the matrix.
  //----------------------------------------------------------------------------
  inline Matrix44f& projection()
  {
    return m_program.matrix44f("projectionMatrix");
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  inline Vector3f& cameraPosition()
  {
    return m_program.vector3f("cameraPosition");
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the texture (Material).
  //! \return the reference of the texture.
  //----------------------------------------------------------------------------
  inline GLTexture2D& texture()
  {
    return m_texture;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the index of VBOs (EBO).
  //! \return the reference of the EBO.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer32& index()
  {
    return m_indices;
  }

  // TODO
  // void computeFaceNormals()
  // void computeVertexNormals()
  // void buildIndex()

  //----------------------------------------------------------------------------
  //! \brief Renderer the instance.
  //! \param[in] matrix the transformation matrix of the model.
  //! \note This method is functional if and only if isValid() returns true.
  //----------------------------------------------------------------------------
  inline void draw(Matrix44f const& matrix)
  {
    model() = matrix;
    draw();
  }

  //----------------------------------------------------------------------------
  //! \brief Renderer the instance.
  //! \note This method is functional if and only if isValid() returns true.
  //----------------------------------------------------------------------------
  inline void draw()
  {
    m_program.draw(m_vao, Mode::TRIANGLES, index());
  }

protected:

  //----------------------------------------------------------------------------
  //! \brief Update the transformation matrix of the model.
  //!
  //! Callback triggered by inherited class Transformable when the
  //! transformation matrix of the model has been changed.
  //!
  //! \param[in] matrix the transformation matrix of the model.
  //----------------------------------------------------------------------------
  virtual void onMatrixTransformUpdated(Matrix44f const& matrix)
  {
    DEBUG("Updating Transform Matrix for '%s'", name().c_str());
    model() = matrix;
  }

private:

  Material_SP               m_material;
  GLProgram&                m_program;
  //! \brief Holds all VBOs.
  GLVAO                     m_vao;
  //! \brief Reference on the VBO holding vertice positions.
  GLVertexBuffer<Vector3f>& m_vertices;
  //! \brief Reference on the VBO holding face normals.
  GLVertexBuffer<Vector3f>& m_normals;
  //! \brief Reference on the VBO holding texture positions.
  GLVertexBuffer<Vector2f>& m_uv;
  //! \brief Texture.
  //! \fixme multitexture and move this to Material class.
  GLTexture2D&              m_texture;
  //! \brief EBO
  //! \fixme is uint8_t enough ?
  GLIndexBuffer32&          m_indices;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_HPP
