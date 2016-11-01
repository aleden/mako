#pragma once
#include "MakoCommon.h"
#include "MakoReferenceCounted.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

//! Identifiers for different stream types
enum STREAM_TYPE
{ ST_READ, ST_WRITE, ST_ENUM_LENGTH };

//! The user needs to supply a Stream implementation to a number of 
//! methods to allow the engine to read or write chunks of binary data.
//! This allows flexibility for the source/destination of the data. For 
//! example the Stream could store data in a file, memory buffer or custom 
//! file format. A Stream has no fixed length- although some specific
//! implementations may have a fixed length (such as file and memory streams).
class Stream : public ReferenceCounted
{
public:
	//! Tell whether the stream is for reading or writing.
	//! \return If ST_READ, it is safe to cast this to a
	//! InputStream. If ST_WRITE, it is safe to cast this to
	//! a OutputStream. Usually in the engine Streams will not
	//! be passed around, and Read/Write Streams will be passed
	//! around (so the use of this method should not be necessary).
	virtual STREAM_TYPE GetType() const = 0;
};

//! This class is used for reading data. Any data read from any
//! read methods will be used in front of any data previously
//! read by read calls (reading starts at the beginnning of the stream).
class InputStream : public Stream
{
public:
	MAKO_INLINE STREAM_TYPE GetType() const
	{ return ST_READ; }

	//! Read a buffer
	//! \param[out] buffer The data to be copied to
	//! \param[in] cBytes The number of bytes to copy
	virtual void ReadTo(void* buffer, UInt32 cBytes) = 0;

	//! Skip a number of bytes that you would have otherwise 
	//! read. The skip method of InputStream creates a byte 
	//! array and then ReadTo's into it cBytes. It is highly
	//! encouraged that subclasses provide a more efficient 
	//! implementation of this virtual method.
	virtual void Skip(UInt32 cBytes)
	{
		UInt8* mem = new UInt8[cBytes];
		ReadTo(mem, cBytes);
	}

	//! Read a Int8
	MAKO_INLINE Int8  Read8BitInt()
	{ Int8 n; ReadTo(&n, sizeof(Int8)); return n; }

	//! Read a UInt8
	MAKO_INLINE UInt8  Read8BitUInt()
	{ UInt8 n; ReadTo(&n, sizeof(UInt8)); return n; }

	//! Read a Int16
	MAKO_INLINE Int16 Read16BitInt()
	{ Int16 n; ReadTo(&n, sizeof(Int16)); return n; }

	//! Read a UInt16
	MAKO_INLINE UInt16 Read16BitUInt()
	{ UInt16 n; ReadTo(&n, sizeof(UInt16)); return n; }

	//! Read a Int32
	MAKO_INLINE Int32 Read32BitInt()
	{ Int32 n; ReadTo(&n, sizeof(Int32)); return n; }

	//! Read a UInt32
	MAKO_INLINE UInt32 Read32BitUInt()
	{ UInt32 n; ReadTo(&n, sizeof(UInt32)); return n; }
	
	//! Read a Float32
	MAKO_INLINE Float32 Read32BitFloat()
	{ Float32 n; ReadTo(&n, sizeof(Float32)); return n; }

	//! Read a Float64
	MAKO_INLINE Float64 Read64BitFloat()
	{ Float64 n; ReadTo(&n, sizeof(Float32)); return n; }

	//! Shorter way of reading data
	//! \param[out] thing The thing to be read
	template <typename T>
	inline void Read(T& ptr)
	{ ReadTo(static_cast<void*>(&ptr), sizeof(T)); }

	//! Specialization to read a String
	//! \param[out] str The string to read
	template <>
	inline void Read(String& str)
	{
		str.Clear();
		StringChar character;
		while (true)
		{
			ReadTo(&character, sizeof(StringChar));
			if (character == StringChar('\0'))
				break;
			str.Append(character);
		}
	}

	//! This function is for Streams which have a fixed size.
	//! It is discouraged from using this function. If you are
	//! making a new stream type that does have a fixed size 
	//! though, make sure to reimplement this function.
	//! \return The number of bytes in the stream. Since many
	//! Stream types do not have a fixed size, this function
	//! has a default implementation of returning zero.
	virtual UInt32 GetSize() const
	{ return 0; }
};

//! This class is used for writing data. Any data passed to any
//! write method will be written in front of any data previously
//! written by write calls (writing starts at the beginnning of the stream).
class OutputStream : public Stream
{
public:
	MAKO_INLINE STREAM_TYPE GetType() const
	{ return ST_WRITE; }

	//! Store a buffer
	//! \param[in] buffer The data to be copied from
	//! \param[in] cBytes The number of bytes to copy
	virtual void WriteData(const void* buffer, UInt32 cBytes) = 0;

	//! Store a Int8
	MAKO_INLINE void Write8BitInt(Int8 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a UInt8
	MAKO_INLINE void Write8BitUInt(UInt8 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a Int16
	MAKO_INLINE void Write16BitInt(Int16 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a UInt16
	MAKO_INLINE void Write16BitUInt(UInt16 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a Int32
	MAKO_INLINE void Write32BitInt(Int32 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a UInt32
	MAKO_INLINE void Write32BitUInt(UInt32 n)
	{ WriteData(&n, sizeof(n)); }

	//! Store a Float32
	MAKO_INLINE void Write32BitFloat(Float32 n)
	{ WriteData(&n, sizeof(n)); }
	
	//! Store a Float64
	MAKO_INLINE void Write64BitFloat(Float64 n)
	{ WriteData(&n, sizeof(n)); }

	//! Shorter way of writing data
	//! \param[in] ptr The data to be written
	template <typename T>
	MAKO_INLINE void Write(const T& ptr)
	{ WriteData(static_cast<const void*>(&ptr), sizeof(T)); }

	//! Specialization to write a string (includes the '\0')
	//! \param[in] str The string to write to the stream
	template <>
	MAKO_INLINE void Write(const String& str)
	{ WriteData(static_cast<const void*>(str.GetData()), sizeof(StringChar) * (str.GetLength() + 1)); }
};

MAKO_END_NAMESPACE