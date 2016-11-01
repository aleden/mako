#pragma once
#include "MakoStream.h"
#include <cstring>

MAKO_BEGIN_NAMESPACE

class MemoryInputStream : public InputStream
{
public:
	const Int8* const origPtr;
private:
	const Int8* data;
	UInt32 cBytes;
public:
	//! Constructor
	//! \param[in] data A dynamically allocated array of data.
	//! \param[in] cBytes The number of bytes that data spans across
	MAKO_INLINE MemoryInputStream(const void* data, UInt32 cBytes)
	: data((const Int8*)data), origPtr((const Int8*)data), cBytes(cBytes) {}
	
	//! Deconstructor
	MAKO_INLINE ~MemoryInputStream()
	{ delete [] origPtr; }

	MAKO_INLINE void ReadTo(void* buffer, UInt32 cBytes)
	{ memcpy(buffer, data, cBytes); data += cBytes; }

	MAKO_INLINE UInt32 Tell() const
	{ return origPtr - data; }

	MAKO_INLINE void Seek(UInt32 byte)
	{ data = (origPtr + byte); }

	MAKO_INLINE void Skip(UInt32 cBytes)
	{ data += cBytes; }
	
	MAKO_INLINE UInt32 GetSize() const
	{ return cBytes; }
};

class MemoryOutputStream : public OutputStream
{
private:
	Int8* data;
	Int8* const origPtr;
	UInt32 cBytes;
public:
	//! Constructor
	//! \param[in] data A dynamically allocated array of data.
	//! \param[in] cBytes The number of bytes that data spans across
	MAKO_INLINE MemoryOutputStream(void* data, UInt32 cBytes)
	: data((Int8*)data), origPtr((Int8*)data), cBytes(cBytes) {}
	
	//! Deconstructor
	MAKO_INLINE ~MemoryOutputStream()
	{ delete [] origPtr; }

	MAKO_INLINE void WriteData(const void* buffer, UInt32 cBytes)
	{ memcpy(data, buffer, cBytes); data += cBytes; }

	MAKO_INLINE UInt32 Tell() const
	{ return origPtr - data; }

	MAKO_INLINE void Seek(UInt32 byte)
	{ data = (origPtr + byte); }
	
	MAKO_INLINE UInt32 GetSize() const
	{ return cBytes; }
};

MAKO_END_NAMESPACE