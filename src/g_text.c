#include "g_text.h"


SDL_Texture* GetTextTexture() {
    return TextTexture;
}
Bool LoadFromRenderedText(TTF_Font* font, const char* textureText, SDL_Color textColor)
{
    TextTexture = NULL;
    SDL_Renderer *renderer = gf2d_graphics_get_renderer();
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText, textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        TextTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (TextTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
            /*{
                //Get image dimensions
                mWidth = textSurface->w;
                mHeight = textSurface->h;
            }*/
            textSurface->w = 320;
        textSurface->h = 100;
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return TextTexture != NULL;
}