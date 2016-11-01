#pragma once
#include "MakoSimpleApplication.h"
#include "MakoReferenceCounted.h"

MAKO_BEGIN_NAMESPACE

//! Represents a state in a game. For more information on it's usage,
//! see the documentation for GameStateApplication.
class GameState : public ReferenceCounted
{
public:
	virtual void Frame() = 0;
};

//! This is an alternative than SimpleApplication when making your game.
//! While SimpleApplication is only appropriate for small games and
//! demos, GameStateApplication is appropriate for larger and full games.
//! It has an "active state", which is merely a pointer to an instance
//! of GameState.
//!
//! The user of GameStateApplication will set the active
//! game state for different states of a game. For example, you may have
//! a game that has a menu screen, and a mode where the user is actually
//! playing the game. To program this, you would create two classes
//! MenuState, and PlayingState inheriting GameState. In each of them,
//! the Frame() function would have different behavior defined. 
//!
//! When a user clicks on a menu item to start playing, you would simply do
//! 
//! <tt>SetActiveGameState(new PlayingState(applicationInstance));</tt>
//!
//! The active game state would be dropped, calling the deconstructor, allowing
//! for the MenuState to drop menu items and stuff allocated in the menu mode. And,
//! when the user decides to stop playing and go to the menu, you simply do
//!
//! <tt>SetActiveGameState(new MenuState(applicationInstance));</tt>
//!
//! The active state will be dropped, and the deconstructor for PlayingState will be
//! called, allowing for the dropping of meshes loaded and other allocated objects
//! needed for the playing state.
class GameStateApplication : public SimpleApplication
{
	GameState* activeState;
public:
	//! Empty constructor
	MAKO_API GameStateApplication();
	
	//! Empty deconstructor
	MAKO_API virtual ~GameStateApplication();
	
	//! Set the new game state
	//! \param[in] gs The new active game state
	MAKO_API void SetActiveGameState(GameState* gs);
	
	//! Not to be overridden. This is only overridden
	//! by GameStateApplication.
	MAKO_API void Frame();
};

MAKO_END_NAMESPACE
