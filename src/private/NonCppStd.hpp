#ifndef OPENGLCPPWRAPPER_NONCPPSTD_HPP
#  define OPENGLCPPWRAPPER_NONCPPSTD_HPP

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
constexpr std::size_t operator "" _z (unsigned long long n) { return n; }

// **************************************************************
//! https://stackoverflow.com/questions/17902405/how-to-implement-make-unique-function-in-c11
// **************************************************************
namespace std
{
  template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

// **************************************************************
//! \brief Return the number of elements in an array
// **************************************************************
#  ifndef ARRAY_SIZE
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#  endif

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

namespace glwrap
{

//------------------------------------------------------------------
//! \brief give the file name with its extension from a given path
//------------------------------------------------------------------
inline static std::string file_name(std::string const& path)
{
  std::string::size_type pos = path.find_last_of("\\/");
  if (pos != std::string::npos)
    return path.substr(pos + 1, std::string::npos);
  return path;
}

} // namespace glwrap

#  include "Verbose.h"

#endif // OPENGLCPPWRAPPER_NONCPPSTD_HPP
