#include "m_Necromancer.h"
#include "collisions.h"
#include "game.h"
#include "simple_logger.h"
#include "m_skeleton.h"


#define MS_IDLE     1
#define MS_SEARCH   2
#define MS_WALK     3
#define MS_ATTACK   4
#define MS_HURT     5
#define MS_DEAD     6

void necro_think(Entity* self) {
    if (!self)return;
   //if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    //else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    self->attack_cooldown -= 0.1;
    if(self->summonCount < 2 && self->attack_cooldown >= 0)slog("%f", self->attack_cooldown);
    if (self->attack_cooldown <= 0) self->attack_cooldown = 0;
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    update_searchbox_position(self);
    if (search_check(self, player_entity) && self->aggro == false)self->aggro = true;
    if (self->aggro && self->summonCount < 2 && (int)self->attack_cooldown == 0) {
        summon_skeleton(self);
        self->attack_cooldown = 16;
        self->summonCount++;
    }
    if (self->state == MS_DEAD) {
        entity_free(self);
    }
    //slog("%i", self->skeletonCount);
}

void necro_touch(Entity* self, Entity* other) { return; }

Entity* necro_spawn(Entity* self) {
    if (!self)return NULL;


    gf2d_actor_load(&self->actor, "../../actors/necromancer.actor");
    gf2d_actor_set_action(&self->actor, "idle");

    self->think = necro_think;
    self->touch = necro_touch;

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
    self->aggro = false;
    set_searchbox(self, self->position.x, self->position.y, 200, 64, -84, 0);
    self->searchbox.isActive = true;

    self->summonCount = 0;
    self->attack_cooldown = 0;
    self->monType = MT_Necro;


    return self;
}