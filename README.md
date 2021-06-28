# FSUtils
File system utilities for linux.

Installation can be done quickly with the MakeInstall script available at https://github.com/MainIntelligence/CMakeScripts
    Otherwise, you can build and use the make --install command to install manually.

The header files installed in /usr/local/include/FSU shows the headers that can be included into your program with
#include <FSU/filename.h>

You can then link to the FSU library installed in /usr/local/lib, by specifying the linker command -lFSU.
(Ex. g++ -o fsutest Test.cpp -lFSU)
