#include <stdlib.h>
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "entity.h"
#include "collisions.h"
#include "level.h"

typedef struct
{
    Uint32 maxEnts;     /**<max entities supported by the system*/
    Entity *entityList; /**<pointer to an allocated array of entities*/
}EntityManager;

static EntityManager entity_manager = {0};


void entity_collide_check(Entity *entity);

void entity_free_all() {
    int i;
    for (i = 0; i < entity_manager.maxEnts; i++)
    {
        if (entity_manager.entityList[i]._inuse)
        {
            entity_free(&entity_manager.entityList[i]);
        }
    }
    change_level("../../levels/demo2.json");
}

void entity_manager_close()
{
    int i;
    for (i = 0; i < entity_manager.maxEnts;i++)
    {
        if (entity_manager.entityList[i]._inuse)
        {
            entity_free(&entity_manager.entityList[i]);
        }
    }
    entity_manager.maxEnts = 0;
    free(entity_manager.entityList);
    entity_manager.entityList = NULL;
    slog("entity manager closed");
}

void entity_manager_init(Uint32 maxEnts)
{
    if (entity_manager.entityList != NULL)
    {
        //TODO: cleanup
    }
    if (!maxEnts)
    {
        slog("cannot intialize a zero size entity list!");
        return;
    }
    entity_manager.entityList = (Entity *)malloc(sizeof(Entity) * maxEnts);
    if (entity_manager.entityList == NULL)
    {
        slog("failed to allocate %i entities for the entity manager",maxEnts);
        return;
    }
    entity_manager.maxEnts = maxEnts;
    memset(entity_manager.entityList,0,sizeof(Entity)*maxEnts);
    slog("entity manager initialized");
    atexit(entity_manager_close);
}


Entity *entity_new()
{
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (entity_manager.entityList[i]._inuse)continue;
        entity_manager.entityList[i]._inuse = 1;
        slog("entity created");
        return &entity_manager.entityList[i];
    }
    slog("out of open entity slots in memory!");
    return NULL;
}


void entity_free(Entity *self)
{
    if (!self)return;
    gf2d_sprite_free(self->sprite);
    memset(self,0,sizeof(Entity));
}

void entity_update(Entity *self)
{
    if (!self)return;
    
    vector2d_add(self->position,self->position,self->velocity);
    entity_tile_collision(get_level()->tiles);
    //entity_collide_check(self);
}

void entity_update_all()
{
    collision_check_all(entity_manager.entityList, entity_manager.maxEnts);
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        if (entity_manager.entityList[i].think)
        {
            entity_manager.entityList[i].think(&entity_manager.entityList[i]);
        }
        entity_update(&entity_manager.entityList[i]);
    }
}


void entity_draw(Entity *self)
{
    Sprite* sprite = NULL;
    Uint32 frame;
    SDL_Rect rect, rect2;
    if (self == NULL)
    {
        slog("cannot draw sprite, NULL entity provided!");
        return;
    }
    if (self->actor.sprite) {
        sprite = self->actor.sprite;
        frame = (Uint32)self->actor.frame;
    }
    else if (self->sprite) {
        sprite = self->sprite;
        frame = (Uint32)self->frame;
    }
    else slog("no sprite found");

    gf2d_sprite_draw(
        sprite,
        vector2d(self->position.x + self->drawOffset.x,self->position.y + self->drawOffset.y),
        &self->scale,
        NULL,
        NULL,
        &self->flip,
        NULL,
        frame);
    //gf2d_draw_circle(self->position, self->radius, vector4d(255,0,255,255));
    gfc_rect_set(rect,self->hitbox.x + self->hitbox.offsetx,self->hitbox.y + self->hitbox.offsety ,self->hitbox.w,self->hitbox.h);
    if (self->searchbox.isActive) {
        gfc_rect_set(rect2, self->searchbox.x + self->searchbox.offsetx, self->searchbox.y + self->searchbox.offsety, self->searchbox.w, self->searchbox.h);
        gf2d_draw_rect(rect2, vector4d(255, 0, 0, 255));
    }
    gf2d_draw_rect(rect,vector4d(255,0,255,255));
}


void entity_entity_collide(Entity *e1,Entity *e2)
{
    /*if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
    {
        if (e1->touch)
        {
            e1->touch(e1,e2);
        }
    }*/
}

void entity_collide_check(Entity *entity)
{
    int i;
    if (!entity)return;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        if (&entity_manager.entityList[i] == entity)continue;
        entity_entity_collide(entity,&entity_manager.entityList[i]);
    }
}

void entity_draw_all()
{
    int i;
    for (i = 0;i < entity_manager.maxEnts;i++)
    {
        if (!entity_manager.entityList[i]._inuse)continue;
        entity_draw(&entity_manager.entityList[i]);
    }
}

void entity_set_position(Entity *self, Vector2D position){
	self->position = position;
}

void set_hitbox(Entity* self, int x, int y, int w, int h, int offsetx, int offsety) {
    self->hitbox.x = x;
    self->hitbox.y = y;
    self->hitbox.w = w;
    self->hitbox.h = h;
    self->hitbox.offsetx = offsetx;
    self->hitbox.offsety = offsety;
}

void update_hitbox_position(Entity* self) {
    self->hitbox.x = self->position.x;
    self->hitbox.y = self->position.y;
    if (self->type == ET_Weapon && self->flip.x == 1) {
        self->hitbox.offsetx = 16 - self->hitbox.w;
    }
    else if (self->type == ET_Weapon && self->flip.x == 0) {
        if (self->weaponType == Shortsword || self->weaponType == Dagger)
            self->hitbox.offsetx = 40;
        else self->hitbox.offsetx = 48;
    }
}

void set_searchbox(Entity* self, int x, int y, int w, int h, int offsetx, int offsety) {
    self->searchbox.x = x;
    self->searchbox.y = y;
    self->searchbox.w = w;
    self->searchbox.h = h;
    self->searchbox.offsetx = offsetx;
    self->searchbox.offsety = offsety;
}

void update_searchbox_position(Entity* self) {
    self->searchbox.x = self->position.x;
    self->searchbox.y = self->position.y;
}

void entity_tile_collision(int** tiles) {
        for (int i = 0; i < entity_manager.maxEnts; i++) {
            if (entity_manager.entityList[i]._inuse) {
                if (entity_manager.entityList[i].type != ET_Player && entity_manager.entityList[i].type != ET_Enemy)
                    continue;
                check_tile_ahead(&entity_manager.entityList[i], tiles);
            }
        }
    }




