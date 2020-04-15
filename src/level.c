#include <stdio.h>
#include "level.h"
#include "m_Necromancer.h"
#include "m_skeleton.h"
#include "m_bat.h"
#include "m_wolf.h"
#include "m_pyromancer.h"
#include "simple_logger.h"
#include "game.h"
#include "obstacles.h"

Level* load_level(const char* filename){
	level = (Level *)malloc(sizeof(Level));
	SJson *value,*file;
	char* tilesheet;
	int numOfTiles;
	level->color = vector4d(255, 255, 255, 255);

	file = sj_load(filename);
	if (file == NULL){
		slog("file not found");
		return level;
	}
	
	value = sj_object_get_value(file, "height");
	sj_get_integer_value(value, &level->height);
	slog("here %i",level->height);
	value = sj_object_get_value(file, "width");
	sj_get_integer_value(value, &level->width);

	value = sj_object_get_value(file, "layers");
	level->tiles = sj_array_2d(sj_array_get_nth(value, 0), level->height, level->width);
	

	value = sj_object_get_value(file, "tileheight");
	sj_get_integer_value(value, &level->tileheight);

	value = sj_object_get_value(file, "tilewidth");
	sj_get_integer_value(value, &level->tilewidth);

	value = sj_object_get_value(file, "tilesheet");
	tilesheet = sj_get_string_value(value);

	value = sj_object_get_value(file, "tileimages");
	sj_get_integer_value(value, &numOfTiles);

	level->tilesheet = gf2d_sprite_load_all(tilesheet, level->tilewidth, level->tileheight, numOfTiles);
	load_level_entities(sj_object_get_value(file, "objects"));
	slog("Level loaded: %s height,width:%i,%i tileheight,tilewidth:%i,%i ", level->tilesheet->filepath, level->height, level->width, level->tileheight, level->tilewidth);

	return level;
}

void load_level_entities(SJson* list){
	SJson *object,*objectContent;
	int ctr = 0;
	for (int j = 0; j < sj_array_get_count(list); j++){
		object = sj_array_get_nth(list, ctr);
		objectContent = sj_object_get_value(object, "name");
		Entity* entity = entity_new();

		if (!strcmp(sj_get_string_value(objectContent),"player")){
			init_player(entity);
			player_entity = entity;
		}
		else if (!strcmp(sj_get_string_value(objectContent), "necro")) {
			necro_spawn(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "skeleton")) {
			skeleton_spawn(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "bat")) {
			bat_spawn(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "wolf")) {
			wolf_spawn(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "pyro")) {
			pyro_spawn(entity);
			entity_free(entity);
		}else if (!strcmp(sj_get_string_value(objectContent), "tree")) {
			init_tree(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "boulder")) {
			init_boulder(entity);
		}
		else if (!strcmp(sj_get_string_value(objectContent), "demondoor")) {
			init_demon_door(entity);
		}else if (!strcmp(sj_get_string_value(objectContent), "lightningdoor")) {
			init_lightning_door(entity);
		}else if (!strcmp(sj_get_string_value(objectContent), "spikes")) {
			init_spikes(entity);
		}
		ctr++;
		int x, y;
		
		objectContent = sj_object_get_value(object, "x");
		sj_get_integer_value(objectContent, &x);
		objectContent = sj_object_get_value(object, "y");
		sj_get_integer_value(objectContent, &y);
		entity_set_position(entity, vector2d(x, y));

	}
	
}

void draw_tiles(Level* level){
	for (int i = 0; i < level->height; i++){
		for (int j = 0; j < level->width; j++){
			if (level->tiles[i][j] == 0)
				continue;
			gf2d_sprite_draw(level->tilesheet, vector2d(j * 32, i * 32), NULL, NULL, NULL, NULL, &level->color, 0);
		}
	}
}

void change_level(int targetlevel) {
	if (level != NULL) {
		memset(level->tiles, 0, sizeof(int*));
		memset(level, 0, sizeof(Level));
	}
	if(targetlevel == 1)level = load_level("../../levels/demolevel.json"); 
	else if(targetlevel == 2) level = load_level("../../levels/demo2.json");
}

Level* get_level() {
	return level;
}