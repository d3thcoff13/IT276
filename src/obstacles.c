#include "obstacles.h"
#include "game.h"

void init_tree(Entity* self) {
	self->sprite = gf2d_sprite_load_image("../../images/tree.png");
	set_hitbox(self,32,32,64,64,0,0);
	self->hitbox.isActive = true;
	self->type = ET_Obstacle;
	self->scale = vector2d(1,1);
	self->think = obstacle_think;
	self->canDamage = false;
	self->scale = vector2d(0.5, 0.5);
	self->obstacleType = OT_Tree;
	self->monType = -1;
}

void init_boulder(Entity* self) {
	self->sprite = gf2d_sprite_load_image("../../images/boulder.png");
	set_hitbox(self, 1, 1, 80, 80, 0, 0);
	self->hitbox.isActive = true;
	self->type = ET_Obstacle;
	self->scale = vector2d(1, 1);
	self->think = obstacle_think;
	self->canDamage = false;
	self->scale = vector2d(0.2, 0.2);
	self->obstacleType = OT_Boulder;
	self->monType = -1;
}

void init_demon_door(Entity* self) {
	set_hitbox(self, 1, 1, 32, 40, 0, 0);
	self->hitbox.isActive = true;
	self->sprite = gf2d_sprite_load_all("../../images/doors.png",32, 40,2);
	self->type = ET_Obstacle;
	self->scale = vector2d(1, 1);
	self->think = obstacle_think;
	self->canDamage = false;
	self->color = vector4d(100, 20, 20, 255);
	self->frame = 0;
	self->obstacleType = OT_DemonDoor;
	self->monType = -1;
}

void init_lightning_door(Entity* self) {
	self->sprite = gf2d_sprite_load_all("../../images/doors.png", 32,40,2);
	set_hitbox(self, 1, 1, 32, 40, 0, 0);
	self->hitbox.isActive = true;
	self->type = ET_Obstacle;
	self->scale = vector2d(1, 1);
	self->think = obstacle_think;
	self->canDamage = false;
	self->color = vector4d(50, 50, 255, 255);
	self->frame = 0;
	self->obstacleType = OT_LightningDoor;
	self->monType = -1;
}


void init_spikes(Entity* self) {
	self->sprite = gf2d_sprite_load_image("../../images/spikes.png");
	set_hitbox(self, 1, 1, 32, 32, 0, 0);
	self->hitbox.isActive = true;
	self->type = ET_Obstacle;
	self->scale = vector2d(1, 1);
	self->think = obstacle_think;
	self->canDamage = true;
	self->damage = player_entity->maxHealth;
	self->scale = vector2d(0.2, 0.2);
	self->obstacleType = OT_Spikes;
	self->monType = -1;
}

void obstacle_think(Entity *self) {
	update_hitbox_position(self);
}
