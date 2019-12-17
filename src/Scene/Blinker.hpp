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

#  include "Scene/Switch.hpp"
#  include "Common/TimedAction.hpp"

namespace glwrap
{
  DECLARE_CLASS(BlinkerNode3D);

  // ***************************************************************************
  //! \brief A special scene node cycling its children at a given frequency.
  //!
  //! Each child are enabled one after another during a given duration (same
  //! duration for each child).
  //!
  //! \note this node has been inspired by OpenInventor soBlinker
  //! https://grey.colorado.edu/coin3d/classSoBlinker.html
  //! \fixme: Can we define a generic Blinker with template function and then
  //! define a Blinker = IBlinker<ShitftChild>
  // ***************************************************************************
  //template<typename Functor> FIXME to be defined
  class BlinkerNode3D
    : public SwitchNode3D
  {
  public:

    //--------------------------------------------------------------------------
    //! \brief Static method allowing to create a new blinker node.
    //! \param name the name of the node. It is used mainly for the debug.
    //! \param the duration (in ms) of each child activity (before switch shift
    //! to the next child).
    //! \return the shared pointer of the newly created blinker node.
    //--------------------------------------------------------------------------
    static BlinkerNode3D_SP create(std::string const& name,
                                   size_t const duration/*, Functor functor*/)
    {
      return glwrap::make_shared<BlinkerNode3D>(name, duration/*, functor*/);
    }

    //--------------------------------------------------------------------------
    //! \brief Switch to the next child and start back to the 1st child if the
    //! last child has done its action.
    //--------------------------------------------------------------------------
    inline void nextChild()
    {
      select((selected() + 1_z) % children().size());
    }

  protected:

    //--------------------------------------------------------------------------
    //! \brief Create a blinker node with a given name and a given activity
    //! duration.
    //!
    //! \param name the name of the node. It is used mainly for the debug.
    //! \param the duration (in ms) of each child activity (before switch shift
    //! to the next child).
    //--------------------------------------------------------------------------
    BlinkerNode3D(std::string const& name, size_t const duration/*, Functor functor*/)
      : SwitchNode3D(name),
        m_timedAction(duration)//,m_functor(functor)
    {
      m_timedAction.start();
    }

  private:

    //--------------------------------------------------------------------------
    //! \brief When the scene is traversed we update the elapsed time and when
    //! the duration of the switch expired we select the next child.
    //! \param dt: delta time from the previous update.
    //--------------------------------------------------------------------------
    virtual void doUpdate(float const /*dt*/) override
    {
      // m_timedAction.update(m_functor);
      m_timedAction.update([this]() { nextChild(); });
    }

  protected:

    //! \brief Class holding the logic for calling the update callback at the
    //! expiration of the activity duration.
    TimedAction m_timedAction;
    //Functor m_functor;
  };

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_SCENE_BLINKER_HPP
