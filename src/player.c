#include "player.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "p_weapon.h"
#include "p_fireball.h"
#include "spells.h"
#include "m_Necromancer.h"
#include "m_skeleton.h"
#include "m_bat.h"
#include "m_wolf.h"
#include "m_pyromancer.h"
#include "obstacles.h"
#include "level.h"
#include "game.h"

#define ES_IDLE 1
#define ES_RUN 2
#define ES_WALK 3
#define ES_CROUCH 4
#define ES_JUMP 5
#define ES_ATTACK 6
#define ES_HURT 7
#define ES_SLIDE 8
#define ES_DASH 9


SDL_Event event;
Entity* entToSpawn;
int currentWeapon;

void player_think(Entity *self){
    const Uint8 *buttons;
    if (!self)return;
    if (!(self->grounded) && self->state != ES_DASH)vector2d_set(self->velocity, self->velocity.x, self->velocity.y + 0.3);
    else if (self->grounded)vector2d_set(self->velocity, self->velocity.x, 0);
    self->attack_cooldown -= 0.1;
    if (self->attack_cooldown <= 0) self->attack_cooldown = 0;
    self->stoneskinTimer -= 0.1;
    if (self->stoneskinTimer <= 0 && self->stoneskin == true) {
        self->stoneskinTimer = 0;
        self->stoneskin = false;
        self->color = vector4d(255, 255, 255, 255);
        slog("stoneskin has faded");
    }
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
            if (!vector3d_equal(self->rotation,vector3d(0,0,0))) {
                self->rotation = vector3d(0, 0, 0);
                set_hitbox(self, self->position.x, self->position.y, 32, 48, 0, 10);
                self->drawOffset.y = 0;
            }

            break;
        case ES_SLIDE:
            if (self->flip.x == 0) {
                self->velocity.x -= 0.1;
                if (self->velocity.x <= 0) {
                    self->state = ES_CROUCH;
                    self->rotation = vector3d(0, 0, 0);
                    set_hitbox(self, self->position.x, self->position.y, 32, 48, 0, 10);
                    self->drawOffset.y = 0;
                }
            } else{
                self->velocity.x += 0.1;
                if (self->velocity.x >= 0) {
                    self->state = ES_CROUCH;
                    self->rotation = vector3d(0, 0, 0);
                    set_hitbox(self, self->position.x, self->position.y, 32, 48, 0, 10);
                    self->drawOffset.y = 0;
                }
            }
            break;
        case ES_DASH:
            if (self->flip.x == 0) {
                self->velocity.x -= 0.1;
                if (self->velocity.x <= 0) {
                    self->state = ES_JUMP;
                }
            }
            else {
                self->velocity.x += 0.1;
                if (self->velocity.x >= 0) {
                    self->state = ES_JUMP;
                }
            }
            break;
        default:
            if (self->state != ES_SLIDE && self->grounded == true) {
                vector2d_set(self->velocity, 0, 0);
                gf2d_actor_set_action(&self->actor, "idle");
            }
                break;
    }
    if (self->cooldown == 0)getPlayerInputs(self);
}

void player_touch(Entity *self, Entity *other){
    if (other->canDamage) {
        if (other->obstacleType != OT_Spikes) {
            self->state = ES_HURT;
            self->hitbox.isActive = false;
            self->health -= other->damage;
            self->position.x += (self->position.x > other->position.x ? -1 : 1) * 20;
            gf2d_actor_set_action(&self->actor, "stand_hurt");
            vector2d_set(self->velocity, (self->position.x > other->position.x ? 4 : -4), self->velocity.y);
            slog("health now %f", self->health);
            if (other->monType == MT_Skeleton) {
                other->state = 1;
                other->attack_cooldown = 3;
            }
            else if (other->monType == MT_Wolf || other->monType == MT_Bat) {
                other->hitbox.isActive = false;
            }
            if (other->type == ET_Projectile) {
                other->owner->summonCount--;
                gf2d_actor_set_action(&other->owner->actor, "cast_fireball");
                other->owner->attack_cooldown = 15;
                entity_free(other);
            }
        }else
        if (other->obstacleType == OT_Spikes && self->stoneskin == false)entity_free(self);
    }
    if (other->type == ET_Obstacle)self->position.x += (self->position.x > other->position.x ? 1 : -1)* other->hitbox.w;
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
    self->grounded = false;
    self->state = ES_IDLE;
    self->canWallJump = true;
    self->canDoubleJump =true;

    currentWeapon = Mace;
    self->owner = self;
    self->weapon = entity_new();
    init_weapon(self->weapon, self, Mace);
    
    //Create Hitbox
    set_hitbox(self, self->position.x, self->position.y, 32, 48, 0, 10);
    self->hitbox.isActive = true;

    //Stats
    self->maxHealth = 100;
    self->health = 100;
    slog("health before %f", self->health);
    loadPlayerData(self);
    return self;
}

void getPlayerInputs(Entity *self) {
    const Uint8* buttons;

    buttons = SDL_GetKeyboardState(NULL);
        if (buttons[SDL_SCANCODE_J]) {
            if (self->state != ES_ATTACK) {
                self->state = ES_ATTACK;
                if (self->weapon->weaponType == Dagger || self->weapon->weaponType == Shortsword)gf2d_actor_set_action(&self->actor, "attack_hori_stand");
                else if (self->weapon->weaponType == Spear || self->weapon->weaponType == Rapier)gf2d_actor_set_action(&self->actor, "attack_stab_stand");
                else gf2d_actor_set_action(&self->actor, "attack_vert_stand");
                vector2d_set(self->velocity, 0, 0);
                self->attack = 1;
            }
        }
        else if (buttons[SDL_SCANCODE_D])
        {
            if (self->grounded == true) {
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
            else {
                self->flip.x = 0;
            }
        }
        else if (buttons[SDL_SCANCODE_A]) {
            if (self->grounded == true) {
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
            else {
                self->flip.x = 1;
            }
        }
        else if (buttons[SDL_SCANCODE_S]) {
            if (self->grounded == true) {
                if (self->state != ES_CROUCH && self->state != ES_SLIDE) {
                    self->state = ES_CROUCH;
                    gf2d_actor_set_action(&self->actor, "crouch");
                }
            }
        }
        else if (buttons[SDL_SCANCODE_C]) {
            savePlayerData(self);
            slog("Player Data saved.");
        }
        else {
            if (self->grounded && self->state != ES_SLIDE)self->state = ES_IDLE;
        }
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_w: {
                if (self->grounded == true) {
                    self->state = ES_JUMP;
                    self->grounded = false;
                    self->velocity.y = -8;
                    gf2d_actor_set_action(&self->actor, "jump");
                }
                else if (self->canWallJump == true && self->attack_cooldown > 0) {
                    self->velocity.x = 5 * self->wall;
                    slog("success");
                    self->velocity.y = -8;
                    self->canWallJump = false;
                    self->actor.frame = 30;
                }
                else {
                    if (self->canDoubleJump && self->grounded == false) {
                        self->velocity.y = -8;
                        self->canDoubleJump = false;
                        self->actor.frame = 30;
                        if (buttons[SDL_SCANCODE_A]) {
                            self->velocity.x = -2;
                        }else if (buttons[SDL_SCANCODE_D]) {
                            self->velocity.x = 2;
                        }
                    }
                    slog("huh.");
                }
            }
            break;
            case SDLK_k:
                if (self->state == ES_CROUCH) {
                    gf2d_actor_set_action(&self->actor, "slide");
                    self->velocity.x = self->flip.x == 1? -6:6;
                    self->state = ES_SLIDE;
                    self->rotation = vector3d(0, 0, 270);
                    self->drawOffset.y = 40;
                    set_hitbox(self, self->position.x, self->position.y, 48, 24, 12, 12);
                }
                if (self->grounded == false && self->canAirDash) {
                    gf2d_actor_set_action(&self->actor, "slide");
                    self->state = ES_DASH;
                    self->velocity.x = self->flip.x == 1 ? -6 : 6;
                    self->velocity.y = 0;
                    self->canAirDash = false;
                }
                break;
            case SDLK_1:
                cast_lightning(self);
                break;
            case SDLK_2:
                cast_stoneskin(self);
                break;
            case SDLK_3:
                cast_fireball(self);
                break;
            case SDLK_4:
                bomb_toss(self);
                break;
            case SDLK_5:
                throw_holy_water(self);
                break;
            case SDLK_q:
                entity_free_all();
                currentLevel++;
                if (currentLevel > 2)currentLevel = 2;
                change_level(currentLevel);
                break;
            case SDLK_6:
                entToSpawn = entity_new();
                necro_spawn(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_7:
                entToSpawn = entity_new();
                pyro_spawn(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_8:
                entToSpawn = entity_new();
                bat_spawn(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y-50);
                break;
            case SDLK_9:
                entToSpawn = entity_new();
                wolf_spawn(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_SEMICOLON:
                entToSpawn = entity_new();
                init_boulder(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_EQUALS:
                entToSpawn = entity_new();
                init_tree(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_LEFTBRACKET:
                entToSpawn = entity_new();
                init_lightning_door(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_RIGHTBRACKET:
                entToSpawn = entity_new();
                init_demon_door(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_MINUS:
                entToSpawn = entity_new();
                init_spikes(entToSpawn);
                entToSpawn->position = vector2d(self->position.x + 200, self->position.y);
                break;
            case SDLK_0:
                if (currentWeapon == Shortsword)currentWeapon = Dagger;
                else currentWeapon++;
                init_weapon(self->weapon, self, currentWeapon);
                break;
            case SDLK_COMMA:
                Save_Game();
                break;
            default:
                break;
            }
        default:
            break;
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
