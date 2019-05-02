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
// OpenGLCppWrapper is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_IGLOBJECT_HPP
#  define OPENGLCPPWRAPPER_IGLOBJECT_HPP

#  include "OpenGL.hpp"

namespace glwrap
{

// *****************************************************************************
//! \class IGLObject IGLObject.hpp
//!
//! \brief IGLObject interface and ancestor class for others classes
//! wrapping OpenGL objects such as VAO, VBO, Program, Texture.
//!
//! T is the type of the OpenGL handle (either GLenum or GLint). C++
//! is strongly typed and OpenGL API not very consistent (handle type
//! changes depending on the object). T avoid to produce signed
//! vs. unsigned conversion compilation warnings or usage of explicit
//! static_cast.
// *****************************************************************************
template<class T>
class IGLObject : private NonCopyable
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor with the name of the object. Initialize
  //! private states of the internal sate machine.
  //!
  //! \param name is the name of the instance. It is mainly used as
  //! key in lookup tables (such as GLProgram). It is also used in
  //! logs for debug purpose.
  //----------------------------------------------------------------------------
  IGLObject(std::string const& name)
    : m_name(name)
  {
    init();
  }

  //----------------------------------------------------------------------------
  //! \brief Virtual destructor because of pure virtual methods.
  //----------------------------------------------------------------------------
  virtual ~IGLObject() {};

  //----------------------------------------------------------------------------
  //! \brief Return the OpenGL identifier of the object that this
  //! class wraps.
  //----------------------------------------------------------------------------
  inline T gpuID() const
  {
    return m_handle;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the const reference of the name of the object.
  //----------------------------------------------------------------------------
  inline const std::string& name() const
  {
    return m_name;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the const char* of the name of the object.
  //----------------------------------------------------------------------------
  inline const char* cname() const
  {
    return m_name.c_str();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the type of OpenGL object.
  //----------------------------------------------------------------------------
  inline GLenum target() const
  {
    return m_target;
  }

  //----------------------------------------------------------------------------
  //! \brief Give the information if the object has been created and
  //! exists in the GPU (its creation is made when the method create()
  //! is called).
  //!
  //! \note Do not confuse with the method needSetup(), indeed the
  //! create() is for GPU allocations (like C malloc but for GPU)
  //! while setup() configures the object behavior.
  //!
  //! \return false if the object has been created by the GPU and does
  //! not be created again. Return true if the GPU failed to create
  //! the object and create() needs to be called again.
  //----------------------------------------------------------------------------
  virtual inline bool needCreate() const
  {
    return m_need_create;
  }

  //----------------------------------------------------------------------------
  //! \brief Give the information if the object has been setup and
  //! by the GPU (its setup is made when the method setup()
  //! is called).
  //!
  //! \note Do not confuse with the method needCreate(), indeed the
  //! create() is for GPU allocations (like C malloc but for GPU)
  //! while setup() configures the object behavior.
  //!
  //! \return false if the object has been setup by the GPU and does
  //! not be setup again. Return true if the GPU failed to setup
  //! the object and setup() needs to be called again.
  //----------------------------------------------------------------------------
  virtual inline bool needSetup() const
  {
    return m_need_setup;
  }

  //----------------------------------------------------------------------------
  //! \brief Give the information if data of the object has been
  //! transferred to the GPU (made when the method update() is called).
  //!
  //! Example vertices position of a VBO has been changed. They are
  //! transferred to the GPU, the model is now updated.
  //!
  //! \return false if the object has been transferred to the GPU and
  //! does not need to transferred again. Return true if the object has
  //! to be transferred again.
  //----------------------------------------------------------------------------
  virtual inline bool needUpdate() const
  {
    return m_need_update;
  }

  //----------------------------------------------------------------------------
  //! \brief Return if the OpenGL object has been created by the
  //! GPU and can be released by the GPU.
  //!
  //! \note this does not necessary means that the GPU will destroyed
  //! the object. It only means that the GPU can do it. It is
  //! equivalent to C code if (pointer != NULL).
  //!
  //! \return true if the GPU can release the object.
  //----------------------------------------------------------------------------
  virtual inline bool canBeReleased() const
  {
    return false == m_need_create;
  }

  //----------------------------------------------------------------------------
  //! \brief Activate the object on the GPU. Perform some pending
  //! operations (creation setup, update) when needed. This method is
  //! kind of state machine and only calls pure virtual methods (such
  //! as create(), setup(), update() ...) which have to be implemented
  //! in derived classes.
  //! \throw OpenGLException Override methods may throw this exception.
  //----------------------------------------------------------------------------
  void begin()
  {
    /* FIXME if (unlikely(!hasCreatedContext()))
       {
       DEBUG("O::begin perdu");
       return ;
       }
    */

    if (likely(needCreate()))
      {
        m_need_create = create();
      }

    if (likely(isValid()))
      {
        activate();

        if (unlikely(needSetup()))
          {
            m_need_setup = setup();
            if (unlikely(m_need_setup))
              {
                return ;
              }
          }

        if (unlikely(needUpdate()))
          {
            m_need_update = update();
          }
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Deactivate the object on the GPU.
  //----------------------------------------------------------------------------
  inline void end()
  {
    /*if (unlikely(!hasCreatedContext()))
      return ;*/

    deactivate();
  }

  //----------------------------------------------------------------------------
  //! \brief Release the memory allocated on CPU and GPU.
  //!
  //! \note the OpenGL object will be created back if the begin()
  //! again.
  //----------------------------------------------------------------------------
  virtual void destroy()
  {
    if (hasCreatedContext())
      {
        if (isValid())
          {
            deactivate();
            release();
          }
      }
    init();
  }

  //----------------------------------------------------------------------------
  //! \brief Check if object is a valid OpenGL object (meaning which
  //! has created with success by the GPU).
  //----------------------------------------------------------------------------
  inline bool isValid() const
  {
    return m_handle > initialHandleValue();
  }

protected:

  //----------------------------------------------------------------------------
  //! \brief Force redoing setup()
  //----------------------------------------------------------------------------
  inline void redoSetup()
  {
    m_need_setup = true;
  }

  //----------------------------------------------------------------------------
  //! \brief Force redoing update()
  //----------------------------------------------------------------------------
  inline void forceUpdate()
  {
    m_need_update = true;
  }

  //----------------------------------------------------------------------------
  //! \brief Force redoing setup()
  //----------------------------------------------------------------------------
  inline void forceSetup()
  {
    m_need_setup = true;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Return the reset value on OpenGL reference
  //! object. Because the OpenGL API is not fully consistent, handle
  //! type changes depending on OpenGL objects as well as initial
  //! value.
  //----------------------------------------------------------------------------
  inline T initialHandleValue() const;

  //----------------------------------------------------------------------------
  //! \brief Reset the states of the class: this will force creating
  //! an OpenGL object, its setup it and no transfer to the GPU.
  //----------------------------------------------------------------------------
  void init()
  {
    m_handle = initialHandleValue();
    m_target = 0U;
    m_need_setup = true;
    m_need_create = true;
    m_need_update = false;
  }

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Allocate resources on the GPU.
  //!
  //! \return false if the object has been created by the GPU and does
  //! not be created again. Return true if the GPU failed to create
  //! the object and create() needs to be called again.
  //----------------------------------------------------------------------------
  virtual bool create() = 0;

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Activate the object on the GPU.
  //----------------------------------------------------------------------------
  virtual void activate() = 0;

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Configure the object behvior on the GPU.
  //!
  //! \return false if the object has been setup by the GPU and does
  //! not be setup again. Return true if the GPU failed to setup
  //! the object and setup() needs to be called again.
  //----------------------------------------------------------------------------
  virtual bool setup() = 0;

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Update the object on the GPU.
  //!
  //! \return false if the object has been transferred to the GPU and
  //! does not need to transferred again. Return true if the object has
  //! to be transferred again.
  //----------------------------------------------------------------------------
  virtual bool update() = 0;

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Deactivate the object on the GPU.
  //----------------------------------------------------------------------------
  virtual void deactivate() = 0;

  //----------------------------------------------------------------------------
  //! \brief Pure virtual. Delete the object from GPU memory.
  //----------------------------------------------------------------------------
  virtual void release() = 0;

private:

  //! \brief Object name for keys in lookup tables.
  std::string m_name;
  //! \brief hold the information if the OpenGL object has to do its
  //! setup.
  bool m_need_setup;
  //! \brief hold the information if the OpenGL object has to be
  //! created.
  bool m_need_create;
  //! \brief hold the information if the OpenGL object has to be
  //! transfered to the GPU.
  bool m_need_update;

protected:

  //! \brief Hold the identifier of the OpenGL object.
  T m_handle;
  //! \brief the type of object on the GPU.
  GLenum m_target;
};

//----------------------------------------------------------------------------
template<>
inline GLenum IGLObject<GLenum>::initialHandleValue() const { return 0u; }

//----------------------------------------------------------------------------
template<>
inline GLint IGLObject<GLint>::initialHandleValue() const { return -1; }

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_IGLOBJECT_HPP
