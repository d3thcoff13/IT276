#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "gf2d_sprite.h"
#include "gf2d_actor.h"

#include <stdbool.h>

typedef enum {
    Dagger,
    Shortsword,
    Spear,
    Rapier,
    Mace
}Weapons;

typedef struct BoundBox
{
    int x, y;
    int w, h;
    int offsetx, offsety;
    bool isActive;
}BoundBox;

typedef struct Weapon_S{
    Sprite* sprite;
    float frame;
    int damage;
    Vector2D position;
    BoundBox hitbox;
    Actor wpnActor;
    enum Weapons weaponType;
    bool active;
    Uint8 _inuse;
    Vector2D flip;
    int state;
    Vector2D scale;
    Vector4D color;
    void (*think)(struct Weapon_S* wpn);
}Weapon;

void init_weapon(Weapon* wpn, enum WeaponType type);
Weapon* weapon_new();
void load_weapon(Weapon* wepaonEnt, enum Weapons wpn);
void weapon_think(Weapon* self, int state);
void weapon_touch(Weapon* self);
void weapon_manager_close();
void weapon_manager_init(Uint32 maxWeapons);
void weapon_free(Weapon* self);
void weapon_free_all();
void weapon_draw(Weapon* wpn);


#endif