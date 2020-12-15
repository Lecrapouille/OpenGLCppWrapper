//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef BEHAVIOR_HPP
#  define BEHAVIOR_HPP

//------------------------------------------------------------------------------
//! \brief Comes from Unity's MonoBehavior class
//------------------------------------------------------------------------------
class Behavior
{
public:

    virtual void onSetup()
    {}

    //--------------------------------------------------------------------------
    //! \brief Callback triggered when visiting the scene for updating
    //! transformation matrices. This method does nothing by default but can be
    //! overrided by TreeNode3D's derived classes for animating/moving the current
    //! node (which will impact on position of descendant nodes).
    //!
    //! \note: the project https://github.com/vahlers/scg3 traverse the tree
    //! using the vistor design pattern. I did not follow because I am a big fan
    //! of visitors: in my personal opinion they make the code harder to
    //! understand. In the course https://research.ncl.ac.uk/game/mastersdegree/
    //! graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf
    //! they let derived classes override the method update() but suppose the
    //! developper will call the method update() of the parent class. I think this
    //! is an error prone pratice. I think doUpdate() does the same thing in a more
    //! simpler way for the developper.
    //!
    //! \param dt: delta time from the previous update.
    //--------------------------------------------------------------------------
    virtual void onUpdate(float const /*dt*/)
    {}
};

#endif
