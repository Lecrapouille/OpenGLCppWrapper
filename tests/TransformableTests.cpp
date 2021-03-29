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
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wfloat-equal"
#    include "Math/Transformable.hpp"
#  pragma GCC diagnostic pop
#undef protected
#undef private

//--------------------------------------------------------------------------
static void CHECK_VECTOR3F(Vector3f const& v, const float x, const float y, const float z)
{
    ASSERT_EQ(x, v.x);
    ASSERT_EQ(y, v.y);
    ASSERT_EQ(z, v.z);
}

//--------------------------------------------------------------------------
template <typename T, size_t r, size_t c>
static void check_matrix(Matrix<T,r,c> const &a, Matrix<T,r,c> const &b)
{
    for (size_t i = 0_z; i < r * c; ++i)
    {
        ASSERT_EQ(true, std::abs(a.m_data[i] - b.m_data[i]) < 0.0001f);
    }
}

//--------------------------------------------------------------------------
TEST(TestTransformable, testConstructor)
{
    Transformable3D tr;

    std::cout << "Constructor" << std::endl;
    {
        CHECK_VECTOR3F(tr.m_origin, 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.m_position, 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.m_scale, 1.0f, 1.0f, 1.0f);
        CHECK_VECTOR3F(tr.m_local_scaling, 1.0f, 1.0f, 1.0f);
        check_matrix(Matrix44f(matrix::Identity), tr.m_transform);
        check_matrix(Matrix44f(matrix::Identity), tr.m_inverse_transform);
        ASSERT_EQ(false, tr.m_transform_needs_update);
        ASSERT_EQ(false, tr.m_inverse_trans_needs_update);

        CHECK_VECTOR3F(tr.origin(), 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.position(), 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.local_position(), 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.scaling(), 1.0f, 1.0f, 1.0f);
        CHECK_VECTOR3F(tr.localScale(), 1.0f, 1.0f, 1.0f);
        check_matrix(Matrix44f(matrix::Identity), tr.matrix());
        check_matrix(Matrix44f(matrix::Identity), tr.invMatrix());
    }

    std::cout << "Change" << std::endl;
    {
        tr.m_origin = tr.m_position = Vector3f(1.0f);
        tr.m_scale = tr.m_local_scaling = Vector3f(2.0f);
        tr.m_transform *= 2.0f;
        tr.m_inverse_transform *= 2.0f;
        tr.m_transform_needs_update = tr.m_inverse_trans_needs_update = true;

        std::cout << "111" << std::endl;
        CHECK_VECTOR3F(tr.origin(), 1.0f, 1.0f, 1.0f);
        std::cout << "222" << std::endl;
        CHECK_VECTOR3F(tr.position(), 1.0f, 1.0f, 1.0f);
        std::cout << "333" << std::endl;
        CHECK_VECTOR3F(tr.local_position(), 0.0f, 0.0f, 0.0f);
        std::cout << "444" << std::endl;
        CHECK_VECTOR3F(tr.scaling(), 2.0f, 2.0f, 2.0f);
        CHECK_VECTOR3F(tr.localScale(), 2.0f, 2.0f, 2.0f);
        std::cout << "555" << tr.matrix()<< std::endl;
        check_matrix(Matrix44f({2.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 2.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 2.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 2.0f}),
            tr.matrix());
        std::cout << "666" << std::endl;
        check_matrix(Matrix44f(matrix::Identity), tr.invMatrix());
    }

    std::cout << "reset" << std::endl;
    {
        tr.reset();
        CHECK_VECTOR3F(tr.m_origin, 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.m_position, 0.0f, 0.0f, 0.0f);
        CHECK_VECTOR3F(tr.m_scale, 1.0f, 1.0f, 1.0f);
        CHECK_VECTOR3F(tr.m_local_scaling, 1.0f, 1.0f, 1.0f);
        check_matrix(Matrix44f(matrix::Identity), tr.m_transform);
        check_matrix(Matrix44f(matrix::Identity), tr.m_inverse_transform);
        ASSERT_EQ(false, tr.m_transform_needs_update);
        ASSERT_EQ(false, tr.m_inverse_trans_needs_update);
    }
}
