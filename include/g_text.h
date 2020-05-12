#include "gf2d_graphics.h"
#include "simple_logger.h"
SDL_Texture *TextTexture;
SDL_Texture * GetTextTexture();
Bool LoadFromRenderedText(TTF_Font* font, const char* textureText, SDL_Color textColor);