#include "m_skeleton.h"
#include "game.h"

#define MS_IDLE     1
#define MS_SEARCH   2
#define MS_WALK     3
#define MS_ATTACK   4
#define MS_HURT     5
#define MS_DEAD     6

void skeleton_think(Entity* self) {
    if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    self->attack_cooldown -= 0.1;
    if (self->attack_cooldown <= 0) self->attack_cooldown = 0;
    if (self->state == MS_IDLE) {
        self->velocity.x = 0;
        if (self->attack_cooldown == 0 && self->aggro == true) {
            self->state = MS_ATTACK;
            gf2d_actor_set_action(&self->actor, "walk");
        }
    }
    if (self->state == MS_DEAD) {
        if(self->owner)self->owner->summonCount--;
        entity_free(self);
    }
    if (self->aggro == true && self->state == MS_ATTACK) {
        self->velocity.x = (self->position.x > player_entity->position.x ? -1 : 1);
        self->flip.x = (self->position.x > player_entity->position.x ? 0 : 1);
    }
    if (self->state == MS_HURT) {
        self->attack_cooldown = gf2d_actor_get_frames_remaining(&self->actor);
        if (self->cooldown <= 0)self->state = MS_IDLE;
    }
}

void skeleton_touch(Entity* self, Entity* other) { return; }

Entity* skeleton_spawn(Entity* self) {
    if (!self)return NULL;


    gf2d_actor_load(&self->actor, "../../actors/skeleton.actor");
    gf2d_actor_set_action(&self->actor, "idle");

    self->think = skeleton_think;
    self->touch = skeleton_touch;

    self->owner = self;
    self->type = ET_Enemy;

    self->state = MS_IDLE;


    self->attack_cooldown = 0;

    vector2d_set(self->scale, 1, 1);
    vector2d_set(self->drawOffset, 0, -4);
    vector2d_set(self->velocity, 0, 0);
    self->flip = vector2d(1, 0);

    self->health = 100;
    self->grounded = true;
    self->canDamage = true;
    self->damage = 10;

    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    self->hitbox.isActive = true;
    self->monType = MT_Skeleton;

    return self;
}

void summon_skeleton(Entity* owner) {
    Entity *self = entity_new();
    skeleton_spawn(self);
    self->owner = owner;
    self->aggro = true;
    self->state = MS_IDLE;
    self->attack_cooldown = 0;
    vector2d_set(self->position, self->owner->position.x + 50 , self->owner->position.y);
}