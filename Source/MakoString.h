#pragma once
#include "MakoCommon.h"
#include <cstdio>
#include <cstring>

MAKO_BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////
// Small Inline/Template string data functions not having external
// dependencies from non-built in types.
template <typename T>
MAKO_INLINE bool IsCharDigit(const T& c)
{ return c >= static_cast<T>('0') && c <= static_cast<T>('9'); }

template <typename T>
MAKO_INLINE bool IsCharSpace(const T& c)
{ return c == static_cast<T>(' ')  || 
         c == static_cast<T>('\f') || 
		 c == static_cast<T>('\n') ||
		 c == static_cast<T>('\r') ||
		 c == static_cast<T>('\t') ||
		 c == static_cast<T>('\v'); }

template <typename T>
MAKO_INLINE bool IsCharUpper(const T& c)
{ return c >= static_cast<T>('A') && c <= static_cast<T>('Z'); }

// Gets the length of any string data
template <typename T>
UInt32 GetAnyStrDataLen(const T* data)
{
	UInt32 r;
	for (r = 0; *data != (T)'\0'; ++data) ++r;
	return r;
}

enum TRIM_STYLE
{ FROM_LEFT_AND_RIGHT, FROM_LEFT, FROM_RIGHT };

typedef UInt16 StringChar;

//! This class represents a string.
template <typename T>
class BasicString
{
	friend class DirectoryPath;
	friend class FilePath;

	UInt32 length;
	mutable T* data;
	mutable char* ascstr;
	mutable wchar_t* aswstr;
	// Has string changed since last ToASCII()
	mutable bool changed;
	// Has string changed since last ToWStringData()
	mutable bool changed2;

	bool fromdata;

	BasicString(T* str, UInt32 len, bool fromdata = false) : length(len), data(str), ascstr(nullptr),
		aswstr(nullptr), changed(false), changed2(false), fromdata(fromdata) {}
public:
	friend BasicString<StringChar> ToString(const char* cstr);
	friend BasicString<StringChar> ToString(const wchar_t* wstr);
	//////////////////////////////////////////////////////////////////////////////
	// Constructors/Deconstructor

	static MAKO_INLINE const BasicString<T> FromData(const T* data)
	{ return BasicString<T>(const_cast<T*>(data), GetAnyStrDataLen(data), true); }

	static MAKO_INLINE const BasicString<T> FromData(const T* data, UInt32 length)
	{ return BasicString<T>(const_cast<T*>(data), length, true); }

	static MAKO_INLINE const BasicString<T> FromAllocData(const T* data)
	{ return BasicString<T>(const_cast<T*>(data), GetAnyStrDataLen(data), false); }

	static MAKO_INLINE const BasicString<T> FromAllocData(const T* data, UInt32 length)
	{ return BasicString<T>(const_cast<T*>(data), length, false); }

	static MAKO_INLINE BasicString<T> From32BitInt(Int32 n)
	{
		UInt32 length;
		T* data = ToStringData(static_cast<Int32>(n), &length);
		return BasicString<T>(data, length, false);
	}

	static MAKO_INLINE BasicString<T> From32BitUInt(UInt32 n)
	{
		UInt32 length;
		T* data = ToStringData(static_cast<UInt32>(n), &length);
		return BasicString<T>(data, length, false);
	}

	static MAKO_INLINE BasicString<T> From32BitFloat(Float32 n)
	{
		UInt32 length;
		T* data = ToStringData(static_cast<Float32>(n), &length);
		return BasicString<T>(data, length, false);
	}

	static MAKO_INLINE BasicString<T> From64BitFloat(Float64 n)
	{
		UInt32 length;
		T* data = ToStringData(static_cast<Float64>(n), &length);
		return BasicString<T>(data, length, false);
	}

	BasicString(const BasicString<T>& string) : ascstr(nullptr),
		aswstr(nullptr), changed(false), changed2(false), fromdata(false)
	{
		if (string.GetLength() == 0)
		{
			data = nullptr;
			length = 0;
			return ;
		}
		length = string.GetLength();
		data = new T[length + 1];
		memcpy(data, string.GetData(), (length + 1) * sizeof(T));
	}

	MAKO_INLINE BasicString(const T* string, UInt32 len = 0xFFFFFFFF)
		: ascstr(nullptr), aswstr(nullptr), changed(false), changed2(false), fromdata(false)
	{
		length = len != 0xFFFFFFFF ? len : GetAnyStrDataLen(string);
		data = new T[length + 1];
		memcpy(data, string, length * sizeof(T));
		data[length] = (T)'\0';
	}

	MAKO_INLINE BasicString() : length(0), data(nullptr), ascstr(nullptr),
		aswstr(nullptr), changed(false), changed2(false), fromdata(false) {}

	MAKO_INLINE ~BasicString()
	{
		if (!fromdata)
			delete [] data;

		delete [] ascstr;
		delete [] aswstr;
	}


	//////////////////////////////////////////////////////////////////////////////
	// Member functions

	static BasicString<T> Concatenate(const T* lhs, const T* rhs)
	{
		UInt32 lhslen = GetAnyStrDataLen(lhs), rhslen = GetAnyStrDataLen(rhs);
		UInt32 combinedLength = lhslen + rhslen;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, lhs, sizeof(T) * lhslen);
		memcpy(newData + lhslen, rhs, sizeof(T) * rhslen);
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	static BasicString<T> Concatenate(const T* lhs, const BasicString<T>& rhs)
	{
		UInt32 lhslen = GetAnyStrDataLen(lhs);
		UInt32 combinedLength = rhs.GetLength() + lhslen;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, lhs, sizeof(T) * lhslen);
		memcpy(newData + lhslen, rhs.GetData(), sizeof(T) * rhs.GetLength());
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	static BasicString<T> Concatenate(const BasicString<T>& lhs, const T* rhs)
	{
		UInt32 rhslen = GetAnyStrDataLen(rhs);
		UInt32 combinedLength = lhs.GetLength() + rhslen;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, lhs.GetData(), sizeof(T) * lhs.GetLength());
		memcpy(newData + lhs.length, rhs, sizeof(T) * rhslen);
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	static BasicString<T> Concatenate(const BasicString<T>& lhs, const BasicString<T>& rhs)
	{
		UInt32 combinedLength = lhs.GetLength() + rhs.GetLength();
		T* newData = new T[combinedLength + 1];
		memcpy(newData, lhs.GetData(), sizeof(T) * lhs.GetLength());
		memcpy(newData + lhs.length, rhs.GetData(), sizeof(T) * rhs.GetLength());
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	static BasicString<T> Concatenate(const BasicString<T>& lhs, const T& rhs)
	{
		UInt32 combinedLength = lhs.GetLength() + 1;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, lhs.GetData(), sizeof(T) * lhs.GetLength());
		newData[combinedLength - 1] = rhs;
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	static BasicString<T> Concatenate(const T& lhs, const BasicString<T>& rhs)
	{
		UInt32 combinedLength = rhs.GetLength() + 1;
		T* newData = new T[combinedLength + 1];
		newData[0] = lhs;
		memcpy(newData + 1, rhs.GetData(), sizeof(T) * rhs.GetLength());
		newData[combinedLength] = (T)'\0';

		return BasicString(newData, combinedLength, false);
	}

	//! Get the length of the BasicString, not including the last null character
	MAKO_INLINE UInt32 GetLength() const
	{ return length; }

	//! Get the BasicString in bytes.
	MAKO_INLINE const T* GetData() const
	{ return data; }

	MAKO_INLINE bool IsEmpty() const
	{ return data == nullptr || length == 0 || *data == '\0'; }

	//! Finds the position of a character in this string from the beginning.
	//! \return The position of the character, or the length of the string
	//! if the character was not found in the string.
	UInt FindFromStart(const T& character) const
	{
		UInt i;
		for (i = 0; *data != character && *data != (T)'\0'; ++data)
			++i;
		data -= i;
		return i;
	}

	//! Finds the position of a character in this string, searching from the end.
	//! \return The position of the character, or zero the character was not 
	//! found in the string.
	UInt FindFromEnd(const T& character) const
	{
		UInt i = length;
		for (data += length; *data != character; --data)
			--i;
		data -= i;
		return i;
	}

	void LowerCase()
	{
		for (UInt i = 0; i < length; ++i)
		{
			if (IsCharUpper(data[i]))
				data[i] += 0x20;
		}
	}


	//! Erase characters from the string.
	//! \param index The index of the start character to erase from
	//! \param n (Default = 1) The number of characters past index to erase
	void Erase(UInt32 index, UInt32 n = 1)
	{
		T* newData = new T[(GetLength() + 1) - n];
		memcpy(newData, data, sizeof(T) * index);
		memcpy(newData + index, data + (index + n), sizeof(T) * (length - (index + n)));
		memcpy(newData + index, data + (index + n), sizeof(T) * ((length - (index + n)) + 1));

		delete [] data;
		data = newData;
		length = GetLength() - n;
		Changed();
	}

	//! Get a sub string from this string. Example:
	//!
	//! String str(Text("0123456789"));
	//! String substr = str.GetSubString(1, 3);
	//! // substr = 123
	//! \param index The index of the start character to copy from
	//! \param n (Default = 1) The number of characters past index to copy
	MAKO_INLINE BasicString<T> GetSubString(UInt32 index, UInt32 n = 1) const
	{ return BasicString<T>(GetData() + index, n); }

	//! This function trims a string from the left and right
	//! \param[in] source The string to trim
	//! \param[in] t What to trim
	//! \return Returns this string
	BasicString<T>& Trim(const T& t = (T)' ', TRIM_STYLE tt = FROM_LEFT_AND_RIGHT)
	{
		switch (tt)
		{
		case FROM_LEFT_AND_RIGHT:
			TrimLeft(t);
			TrimRight(t);
			break;

		case FROM_LEFT:
			TrimLeft(t);
			break;

		case FROM_RIGHT:
			TrimRight(t);
			break;
		}

		Changed();
		return *this;
	}

	void Clear()
	{
		delete [] data;
		data = nullptr;
		length = 0;
		Changed();
	}

	//////////////////////////////////////////////////////////////////////////////
	// Operator overloads

	//! Get a character. Do not replace a character in this string
	//! with '\0', it will mess up the length.
	MAKO_INLINE T& operator [] (UInt32 i)
	{ return data[i]; Changed(); }

	//! Get a character
	MAKO_INLINE const T& operator [] (UInt32 i) const
	{ return data[i]; }

	//! Get a character. Do not replace a character in this string
	//! with '\0', it will mess up the length.
	MAKO_INLINE T& At(UInt32 i)
	{ return data[i]; Changed(); }

	//! Get a character
	MAKO_INLINE const T& At(UInt32 i) const
	{ return data[i]; }

	//! Append's to this string's contents
	MAKO_INLINE BasicString<T>& operator += (const BasicString<T>& rhs)
	{ Append(rhs); return *this; }

	//! Append's to this string's contents
	MAKO_INLINE BasicString<T>& operator += (const T* rhs)
	{ Append(rhs); return *this; }

	//! Append's to this string's contents a character
	MAKO_INLINE BasicString<T>& operator += (const T& rhs)
	{ Append(rhs); return *this; }

	//! Creates a new string that is the Concatenation of this string
	//! and the other string.
	MAKO_INLINE BasicString<T> operator + (const BasicString<T>& rhs) const
	{ return Concatenate(*this, rhs); }

	//! Creates a new string that is the Concatenation of this string
	//! and the other string.
	MAKO_INLINE BasicString<T> operator + (const T* rhs) const
	{ return Concatenate(*this, rhs); }

	//! Creates a new string that is the Concatenation of this string
	//! and the other stringchar.
	MAKO_INLINE BasicString<T> operator + (const T& rhs) const
	{ return Concatenate(*this, rhs); }

	//! AssignTo this string's contents to the other's
	MAKO_INLINE BasicString<T>& operator = (const BasicString<T>& rhs)
	{ AssignTo(rhs); return *this; }

	//! AssignTo this string's contents to the other's
	MAKO_INLINE BasicString<T>& operator = (const T* rhs)
	{ AssignTo(rhs); return *this; }

	//! AssignTo this string's contents to the stringchar
	MAKO_INLINE BasicString<T>& operator = (const T& rhs)
	{ AssignTo(rhs); return *this; }

	//! Check whether this string's contents are identical to the other's
	bool operator == (const BasicString<T>& rhs) const
	{
		for (UInt i = 0; i < rhs.GetLength(); ++i)
			if (data[i] != rhs[i]) return false;
		return true;
	}

	//! Check whether this string's contents are identical to the other's
	bool operator == (const T* rhs) const
	{
		for (UInt i = 0, len = GetAnyStrDataLen(rhs); i < len; ++i)
			if (data[i] != rhs[i]) return false;
		return true;
	}

	//! Check if this string is identical to a single character.
	bool operator == (const T& rhs) const
	{ return *data == rhs && GetLength() == 1; }


	//////////////////////////////////////////////////////////////////////////////
	// Append
	MAKO_INLINE void Append32BitUInt(UInt32 n)
	{ UInt32 len; T* d = ToStringData(n, &len); Append(d, len); delete [] d; Changed(); }

	MAKO_INLINE void Append32BitInt(Int32 n)
	{ UInt32 len; T* d = ToStringData(n, &len); Append(d, len); delete [] d; Changed(); }

	MAKO_INLINE void Append32BitFloat(Float32 n)
	{ UInt32 len; T* d = ToStringData(n, &len); Append(d, len); delete [] d; Changed(); }

	MAKO_INLINE void Append64BitFloat(Float64 n)
	{ UInt32 len; T* d = ToStringData(n, &len); Append(d, len); delete [] d; Changed(); }

	//! Append's the string's contents
	void Append(const BasicString<T>& rhs)
	{
		UInt32 combinedLength = GetLength() + rhs.length;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, data, sizeof(T) * length);
		memcpy(newData + length, rhs.data, sizeof(T) * rhs.length);
		newData[combinedLength] = (T)'\0';

		delete [] data;
		data = newData;

		length = combinedLength;

		Changed();
	}

	//! Append's a char
	void Append(const T& character)
	{
		UInt32 combinedLength = GetLength() + 1;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, data, sizeof(T) * length);
		(*(newData + length)) = character;
		newData[combinedLength] = (T)'\0';

		delete [] data;
		data = newData;

		length = combinedLength;

		Changed();
	}

	//! Append's the string's contents
	void Append(const T* rhs, UInt32 strlen = 0xFFFFFFFF)
	{
		UInt32 rhslen = strlen != 0xFFFFFFFF ? strlen : GetAnyStrDataLen(rhs);
		UInt32 combinedLength = GetLength() + rhslen;
		T* newData = new T[combinedLength + 1];
		memcpy(newData, data, sizeof(T) * GetLength());
		memcpy(newData + GetLength(), rhs, sizeof(T) * rhslen);
		newData[combinedLength] = (T)'\0';

		delete [] data;
		data = newData;

		length = combinedLength;

		Changed();
	}

	//////////////////////////////////////////////////////////////////////////////
	// AssignTo
	MAKO_INLINE void AssignToU32BitInt(UInt32 n)
	{ delete [] data; data = ToStringData(n, &length); Changed(); }

	MAKO_INLINE void AssignTo32BitInt(Int32 n)
	{ delete [] data; data = ToStringData(n, &length); Changed(); }

	MAKO_INLINE void AssignTo32BitFloat(Float32 n)
	{ delete [] data; data = ToStringData(n, &length); Changed(); }

	MAKO_INLINE void AssignTo64BitFloat(Float64 n)
	{ delete [] data; data = ToStringData(n, &length); Changed(); }

	//! Copy the string's contents to be this string's contents
	void AssignTo(const BasicString<T>& string)
	{
		if (string.IsEmpty())
			return;

		delete [] data;
		length = string.GetLength();
		data = new T[length + 1];
		memcpy(data, string.GetData(), (length + 1) * sizeof(T));
		Changed();
	}

	//! Copy the string's contents to be this string's contents
	void AssignTo(const T* string, UInt32 len = 0xFFFFFFFF)
	{
		delete [] data;
		length = len != 0xFFFFFFFF ? len : GetAnyStrDataLen(string);
		data = new T[length + 1];
		memcpy(data, string, length * sizeof(T));
		data[length] = static_cast<T>('\0');
		Changed();
	}

	//! Set this string to contain a single string char
	void AssignTo(const T& character)
	{
		delete [] data;
		length = 1;
		data = new T[2];
		data[0] = character;
		data[1] = static_cast<T>('\0');
		Changed();
	}

	//////////////////////////////////////////////////////////////////////////////
	// To functions

	//! Converts this BasicString to a 64-bit floating point number
	Float64 To64BitFloat() const
	{
		if (sizeof(T) != sizeof(char))
		{
			char* cstr = ToASCII();
			Float64 r = atof(cstr);
			return r;
		}
		else
		{
			return atof((char*)data);
		}
	}

	//! Converts this BasicString to a 64-bit floating point number
	Float32 To32BitFloat() const
	{
		if (sizeof(T) != sizeof(char))
		{
			char* cstr = ToASCII();
			Float32 r = static_cast<Float32>(atof(cstr));
			return r;
		}
		else
		{
			return static_cast<Float32>(atof((char*)data));
		}
	}

	UInt32 To32BitUInt() const
	{
		if (sizeof(T) != sizeof(char))
		{
			const char* cstr = ToASCII();
			UInt32 r = static_cast<UInt32>(strtoul(cstr, NULL, 10));
			return r;
		}
		else
		{
			return static_cast<UInt32>(strtoul((char*)data, NULL, 10));
		}
	}

	Int32 To32BitInt() const
	{
		if (sizeof(T) != sizeof(char))
		{
			char* cstr = ToASCII();
			Int32 r = static_cast<Int32>(atoi(cstr));
			return r;
		}
		else
		{
			return static_cast<Int32>(atoi((char*)data));
		}
		return nullptr;
	}

	//! Get BasicString as a C string (char*)
	//! \return A C string.
	const char* ToASCII() const
	{
		if (sizeof(T) != sizeof(char))
		{
			if (!ascstr || (changed && aswstr))
			{
				ascstr = new char[length + 1];
				for (UInt i = 0; i < length; ++i)
					ascstr[i] = (char)data[i];
				ascstr[length] = '\0';
				return ascstr;
			}
			else if (ascstr && !changed)
			{
				return ascstr;
			}
		}
		else
		{
			return (char*)data;
		}
		return nullptr;
	}

	//! Get BasicString as a c wide string (wchar_t*)
	//! \return A wide string.
	const wchar_t* ToWStringData() const
	{
		if (!data)
			return L"";
		if (sizeof(T) != sizeof(wchar_t))
		{
			if (!aswstr || (changed2 && aswstr))
			{
				aswstr = new wchar_t[length + 1];
				for (UInt i = 0; i < length; ++i)
					aswstr[i] = (char)data[i];
				aswstr[length] = '\0';
				changed2 = false;
				return aswstr;
			}
			else if (aswstr && !changed2)
			{
				return aswstr;
			}
		}
		else
		{
			return (wchar_t*)data;
		}
	}
private:

	// This is called when the string has been modified
	MAKO_INLINE void Changed()
	{ changed = changed2 = true; }

	static T* ToStringData(Float32 f, UInt32* strlen = 0)
	{
		char buff[32];
		UInt32 len = _snprintf(buff, 32, "%f", f);

		T* newData = new T[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = buff[i];
		newData[len] = '\0';

		if (strlen)
			*strlen = len;

		return newData;
	}

	static T* ToStringData(Float64 f, UInt32* strlen = 0)
	{
		char buff[32];
		UInt32 len = _snprintf(buff, 32, "%f", f);

		T* newData = new T[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = buff[i];
		newData[len] = '\0';

		if (strlen)
			*strlen = len;

		return newData;
	}

	static T* ToStringData(Int32 num, UInt32* strlen = 0)
	{
		char buff[32];
		UInt32 len = _snprintf(buff, 32, "%i", num);

		T* newData = new T[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = buff[i];
		newData[len] = '\0';

		if (strlen)
			*strlen = len;

		return newData;
	}

	static T* ToStringData(UInt32 num, UInt32* strlen = 0)
	{
		char buff[32];
		UInt32 len = _snprintf(buff, 32, "%u", num);

		T* newData = new T[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = buff[i];
		newData[len] = '\0';

		if (strlen)
			*strlen = len;

		return newData;
	}

	//! This function trims a string from the right
	//! \param[in] source The string to trim
	//! \param[in] t What to trim
	void TrimRight(const T& t = (T)' ')
	{
		if (At(GetLength() - 1) != t)
			return ;

		UInt32 pos = GetLength() - 1;
		for (UInt i = GetLength() - 2; i >= 0; --i)
		{
			if (At(i) != t)
				break;
			pos = i;
		}
		Erase(pos, GetLength() - pos);
	}

	//! This function trims a string from the left
	//! \param[in] source The string to trim
	//! \param[in] t What to trim
	void TrimLeft(const T& t = (T)' ')
	{
		if (At(0) != t)
			return ;

		UInt32 pos = 0;
		for (UInt i = 1; i < GetLength(); ++i)
		{
			if (At(i) != t)
				break;
			pos = i;
		}
		Erase(0, pos + 1);
	}

	static T* ToStringData(const char* cstr, UInt32* strlen = 0)
	{
		UInt32 len = GetAnyStrDataLen(cstr);
		T* newData = new T[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = (T)cstr[i];
		newData[len] = (T)'\0';


		if (strlen)
			*strlen = len;
		return newData;
	}
};


typedef BasicString<wchar_t> WString;
typedef BasicString<char> ASCIIString;
typedef BasicString<StringChar> String;


#ifndef ASCIIText
	#define ASCIIText(QUOTES) ASCIIString::FromData(QUOTES)
#else
	#error "ASCIIText has already been defined."
#endif

#ifndef Text
	#define Text(QUOTES) String::FromData((Mako::StringChar*)L##QUOTES)
#else
	#error "Text has already been defined."
#endif

//! Convert to a String
//! \param[in] x The thing to convert
//! \return x converted to a string
MAKO_INLINE String ToString(const char* cstr)
{
	if (sizeof(StringChar) != sizeof(char))
	{
		UInt32 len = GetAnyStrDataLen(cstr);
		StringChar* newData = new StringChar[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = cstr[i];
		newData[len] = (StringChar)'\0';

		return String(newData, len);
	}
	else
	{
		return String((StringChar*)cstr);
	}
}

//! Convert to a String
//! \param[in] x The thing to convert
//! \return x converted to a string
MAKO_INLINE String ToString(const wchar_t* wstr)
{
	if (sizeof(StringChar) != sizeof(wchar_t))
	{
		UInt32 len = GetAnyStrDataLen(wstr);
		StringChar* newData = new StringChar[len + 1];
		for (UInt i = 0; i < len; ++i)
			newData[i] = wstr[i];
		newData[len] = (StringChar)'\0';

		return String(newData, len);
	}
	else
	{
		return String((StringChar*)wstr);
	}
}

MAKO_END_NAMESPACE

template <typename T>
Mako::BasicString<T> operator + (const T* lhs, const Mako::BasicString<T>& rhs)
{ return Mako::BasicString<T>::Concatenate(lhs, rhs); }

template <typename T>
Mako::BasicString<T> operator + (const T& character, const Mako::BasicString<T>& rhs)
{ return Mako::BasicString<T>::Concatenate(character, rhs); }

template <typename T>
bool operator == (const T* lhs, const Mako::BasicString<T>& rhs)
{ return rhs == lhs; }

template <typename T>
bool operator == (const T& lhs, const Mako::BasicString<T>& rhs)
{ return lhs == rhs[0] && rhs.GetLength() == 1; }