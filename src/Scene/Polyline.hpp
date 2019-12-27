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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_POLYLINE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_POLYLINE_HPP

#  include "Scene/Node.hpp"
#  include "OpenGL/Program.hpp"
#  include "Material/Material.hpp"

namespace glwrap
{

DECLARE_CLASS(Polyline3D);

// *****************************************************************************
//! \brief
// *****************************************************************************
class Polyline3D: public Node3D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //----------------------------------------------------------------------------
  Polyline3D(std::string const& name, Mode const mode, Material_SP material)
    : Node3D(name, true),
      m_mode(mode),
      m_material(material),
      m_program(m_material->program()),
      m_vao("VAO_" + name),
      m_vertices(m_vao.vector3f("position")),
      m_colors(m_vao.vector4f("colors")),
      m_indices(m_vao.index32())
  {
    throwIfOpenGLClassCalledBeforeContext();

    DEBUG("%s", "<<<---------- NewPolyline+Material ---------------------------------------------");

    if (unlikely(!m_program.compile()))
      throw OpenGLException(m_program.getError());

    m_program.bind(m_vao);
    model() = Matrix44f(matrix::Identity);

    DEBUG("%s", "------------- NewPolyline+Material ------------------------------------------>>>");
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release VBOs.
  //----------------------------------------------------------------------------
  virtual ~Polyline3D()
  {
    ERROR("Destroying polyline '%s'", cname());
  }

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
  //! \brief Return the reference of the VBO holding vertices positions.
  //! \return VBO holding vertex positions of the model.
  //----------------------------------------------------------------------------
  inline GLVertexBuffer<Vector3f>& vertices()
  {
    return m_vertices;
  }

  inline GLVertexBuffer<Vector4f>& colors()
  {
    return m_colors;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the reference of the index of VBOs (EBO).
  //! \return the reference of the EBO.
  //----------------------------------------------------------------------------
  inline GLIndexBuffer32& index()
  {
    return m_indices;
  }

  //----------------------------------------------------------------------------
  //! \brief Renderer the instance.
  //! \param[in] matrix the transformation matrix of the model.
  //! \note This method is functional if and only if isValid() returns true.
  //----------------------------------------------------------------------------
  inline void draw(Matrix44f const& matrix)
  {
    DEBUG("Draw Shape '%s'", cname());
    model() = matrix;
    draw();
  }

  //----------------------------------------------------------------------------
  //! \brief Renderer the instance.
  //! \note This method is functional if and only if isValid() returns true.
  //----------------------------------------------------------------------------
  inline void draw()
  {
    DEBUG("Draw Shape '%s'", cname());
    m_program.draw(m_vao, m_mode, index());
  }

  static Polyline3D_SP create(std::string const& name, Mode const mode, Material_SP material)
  {
    return std::make_shared<Polyline3D>(name, mode, material);
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

  Mode                      m_mode;
  Material_SP               m_material;
  GLProgram&                m_program;
  GLVAO                     m_vao;
  GLVertexBuffer<Vector3f>& m_vertices;
  GLVertexBuffer<Vector4f>& m_colors;
  GLIndexBuffer32&          m_indices;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_POLYLINE_HPP
