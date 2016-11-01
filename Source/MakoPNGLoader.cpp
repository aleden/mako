#include "MakoPNGLoader.h"
#include "MakoStream.h"
#include "MakoTexture.h"
#include "MakoException.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include <png.h>

MAKO_BEGIN_NAMESPACE

// PNG function for error handling
static void png_cpexcept_error(png_structp png_ptr, png_const_charp msg)
{
	longjmp(png_ptr->jmpbuf, 1);
}

// PNG function for file reading
void PNGAPI user_read_data_fcn(png_structp png_ptr, png_bytep data, png_size_t length)
{
	InputStream* is = static_cast<InputStream*>(png_ptr->io_ptr);
	is->ReadTo(data, length);
}

// load in the image data
Texture* PNGLoader::Load(InputStream* stream)
{
	//Used to point to image rows
	UInt8** rowPointers = nullptr;
	UInt8* data         = nullptr;
	UInt8* data2        = nullptr;


	png_byte buffer[8];
	
	// Read the first few bytes of the PNG file
	stream->ReadTo(buffer, 8);

	// Check if it really is a PNG file
	if(png_sig_cmp(buffer, 0, 8))
	{
		// not really a png
		return 0;
	}

	// Allocate the png read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, (png_error_ptr)png_cpexcept_error, NULL);
	if (!png_ptr)
	{
		// Internal PNG create read struct failure
		return 0;
	}

	// Allocate the png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		// Internal PNG create info struct failure
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		delete [] rowPointers;
		delete [] data;
		delete [] data2;
		
		throw Exception(Text("An error occured in PNGLoader::Load()"));
	}

	// changed by zola so we don't need to have public FILE pointers
	png_set_read_fn(png_ptr, stream, user_read_data_fcn);

	png_set_sig_bytes(png_ptr, 8); // Tell png that we read the signature

	png_read_info(png_ptr, info_ptr); // Read the info section of the png file

	UInt32 width;
	UInt32 height;
	Int32 bitDepth;
	Int32 colorType;
	{
		// Use temporary variables to avoid passing casted pointers
		png_uint_32 w,h;
		
		// Extract info
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bitDepth, &colorType, NULL, NULL, NULL);
		
		width  = w;
		height = h;
	}

	// Convert palette color to true color
	if (colorType == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// Convert low bit colors to 8 bit colors
	if (bitDepth < 8)
	{
		if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_1_2_4_to_8(png_ptr);
		else
			png_set_packing(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// Convert high bit colors to 8 bit colors
	if (bitDepth == 16)
		png_set_strip_16(png_ptr);

	// Convert gray color to true color
	if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// Update the changes
	png_read_update_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height,
		&bitDepth, &colorType, NULL, NULL, NULL);

	// Convert RGBA to BGRA
	if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
	{
#if MAKO_ENDIAN == MAKO_LITTLE_ENDIAN
		png_set_bgr(png_ptr);
#else
		png_set_swap_alpha(png_ptr);
#endif
	}

	// Update the changes
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height,
		&bitDepth, &colorType, NULL, NULL, NULL);

#if 0
	// Create the image structure to be filled by png data
	if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
		;//$$ image = new CImage(ECF_A8R8G8B8, core::dimension2d<Int32>(width, height));
	else
		;//$$image = new CImage(ECF_R8G8B8, core::dimension2d<Int32>(width, height));
	if (nullptr /*!image*/)
	{
		// Internal PNG create image struct failure
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}
#endif

	// Create array of pointers to rows in image data
	rowPointers = new png_bytep[height];
	if (!rowPointers)
	{
		// Internal PNG create row pointers failure
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return 0;
	}

	UInt colorStride;
	if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		colorStride = GetColorStride(CF_R8G8B8A8) / 8;
		data = new UInt8[height * width * colorStride];
	}
	else
	{
		colorStride = 3;
		data = new UInt8[height * width * colorStride];
	}
	// Fill array of pointers to rows in image data
	UInt8* dataptr = data;
	for (UInt32 i = 0; i < height; ++i)
	{
		rowPointers[i] = dataptr;
		dataptr += (width * colorStride);
	}

	// for proper error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete [] rowPointers;
		delete [] data;
		delete [] data2;
		
		throw Exception(Text("An error occured in PNGLoader::Load()"));
	}

	// Read data using the library function that handles all transformations including interlacing
	png_read_image(png_ptr, rowPointers);

	png_read_end(png_ptr, NULL);
	
	if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		data2 = data;
	}
	else
	{
		UInt colorStride2 = GetColorStride(CF_R8G8B8A8) / 8;
		data2 = new UInt8[height * width * colorStride2];

		for (UInt8* ptr = data2, *ptr2 = data;
			ptr != (data2 + (height * width * (32 / 8)));
			ptr += (32 / 8), ptr2 += (24 / 8))
		{
			// Copy the rgb values
			memcpy(ptr, ptr2, sizeof(UInt8) * 3);

			// Set the alpha to 255.
			ptr[3] = 255;
		}
	}
	TextureCreationParams texParams;
	texParams.format = CF_R8G8B8A8;
	texParams.size.x = width;
	texParams.size.y = height;
	texParams.data   = data2;

	Texture* texture = APP()->GD()->CreateTexture(texParams);

	// Clean up
	delete [] rowPointers;
	if (colorType != PNG_COLOR_TYPE_RGB_ALPHA)
		delete [] data;
	png_destroy_read_struct(&png_ptr,&info_ptr, 0); // Clean up memory

	return texture;
}

MAKO_END_NAMESPACE