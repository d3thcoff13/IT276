
#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "entity.h"

int collide_rect(SDL_Rect r1,SDL_Rect r2);
int collide_circle(Vector2D p1, float r1, Vector2D p2, float r2);

void collision_check_all(Entity* ents, Uint32 entity_max);
bool collision_check(Entity* self, Entity* other);
void check_tile_ahead(Entity* self, int** tiles);


#endif
