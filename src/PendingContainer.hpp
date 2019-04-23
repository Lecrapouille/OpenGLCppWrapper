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

#ifndef OPENGLCPPWRAPPER_PENDING_CONTAINER_HPP
#  define OPENGLCPPWRAPPER_PENDING_CONTAINER_HPP

#  include "PendingData.hpp"
#  include <vector>
#  include <algorithm>
#  include <cmath>
#  include <fstream>

namespace glwrap
{

// **************************************************************
//! \brief PendingContainer is a std::valarray memorizing elements
// which have been changed and has to updated.
// **************************************************************
template<class T>
class PendingContainer: public /*protected*/ PendingData
{
public:

  PendingContainer()
    : PendingData()
  {}

  PendingContainer(std::size_t count)
    : PendingData(count),
      m_container(count)
  {}

  PendingContainer(std::size_t count, T const& val)
    : PendingData(count),
      m_container(count, val)
  {}

 PendingContainer(PendingContainer const& other)
   : PendingData(other.getPendingData()),
      m_container(other.m_container)
  {
  }

  PendingContainer(std::vector<T> const& other)
    : PendingData(other.size()),
      m_container(other)
  {}

   PendingContainer(std::initializer_list<T> il)
     : PendingData(il.size()),
       m_container(il)
  {}

  ~PendingContainer()
  {}

  inline size_t capacity() const
  {
    return m_container.capacity();
  }

  inline size_t size() const
  {
    return m_container.size();
  }

  inline void reserve(std::size_t count)
  {
    throw_if_cannot_expand();
    m_container.reserve(count);
  }

  inline T& operator[](size_t nth)
  {
    if (unlikely(nth > m_container.capacity()))
      {
        reserve(nth);
      }
    tagAsPending(nth);
    //std::cout << "tagPend " << std::endl;
    return m_container.operator[](nth);
  }

  inline T const& operator[](size_t nth) const
  {
    return m_container.operator[](nth);
  }

  inline T& at(size_t nth)
  {
    return m_container.at(nth);
  }

  inline T const& at(size_t nth) const
  {
    return m_container.at(nth);
  }

  void append(std::initializer_list<T> il)
  {
    throw_if_cannot_expand();
    //size_t s = std::max(1_z, m_container.size()) - 1_z;
    m_container.insert(m_container.end(), il);
    tagAsPending(/*s*/0_z, m_container.size() - 1_z);
  }

  void append(std::vector<T> const& other)
  {
    throw_if_cannot_expand();
    m_container.insert(m_container.end(),
                       other.begin(),
                       other.end());
    tagAsPending(0_z, m_container.size() - 1_z);
  }

  void append(PendingContainer const& other)
  {
    PendingContainer<T>::append(other.m_container);
  }

  void append(T const& val)
  {
    throw_if_cannot_expand();

    m_container.push_back(val);
    tagAsPending(0_z, m_container.size() - 1_z);
  }

  inline T sum() const
  {
    T sum_of_elems = 0;

    for (auto& n : m_container)
      sum_of_elems += n;

    return sum_of_elems;
  }

  inline T min() const
  {
    return std::min_element(m_container.begin(), m_container.end());
  }

  inline T max() const
  {
    return std::max_element(m_container.begin(), m_container.end());
  }

  template<class Function>
  inline PendingContainer<T>& apply(Function& f)
  {
    clearPending(m_container.size());
    std::for_each(m_container.begin(), m_container.end(), f);
    return *this;
  }

  inline void abs() const
  {
    apply([](T& x){ std::abs(x); });
  }

  inline void sqrt() const
  {
    apply([](T& x){ std::sqrt(x); });
  }

  inline void sin() const
  {
    apply([](T& x){ std::sin(x); });
  }

  inline void cos() const
  {
    apply([](T& x){ std::cos(x); });
  }

#if 0
  inline PendingContainer<T>& operator=(PendingContainer<T> const& other)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = other.m_container.size();

    if (other_size > my_size)
      throw_if_cannot_expand();

    m_container = other.m_container;
    tagAsPending(0_z, other_size - 1_z);
    return *this;
  }

  inline PendingContainer<T>& operator=(std::initializer_list<T> il)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = il.size();

    if (other_size > my_size)
      throw_if_cannot_expand();

    m_container = il;
    tagAsPending(0_z, other_size - 1_z);
    return *this;
  }
#endif

  inline PendingContainer<T>& operator*=(T const& val)
  {
    //FIXME return apply([val](T& x){ x *= val; });
    for (auto& x: m_container)
      x *= val;
    return *this;
  }

  inline PendingContainer<T>& operator+=(T const& val)
  {
    //FIXME return apply([val](T& x){ x += val; });
    for (auto& x: m_container)
      x += val;
    return *this;
  }

  inline PendingContainer<T>& operator-=(T const& val)
  {
    //FIXME return apply([val](T& x){ x -= val; });
    for (auto& x: m_container)
      x -= val;
    return *this;
  }

  inline PendingContainer<T>& operator/=(T const& val)
  {
    constexpr T inv = T(1) / val;
    return PendingContainer<T>::operator*=(inv);
  }

  friend std::ostream& operator<<(std::ostream& stream, const PendingContainer& cont)
  {
    stream << "PendingContainer: ";
    if (cont.size())
      {
        stream << cont[0];
        for (size_t i = 1_z; i < cont.size(); ++i)
          stream << ", " << cont[i];
      }
    return stream;
  };

  inline const T* to_array() const
  {
    return &m_container[0];
  }

  //private:
protected:

  inline void throw_if_cannot_expand()
  {
     if (likely(!m_can_expand))
       throw std::out_of_range("Cannot change buffer size once loaded on GPU");
  }

  inline void set_cannot_expand()
  {
    m_can_expand = false;
  }

  bool m_can_expand = true;
  std::vector<T> m_container;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PENDING_CONTAINER_HPP
