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

  PendingContainer(size_t count)
    : PendingData(count),
      m_container(count)
  {}

  PendingContainer(size_t count, T const& val)
    : PendingData(count),
      m_container(count, val)
  {}

 PendingContainer(PendingContainer const& other)
   : PendingData(other.getPendingData()),
      m_container(other.m_container)
  {}

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

  inline void reserve(const size_t count)
  {
    throw_if_cannot_expand();
    m_container.reserve(count);
  }

  inline void resize(const size_t count)
  {
    throw_if_cannot_expand();
    m_container.resize(count);
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

  inline T& at(const size_t nth)
  {
    return m_container.at(nth);
  }

  inline T const& at(const size_t nth) const
  {
    return m_container.at(nth);
  }

  void clear()
  {
    throw_if_cannot_expand();
    m_container.clear();
    clearPending(0_z);
  }

  PendingContainer<T>&
  append(std::initializer_list<T> il)
  {
    throw_if_cannot_expand();
    m_container.insert(m_container.end(), il);
    tagAsPending(m_container.size() - 1_z);
    return *this;
  }

  PendingContainer<T>&
  append(const T* other, const size_t size)
  {
    throw_if_cannot_expand();
    m_container.insert(m_container.end(),
                       other,
                       other + size);
    tagAsPending(m_container.size() - 1_z);
    return *this;
  }

  PendingContainer<T>&
  append(std::vector<T> const& other)
  {
    throw_if_cannot_expand();

    m_container.insert(m_container.end(),
                       other.begin(),
                       other.end());
    tagAsPending(m_container.size() - 1_z);
    return *this;
  }

  PendingContainer<T>&
  append(PendingContainer const& other)
  {
    return PendingContainer<T>::append(other.m_container);
  }

  PendingContainer<T>&
  append(T const& val)
  {
    throw_if_cannot_expand();

    m_container.push_back(val);
    tagAsPending(0_z, m_container.size() - 1_z);
    return *this;
  }

  //! \brief Append EBO
  PendingContainer<T>&
  appendIndex(std::vector<T> const& other)
  {
    throw_if_cannot_expand();

    T older_index{0};
    if (likely(0_z != size()))
      {
        older_index = max();
        older_index += T(1);
      }

    DEBUG("appendIndex => older_index %zu", static_cast<size_t>(older_index));

    m_container.reserve(other.size());
    for (auto it: other)
      {
        m_container.push_back(it + older_index);
      }
    tagAsPending(m_container.size() - 1_z);
    return *this;
  }

  PendingContainer<T>&
  appendIndex(PendingContainer const& other)
  {
    return PendingContainer<T>::appendIndex(other.m_container);
  }

  inline T sum() const
  {
    T sum_of_elems = 0;

    for (auto& n : m_container) {
      sum_of_elems += n;
    }

    return sum_of_elems;
  }

  inline T min() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the min of an empty container");
      }
    return *std::min_element(m_container.begin(), m_container.end());
  }

  inline T max() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the max of an empty container");
      }
    return *std::max_element(m_container.begin(), m_container.end());
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
    return this->operator=(other.m_container);
  }

  template<class U>
  inline PendingContainer<T>& operator=(std::vector<U> const& other)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = other.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    m_container = other;
    tagAsPending(0_z, other_size - 1_z);
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator=(std::initializer_list<U> il)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = il.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    m_container = il;
    tagAsPending(0_z, other_size - 1_z);
    return *this;
  }
#endif

  template<class U>
  inline PendingContainer<T>& operator*=(U const& val)
  {
    //FIXME return apply([val](T& x){ x *= val; });
    for (auto& x: m_container) {
      x *= val;
    }
    tagAsPending(0_z, m_container.size() - 1_z);
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator+=(U const& val)
  {
    //FIXME return apply([val](T& x){ x += val; });
    for (auto& x: m_container) {
      x += val;
    }
    tagAsPending(0_z, m_container.size() - 1_z);
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator-=(U const& val)
  {
    //FIXME return apply([val](T& x){ x -= val; });
    for (auto& x: m_container) {
      x -= val;
    }
    tagAsPending(0_z, m_container.size() - 1_z);
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator/=(U const& val)
  {
    return PendingContainer<T>::operator*=(U(1) / val);
  }

  friend std::ostream& operator<<(std::ostream& stream, const PendingContainer& cont)
  {
    stream << "PendingContainer: ";
    if (cont.size())
      {
        stream << cont[0];
        for (size_t i = 1_z; i < cont.size(); ++i) {
          stream << ", " << cont[i];
        }
      }
    return stream;
  };

  inline T* to_array()
  {
    if (likely(0_z != m_container.size()))
      {
        return m_container.data();
      }
    else
      {
        return nullptr;
      }
  }

  inline const T* to_array() const
  {
    if (likely(0_z != m_container.size()))
      {
        return m_container.data();
      }
    else
      {
        return nullptr;
      }
  }

  inline std::vector<T>& data()
  {
    return m_container;
  }

  //private:
protected:

  inline void throw_if_cannot_expand()
  {
    if (likely(!m_can_expand)) {
      throw std::out_of_range("Cannot change buffer size once loaded on GPU");
    }
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
