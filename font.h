
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
