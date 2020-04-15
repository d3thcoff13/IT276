#include "m_wolf.h"
#include "game.h"
#include "collisions.h"
#include "simple_logger.h"

#define MS_IDLE     1
#define MS_SEARCH   2
#define MS_WALK     3
#define MS_ATTACK   4
#define MS_HURT     5
#define MS_DEAD     6

void wolf_think(Entity* self) {
    if (!(self->grounded) && self->state == MS_ATTACK)vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.1);
    else if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    update_searchbox_position(self);
    self->attack_cooldown -= 0.1;
    if (self->attack_cooldown <= 0) self->attack_cooldown = 0;
    if (self->state == MS_IDLE) {
        if (search_check(self, player_entity)) {
            self->state = MS_WALK;
            gf2d_actor_set_action(&self->actor, "run");
            self->aggro = true;
        }
    }
    if (self->state == MS_DEAD) {
        entity_free(self);
    }
    if (self->aggro == true && self->state == MS_ATTACK) {
        if (self->frame <= 38 && self->grounded == false) {
            self->frame = 38;
        }
        if (self->grounded == true) {
            self->state = MS_WALK;
            gf2d_actor_set_action(&self->actor,"run");
            self->attack_cooldown =  16;
        }
    }
    if (self->state == MS_HURT) {
        self->attack_cooldown = gf2d_actor_get_frames_remaining(&self->actor);
        if (self->cooldown <= 0)self->state = MS_IDLE;
        slog("whoops.");
    }
    if (self->state == MS_WALK && self->aggro == true) {
        if (self->attack_cooldown == 0) {
            self->velocity.x = (self->position.x > player_entity->position.x ? -1 : 1);
            self->flip.x = (self->position.x > player_entity->position.x ? 1 : 0);
            if (vector2d_magnitude_between(vector2d(self->position.x, 0), vector2d(player_entity->position.x, 0)) < 64) {
                self->state = MS_ATTACK;
                gf2d_actor_set_action(&self->actor, "attack");
                self->velocity = vector2d((self->position.x > player_entity->position.x ? -1 : 1) * 3, -3);
                self->grounded = false;
                self->hitbox.isActive = true;
            }
        }
        else {
            self->velocity.x = (self->position.x > player_entity->position.x ? 1 : -1);
            self->flip.x = (self->position.x > player_entity->position.x ? 0 : 1);
        }
    }
}

void wolf_touch(Entity* self, Entity* other) { return; }

Entity* wolf_spawn(Entity* self) {
    if (!self)return NULL;


    gf2d_actor_load(&self->actor, "../../actors/wolf.actor");
    gf2d_actor_set_action(&self->actor, "idle");

    self->think = wolf_think;
    self->touch = wolf_touch;

    self->owner = self;
    self->type = ET_Enemy;

    vector2d_set(self->scale, 1, 1);
    vector2d_set(self->drawOffset, 0, -4);
    vector2d_set(self->velocity, 0, 0);
    self->flip = vector2d(1, 0);

    self->health = 100;
    self->grounded = true;
    self->canDamage = true;
    self->damage = 10;
    self->state = MS_IDLE;

    set_hitbox(self, self->position.x, self->position.y, 64, 32, 0, 0);
    self->hitbox.isActive = true;
    self->aggro = false;
    set_searchbox(self, self->position.x, self->position.y, 200, 64, -70, -16);
    self->searchbox.isActive = true;
    self->attack_cooldown = 0;
    self->monType = MT_Wolf;


    return self;
}