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
    if (surface->format->Amask)
        colortype=GL_RGBA;
    else
        colortype=GL_RGB;

    glGenTextures(1, &textureId);
    glCheck();
    
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCheck();
    
    if (isSkybox) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glCheck();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glCheck();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glCheck();
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheck();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheck();
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, surface->w, surface->h, colortype, GL_UNSIGNED_BYTE, surface->pixels);
    glCheck();

    SDL_FreeSurface(surface);

    return textureId;
}

