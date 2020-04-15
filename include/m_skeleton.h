#include "entity.h"
Entity* skeleton_spawn(Entity* self);
void skeleton_touch(Entity* self, Entity* other);
void summon_skeleton(Entity* owner);
void skeleton_think(Entity* self);