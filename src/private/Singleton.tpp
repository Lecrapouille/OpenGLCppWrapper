// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef SINGLETON_TPP_
#  define SINGLETON_TPP_

// *************************************************************************************************
//! \brief Long-life singleton (use reference for returning the
//! instance).
//!
//! Cannot be destroyed so it will follow the program life. The
//! instance is created with the creation of the program so be careful
//! fore example this class cannot be used for storing gtkmm widgets
//! or OpenGL objects because the singleton will be created before the
//! creation of their context. In this case prefer using the lazy
//! singleton.
//!
//! \note This class only works in compilers that fully comply with
//! this part of the C++11 standard.
//!
//! \note: Beware when using several singletons depending each others:
//! the order of destructor calls is undefined. Therefore this can
//! destroyed a singleton that another needs. Example: Logger is
//! singleton class letting other class to log; Singleton1 logs its
//! information during its destruction. If Logger is destroyed before
//! Singleton1, a crash will occur when trying to write in the log.
// *************************************************************************************************
template <class T>
class LongLifeSingleton
{
public:

  static T& instance()
  {
    static T instance;
    return instance;
  }

protected:

  LongLifeSingleton() { }
  ~LongLifeSingleton() { }

private:

  //! \brief Forbid usage of constructor by copy.
  LongLifeSingleton(LongLifeSingleton const&) = delete;
  //! \brief Forbid usage of constructor by moving.
  LongLifeSingleton(LongLifeSingleton&&) = delete;
  //! \brief Forbid usage of the copy assignement.
  LongLifeSingleton& operator=(LongLifeSingleton const&) = delete;
  //! \brief Forbid usage of the move assignement.
  LongLifeSingleton& operator=(LongLifeSingleton &&) = delete;
};

// *************************************************************************************************
//! \brief
// *************************************************************************************************
template <class T>
class LazySingleton;

// *************************************************************************************************
//! \brief Class for destroying a lazy singleton automaticly.
//! https://sourcemaking.com/design_patterns/to_kill_a_singleton
// *************************************************************************************************
template <class T>
class SingletonDestroyer
{
public:

  SingletonDestroyer()
    : m_singleton(nullptr)
  {
  }

  ~SingletonDestroyer()
  {
    if (nullptr != m_singleton)
      {
        m_singleton->destroy();
      }
  }

  void set(LazySingleton<T> &s)
  {
    m_singleton = &s;
  }

private:

  LazySingleton<T> *m_singleton;
};

// *************************************************************************************************
//! \brief The regular singleton (use pointer for returning the instance).
//!
//! You have to manage by yourself its life (when to instanciated and
//! when to to released it). This class is not thread safe when it's
//! instancing.
//!
//! \note: Beware when using several singletons depending each others:
//! the order of destructor calls is undefined. Therefore this can
//! destroyed a singleton that another needs. Example: Logger is
//! singleton class letting other class to log; Singleton1 logs its
//! information during its destruction. If Logger is destroyed before
//! Singleton1, a crash will occur when trying to write in the log.
// *************************************************************************************************
template <class T> class Singleton
{
public:

  static T& instance()
  {
    if (nullptr == s_instance)
      {
        s_instance = new T;
      }
    return *s_instance;
  }

  static void destroy()
  {
    if (nullptr != s_instance)
      {
        delete s_instance;
        s_instance = nullptr;
      }
  }

protected:

  Singleton() { }
  ~Singleton() { }

private:

  static T* s_instance;

  Singleton(Singleton&);
  void operator=(Singleton);
};

template <class T> T* Singleton<T>::s_instance = nullptr;

// *************************************************************************************************
//! \brief Lazy singleton (use pointer for returning the instance).
//!
//! This class is observed by the class SingletonDestroyer for an
//! automatic release when the main program ends. It acts like than
//! LongLifeSingleton.
//!
//! \note: Beware when using several singletons depending each others:
//! the order of destructor calls is undefined. Therefore this can
//! destroyed a singleton that another needs. Example: Logger is
//! singleton class letting other class to log; Singleton1 logs its
//! information during its destruction. If Logger is destroyed before
//! Singleton1, a crash will occur when trying to write in the log.
// *************************************************************************************************
template <class T>
class LazySingleton
{
private:

  friend class SingletonDestroyer<T>;

public:

  static T& instance()
  {
    //! See http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
    if (nullptr == s_instance)
      {
        //FIXME add lock for thread safe
        // if (nullptr == s_instance)
          {
            T* volatile temp = new T;
            s_instance = temp;
            s_destroyer.set(*s_instance);
          }
      }
    return *s_instance;
  }

  //\! brief Create the constructor and pass infinite number of args
  template<class Fn, class ... Args>
  static T& instance(Fn&& fn, Args&&... args)
  {
    //! See http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
    if (nullptr == s_instance)
      {
        //FIXME add lock for thread safe
        // if (nullptr == s_instance)
          {
            T* volatile temp = new T(fn, args...);
            s_instance = temp;
            s_destroyer.set(*s_instance);
          }
      }
    return *s_instance;
  }

  static void destroy()
  {
    if (nullptr != s_instance)
      {
        delete s_instance;
        s_instance = nullptr;
      }
  }

protected:

  LazySingleton() { }
  ~LazySingleton() { }

private:

  //! \brief Forbid usage of constructor by copy.
  LazySingleton(LazySingleton const&) = delete;
  //! \brief Forbid usage of constructor by moving.
  LazySingleton(LazySingleton&&) = delete;
  //! \brief Forbid usage of the copy assignement.
  LazySingleton& operator=(LazySingleton const&) = delete;
  //! \brief Forbid usage of the move assignement.
  LazySingleton& operator=(LazySingleton &&) = delete;
  //! \brief The instance of the singleton (init to nullptr).
  static SingletonDestroyer<T> s_destroyer;
  static T* volatile s_instance;
};

template <class T>
T* volatile LazySingleton<T>::s_instance = nullptr;
template <class T>
SingletonDestroyer<T> LazySingleton<T>::s_destroyer;

#endif /* SINGLETON_TPP_ */
