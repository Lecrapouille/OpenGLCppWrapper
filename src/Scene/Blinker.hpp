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

#ifndef OPENGLCPPWRAPPER_SCENE_BLINKER_HPP
#  define OPENGLCPPWRAPPER_SCENE_BLINKER_HPP

#  include "Scene/Node.hpp"
#  include "Common/TimedAction.hpp"

namespace glwrap
{
  DECLARE_CLASS(BlinkerNode3D)

  // ***************************************************************************
  //! \brief A special switch node cycling its children.
  //!
  //! Each child are selected one after another after a given duration (same
  //! duration for each child).

  //! \note this node has been inspired by OpenInventor soBlinker
  //! https://grey.colorado.edu/coin3d/classSoBlinker.html
  //! \fixme: Can we define a aeneric Blinker with template function and then
  //! define a Blinker = IBlinker<ShitftChild>
  // ***************************************************************************
  //template<typename Functor> FIXME to be defined
  class BlinkerNode3D
    : public SwitchNode3D
  {
  //FIXME protected:
  public:

    //--------------------------------------------------------------------------
    //! \brief Create a switch node with a given name.
    //!
    //! \param name the name of the node. It is used mainly for the debug.
    //! \param the duration of each child activity (before switch shift to the
    //! next child).
    //--------------------------------------------------------------------------
    BlinkerNode3D(std::string const& name, size_t const duration/*, Functor functor*/)
      : SwitchNode3D(name),
        m_timedAction(duration)//,m_functor(functor)
    {
      m_timedAction.start();
    }

    //--------------------------------------------------------------------------
    //! \brief Static method allowing to create a switch node.
    //--------------------------------------------------------------------------
    static BlinkerNode3D_SP create(std::string const& name,
                                   size_t const duration/*, Functor functor*/)
    {
      return std::make_shared<BlinkerNode3D>(name, duration/*, functor*/);
    }

  private:

    //--------------------------------------------------------------------------
    //! \brief When the scene is traversed we update the elapsed time and when
    //! the duration of the switch expired we select the next child.
    //--------------------------------------------------------------------------
    virtual void doUpdate(float const /*dt*/) override
    {
      // m_timedAction.update(m_functor);
      m_timedAction.update([this]() { nextChild(); });
    }

  public:

    //--------------------------------------------------------------------------
    //! \brief Switch to the next child and start back to the 1st child when the
    //! last child has done it's action.
    //--------------------------------------------------------------------------
    inline void nextChild()
    {
      select((selected() + 1_z) % children().size());
    }

  protected:

    //! \brief Logic of the timer and of calling the callback.
    TimedAction m_timedAction;
    //Functor m_functor;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_SCENE_BLINKER_HPP
