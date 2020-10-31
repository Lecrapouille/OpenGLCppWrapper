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
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
#  define OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP

#  include "OpenGL/Uniform.hpp"
#  include "OpenGL/Buffers.hpp"
#  include "OpenGL/Texture1D.hpp"
#  include "OpenGL/Texture2D.hpp"
#  include "OpenGL/Texture3D.hpp"
#  include "Math/Vector.hpp"
#  include "Common/Any.hpp"

class GLProgram;

class GLVAO: public GLObject<GLenum>
{
    //! \brief GLProgram directly modifies GLVAO states.
    friend class GLProgram;

public:

    //--------------------------------------------------------------------------
    //! \brief Constructor. Give a name to the instance. This name is mainly used
    //! for logs as debug purpose. This constructor makes no other actions.
    //!
    //! \param[in] name the name of the VAO instance.
    //--------------------------------------------------------------------------
    GLVAO(std::string const& name)
        : GLObject(name)
    {
        m_target = GL_ARRAY_BUFFER;
    }

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release elements in CPU and GPU memories.
    //--------------------------------------------------------------------------
    virtual ~GLVAO() override
    {
        release();
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline bool isBound(GLenum const prog_id) const
    {
        return m_prog_id == prog_id;
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    inline bool isBound() const
    {
        return m_prog_id != 0u;
    }

    //--------------------------------------------------------------------------
    //! \brief Check if this instance has VBOs.
    //!
    //! \note Having no VBO generally means that this instance of VAO have not yet
    //! been bound to a GLProgram. See the bind() method.
    //!
    //! \return true if this instance has VBOs.
    //--------------------------------------------------------------------------
    inline bool hasVBOs() const
    {
        return 0_z != m_vbos.size();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 4D vector of type
    //! float.
    //!
    //! This method wraps the \a VBO() method hidding the misery of the template.
    //!
    //! \throw OpenGLException if the VBO does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLVertexBuffer<Vector4f>& vector4f(const char *name)
    {
        return getVBO<Vector4f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 3D vector of type
    //! float.
    //!
    //! This method wraps the \a VBO() method hidding the misery of the template.
    //!
    //! \throw OpenGLException if the VBO does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLVertexBuffer<Vector3f>& vector3f(const char *name)
    {
        return getVBO<Vector3f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 2D vector of type
    //! float.
    //!
    //! This method wraps the \a VBO() method hidding the misery of the template.
    //!
    //! \throw OpenGLException if the VBO does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLVertexBuffer<Vector2f>& vector2f(const char *name)
    {
        return getVBO<Vector2f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a float scalar.
    //!
    //! This method wraps the \a VBO() method hidding the misery of the template.
    //!
    //! \throw OpenGLException if the VBO does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLVertexBuffer<float>& scalarf(const char *name)
    {
        return getVBO<float>(name);
    }

private:

    void init(GLProgram& prog, BufferUsage const usage, size_t const vbo_size);

    //--------------------------------------------------------------------------
    //! \brief Create a new OpenGL VAO.
    //--------------------------------------------------------------------------
    virtual bool onCreate() override
    {
        glCheck(glGenVertexArrays(1, &m_handle));
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Bind the VAO to OpenGL.
    //--------------------------------------------------------------------------
    virtual void onActivate() override
    {
        glCheck(glBindVertexArray(m_handle));
    }

    //--------------------------------------------------------------------------
    //! \brief Dummy method. Nothing is made.
    //--------------------------------------------------------------------------
    virtual bool onSetup() override
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Dummy method. Nothing is made.
    //--------------------------------------------------------------------------
    virtual bool onUpdate() override
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //! \brief Unbind the VAO from OpenGL.
    //--------------------------------------------------------------------------
    virtual void onDeactivate() override
    {
        glCheck(glBindVertexArray(0U));
    }

    //--------------------------------------------------------------------------
    //! \brief Destroy the OpenGL VAO.
    //--------------------------------------------------------------------------
    virtual void onRelease() override
    {
        glCheck(glDeleteVertexArrays(1, &m_handle));
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<class T>
    GLVertexBuffer<T>& getVBO(const char *name)
    {
        if (unlikely(nullptr == name))
            throw GL::Exception("nullptr passed to uniform");

        if (likely(isBound()))
        {
            try
            {
                return *(m_vbos.get<std::shared_ptr<GLVertexBuffer<T>>>(name));
            }
            catch (std::exception&)
            {
                throw GL::Exception("GLUniform '" + std::string(name) + "' does not exist");
            }
        }
        //else
        //{
        //    return m_vboss.add<GLVertexBuffer<T>>(name, T{});
        //}
    }

private:

    using VBOs = Any;
    using Textures = Any;

    VBOs     m_vbos;
    Textures m_textures;
    GLenum   m_prog_id = 0;
};

#endif // OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
