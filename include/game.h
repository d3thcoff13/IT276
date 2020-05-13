#include "level.h"
#ifndef __GameState_e__
#define __GameState_e__

typedef enum {
	Paused,
	MainMenu,
	InGame,
	Quit
}GameState;
#endif
GameState currentGameState;
void SetGameState(GameState newGameState);
Level* level;
int* currentLevel;
Entity* player_entity;
void Save_Game();
void Load_Game();

