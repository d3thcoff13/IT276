#include "entity.h"

void init_weapon(Entity* self, Entity* other, enum WeaponType weaponType);
void weapon_think(Entity* self);
void weapon_touch(Entity* self, Entity* other);