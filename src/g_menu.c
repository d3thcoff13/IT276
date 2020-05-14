#include "g_menu.h"
#include "g_text.h"
#include "game.h"

void initMenu(int type) {
	if (type == 0) {
		mainmenu = (menu*)malloc(sizeof(menu));
		mainmenu->type = type;
		mainmenu->selection = 0;
		mainmenu->font = TTF_OpenFont("../../images/calibri.ttf", 28);
	}
	if (type == 1) {
		pausemenu = (menu*)malloc(sizeof(menu));
		pausemenu->type = type;
		pausemenu->selection = 0;
		pausemenu->active = 0;
		pausemenu->font = TTF_OpenFont("../../images/calibri.ttf", 28);
	}
}

void DrawMenu(menu *Menu) {
	SDL_Rect rect1, rect2, rect3, rect4;
	SDL_Texture* start, * quit, *paused, *resume, *editor;
	SDL_Color colorSelected, colorDefault;
	switch (Menu->type) {
	case 0:
		rect1 = (SDL_Rect){ 540,160,120,30 };
		rect2 = (SDL_Rect){540,220,120,30 };
		rect3 = (SDL_Rect){ 540,300,120,30 };
		colorDefault = (SDL_Color){ 255,255,255 };
		colorSelected = (SDL_Color){ 255,255,0 };
		if(mainmenu->selection == 0)start = LoadFromRenderedText(mainmenu->font,"Start", colorSelected);
		else start = LoadFromRenderedText(mainmenu->font, "Start", colorDefault);
		if (mainmenu->selection == 1)editor = LoadFromRenderedText(mainmenu->font, "Editor", colorSelected);
		else editor = LoadFromRenderedText(mainmenu->font, "Editor", colorDefault);
		if (mainmenu->selection == 2)quit = LoadFromRenderedText(mainmenu->font, "Quit", colorSelected);
		else quit = LoadFromRenderedText(mainmenu->font, "Quit", colorDefault);
		gf2d_graphics_render_texture_to_screen(start, NULL, &rect1);
		gf2d_graphics_render_texture_to_screen(editor, NULL, &rect2);
		gf2d_graphics_render_texture_to_screen(quit, NULL, &rect3);
		break;
	case 1:
		rect1 = (SDL_Rect){ 540,160,120,30 };
		rect2 = (SDL_Rect){ 540,220,120,30 };
		rect3 = (SDL_Rect){ 540,240,120,30 };
		colorDefault = (SDL_Color){ 255,255,255 };
		colorSelected = (SDL_Color){ 255,255,0 };
		//if (mainmenu->selection == 0)paused = LoadFromRenderedText(mainmenu->font, "Paused", colorSelected);
		paused = LoadFromRenderedText(mainmenu->font, "Paused", colorDefault);
		gf2d_graphics_render_texture_to_screen(paused, NULL, &rect1);
		if (pausemenu->selection == 0)resume = LoadFromRenderedText(pausemenu->font, "Resume", colorSelected);
		else resume = LoadFromRenderedText(mainmenu->font, "Resume", colorDefault);
		gf2d_graphics_render_texture_to_screen(resume, NULL, &rect2);
		if (pausemenu->selection == 1)quit = LoadFromRenderedText(pausemenu->font, "Quit", colorSelected);
		else quit = LoadFromRenderedText(mainmenu->font, "Quit", colorDefault);

		gf2d_graphics_render_texture_to_screen(quit, NULL, &rect3);
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
					if (mainmenu->selection == 0) mainmenu->selection = 2;
					else mainmenu->selection--;
					break;
				case SDLK_s:
					if (mainmenu->selection == 2) mainmenu->selection = 0;
					else mainmenu->selection++;
					break;
				case SDLK_j:
					if (mainmenu->selection == 0)SetGameState(InGame);
					else if (mainmenu->selection == 1)SetGameState(Editor);
					else if (mainmenu->selection == 2)SetGameState(Quit);
					break;
			}

		}
	}
}

void GetPauseMenuInputs() {
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				/*case SDLK_w:
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
				}*/
			case SDLK_p:
				pausemenu->active = 0;
			case SDLK_w:
				if (pausemenu->selection == 0)pausemenu->selection = 1;
				else if (pausemenu->selection == 1)pausemenu->selection = 0;
				break;
			case SDLK_s:
				if (pausemenu->selection == 0)pausemenu->selection = 1;
				else if (pausemenu->selection == 1)pausemenu->selection = 0;
				break;
			case SDLK_j:
				if (pausemenu->selection == 0)pausemenu->active = 0;
				else if (pausemenu->selection == 1)SetGameState(Quit);
				break;

			}
		}
	}
}
