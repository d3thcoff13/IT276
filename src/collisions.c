
#include "collisions.h"
#include "simple_logger.h"

void check_tile_ahead(Entity* self, int** tiles) {

	//Check bottom tile
	int left_bound = ((self->position.x + self->hitbox.offsetx) / 32);
	int right_bound = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 32);
	int bottom_tile = ((self->position.y + self->hitbox.offsety + self->hitbox.h) / 32);
	int upper_tile = ((self->position.y + self->hitbox.offsety) / 32);
	int t = 0;
	for (int i = left_bound; i <= right_bound; i++) {
		if (tiles[bottom_tile][i] != 0 && vector2d_magnitude_between(vector2d(0, self->position.y + self->hitbox.offsety + self->hitbox.h - 16), vector2d(0, bottom_tile * 32)) <= 1.5) {
			self->grounded = true;
			t++;
		}
		else self->grounded = false;
	}
}

void collision_check_all(Entity* ents, Uint32 entity_max) {
	for (int i = 0; i < entity_max - 1; i++) {
		if (ents[i]._inuse != 1 || ents[i].hitbox.isActive == 0)continue;
		if (ents[i].type != ET_Player && ents[i].type != ET_Weapon) continue;
		for (int j = 1; j < entity_max; j++) {
			if(ents[j]._inuse != 1 || ents[j].hitbox.isActive == 0)continue;
			if (ents[i].owner == ents[j].owner || i == j) continue;
				if (collision_check(&ents[i], &ents[j])) {
					//slog("collision");
					if (&ents[i].touch && &ents[i].hitbox.isActive)ents[i].touch(&ents[i], &ents[j]);
				}
		}
	}
}

bool collision_check(Entity* self, Entity* other) {
	return(
		self->position.x + self->hitbox.w + self->hitbox.offsetx > other->position.x + other->hitbox.offsetx &&
		self->position.x + self->hitbox.offsetx  < other->position.x + other->hitbox.offsetx + other->hitbox.w &&
		self->position.y + self->hitbox.h + self->hitbox.offsety > other->position.y + other->hitbox.offsety &&
		self->position.y + self->hitbox.offsety < other->position.y + other->hitbox.offsety + other->hitbox.h);
}
