#ifndef NONCPPSTD_HPP
#  define NONCPPSTD_HPP

#  include <cstddef>
#  include <memory>

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

#endif
