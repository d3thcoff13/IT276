#include "m_Necromancer.h"

void monster_think(Entity* self) {
    update_hitbox_position(self);
}
Entity* monster_spawn(Entity* self) {
    if (!self)return NULL;
    self->radius = 5;
    self->size.x = 5;
    self->size.y = 5;
    gf2d_actor_load(&self->actor, "../../actors/skeleton.actor");
    gf2d_actor_set_action(&self->actor, "idle");
    self->think = monster_think;
    vector2d_set(self->scale, 1,1);
    vector2d_set(self->drawOffset, 0, -4);
    self->flip = vector2d(1, 0);
    self->grounded = true;
    vector2d_set(self->velocity, 0, 0);
    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    return self;
}