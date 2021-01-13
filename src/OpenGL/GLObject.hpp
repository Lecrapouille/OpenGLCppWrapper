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

#  include "OpenGL/Context/OpenGL.hpp"

// ***************************************************************************
//! \class GLObject GLObject.hpp
//! \ingroup OpenGL
//!
//! \brief GLObject is an interface and ancestor class for wrapping OpenGL
//! objects such as VAO, VBO, Uniform, Attribute, Shader, Program, Texture,
//! FrameBuffer.
//!
//! This class implements the template method pattern through public methods
//! begin() and end() defining the skeleton of calling virtual methods such as
//! onCreate(), onActivate(), onSetup(), onUpdate(), onDeactivate() which have to be
//! implemented in derived classes.
//!
//! \tparam T is the type of the OpenGL handle: either GLenum (aka GLunit) or
//! GLint. C++ is strongly typed and OpenGL API not fully consistent on this
//! type and change depending on the object (ie GLUniform). T avoid to produce
//! signed vs. unsigned conversion compilation warnings or usage of explicit
//! static_cast.
//!
// ***************************************************************************
template<class T>
class GLObject : private NonCopyable
{
public:

    //--------------------------------------------------------------------------
    //! \brief Constructor with the name of the object. Initialize private
    //! states of the sate machine begin() calling methods onCreate(), onActivate(),
    //! onSetup(), onUpdate().
    //!
    //! \param[in] name is the name of the instance. It is mainly used as key in
    //! lookup tables (such as GLProgram). It is also used in logs for debug
    //! purpose.
    //! \param[in] target: the target type.
    //--------------------------------------------------------------------------
    GLObject(std::string const& name, const GLenum target = 0u)
        : m_name(name), m_target(target)
    {}

    //--------------------------------------------------------------------------
    //! \brief Virtual destructor because of pure virtual methods.
    //--------------------------------------------------------------------------
    virtual ~GLObject() = default;

    //--------------------------------------------------------------------------
    //! \brief Return the OpenGL object identifier that this instance wraps.
    //--------------------------------------------------------------------------
    inline T handle() const
    {
        return m_handle;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const reference of the name of this instance.
    //--------------------------------------------------------------------------
    inline const std::string& name() const
    {
        return m_name;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the const char* of the name of this instance.
    //--------------------------------------------------------------------------
    inline const char* cname() const
    {
        return m_name.c_str();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the target type of the wrapped OpenGL object.
    //--------------------------------------------------------------------------
    inline GLenum target() const
    {
        return m_target;
    }

    //--------------------------------------------------------------------------
    //! \brief Give the information if the object has been created and exists in
    //! the GPU (made by the method onCreate()).
    //!
    //! \note Do not confuse with the method needSetup(), indeed the onCreate() is
    //! for GPU allocations (like C malloc but for GPU) while onSetup() configures
    //! the object behavior.
    //!
    //! \return false if the object has been created by the GPU and does not be
    //! created again. Return true if the GPU has not yet been created or the
    //! creation failed previously and onCreate() needs to be called again.
    //--------------------------------------------------------------------------
    virtual inline bool needCreate() const
    {
        return m_need_create;
    }

    //--------------------------------------------------------------------------
    //! \brief Give the information if the object has been setup by the GPU
    //! (made by the method onSetup()).
    //!
    //! \note Do not confuse with the method needCreate(), indeed the onCreate()
    //! is for GPU allocations (like C malloc but for GPU) while onSetup()
    //! configures the object behavior.
    //!
    //! \return false if the object has been setup by the GPU and does not be
    //! setup again. Return true if the GPU has yet not setup the OpenGL obejct
    //! or if setup has failed previously and onSetup() needs to be called again.
    //--------------------------------------------------------------------------
    virtual inline bool needSetup() const
    {
        return m_need_setup;
    }

    //--------------------------------------------------------------------------
    //! \brief Give the information if data of the object has been
    //! transferred to the GPU (made when the method onUpdate() is called).
    //!
    //! Example vertices position of a VBO has been changed. They are
    //! transferred to the GPU, the model is now updated.
    //!
    //! \return false if the object has been transferred to the GPU and
    //! does not need to transferred again. Return true if the object has
    //! to be transferred again.
    //--------------------------------------------------------------------------
    virtual inline bool needUpdate() const
    {
        return m_need_update;
    }

    //--------------------------------------------------------------------------
    //! \brief Template method pattern. Perform some pending operations
    //! (creation, setup, update) when needed. This method is kind of state
    //! machine and only calls pure virtual methods such as onCreate(), onSetup(),
    //! onUpdate() ... which have to be implemented in derived classes.
    //--------------------------------------------------------------------------
    void begin()
    {
        if (unlikely(needCreate()))
        {
            // OpenGL context shall be present
            if (!likely(GL::Context::isCreated()))
                return ;

            // Usually call glCreateXXX() or glXXX()
            m_need_create = onCreate();
        }

        // Usually call glBindXXX()
        onActivate();

        // Configure (shaders, textures, VBOs ...)
        if (unlikely(needSetup()))
        {
            m_need_setup = onSetup();
            if (unlikely(m_need_setup))
            {
                return ;
            }
        }

        // Transfer CPU data to GPU data (texture, VBOs ...)
        if (unlikely(needUpdate()))
        {
            m_need_update = onUpdate();
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Deactivate the object on the GPU.
    //--------------------------------------------------------------------------
    inline void end()
    {
        if (likely(m_handle > initialHandleValue()))
        {
            onDeactivate();
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Release the memory allocated only for GPU, states of the instance
    //! wrapping the OpenGL are reseted. This means the instance is still reusable.
    //!
    //! \note the OpenGL object will be created back after a call of begin().
    //--------------------------------------------------------------------------
    void release()
    {
        if (likely(GL::Context::isCreated()))
        {
            if (likely(m_handle > initialHandleValue()))
            {
                onDeactivate();
                onRelease();
            }
        }

        m_handle = initialHandleValue();
        m_target = 0U;
        m_need_setup = true;
        m_need_create = true;
        m_need_update = false;
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Return the reseted value of the handle of the OpenGL
    //! object.
    //!
    //! Because the OpenGL API is sometimes not fully consistent, the handle
    //! type may change depending on OpenGL objects as therefore its initial
    //! value. This method makes all behaviors consistent.
    //--------------------------------------------------------------------------
    inline T initialHandleValue() const;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Allocate resources on the GPU.
    //!
    //! \return false if the object has been created by the GPU and does
    //! not be created again. Return true if the GPU failed to create
    //! the object and onCreate() needs to be called again.
    //--------------------------------------------------------------------------
    virtual bool onCreate() = 0;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Activate the object on the GPU.
    //--------------------------------------------------------------------------
    virtual void onActivate() = 0;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Configure the object behvior on the GPU.
    //!
    //! \return false if the object has been setup by the GPU and does
    //! not be setup again. Return true if the GPU failed to setup
    //! the object and onSetup() needs to be called again.
    //--------------------------------------------------------------------------
    virtual bool onSetup() = 0;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Update the object on the GPU.
    //!
    //! \return false if the object has been transferred to the GPU and
    //! does not need to transferred again. Return true if the object has
    //! to be transferred again.
    //--------------------------------------------------------------------------
    virtual bool onUpdate() = 0;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Deactivate the object on the GPU.
    //--------------------------------------------------------------------------
    virtual void onDeactivate() = 0;

    //--------------------------------------------------------------------------
    //! \brief Pure virtual. Delete the object from GPU memory.
    //--------------------------------------------------------------------------
    virtual void onRelease() {};

protected:

    //! \brief Object name used as the same time as a key for lookup tables and
    //! for debuging (logs, ...)
    std::string m_name;
    //! \brief hold the information if the OpenGL object has to do its setup.
    bool m_need_setup = true;
    //! \brief hold the information if the OpenGL object has to be created.
    bool m_need_create = true;
    //! \brief hold the information if the OpenGL object has to be transfered to
    //! the GPU.
    bool m_need_update = false;

protected:

    //! \brief Hold the identifier of the OpenGL object.
    //! \tparam can be either GLint or GLenum (aka for GLuint).
    T m_handle = initialHandleValue();
    //! \brief the type of object on the GPU.
    GLenum m_target;
};

//--------------------------------------------------------------------------
template<>
inline GLenum GLObject<GLenum>::initialHandleValue() const { return 0u; }

//--------------------------------------------------------------------------
template<>
inline GLint GLObject<GLint>::initialHandleValue() const { return -1; }

#endif // OPENGLCPPWRAPPER_GLOBJECT_HPP
