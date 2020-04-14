#include "p_weapon.h"
#include "simple_logger.h"

#define WS_IDLE 1
#define WS_ATTACK 2
#define ES_ATTACK 6

typedef struct
{
	Uint32 maxWeapons;  
	Weapon* weaponList; 
}WeaponManager;

static WeaponManager weapon_manager = { 0 };

void weapon_manager_init(Uint32 maxWeapons)
{
	if (weapon_manager.weaponList != NULL)
	{
		//TODO: cleanup
	}
	if (!maxWeapons)
	{
		slog("cannot intialize a zero size weapon list!");
		return;
	}
	weapon_manager.weaponList = (Weapon*)malloc(sizeof(Weapon) * maxWeapons);
	if (weapon_manager.weaponList == NULL)
	{
		slog("failed to allocate %i entities for the entity manager", maxWeapons);
		return;
	}
	weapon_manager.maxWeapons = maxWeapons;
	memset(weapon_manager.weaponList, 0, sizeof(Weapon) * maxWeapons);
	slog("entity manager initialized");
	atexit(weapon_manager_close);
}

Weapon* weapon_new()
{
	int i;
	for (i = 0; i < weapon_manager.maxWeapons; i++)
	{
		if (weapon_manager.weaponList[i]._inuse)continue;
		weapon_manager.weaponList[i]._inuse = 1;
		return &weapon_manager.weaponList[i];
	}
	slog("out of open weapon slots in memory!");
	return NULL;
}

void weapon_manager_close()
{
	int i;
	for (i = 0; i < weapon_manager.maxWeapons; i++)
	{
		if (weapon_manager.weaponList[i]._inuse)
		{
			weapon_free(&weapon_manager.weaponList[i]);
		}
	}
	weapon_manager.maxWeapons = 0;
	free(weapon_manager.weaponList);
	weapon_manager.weaponList = NULL;
	slog("weapon manager closed");
}

void weapon_set_position(Weapon* wpn, Vector2D pos, Vector2D flip) {
	//wpn->position = pos;
	wpn->flip = flip;
}

void init_weapon(Weapon *wpn, enum Weapons type) {
	wpn->weaponType = type;
	wpn->active = false;
	wpn->state = WS_IDLE;
	wpn->think = weapon_think;
	wpn->frame = 0.0f;
	vector4d_set(wpn->color, 255, 255, 255, 255);
	vector2d_set(wpn->position, 417, 261.5);
	vector2d_set(wpn->scale, 1, 1);
	switch (wpn->weaponType) {
		case Dagger:
			wpn->damage = 5;
			//wpn->hitbox;
			gf2d_actor_load(&wpn->wpnActor, "../../actors/dagger.actor");
			gf2d_actor_set_action(&wpn->wpnActor, "idle");
			break;
		case Shortsword:
			break;
		case Spear:
			break;
		case Rapier:
			break;
		case Mace:
			break;
		}
	}

void weapon_free_all() {
	int i;
	for (i = 0; i < weapon_manager.maxWeapons; i++)
	{
		if (weapon_manager.weaponList[i]._inuse)
		{
			weapon_free(&weapon_manager.weaponList[i]);
		}
	}
}

void weapon_draw(Weapon* wpn) {
	//gf2d_actor_draw(&wpn->wpnActor, vector2d(417, 250), NULL, NULL, NULL, NULL, &wpn->color, &wpn->flip);
}

void weapon_think(Weapon* wpn, int state) {
	if (state != wpn->state)
	{
		wpn->state = state;
		switch (wpn->state) {
		case WS_IDLE:
			gf2d_actor_set_action(&wpn->wpnActor, "idle");
			break;
		case WS_ATTACK:
			gf2d_actor_set_action(&wpn->wpnActor, "attack");
			break;
		}
	}
	//gf2d_actor_draw(&wpn->wpnActor,vector2d(417,250),&wpn->scale,NULL,NULL,&wpn->flip);
	//gf2d_actor_next_frame(&wpn->wpnActor);
}

void weapon_free(Weapon* self)
{
	if (!self)return;
	memset(self, 0, sizeof(Weapon));
}


/*void weapon_touch(Entity* self, Entity* other) {}

void load_weapon(Entity* wepaonEnt, enum Weapons wpn) {
	switch (wpn) {
	case Dagger:
		break;
	case Shortsword:
		break;
	case Spear:
		break;
	case Rapier:
		break;
	case Mace:
		break;
	}
}*/