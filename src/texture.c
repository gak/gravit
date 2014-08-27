/*

Gravit - A gravity simulator
Copyright 2003-2012 Gerald Kaszuba

Gravit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Gravit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gravit; if not, write to the Free Software
Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/


#include "gravit.h"

GLuint texID;

GLuint loadTexture(char *fileName, int isSkybox) {

    GLuint textureId;
    GLenum colortype;

    SDL_Surface *surface;
    
    char *path = findFile(fileName);
    
    if (!path) {
        conAdd(LERR, "Could not find %s", fileName);
        return 0;
    }
    
    surface = IMG_Load(path);
    if (!surface) {
        sdlCheck();
        conAdd(LERR, "Could not load %s", path);
        return 0;
    }
    
    // get Type : RGB or RGBA
    if (surface->format->BytesPerPixel == 4)
        colortype = GL_RGBA;
    else if (surface->format->BytesPerPixel == 3)
        colortype = GL_RGB;
    else {
        conAdd(LERR, "Unknown BBP: %i for %s", surface->format->BytesPerPixel, path);
        return 0;
    }

    glGenTextures(1, &textureId);
    glCheck();
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCheck();

    if (isSkybox) {

        // use GL_CLAMP_TO_EDGE if supported
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        if(glGetError()) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        } else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glCheck();
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheck();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheck();
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, colortype, surface->w, surface->h, colortype, GL_UNSIGNED_BYTE, surface->pixels);
    glCheck();

    SDL_FreeSurface(surface);

    return textureId;
}

