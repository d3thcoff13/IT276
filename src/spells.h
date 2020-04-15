#include "entity.h"
void cast_lightning(Entity* caster);
void cast_stoneskin(Entity* caster);
void bomb_toss(Entity* caster);
void throw_holy_water(Entity* caster);

void lightning_think(Entity* self);
void bomb_think(Entity* self);
void holy_water_think(Entity* self);

void lightning_touch(Entity* self);
void bomb_touch(Entity* self);
void holy_water_touch(Entity* self);


