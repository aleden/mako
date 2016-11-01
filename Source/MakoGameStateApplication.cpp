#include "MakoGameStateApplication.h"

MAKO_BEGIN_NAMESPACE

GameStateApplication::GameStateApplication()
{ activeState = 0; }

GameStateApplication::~GameStateApplication()
{ if (activeState) activeState->Drop(); }

void GameStateApplication::SetActiveGameState(GameState* gs)
{
	if (activeState)
		activeState->Drop();
	activeState = gs;
	activeState->Hold();
}

void GameStateApplication::Frame()
{
	if (activeState)
		activeState->Frame();
}

MAKO_END_NAMESPACE
