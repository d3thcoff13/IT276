
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
		if (tiles[bottom_tile][i] != 0 && vector2d_magnitude_between(vector2d(0, self->position.y + self->hitbox.offsety + self->hitbox.h - 16), vector2d(0, bottom_tile * 32)) <= 16) {
			self->grounded = true;
			self->position.y = (bottom_tile * 32) - self->hitbox.h - self->hitbox.offsety;
			if (self->type == ET_Player) {
				self->canDoubleJump = true;
				self->canAirDash = true;
				self->canWallJump = true;
			}
			t++;
		}
	}
	if(t==0) self->grounded = false;

	//Check top tile
	for (int i = left_bound; i <= right_bound; i++) {
		if (tiles[upper_tile][i] != 0 && vector2d_magnitude_between(vector2d(0, self->position.y + self->hitbox.offsety), vector2d(0, upper_tile * 32 + 32)) <= 16) {
			self->position.y = upper_tile*32 + 16;
		}
	}


	int upper_bound = ((self->position.y + self->hitbox.offsety) / 32);
	int lower_bound = ((self->position.y + self->hitbox.offsety + self->hitbox.h) / 32);
	int left_tile = ((self->position.x + self->hitbox.offsetx) / 32) - 1;
	int right_tile = ((self->position.x + self->hitbox.w + self->hitbox.offsetx) / 32) + 1;
	int l = 0;
	int r = 0;

	//Check left tile
	for (int i = upper_bound; i <= lower_bound; i++) {
		if (self->grounded && i == lower_bound)
			continue;
		if (tiles[i][left_tile] != 0 && vector2d_magnitude_between(vector2d(self->position.x + self->hitbox.offsetx, 0), vector2d(left_tile * 32 + 32, 0)) <= 1) {
			self->position.x = left_tile * 32 + 34;
			self->leftWall = true;
			self->attack_cooldown = 2;
			self->wall = 1;
			l++;
		}
	}
	if (l == 0) {
		self->leftWall - false;
	}


	//Check right tile

	for (int i = upper_bound; i <= lower_bound; i++) {
		if (self->grounded && i == lower_bound)
			continue;
		if (tiles[i][right_tile] != 0 && vector2d_magnitude_between(vector2d(self->position.x + self->hitbox.w + self->hitbox.offsetx, 0), vector2d(right_tile * 32, 0)) <= 2) {
			self->position.x = right_tile * 32 - self->hitbox.w - 4;
			self->rightWall = true;
			self->attack_cooldown = 2;
			//slog("right.");
			self->wall = -1;
			r++;

		}

	}
	if (r == 0) {
		self->rightWall = false;
	}

}

void collision_check_all(Entity* ents, Uint32 entity_max) {
	for (int i = 0; i < entity_max - 1; i++) {
		if (ents[i]._inuse != 1 || ents[i].hitbox.isActive == 0)continue;
		if (ents[i].type != ET_Player && ents[i].type != ET_Weapon && ents[i].type != ET_Projectile) continue;
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

bool search_check(Entity* self, Entity* other) {
	return(
		self->position.x + self->searchbox.w + self->searchbox.offsetx > other->position.x + other->hitbox.offsetx &&
		self->position.x + self->searchbox.offsetx  < other->position.x + other->hitbox.offsetx + other->hitbox.w &&
		self->position.y + self->searchbox.h + self->searchbox.offsety > other->position.y + other->hitbox.offsety &&
		self->position.y + self->searchbox.offsety < other->position.y + other->hitbox.offsety + other->hitbox.h);
}
