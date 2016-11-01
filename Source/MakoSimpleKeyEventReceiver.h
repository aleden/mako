#pragma once
#include "MakoCommon.h"
#include "Makoevents.h"
#include "MakoMap.h"

MAKO_BEGIN_NAMESPACE

class Application;

//! This is a simple implementation of KeyEventReceiver. Users
//! of the Mako game engine can use this, it provides an easy 
//! to use method IsKeyDown() to tell if any key is down or not
//! at any given time.
class SimpleKeyEventReceiver: public KeyEventReceiver
{
private:
	Map<UInt16, bool> keys;
	
	Application* app;
public:
	//! Constructor
	MAKO_API SimpleKeyEventReceiver(Application* app);
	
	//! Deconstructor
	MAKO_API MAKO_INLINE ~SimpleKeyEventReceiver() {}
	
	//! Tell if a key is down or is not
	//! \param[in] key The key to check
	//! \return True if the key is being held down, false if it is
	//! not
	MAKO_API MAKO_INLINE bool IsKeyDown(KEY key)
	{ return keys[key]; }

	MAKO_API MAKO_INLINE void OnEvent(const KeyEvent* event)
	{ keys[static_cast<const KeyEvent*>(event)->GetKey()] =
	       static_cast<const KeyEvent*>(event)->IsDown(); }

};

MAKO_END_NAMESPACE