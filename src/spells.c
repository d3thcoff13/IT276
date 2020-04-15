#include "spells.h"
#include "simple_logger.h"

void cast_lightning(Entity* caster) {
	Entity *self = entity_new();
	self->owner = caster;
	self->think = lightning_think;
	self->touch = lightning_touch;
	self->type = ET_Projectile;
	self->canDamage = true;
	self->damage = 40;
	self->sprite = gf2d_sprite_load_all("../../images/spritesheets/lightning.png", 260, 360, 8);
	self->frame = 0;
	self->flip.x = self->owner->flip.x == 1 ? 0 : 1;
	self->position = vector2d(self->owner->position.x + 30, self->owner->position.y-10);
	set_hitbox(self, 0, 0, 52, 72, 0, 0);
	self->hitbox.isActive = false;
	self->scale = vector2d(0.2, 0.2);
	self->frameCount = 0;
}
void cast_stoneskin(Entity* caster) {
	caster->stoneskin = true;
	caster->stoneskinTimer = 10;
	caster->color = vector4d(0, 255, 0, 255);
	slog("stoneskin activated");
}
void bomb_toss(Entity* caster) {
	Entity* self = entity_new();
	self->owner = caster;
	self->think = bomb_think;
	self->touch = bomb_touch;
	self->type = ET_Projectile;
	self->canDamage = true;
	self->damage = 80;
	self->sprite = gf2d_sprite_load_all("../../images/bomb.png", 20, 26, 6);
	self->frame = 0;
	self->flip.x = self->owner->flip.x == 1 ? 0 : 1;
	self->position = vector2d(self->owner->position.x, self->owner->position.y);
	self->velocity = vector2d(1 * (self->owner->flip.x == 1? -1:1), -2);
	set_hitbox(self, 0, 0, 32, 32, 0, 0);
	self->hitbox.isActive = false;
	self->scale = vector2d(1, 1);
	self->frameCount = 0;
}
void throw_holy_water(Entity* caster) {
	Entity* self = entity_new();
	self->owner = caster;
	self->think = holy_water_think;
	self->touch = holy_water_touch;
	self->type = ET_Projectile;
	self->canDamage = true;
	self->grounded = false;
	self->damage = 40;
	self->sprite = gf2d_sprite_load_image("../../images/holywater.png");
	self->frame = 0;
	self->flip.x = self->owner->flip.x == 1 ? 0 : 1;
	self->position = vector2d(self->owner->position.x, self->owner->position.y);
	self->velocity = vector2d(3 * (self->owner->flip.x == 1 ? -1 : 1), -1);
	set_hitbox(self, 0, 0, 16, 16, 0, 0);
	self->hitbox.isActive = true;
	self->scale = vector2d(1, 1);
	self->frameCount = 0;
}



void lightning_think(Entity* self) {
	update_hitbox_position(self);
	if (self->frame == 4) self->hitbox.isActive = true;
	self->frameCount += 0.1;
	self->frame = (int)self->frameCount;
	if (self->frame > 8)entity_free(self);
}
void bomb_think(Entity* self) {
	update_hitbox_position(self);
	if (self->frame == 5) self->hitbox.isActive = true;
	if (self->grounded == false) {
		self->velocity.y += 0.1;
		self->velocity.x += 0.2* (self->owner->flip.x == 1 ? -1 : 1);
	}
	else if (self->grounded == true)self->velocity = vector2d(0, 0);
	self->frameCount += 0.1;
	self->frame = (int)self->frameCount;
	if (self->frame > 5)entity_free(self);
}
void holy_water_think(Entity* self) {
	update_hitbox_position(self);
	if (self->grounded == false) {
		self->velocity.y += 0.1;
		self->velocity.x += 0.2 * (self->owner->flip.x == 1 ? -1 : 1);
	}
	else if (self->grounded == true)entity_free(self);
}



void lightning_touch(Entity* self, Entity* other) {
	if (other->obstacleType == OT_LightningDoor) {
		entity_free(other);
	}
}
void bomb_touch(Entity* self, Entity* other) {
	if (other->obstacleType == OT_Boulder) {
		entity_free(other);
	}
}
void holy_water_touch(Entity* self, Entity* other) {
	if (other->obstacleType == OT_DemonDoor) {
		entity_free(other);
	}
}