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

#ifndef NO_GUI

font_t fonts[256];
float fontHeight;

int loadFonts() {

    TTF_Font		*font = NULL;
    SDL_Surface		*fontSurface = NULL;
    SDL_Surface		*tmp = NULL;
    Uint32 saved_flags;
    Uint8  saved_alpha;
    static SDL_Color fontColour = {255,255,255};
    char letter[2];
    int i;
    char *p;

    letter[1] = 0;

    p = findFile(va("%s/%s", MISCDIR, video.fontFile));
    
    if (!p) {
        conAdd(LERR, "Could not open font");
        return 0;
    }

    font = TTF_OpenFont(p, video.fontSize);

    if (!font) {
        conAdd(LERR, "Could not open %s", p);
        return 0;
    }

    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
#ifdef TTF_HINTING_NORMAL
    TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
#endif

    memset(fonts, 0, sizeof(fonts));
    fontHeight = 0;

    for (i = 32; i < 128; i++) {

        letter[0] = i;

        fontSurface = TTF_RenderText_Blended(font, letter, fontColour);

        if (!fontSurface) {

#ifdef WIN32
            MessageBox(NULL, TTF_GetError(), "gravit: font engine error", MB_OK);
#else
            printf("%s\n", TTF_GetError());
#endif
            TTF_CloseFont(font);
            return 0;

        }

        fonts[i].ow = fontSurface->w;
        fonts[i].oh = fontSurface->h;

        if (fonts[i].oh > fontHeight)
            fontHeight = (float)fonts[i].oh;

        fonts[i].w = gfxPowerOfTwo(fonts[i].ow);
        fonts[i].h = gfxPowerOfTwo(fonts[i].oh);

        if (fonts[i].w > fonts[i].h)
            fonts[i].h = fonts[i].w;
        if (fonts[i].h > fonts[i].w)
            fonts[i].w = fonts[i].h;

        tmp = SDL_CreateRGBSurface(SDL_SWSURFACE, fonts[i].w, fonts[i].h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks depend on byteorder */
			0x000000FF,
			0x0000FF00,
			0x00FF0000,
			0xFF000000);
#else
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF);
#endif

        if (!tmp) {

            TTF_CloseFont(font);
            SDL_FreeSurface(fontSurface);

            return 0;

        }


	/* Save the alpha blending attributes */
	saved_flags = fontSurface->flags & (SDL_SRCALPHA|SDL_RLEACCELOK);
#if SDL_VERSION_ATLEAST(1, 3, 0)
	SDL_GetSurfaceAlphaMod(fontSurface, &saved_alpha);
	SDL_SetSurfaceAlphaMod(fontSurface, 0xFF);
#else
	saved_alpha = fontSurface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(fontSurface, 0, 0);
	}
#endif

	/* copy to texture surface */
        if (SDL_BlitSurface(fontSurface, NULL, tmp, NULL)) {

            TTF_CloseFont(font);
            SDL_FreeSurface(tmp);
            SDL_FreeSurface(fontSurface);

            return 0;

        }

        glGenTextures(1, &fonts[i].id);
        glCheck();

        glBindTexture(GL_TEXTURE_2D, fonts[i].id);
        glCheck();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fonts[i].w, fonts[i].h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);
        glCheck();

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glCheck();

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glCheck();

        SDL_FreeSurface(tmp);
        SDL_FreeSurface(fontSurface);

    }

    TTF_CloseFont(font);

    return 1;


}

void drawFontLetter(float x, float y, int letter) {

    // do not draw blanks and unprintable chars
    if (letter <= 32) return;
    if (letter > 127) letter = (int)'?';

    // make sure the font letter has a valid texture ID
    if ((fonts[letter].id == 0) || (!glIsTexture(fonts[letter].id))) {
        conAdd(LERR, "texture id %u for character 0x%x is invalid", fonts[letter].id, letter);
    }

    // draw letter texture
    glBindTexture(GL_TEXTURE_2D, fonts[letter].id);
    //glCheck();

    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0, 0); glVertex2i(x, y);
        glTexCoord2f(1, 0); glVertex2i(x+fonts[letter].w, y);
        glTexCoord2f(0, 1); glVertex2i(x, y+fonts[letter].h);
        glTexCoord2f(1, 1); glVertex2i(x+fonts[letter].w, y+fonts[letter].h);
    glEnd();

}

float drawFontWord(float x, float y, char *word) {

    int i;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (i = 0; i < (signed)strlen(word); i++) {

        drawFontLetter(x, y, word[i]);
        x += fonts[(int)word[i]].ow;

    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheck();

    return y + fontHeight;

}

float getWordWidth(char *s) {

    int i;
    float x = 0;

    for (i = 0; i < (signed)strlen(s); i++) {

        x += fonts[(int)s[i]].ow;

    }

    return x;

}

float getnWordWidth(char *s, int n) {

    int i;
    float x = 0;

    for (i = 0; i < n; i++) {

        x += fonts[(int)s[i]].ow;

    }

    return x;

}

void drawFontWordRA(float x, float y, char *word) {

    int i;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (i = (int)strlen(word) - 1; i >= 0; i--) {

        x -= fonts[(int)word[i]].ow;
        drawFontLetter(x, y, word[i]);

    }
    glCheck();
}

void drawFontWordCA(float x, float y, char *word) {
    
    drawFontWord((int)(x - getWordWidth(word) / 2), y, word);
    
}

#endif
