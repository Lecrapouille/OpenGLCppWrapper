#ifndef OPENGLCPPWRAPPER_NONCPPSTD_HPP
#  define OPENGLCPPWRAPPER_NONCPPSTD_HPP

#  include "Common/Verbose.hpp"
#  include <cstddef>
#  include <memory>

// *****************************************************************************
//! \brief Make the class and its derived class non copyable by deleting methods
//! such as copy constructor, operator=().
//!
//! Inspired by http://www.boost.org/doc/libs/1_66_0/boost/core/noncopyable.hpp
// *****************************************************************************
class NonCopyable
{
protected:

  constexpr NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(const NonCopyable&) = delete;
  const NonCopyable& operator=(const NonCopyable&) = delete;
};

// *****************************************************************************
//! \brief Allows to create literal values of type std::size_t In the same way
//! than U, LL or UL macros.
//!
//! Indeed size_t can be uint32_t or uint64_t depending on the architecture.
//! \code
//! size_t i = 42_z;
//! \endcode
// *****************************************************************************
constexpr std::size_t operator "" _z (unsigned long long const n)
{
  return static_cast<std::size_t>(n);
}

// *****************************************************************************
// Enable for C++11 and Visual Studio
// *****************************************************************************
#if !((defined(_MSC_VER) && (_MSC_VER >= 1800)) ||                                    \
      (defined(__clang__) && defined(__APPLE__) && (COMPILER_VERSION >= 60000)) ||    \
      (defined(__clang__) && (!defined(__APPLE__)) && (COMPILER_VERSION >= 30400)) && (__cplusplus > 201103L) || \
      (defined(__GNUC__) && (COMPILER_VERSION >= 40900) && (__cplusplus > 201103L)))

// These compilers do not support make_unique so redefine it
namespace std
{
    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
        }

    //! \brief Implement the C++14 std::make_unique for C++11
    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;
}
#  endif

// *****************************************************************************
//! \brief Return the number of elements in an array.
//! \tparam S for the size of the array.
//! \tparam T for the type of data.
// *****************************************************************************
template<size_t S, typename T>
inline size_t ARRAY_SIZE(T (&)[S])
{
  return S;
}

// *****************************************************************************
//! \brief Forward declaration of classes with shared and unique
//! pointers:
//! - Class
//! - ClassSP: shared_pointer<Class>
//! - classUP: unique_pointer<Class>
// *****************************************************************************
#define DECLARE_CLASS(TypeName)                      \
    class TypeName;                                  \
    using TypeName##_SP = std::shared_ptr<TypeName>; \
    using TypeName##_UP = std::unique_ptr<TypeName>;

// *****************************************************************************
//! \brief One of the most used optimization used in Linux kernel. When
//! working with conditional code (if-else statements), we often know
//! which branch is true and which is not. If compiler knows this
//! information in advance, it can generate most optimized code.
// *****************************************************************************
#  ifndef likely
#    define likely(x)       __builtin_expect(!!(x),1)
#  endif
#  ifndef unlikely
#    define unlikely(x)     __builtin_expect(!!(x),0)
# endif

#  ifndef NORETURN
#    if __GNUC__ > 2 || defined(__INTEL_COMPILER)
#      define NORETURN __attribute__((__noreturn__))
#    else
#      define NORETURN
#    endif
#  endif

#endif // OPENGLCPPWRAPPER_NONCPPSTD_HPP
