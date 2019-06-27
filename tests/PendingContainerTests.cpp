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

#define protected public
#define private public
#include "PendingContainer.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

TESTSUITE(PendingContainerTests)
{
  TEST(TestConstructors)
    {
      // --- Test 1
      PendingContainer<int> pc0;
      ASSERT_EQ(false, pc0.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);
      ASSERT_EQ(0_z, pc0.m_container.size());
      ASSERT_EQ(0_z, pc0.m_container.capacity());
      ASSERT_EQ(0_z, pc0.size());
      ASSERT_EQ(0_z, pc0.capacity());
      pc0.resize(0_z);
      ASSERT_EQ(false, pc0.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);

      // --- Test 2
      PendingContainer<int> pc1(10_z);
      ASSERT_EQ(false, pc1.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc1.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc1.getPendingData().second);
      ASSERT_EQ(0_z, pc1.m_container.size());
      ASSERT_EQ(10_z, pc1.m_container.capacity());
      ASSERT_EQ(0_z, pc1.size());
      ASSERT_EQ(10_z, pc1.capacity());
      pc1.reserve(20_z);
      ASSERT_EQ(0_z, pc1.size());
      ASSERT_EQ(20_z, pc1.capacity());
      pc1.resize(10_z);
      ASSERT_EQ(10_z, pc1.size());
      ASSERT_EQ(20_z, pc1.capacity());
      ASSERT_EQ(false, pc1.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc1.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc1.getPendingData().second);

      // --- Test 3
      PendingContainer<int> pc2(10_z, 42);
      ASSERT_EQ(true, pc2.hasPendingData());
      ASSERT_EQ(0_z, pc2.getPendingData().first);
      ASSERT_EQ(10_z, pc2.getPendingData().second);
      ASSERT_EQ(10_z, pc2.m_container.size());
      ASSERT_EQ(10_z, pc2.m_container.capacity());
      ASSERT_EQ(10_z, pc2.size());
      ASSERT_EQ(10_z, pc2.capacity());
      for (auto it: pc2.m_container)
        {
          ASSERT_EQ(42, it);
        }
      pc2.reserve(20_z);
      ASSERT_EQ(10_z, pc2.size());
      ASSERT_EQ(20_z, pc2.capacity());
      pc2.resize(2_z);
      ASSERT_EQ(2_z, pc2.size());
      ASSERT_EQ(20_z, pc2.capacity());
      for (auto it: pc2.m_container)
        {
          ASSERT_EQ(42, it);
        }
      ASSERT_EQ(true, pc2.hasPendingData());
      ASSERT_EQ(0_z, pc2.getPendingData().first);
      ASSERT_EQ(2_z, pc2.getPendingData().second);
      pc2.resize(5_z);
      ASSERT_EQ(5_z, pc2.size());
      ASSERT_EQ(20_z, pc2.capacity());
      ASSERT_EQ(true, pc2.hasPendingData());
      ASSERT_EQ(0_z, pc2.getPendingData().first);
      ASSERT_EQ(2_z, pc2.getPendingData().second);
      size_t i = pc2.size();
      while (i--)
        {
          if (i < 2_z) {
            ASSERT_EQ(42, pc2.m_container[i]);
          } else {
            ASSERT_EQ(0, pc2.m_container[i]);
          }
        }

      // --- Test 4
      PendingContainer<int> pc3(pc1);
      ASSERT_EQ(false, pc3.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().second);
      ASSERT_EQ(pc1.getPendingData().first, pc3.getPendingData().first);
      ASSERT_EQ(pc1.getPendingData().second, pc3.getPendingData().second);
      ASSERT_EQ(10_z, pc3.m_container.size());
      ASSERT_EQ(20_z, pc3.m_container.capacity());
      ASSERT_EQ(10_z, pc3.size());
      ASSERT_EQ(20_z, pc3.capacity());
      ASSERT_EQ(pc1.size(), pc3.size());
      ASSERT_EQ(pc1.capacity(), pc3.capacity());
      pc3.reserve(5_z);
      ASSERT_EQ(10_z, pc3.size());
      ASSERT_EQ(20_z, pc3.capacity());
      ASSERT_EQ(false, pc3.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().second);
      pc3.resize(10_z);
      ASSERT_EQ(10_z, pc3.size());
      ASSERT_EQ(20_z, pc3.capacity());
      ASSERT_EQ(false, pc3.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc3.getPendingData().second);

      // --- Test 5
      PendingContainer<int> pc4(pc2);
      ASSERT_EQ(true, pc4.hasPendingData());
      ASSERT_EQ(0_z, pc4.getPendingData().first);
      ASSERT_EQ(2_z, pc4.getPendingData().second);
      ASSERT_EQ(pc2.getPendingData().first, pc4.getPendingData().first);
      ASSERT_EQ(pc2.getPendingData().second, pc4.getPendingData().second);
      ASSERT_EQ(5_z, pc4.m_container.size());
      ASSERT_EQ(20_z, pc4.m_container.capacity());
      ASSERT_EQ(5_z, pc4.size());
      ASSERT_EQ(20_z, pc4.capacity());
      ASSERT_EQ(pc2.size(), pc4.size());
      ASSERT_EQ(pc2.capacity(), pc4.capacity());
      i = pc4.size();
      while (i--)
        {
          if (i < 2_z) {
            ASSERT_EQ(42, pc4.m_container[i]);
          } else {
            ASSERT_EQ(0, pc4.m_container[i]);
          }
        }

      pc4.reserve(0_z);
      ASSERT_EQ(5_z, pc4.size());
      ASSERT_EQ(20_z, pc4.capacity());

      pc4.resize(0_z);
      ASSERT_EQ(false, pc4.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc4.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc4.getPendingData().second);

      // --- Test 6
      PendingContainer<int> pc5({66, 42, 55});
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(3_z, pc5.getPendingData().second);
      ASSERT_EQ(3_z, pc5.m_container.size());
      ASSERT_EQ(3_z, pc5.m_container.capacity());
      ASSERT_EQ(3_z, pc5.size());
      ASSERT_EQ(3_z, pc5.capacity());
      ASSERT_EQ(66, pc5.m_container[0]);
      ASSERT_EQ(42, pc5.m_container[1]);
      ASSERT_EQ(55, pc5.m_container[2]);
      pc5.reserve(10_z);
      ASSERT_EQ(3_z, pc5.size());
      ASSERT_EQ(10_z, pc5.capacity());

      pc5.clearPending();
      ASSERT_EQ(false, pc5.hasPendingData());
      pc5.append({56, 57, 58, 59, 60});
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(3_z, pc5.getPendingData().first);
      ASSERT_EQ(8_z, pc5.getPendingData().second);
      pc5.resize(2_z);
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(2_z, pc5.getPendingData().first);
      ASSERT_EQ(2_z, pc5.getPendingData().second);
      pc5.resize(0_z);
      ASSERT_EQ(false, pc5.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().second);
      pc5.resize(10_z);
      ASSERT_EQ(false, pc5.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().second);
      int a = pc5.get(0);
      ASSERT_EQ(false, pc5.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().second);
      pc5.set(0) = a;
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(0_z, pc5.getPendingData().second);

      ASSERT_EQ(10_z, pc5.m_container.capacity());
      ASSERT_EQ(10_z, pc5.m_container.size());

      pc5.set(20) = a;
      ASSERT_EQ(20_z, pc5.m_container.size());
      ASSERT_EQ(20_z, pc5.m_container.capacity());
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(20_z, pc5.getPendingData().second);

      pc5.append(std::vector<int>({1, 2, 3}));
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(23_z, pc5.getPendingData().second);
      ASSERT_EQ(23_z, pc5.size());
      ASSERT_EQ(40_z, pc5.capacity()); // 20_z * 2

      pc5.append(4);
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(24_z, pc5.getPendingData().second);
      ASSERT_EQ(24_z, pc5.size());
      ASSERT_EQ(40_z, pc5.capacity());

      pc5.append(PendingContainer<int>({5}));
      ASSERT_EQ(true, pc5.hasPendingData());
      ASSERT_EQ(0_z, pc5.getPendingData().first);
      ASSERT_EQ(25_z, pc5.getPendingData().second);
      ASSERT_EQ(25_z, pc5.size());
      ASSERT_EQ(40_z, pc5.capacity());

      pc5.clear();
      ASSERT_EQ(0_z, pc5.size());
      ASSERT_EQ(40_z, pc5.capacity());
      ASSERT_EQ(false, pc5.hasPendingData());
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pc5.getPendingData().second);
    }

  TEST(TestArithmeticsValid)
  {
    PendingContainer<int> pc1({42, 5, 50, 10});
    ASSERT_EQ(true, pc1.hasPendingData());
    ASSERT_EQ(0_z, pc1.getPendingData().first);
    ASSERT_EQ(4_z, pc1.getPendingData().second);
    pc1.clearPending();
    ASSERT_EQ(false, pc1.hasPendingData());
    ASSERT_EQ(107, pc1.sum());
    ASSERT_EQ(false, pc1.hasPendingData());
    ASSERT_EQ(105000, pc1.prod());
    ASSERT_EQ(false, pc1.hasPendingData());
    ASSERT_EQ(5, pc1.min());
    ASSERT_EQ(false, pc1.hasPendingData());
    ASSERT_EQ(50, pc1.max());
    ASSERT_EQ(false, pc1.hasPendingData());

    PendingContainer<int> pc2({-42, -5, -50, -10});
    ASSERT_EQ(true, pc2.hasPendingData());
    pc2.clearPending();
    pc2.abs();
    ASSERT_EQ(std::vector<int>({42, 5, 50, 10}), pc2.m_container);
    ASSERT_EQ(true, pc2.hasPendingData());
    ASSERT_EQ(0_z, pc2.getPendingData().first);
    ASSERT_EQ(4_z, pc2.getPendingData().second);

    PendingContainer<float> pc3({4.0f, 9.0f, 16.0f, 25.0f});
    pc3.clearPending();
    pc3.sqrt();
    ASSERT_EQ(std::vector<float>({2.0f, 3.0f, 4.0f, 5.0f}), pc3.m_container);
    ASSERT_EQ(true, pc3.hasPendingData());
    ASSERT_EQ(0_z, pc3.getPendingData().first);
    ASSERT_EQ(4_z, pc3.getPendingData().second);
    pc3.squared();
    ASSERT_EQ(std::vector<float>({4.0f, 9.0f, 16.0f, 25.0f}), pc3.m_container);
    pc3.cos();
    ASSERT_EQ(std::vector<float>({std::cos(4.0f), std::cos(9.0f), std::cos(16.0f), std::cos(25.0f)}), pc3.m_container);
    pc3.clearPending();

    PendingContainer<float> pc4;
    pc4 = pc3;
    ASSERT_EQ(true, pc4.hasPendingData());
    ASSERT_EQ(0_z, pc4.getPendingData().first);
    ASSERT_EQ(4_z, pc4.getPendingData().second);
    ASSERT_EQ(std::vector<float>({std::cos(4.0f), std::cos(9.0f), std::cos(16.0f), std::cos(25.0f)}), pc4.m_container);

    pc4.clearPending();
    pc4 = std::vector<float>({2.0f, 3.0f, 4.0f, 5.0f});
    ASSERT_EQ(true, pc4.hasPendingData());
    ASSERT_EQ(0_z, pc4.getPendingData().first);
    ASSERT_EQ(4_z, pc4.getPendingData().second);
    pc4.sin();
    ASSERT_EQ(std::vector<float>({std::sin(2.0f), std::sin(3.0f), std::sin(4.0f), std::sin(5.0f)}), pc4.m_container);
    ASSERT_EQ(true, pc4.hasPendingData());
    ASSERT_EQ(0_z, pc4.getPendingData().first);
    ASSERT_EQ(4_z, pc4.getPendingData().second);

    pc4.clear();
    ASSERT_EQ(false, pc4.hasPendingData());
    pc4.clearPending();
    pc4 = {2.0f, 3.0f, 4.0f, 5.0f};
    ASSERT_EQ(true, pc4.hasPendingData());
    ASSERT_EQ(0_z, pc4.getPendingData().first);
    ASSERT_EQ(4_z, pc4.getPendingData().second);
    ASSERT_EQ(std::vector<float>({2.0f, 3.0f, 4.0f, 5.0f}), pc4.m_container);

    pc4.clearPending();
    pc4 *= 2.0f;
    ASSERT_EQ(true, pc4.hasPendingData());
    ASSERT_EQ(0_z, pc4.getPendingData().first);
    ASSERT_EQ(4_z, pc4.getPendingData().second);
    ASSERT_EQ(std::vector<float>({4.0f, 6.0f, 8.0f, 10.0f}), pc4.m_container);

    pc4 += 2.0f;
    ASSERT_EQ(std::vector<float>({6.0f, 8.0f, 10.0f, 12.0f}), pc4.m_container);
    pc4 -= 2.0f;
    ASSERT_EQ(std::vector<float>({4.0f, 6.0f, 8.0f, 10.0f}), pc4.m_container);
    pc4 /= 2.0f;
    ASSERT_EQ(std::vector<float>({2.0f, 3.0f, 4.0f, 5.0f}), pc4.m_container);

    std::cout << pc4 << std::endl;
    float* a = pc4.to_array();
    float const* ca = pc4.to_array();
    ASSERT_NE(nullptr, a); ASSERT_NE(nullptr, ca);
    ASSERT_EQ(2.0f, a[0]); ASSERT_EQ(2.0f, ca[0]);
    ASSERT_EQ(3.0f, a[1]); ASSERT_EQ(3.0f, ca[1]);
    ASSERT_EQ(4.0f, a[2]); ASSERT_EQ(4.0f, ca[2]);
    ASSERT_EQ(5.0f, a[3]); ASSERT_EQ(5.0f, ca[3]);

    PendingContainer<int> pc0;
    int* a0 = pc0.to_array();
    int const* ca0 = pc0.to_array();
    ASSERT_EQ(nullptr, a0); ASSERT_EQ(nullptr, ca0);

    PendingContainer<int> const pc00;
    int const* ca00 = pc00.to_array();
    ASSERT_EQ(nullptr, ca00);
  }

  TEST(TestArithmeticsInvalid)
  {
    PendingContainer<int> pc0;
    try {
      pc0.sum();
      ASSERT_FALSE("Exception not caught");
    } catch (std::out_of_range) { }

    try {
      pc0.prod();
      ASSERT_FALSE("Exception not caught");
    } catch (std::out_of_range) { }

    try {
      pc0.min();
      ASSERT_FALSE("Exception not caught");
    } catch (std::out_of_range) { }

    try {
      pc0.max();
      ASSERT_FALSE("Exception not caught");
    } catch (std::out_of_range) { }
  }

  TEST(TestNotExpandable)
  {
    PendingContainer<int> pc0;
    ASSERT_EQ(true, pc0.m_can_expand);
    pc0.set_cannot_expand();
    ASSERT_EQ(false, pc0.m_can_expand);
    ASSERT_EQ(0_z, pc0.size());

    try
      {
        pc0 = {4, 6, 8, 10};
        ASSERT_FALSE("Exception not caught");
      }
    catch (std::out_of_range)
      {
        ASSERT_EQ(false, pc0.hasPendingData());
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);
        ASSERT_EQ(0_z, pc0.size());
      }

    try
      {
        pc0.append({4, 6, 8, 10});
        ASSERT_FALSE("Exception not caught");
      }
    catch (std::out_of_range)
      {
        ASSERT_EQ(false, pc0.hasPendingData());
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);
        ASSERT_EQ(0_z, pc0.size());
      }

    ASSERT_EQ(0_z, pc0.capacity());
    try
      {
        pc0.reserve(10_z);
        ASSERT_FALSE("Exception not caught");
      }
    catch (std::out_of_range)
      {
        ASSERT_EQ(false, pc0.hasPendingData());
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);
        ASSERT_EQ(0_z, pc0.size());
        ASSERT_EQ(0_z, pc0.capacity());
      }

    try
      {
        pc0.resize(10_z);
        ASSERT_FALSE("Exception not caught");
      }
    catch (std::out_of_range)
      {
        ASSERT_EQ(false, pc0.hasPendingData());
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().first);
        ASSERT_EQ(PendingData::npos, pc0.getPendingData().second);
        ASSERT_EQ(0_z, pc0.size());
        ASSERT_EQ(0_z, pc0.capacity());
      }
  }
}
