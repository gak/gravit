#include "graviton.h"

GLuint texID;

int loadTexture() {

	SDL_Surface		*tmp = NULL;
	SDL_Surface		*tmp2 = NULL;

	tmp = SDL_LoadBMP("texture.bmp");

	if (!tmp) {
		conAdd(1, "Failed loading texture");
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

	#define CE if ( glGetError() ) return 0;

	glGenTextures(1, &texID);
	CE

	glBindTexture(GL_TEXTURE_2D, texID);
	CE

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp2->pixels);
	CE

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	CE

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	CE

	SDL_FreeSurface(tmp);
	SDL_FreeSurface(tmp2);

	return 1;

}

