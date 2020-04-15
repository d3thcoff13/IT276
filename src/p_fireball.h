#include "entity.h"
Entity* fireball_init(Entity* self, Entity *owner);
void fireball_touch(Entity* self, Entity* other);
void fireball_think(Entity* self);
void cast_fireball(Entity* owner);

