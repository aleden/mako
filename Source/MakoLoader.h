#pragma once
#include "MakoCommon.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

//! A Loader is a class capable of parsing data and
//! interpreting it by a specific format, such as 
//! a JPEG file.
class Loader
{
public:
	//! Test whether the format that this Loader is
	//! meant to load uses the specified file extension.
	//! \param ext The file format's extension. Since
	//! file extensions are not case-sensitive, it is
	//! a precondition that ext is in all lower-case.
	//! Example of an extension given: "jpg".
	virtual bool IsLoadableFileExt(const String& ext) const = 0;
};

MAKO_END_NAMESPACE