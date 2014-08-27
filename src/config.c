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
Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/

#include "gravit.h"

int configRead(char *filename, int ignoreMissing) {

    FILE *fp;
    char buffer[FILE_CHUNK_SIZE_SMALL];

    if (!filename || filename[0] == 0) {
        if (ignoreMissing) return 1;
        conAdd(LERR, "Can't open NULL script");
        return 0;
    }

    fp = fopen(filename, "rb");
    if (!fp) {
        if (ignoreMissing) return 1;
        conAdd(LERR, "Could not open script: %s", filename);
        return 0;
    }

    conAdd(LLOW, "Executing Script: %s", filename);

    while (fgets(buffer, FILE_CHUNK_SIZE_SMALL, fp)) {
        int len = (int)strlen(buffer) - 1;

        while (len >= 0 && (isspace(buffer[len]) || buffer[len] == 13 || buffer[len] == 10)) buffer[len--] = 0;
        if (len <= 0 || buffer[0] == '#') continue;
        cmdExecute(buffer);
    }

    return 1;

}
