#pragma once

#include "MakoCommon.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

//! This class is what's to be thrown when an exception needs to be thrown.
class Exception
{
public:
	//! This is a numeric error code, just in case there is a need for one.
	Int32 errorCode;
	
	//! This identifies the intent of the exception
	String description;
	
	//! Simple constructor
	//! \param[in] description This identifies the intent of the exception
	//! \param[in] errorCode (optional) This is a numeric error code, just in 
	//! case there is a need for one.
	MAKO_API MAKO_INLINE Exception(const String& description, Int32 errorCode = -1)
		: description(description), errorCode(errorCode)
	{}
	
	MAKO_API MAKO_INLINE ~Exception() {}
};

MAKO_END_NAMESPACE
