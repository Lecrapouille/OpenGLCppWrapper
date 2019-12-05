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

#  include "Math/Maths.hpp"
#  include "Common/GPUMemory.hpp"
#  include "Common/PendingData.hpp"
#  include <vector>
#  include <algorithm>
#  include <cmath>
#  include <fstream>

namespace glwrap
{

// *****************************************************************************
//! \brief PendingContainer is a class over std::vector for memorizing elements
//! and knowing which elements have been modified (pending data).
//!
//! This class is used as buffer between CPU and GPU where pending elements are
//! flushed to the GPU memory. Mostly method impacting on the container size
//! have a side effect: they modify the estimator on the GPU memory usage for
//! the current running application.
// *****************************************************************************
template<class T>
class PendingContainer: public PendingData
{
  // ***************************************************************************
  //! \brief Allow to PendingContainer to set or get an element in the container
  //! when using the operator[].
  //!
  //! operator[] when not const will always be used as a setter even when it is
  //! supposed to call a getter. For example a = std::container[0] will call the
  //! std::container::set() method while desired to call the get() method (in
  //! the same way than (PendingContainer[0] = a).
  //!
  //! For PendingContainer this is not a desirable behavior because this will
  //! set the nth element as dirty (pending data) and imply a flush to the GPU
  //! memory in the case of VBO.
  //!
  //! To allow C++ to understand to correctly select setter getter to use this
  //! class is needed.
  // ***************************************************************************
  struct Deref
  {
    PendingContainer& m_ref;
    size_t m_index;

    //! \brief Constructor takes a reference of PendingContainer and the nth
    //! element of the container.
    explicit Deref(PendingContainer& container, size_t const nth)
      : m_ref(container), m_index(nth)
    {}

    //! \brief Getter method.
    inline operator T() const
    {
      return m_ref.get(m_index);
    }

    //! \brief Setter method.
    inline T& operator=(T const& other)
    {
      return m_ref.set(m_index) = other;
    }

    //! \brief This method allow to write std::cout << PendingContainer[0]
    inline friend std::ostream& operator<<(std::ostream& os, const Deref& d)
    {
      return os << d.m_ref.get(d.m_index);
    }
  };

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Do nothing.
  //----------------------------------------------------------------------------
  PendingContainer()
  {}

  //----------------------------------------------------------------------------
  //! \brief Constructor and reserve the correct amount of elements.
  //!
  //! \note: hack! We call std::vector::reserve(count) but
  //! not :std::vector:resize(count) or pass count to the std::vector
  //! constructor (which called resize) because we do not want, for VBOs, to
  //! transfer invalid dummy data to the GPU.
  //----------------------------------------------------------------------------
  explicit PendingContainer(size_t const count)
  {
    m_container.reserve(count);
    GPUMemory() += memory();
    DEBUG("'%s': Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
          cdebug(), count, sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  explicit PendingContainer(size_t const count, T const& val)
    : PendingData(count),
      m_container(count, val)
  {
    GPUMemory() += memory();
    DEBUG("'%s': Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
          cdebug(), count, sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  explicit PendingContainer(PendingContainer<T> const& other)
    : PendingData()
  {
    // Copy container as well as the capacity
    m_container.reserve(other.capacity());
    m_container = other.m_container;

    // Set pending data
    const auto bound = other.getPendingData();
    tagAsPending(bound.first, bound.second);

    GPUMemory() += memory();
    DEBUG("'%s': Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
          cdebug(), other.size(), sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  explicit PendingContainer(std::vector<T> const& other)
    : PendingData(other.size()),
      m_container(other)
  {
    GPUMemory() += memory();
    DEBUG("'%s': Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
          cdebug(), other.size(), sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  explicit PendingContainer(std::initializer_list<T> il)
    : PendingData(il.size()),
      m_container(il)
  {
    GPUMemory() += memory();
    DEBUG("'%s': Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
          cdebug(), il.size(), sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor.
  //----------------------------------------------------------------------------
  ~PendingContainer()
  {
    GPUMemory() -= memory();
    DEBUG("'%s': Removing %zu bytes. GPU memory: %zu bytes",
          cdebug(), memory(), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief Return the container current capacity.
  //----------------------------------------------------------------------------
  inline size_t capacity() const
  {
    return m_container.capacity();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the number of elements inside the container.
  //----------------------------------------------------------------------------
  inline size_t size() const
  {
    return m_container.size();
  }

  //----------------------------------------------------------------------------
  //! \brief return the container size in bytes.
  //----------------------------------------------------------------------------
  inline size_t memory() const
  {
    DEBUG("'%s': Internal GPU memory: %zu bytes",
          cdebug(), sizeof (T) * m_container.size());
    return sizeof (T) * m_container.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Reserve the container when this is possible.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  inline void reserve(size_t const count)
  {
    throw_if_cannot_expand();
    m_container.reserve(count);
  }

  //----------------------------------------------------------------------------
  //! \brief Resize the container when this is possible.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  inline void resize(size_t const count)
  {
    size_t old_count = size();

    // Same size: ignore!
    if (unlikely((0_z == count) || (count == old_count)))
      return ;

    // Resize the container
    throw_if_cannot_expand();
    m_container.resize(count);
    // FIXME not optimized concerning m_pending_start
    tagAsPending(0_z, m_container.size());

    // Case of container is reduced: update pending data
    if (unlikely(count < old_count))
      {
        if (0_z == count)
          {
            clearPending();
          }
        else
          {
            size_t pos_start;
            size_t pos_end;
            getPendingData(pos_start, pos_end);
            clearPending();
            tagAsPending(std::min(pos_start, count), std::min(pos_end, count));
          }
      }

    GPUMemory() += (count * sizeof (T) - memory());
    DEBUG("'%s': Resizing %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), count, sizeof (T), GPUMemory().load());
  }

  //----------------------------------------------------------------------------
  //! \brief Call either the set() or the get() methods.
  //!
  //! \code
  //! container[0] = a; will call set()
  //! a = container[0]; will call get()
  //! \endcode
  //----------------------------------------------------------------------------
  Deref operator[](size_t const index)
  {
    return Deref(*this, index);
  }

  //----------------------------------------------------------------------------
  //! \brief Setter of the nth element. Resize the container to nth + 1 elements
  //! when needed.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  inline T& set(size_t const nth)
  {
    if (unlikely(nth >= m_container.size()))
      {
        throw_if_cannot_expand();
        m_container.capacity();
        m_container.resize(nth + 1_z);
        // FIXME not optimized concerning m_pending_start
        tagAsPending(0_z, m_container.size());

        GPUMemory() += sizeof (T);
        DEBUG("'%s': Resizing %zu elements of size %zu bytes. GPU memory: %zu bytes",
              cdebug(), 1_z, sizeof (T), GPUMemory().load());
      }
    else
      {
        tagAsPending(nth);
      }
    return m_container[nth];
  }

  //----------------------------------------------------------------------------
  //! \brief Getter of the nth element.
  //! \throw std::out_of_range when trying to access out of bound index.
  //----------------------------------------------------------------------------
  inline T const& get(size_t const nth) const
  {
    return m_container.at(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Remove all elements of the container.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  void clear()
  {
    throw_if_cannot_expand();
    GPUMemory() -= memory();
    DEBUG("'%s': Clearing %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), size(), sizeof (T), GPUMemory().load());
    m_container.clear();
    clearPending(0_z);
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  append(std::initializer_list<T> il)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(), il);
    tagAsPending(start, m_container.size());
    GPUMemory() += il.size() * sizeof (T);
    DEBUG("'%s': Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), il.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  append(const T* other, size_t const size)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(),
                       other,
                       other + size);
    tagAsPending(start, m_container.size());
    GPUMemory() += size * sizeof (T);
    DEBUG("'%s': Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), size, sizeof (T), GPUMemory().load());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  append(std::vector<T> const& other)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(),
                       other.begin(),
                       other.end());
    tagAsPending(start, m_container.size());
    GPUMemory() += other.size() * sizeof (T);
    DEBUG("'%s': Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), other.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  append(PendingContainer const& other)
  {
    return PendingContainer<T>::append(other.m_container);
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers.
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  append(T const& val)
  {
    throw_if_cannot_expand();

    m_container.push_back(val);
    tagAsPending(0_z, m_container.size());
    GPUMemory() += sizeof (T);
    DEBUG("'%s': Appending 1 element of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), sizeof (T), GPUMemory().load());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers holding indexes of VBOs. This method shall
  //! only called for EBOs.
  //!
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
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

    DEBUG("'%s': AppendIndex => older_index %zu",
          cdebug(), static_cast<size_t>(older_index));

    m_container.reserve(other.size());
    for (auto it: other)
      {
        m_container.push_back(it + older_index);
      }
    tagAsPending(m_container.size());
    GPUMemory() += other.size() * sizeof (T);
    DEBUG("'%s': AppendingIndex %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), other.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Concat two containers holding indexes of VBOs. This method shall
  //! only called for EBOs.
  //!
  //! \throw std::out_of_range if the container cannot be resized.
  //----------------------------------------------------------------------------
  PendingContainer<T>&
  appendIndex(PendingContainer const& other)
  {
    return PendingContainer<T>::appendIndex(other.m_container);
  }

  //----------------------------------------------------------------------------
  //! \brief Compute the summation of all elements in the container.
  //! \throw std::out_of_range if the container has no elements.
  //----------------------------------------------------------------------------
  inline T sum() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        mythrow("Cannot compute the summation of an empty container");
      }

    T sum_of_elems = 0;
    for (auto& n : m_container)
      {
        sum_of_elems += n;
      }

    return sum_of_elems;
  }

  //----------------------------------------------------------------------------
  //! \brief Compute the product of all elements in the container.
  //! \throw std::out_of_range if the container has no elements.
  //----------------------------------------------------------------------------
  inline T prod() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        mythrow("Cannot compute the product of an empty container");
      }

    T prod_of_elems = 1;

    for (auto& n : m_container) {
      prod_of_elems *= n;
    }

    return prod_of_elems;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the lower element of the container.
  //! \throw std::out_of_range if the container has no elements.
  //----------------------------------------------------------------------------
  inline T min() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        mythrow("Cannot compute the min of an empty container");
      }
    return *std::min_element(m_container.begin(), m_container.end());
  }

  //----------------------------------------------------------------------------
  //! \brief Return the greater element of the container.
  //! \throw std::out_of_range if the container has no elements.
  //----------------------------------------------------------------------------
  inline T max() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        mythrow("Cannot compute the max of an empty container");
      }
    return *std::max_element(m_container.begin(), m_container.end());
  }

  //----------------------------------------------------------------------------
  //! \brief Compute the functor \p f for each element of the container. The
  //! whole container is set a dirty.
  //----------------------------------------------------------------------------
  template<class Function>
  inline PendingContainer<T>& apply(Function f)
  {
    clearPending(m_container.size());
    std::for_each(m_container.begin(), m_container.end(), f);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Compute absolute value for each element of the container. The whole
  //! container is set a dirty.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& abs()
  {
    return apply([](T& x){ x = maths::abs(x); });
  }

  //----------------------------------------------------------------------------
  //! \brief Compute square root for each element of the container. The whole
  //! container is set a dirty.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& sqrt()
  {
    return apply([](T& x){ x = std::sqrt(x); });
  }

  //----------------------------------------------------------------------------
  //! \brief Compute ^2 for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& squared()
  {
    return apply([](T& x){ x = x * x; });
  }

  //----------------------------------------------------------------------------
  //! \brief Compute sinus for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& sin()
  {
    return apply([](T& x){ x = std::sin(x); });
  }

  //----------------------------------------------------------------------------
  //! \brief Compute cosinus for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& cos()
  {
    return apply([](T& x){ x = std::cos(x); });
  }

  //----------------------------------------------------------------------------
  //! \brief Copy operator.
  //!
  //! \throw std::out_of_range if \p other has more elements and the container
  //! cannot be resized.
  //----------------------------------------------------------------------------
  inline PendingContainer<T>& operator=(PendingContainer<T> const& other)
  {
    return this->operator=(other.m_container);
  }

  //----------------------------------------------------------------------------
  //! \brief Copy operator from a std::vector. Impacted elements are set dirty.
  //!
  //! \throw std::out_of_range if the vector has more elements and the container
  //! cannot be resized.
  //----------------------------------------------------------------------------
  template<class U>
  PendingContainer<T>& operator=(std::vector<U> const& other)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = other.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    GPUMemory() += (other.size() * sizeof (T) - memory());
    DEBUG("Affecting %zu elements of size %zu bytes. GPU memory: %zu bytes",
          other.size(), sizeof (T), GPUMemory().load());

    m_container = other;
    tagAsPending(0_z, other_size);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Copy operator from an initializer list. Impacted elements are set
  //! dirty.
  //!
  //! \throw std::out_of_range if initializer list has more elements and the
  //! container cannot be resized.
  //----------------------------------------------------------------------------
  template<class U>
  PendingContainer<T>& operator=(std::initializer_list<U> il)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = il.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    GPUMemory() += (il.size() * sizeof (T) - memory());
    DEBUG("'%s': Affecting %zu elements of size %zu bytes. GPU memory: %zu bytes",
          cdebug(), il.size(), sizeof (T), GPUMemory().load());

    m_container = il;
    tagAsPending(0_z, other_size);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Compute * val for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  template<class U>
  inline PendingContainer<T>& operator*=(U const& val)
  {
    //FIXME return apply([val](T& x){ x *= val; });
    for (auto& x: m_container) {
      x *= val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  template<class U>
  inline PendingContainer<T>& operator+=(U const& val)
  {
    //FIXME return apply([val](T& x){ x += val; });
    for (auto& x: m_container) {
      x += val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Compute - val for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  template<class U>
  inline PendingContainer<T>& operator-=(U const& val)
  {
    //FIXME return apply([val](T& x){ x -= val; });
    for (auto& x: m_container) {
      x -= val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Compute / val for each element of the container. The whole container
  //! is set a dirty.
  //----------------------------------------------------------------------------
  template<class U>
  inline PendingContainer<T>& operator/=(U const& val)
  {
    return PendingContainer<T>::operator*=(U(1) / val);
  }

  //----------------------------------------------------------------------------
  //! \brief Display the content of the container on the console.
  //----------------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& stream, const PendingContainer& cont)
  {
    stream << "PendingContainer:" << cont.size()
           << ':' << cont.cdebug() << ": ";

    if (likely(0_z != cont.size()))
      {
        stream << cont.m_container[0];
        for (size_t i = 1_z; i < cont.size(); ++i) {
          stream << ", " << cont.m_container[i];
        }
      }
    return stream;
  };

  //----------------------------------------------------------------------------
  //! \brief Return the container as read-write array.
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //! \brief Return the container as read only array.
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //! \brief Return the container in read write access. FIXME should not be const ?
  //----------------------------------------------------------------------------
  inline std::vector<T>& data()
  {
    return m_container;
  }

  void setDebugName(std::string const& n)
  {
    m_debug = n;
  }

protected:

  //----------------------------------------------------------------------------
  //! \brief Throw std::out_of_range if the container cannot be resied and the
  //! user tried to resize it.
  //----------------------------------------------------------------------------
  inline void throw_if_cannot_expand()
  {
    if (likely(!m_can_expand)) {
      mythrow("Cannot change buffer size once loaded on GPU");
    }
  }

  //----------------------------------------------------------------------------
  //! \brief Forbidding resizing the container. For example once the associated
  //! VBOs (GPU memory) is allocated and cannot be resized.
  //----------------------------------------------------------------------------
  inline void set_cannot_expand()
  {
    m_can_expand = false;
  }

  //! \brief The container holding elements.
  std::vector<T> m_container;

  //! \brief When set to true the m_container can increase its size, else not
  //! which produce an exception. Forbidding resizing the container is needed
  //! because VBOs cannot be resized.
  bool m_can_expand = true;

private:

  //----------------------------------------------------------------------------
  //! \brief Wrapper method on throw std::out_of_range for logging more information.
  //----------------------------------------------------------------------------
  void mythrow(std::string const& msg) const
  {
    ERROR("'%s': throw %s", cdebug(), msg.c_str());

    std::string full_msg(m_debug + ": " + msg);
    throw std::out_of_range(full_msg.c_str());
  }

  //----------------------------------------------------------------------------
  //! \brief Return the degun name as char* for logs.
  //----------------------------------------------------------------------------
  inline const char* cdebug() const
  {
    return m_debug.c_str();
  }

  std::string m_debug = "PC";
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PENDING_CONTAINER_HPP
