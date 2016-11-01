#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoDirectoryPath.h"

MAKO_BEGIN_NAMESPACE

class FilePath
{
	String fp;
	mutable String ext;

	MAKO_INLINE bool IsRelative(const String& fp)
	{ return fp[1] == ':'; }
public:
	MAKO_INLINE FilePath() {}
	MAKO_INLINE FilePath(const String& fp) : fp(fp) {}
	MAKO_INLINE FilePath(const StringChar* text) : fp(text) {}

	MAKO_INLINE ~FilePath() {}

	MAKO_INLINE FilePath& operator = (const FilePath& rhs)
	{
		fp = rhs.fp;
		ext = rhs.ext;
		return *this;
	}

	MAKO_INLINE const String& GetAbs() const
	{ return fp; }

	MAKO_INLINE const String& GetExt() const
	{
		if (ext.IsEmpty())
		{
			UInt dotPos = fp.FindFromEnd(StringChar('.'));
			ext = fp.GetSubString(dotPos + 1, fp.GetLength() - dotPos - 1);
		}
		return ext;
	}

	MAKO_INLINE DirectoryPath GetDir() const
	{
		UInt dotPos = fp.FindFromEnd(StringChar('/'));
		return fp.GetSubString(0, dotPos);
	}

	MAKO_INLINE operator const String& () const
	{ return fp; }
};

MAKO_END_NAMESPACE

//template <typename T>
//Mako::BasicString<T> operator + (const T* lhs, const Mako::FilePath& rhs)
//{ return Mako::BasicString<T>::Concatenate(lhs, rhs.GetAbs()); }
//
//template <typename T>
//Mako::BasicString<T> operator + (const T& character, const Mako::FilePath& rhs)
//{ return Mako::BasicString<T>::Concatenate(character, rhs.GetAbs()); }
//
//template <typename T>
//bool operator == (const T* lhs, const Mako::FilePath& rhs)
//{ return rhs == lhs.GetAbs(); }
//
//template <typename T>
//bool operator == (const T& lhs, const Mako::FilePath& rhs)
//{ return lhs == rhs.GetAbs()[0] && rhs.GetAbs().GetLength() == 1; }