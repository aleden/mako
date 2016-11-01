#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoReferenceCounted.h"
#include "MakoHardwareBuffer.h"
#include "MakoVec2d.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class Texture;

//! This class represents a Texture in hardware memory, which is actually
//! accessed through either the Direct3d or OpenGL api.
class TextureHardwareBuffer : public HardwareBuffer
{
public:
	virtual Texture* GetParent() = 0;

	virtual ~TextureHardwareBuffer() {}
};

//! A color format specifies how color information is stored in memory.
//! The Mako engine uses CF_A8R8G8B8 as default. The colors' order in
//! the enum states the order in memory, regardless of the platform's
//! endianess.
enum COLOR_FORMAT
{
	//! Default 32 bit color format. 8 bits are used for every component:
	//! red, green, blue and alpha. This is the most widely used color 
	//! format of the Mako Engine.
	CF_R8G8B8A8,
	
	CF_ENUM_LENGTH
};

//! Texture resizing algorithms
enum RESIZE_ALGORITHM
{
	RA_BILINEAR,
	RA_NEAREST_NEIGHBOR,
	RA_ENUM_LENGTH
};

//! This function gets the stride for a particular color format, in the 
//! number of bits.
//! \return The stride for the color format, in the number of bits.
MAKO_INLINE UInt32 GetColorStride(COLOR_FORMAT format)
{
	switch (format)
	{
	case CF_R8G8B8A8:
		return 32;
	}
	return 0xFFFFFFFF;
}

struct TextureCreationParams
{
	MAKO_INLINE TextureCreationParams() : data(nullptr), copyFromData(false) {}
	MAKO_INLINE ~TextureCreationParams() {}

	//! Image data for a texture is stored row by row in memory. Therefore,
	//! to access a single pixel, the offset is calculated by:
	//! u8pvar += ((y * colorStride) * width) + (x * colorStride)
	//! It must be in the standard format of the Mako engine (A8R8G8B8 format).
	void* data;
	
	//! The dimensions of the texture.
	Size2d size;

	//! The color format of the data.
	COLOR_FORMAT format;

	//! If set to true, the Texture will copy the memory from data instead of
	//! merely holding the pointer to data and deleting it's contents on 
	//! deconstruction.
	bool copyFromData;
};

// Forward declaration
class GraphicsDevice;

//! A texture map is applied (mapped) to the surface of a shape or polygon. 
//! This process is akin to applying patterned paper to a plain white box.
//! In the Mako engine, a Texture is also used to represent a general
//! purpose image.
class Texture : public ReferenceCounted
{
private:
	GraphicsDevice* gd;
	TextureHardwareBuffer* hardwareTex;

	void* data;
	Size2d size;

	const COLOR_FORMAT format;
	
	const UInt32 bitsPerPixel, bytesPerPixel;

	void ResizePixelsNearestNeighbor(const Size2d& newSize, void* out) const;
public:
	Texture(GraphicsDevice* gd, const TextureCreationParams& params);
	~Texture();

	MAKO_API Texture* CreateStretched(const Size2d& newSize, RESIZE_ALGORITHM alg = RA_NEAREST_NEIGHBOR) const;
	
	MAKO_INLINE const Size2d& GetSize() const
	{ return size; }
	
	MAKO_INLINE GraphicsDevice* GetGraphicsDevice() const
	{ return gd; }

	MAKO_INLINE COLOR_FORMAT GetColorFormat() const
	{ return format; }

	MAKO_INLINE TextureHardwareBuffer* GetTextureHardwareBuffer()
	{ return hardwareTex; }

	MAKO_INLINE const void* GetImageData() const
	{ return data; }

	MAKO_INLINE void* GetImageData()
	{ return data; }

	MAKO_INLINE const void* GetImageDataEnd() const
	{ return (UInt8*)data + (GetSize().x * GetSize().y * GetBytesPerPixel()); }

	MAKO_INLINE void* GetImageDataEnd()
	{ return (UInt8*)data + (GetSize().x * GetSize().y * GetBytesPerPixel()); }

	MAKO_INLINE UInt32 GetBitsPerPixel() const
	{ return bitsPerPixel; }

	MAKO_INLINE UInt32 GetBytesPerPixel() const
	{ return bytesPerPixel; }

	MAKO_INLINE UInt GetNumPixels()
	{ return GetSize().x * GetSize().y; }

	MAKO_INLINE void* GetPixel(UInt x, UInt y) const
	{ return static_cast<void*>(static_cast<UInt8*>(data) + 
	                            (((y * GetBytesPerPixel()) * GetSize().y) + 
								 (x * GetBytesPerPixel()))); }

	MAKO_INLINE void* GetPixel(UInt index) const
	{ return GetPixel(index % GetSize().y, index / GetSize().y); }
};

MAKO_END_NAMESPACE