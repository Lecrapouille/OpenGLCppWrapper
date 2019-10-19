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

#ifndef OPENGLCPPWRAPPER_PENDINGDATA_HPP
#  define OPENGLCPPWRAPPER_PENDINGDATA_HPP

// *****************************************************************************
//! \file Define an interface class keeping track of the smallest
//! contiguous area having been changed and needs to be
//! uploaded. This class cannot be use alone but for inheritance.
// *****************************************************************************

#  include "Common/NonCppStd.hpp"

namespace glwrap
{

// ***************************************************************************
//! \class PendingData PendingData.hpp
//!
//! \brief Define class tracks the smallest contiguous area in a container
//! having been modified (aka updated aka dirty). This class cannot be use
//! alone. Use it with inheritance.
//!
//! This class is used in this API for knowing dirty CPU data that needs to be
//! updated to the GPU.
// ***************************************************************************
class PendingData
{
public:

  //--------------------------------------------------------------------------
  //! \brief When the container is empty place the smallest contiguous area of
  //! dirty elements is not defined. So use -1 instead of 0. It is equivalent
  //! of std::string::npos.
  //--------------------------------------------------------------------------
  static constexpr size_t npos = static_cast<size_t>(-1);

protected:

  //--------------------------------------------------------------------------
  //! \brief Make this class abstract to forbid to use it as standalone.
  //! Derivate this class to use it.
  //--------------------------------------------------------------------------
  virtual ~PendingData() {};

public:

  //--------------------------------------------------------------------------
  //! \brief Empty constructor with no pending data.
  //--------------------------------------------------------------------------
  PendingData()
  {}

  //--------------------------------------------------------------------------
  //! \brief Constructor with an already known number of dirty elements
  //! (starting from the first).
  //--------------------------------------------------------------------------
  PendingData(size_t const nb_elt)
  {
    clearPending(nb_elt);
  }

  //--------------------------------------------------------------------------
  //! \brief Return a boolean indicating if some elements of the container
  //! are dirty (modified).
  //!
  //! \return true if data are dirty.
  //--------------------------------------------------------------------------
  inline bool hasPendingData() const
  {
    return (npos != m_pending_start);
  }

  //--------------------------------------------------------------------------
  //! \brief Return the smallest contiguous area of the container having
  //! dirty elements.
  //!
  //! In the case there is no pending data, the return area bound is logicaly
  //! undefined but we return pos_start and pos_end are set to \c npos. You
  //! can call hasPendingData() before calling this method.
  //--------------------------------------------------------------------------
  void getPendingData(size_t& pos_start, size_t& pos_end) const
  {
    pos_start = m_pending_start;
    pos_end = m_pending_end;
  }

  //--------------------------------------------------------------------------
  //! \brief Return the smallest contiguous area of the container having
  //! dirty elements.
  //!
  //! In the case there is no pending data, the return area bound is logicaly
  //! undefined but we return pos_start and pos_end are set to \c npos. You
  //! can call hasPendingData() before calling this method.
  //!
  //! \return {pos_start, pos_end}
  //--------------------------------------------------------------------------
  std::pair<size_t, size_t> getPendingData() const
  {
    return { m_pending_start, m_pending_end };
  }

  //--------------------------------------------------------------------------
  //! \brief Reset the smallest contiguous area of the container having
  //! dirty elements.
  //!
  //! Call this the smallest contiguous area have been updated and therefore
  //! when there is no more dirty data.
  //--------------------------------------------------------------------------
  void clearPending()
  {
    m_pending_start = npos;
    m_pending_end = npos;
  }

  //--------------------------------------------------------------------------
  //! \brief Reset the smallest contiguous area of the container having dirty
  //! elements and tag the \c nb_elt first elements as dirty. Use this method
  //! from constructors.
  //--------------------------------------------------------------------------
  void clearPending(size_t const nb_elt)
  {
    if (0_z == nb_elt)
      {
        m_pending_start = npos;
        m_pending_end = npos;
      }
    else
      {
        m_pending_start = 0_z;
        m_pending_end = nb_elt;
      }
  }

  //--------------------------------------------------------------------------
  //! \brief Update the area of dirty elements with a new range.
  //--------------------------------------------------------------------------
  void tagAsPending(size_t const pos_start, size_t const pos_end)
  {
    if (likely(hasPendingData()))
      {
        m_pending_start = std::min(m_pending_start, pos_start);
        m_pending_end = std::max(m_pending_end, pos_end);
      }
    else
      {
        m_pending_start = pos_start;
        m_pending_end = pos_end;
      }
  }

  //--------------------------------------------------------------------------
  //! \brief Update the area of dirty elements by passing the newly dirty
  //! element.
  //--------------------------------------------------------------------------
  inline void tagAsPending(size_t const pos)
  {
    if (likely(hasPendingData()))
      {
        m_pending_start = std::min(m_pending_start, pos);
        m_pending_end = std::max(m_pending_end, pos);
      }
    else
      {
        m_pending_start = pos;
        m_pending_end = pos;
      }
  }

protected:

  //! Tracks the staring bound of elementsthat  have been changed.
  size_t m_pending_start = npos;

  //! Tracks the ending bound of elementsthat  have been changed.
  size_t m_pending_end = npos;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PENDINGDATA_HPP
