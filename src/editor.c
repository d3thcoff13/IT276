#include "string.h"
#include "g_editor.h"
#include "SDL_ttf.h"
#include "g_text.h"
#include "simple_logger.h"
#include "entity.h"
#include "g_menu.h"
#include "gf2d_draw.h"
#include "level.h"

typedef enum {
	EditorMainMenu,
	SetBackground,
	ToolBox,
	LevelView,
	Save
}EditorScreen;

typedef struct {
	char* name;
	char* text;
	SDL_Rect rect;
	SDL_Texture *texture;
}TextBox;

typedef struct {
	TextBox** menu_categories;
	TextBox* menu_items;
	char* selectionName;
	int active;
	int selection;
	EditorScreen type;
	TTF_Font* font;
	SDL_Color color;
	SDL_Color highlight;
	Entity* selectedObject;
	int objectSelected;
	char* objectList;
}EditorMenu;

EditorMenu* edmainmenu, *setbackground, *toolbox, *levelview, *save;
void initEditor(int type) {
	if (type == 0) {
		if (edmainmenu == NULL) {
			edmainmenu = (EditorMenu*)malloc(sizeof(EditorMenu));
			edmainmenu->type = EditorMainMenu;
			edmainmenu->font = TTF_OpenFont("../../images/calibri.ttf", 28);
			edmainmenu->color = (SDL_Color){ 255,255,255 };
			edmainmenu->highlight = (SDL_Color){ 255,255,0 };
			edmainmenu->menu_items = (TextBox*)malloc(sizeof(TextBox) * 4);

			edmainmenu->menu_items[0] = (TextBox){ "New" , "New", (SDL_Rect) { 810,600,120,30 } ,NULL };
			edmainmenu->menu_items[1] = (TextBox){ "Load" , "Load", (SDL_Rect) { 270,600,120,30 } ,NULL };
			edmainmenu->menu_items[2] = (TextBox){ "FlieName" , "Enter Name of file to save or load", (SDL_Rect) { 270,300,300,30 } ,NULL };
			edmainmenu->menu_items[3] = (TextBox){ "FlieName" , "Filename", (SDL_Rect) { 270,450,300,30 } ,NULL };
			edmainmenu->menu_items[3].text = (char*)malloc(sizeof(char) * 50);
			strcpy(edmainmenu->menu_items[3].text, "Filename");
		}
		edmainmenu->selection = 0;
		edmainmenu->active = 1;
		currentmenu = EditorMainMenu;
	}
	else if (type == 1) {
		if (setbackground == NULL) {
			setbackground = (EditorMenu*)malloc(sizeof(EditorMenu));
			setbackground->type = SetBackground;
			setbackground->font = TTF_OpenFont("../../images/calibri.ttf", 28);
			setbackground->color = (SDL_Color){ 255,255,255 };
			setbackground->highlight = (SDL_Color){ 255,255,0 };
			setbackground->menu_items = (TextBox*)malloc(sizeof(TextBox) * 2);
			setbackground->selection = 0;
			setbackground->menu_items[0].rect = (SDL_Rect){ 540,220,300,30 };
			setbackground->menu_items[0].text = (char*)malloc(sizeof(char) * 50);
			strcpy(setbackground->menu_items[0].text, "Enter File Path");
			setbackground->menu_items[1] = (TextBox){ "background" , "Enter filepath of background and press enter", (SDL_Rect) { 540,160,300,30 } ,NULL };

		}
		SDL_StartTextInput();
		setbackground->active = 1;
		currentmenu = SetBackground;
	}
	else if (type == 2) {
		if (toolbox == NULL) {
			toolbox = (EditorMenu*)malloc(sizeof(EditorMenu));
			toolbox->type = ToolBox;
			toolbox->font = TTF_OpenFont("../../images/calibri.ttf", 28);
			toolbox->color = (SDL_Color){ 255,255,255 };
			toolbox->highlight = (SDL_Color){ 255,255,0 };
			toolbox->menu_categories = (TextBox**)malloc(sizeof(TextBox*) * 3);

			toolbox->menu_categories[0] = (TextBox*)malloc(sizeof(TextBox) * 6);
			toolbox->menu_categories[0][0] = (TextBox){ "player", "Player", (SDL_Rect) { 300, 100, 120, 30 }, NULL };
			toolbox->menu_categories[0][1] = (TextBox){ "pyro", "Pyromancer", (SDL_Rect) { 300, 200, 120, 30 }, NULL };
			toolbox->menu_categories[0][2] = (TextBox){ "bat", "Bat", (SDL_Rect) { 300, 300, 120, 30 }, NULL };
			toolbox->menu_categories[0][3] = (TextBox){ "wolf", "Wolf", (SDL_Rect) { 300, 400, 120, 30 }, NULL };
			toolbox->menu_categories[0][4] = (TextBox){ "necro", "Necromancer", (SDL_Rect) { 300, 500, 120, 30 }, NULL };
			toolbox->menu_categories[0][5] = (TextBox){ "skeleton", "Skeleton", (SDL_Rect) { 300, 600, 120, 30 }, NULL };



			toolbox->menu_categories[1] = (TextBox*)malloc(sizeof(TextBox) * 5);
			toolbox->menu_categories[1][0] = (TextBox){ "lightningdoor", "Lightning Door", (SDL_Rect) { 600, 100, 120, 30 }, NULL };
			toolbox->menu_categories[1][1] = (TextBox){ "demondoor", "Demon Door", (SDL_Rect) { 600, 200, 120, 30 }, NULL };
			toolbox->menu_categories[1][2] = (TextBox){ "boulder", "Boulder", (SDL_Rect) { 600, 300, 120, 30 }, NULL };
			toolbox->menu_categories[1][3] = (TextBox){ "spikes", "Spikes", (SDL_Rect) { 600, 400, 120, 30 }, NULL };
			toolbox->menu_categories[1][4] = (TextBox){ "tree", "Tree", (SDL_Rect) { 600, 500, 120, 30 }, NULL };


			toolbox->menu_categories[2] = (TextBox*)malloc(sizeof(TextBox));
			toolbox->menu_categories[2][0] = (TextBox){ "tile", "Tile", (SDL_Rect) { 900, 100, 120, 30 }, NULL };
		}

		toolbox->active = 1;
		currentmenu = ToolBox;
	}
	else if (type == 3) {
		if (levelview == NULL) {
			levelview = (EditorMenu*)malloc(sizeof(EditorMenu));
			levelview->objectList = (char*)malloc(sizeof(char) * 50);
			levelview->type = LevelView;
			levelview->font = TTF_OpenFont("../../images/calibri.ttf", 28);
			levelview->color = (SDL_Color){ 255,255,255 };
			if (edmainmenu->selection == 0) {
				load_level("../../levels/empty.json");
				get_level()->background = gf2d_sprite_load_image(setbackground->menu_items[0].text);
			}
			else if (edmainmenu->selection == 1) {
				load_level(edmainmenu->menu_items[3].text);
			}
		}
		levelview->active = 1;
		currentmenu = LevelView;
	}



	/*char* test[] = { "njnjnj", "nknknk" };
	SDL_StartTextInput();
	box = (TextBox*)malloc(sizeof(TextBox));
	box->rect = (SDL_Rect){ 540,160,120,30 };
	box->font = TTF_OpenFont("../../images/calibri.ttf", 28);
	box->color = (SDL_Color){ 255,255,255 };
	box->highlight = (SDL_Color){ 255,255,0 };
	box->text = (char*)malloc(sizeof(char) * 24);
	strcpy(box->text, test[0]);
	box->texture = LoadFromRenderedText(box->font, box->text, box->color);
	gf2d_graphics_render_texture_to_screen(box->texture, NULL, &box->rect);*/
}

void initEditorMenu() {
	
}


void ChangeEditorMenu(EditorMenu * oldMenu, int menu) {
	oldMenu->active = 0;
	currentmenu = menu;
	initEditor(menu);
	/*if (menu == ToolBox) {
		currentmenu = ToolBox;
		initEditor(2);
	}
	if (menu == LevelView) {
		c
	}*/
}

void Type(char* string) {
	SDL_Event e;
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(string) > 0)
			{
				BackSpace(string);
				//string[strlen(string)] = '\0';
				//slog(string[3]);
			}
			if (e.key.keysym.sym == SDLK_RETURN && currentmenu == 1){
				if (strlen(setbackground->menu_items[0].text) == 0)setbackground->menu_items[0].text = "../../images/backgrounds/bg_flat.png";
				ChangeEditorMenu(setbackground, 3);
			}
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			strcat(string, e.text.text);
			slog("%i", strlen(string));
			//log(box->text);

		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && currentmenu == 0)
		{
			if (MouseInBox(&edmainmenu->menu_items[0])){
				edmainmenu->selection = 0;
				ChangeEditorMenu(edmainmenu, 1);
			}
			else if (MouseInBox(&edmainmenu->menu_items[1])) {
				edmainmenu->selection = 1;
				ChangeEditorMenu(edmainmenu, 3);
			}
		}
	}
}

void DrawEditor(int type){
	if(type == EditorMainMenu){
		if(MouseInBox(&edmainmenu->menu_items[0])) edmainmenu->menu_items[0].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[0].text, edmainmenu->highlight);
		else edmainmenu->menu_items[0].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[0].text, edmainmenu->color);
		gf2d_draw_rect(edmainmenu->menu_items[0].rect, (Vector4D) { 255, 255, 255, 255 });
		gf2d_graphics_render_texture_to_screen(edmainmenu->menu_items[0].texture, NULL, &edmainmenu->menu_items[0].rect);
		
		if (MouseInBox(&edmainmenu->menu_items[1])) edmainmenu->menu_items[1].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[1].text, edmainmenu->highlight);
		else edmainmenu->menu_items[1].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[1].text, edmainmenu->color); 
		gf2d_draw_rect(edmainmenu->menu_items[1].rect, (Vector4D) { 255, 255, 255, 255 });
		gf2d_graphics_render_texture_to_screen(edmainmenu->menu_items[1].texture, NULL, &edmainmenu->menu_items[1].rect);

		edmainmenu->menu_items[3].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[3].text, edmainmenu->color);
		gf2d_draw_rect(edmainmenu->menu_items[3].rect, (Vector4D) { 255, 255, 255, 255 });
		gf2d_graphics_render_texture_to_screen(edmainmenu->menu_items[3].texture, NULL, &edmainmenu->menu_items[3].rect);

		edmainmenu->menu_items[2].texture = LoadFromRenderedText(edmainmenu->font, edmainmenu->menu_items[2].text, edmainmenu->color);
		gf2d_graphics_render_texture_to_screen(edmainmenu->menu_items[2].texture, NULL, &edmainmenu->menu_items[2].rect);

	}
	else if (type == 1) {
		setbackground->menu_items[0].texture = LoadFromRenderedText(setbackground->font, setbackground->menu_items[0].text, setbackground->color);
		gf2d_draw_rect(setbackground->menu_items[0].rect, (Vector4D) { 255, 255, 255, 255 });
		gf2d_graphics_render_texture_to_screen(setbackground->menu_items[0].texture, NULL, &setbackground->menu_items[0].rect);
		setbackground->menu_items[1].texture = LoadFromRenderedText(setbackground->font, setbackground->menu_items[1].text, setbackground->color);
		gf2d_graphics_render_texture_to_screen(setbackground->menu_items[1].texture, NULL, &setbackground->menu_items[1].rect);
	}
	else if (type == 2) {
		TextBox* textbox;
		int size;
		for (int i = 0; i < 3; i++) {
			if (i == 0)size = 6;
			else if (i == 1)size = 5;
			else if (i == 2)size = 1;
			for(int j = 0; j < size; j++){
				textbox = &toolbox->menu_categories[i][j];
				if (MouseInBox(textbox) == 1) {
					textbox->texture = LoadFromRenderedText(toolbox->font, textbox->text, toolbox->highlight);
					toolbox->selectionName = textbox->name;
				}else textbox->texture = LoadFromRenderedText(toolbox->font, textbox->text, toolbox->color);
				gf2d_draw_rect(textbox->rect, (Vector4D) { 255, 255, 255, 255 });
				gf2d_graphics_render_texture_to_screen(textbox->texture, NULL, &textbox->rect);
			}
		}
	}
	else if (type == LevelView) {
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		mx = ((int)mx / 32) * 32;
		my = ((int)my / 32) * 32;
		draw_tiles(get_level());
		if (levelview->objectSelected == 1) {
			gf2d_sprite_draw(levelview->selectedObject->sprite, vector2d(mx,my), NULL, NULL, NULL, NULL, &get_level()->color, 0);
			levelview->selectedObject->position.x = mx;
			levelview->selectedObject->position.y = my;
		}
	}
	/*type(box->text);
		if (MouseInBox() == 1)box->texture = LoadFromRenderedText(->font, box->text, box->highlight);
		else box->texture = LoadFromRenderedText(box->font, box->text, box->color);
		gf2d_draw_rect(box->rect, (Vector4D) { 255, 255, 255, 255 });
		gf2d_graphics_render_texture_to_screen(box->texture, NULL, &box->rect);*/
}

void BackSpace(char *string) {
	int i=0,length = strlen(string);
	char substring[50];
	strcpy(substring, string);
	substring[strlen(string)-1] = '\0';
	strcpy(string, substring);}

int CheckClick(char* name) {
	TextBox* textbox;
	int size;
	for (int i = 0; i < 3; i++) {
		if (i == 0)size = 6;
		else if (i == 1)size = 5;
		else if (i == 2)size = 1;
		for (int j = 0; j < size; j++) {
			textbox = &toolbox->menu_categories[i][j];
			if (MouseInBox(textbox))return 1;
		}
	}
	toolbox->selectionName = NULL;
	return 0;
}

int MouseInBox(TextBox *box) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	if ((mx<(box->rect.x + box->rect.w) && mx >(box->rect.x)) && (my<(box->rect.y + box->rect.h) && my >(box->rect.y)))return 1;
	else return 0;

}

void SelectTool(char* name){
	levelview->objectSelected = 1;
	levelview->selectedObject = LoadEntity(name);
	ChangeEditorMenu(toolbox,LevelView);
}

Entity* LoadEntity(char *name) {
	Entity* self;
	self = entity_new();
	if (name == "tile")self->sprite = get_level()->tilesheet;
	else if (name == "player")self->sprite = gf2d_sprite_load_all("../../images/spritesheets/adventurer.png", 32, 64, 6);
	else if (name == "bat")self->sprite = gf2d_sprite_load_all(".. /../images/spritesheets/bat.png", 32, 32, 7);
	else if (name == "wolf")self->sprite = gf2d_sprite_load_all("../ ../images/spritesheets/wolf.png", 64, 32, 9);
	else if (name == "necro")self->sprite = gf2d_sprite_load_all("../../images/spritesheets/goblin_mage.png", 32, 64, 6);
	else if (name == "pyro")self->sprite = gf2d_sprite_load_all("../../images/spritesheets/goblin_redmage.png", 32, 64, 6);
	else if (name == "skeleton")self->sprite = gf2d_sprite_load_all("../../images/spritesheets/skeleton.png", 32, 64, 1);
	self->name = name;
	self->frame = 0;
	self->position.x = 100;
	self->position.y = 100;
	return self;
}


///////////////// Controls//////////////////////////////////////
SDL_Event event;
void GetEdMenuInputs(int type) {
	if (type == EditorMainMenu) {
		Type(edmainmenu->menu_items[3].text);
	}
	else if (type == SetBackground) {
		Type(setbackground->menu_items[0].text);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_RETURN:
					if (strlen(setbackground->menu_items[0].text) == 0)setbackground->menu_items[0].text = "../../images/backgrounds/bg-flat.png";
					ChangeEditorMenu(setbackground, 3);
					break;
				/*case SDLK_w:
					if (edmainmenu->selection == 0)edmainmenu->selection = 1;
					else if (edmainmenu->selection == 1)edmainmenu->selection = 0;
					break;
				case SDLK_s:
					if (edmainmenu->selection == 0)edmainmenu->selection = 1;
					else if (edmainmenu->selection == 1)edmainmenu->selection = 0;
					break;
				case SDLK_j:
					//if (edmainmenu->selection == 0)edm;
					//else 
					if (edmainmenu->selection == 1)ChangeEditorMenu(edmainmenu, 3);
					break;*/

				}
			//case SDL_TEXTINPUT:
			//break;
			}
		}
	}
	else if (type == ToolBox) {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if(CheckClick(toolbox->selectionName) == 1)slog(toolbox->selectionName);
				SelectTool(toolbox->selectionName);
				break;
			}
		}
	}
	else if (type == LevelView) {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_i:
					ChangeEditorMenu(levelview, ToolBox);
					break;
				case SDLK_s:
					SaveLevel(edmainmenu->menu_items[3].text);
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				if (levelview->objectSelected == 1 && levelview->selectedObject->name =="tile") {
					int mx, my;
					SDL_GetMouseState(&mx, &my);
					get_level()->tiles[(int)my/32][(int)mx/32] = 1;
					levelview->objectSelected = 0;
					entity_free(levelview->selectedObject);
					levelview->selectedObject = NULL;
				}
				else if (levelview->objectSelected == 1) {
					levelview->objectSelected = 0;
					//levelview->selectedObject->position.x = 
				}
				break;
			}
		}
	}
}