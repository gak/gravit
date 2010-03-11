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

#ifndef FONT_H_
#define FONT_H_

#ifndef NO_GUI

typedef struct font_s {

    GLuint id;
    int w;
    int h;
    int ow;
    int oh;

} font_t;

extern float fontHeight;
void drawFontWordRA(float x, float y, char *word);
float drawFontWord(float x, float y, char *word);
int loadFonts();
float getWordWidth(char *s);
float getnWordWidth(char *s, int n);

#endif

#endif
