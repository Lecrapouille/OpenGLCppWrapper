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
// OpenGLCppWrapper is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================
//
// This file is a derived work of
// https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/
//=====================================================================

#ifndef CONTAINER_ANY_HPP
#  define CONTAINER_ANY_HPP

#  include <unordered_map>
#  include <map>
#  include <vector>
#  include <string>
#  include <functional>
#  include <experimental/type_traits>

// *****************************************************************************
//! \brief Container typed of std::map but for storing heterogeneous elements.
//! Based on original code source:
//! https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/
//! Need C++14 since using variable template
// *****************************************************************************
struct Any
{
public:

    Any() = default;

    //--------------------------------------------------------------------------
    //! \brief Copy constructor.
    //--------------------------------------------------------------------------
    Any(Any const& other)
    {
       *this = other;
    }

    //--------------------------------------------------------------------------
    //! \brief Copy operator.
    //--------------------------------------------------------------------------
    Any& operator=(Any const& other)
    {
        clear();
        m_clear_functions = other.m_clear_functions;
        m_copy_functions = other.m_copy_functions;
        m_size_functions = other.m_size_functions;

        for (auto&& copy_function : m_copy_functions)
        {
            copy_function(other, *this);
        }
        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Destructor. Call clear().
    //--------------------------------------------------------------------------
    ~Any()
    {
        clear();
    }

    //--------------------------------------------------------------------------
    //! \brief Clear the whole container. All elements are removed.
    //--------------------------------------------------------------------------
    void clear()
    {
        for (auto&& clear_func : m_clear_functions)
        {
            clear_func(*this);
        }
        m_clear_functions.clear();
        m_copy_functions.clear();
        m_size_functions.clear();
    }

    //--------------------------------------------------------------------------
    //! \brief Return the number of elements of type T.
    //--------------------------------------------------------------------------
    template<class T>
    size_t count() const
    {
        auto iter = s_items<T>.find(this);
        if (iter != s_items<T>.cend())
            return s_items<T>[this].size();
        return 0;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the total number of elements inside the container.
    //--------------------------------------------------------------------------
    size_t size() const
    {
        size_t sum = 0u;
        for (auto&& size_func : m_size_functions)
        {
            sum += size_func(*this);
        }
        return sum;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the std::map<std::string, T> for the given template T.
    //--------------------------------------------------------------------------
    template <class T>
    std::map<std::string, T>& map()
    {
        return s_items<T>[this];
    }

    //--------------------------------------------------------------------------
    //! \brief Insert the element \c t of type T to the \c key slot.
    //--------------------------------------------------------------------------
    template <class T>
    T& add(const char* key, const T& t)
    {
        // Don't have it yet ? So create functions for destroying
        if (s_items<T>.find(this) == std::end(s_items<T>))
        {
            m_clear_functions.emplace_back([](Any& a)
            {
                s_items<T>.erase(&a);
            });

            m_copy_functions.emplace_back([](Any const& from, Any& to)
            {
                s_items<T>[&to] = s_items<T>[&from];
            });

            m_size_functions.emplace_back([](Any const& a)
            {
                return s_items<T>[&a].size();
            });
        }

        // Insert
        return s_items<T>[this][key] = t;
    }

    //--------------------------------------------------------------------------
    //! \brief Get the element of type T given its key.
    //--------------------------------------------------------------------------
    template <class T>
    T& get(const char* key)
    {
        return s_items<T>[this].at(key);
    }

    template <class T>
    bool has(const char* key)
    {
        return s_items<T>[this].find(key) != s_items<T>[this].end();
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Container for any type of elements. It uses variable template so
    //! it will need C++14 to compile.
    //!
    //! Variable templates create separate variables. The static keyword allows
    //! to create a new member std::map<std::string, T> across all class
    //! instances. But since we only want to care about the types that are used
    //! for that specific instancen we have to use a map of pointers to Any as
    //! keys.
    //--------------------------------------------------------------------------
    template<class T>
    static std::unordered_map<const Any*, std::map<std::string, T>> s_items;

    //! \brief
    std::vector<std::function<void(Any&)>> m_clear_functions;

    //! \brief
    std::vector<std::function<void(Any const&, Any&)>> m_copy_functions;

    //! \brief
    std::vector<std::function<size_t(Any const&)>> m_size_functions;
};

// storage for our static members
template<class T>
std::unordered_map<const Any*, std::map<std::string, T>> Any::s_items;

#endif
