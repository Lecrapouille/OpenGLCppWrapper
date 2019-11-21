#ifndef OPENGLCPPWRAPPER_NONCPPSTD_HPP
#  define OPENGLCPPWRAPPER_NONCPPSTD_HPP

#  include "Common/Verbose.hpp"
#  include <cstddef>
#  include <memory>

// **************************************************************
//! \brief Inspired by
//! http://www.boost.org/doc/libs/1_66_0/boost/core/noncopyable.hpp
// **************************************************************
class NonCopyable
{
protected:

  constexpr NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(const NonCopyable&) = delete;
  const NonCopyable& operator=(const NonCopyable&) = delete;
};

// **************************************************************
//! \brief Allows to create literal values of type std::size_t
//! In the same way than U, LL or UL macros. Indeed size_t can
//! be uint32_t or uint64_t depending on the architecture.
//! Example: size_t i = 42_z;
// **************************************************************
constexpr std::size_t operator "" _z (unsigned long long const n)
{
  return static_cast<std::size_t>(n);
}

// **************************************************************
// Enable for C++11 and Visual Studio
// **************************************************************
#  if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
namespace std
{
  //! \brief std::make_unique is for C++14 enable it for C++11
  template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}
#  endif

// **************************************************************
//! \brief Return the number of elements in an array.
//! S for size and T for the type.
// **************************************************************
template<size_t S, typename T>
inline size_t ARRAY_SIZE(T (&)[S])
{
  return S;
}

// **************************************************************
//! \brief Forward declaration of classes with shared and unique
//! pointers:
//! - Class,
//! - ClassSP: shared_pointer<Class>
//! - classUP: unique_pointer<Class>
// **************************************************************
#define DECLARE_CLASS(TypeName)                      \
    class TypeName;                                  \
    using TypeName##_SP = std::shared_ptr<TypeName>; \
    using TypeName##_UP = std::unique_ptr<TypeName>;

// **************************************************************
//! \brief One of the most used optimization used in Linux kernel. When
//! working with conditional code (if-else statements), we often know
//! which branch is true and which is not. If compiler knows this
//! information in advance, it can generate most optimized code.
// **************************************************************
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
