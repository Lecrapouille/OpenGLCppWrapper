// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef PENDING_CONTAINER_HPP_
#  define PENDING_CONTAINER_HPP_

#  include "PendingData.hpp"
#  include <valarray>

// **************************************************************
//! \brief PendingContainer is a std::valarray memorizing elements
// which have been changed and has to updated.
// **************************************************************
template<class T>
class PendingContainer: public /*protected*/ PendingData
{
public:

  ~PendingContainer() {}
  PendingContainer() : PendingData() {}
  explicit PendingContainer(std::size_t count) : PendingData(count), m_container(count) {}
  PendingContainer(const T& val, std::size_t count) : PendingData(count), m_container(val, count) {}
  PendingContainer(const T* vals, std::size_t count) : PendingData(count), m_container(vals, count) {}
  PendingContainer(const std::valarray<T>& other) : PendingData(other.size()), m_container(other) {}
  PendingContainer(std::valarray<T>&& other) noexcept : PendingData(other.size()), m_container(other) {}
  PendingContainer(const std::slice_array<T>& sa) : PendingData(sa.size()), m_container(sa) {}
  PendingContainer(const std::gslice_array<T>& ga) : PendingData(ga.size()), m_container(ga) {}
  PendingContainer(const std::mask_array<T>& ma) : PendingData(ma.size()), m_container(ma) {}
  PendingContainer(const std::indirect_array<T>& ia) : PendingData(ia.size()), m_container(ia) {}
  PendingContainer(std::initializer_list<T> il) : PendingData(il.size()), m_container(il) {}

  inline const T& operator[](size_t nth) const { return m_container.operator[](nth); }
  inline T& operator[](size_t nth)
  {
     if (nth > PendingContainer<T>::capacity())
      {
        throw_if_cannot_expand();
        m_container.resize(nth);
      }
    tagAsPending(nth);
    return m_container.operator[](nth);
  }

  friend std::ostream& operator<<(std::ostream& stream, const PendingContainer& cont)
  {
    if (0_z != cont.size())
      {
        stream << cont[0];
        for (size_t i = 1; i < cont.size(); ++i)
          stream << ", " << cont[i];
      }
    return stream;
  }

  inline void throw_if_cannot_expand()
  {
     if (likely(!m_can_expand))
       throw std::out_of_range("Cannot change buffer size once loaded on GPU");
  }

  inline void set_cannot_expand()
  {
    m_can_expand = false;
  }

  void append(std::initializer_list<T> il)
  {
    size_t s = m_container.size();
    m_container.resize(s + il.size());
    auto iter = il.begin();
    for (size_t i = 0; i < il.size(); ++i)
      {
        m_container[s + i] = *iter;
        ++iter;
      }
    tagAsPending(0, s + il.size());
  }

  void append(const T *array, const size_t size)
  {
    size_t s = m_container.size();
    m_container.resize(s + size);
    for (size_t i = 0; i < size; ++i)
      {
        m_container[s + i] = array[i];
      }
    tagAsPending(0, s + size);
  }

  inline size_t capacity() const { return m_container.size(); } // capacity() does not exist
  inline size_t size() const { return m_container.size(); }
  inline T sum() const { return m_container.sum(); }
  inline T min() const { return m_container.min(); }
  inline T max() const { return m_container.max(); }

  inline std::valarray<T>& operator*=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator*=(rhs); }
  inline std::valarray<T>& operator/=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator/=(rhs); }
  inline std::valarray<T>& operator%=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator%=(rhs); }
  inline std::valarray<T>& operator+=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator+=(rhs); }
  inline std::valarray<T>& operator-=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator-=(rhs); }
  inline std::valarray<T>& operator^=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator^=(rhs); }
  inline std::valarray<T>& operator&=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator&=(rhs); }
  inline std::valarray<T>& operator|=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator|=(rhs); }
  inline std::valarray<T>& operator<<=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator<<=(rhs); }
  inline std::valarray<T>& operator>>=(const std::valarray<T>& rhs)
  { clearPending(m_container.size()); return m_container.operator>>=(rhs); }

  inline std::valarray<T>& operator*=(const T& val)
  { clearPending(m_container.size()); return m_container.operator*=(val); }
  inline std::valarray<T>& operator/=(const T& val)
  { clearPending(m_container.size()); return m_container.operator/=(val); }
  inline std::valarray<T>& operator%=(const T& val)
  { clearPending(m_container.size()); return m_container.operator%=(val); }
  inline std::valarray<T>& operator+=(const T& val)
  { clearPending(m_container.size()); return m_container.operator+=(val); }
  inline std::valarray<T>& operator-=(const T& val)
  { clearPending(m_container.size()); return m_container.operator-=(val); }
  inline std::valarray<T>& operator^=(const T& val)
  { clearPending(m_container.size()); return m_container.operator^=(val); }
  inline std::valarray<T>& operator&=(const T& val)
  { clearPending(m_container.size()); return m_container.operator&=(val); }
  inline std::valarray<T>& operator|=(const T& val)
  { clearPending(m_container.size()); return m_container.operator|=(val); }
  inline std::valarray<T>& operator<<=(const T& val)
  { clearPending(m_container.size()); return m_container.operator<<=(val); }
  inline std::valarray<T>& operator>>=(const T& val)
  { clearPending(m_container.size()); return m_container.operator>>=(val); }
  inline std::valarray<T>& operator=(const std::valarray<T>& other)
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(std::valarray<T>&& other) noexcept
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(const T& val)
  { clearPending(m_container.size()); return m_container.operator=(val); }
  inline std::valarray<T>& operator=(const std::slice_array<T>& other)
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(const std::gslice_array<T>& other)
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(const std::mask_array<T>& other)
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(const std::indirect_array<T>& other)
  { clearPending(other.size()); return m_container.operator=(other); }
  inline std::valarray<T>& operator=(std::initializer_list<T> il)
  { LOGD("PendingContainer initializer list"); clearPending(il.size()); return m_container.operator=(il); }

  std::valarray<T> m_container;

private:

  bool m_can_expand = true;

};

#endif
