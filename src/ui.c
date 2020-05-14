#include "gf2d_graphics.h"
#include "game.h"
#include "SDL_ttf.h"
#include "g_text.h"
#include "ui.h"

typedef struct {
    char* name;
    SDL_Texture *texture;
    float stat;
    float maxstat;
    TTF_Font* font;
    SDL_Rect rect;
    SDL_Color color;
    enum WeaponType *weapon;
}HUD_element;

typedef struct{
    HUD_element* textureList;
    HUD_element health;
    HUD_element weapon;
}HUD;

static HUD hud = { 0 };

void init_HUD() {
    hud.textureList = (HUD_element*)malloc(sizeof(HUD_element) * 10);
    if (hud.textureList == NULL)
    {
        slog("failed to allocate %i entities for the entity manager", 10);
        return;
    }
    memset(hud.textureList, 0, sizeof(HUD_element) * 10);
    
    hud.textureList[0].font = TTF_OpenFont("../../images/calibri.ttf", 48);
    SDL_Rect rect = { 200,640,120, 40 };
    hud.textureList[0].rect = rect;

    SDL_Color color = { 255,255,255 };
    hud.textureList[0].color = color;

    char health[24], max[24], *output;
    output = (char*)malloc( sizeof (char) * 24);
    hud.textureList[0].stat = player_entity->health;
    hud.textureList[0].maxstat = player_entity->maxHealth;
    _gcvt(hud.textureList[0].stat, 3, health);
    _gcvt(hud.textureList[0].maxstat, 3, max);
    strcpy(output, "Health: ");
    strcat(output, health);
    strcat(output, " / ");
    strcat(output, max);

    hud.textureList[0].texture = LoadFromRenderedText(hud.textureList[0].font, output, hud.textureList[0].color);
    //other stat

    hud.textureList[1].font = TTF_OpenFont("../../images/calibri.ttf", 48);

    hud.textureList[1].rect = (SDL_Rect){ 200, 680, 120, 40 };


    hud.textureList[1].color = color;

    char weapon[24];
    output = (char*)malloc(sizeof(char) * 24);
    hud.textureList[1].stat = (float)player_entity->weapon->weaponType;
    strcpy(output, "Weapon: ");
    if (hud.textureList[1].stat == 0)strcat(output, "Dagger");
    else if (hud.textureList[1].stat == 1)strcat(output, "Rapier");
    else if (hud.textureList[1].stat == 2)strcat(output, "Mace");
    else if (hud.textureList[1].stat == 3)strcat(output, "Spear");
    else if (hud.textureList[1].stat == 4)strcat(output, "Shortsword");
    else strcat(output, "Error");
    hud.textureList[1].texture = LoadFromRenderedText(hud.textureList[1].font, output, hud.textureList[1].color);






}

void Update_HUD() {
    for (int i = 0; i < 2; i++)Update_HUD_Stat(i);
    for (int i = 0; i < 2; i++)Draw_HUD(hud.textureList[i].rect, hud.textureList[i].texture);
}

void Update_HUD_Stat(int stat) {
    char* output = NULL;
    if (stat == 0) {
        char health[24], max[24];
        output = (char*)malloc(sizeof(char) * 24);
        hud.textureList[0].stat = player_entity->health;
        hud.textureList[0].maxstat = player_entity->maxHealth;
        _gcvt(hud.textureList[0].stat, 3, health);
        _gcvt(hud.textureList[0].maxstat, 3, max);
        strcpy(output, "Health: ");
        strcat(output, health);
        strcat(output, " / ");
        strcat(output, max);
        hud.textureList[0].texture = LoadFromRenderedText(hud.textureList[0].font, output, hud.textureList[0].color);
    }
    else if(stat == 1){
        char weapon[24];
        output = (char*)malloc(sizeof(char) * 24);
        hud.textureList[1].stat = (float)player_entity->weapon->weaponType;
        strcpy(output, "Weapon: ");
        if (hud.textureList[1].stat == 0)strcat(output, "Dagger");
        else if (hud.textureList[1].stat == 1)strcat(output, "Rapier");
        else if (hud.textureList[1].stat == 2)strcat(output, "Mace");
        else if (hud.textureList[1].stat == 3)strcat(output, "Spear");
        else if (hud.textureList[1].stat == 4)strcat(output, "Shortsword");
        else strcat(output, "Error");
        hud.textureList[1].texture = LoadFromRenderedText(hud.textureList[1].font, output, hud.textureList[1].color);
    }

}
void Draw_HUD(SDL_Rect rect, SDL_Texture* hud_item) {
    gf2d_graphics_render_texture_to_screen(hud_item, NULL, &rect);
}