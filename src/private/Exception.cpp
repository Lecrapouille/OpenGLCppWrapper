// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:     BSL-1.0
//
// NOTE: This file has been adapted for the OpenGLCppWrapper project.
// For more informations, see official Poco webpage: https://pocoproject.org/index.html


#include "Exception.hpp"
#include "Logger.hpp"

Exception::Exception(int code)
  : m_pNested(0), m_code(code)
{
  // FIXME: passer en param par default __LINE__ et __FILE__ pour logger d'ou vient l'exception
  LOGE("Exception %u", code);
}


Exception::Exception(const std::string& msg, int code)
  : m_msg(msg), m_pNested(0), m_code(code)
{
  LOGE("Exception %u '%s'", code, msg.c_str());
}


Exception::Exception(const std::string& msg, const std::string& arg, int code)
  : m_msg(msg), m_pNested(0), m_code(code)
{
  if (!arg.empty())
    {
      m_msg.append(": ");
      m_msg.append(arg);
    }
  LOGE("Exception %u '%s'", code, m_msg.c_str());
}


Exception::Exception(const std::string& msg, const Exception& nested, int code)
  : m_msg(msg), m_pNested(nested.clone()), m_code(code)
{
}


Exception::Exception(const Exception& exc)
  : std::exception(exc),
    m_msg(exc.m_msg),
    m_code(exc.m_code)
{
  m_pNested = exc.m_pNested ? exc.m_pNested->clone() : 0;
}


Exception::~Exception() throw()
{
  delete m_pNested;
}


Exception& Exception::operator = (const Exception& exc)
{
  if (&exc != this)
    {
      Exception* newPNested = exc.m_pNested ? exc.m_pNested->clone() : 0;
      delete m_pNested;
      m_msg     = exc.m_msg;
      m_pNested = newPNested;
      m_code    = exc.m_code;
    }
  return *this;
}


const char* Exception::name() const throw()
{
  return "Exception";
}


const char* Exception::className() const throw()
{
  return typeid(*this).name();
}


const char* Exception::what() const throw()
{
  return name();
}


std::string Exception::displayText() const
{
  std::string txt = name();
  if (!m_msg.empty())
    {
      txt.append(": ");
      txt.append(m_msg);
    }
  return txt;
}


void Exception::extendedMessage(const std::string& arg)
{
  if (!arg.empty())
    {
      if (!m_msg.empty()) m_msg.append(": ");
      m_msg.append(arg);
    }
}


Exception* Exception::clone() const
{
  return new Exception(*this);
}


void Exception::rethrow() const
{
  throw *this;
}
