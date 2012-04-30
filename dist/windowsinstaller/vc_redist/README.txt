LUA (and possibly other precompiled dll you used to build gravit) may require additional runtime libraries (MSVCR90.dll) from microsoft.

The download links for the Microsoft Visual C++ 2008 SP1 Redistributable Packages are:
32bit (x86) : http://www.microsoft.com/download/en/details.aspx?id=5582
64bit (x64) : http://www.microsoft.com/download/en/details.aspx?id=2092

After downloading vcredist_x86.exe (or vcredist_x64.exe for 64bit) from microsoft, put them into this directory so the setup compiler can pick them up.

