#include "gf2d_sprite.h"
#include "entity.h"
#include "SDL_ttf.h"

typedef struct menu_s {
	Entity* cursor;
	Entity** menu_items;
	int selection;
	int active;
	int type;
	TTF_Font *font;
}menu;

menu *mainmenu;
menu* pausemenu;
void initMenu(int type);

void GetPauseMenuInputs();

void GetMainMenuInputs();

void DrawMenu(menu *Menu);

