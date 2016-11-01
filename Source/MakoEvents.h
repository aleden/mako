#pragma once

#include "MakoCommon.h"
#include "MakoReferenceCounted.h"
#include "MakoPhysics3dDevice.h"
#include "MakoVec2d.h"

MAKO_BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////
// Enums

//! This enum identifies keys on a keyboard
//! Protip: The values of MAKO_KEYS are identical
//! to the winapi virtual key code values
enum KEY
{
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,

	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,

	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,

	KEY_LEFT  = 0x25,
	KEY_UP    = 0x26,
	KEY_RIGHT = 0x27,
	KEY_DOWN  = 0x28,

	KEY_ESCAPE = 0x1B,

	KEY_ENUM_LENGTH = 53
};

//! This enum identifies mouse buttons
enum MOUSE_BUTTON
{ MBTN_LEFT, MBTN_RIGHT, MBTN_MIDDLE, MBTN_ENUM_LENGTH };

//! This enum identifies possible event types
enum EVENT_TYPE
{
	ET_KEY,
	ET_MOUSE_BTN,
	ET_MOUSE_MOVE,
	ET_MOUSE_WHEEL,
	ET_PHYSICS_3D_ACTOR_PAIR_EVENT,
	ET_ENUM_LENGTH
};

///////////////////////////////////////////////////////////////////////////
// Events

//! An event is something that can happen at any given time. There is an
//! implementation for Event for each corrosponding EVENT_TYPE. To be notified
//! of when an event occurs, see EventReceiver.
class Event
{
public:
	//! Get the event type
	//! \return The event type
	virtual EVENT_TYPE GetEventType() const = 0;
	
	//! Virtual empty deconstructor
	virtual ~Event() {}
};

//! This event indicates that a key was pressed
class KeyEvent : public Event
{
private:
	bool down;
	KEY key;
public:
	
	//! Constructor
	MAKO_INLINE KeyEvent(KEY key, bool down) : key(key), down(down) {}

	//! Empty deconstructor
	MAKO_INLINE ~KeyEvent() {}
	
	MAKO_INLINE EVENT_TYPE GetEventType() const
	{ return ET_KEY; }

	//! Get the key that is involved with the event
	MAKO_INLINE KEY GetKey() const
	{ return key; }

	//! Find whether the event is indicating that the key has been released, or 
	//! pressed. This is important.
	MAKO_INLINE bool IsDown() const
	{ return down; }
};

//! This event indicates that a mouse button was pressed
class MouseButtonEvent : public Event
{
private:
	bool down;
	MOUSE_BUTTON btn;
public:
	//! Constructor
	MAKO_INLINE MouseButtonEvent(MOUSE_BUTTON btn, bool down) : down(down), btn(btn) {}

	//! Empty deconstructor
	MAKO_INLINE ~MouseButtonEvent() {}

	MAKO_INLINE EVENT_TYPE GetEventType() const
	{ return ET_MOUSE_BTN; }

	//! Get the button that is involved with the event
	MAKO_INLINE MOUSE_BUTTON GetButton() const
	{ return btn; }

	//! Find whether the event is indicating that the button has been released, or 
	//! pressed. This is important.
	MAKO_INLINE bool IsDown() const
	{ return down; }
};

class MouseMoveEvent : public Event
{
private:
	Vec2di change;
public:
	MAKO_INLINE MouseMoveEvent(const Vec2di& movement) : change(movement) {}
	MAKO_INLINE ~MouseMoveEvent() {}

	MAKO_INLINE EVENT_TYPE GetEventType() const
	{ return ET_MOUSE_MOVE; }

	MAKO_INLINE const Vec2di& GetMovement() const
	{ return change; }
};

class MouseWheelEvent : public Event
{
private:
	Int32 change;
public:
	MAKO_INLINE MouseWheelEvent(Int32 movement) : change(movement) {}
	MAKO_INLINE ~MouseWheelEvent() {}

	MAKO_INLINE EVENT_TYPE GetEventType() const
	{ return ET_MOUSE_WHEEL; }

	MAKO_INLINE Int32 GetMovement() const
	{ return change; }
};

class ActorPair
{
public:
	MAKO_INLINE ActorPair(Physics3dActor* actor1, Physics3dActor* actor2)
	: actor1(actor1), actor2(actor2) {}

	MAKO_INLINE ~ActorPair() {}

	Physics3dActor* actor1, *actor2;
};

//! This event indicates that a 3d physics event has occured
class Physics3dActorPairEvent : public Event
{
private:
	Physics3dActor* actor1, *actor2;
	CONTACT_PAIR_EVENT_TYPE ct;
public:
	//! Constructor
	MAKO_INLINE Physics3dActorPairEvent(Physics3dActor* actor1,
		                                Physics3dActor* actor2,
										CONTACT_PAIR_EVENT_TYPE ct)
	: actor1(actor1), actor2(actor2), ct(ct) {}
	
	//! Empty deconstructor
	MAKO_INLINE ~Physics3dActorPairEvent() {}
	
	MAKO_INLINE EVENT_TYPE GetEventType() const
	{ return ET_PHYSICS_3D_ACTOR_PAIR_EVENT; }
	
	//! Get the Physics3dActor that is involved in this event
	//! \return The Physics3dActor
	MAKO_INLINE ActorPair GetActorPair() const
	{ return ActorPair(actor1, actor2); }
	
	//! Get the collision type that has caused the event
	//! \return The collision type
	MAKO_INLINE CONTACT_PAIR_EVENT_TYPE GetCollisionType() const
	{ return ct; }
};

///////////////////////////////////////////////////////////////////////////
// Event receivers

//! An event receiver is a class that allows for the notification of an Event.
//! Like Event, for every enum in EVENT_TYPE there is a corrosponding implementation
//! of EventReceiver for it. 
//! 
//! DO NOT INHERT FROM EVENTRECEIVER TO RECEIVE EVENTS! INHERT FROM THE 
//! CLASSES DECLARED BELOW EVENTRECEIVER TO HANDLE EVENTS.
class EventReceiver : public ReferenceCounted
{
public:
	//! Get the type of event that the event receiver implementing this
	//! receives
	virtual EVENT_TYPE GetTypeOfEventHandled() const = 0;
};

//! Receive a KeyEvent
class KeyEventReceiver : public EventReceiver
{
public:
	//! Implement this method to receive KeyEvent s. 
	virtual void OnEvent(const KeyEvent* event) = 0;
	
	MAKO_INLINE EVENT_TYPE GetTypeOfEventHandled() const
	{ return ET_KEY; }
};

//! Receive a MouseButtonEvent
class MouseButtonEventReceiver : public EventReceiver
{
public:
	//! Implement this method to receive MouseButtonEvent s. 
	virtual void OnEvent(const MouseButtonEvent* event) = 0;

	MAKO_INLINE EVENT_TYPE GetTypeOfEventHandled() const
	{ return ET_MOUSE_BTN; }
};

//! Receive a MouseButtonEvent
class MouseMoveEventReceiver : public EventReceiver
{
public:
	//! Implement this method to receive MouseButtonEvent s. 
	virtual void OnEvent(const MouseMoveEvent* event) = 0;

	MAKO_INLINE EVENT_TYPE GetTypeOfEventHandled() const
	{ return ET_MOUSE_MOVE; }
};

//! Receive a MouseButtonEvent
class MouseWheelEventReceiver : public EventReceiver
{
public:
	//! Implement this method to receive MouseButtonEvent s. 
	virtual void OnEvent(const MouseWheelEvent* event) = 0;

	MAKO_INLINE EVENT_TYPE GetTypeOfEventHandled() const
	{ return ET_MOUSE_WHEEL; }
};

//! Receive a Physics3dActorPairEvent
class Physics3dActorPairEventRecevier : public EventReceiver
{
public:
	//! Implement this method to receive Physics3dActorPairEvent s. 
	virtual void OnEvent(const Physics3dActorPairEvent* event) = 0;

	MAKO_INLINE EVENT_TYPE GetTypeOfEventHandled() const
	{ return ET_PHYSICS_3D_ACTOR_PAIR_EVENT; }
};

MAKO_END_NAMESPACE
