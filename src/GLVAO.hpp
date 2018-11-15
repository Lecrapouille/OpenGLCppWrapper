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
//
// This file is a derivated work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef GL_VERTEX_ARRAY_HPP_
#  define GL_VERTEX_ARRAY_HPP_

#  include "GLVBO.tpp"
#  include <map>
#  include <vector>

class GLVAO: public IGLObject<GLenum>
{
public:

  //! \brief Empty constructor without name
  GLVAO()
    : IGLObject()
  {
    prog = 0;
    m_target = GL_ARRAY_BUFFER;
  }

  //! \brief Constructor with the object name
  GLVAO(const char *name)
    : IGLObject(name)
  {
    prog = 0;
    m_target = GL_ARRAY_BUFFER;
  }

  virtual ~GLVAO() override
  {
    destroy();
  }

  //------------------------------------------------------------------
  //! \brief Return the list of VBO names
  //------------------------------------------------------------------
  std::vector<std::string> VBONames()
  {
    std::vector<std::string> list;
    list.reserve(m_vbos.size());
    for (auto& it: m_vbos)
      {
        list.push_back(it.first);
      }
    return list;
  }

  static void unbind()
  {
    glCheck(glBindVertexArray(0U));
  }

  //FIXME: private + friend GLProg
  //FIXME: ajouter nombre de vertex pour reserver taille
  //FIXME: gerer duplicata de noms ?
  template<typename T>
  bool createVBO(const char *name)
  {
    if (unlikely(hasVBO(name)))
      {
        LOGE("Try to create a VBO with name '%s' already used", name);
        return false;
      }
    m_vbos[name] = std::make_unique<GLVertexBuffer<T>>(name);
    LOGD("allocate new VBO '%s' %p", name, m_vbos[name].get());
    return true;
  }

  inline bool hasVBOs() const
  {
    return 0_z != m_vbos.size();
  }

  inline bool hasVBO(const char *name) const
  {
    return m_vbos.end() != m_vbos.find(name);
  }

  template<typename T>
  GLVertexBuffer<T>& VBO(const char *name)
  {
    if (unlikely(nullptr == name))
      {
        throw std::invalid_argument("nullptr passed to getVBO");
      }

    auto ptr = m_vbos[name].get();
    if (unlikely(nullptr == ptr))
      {
        throw std::out_of_range("GLVertexBuffer '" + std::string(name) +
                                "' does not exist");
      }

    GLVertexBuffer<T> *vbo = dynamic_cast<GLVertexBuffer<T>*>(ptr);
    if (unlikely(nullptr == vbo))
      {
        throw std::invalid_argument("GLVertexBuffer '" + std::string(name) +
                                    "' exists but has wrong template type");
      }

    LOGD("VAO::GetVBO '%s' %p", name, vbo);
    return *vbo;
  }

private:

  virtual bool create() override
  {
    LOGD("VAO '%s' create", name().c_str());
    glCheck(glGenVertexArrays(1, &m_handle));
    return false;
  }

  virtual void release() override
  {
    LOGD("VAO '%s' release", name().c_str());
    glCheck(glDeleteVertexArrays(1, &m_handle));
  }

  virtual void activate() override
  {
    LOGD("VAO '%s' activate", name().c_str());
    glCheck(glBindVertexArray(m_handle));
  }

  virtual void deactivate() override
  {
    LOGD("VAO '%s' deactivate", name().c_str());
    glCheck(glBindVertexArray(0U));
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

private:
public: //FIXME

  std::map<std::string, std::unique_ptr<IGLObject>> m_vbos;
  GLenum prog; // attached prog
};

#endif /* GL_VERTEX_ARRAY_HPP_ */
