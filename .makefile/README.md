This directory contains Makefile utility files.

The following files contain common rules used by sub-project Makefiles for avoiding code duplication.
* Makefile.header: header part for Makefile.
* Makefile.footer: footer part for Makefile.

Auto-generate help rules for Makefile:
* Makefile.help: Allow Makefile to auto parse and display its own rules.

Bash scripts called by Mekfile:
* targz.sh: for creating backup tarball of the code source project.
* version.sh: for creating version.h file needed when compiling the project.
