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
#include "PendingData.hpp"
#undef protected
#undef private
#include <crpcut.hpp>
#include <string>

using namespace glwrap;

TESTSUITE(PendingDataTests)
{
  TEST(TestEmptyConstructor)
    {
      PendingData pd;

      ASSERT_EQ(PendingData::npos, pd.m_pending_start);
      ASSERT_EQ(PendingData::npos, pd.m_pending_end);
      ASSERT_EQ(false, pd.hasPendingData());

      size_t pos_start;
      size_t pos_end;
      pd.getPendingData(pos_start, pos_end);
      ASSERT_EQ(PendingData::npos, pos_start);
      ASSERT_EQ(PendingData::npos, pos_end);
      ASSERT_EQ(PendingData::npos, pd.getPendingData().first);
      ASSERT_EQ(PendingData::npos, pd.getPendingData().second);

      pd.clearPending();
      ASSERT_EQ(false, pd.hasPendingData());
      ASSERT_EQ(PendingData::npos, pd.m_pending_start);
      ASSERT_EQ(PendingData::npos, pd.m_pending_end);

      pd.clearPending(0_z);
      ASSERT_EQ(false, pd.hasPendingData());
      ASSERT_EQ(PendingData::npos, pd.m_pending_start);
      ASSERT_EQ(PendingData::npos, pd.m_pending_end);

      pd.clearPending(10_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(10_z, pd.m_pending_end);
      ASSERT_EQ(0_z, pd.getPendingData().first);
      ASSERT_EQ(10_z, pd.getPendingData().second);
    }

  TEST(TestConstructor)
    {
      PendingData pd(10_z);

      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(10_z, pd.m_pending_end);
      ASSERT_EQ(true, pd.hasPendingData());

      size_t pos_start;
      size_t pos_end;
      pd.getPendingData(pos_start, pos_end);
      ASSERT_EQ(0_z, pos_start);
      ASSERT_EQ(10_z, pos_end);
      ASSERT_EQ(0_z, pd.getPendingData().first);
      ASSERT_EQ(10_z, pd.getPendingData().second);

      pd.clearPending();
      ASSERT_EQ(false, pd.hasPendingData());
      ASSERT_EQ(PendingData::npos, pd.m_pending_start);
      ASSERT_EQ(PendingData::npos, pd.m_pending_end);

      pd.clearPending(0_z);
      ASSERT_EQ(false, pd.hasPendingData());
      ASSERT_EQ(PendingData::npos, pd.m_pending_start);
      ASSERT_EQ(PendingData::npos, pd.m_pending_end);

      pd.clearPending(10_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(10_z, pd.m_pending_end);
    }

    TEST(TestPending)
    {
      PendingData pd;

      pd.tagAsPending(0_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(0_z, pd.m_pending_end);
      ASSERT_EQ(0_z, pd.getPendingData().first);
      ASSERT_EQ(0_z, pd.getPendingData().second);

      pd.tagAsPending(1_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(1_z, pd.m_pending_end);
      ASSERT_EQ(0_z, pd.getPendingData().first);
      ASSERT_EQ(1_z, pd.getPendingData().second);

      pd.clearPending();
      pd.tagAsPending(3_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(3_z, pd.m_pending_start);
      ASSERT_EQ(3_z, pd.m_pending_end);
      ASSERT_EQ(3_z, pd.getPendingData().first);
      ASSERT_EQ(3_z, pd.getPendingData().second);

      pd.tagAsPending(1_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(1_z, pd.m_pending_start);
      ASSERT_EQ(3_z, pd.m_pending_end);
      ASSERT_EQ(1_z, pd.getPendingData().first);
      ASSERT_EQ(3_z, pd.getPendingData().second);

      pd.tagAsPending(5_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(1_z, pd.m_pending_start);
      ASSERT_EQ(5_z, pd.m_pending_end);
      ASSERT_EQ(1_z, pd.getPendingData().first);
      ASSERT_EQ(5_z, pd.getPendingData().second);

      pd.tagAsPending(0_z, 8_z);
      ASSERT_EQ(true, pd.hasPendingData());
      ASSERT_EQ(0_z, pd.m_pending_start);
      ASSERT_EQ(8_z, pd.m_pending_end);
      ASSERT_EQ(0_z, pd.getPendingData().first);
      ASSERT_EQ(8_z, pd.getPendingData().second);
    }
}
