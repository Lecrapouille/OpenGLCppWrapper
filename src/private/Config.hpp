//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019-2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef CONFIG_HPP_
#  define CONFIG_HPP_

#  include "version.h"

namespace config
{
  //! \brief
  enum Mode { Debug, Release };

  //! \brief
  static const Mode mode = config::Debug;
  //! \brief Used for logs and GUI.
  static const std::string project_name("OpenGLCppWrapper");
  //! \brief Major version of project
  static const uint32_t major_version(PROJECT_MAJOR_VERSION);
  //! \brief Minor version of project
  static const uint32_t minor_version(PROJECT_MINOR_VERSION);
  //! \brief Save the git SHA1
  static const std::string git_sha1(PROJECT_SHA1);
  //! \brief Save the git branch
  static const std::string git_branch(PROJECT_BRANCH);
  //! \brief Pathes where default project resources have been installed
  //! (when called  by the shell command: sudo make install).
  static const std::string data_path(PROJECT_DATA_PATH);
}

#endif /* CONFIG_HPP_ */
