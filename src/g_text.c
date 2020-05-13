#include "g_text.h"
#include "simple_logger.h""

/*typedef struct
{
    Uint32 maxTextures;     /**<max entities supported by the system*/
    SDL_Texture* textureList; /**<pointer to an allocated array of entities*/
/*}TextureManager;

static TextureManager texture_manager = { 0 };

void texture_manager_init(Uint32 maxTextures)
{
    if (texture_manager.textureList != NULL)
    {
        //TODO: cleanup
    }
    if (!maxTextures)
    {
        slog("cannot intialize a zero size texture list!");
        return;
    }
    texture_manager.textureList = (SDL_Texture*)malloc(sizeof(SDL_Texture *) * maxTextures);
    if (texture_manager.textureList == NULL)
    {
        slog("failed to allocate %i entities for the texture manager", maxTextures);
        return;
    }
    texture_manager.maxTextures = maxTextures;
    memset(texture_manager.textureList, 0, sizeof(SDL_Texture *) * maxTextures);
    slog("texture manager initialized");
    atexit(texture_manager_close);
}*/

SDL_Texture *LoadFromRenderedText(TTF_Font* font, const char* textureText, SDL_Color textColor)
{
    SDL_Texture *TextTexture = NULL;
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
    return TextTexture;
}

/*void texture_manager_close()
{
    int i;
    for (i = 0; i < texture_manager.maxTextures; i++)
    {
        if (&texture_manager.textureList[i]._inuse)
        {
            texture_free(&texture_manager.textureList[i]);
        }
    }
    texture_manager.maxTextures = 0;
    free(texture_manager.textureList);
    texture_manager.textureList = NULL;
    slog("texture manager closed");
}*/

