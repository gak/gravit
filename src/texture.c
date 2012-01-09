#include "gravit.h"

GLuint texID;

GLuint loadTexture(char *fileName) {

    GLuint textureId;
    SDL_Surface *surface;
    
    char *path = findFile(fileName);
    
    if (!path) {
        conAdd(LERR, "Could not find %s", fileName);
        return 0;
    }
    
    surface = IMG_Load(path);
    if (!surface) {
        conAdd(LERR, "Could not load %s", path);
        return 0;
    }
    
    glGenTextures(1, &textureId);
    glCheck();
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCheck();
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glCheck();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheck();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheck();
    
    SDL_FreeSurface(surface);

    return textureId;
}

