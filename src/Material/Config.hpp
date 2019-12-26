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

#ifndef OPENGLCPPWRAPPER_MATERIAL_CONFIG_HPP
#  define OPENGLCPPWRAPPER_MATERIAL_CONFIG_HPP

namespace glwrap
{
  struct BasicMaterialConfig
  {
    //! \brief
    bool useAlphaTest = false;
    //! \brief
    bool useGammaInput = false;
    //! \brief
    bool useGammaOutput = false;
    //! \brief
    bool useMap = false;
    //! \brief
    bool useBumpMap = false;
    //! \brief
    bool useSpecularMap = false;
    //! \brief
    bool useColor = true;
    //! \brief
    bool useFog = false;
    //! \brief
    bool useExpFog = false;
  };
} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIAL_
