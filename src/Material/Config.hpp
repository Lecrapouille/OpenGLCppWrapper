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
  //! \brief Configure allows to enable or disable portion of shader code for
  //! adding or removing behaviors in shaders.
  struct BasicMaterialConfig
  {
    //! \brief If true then allow to drop fragment colors if lower than a given
    //! threshold.
    bool useAlphaTest = false;
    //! \brief If true then taken into account gamma correction.
    bool useGammaInput = false;
    //! \brief If true then taken into account gamma correction.
    bool useGammaOutput = false;
    //! \brief If true then apply a texture to the object.
    bool useMap = false;
    //! \brief If true then apply a bump mapping texture to the object.
    bool useBumpMap = false;
    //! \brief If true then simulate the bright spot of a light that appears on
    //! shiny objects. Specular highlights are often more inclined to the color
    //! of the light than the color of the object.
    bool useSpecularMap = false;
    //! \brief If true then apply a color to the object.
    bool useColor = true;
    //! \brief If true then apply a linear fog. Disabled if useExpFog is set to true.
    bool useFog = false;
    //! \brief If true then apply exponential fog and disable useFog.
    bool useExpFog = false;
  };
} // namespace glwrap

#endif // OPENGLCPPWRAPPER_MATERIAL_
