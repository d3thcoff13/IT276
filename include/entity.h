#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf2d_sprite.h"
#include "gf2d_actor.h"
#include "gf2d_physics.h"
#include "p_weapon.h"
#include <stdbool.h>


typedef enum
{
    ET_Player = 0,
    ET_Projectile = 1,
    ET_Hitbox = 2,
    ET_Enemy = 3,
    ET_Stage = 4,
    ET_Hazard = 5,
    ET_Healthbar = 6,
    ET_Energybar = 7,
    ET_Weapon
}EntityType;

/*typedef struct BoundBox
{
    int x, y;
    int w, h;
    int offsetx, offsety;
    bool isActive;
}BoundBox;*/

typedef struct WeaponList
{
    int* weapon_list;
    int currentWeapon;
}WeaponList;



typedef struct Entity_S
{
    Uint8       _inuse;     /**<check if this entity in memory is active or not*/
    Sprite     *sprite;     /**<a pointer to the sprite that is used by this entity*/
    int         state;
    float       frame;      /**<current frame for the sprite*/
    char   name;

    Vector2D    position;   /**<where the entity is in 2D space*/
    Vector2D    velocity;
    
    Vector2D    drawOffset;
    
    BoundBox		hitbox;

    void (*think)(struct Entity_S *self);   /**<called when an entity draws*/
    void (*touch)(struct Entity_S *self, struct Entity_S *other);   /**<called when an entity touches another entity*/
    
    Vector2D flip;
    Vector2D scale;
    Actor actor;

    float health;
    int maxHealth;
    int attack;
    int cooldown;

    Sprite* wpnsprite;

    bool canDamage;
    int damage;

    EntityType		type;
    bool grounded;
    //Physics     physics;

    bool canDoubleJump;
    bool canAirdash;

    enum Weapons weaponType;
    Weapon *wpn;
}Entity;

void entity_free_all();
/**
 * @brief get a pointer to a new entity
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
Entity *entity_new();

/**
 * @brief initialize the entity resource manager
 * @param maxEnts upper bound of maximum concurrent entities to be supported
 * @note must be called before creating a new entity
 */
void entity_manager_init(Uint32 maxEnts);

/**
 * @brief free a previously allocated entity
 * @param self a pointer to the entity to free
 */
void entity_free(Entity *self);

/**
 * @brief update every active entity
 */
void entity_update_all();
/**
 * @brief draww every active entity
 */
void entity_draw_all();

void entity_set_position(Entity *self, Vector2D position);

void set_hitbox(Entity* self, int x, int y, int w, int h, int offsetx, int offsety);

void update_hitbox_position(Entity* self);

void entity_tile_collision(int** tiles);
#endif
