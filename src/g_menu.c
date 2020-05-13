#include "g_menu.h"
#include "g_text.h"
#include "game.h"

void initMenu(int type) {
	if (type == 0) {
		mainmenu = (menu*)malloc(sizeof(menu));
		mainmenu->type = 0;
		mainmenu->selection = 0;
		mainmenu->font = TTF_OpenFont("../../images/calibri.ttf", 28);
	}
}

void DrawMenu(menu *Menu) {
	SDL_Rect rect1, rect2;
	SDL_Texture* start, * quit;
	switch (Menu->type) {
	case 0:
		rect1 = (SDL_Rect){ 540,160,120,30 };
		rect2 = (SDL_Rect){540,220,120,30 };
		SDL_Color colorDefault = { 255,255,255 };
		SDL_Color colorSelected = { 255,255,0 };
		if(mainmenu->selection == 0)start = LoadFromRenderedText(mainmenu->font,"Start", colorSelected);
		else start = LoadFromRenderedText(mainmenu->font, "Start", colorDefault);
		if (mainmenu->selection == 1)quit = LoadFromRenderedText(mainmenu->font, "Quit", colorSelected);
		else quit = LoadFromRenderedText(mainmenu->font, "Quit", colorDefault);
		gf2d_graphics_render_texture_to_screen(start, NULL, &rect1);
		gf2d_graphics_render_texture_to_screen(quit, NULL, &rect2);
		break;
	default:
		break;
	}
}

SDL_Event event;
void GetMainMenuInputs() {
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_w:
				if (mainmenu->selection == 0)mainmenu->selection = 1;
				else if (mainmenu->selection == 1)mainmenu->selection = 0;
				break;
			case SDLK_s:
				if (mainmenu->selection == 0)mainmenu->selection = 1;
				else if (mainmenu->selection == 1)mainmenu->selection = 0;
				break;
			case SDLK_j:
				if (mainmenu->selection == 0)SetGameState(InGame);
				else if (mainmenu->selection == 1)SetGameState(Quit);
			}

		}
	}
}