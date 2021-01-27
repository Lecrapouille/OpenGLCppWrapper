//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef OPENGLCPPWRAPPER_GLPROGRAM_HPP
#  define OPENGLCPPWRAPPER_GLPROGRAM_HPP

#  include "OpenGL/Shaders/Shaders.hpp"
#  include "OpenGL/Variables/Attribute.hpp"
#  include "OpenGL/Variables/Uniform.hpp"
#  include "OpenGL/Variables/Samplers.hpp"
#  include "Common/Any.hpp"

class GLVAO;

// *****************************************************************************
//! \brief This class allows to track GLSL variables (uniforms and attributes)
//! from given shaders (vertex, fragment, geometry). It allows to populate VBOs
//! and textures to VAO instances bound to it.
// *****************************************************************************
class GLProgram: public GLObject<GLenum>
{
    friend class GLVAO;

    using Shaders = std::vector<GLShader*>;
    using Attributes = std::map<std::string, std::shared_ptr<GLAttribute>>;
    using Samplers = std::map<std::string, std::shared_ptr<GLSampler>>;
    using Uniforms = Any;
    using UniformLocations = std::map<std::string, std::shared_ptr<GLLocation>>;

public:

    //--------------------------------------------------------------------------
    //! \brief Constructor. Give a name to the instance. This constructor makes
    //! no other actions.
    //!
    //! \param[in] name the name of this instance (used for debug and traces).
    //--------------------------------------------------------------------------
    GLProgram(std::string const& name);

    //--------------------------------------------------------------------------
    //! \brief Destructor. Clear list of shaders, list of uniforms and list of
    //! attributes. Unbound last VAO. Clear errors.
    //--------------------------------------------------------------------------
    virtual ~GLProgram() override;

    //--------------------------------------------------------------------------
    //! \brief Compile a vertex shader and a fragment shader and populate list
    //! of uniforms and attributes.
    //!
    //! \note: this method is equivalent to the method GLObject::begin() but
    //! with a more explicit name and return the status of the compilation.
    //!
    //! \return true if the program has been successfully compiled, else return
    //! false. A internal error message is created in case of error that can be
    //! get with strerror().
    //--------------------------------------------------------------------------
    bool compile(GLVertexShader& vertex, GLFragmentShader& fragment);

    //--------------------------------------------------------------------------
    //! \brief Compile a vertex shader, a fragment and geometry shader and
    //! populate list of uniforms and attributes.
    //!
    //! \note: this method is equivalent to the method GLObject::begin() but
    //! with a more explicit name and return the status of the compilation.
    //!
    //! \return true if the program has been successfully compiled, else return
    //! false. A internal error message is created in case of error that can be
    //! get with strerror().
    //--------------------------------------------------------------------------
    bool compile(GLVertexShader& vertex, GLFragmentShader& fragment,
                 GLGeometryShader& geometry);

    //--------------------------------------------------------------------------
    //! \brief Check if attached shaders (passed to attachShaders() methods) have
    //! been compiled and linked with success.
    //!
    //! \return true if shaders have been compiled and linked successfuly, else
    //! return false.
    //--------------------------------------------------------------------------
    bool compiled() const
    {
        return !needSetup();
    }

    //--------------------------------------------------------------------------
    //! \brief Bind VAO with this GLProgram instance.
    //!
    //! Two cases:
    //!   - If it's the first time that the VAO is bound to the GLProgram then
    //!     the VAO has its internal list of VBOs and textures populated. They
    //!     are refered by the variable name used in the GLSL code. VBOs are
    //!     pre-allocated with the number of elements (either set from the by
    //!     GLProgram constructor or by the method bind()) but no data are
    //!     filled, this is the job of the developper to do it explicitly.
    //!
    //!   - Else, if the VAO was bound previously with this instance of program,
    //!     nothing is made (list are not created/updated).
    //!
    //!   - Else, if the VAO was bound previously with another instance of
    //!     program, this is considered as an error and the method return false.
    //!
    //! \note A VAO bound to a GLProgram cannot be bound to a another
    //! GLProgram. This case is refused to avoid to the developer to have a
    //! silent error with an unexpected behavior.
    //!
    //! \param[in] vao the VAO to be bound with this instance of GLProgram.
    //!
    //! \return true if the GLVAO has been bound. If bounding cannot be made
    //! returns false (for one of the following reasons: the GLProgram cannot be
    //! compiled (syntax errors in shader code) or if the VOA has already been
    //! bound by another GLProgram (incompatibility). Call strerror() to get the
    //! error message.
    //--------------------------------------------------------------------------
    bool bind(GLVAO& vao);

    //--------------------------------------------------------------------------
    //! \brief Return all error messages (concated by '\\n' char) produced either
    //! during the shader compilation or by an other event.
    //!
    //! \note Once this method as been called the error message is
    //! automatically cleared.
    //!
    //! \return the error message (the message is empty if no error have occured).
    //--------------------------------------------------------------------------
    std::string strerror();

    //--------------------------------------------------------------------------
    //! \brief Return the list of shaders that failed to be compiled (if happened).
    //! This is method is mainly used for debug purpose.
    //!
    //! \note if no shader have been attached to a GLProgram or no compilation
    //! error happened or no compilation have been made this method will return an
    //! empty list.
    //!
    //! \param[in,out] list the list where to insert shaders.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getFailedShaders(std::vector<std::string>& list, bool const clear = true) const;

    //--------------------------------------------------------------------------
    //! \brief Return the list of uniform names. This is method is mainly used for
    //! debug purpose.
    //!
    //! \note if no shader have been attached to a GLProgram or compilation error
    //! happened or no compilation have been made or the shader code did not use
    //! uniforms this method will return an empty list.
    //!
    //! \param[in,out] list the list where to insert attributes names.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getUniformNames(std::vector<std::string>& list, bool const clear = true) const;

    //--------------------------------------------------------------------------
    //! \brief Return the list of attributes names. This is method is mainly
    //! used for debug purpose.
    //!
    //! \note if no shader have been attached to a GLProgram or compilation error
    //! happened or no compilation have been made this method will return an empty
    //! list.
    //!
    //! \param[in,out] list the list where to insert attributes names.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getAttributeNames(std::vector<std::string>& list, bool const clear = true) const;

    //--------------------------------------------------------------------------
    //! \brief Return the list of texture names. This is method is mainly
    //! used for debug purpose.
    //!
    //! \note Do not confuse samplers with textures. In this method, we do not
    //! refer to the file name with jpeg, png or bmp ... extensions but to the
    //! GLSL uniform name (used inside shader code sources).
    //!
    //! \note if no shader have been attached to a GLProgram or compilation error
    //! happened or no compilation have been made or the shader code did not use
    //! sampler, this method will return an empty list.
    //!
    //! \param[in,out] list the list where to insert sampler names.
    //! \param[in] if the list has to be cleared before being filled.
    //!
    //! \return the number of inserted elements.
    //--------------------------------------------------------------------------
    size_t getSamplerNames(std::vector<std::string>& list, bool const clear = true) const;

    //--------------------------------------------------------------------------
    //! \brief Check if program has a non empty list of shader attributes.
    //! \return true if the list is non empty.
    //--------------------------------------------------------------------------
    inline bool hasAttributes() const
    {
        return 0_z != m_attributes.size();
    }

    //--------------------------------------------------------------------------
    //! \brief Check if program has a non empty list of uniform texture sampler.
    //! \return true if the list is non empty.
    //--------------------------------------------------------------------------
    inline bool hasSamplers() const
    {
        return 0_z != m_samplers.size();
    }

    //--------------------------------------------------------------------------
    //! \brief Check if program has a non empty list of shader uniforms.
    //!
    //! \return true if the list is non empty.
    //--------------------------------------------------------------------------
    inline bool hasUniforms() const
    {
        return 0_z != m_uniforms.size();
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 4x4 matrix. This method
    //! wraps the \a uniform() method hidding the misery of the template.
    //--------------------------------------------------------------------------
    inline Matrix44f& matrix44f(const char *name)
    {
        return uniform<Matrix44f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 3x3 matrix. This method
    //! wraps the \a uniform() method hidding the misery of the template.
    //--------------------------------------------------------------------------
    inline Matrix33f& matrix33f(const char *name)
    {
        return uniform<Matrix33f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 2x2 matrix.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Matrix22f& matrix22f(const char *name)
    {
        return uniform<Matrix22f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 4D vector.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector4f& vector4f(const char *name)
    {
        return uniform<Vector4f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 3D vector.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector3f& vector3f(const char *name)
    {
        return uniform<Vector3f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float 2D vector.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector2f& vector2f(const char *name)
    {
        return uniform<Vector2f>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform float scalar.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline float& scalarf(const char *name)
    {
        return uniform<float>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform integer 4D vector.
    //!  \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector4i& vector4i(const char *name)
    {
        return uniform<Vector4i>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform integer 3D vector.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector3i& vector3i(const char *name)
    {
        return uniform<Vector3i>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform integer 2D vector.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline Vector2i& vector2i(const char *name)
    {
        return uniform<Vector2i>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate and return the shader uniform integer scalar.
    //! This method wraps the \a uniform() method hidding the misery of
    //! the template.
    //--------------------------------------------------------------------------
    inline int& scalar(const char *name)
    {
        return uniform<int>(name);
    }

    //--------------------------------------------------------------------------
    //! \brief Locate the uniform variable by its name and its type T.
    //! \return the uniform instance if found else throw an exception.
    //! \throw OpenGLException if the uniform does not exist or bad T type param.
    //--------------------------------------------------------------------------
    template<class T>
    GLUniform<T>& uniform(const char *name)
    {
        if (unlikely(nullptr == name))
            throw GL::Exception("nullptr passed to uniform");

        if (unlikely(!compiled()))
        {
            createUniform(getGLType<T>(), name, handle());
        }

        try
        {
            return *(m_uniforms.get<std::shared_ptr<GLUniform<T>>>(name));
        }
        catch (std::exception&)
        {
            throw GL::Exception("GLUniform '" + std::string(name) + "' does not exist");
        }
    }

    template<class T>
    bool hasUniform(const char *name)
    {
        bool ret = m_uniforms.has<std::shared_ptr<GLUniform<T>>>(name);
std::cout << "hasUniform " << name << " " << ret << std::endl;
return ret;
    }

private:

    template<class T>
    void doCreateUniform(GLint dim, GLenum type, const char *name, const GLuint prog)
    {
        if (!m_uniforms.has<std::shared_ptr<GLUniform<T>>>(name))
        {
            auto ptr = std::make_shared<GLUniform<T>>(name, dim, type, prog);
            m_uniforms.add(name, ptr);
            m_uniformLocations[name] = ptr;
        }
        else
        {
            m_uniforms.get<std::shared_ptr<GLUniform<T>>>(name)->m_program = prog;
        }
    }

    //--------------------------------------------------------------------------
    //! \brief From C++ type return the OpenGL enum (ie float => GL_FLOAT).
    //--------------------------------------------------------------------------
    template<class T> GLenum getGLType();

    //--------------------------------------------------------------------------
    //! \brief Return the list of attributes. Only reachable by VAO class.
    //--------------------------------------------------------------------------
    Attributes const& attributes() const
    {
        return m_attributes;
    }

    //--------------------------------------------------------------------------
    //! \brief  Return the list of samplers. Only reachable by VAO class.
    //--------------------------------------------------------------------------
    Samplers const& samplers() const
    {
        return m_samplers;
    }

    //--------------------------------------------------------------------------
    //! \brief Do the shader compilation
    //--------------------------------------------------------------------------
    bool compile();

    //--------------------------------------------------------------------------
    //! \brief Concat the last error to the list of errors
    //--------------------------------------------------------------------------
    void concatError(std::string const& msg);

    //--------------------------------------------------------------------------
    //! \brief Create a new OpenGL program. Compile and link shaders bound to
    //! this GLProgram instance.
    //!
    //! \return false is compilation succeeded (indicating setup has not to be
    //! redone). Return true if an error occurred (errors in the code source of
    //! shaders).
    //--------------------------------------------------------------------------
    virtual bool onCreate() override;

    //--------------------------------------------------------------------------
    //! \brief Activate in OpenGL the program, its attributes, its uniforms and
    //! samplers. A VAO shall be bound else nothing is made.
    //--------------------------------------------------------------------------
    virtual void onActivate() override;

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    virtual bool onSetup() override;

    //--------------------------------------------------------------------------
    //! \brief Dummy method. Nothing is made.
    //--------------------------------------------------------------------------
    virtual bool onUpdate() override;

    //--------------------------------------------------------------------------
    //! \brief Deactivate the OpenGL program, its uniform, attributes, samplers.
    //!
    //! \note A program can be deactivated if and only if shaders have
    //! been loaded into a program (else nothing is done).
    //--------------------------------------------------------------------------
    virtual void onDeactivate() override;

    //--------------------------------------------------------------------------
    //! \brief Once program is no longer used, release it from the GPU
    //! memory. Can be used to abort the shader.
    //--------------------------------------------------------------------------
    virtual void onRelease() override;

    //--------------------------------------------------------------------------
    //! \brief Check if the shaders have been successfully linked.
    //!
    //! \return true if case of success, else return false.
    //--------------------------------------------------------------------------
    bool checkLinkageStatus(GLuint handle);

    //--------------------------------------------------------------------------
    //! \brief Release the GPU memory by removing compiled shaders.
    //--------------------------------------------------------------------------
    void detachAllShaders();

    //--------------------------------------------------------------------------
    //! \brief Create uniform, texture sampler and attribute lists.
    //--------------------------------------------------------------------------
    void generateLocations();

    //--------------------------------------------------------------------------
    //! \brief Create texture sampler instances.
    //--------------------------------------------------------------------------
    void createSampler(GLenum type, const char *name, const size_t count, const GLuint prog);

    //--------------------------------------------------------------------------
    //! \brief Create uniform instances.
    //--------------------------------------------------------------------------
    void createUniform(GLenum type, const char *name, const GLuint prog);

    //--------------------------------------------------------------------------
    //! \brief Create Attribute instances
    //--------------------------------------------------------------------------
    void createAttribute(GLenum type, const char *name, const GLuint prog);

private:

    Shaders    m_shaders;
    Attributes m_attributes;
public:    Samplers   m_samplers; // FIXME private
private:    Uniforms   m_uniforms;
    UniformLocations m_uniformLocations;
    std::vector<std::string> m_failedShaders;
    std::string m_error;
};

template<> inline GLenum GLProgram::getGLType<float>() { return GL_FLOAT; }
template<> inline GLenum GLProgram::getGLType<Vector2f>() { return GL_FLOAT_VEC2; }
template<> inline GLenum GLProgram::getGLType<Vector3f>() { return GL_FLOAT_VEC3; }
template<> inline GLenum GLProgram::getGLType<Vector4f>() { return GL_FLOAT_VEC4; }
template<> inline GLenum GLProgram::getGLType<Matrix22f>() { return GL_FLOAT_MAT2; }
template<> inline GLenum GLProgram::getGLType<Matrix33f>() { return GL_FLOAT_MAT3; }
template<> inline GLenum GLProgram::getGLType<Matrix44f>() { return GL_FLOAT_MAT4; }
template<> inline GLenum GLProgram::getGLType<int>() { return GL_INT; }
template<> inline GLenum GLProgram::getGLType<Vector2i>() { return GL_INT_VEC2; }
template<> inline GLenum GLProgram::getGLType<Vector3i>() { return GL_INT_VEC3; }
template<> inline GLenum GLProgram::getGLType<Vector4i>() { return GL_INT_VEC4; }
template<> inline GLenum GLProgram::getGLType<GLSampler1D>() { return GL_SAMPLER_1D; }
template<> inline GLenum GLProgram::getGLType<GLSampler2D>() { return GL_SAMPLER_2D; }
template<> inline GLenum GLProgram::getGLType<GLSampler3D>() { return GL_SAMPLER_3D; }
template<> inline GLenum GLProgram::getGLType<GLSamplerCube>() { return GL_SAMPLER_CUBE; }

#endif // OPENGLCPPWRAPPER_GLPROGRAM_HPP
