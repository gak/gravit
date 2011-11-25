======================================
About
======================================

This directory contains a pre-compiled version of LUA 5.1.4 that can
be used both in VisualStudio (2008 SP1 or newer) and with MinGW gcc.

include: header files
lib32: 32bit libraries and runtime dlls
lib64: 64bit libraries and runtime dlls

The source code is from the "LuaBinaries" project (see below), but
compiled with VisualStudio 2008 (insted of VS 2005) for 32bit (x86)
and 64bit (x64) targets.

You may need to install the VS 2008 SP1 redistributables (free from microsoft).


   Frank.



======================================
README for LuaBinaries DLL packages
http://sourceforge.net/projects/luabinaries/files/5.1.4/Windows%20Libraries/
======================================

We choose the Visual C++ 8 compiler to build the LuaBinaries executables.

Up to Visual C++ version 6, the C run time library was called "msvcrt.dll" and this DLL is today included in all modern Windows systems (W2K, WinXP, Vista and 7). But for Visual C++ 8, Microsoft released a free version of the compiler called "Express". We consider that an important step for developers using LuaBinaries DLLs. Also It is know that "msvcrt.dll" has some problems and limitations, particularly with multi-threading. So we decided to move to Visual C++ 8 version for which the C run time library is called "msvcr80.dll".

So your module DLL or application EXE must link with the stub library made for Visual C++ 8 (Visual Studio 2005), that it is distributed in the LuaBinaries "dll8" package for 32 bits or in the "dll8_64" package for 64 bits.

The LuaBinaries DLL packages have a dll proxy called "lua51.dll". It can be used to replace other "lua51.dll" released by other distributions. It will simply forward calls to the "lua5.1.dll". There is no compiled source code involved in the forwarding.

Notice that MingW can generate DLLs with different MSVCR*.DLL dependencies. To generate an import library for MingW just run (you will need the LuaBinaries source code to obtain the "lua5.1.def" file):

dlltool -d lua5.1.def -D lua5.1.dll -l liblua5.1.a

The MSVCR80 Installation for Lua 5.1

One problem that rises when using the Visual C++ 8 C run time library is that it is not installed on the system. But you can redistribute it with your application. It can be in a folder called "Microsoft.VC80.CRT" on the same folder of the executable or it can be installed by the redistributable package provided at "Microsoft Visual C++ 2005 SP1 Redistributable Package (x86)" or "Microsoft Visual C++ 2005 Redistributable Package (x64)". To install the packages you will need administrative privileges. The Visual C++ 2005 used to compile LuaBinaries has the Service Pack 1 applied.

The LuaBinaries executables includes the folder "Microsoft.VC80.CRT" so you can use it in systems that do not have the redistributable package installed and it will not conflict with systems that have the package installed.

The Lua for Windows distribution already includes the redistributable package.



======================================
README for Lua 5.1
======================================

See INSTALL for installation instructions.
See HISTORY for a summary of changes since the last released version.

* What is Lua?
  ------------
  Lua is a powerful, light-weight programming language designed for extending
  applications. Lua is also frequently used as a general-purpose, stand-alone
  language. Lua is free software.

  For complete information, visit Lua's web site at http://www.lua.org/ .
  For an executive summary, see http://www.lua.org/about.html .

  Lua has been used in many different projects around the world.
  For a short list, see http://www.lua.org/uses.html .

* Availability
  ------------
  Lua is freely available for both academic and commercial purposes.
  See COPYRIGHT and http://www.lua.org/license.html for details.
  Lua can be downloaded at http://www.lua.org/download.html .

* Installation
  ------------
  Lua is implemented in pure ANSI C, and compiles unmodified in all known
  platforms that have an ANSI C compiler. In most Unix-like platforms, simply
  do "make" with a suitable target. See INSTALL for detailed instructions.

* Origin
  ------
  Lua is developed at Lua.org, a laboratory of the Department of Computer
  Science of PUC-Rio (the Pontifical Catholic University of Rio de Janeiro
  in Brazil).
  For more information about the authors, see http://www.lua.org/authors.html .

(end of README)

