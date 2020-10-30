//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_TIMED_ACTION_HPP
#  define OPENGLCPPWRAPPER_TIMED_ACTION_HPP

#  include "Math/Maths.hpp"
#  include <chrono>

constexpr std::chrono::milliseconds operator ""_ms(unsigned long long ms)
{
  return std::chrono::milliseconds(ms);
}

namespace glwrap
{
  using namespace std::chrono;

  // \brief Base class for all animations (abstract), providing general functionality.
  class TimedAction
  {
    using Clock = steady_clock;
    using Time = Clock::time_point;

  public:

    TimedAction(milliseconds const duration)
      : m_duration(duration)
    {}

    TimedAction()
      : m_duration(0_ms)
    {}

    virtual ~TimedAction()
    {}

    //! \brief Check if animation has been started at least once.
    inline bool isStarted() const
    {
      return m_started;
    }

    //! \brief Check if animation is running.
    inline bool isRunning() const
    {
      return m_running;
    }

    //! \brief Start or restart animation with given time.
    //! \param duration milliseconds
    void start(milliseconds const duration)
    {
      if (m_started)
        return ;

      m_started = true;

      m_duration = std::min(1_ms, duration);
    }

    //! \brief if and only if duration has been set
    bool start()
    {
      if (0_ms == m_duration)
        {
          ERROR("Duration has not been initialized");
          return false;
        }
      m_started = true;
      return true;
    }

    //! \brief Stop or pause animation.
    inline void stop()
    {
      m_started = false;
    }

    //! \brief Reset animation, i.e., set all variables to their initial states.
    inline void reset(milliseconds const duration)
    {
      m_started = false;
      m_running = false;
      m_duration = std::min(1_ms, duration);
    }

    //! \brief Update animation with given time.
    //! \param currTime current time (milliseconds)
    template<typename Functor>
    bool update(Functor functor)
    {
      if (unlikely(!m_started))
        return false;

      if (likely(m_running))
        {
          Time currentTime = Clock::now();
          milliseconds dt =
            duration_cast<milliseconds>(currentTime - m_startedTime);
          if (dt > m_duration)
            {
              m_startedTime = currentTime;
              functor();
              return true;
            }
          return false;
        }
      else
        {
          if (unlikely(0_ms == m_duration))
            {
              ERROR("duration has not been set");
              return false;
            }

          m_running = true;
          m_startedTime = Clock::now();
          functor();
          return true;
        }
    }

  protected:

    //! \brief true if animation has been started (may be stopped temporarily)
    //! \fixme TBD atomic bool for start()/stop() from different threads ???
    bool m_started = false;
    //! \brief true if animation is running
    bool m_running = false;
    //! \brief time at start (milliseconds)
    Time m_startedTime;
    //! \brief
    milliseconds m_duration;
  };
} // namespace glwrap

#endif // OPENGLCPPWRAPPER_TIMED_ACTION_HPP
