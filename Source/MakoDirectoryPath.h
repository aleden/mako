#pragma once
#include "MakoCommon.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

class DirectoryPath
{
	String dir;
	mutable String name;
public:
	MAKO_INLINE DirectoryPath() {}

	MAKO_INLINE DirectoryPath(const StringChar* text) : dir(text)
	{
		if (dir[dir.GetLength()-1] == '/' || dir[dir.GetLength()-1] == '\\')
		{
			this->dir[dir.GetLength()-1] = '\0';
			--this->dir.length;
		}
	}

	MAKO_INLINE DirectoryPath(const String& dir) : dir(dir)
	{
		if (dir[dir.GetLength()-1] == '/' || dir[dir.GetLength()-1] == '\\')
		{
			this->dir[dir.GetLength()-1] = '\0';
			--this->dir.length;
		}
	}

	MAKO_INLINE ~DirectoryPath() {}

	MAKO_INLINE const String& GetFull() const
	{ return dir; }

	MAKO_INLINE const String& GetName() const
	{
		if (name.IsEmpty())
		{
			UInt slashPos = dir.FindFromEnd(StringChar('/'));
			if (slashPos == 0)
				slashPos = dir.FindFromEnd(StringChar('\\'));

			name = dir.GetSubString(slashPos + 1, dir.GetLength() - slashPos);
		}
		return name;
	}

	MAKO_INLINE operator const String& () const
	{ return dir; }
};

MAKO_END_NAMESPACE