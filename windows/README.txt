The files in this directory should help you build gravit on windows,
using Visual Studio or gcc (MinGW).

A set of pre-build libraries is contained in windows_libs.zip.

If gravit.exe complains that msvcrt90.dll is missing, you need to install the 
visual C runtime distributable (vcredist_x86.exe or vcredist_x64.exe).
You can download them from microsoft:
vcredist_x86 : http://www.microsoft.com/download/en/details.aspx?id=5582
vcredist_x64 : http://www.microsoft.com/download/en/details.aspx?id=2092


gcc (MinGW gcc 4.5.x or newer recommended)
========================
gcc delivers the best performance, and should be used if possible.
If you want the 64bit executable, you need to compile with gcc.

To build gravit, open a shell, enter this direcctory and 
type "make 32bit" or "make 64bit". 
Afterwards, you will find gravit.exe in the main source directory.

Copy all *.dll from lib32 and mingw_lib32 next to gravit.exe 
(the 64bit version needs the dll files from lib64 and mingw_lib64).
If gravit still does not start, you also need to copy 
libgomp-1.dll and libpthread-2.dll from your mingw gcc installation path.


Microsoft Visual Studio
========================
If you have only little experience with the GNU compiler toolchain, you could try
building gravit in Visual Studio. The code is a bit slower, but you can use a neat IDE.
Debugging is easier with Visual Studio. 

There are free editions availeable from Microsoft (i.e. "Visual C++ 2008 Express Edition with SP1") http://www.microsoft.com/visualstudio/en-us/products/2008-editions/express
(You may also need to dowenload and install the the Windows SDK and DirectX SDK)


project files availeable:

* gravit.dsw (and gravit.dsp) for Visual Studio 6. 
             Avoid this old compiler if you can.

* gravit.sln (and gravit.vcproj) for Visual Studio 2008 or newer. 
             Builds gravit.exe without multithreading (OpenMP).

* gravit_omp.sln (and gravit_omp.vcproj) for Visual Studio 2008 or newer. 
             Builds gravit_omp.exe with multithreading (OpenMP). 
             Usually, only the professional editions of Visual Studio 
             have the required OpenMP header and libraries. 


Your new gravit.exe is put in the main source directory.
Make sure all *.dll files from lib32 and vc9_lib32 are next to gravit.exe.



pre-compiled libraries:
========================

(unzip windows_libs.zip into this directory)

As getting the "right" library for your compiler can be a real pain in the ass,
I have collected some pre-compiled libraries and organized then into the following
direcctories:


include - header files that work for all compilers
lib32   - 32bit libraries and dll's that work for all compilers
lib64   - 64bit libraries and dll's that work for all compilers


mingw_include32 - header files for the 32bit MinGW gcc
mingw_include64 - header files for the 64bit MinGW gcc
mingw_lib32     - 32bit libraries and dll's for MinGW gcc
mingw_lib64     - 64bit libraries and dll's for MinGW gcc


vc9_include - header files for Microsoft Visual Studio (2008 or newer)
vc9_lib32   - 32bit libraries and dll's for Microsoft Visual Studio
vc9_lib64   - just a stub; I have no 64 bit for you yet ...


All libraries have been build from the original, plain sources
(see README-lua.txt, README-png.txt, README-SDL.txt, README-zlib.txt);
if you want to use a newer version, just drop the libs, dlls and include files
into this directory structure.
