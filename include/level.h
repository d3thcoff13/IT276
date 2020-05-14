#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"
#include "simple_json.h"
#include "player.h"

typedef struct{
	SDL_Rect bounds;
	Sprite *background;
	Sprite *tilesheet;
	Vector4D color;
	int** tiles;
	int width;
	int height;
	int tilewidth;
	int tileheight;
	int tileimages;
	float levelNumber;
}Level;

Level* load_level(const char* filename);
void load_level_entities(SJson* list);
void draw_tiles(Level* level);
Level* get_level();
void change_level(float targetlevel);
void RestartLevel();
void SaveLevel(char* filename);
#endif