#include "entity.h"
#include "gf2d_graphics.h"
#include "game.h"
#include "SDL_ttf.h"
#include "g_text.h"

/*struct HUD {
    SDL_Texture* textureList;
    float *player_health;
    int *player_maxHealth;
    int *player_mana;
};

static HUD hud = { 0 };
*/
void init_HUD();
void Update_HUD();

void Update_HUD_Stat();
void Draw_HUD(SDL_Rect rect, SDL_Texture* hud_item);