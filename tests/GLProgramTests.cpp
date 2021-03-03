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
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "main.hpp"
#define protected public
#define private public
#  include "OpenGL/Shaders/Program.hpp"
#  include "OpenGL/Buffers/VAO.hpp"
#undef protected
#undef private

// Check initial states
TEST(TestGLPrograms, TestCreators)
{
    std::vector<std::string> list;

    GLProgram prog("prog");

    ASSERT_STREQ("prog", prog.cname());
    ASSERT_EQ(false, prog.compiled());
    ASSERT_STREQ("", prog.strerror().c_str());
    ASSERT_EQ(0_z, prog.getFailedShaders(list, true));
    ASSERT_EQ(0_z, list.size());
    ASSERT_EQ(0_z, prog.getUniformNames(list, true));
    ASSERT_EQ(0_z, list.size());
    ASSERT_EQ(0_z, prog.getAttributeNames(list, true));
    ASSERT_EQ(0_z, list.size());
    ASSERT_EQ(0_z, prog.getSamplerNames(list, true));
    ASSERT_EQ(0_z, list.size());
    ASSERT_EQ(false, prog.hasAttributes());
    ASSERT_EQ(false, prog.hasUniforms());
    ASSERT_EQ(false, prog.hasSamplers());
}

// Test we can create uniform if the prog is not compiled
TEST(TestGLPrograms, TestCreatUniformProgNotCompiled)
{
    std::vector<std::string> list;

    GLProgram prog("prog");

    // Check uniforms creation
    prog.createUniform(GL_FLOAT, "u1", 42u);
    prog.createUniform(GL_FLOAT_VEC2, "u1", 42u); // double name with dif type
    prog.createUniform(GL_FLOAT_VEC2, "u2", 42u);
    prog.createUniform(GL_FLOAT_VEC3, "u3", 42u);
    prog.createUniform(GL_FLOAT_VEC4, "u4", 42u);
    ASSERT_EQ(4_z, prog.getUniformNames(list, true)); // instead of 5
    ASSERT_EQ(list.size(), 4u);
    ASSERT_THAT(list, ElementsAre("u1", "u2", "u3", "u4")); // "u1" not in double
    ASSERT_EQ(true, prog.hasUniform<float>("u1"));
    ASSERT_EQ(true, prog.hasUniform<Vector2f>("u1"));
    ASSERT_EQ(true, prog.hasUniform<Vector2f>("u2"));
    ASSERT_EQ(true, prog.hasUniform<Vector3f>("u3"));
    ASSERT_EQ(true, prog.hasUniform<Vector4f>("u4"));

    // Check bad accessor
    try {
        prog.uniform<int>(nullptr);
        FAIL() << "Exception should have occured";
    } catch(...) { }

    // Check accessor with known names and correct type
    try { prog.uniform<float>("u1"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.uniform<Vector2f>("u1"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.uniform<Vector2f>("u2"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.uniform<Vector3f>("u3"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.uniform<Vector4f>("u4"); }
    catch(...) { FAIL() << "Exception should not have occured"; }

    // Check accessor with unknown names are inserted
    // FIXME should be inserted ?
    try { prog.uniform<int>(""); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasUniform<int>(""));
    try { prog.uniform<int>("aaa"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasUniform<int>("aaa"));
    ASSERT_EQ(6_z, prog.getUniformNames(list, true));
    ASSERT_EQ(list.size(), 6u);
    ASSERT_THAT(list, ElementsAre("", "aaa", "u1", "u2", "u3", "u4"));

    // Check accessor with known names but wrong type
    // FIXME should be inserted ?
    try { prog.uniform<Vector3f>("u2"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasUniform<Vector2f>("u2"));
    ASSERT_EQ(true, prog.hasUniform<Vector3f>("u2"));
    ASSERT_EQ(6_z, prog.getUniformNames(list, true));
    ASSERT_EQ(list.size(), 6u);
    ASSERT_THAT(list, ElementsAre("", "aaa", "u1", "u2", "u3", "u4"));
}

// Test we cannot create uniform if the prog is compiled
TEST(TestGLPrograms, TestCreatUniformProgCompiled)
{
    std::vector<std::string> list;

    GLProgram prog("prog");
    prog.m_need_setup = false;
    ASSERT_EQ(true, prog.compiled());

    std::cout << "-------------------------------" << std::endl;

    // Unknown name: not added
    try { prog.uniform<float>("u1");  FAIL() << "Exception should have occured"; }
    catch(...) { }

    try { prog.attribute("u1");  FAIL() << "Exception should have occured"; }
    catch(...) { }
}



// Test we can create attribute if the prog is not compiled
TEST(TestGLPrograms, TestCreatAttributeProgNotCompiled)
{
    std::vector<std::string> list;
    GLProgram prog("prog");

    // Check attributes creation
    prog.createAttribute(GL_FLOAT, "u1", 42u);
    prog.createAttribute(GL_FLOAT_VEC2, "u1", 42u); // double name with dif type
    prog.createAttribute(GL_FLOAT_VEC2, "u2", 42u);
    prog.createAttribute(GL_FLOAT_VEC3, "u3", 42u);
    prog.createAttribute(GL_FLOAT_VEC4, "u4", 42u);
    ASSERT_EQ(4_z, prog.getAttributeNames(list, true)); // instead of 5
    ASSERT_EQ(list.size(), 4u);
    ASSERT_THAT(list, ElementsAre("u1", "u2", "u3", "u4")); // "u1" not in double
    ASSERT_EQ(true, prog.hasAttribute("u1"));
    ASSERT_EQ(true, prog.hasAttribute("u1"));
    ASSERT_EQ(true, prog.hasAttribute("u2"));
    ASSERT_EQ(true, prog.hasAttribute("u3"));
    ASSERT_EQ(true, prog.hasAttribute("u4"));

    // Check bad accessor
    try {
        prog.attribute(nullptr);
        FAIL() << "Exception should have occured";
    } catch(...) { }

    // Check accessor with known names and correct type
    try { prog.attribute("u1"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.attribute("u1"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.attribute("u2"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.attribute("u3"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    try { prog.attribute("u4"); }
    catch(...) { FAIL() << "Exception should not have occured"; }

    // Check accessor with unknown names are inserted
    // FIXME should be inserted ?
    try { prog.attribute(""); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasAttribute(""));
    try { prog.attribute("aaa"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasAttribute("aaa"));
    ASSERT_EQ(6_z, prog.getAttributeNames(list, true));
    ASSERT_EQ(list.size(), 6u);
    ASSERT_THAT(list, ElementsAre("", "aaa", "u1", "u2", "u3", "u4"));

    // Check accessor with known names but wrong type
    // FIXME should be inserted ?
    try { prog.attribute("u2"); }
    catch(...) { FAIL() << "Exception should not have occured"; }
    ASSERT_EQ(true, prog.hasAttribute("u2"));
    ASSERT_EQ(true, prog.hasAttribute("u2"));
    ASSERT_EQ(6_z, prog.getAttributeNames(list, true));
    ASSERT_EQ(list.size(), 6u);
    ASSERT_THAT(list, ElementsAre("", "aaa", "u1", "u2", "u3", "u4"));
}

// Test we cannot create attribute if the prog is compiled
TEST(TestGLPrograms, TestCreatAttributeProgCompiled)
{
    std::vector<std::string> list;

    GLProgram prog("prog");
    prog.m_need_setup = true;
    ASSERT_EQ(true, prog.compiled());

    // Unknown name: not added
    try { prog.attribute("u1");  FAIL() << "Exception should have occured"; }
    catch(...) { }

    ASSERT_EQ(0_z, prog.getAttributeNames(list, true));
    ASSERT_EQ(list.size(), 0u);
}

// Test binding a VAO to a GLProgram
TEST(TestGLPrograms, TestVAObind)
{
    std::vector<std::string> list;

    GLProgram prog("prog");
    prog.m_handle = 42;
    prog.uniform<float>("u1");
    prog.uniform<Vector2f>("u2");
    prog.uniform<Vector3f>("u3");
    prog.attribute("a1");
    prog.attribute("a2");
    prog.attribute("a3");

    GLVAO vao("VAO");
    ASSERT_EQ(nullptr, vao.m_program);
    ASSERT_EQ(false, vao.isBound());

    ASSERT_EQ(true, prog.bind(vao));
    ASSERT_EQ(true, vao.isBound());
    ASSERT_EQ(&prog, vao.m_program);
    ASSERT_EQ(42, vao.m_program->handle());
    ASSERT_EQ(true, vao.hasVBOs());
    ASSERT_EQ(true, vao.has<float>("a1"));
    ASSERT_EQ(true, vao.has<Vector2f>("a2"));
    ASSERT_EQ(true, vao.has<Vector3f>("a3"));
    ASSERT_EQ(true, vao.has<Vector4f>("a4"));
}

#if 0
TEST(TestGLPrograms, bindVAOtoWrongGLProg)
{
    GLProgram prog1("prog1");
    prog1.m_handle = 42;
    prog1.m_compiled = true;
    GLProgram prog2("prog2");
    prog2.m_handle = 43;
    prog2.m_compiled = true;
    GLVAO vao1("VAO1");
    GLVAO vao2("VAO2");

    ASSERT_EQ(0, vao1.m_program);
    ASSERT_EQ(0, vao2.m_program);
    ASSERT_EQ(true, prog1.bind(vao1));
    ASSERT_EQ(true, prog2.bind(vao2));
    ASSERT_EQ(42, vao1.m_program);
    ASSERT_EQ(43, vao2.m_program);
    // Try binding VAO to the wrong GLProg
    ASSERT_EQ(false, prog1.bind(vao2));
    ASSERT_EQ(false, prog2.bind(vao1));
}

#endif
