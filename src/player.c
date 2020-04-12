#include "player.h"
#include "simple_logger.h"

#define ES_IDLE 1
#define ES_RUN 2
#define ES_CROUCH 3
#define ES_JUMP 4
#define ES_ATTACK 5

void player_think(Entity *self){
    const Uint8 *buttons;
    if (!self)return;
    if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    //entity_tile_collision(tiles);
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    switch (self->state)
    {
        case ES_RUN:
            vector2d_equal(self->flip,vector2d(1,self->flip.y))?vector2d_set(self->velocity,-5,self->velocity.y):vector2d_set(self->velocity,5,self->velocity.y);
            break;
        case ES_ATTACK:
            self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
            if(self->cooldown <= 0 && self->attack == 1){
                self->state = ES_IDLE;
                self->cooldown = 0;
            }
            break;
        case ES_CROUCH:
            if(self->actor.frame > 14) self->actor.frame = 14;
            vector2d_set(self->velocity,0,self->velocity.y);
            break;
        case ES_JUMP:
            if (self->actor.frame > 32) self->actor.frame = 32;
            if (self->grounded)self->state = ES_IDLE;
            break;
        default:
            vector2d_set(self->velocity,0,0);
            gf2d_actor_set_action(&self->actor,"idle");
            break;
    }
    
    buttons = SDL_GetKeyboardState(NULL);
    if (buttons[SDL_SCANCODE_J]){
            if(self->state != ES_ATTACK){
                self->state = ES_ATTACK;
                gf2d_actor_set_action(&self->actor,"attack_vert_stand");
                vector2d_set(self->velocity, 0, 0);
                self->attack = 1;
            }
    }
    else if (buttons[SDL_SCANCODE_W]) {
        if (self->state != ES_JUMP) {
            self->state = ES_JUMP;
            self->grounded = false;
            vector2d_set(self->velocity, self->velocity.x, self->velocity.y - 7);
            gf2d_actor_set_action(&self->actor, "jump");
        }
    }else if (buttons[SDL_SCANCODE_D])
        {
            if(self->state != ES_RUN){
                self->state = ES_RUN;
                gf2d_actor_set_action(&self->actor,"walk");
            }
            if(self->flip.x != 0)
            vector2d_set(self->flip, 0, 0);
    }else if (buttons[SDL_SCANCODE_A]){
            if(self->state != ES_RUN){
                self->state = ES_RUN;
                gf2d_actor_set_action(&self->actor,"walk");
            }
            if (self->flip.x != 1)
                vector2d_set(self->flip, 1, 0);
    }else if (buttons[SDL_SCANCODE_S]){
            if(self->state != ES_CROUCH){
                self->state = ES_CROUCH;
                gf2d_actor_set_action(&self->actor,"crouch");
            }
      }else{
            if(self->grounded)self->state = ES_IDLE;
        }
}

void player_touch(Entity *self, Entity *other){}

Entity *player_spawn(Entity *self){
    if (!self)return NULL;
    self->radius = 5;
    self->size.x = 5;
    self->size.y = 5;
    self->think = player_think;
    self->touch = player_touch;
    gf2d_actor_load(&self->actor,"../../actors/player.actor");
    gf2d_actor_set_action(&self->actor,"idle");
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->drawOffset,0,0);
    self->flip = vector2d(0,0);
    self->grounded = true;
    self->state = ES_IDLE;
    vector2d_set(self->velocity, 0, 0);
    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    return self;
}
