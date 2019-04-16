#ifndef DEBUG_HPP
#  define DEBUG_HPP

#  include <stdio.h>
#  include <stdarg.h>
#  include <stdlib.h>

#  define SHORT_FILENAME file_name(__FILE__).c_str()

__attribute__ ((format (printf, 4, 5)))
inline void errout(const char* type, const char* file, const int line, const char* format, ...)
{
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s:%s:%d ", type, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "\n");

  va_end(args);
}

#  ifdef DEBUG
#    undef DEBUG
#    define DEBUG_HELPER(format, ...) errout("DEBUG", SHORT_FILENAME, __LINE__, format "%s", __VA_ARGS__)
#    define DEBUG(...) DEBUG_HELPER(__VA_ARGS__, "")
#  else
#    define DEBUG(format, ...)
#  endif

#  ifdef ERROR
#    undef ERROR
#    define ERROR_HELPER(format, ...) errout("ERROR", SHORT_FILENAME, __LINE__, format "%s", __VA_ARGS__)
#    define ERROR(...) ERROR_HELPER(__VA_ARGS__, "")
#  else
#    define ERROR(format, ...)
#  endif

#endif /* DEBUG_HPP */
