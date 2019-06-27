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

#ifndef OPENGLCPPWRAPPER_MEMORY_HPP
#  define OPENGLCPPWRAPPER_MEMORY_HPP

#  include "NonCppStd.hpp"
#  include <atomic>

namespace glwrap
{
  //----------------------------------------------------------------------------
  //! \brief Track the usage of the estimated GPU memory.
  //! \return The estimated GPU used by this program in kilo bytes.
  //----------------------------------------------------------------------------
  inline std::atomic<size_t>& GPUMemory()
  {
    static std::atomic<size_t> mem_gpu{0_z};
    return mem_gpu;
  }
} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MEMORY_HPP
