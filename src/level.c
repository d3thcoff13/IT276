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
#include "g_editor.h"

Level* load_level(const char* filename){
	level = (Level *)malloc(sizeof(Level));
	SJson *value,*file;
	char* tilesheet;
	const char* bgpath;
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

	value = sj_object_get_value(file, "background");
	bgpath = sj_get_string_value(value);
	level->background = gf2d_sprite_load_image(bgpath);

	level->tilesheet = gf2d_sprite_load_all(tilesheet, level->tilewidth, level->tileheight, numOfTiles);
	if(currentGameState == InGame)load_level_entities(sj_object_get_value(file, "objects"));
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
		//	entity_free(entity);
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
	gf2d_sprite_draw_image(level->background, vector2d(-10, 0));
	for (int i = 0; i < level->height; i++){
		for (int j = 0; j < level->width; j++){
			if (level->tiles[i][j] == 0)
				continue;
			gf2d_sprite_draw(level->tilesheet, vector2d(j * 32, i * 32), NULL, NULL, NULL, NULL, &level->color, 0);
		}
	}
}

void change_level(float targetlevel) {
	if (level != NULL) {
		memset(level->tiles, 0, sizeof(int*));
		memset(level, 0, sizeof(Level));
		free(level);
		level = NULL;
	}
	if(targetlevel == 1)level = load_level("../../levels/demolevel.json"); 
	else if(targetlevel == 2) level = load_level("../../levels/demo2.json");
}

Level* get_level() {
	return level;
}

void RestartLevel() {
	entity_free_all();
	change_level(currentLevel);
}

void SaveLevel(char * filename) {
	Entity* currentEnt;
	SJson* file, * layers, * array, *array2, * data, *objects, *object;
	file = sj_object_new();
	layers = sj_object_new();
	object = sj_object_new();
	objects = sj_array_new();
	array = sj_array_new();
	array2 = sj_array_new();
	
	data = sj_new_int(24);
	sj_object_insert(file, "height", data);

	data = sj_new_int(42);
	sj_object_insert(file, "width", data);

	data = sj_new_int(32);
	sj_object_insert(file, "tileheight", data);

	data = sj_new_int(32);
	sj_object_insert(file, "tilewidth", data);
	
	data = sj_new_str("../../images/perlin-grey.png");
	sj_object_insert(file, "tilesheet", data);

	data = sj_new_int(1);
	sj_object_insert(file, "tileimages", data);

	data = sj_new_str("../../images/backgrounds/bg_flat.png");
	sj_object_insert(file, "background", data);

	for (int i = 0; i < level->height; i++) {
		for (int j = 0; j < level->width; j++) {
			data = sj_new_int(level->tiles[i][j]);
			sj_array_append(array, data);
		}
	}

	for (int i = 0; i < getMaxEnts(); i++) {
		currentEnt = Find(i);
		if (currentEnt->_inuse) {
			data = sj_new_str(currentEnt->name);
			sj_object_insert(object, "name", data);

			data = sj_new_float(currentEnt->position.x);
			sj_object_insert(object, "x", data);

			data = sj_new_float(currentEnt->position.y);
			sj_object_insert(object, "y", data);

			sj_array_append(objects, object);
		}
	}

	sj_object_insert(layers, "data", array);
	sj_object_insert(file, "layers", layers);
	//sj_object_insert(file, "data", array);
	sj_object_insert(file,"objects",objects);

	sj_save(file, filename);
}