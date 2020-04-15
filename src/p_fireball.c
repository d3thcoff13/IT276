#include "p_fireball.h"
#include "simple_logger.h"

#define ES_FLY 1
#define ES_DEAD 2

Entity* fireball_init(Entity* self, Entity *owner) {
	self->owner = owner;
	self->position = vector2d( owner->position.x - 30, owner->position.y + 24);
	self->velocity.x = self->owner->flip.x == 1? -2 : 2;
	self->flip.x = self->owner->flip.x == 1? 0: 1;
	self->type = ET_Projectile;
	self->canDamage = true;
	self->damage = 50;
	self->think = fireball_think;
	self->touch = fireball_touch;
	gf2d_actor_load(&self->actor, "../../actors/fireball.actor");
	gf2d_actor_set_action(&self->actor, "fly");
	vector2d_copy(self->scale, self->actor.al->scale);
	set_hitbox(self, self->position.x, self->position.y, 32, 16, 0, 0);
	self->hitbox.isActive = true;
	self->state = ES_FLY;
}

void fireball_think(Entity* self) {
	gf2d_actor_next_frame(&self->actor);
	update_hitbox_position(self);
	if (self->state == ES_DEAD) {
		entity_free(self);
	}
}
void fireball_touch(Entity* self, Entity* other) {
	slog("...");
	if (other->type == ET_Enemy) {
		other->health -= self->damage;
		other->position.x += (self->owner->position.x > other->position.x ? -1 : 1) * 20;
		gf2d_actor_set_action(&other->actor, "hurt");
		other->state = 5;
		vector2d_set(self->velocity, (self->flip.x == 0 ? -4 : 4), self->velocity.y);
		slog("health now %f", self->health);
		if (other->health <= 0)other->state = 6;
	}
	else if (other->obstacleType == OT_Tree)entity_free(other);
	self->state = ES_DEAD;
	//entity_free(self);
}
void cast_fireball(Entity* owner) {
	Entity* self = entity_new();
	fireball_init(self, owner);
}