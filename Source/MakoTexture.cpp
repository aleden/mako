#include "MakoTexture.h"
#include "MakoGraphicsDevice.h"
#include "MakoException.h"
#include "MakoMath.h"

MAKO_BEGIN_NAMESPACE

Texture::Texture(GraphicsDevice* gd, const TextureCreationParams& params)
: gd(gd), size(params.size), format(params.format), 
  bitsPerPixel(GetColorStride(format)), bytesPerPixel(bitsPerPixel / 8)
{
	if (params.copyFromData)
	{
		data = new Byte[bytesPerPixel * size.x * size.y];
		memcpy(data, params.data, bytesPerPixel * size.x * size.y);
	}
	else
	{
		data = params.data;
	}

	hardwareTex = gd->CreateTextureHardwareBuffer(this);
}


Texture::~Texture()
{
	delete hardwareTex;
	delete [] data;
}

void Texture::ResizePixelsNearestNeighbor(const Size2d& newSize, void* out) const
{
	for (UInt x = 0; x < newSize.x; ++x)
	{
		for (UInt y = 0; y < newSize.y; ++y)
		{
			Float32 fracx = static_cast<Float32>(x) / static_cast<Float32>(newSize.x);
			Float32 fracy = static_cast<Float32>(y) / static_cast<Float32>(newSize.y);

			switch (GetColorFormat())
			{
			case CF_R8G8B8A8:
				{
					UInt offset = static_cast<UInt>(fracy*static_cast<Float32>(size.y))*size.x +
						          static_cast<UInt>(fracx*static_cast<Float32>(size.x));
					(static_cast<UInt32*>(out))[y*newSize.x + x] = static_cast<const UInt32*>(GetImageData())[offset];
					break;
				}
			default:
				{
					throw Exception(Text("Failed to identify color format in Texture::CreateStretched()."));
				}
			}
		}
	}
}

Texture* Texture::CreateStretched(const Size2d& newSize, RESIZE_ALGORITHM alg) const
{
	Byte* newData = new Byte[GetBytesPerPixel() * newSize.x * newSize.y];

	TextureCreationParams tp;
	tp.copyFromData = false;
	tp.data = newData;
	tp.format = format;
	tp.size = newSize;

	switch (alg)
	{
	case RA_BILINEAR:
		throw Exception(Text("RA_BILINEAR NOT SUPPORTED!"));
		break;
	case RA_NEAREST_NEIGHBOR:
		ResizePixelsNearestNeighbor(newSize, newData);
		break;
	}
	return gd->CreateTexture(tp);
}

MAKO_END_NAMESPACE