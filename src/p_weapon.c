#include "p_weapon.h"
#include "simple_logger.h"

#define WS_IDLE 1
#define WS_ATTACK 6

void init_weapon(Entity* self, Entity* owner, enum WeaponType weaponType) {
	self->owner = owner;
	self->type = ET_Weapon;
	self->canDamage = true;
	self->hitbox.isActive = false;
	self->weaponType = weaponType;
	self->state = WS_IDLE;
	self->think = weapon_think;
	self->touch = weapon_touch;
	switch (self->weaponType) {
	case Dagger:
		self->damage = 5;
		gf2d_actor_load(&self->actor, "../../actors/dagger.actor");
		gf2d_actor_set_action(&self->actor, "idle");
		set_hitbox(self, 16, 16, 16, 16, 40, 20);
		break;
	case Rapier:
		self->damage = 10;
		gf2d_actor_load(&self->actor, "../../actors/rapier.actor");
		gf2d_actor_set_action(&self->actor, "idle");
		set_hitbox(self, 16, 16, 16, 16, 48, 20);
		break;
	case Mace:
		self->damage = 30;
		gf2d_actor_load(&self->actor, "../../actors/mace.actor");
		gf2d_actor_set_action(&self->actor, "idle");
		set_hitbox(self, 16, 16, 16, 40, 48, 4);
		break;
	case Spear:
		self->damage = 15;
		gf2d_actor_load(&self->actor, "../../actors/spear.actor");
		gf2d_actor_set_action(&self->actor, "idle");
		set_hitbox(self, 16, 16, 32, 16, 48, 20);
		break;
	case Shortsword:
		self->damage = 20;
		gf2d_actor_load(&self->actor, "../../actors/shortsword.actor");
		gf2d_actor_set_action(&self->actor, "idle");
		set_hitbox(self, 16, 16, 24, 16, 40, 20);
		break;
	}
	vector2d_copy(self->scale, self->actor.al->scale);
}

void weapon_think(Entity* self) {
	self->position = vector2d(self->owner->position.x - 16, self->owner->position.y);
	update_hitbox_position(self);
	self->flip = self->owner->flip;
	gf2d_actor_next_frame(&self->actor);
	if (self->state != self->owner->state) {
		if (self->owner->state == WS_ATTACK)self->state = self->owner->state;
		else self->state = WS_IDLE;
		switch (self->state) {
		case WS_IDLE:
			gf2d_actor_set_action(&self->actor, "idle");
			self->hitbox.isActive = false;
			break;
		case WS_ATTACK:
			gf2d_actor_set_action(&self->actor, "attack");
			self->hitbox.isActive = true;
			break;
		}
	}
}

void weapon_touch(Entity* self, Entity* other) { 
	if (other->type == ET_Enemy) {
		other->health -= self->damage;
		other->position.x += (self->owner->position.x > other->position.x?-1:1) * 20;
		gf2d_actor_set_action(&other->actor, "hurt");
		other->state = 5;
		vector2d_set(self->velocity, (self->flip.x == 0 ? -4 : 4), self->velocity.y);
		slog("health now %f", self->health);
		if (other->health <= 0)other->state = 6;

	}
	return; 
}