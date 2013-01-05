/*

Gravit - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Gravit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Gravit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gravit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "gravit.h"

#ifdef WIN32
    #include <string.h>
    #include <stdio.h>
    #include <errno.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #ifdef __GNUC__
        #include <unistd.h>
        #define mkdir(path, mode) mymkdir(path)
    #else
        #define mkdir(path, mode) mkdir(path)
    #endif
#endif

#ifdef __MACH__
    #include <stdio.h>
    #include <stdint.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif


#define shitzta 4

char * va( char *format, ... ) {

    va_list argptr;
    static char string[shitzta][10000];    // in case va is called by nested functions
    static unsigned short index = 0;
    char *buf;

    buf = string[index];
    index++;

    if (index == shitzta)
        index = 0;

    va_start (argptr, format);
    vsprintf (buf, format, argptr);
    va_end (argptr);

    return buf;
}

int gfxPowerOfTwo(int input) {

    int value = 1;

    while ( value < input )
        value <<= 1;

    return value;

}

//tool.c(96) : warning C4267: '=' : conversion from 'size_t' to 'unsigned int', possible loss of data
// make p, pos, size, amountToRead become size_t
size_t LoadMemoryDump(char *fileName, unsigned char *d, size_t size, size_t chunk) {

    FILE *fp;
    size_t i, pos, p, amountToRead;
    size_t bytes;
    size_t chunkMax = FILE_CHUNK_SIZE;

    fp = fopen(fileName, "rb");
    if (!fp) {

        conAdd(LNORM, "Count not open %s for reading.", fileName);
        return 0;

    }

    if (chunk > 0) chunkMax = chunk;
    i = 0;
    pos = 0;
    bytes = 0;

    while ((pos < size) && !feof(fp)) {

        if (size - pos < chunkMax)
            amountToRead = size - pos;
        else
            amountToRead = chunkMax;

        p = fread(d, 1, amountToRead, fp);

        if (p < amountToRead) {
	    if (bytes == 0) {
                conAdd(LERR, "Short read on %s (%ld bytes)", fileName, (long)(p+bytes));
                fclose(fp);
                return 0;
            } else {
                conAdd(LLOW, "Short read on %s (%ld bytes)", fileName, (long)(p+bytes));
            }
        }


        d += p;
        pos += p;
        bytes += p;

        if (ferror(fp)) {

            conAdd(LNORM, "%s", strerror( errno ));
            fclose(fp);
            return 0;

        }
#if 0
        if (i) {
            conAdd(LLOW, "%3.1f%% (%.0f / %.0f)", 100*(float)pos/(float)size,(float)pos / 1024 / 1024,(float)size / 1024 / 1024);
#ifndef NO_GUI
            drawAll();
#endif
            i=1000;
        }

        i--;
#endif

    }

    fclose(fp);

    return bytes;
}

//tool.c(96) : warning C4267: '=' : conversion from 'size_t' to 'unsigned int', possible loss of data
// make p, pos, size, amountToRead become size_t
int SaveMemoryDump(char *fileName, unsigned char *d, size_t total) {

    FILE *fp;
    size_t written, p, write;

    fp = fopen(fileName, "wb");
    if (!fp) {

        conAdd(LNORM, "count not open %s for writing", fileName);
        return 0;

    }

    written = 0;

    while (written < total) {

        write = FILE_CHUNK_SIZE;

        if (written + write > total)
            write = total - written;

        p = fwrite(d, 1, write, fp);
        d += p;
        written += p;

    }

    fclose(fp);

    if (((unsigned long)(written >>20)) > 5)
        conAdd(LHELP, "written %lu MB to %s", (unsigned long)(written >>20), fileName);
    else
        conAdd(LLOW, "written %lu bytes to %s", (unsigned long)written, fileName);

    return 1;

}

Uint32 getMS() {

#ifndef NO_GUI
    return SDL_GetTicks();
#else

#ifdef WIN32
    return GetTickCount();
#else

    struct timeb mytime;
    ftime(&mytime);
    return mytime.time * 1000 + mytime.millitm;

#endif

#endif

}

void setTitle(char *state) {

#ifndef NO_GUI

    char *a;

    if (state)
        a = va("%s - %s", GRAVIT_VERSION, state);
    else
        a = GRAVIT_VERSION;

    SDL_WM_SetCaption(a, a);

#endif

}

#ifndef _MSC_VER
// Borrowed from http://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux

typedef struct stat Stat;

static int do_mkdir(const char *path, mode_t mode)
{
    Stat            st;
    int             status = 0;
    
    if (stat(path, &st) != 0)
    {
        /* Directory does not exist */
        if (mkdir(path, mode) != 0)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }
    
    return(status);
}
#else
//damnwindows 
#define do_mkdir(path, mode) mymkdir(path)
typedef int mode_t;
#endif

int mkpath(const char *path, mode_t mode)
{
    char           *pp;
    char           *sp;
    int             status;
    char           *copypath = strdup(path);
    
    status = 0;
    pp = copypath;
    while (status == 0 && (sp = strchr(pp, '/')) != 0)
    {
        if (sp != pp)
        {
            /* Neither root nor double slash in path */
            *sp = '\0';
            status = do_mkdir(copypath, mode);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == 0)
        status = do_mkdir(path, mode);
    free(copypath);
    return (status);
}

int mymkdir(const char *path) {

#ifdef WIN32
    {
        WIN32_FIND_DATA damnwindows;
        if (FindFirstFile(path, &damnwindows) == INVALID_HANDLE_VALUE) {
            CreateDirectory(path, NULL);
        }
    }
#else
    {

        mkpath(path, 0755);

    }
#endif

    return 1;

}

void freeFileName() {

    if (state.fileName) {
        free(state.fileName);
        state.fileName = 0;
    }

}

void setFileName(char *name) {

    char buf[256];

    strncpy(buf, name, 255);

    freeFileName();

    state.fileName = malloc(strlen(buf)+1);	// +1 for \0
    strcpy(state.fileName, buf);

}

char *getRegistryString(char *variable) {
#ifdef WIN32

    static char buf[MAX_PATH+1];
    DWORD len = MAX_PATH;

    HKEY hkResult;
    RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL);
    RegQueryValueEx(hkResult, variable, 0, NULL, (unsigned char *)buf, &len);
    RegCloseKey(hkResult);

    // try HKLM if thy key was not found in HKCU
    if (!buf || strlen(buf) == 0) {
        LONG returnValue = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_KEY, 0, KEY_READ , &hkResult);
        if (returnValue == ERROR_SUCCESS) {
	    RegQueryValueEx(hkResult, variable, 0, NULL, (unsigned char *)buf, &len);
        }
        RegCloseKey(hkResult);
    }

    return buf;

#endif
    return NULL;
}

void setRegistryString(char *variable, char *value) {
#ifdef WIN32

    HKEY hkResult;
    RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL);
    RegSetValueEx(hkResult, variable, 0, REG_SZ, (unsigned char *)value, (DWORD)(strlen(value)+1));
    RegCloseKey(hkResult);

#endif
}

// return 0 on failure
int myunlink(char *filename) {

#ifdef WIN32
    return DeleteFile(filename);
#else
    return !unlink(filename);
#endif

}

int fileExists(char *path) {

    FILE *fp;
    fp = fopen(path, "rb");
    if (!fp) return 0;
    fclose(fp);
    return 1;

}

// finds a file in gravit's search paths
char *findFile(char *file) {

#ifndef WIN32
    char *homeDir;
#endif
    char *tmp;

    // first assume no path, so someone could load "/tmp/script" or "currentdirectory.cfg"
    if (fileExists(file))
        return file;

#ifdef WIN32
    // windows users only have one place to look, that is the current directory
    return 0;
#endif

#if __MACH__
    
    NSBundle *mainBundle = [NSBundle mainBundle];
    tmp = va("%s/../%s", [[mainBundle pathForResource:@"" ofType:@""] UTF8String], file);
    if (fileExists(tmp))
        return tmp;

#endif
    
#ifndef WIN32
    // now look in the users gravit directory (only in UNIX) - /home/user/.gravit/file
    homeDir = getenv("HOME");
    if (homeDir) {
        tmp = va("%s/.gravit/%s", homeDir, file);
        if (fileExists(tmp)) {
            return tmp;
        }
    }
#endif

    // finally look in SYSCONFDIR
    tmp = va("%s/%s", SYSCONFDIR, file);
    if (fileExists(tmp)) {
        return tmp;
    }

    conAdd(LLOW, "Not Found: %s", file);
    
    return 0;

}

// check to see if we can save into $HOME/.gravit/save/ directory or not
int checkHomePath() {

#ifndef WIN32
    char *user;
#endif

#ifdef WIN32
    return 1;
#else
    user = getenv("HOME");
    if (!user) {
        conAdd(LERR, "$HOME environment variable not set, can not continue.");
        return 0;
    }
    mymkdir(va("%s/.gravit", user));
    return 1;
#endif

}

size_t getMemory() {

    // From http://stackoverflow.com/questions/2513505/how-to-get-available-memory-c-g
    size_t realMemory;
    
#ifdef WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#else
#ifdef __MACH__
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    u_int namelen = sizeof(mib) / sizeof(mib[0]);
    uint64_t size;
    size_t len = sizeof(size);
    sysctl(mib, namelen, &size, &len, NULL, 0);
    realMemory = size;
    return realMemory;
    
#else
    return sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE);
    
#endif
#endif
    
}

size_t getMemoryAvailable() {
    size_t realMemory;
    
    if (state.memoryAvailable > 0)
        return state.memoryAvailable;

    // This a bit awkward. Let's just pretend there is a value here.
    if (state.memoryPercentage == 0) {
        conAdd(LERR, "memoryAvailable and memoryPercentage are both 0. I'm just going to set memoryPercentage for you.");
        state.memoryPercentage = 50;
    }
    
    realMemory = getMemory();
    // We've detected 0 memory here, bad news. We have to assume something
    if (realMemory == 0) {
        realMemory = 128;
    }
    return state.memoryPercentage / 100. * realMemory / 1024 / 1024;
}
