#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXMemoryBuffer.h"

MAKO_BEGIN_NAMESPACE

PhysXMemoryWriteBuffer::PhysXMemoryWriteBuffer() : currentSize(0), maxSize(0), data(nullptr)
{}

PhysXMemoryWriteBuffer::~PhysXMemoryWriteBuffer()
{
	if(data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}

void PhysXMemoryWriteBuffer::clear()
{
	currentSize = 0;
}

NxStream& PhysXMemoryWriteBuffer::storeByte(NxU8 b)
{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}
NxStream& PhysXMemoryWriteBuffer::storeWord(NxU16 w)
{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}
NxStream& PhysXMemoryWriteBuffer::storeDword(NxU32 d)
{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}
NxStream& PhysXMemoryWriteBuffer::storeFloat(NxReal f)
{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}
NxStream& PhysXMemoryWriteBuffer::storeDouble(NxF64 f)
{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}
NxStream& PhysXMemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
	{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData != nullptr);

		if(data)
		{
			memcpy(newData, data, currentSize);
			delete [] data;
		}
		data = newData;
	}
	memcpy(data + currentSize, buffer, size);
	currentSize += size;
	return *this;
}


PhysXMemoryReadBuffer::PhysXMemoryReadBuffer(const NxU8* data) : buffer(data)
{}

PhysXMemoryReadBuffer::~PhysXMemoryReadBuffer()
{} // We don't own the data => no delete

NxU8 PhysXMemoryReadBuffer::readByte() const
{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

NxU16 PhysXMemoryReadBuffer::readWord() const
{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

NxU32 PhysXMemoryReadBuffer::readDword() const
{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

float PhysXMemoryReadBuffer::readFloat() const
{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
}

double PhysXMemoryReadBuffer::readDouble() const
{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

void PhysXMemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
{
	memcpy(dest, buffer, size);
	buffer += size;
}

MAKO_END_NAMESPACE
#endif