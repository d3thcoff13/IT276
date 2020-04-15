#include "m_pyromancer.h"
#include "collisions.h"
#include "game.h"
#include "simple_logger.h"
#include "p_fireball.h"


#define MS_IDLE     1
#define MS_SEARCH   2
#define MS_WALK     3
#define MS_ATTACK   4
#define MS_HURT     5
#define MS_DEAD     6

void pyro_think(Entity* self) {
    if (!self)return;
    if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    self->attack_cooldown -= 0.1;
    if (self->attack_cooldown <= 0) self->attack_cooldown = 0;
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    update_searchbox_position(self);
    if (search_check(self, player_entity) && self->aggro == false) {
        self->aggro = true;
        self->attack_cooldown = 15;
        gf2d_actor_set_action(&self->actor, "cast_fireball");
    }
    if (self->aggro == true && self->attack_cooldown > 3.0) {
        if (self->actor.frame >= 30)self->actor.frame = 30;
    }else {
        if (self->aggro && self->summonCount < 1 && (int)self->attack_cooldown == 0) {
            cast_fireball(self);
            self->attack_cooldown = 15;
            self->summonCount++;
            gf2d_actor_set_action(&self->actor, "idle");
        }
    }
    if (self->state == MS_HURT) {
        self->attack_cooldown = gf2d_actor_get_frames_remaining(&self->actor);
        if (self->cooldown <= 0) {
            self->state = MS_ATTACK;
            gf2d_actor_set_action(&self->actor, "cast_fireball");
        }
    }
    if (self->state == MS_DEAD) {
        entity_free(self);
    }
    
}

void pyro_touch(Entity* self, Entity* other) { return; }

Entity* pyro_spawn(Entity* self) {
    if (!self)return NULL;


    gf2d_actor_load(&self->actor, "../../actors/pyromancer.actor");
    gf2d_actor_set_action(&self->actor, "idle");

    self->think = pyro_think;
    self->touch = pyro_touch;

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

    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    self->hitbox.isActive = true;
    self->aggro = false;
    set_searchbox(self, self->position.x, self->position.y, 200, 64, -84, 0);
    self->searchbox.isActive = true;

    self->summonCount = 0;
    self->attack_cooldown = 0;
    self->monType = MT_Pyro;


    return self;
}

