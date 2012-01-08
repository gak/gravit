#include "gravit.h"

GLuint texID;

int loadTexture() {

    SDL_Surface		*tmp = NULL;
    SDL_Surface		*tmp2 = NULL;

    tmp = SDL_LoadBMP("texture.bmp");

    if (!tmp) {
        conAdd(LNORM, "Failed loading texture");
        return 0;
    }

    tmp2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 16, 16, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    if (!tmp2) {
        SDL_FreeSurface(tmp);
        return 0;
    }

    if (SDL_BlitSurface(tmp, NULL, tmp2, NULL)) {
        SDL_FreeSurface(tmp);
        SDL_FreeSurface(tmp2);
    }

    glGenTextures(1, &texID);
    glCheck();

    glBindTexture(GL_TEXTURE_2D, texID);
    glCheck();

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp2->pixels);
    glCheck();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glCheck();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glCheck();

    SDL_FreeSurface(tmp);
    SDL_FreeSurface(tmp2);

    return 1;

}

