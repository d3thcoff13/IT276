#include "m_Necromancer.h"

void monster_think(Entity* self) {
    update_hitbox_position(self);
}

void monster_touch(Entity* self, Entity* other) { return; }

Entity* monster_spawn(Entity* self) {
    if (!self)return NULL;


    gf2d_actor_load(&self->actor, "../../actors/skeleton.actor");
    gf2d_actor_set_action(&self->actor, "idle");

    self->think = monster_think;
    self->touch = monster_touch;

    self->owner = self;
    self->type = ET_Enemy;

    vector2d_set(self->scale, 1,1);
    vector2d_set(self->drawOffset, 0, -4);
    vector2d_set(self->velocity, 0, 0);
    self->flip = vector2d(1, 0);

    self->health = 100;
    self->grounded = true;
    self->canDamage = true;
    self->damage = 10;

    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    self->hitbox.isActive = true;

    return self;
}