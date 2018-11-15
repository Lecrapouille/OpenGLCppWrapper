//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef ILOGGER_HPP_
#  define ILOGGER_HPP_

#  include <mutex>
#  include <stdarg.h>
#  include <fstream>
#  include <cassert>
#  include <sstream>
#  include <string>
#  include <iostream>

namespace logger
{
  enum LoggerSeverity { None, Info, Debug, Warning, Failed, Error, Fatal, MaxLoggerSeverity = Fatal };
}

// **************************************************************
//
// **************************************************************
class ILogger
{
public:

  virtual ~ILogger() {};
  void log(const char* format, ...);
  void log(std::ostream *stream, enum logger::LoggerSeverity severity, const char* format, ...);
  template <class T> ILogger& operator<<(const T& tolog);
  const char *strtime();

protected:

  void currentDate();
  void currentTime();

private:

  virtual void write(std::string const& message) = 0;
  virtual void write(const char *message, const int length = -1) = 0;
  virtual void beginLine() = 0;

protected:

  std::mutex m_mutex;
  constexpr static const uint32_t c_buffer_size = 1024;
  char m_buffer[c_buffer_size];
  enum logger::LoggerSeverity m_severity = logger::None;
  char m_buffer_time[32];
  std::ostream *m_stream = nullptr;
};

template <class T> ILogger& ILogger::operator <<(const T& to_log)
{
  std::ostringstream stream;
  stream << to_log;
  write(stream.str());

  return *this;
}

// **************************************************************
//
// **************************************************************
class FileLogger: public ILogger
{
private:

  virtual bool open(std::string const& filename) = 0;
  virtual void close() = 0;
  virtual void header() = 0;
  virtual void footer() = 0;
};

#endif /* ILOGGER_HPP_ */
