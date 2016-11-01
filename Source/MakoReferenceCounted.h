#pragma once

#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

//! This class provides reference counting through the methods Hold() and Drop().
//! if you no longer need the object, you have to call Drop(). This will destroy 
//! the object, if Hold() was not called in another part of you program, 
//! because this part still needs the object.
class ReferenceCounted
{
private:
	mutable Int32 referenceCounter;
public:
	// Constructor sets referenceCounter to zero
	MAKO_API MAKO_INLINE ReferenceCounted() : referenceCounter(0) {}

	// Empty deconstructor
	MAKO_API virtual MAKO_INLINE ~ReferenceCounted() {}

	//! Takes the object. Increments the reference counter by one. Someone who calls Hold() to an object, 
	//! should later also call Drop() to it. f an object never gets as much Hold() as
	//! Release() calls, it will never be destroyed.
	MAKO_API MAKO_INLINE void Hold() const
	{ ++referenceCounter; }

	//! Drops the object. Decrements the reference counter by one. If the object is deleted, Drop() will
	//! return true. Else, it will return false.
	MAKO_API bool Drop() const;

	//! Get the reference count.
	MAKO_API MAKO_INLINE Int32 GetReferenceCount() const
	{ return referenceCounter; }
};

MAKO_END_NAMESPACE
