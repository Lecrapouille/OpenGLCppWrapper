#ifndef DEBUG_HPP
#  define DEBUG_HPP

#  include <stdio.h>
#  include <stdarg.h>
#  include <stdlib.h>

#  define SHORT_FILENAME file_name(__FILE__).c_str()

inline void errout(const char* type, const char* file, const int line, const char* format, ...)
{
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s:%s:%d ", type, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "\n");

  va_end(args);
}

#  define ERROUT(type, format, ...) errout(type, SHORT_FILENAME, __LINE__, format, ## __VA_ARGS__);

#  ifdef DEBUG
#    undef DEBUG
#    define DEBUG(format, ...) ERROUT("DEBUG", format, ## __VA_ARGS__);
#  else
#    define DEBUG(format, ...)
#  endif

#  ifdef ERROR
#    undef ERROR
#    define ERROR(format, ...) ERROUT("ERROR", format, ## __VA_ARGS__);
#  else
#    define ERROR(format, ...)
#  endif

#endif /* DEBUG_HPP */
