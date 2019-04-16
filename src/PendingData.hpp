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

#ifndef PENDINGDATA_HPP_
#  define PENDINGDATA_HPP_

#  include "private/NonCppStd.hpp"

// **************************************************************
//! \brief Define an interface class keeping track of the smallest
//! contiguous area that have been changed and needs to be
//! uploaded. This class cannot be use alone but for inheritance.
// **************************************************************

class PendingData
{
private:

  static constexpr size_t c_initial_position = static_cast<size_t>(-1);

public:

  //! \brief Empty constructor: no pending data.
  PendingData()
  {
    clearPending();
  }

  //! \brief Constructor: number of elements of the container.
  PendingData(size_t nb_elt)
  {
    clearPending(nb_elt);
  }

  //! \brief Pure virtual destructor, but with a definition. The class
  //! will be abstract, but any inheriting classes will not by default
  //! be abstract.
  virtual ~PendingData() = 0;

  //! \brief Return a boolean indicating if some elements of the block
  //! has chnaged.
  inline bool hasPendingData() const
  {
    return (c_initial_position != m_pending_start);
  }

  //! \brief Return the smallest contiguous area that needs to be
  //! uploaded. If there is no pending data, pos_start will be set to
  //! -1. You can call hasPendingData() before this method.
  void getPendingData(size_t &pos_start, size_t &pos_end) const
  {
    if (hasPendingData())
    {
      pos_start = m_pending_start;
      pos_end = m_pending_end;
    }
    else
    {
      pos_start = 0_z;
      pos_end = 0_z;
    }
  }

  //! \brief Return the smallest contiguous area that needs to be
  //! uploaded. If there is no pending data, pos_start will be set to
  //! -1. You can call hasPendingData() before this method.
  std::pair<size_t, size_t> getPendingData() const
  {
    if (hasPendingData())
    {
      return std::make_pair(m_pending_start, m_pending_end);
    }
    else
    {
      return std::make_pair(0_z, 0_z);
    }
  }

  //! \brief Call this function when changed elements have been uploaded.
  void clearPending()
  {
    m_pending_start = c_initial_position;
    m_pending_end = c_initial_position;
  }

  void clearPending(size_t nb_elt)
  {
    if (0_z == nb_elt)
    {
      m_pending_start = c_initial_position;
      m_pending_end = c_initial_position;
    }
    else
    {
      m_pending_start = 0_z;
      m_pending_end = nb_elt - 1_z;
    }
  }

  //! \brief Update the range indexes of changed elements with a new range.
  void tagAsPending(const size_t pos_start, const size_t pos_end)
  {
    if (!hasPendingData())
      {
        m_pending_start = pos_start;
        m_pending_end = pos_end;
      }
    else
      {
        m_pending_start = std::min(m_pending_start, pos_start);
        m_pending_end = std::max(m_pending_end, pos_end);
      }
  }

  //! \brief Update the range indexes of changed elements with a new range.
  inline void tagAsPending(const size_t pos_start)
  {
    tagAsPending(pos_start, pos_start);
  }

protected:

  //! Indicate which elements have been changed.
  size_t m_pending_start, m_pending_end;
};

#endif
