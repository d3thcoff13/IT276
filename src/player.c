#include "player.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "p_weapon.h"

#define ES_IDLE 1
#define ES_RUN 2
#define ES_WALK 3
#define ES_CROUCH 4
#define ES_JUMP 5
#define ES_ATTACK 6
#define ES_HURT 7

void player_think(Entity *self){
    const Uint8 *buttons;
    if (!self)return;
    if (!(self->grounded))vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.5);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    gf2d_actor_next_frame(&self->actor);
    update_hitbox_position(self);
    switch (self->state)
    {
        case ES_WALK:
            vector2d_equal(self->flip, vector2d(1, self->flip.y)) ? vector2d_set(self->velocity, -2, self->velocity.y) : vector2d_set(self->velocity, 2, self->velocity.y);
            break;
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
        case ES_HURT:
            self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
            if (self->cooldown <= 0) {
                self->state = ES_IDLE;
                self->cooldown = 0;
                self->hitbox.isActive = true;
            }
            break;
        default:
            vector2d_set(self->velocity,0,0);
            gf2d_actor_set_action(&self->actor,"idle");
            break;
    }
    if (self->cooldown == 0)getPlayerInputs(self);
    //self->wpn->frame += 0.1;
    Vector4D color = vector4d(255, 255, 255, 255);
    gf2d_sprite_draw(self->wpnsprite, vector2d(self->position.x, self->position.y -10), NULL, NULL, NULL, NULL, &color, 1);
    //self->wpn->position = self->position;
    weapon_think(self->wpn, (self->state == ES_ATTACK? 2: 1));
    if(self->position.y >= 500)entity_free_all();
}

void player_touch(Entity *self, Entity *other){
    if (other->canDamage) {
        self->state = ES_HURT;
        self->hitbox.isActive = false;
        self->health -= other->damage;
        self->position.x -= 20;
        gf2d_actor_set_action(&self->actor, "stand_hurt");
        vector2d_set(self->velocity, (self->flip.x == 0? -4:4), self->velocity.y);
        slog("health now %f", self->health);
    }
}

Entity *init_player(Entity *self){
    if (!self)return NULL;

    //Functions
    self->think = player_think;
    self->touch = player_touch;

    //Actor info
    gf2d_actor_load(&self->actor,"../../actors/player.actor");
    gf2d_actor_set_action(&self->actor,"idle");

    //Draw info
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->drawOffset,0,0);
    self->flip = vector2d(0,0);
    vector2d_set(self->velocity, 0, 0);

    //State info
    self->grounded = true;
    self->state = ES_IDLE;
    self->canDoubleJump;
    self->canAirdash;

    self->wpn = weapon_new();
    self->weaponType = Dagger;
    init_weapon(self->wpn, self->weaponType);
    //self->wpn->position = self->position;
    self->wpn->sprite = gf2d_sprite_load_all("../../images/spritesheets/weapons/player-dagger.png", 16, 16, 4);
    
    //Create Hitbox
    set_hitbox(self, self->position.x, self->position.y, 32, 58, 0, 16);
    self->hitbox.isActive = true;

    //Stats
    self->maxHealth = 100;
    self->health = 100;//self->maxHealth;
    slog("health before %f", self->health);
    loadPlayerData(self);
    return self;
}

void getPlayerInputs(Entity *self) {
    const Uint8* buttons;

    buttons = SDL_GetKeyboardState(NULL);
    if (self->grounded) {
        if (buttons[SDL_SCANCODE_J]) {
            if (self->state != ES_ATTACK) {
                self->state = ES_ATTACK;
                gf2d_actor_set_action(&self->actor, "attack_vert_stand");
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
            if (self->state == ES_JUMP || self->grounded == false) {
                self->canDoubleJump == false;
            }
        }
        else if (buttons[SDL_SCANCODE_D])
        {
            if (buttons[SDL_SCANCODE_SPACE])
            {
                if (self->state != ES_RUN) {
                    self->state = ES_RUN;
                    gf2d_actor_set_action(&self->actor, "walk");
                }
            }
            else if (self->state != ES_WALK) {
                self->state = ES_WALK;
                gf2d_actor_set_action(&self->actor, "walk");
            }
            if (self->flip.x != 0)
                vector2d_set(self->flip, 0, 0);
        }
        else if (buttons[SDL_SCANCODE_A]) {
            if (buttons[SDL_SCANCODE_SPACE])
            {
                if (self->state != ES_RUN) {
                    self->state = ES_RUN;
                    gf2d_actor_set_action(&self->actor, "walk");
                }
            }
            else if (self->state != ES_WALK) {
                self->state = ES_WALK;
                gf2d_actor_set_action(&self->actor, "walk");
            }
            if (self->flip.x != 1)
                vector2d_set(self->flip, 1, 0);
        }
        else if (buttons[SDL_SCANCODE_S]) {
            if (self->state != ES_CROUCH) {
                self->state = ES_CROUCH;
                gf2d_actor_set_action(&self->actor, "crouch");
            }
        }
        else if (buttons[SDL_SCANCODE_C]) {
            savePlayerData(self);
            slog("Player Data saved.");
        }
        /*else if (buttons[SDL_SCANCODE_T]) {
            slog("%i", SDL_GetTicks());
        }*/
        else {
            if (self->grounded)self->state = ES_IDLE;
        }
    }
    else if (self->grounded == false) {
        if (buttons[SDL_SCANCODE_W]) {
            if (self->state != ES_JUMP) {
                self->state = ES_JUMP;
                self->grounded = false;
                vector2d_set(self->velocity, self->velocity.x, self->velocity.y - 7);
                gf2d_actor_set_action(&self->actor, "jump");
            }
            if (self->state == ES_JUMP || self->grounded == false) {
                self->canDoubleJump == false;
                vector2d_set(self->velocity, self->velocity.x, self->velocity.y - 7);
                gf2d_actor_set_action(&self->actor, "jump");
                slog("double jumped");
            }
        }
    }

}

void savePlayerData(Entity* self) {
    SJson* file, * array, * data;
    file = sj_object_new();
    array = sj_array_new();

    data = sj_new_int(1);
    sj_array_append(array, data);
    sj_object_insert(file, "LoadFromFile", data);

    data = sj_new_int(self->health);
    sj_array_append(array, data);
    sj_object_insert(file, "health", data);

    data = sj_new_int(self->maxHealth);
    sj_array_append(array, data);
    sj_object_insert(file, "maxHealth", data);

    data = sj_new_float(self->position.x);
    sj_array_append(array, data);
    sj_object_insert(file, "pos.x", data);

    data = sj_new_float(self->position.y);
    sj_array_append(array, data);
    sj_object_insert(file, "pos.y", data);

    data = sj_new_float(self->flip.x);
    sj_array_append(array, data);
    sj_object_insert(file, "flip.x", data);

    data = sj_new_float(self->flip.y);
    sj_array_append(array, data);
    sj_object_insert(file, "flip.y", data);

    sj_save(file, "../../savefiles/playerData.json");
}

void loadPlayerData(Entity* self) {
    SJson* value, * file;
    int loadfromfile;
    file = sj_load("../../savefiles/playerData.json");
    if (file == NULL) {
        slog("file not found");
        return NULL;
    }
    loadfromfile = 0;
    value = sj_object_get_value(file, "LoadFromFile");
    sj_get_integer_value(value, &loadfromfile);

    if (loadfromfile == 1) {
        value = sj_object_get_value(file, "health");
        sj_get_float_value(value, &self->health);

        value = sj_object_get_value(file, "maxHealth");
        sj_get_integer_value(value, &self->maxHealth);
    }

    slog("Health: %f", self->health);

}
