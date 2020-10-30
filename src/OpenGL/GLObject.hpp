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

#ifndef OPENGLCPPWRAPPER_GLOBJECT_HPP
#  define OPENGLCPPWRAPPER_GLOBJECT_HPP

#  include "OpenGL/OpenGL.hpp"

namespace glwrap
{

  // *****************************************************************************
  //! \class GLObject GLObject.hpp
  //! \ingroup OpenGL
  //!
  //! \brief GLObject is an interface and ancestor class for wrapping OpenGL
  //! objects such as VAO, VBO, Uniform, Attribute, Shader, Program, Texture,
  //! FrameBuffer.
  //!
  //! This class implements the template method pattern through public methods
  //! begin() and end() defining the skeleton of calling virtual methods such as
  //! create(), activate(), setup(), update(), deactivate() which have to be
  //! implemented in derived classes.
  //!
  //! \tparam T is the type of the OpenGL handle: either GLenum (aka GLunit) or
  //! GLint. C++ is strongly typed and OpenGL API not fully consistent on this
  //! type and change depending on the object (ie GLUniform). T avoid to produce
  //! signed vs. unsigned conversion compilation warnings or usage of explicit
  //! static_cast.
  //!
  // *****************************************************************************
  template<class T>
  class GLObject : private NonCopyable
  {
  public:

    //----------------------------------------------------------------------------
    //! \brief Constructor with the name of the object. Initialize private
    //! states of the sate machine begin() calling methods create(), activate(),
    //! setup(), update().
    //!
    //! \param[in] name is the name of the instance. It is mainly used as key in
    //! lookup tables (such as GLProgram). It is also used in logs for debug
    //! purpose.
    //----------------------------------------------------------------------------
    GLObject(std::string const& name)
      : m_name(name)
    {
      resetStates();
    }

    //----------------------------------------------------------------------------
    //! \brief Virtual destructor because of pure virtual methods.
    //----------------------------------------------------------------------------
    virtual ~GLObject()
    {}

    //----------------------------------------------------------------------------
    //! \brief Return the OpenGL identifier of the object that this class wraps.
    //----------------------------------------------------------------------------
    inline T handle() const
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
    //! \brief Return the target type of the wrapped OpenGL object.
    //----------------------------------------------------------------------------
    inline GLenum target() const
    {
      return m_target;
    }

    //----------------------------------------------------------------------------
    //! \brief Give the information if the object has been created and exists in
    //! the GPU (made by the method create()).
    //!
    //! \note Do not confuse with the method needSetup(), indeed the create() is
    //! for GPU allocations (like C malloc but for GPU) while setup() configures
    //! the object behavior.
    //!
    //! \return false if the object has been created by the GPU and does not be
    //! created again. Return true if the GPU has yet not been created or its
    //! creation failed and create() needs to be called again.
    //----------------------------------------------------------------------------
    virtual inline bool needCreate() const
    {
      return m_need_create;
    }

    //----------------------------------------------------------------------------
    //! \brief Give the information if the object has been setup and by the GPU
    //! (made by the method setup()).
    //!
    //! \note Do not confuse with the method needCreate(), indeed the create()
    //! is for GPU allocations (like C malloc but for GPU) while setup()
    //! configures the object behavior.
    //!
    //! \return false if the object has been setup by the GPU and does not be
    //! setup again. Return true if the GPU has yet not setup the OpenGL obejct
    //! or if setup has failed setup() needs to be called again.
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
    //!
    //! \fixme not in double with isValid() ?
    //----------------------------------------------------------------------------
    virtual inline bool needUpdate() const
    {
      return m_need_update;
    }

    //----------------------------------------------------------------------------
    //! \brief Template method pattern. Perform some pending operations
    //! (creation, setup, update) when needed. This method is kind of state
    //! machine and only calls pure virtual methods such as create(), setup(),
    //! update() ... which have to be implemented in derived classes.
    //----------------------------------------------------------------------------
    void begin()
    {
      if (unlikely(needCreate()))
        {
          // OpenGL context shall be present
          if (unlikely(!isContextCreated()))
            {
              ERROR("OpenGL context is not yet created");
              return ;
            }

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
      /*if (unlikely(!isContextCreated()))
        return ;*/

      deactivate();
    }

    //----------------------------------------------------------------------------
    //! \brief Release the memory allocated only for GPU, states of the instance
    //! wrapping the OpenGL are reseted. This means the instance is still reusable.
    //!
    //! \note the OpenGL object will be created back after a call of begin().
    //----------------------------------------------------------------------------
    void free()
    {
      destroy();
      resetStates();
      onReset();
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
    //! \brief Force redoing setup() for the next call of begin().
    //----------------------------------------------------------------------------
    inline void redoSetup()
    {
      m_need_setup = true;
    }

    //----------------------------------------------------------------------------
    //! \brief Force redoing update() for the next call of begin().
    //----------------------------------------------------------------------------
    inline void redoUpdate()
    {
      m_need_update = true;
    }

    //----------------------------------------------------------------------------
    //! \brief Release the memory allocated on CPU and GPU.
    //!
    //! \note the OpenGL object will be created back if the begin()
    //! again.
    //----------------------------------------------------------------------------
    void destroy()
    {
      if (likely(isContextCreated()))
        {
          if (likely(isValid()))
            {
              deactivate();
              release();
            }
        }
    }

    //----------------------------------------------------------------------------
    //! \brief Release the memory allocated on CPU and GPU. By default do nothing.
    //----------------------------------------------------------------------------
    virtual void onReset()
    {}

  private:

    //----------------------------------------------------------------------------
    //! \brief Reset the states of the class: this will force creating an OpenGL
    //! object, its setup it and no transfer to the GPU.
    //----------------------------------------------------------------------------
    void resetStates()
    {
      m_handle = initialHandleValue();
      m_target = 0U;
      m_need_setup = true;
      m_need_create = true;
      m_need_update = false;
    }

    //----------------------------------------------------------------------------
    //! \brief Return the reseted value of the handle of the OpenGL
    //! object.
    //!
    //! Because the OpenGL API is sometimes not fully consistent, the handle
    //! type may change depending on OpenGL objects as therefore its initial
    //! value. This method makes all behaviors consistent.
    //----------------------------------------------------------------------------
    inline T initialHandleValue() const;

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

  protected:

    //! \brief Object name used as the same time as a key for lookup tables and
    //! for debuging (logs, ...)
    std::string m_name;

  private:

    //! \brief hold the information if the OpenGL object has to do its setup.
    bool m_need_setup;
    //! \brief hold the information if the OpenGL object has to be created.
    bool m_need_create;
    //! \brief hold the information if the OpenGL object has to be transfered to
    //! the GPU.
    bool m_need_update;

  protected:

    //! \brief Hold the identifier of the OpenGL object.
    //! \tparam can be either GLint or GLenum (aka for GLuint).
    T m_handle;
    //! \brief the type of object on the GPU.
    GLenum m_target;
  };

  //----------------------------------------------------------------------------
  template<>
  inline GLenum GLObject<GLenum>::initialHandleValue() const { return 0u; }

  //----------------------------------------------------------------------------
  template<>
  inline GLint GLObject<GLint>::initialHandleValue() const { return -1; }

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLOBJECT_HPP
