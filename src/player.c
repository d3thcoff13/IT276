#include "player.h"
#include "simple_logger.h"

#define ES_IDLE 1
#define ES_RUN 2
#define ES_ATTACK 3
#define ES_TELEPORT 4


void player_think(Entity *self){
    const Uint8 *buttons;
    if (!self)return;
    self->frame += 0.1;
    switch (self->state)
    {
        
        case ES_RUN:
            self->sprite = gf2d_sprite_load_all("images/knight_run.png",800,480,1);
            vector2d_set(self->drawOffset,0,40);
            vector2d_equal(self->flip,vector2d(1,self->flip.y))?vector2d_set(self->velocity,-5,0):vector2d_set(self->velocity,5,0);
            if(self->frame >= 8)self->frame=0;
            break;
        case ES_ATTACK:
            self->sprite = gf2d_sprite_load_all("images/knight_attack.png",800,560,1);
            if(self->frame >= 13)self->frame=0;
            break;
        default:
            vector2d_set(self->drawOffset,0,0);
            self->sprite = gf2d_sprite_load_all("images/knight_idle.png",800,560,1);
            vector2d_set(self->velocity,0,0);
            if(self->frame >= 2)self->frame=0;
    }
    
    buttons = SDL_GetKeyboardState(NULL);
    if (buttons[SDL_SCANCODE_D])
        {
            self->state = ES_RUN;
            vector2d_set(self->flip,0,0);
        }else if (buttons[SDL_SCANCODE_A]){
            self->state = ES_RUN;
            vector2d_set(self->flip,1,0);
        }else if (buttons[SDL_SCANCODE_J]){
            self->state = ES_ATTACK;
        }else{
            self->state = ES_IDLE;
        }
}

void player_touch(Entity *self, Entity *other){}

Entity *player_spawn(Vector2D position, Vector2D velocity){
    Entity *self;
    self = entity_new();
    if (!self)return NULL;
    self->sprite = gf2d_sprite_load_all(
        "images/knight_idle.png",
        800,
        560,
        1);
    self->radius = 5;
    self->size.x = 5;
    self->size.y = 5;
    self->think = player_think;
    self->touch = player_touch;
    vector2d_copy(self->position,position);
    vector2d_copy(self->velocity,velocity);
    vector2d_set(self->drawOffset,0,0);
    self->flip = vector2d(0,0);
    self->scale = vector2d(0.5,0.5);
    return self;
}
