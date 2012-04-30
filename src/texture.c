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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glCheck();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

