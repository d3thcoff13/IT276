#include "gf2d_sprite.h"
#include "entity.h"

typedef struct menu_s {
	Entity* cursor;
	Entity** menu_items;
	int select;
	int active;
}menu;
