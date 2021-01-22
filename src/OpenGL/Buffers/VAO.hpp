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

#  include "OpenGL/Buffers/VBO.hpp"
#  include "OpenGL/Textures/Textures.hpp"
#  include "OpenGL/Shaders/Program.hpp"

//--------------------------------------------------------------------------
//! \brief Mode for drawing primitives (points, lines, triangles ...)
//--------------------------------------------------------------------------
enum class Mode : GLenum
{
    /* 0x0000 */ POINTS = GL_POINTS,
    /* 0x0001 */ LINES = GL_LINES,
    /* 0x0002 */ LINE_LOOP = GL_LINE_LOOP,
    /* 0x0003 */ LINE_STRIP = GL_LINE_STRIP,
    /* 0x0004 */ TRIANGLES = GL_TRIANGLES,
    /* 0x0005 */ TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    /* 0x0006 */ TRIANGLE_FAN = GL_TRIANGLE_FAN,
    /* 0x000A */ LINES_ADJACENCY = GL_LINES_ADJACENCY,
    /* 0x000B */ LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
    /* 0x000C */ TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    /* 0x000D */ TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
    /* 0x000E */ PATCHES = GL_PATCHES,
};

//******************************************************************************
//! \brief Class Wrapping OpenGL VAO
//******************************************************************************
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
    //! \param[in] usage for VBOs when they are created:
    //!   - BufferUsage::STREAM_DRAW: The data store contents will be modified once and
    //!     used at most a few times.
    //!   - BufferUsage::STATIC_DRAW: The data store contents will be modified once and
    //!     used many times.
    //!   - BufferUsage::DYNAMIC_DRAW: The data store contents will be modified repeatedly
    //!     and used many times.
    //! \param[in] reserve the number of elements when creating VBOs.
    //--------------------------------------------------------------------------
    GLVAO(std::string const& name, BufferUsage const usage = BufferUsage::DYNAMIC_DRAW,
          size_t const reserve = 3u)
        : GLObject(name, GL_ARRAY_BUFFER)
    {
        m_usage = usage;
        m_reserve = reserve;
    }

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release elements in CPU and GPU memories.
    //--------------------------------------------------------------------------
    virtual ~GLVAO() override
    {
        release();
    }

    //--------------------------------------------------------------------------
    //! \brief Set the usage for VBOs when they are created:
    //!   - BufferUsage::STREAM_DRAW: The data store contents will be modified once and
    //!     used at most a few times.
    //!   - BufferUsage::STATIC_DRAW: The data store contents will be modified once and
    //!     used many times.
    //!   - BufferUsage::DYNAMIC_DRAW: The data store contents will be modified repeatedly
    //!     and used many times.
    //--------------------------------------------------------------------------
    void usage(BufferUsage const usage)
    {
        m_usage = usage;
    }

    //--------------------------------------------------------------------------
    //! \brief Set the number of elements to reserve when creating VBOs.
    //--------------------------------------------------------------------------
    void reserve(size_t const reserve)
    {
        m_reserve = reserve;
    }

    //--------------------------------------------------------------------------
    //! \brief Wrap the glDrawArrays() function
    //--------------------------------------------------------------------------
    bool draw(Mode const mode, size_t const first, size_t const count);

    //--------------------------------------------------------------------------
    //! \brief Wrap the glDrawArrays() function
    //--------------------------------------------------------------------------
    inline bool draw(Mode const mode = Mode::TRIANGLES, size_t const first = 0u)
    {
        return GLVAO::draw(mode, first, m_count);
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
        return 0_z != m_VBOs.size();
    }

    //--------------------------------------------------------------------------
    template<class T>
    bool has(const char *name)
    {
        if (nullptr == name)
            return false;

        return m_VBOs.has<std::shared_ptr<GLVertexBuffer<T>>>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Check if this instance has textures.
    //!
    //! \note Having no textures generally means that the shader has no sampler or
    //! this instance of VAO have not yet been bound to a GLProgram. See the bind()
    //! method.
    //!
    //! \return true if this instance has textures.
    //--------------------------------------------------------------------------
    inline bool hasTextures() const
    {
        return 0_z != m_textures.size();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the list of VBO names. VBO names come from names of
    //! attributes variables used inside GLSL shaders. This method is mainly
    //! used for debug purpose.
    //!
    //! \note if the VAO has never been bound to a GLProgram this method will
    //! return an empty list.
    //!
    //! \param[in,out] list the list where to insert VBO names.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getVBONames(std::vector<std::string> &list, bool const clear = true) const;

    //--------------------------------------------------------------------------
    //! \brief Return the list of sampler names. Samplers come from names of
    //! uniform variables used in GLSL shaders. This is method is mainly used for
    //! debug purpose.
    //!
    //! \note if the VAO has never been bound to a GLProgram this method will
    //! return an empty list.
    //!
    //! \param[in,out] list the list where to insert sampler names.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \note Do not be confused with the sense of 'sampler name'. We do not refer
    //! to the name of the jpeg, png or bmp file. We refer to the shader sampler
    //! name (GLSL code). The GLTexture holds the acces of the file. To access to
    //! it do this VAO.texture2D["sampler_name"].name().
    //!
    //! \todo: get the tuple sampler name and texture name ?
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getTexturesNames(std::vector<std::string>& list, bool const clear = true) const;
    size_t getUnloadedTextures(std::vector<std::string>& list, bool const clear = true) const;

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

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 1D texture.
    //!
    //! This method wraps the \a texture() method hidding the misery of the
    //! template.
    //!
    //! \throw OpenGLException if the texture does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLTexture1D& texture1D(const char *name)
    {
        return getTexture<GLTexture1D>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 2D texture.
    //!
    //! This method wraps the \a texture() method hidding the misery of the
    //! template.
    //!
    //! \throw OpenGLException if the texture does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLTexture2D& texture2D(const char *name)
    {
        return getTexture<GLTexture2D>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a 3D texture.
    //!
    //! This method wraps the \a texture() method hidding the misery of the
    //! template.
    //!
    //! \throw OpenGLException if the texture does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLTexture3D& texture3D(const char *name)
    {
        return getTexture<GLTexture3D>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the reference of the named VBO holding a cubic texture.
    //!
    //! This method wraps the \a texture() method hidding the misery of the
    //! template.
    //!
    //! \throw OpenGLException if the texture does not exist or does not have the
    //! correct type.
    //--------------------------------------------------------------------------
    inline GLTextureCube& textureCube(const char *name)
    {
        return getTexture<GLTextureCube>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Return true if this instance of VAO is bound to a GLProgram
    //--------------------------------------------------------------------------
    inline bool isBound() const
    {
        return (m_program != nullptr) && (m_program->handle() != 0u);
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Return true if this instance of VAO is bound to the given GLProgram
    //--------------------------------------------------------------------------
    inline bool isBoundTo(GLenum const prog_id) const
    {
        return (m_program != nullptr) && (m_program->handle() == prog_id);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the number of elements in buffers.
    //--------------------------------------------------------------------------
    inline size_t count() const
    {
        return m_count;
    }

    //--------------------------------------------------------------------------
    //! \brief Populate VBOs
    //--------------------------------------------------------------------------
    void createTexturesFromSamplers(GLProgram::Samplers const& samplers);
    void createVBOsFromAttribs(GLProgram::Attributes const& attributes);

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
    //! \brief
    //--------------------------------------------------------------------------
    virtual bool onUpdate() override;

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
    //! \brief All VBOs shall have the same number of elements
    //--------------------------------------------------------------------------
    bool checkVBOSizes();

    //--------------------------------------------------------------------------
    //! \brief Find and return a VBO. Create and store a VBO if and only if the
    //! VAO is not yet bound to a GLProgram.
    //--------------------------------------------------------------------------
    template<class T>
    GLVertexBuffer<T>& getVBO(const char *name)  // TODO const:  foo = getVBO(()
    {
        if (unlikely(nullptr == name))
            throw GL::Exception("nullptr passed to uniform");

        try
        {
            GLVertexBuffer<T>& vbo = *(m_VBOs.get<std::shared_ptr<GLVertexBuffer<T>>>(name));
            m_need_update = true;// TODO const:  foo = getVBO()
            //std::cout << "getVBO need update = true" << std::endl;

            return vbo;
        }
        catch (std::exception&)
        {
            if (likely(!isBound()))
            {
                auto vbo = std::make_shared<GLVertexBuffer<T>>(name, m_reserve, m_usage);
                m_VBOs.add(name, vbo);
                m_listBuffers[name] = vbo;

                return *vbo;
            }

            throw GL::Exception("GLUniform '" + std::string(name) + "' does not exist");
        }
    }

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    template<class T>
    T& getTexture(const char *name) // TODO const:  foo = getTexture()
    {
        if (unlikely(nullptr == name))
            throw GL::Exception("nullptr passed to uniform");

        if (likely(!isBound()))
        {
            m_textures.add(name, std::make_shared<T>(name));
        }

        try
        {
            m_need_update = true; // TODO gerer foo = getTexture()
            //std::cout << "getTexture need update = true" << std::endl;
            return *(m_textures.get<std::shared_ptr<T>>(name));
        }
        catch (std::exception&)
        {
            throw GL::Exception("GLTexture '" + std::string(name) + "' does not exist");
        }
    }

protected:

    using VBOs = Any;
    using Textures = Any;
    using ListBuffers = std::map<std::string, std::shared_ptr<IGLBuffer>>;
    using ListTextures = std::map<std::string, std::shared_ptr<GLTexture>>;

    VBOs         m_VBOs;
    Textures     m_textures;
    ListBuffers  m_listBuffers;
    ListTextures m_listTextures;
    GLProgram*   m_program = nullptr;
    size_t       m_count = 0u;

    BufferUsage  m_usage;
    size_t       m_reserve;
};

#endif // OPENGLCPPWRAPPER_GLVERTEX_ARRAY_HPP
