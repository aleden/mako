#include "MakoFreeType2Font.h"
#include "MakoGenericGraphicsDevice.h"
#include "MakoTexture.h"
#include <cstdio>

MAKO_BEGIN_NAMESPACE

FreeType2Font::FreeType2Font(Mako::GenericGraphicsDevice *gd, const FilePath& filepath)
: gd(gd), fontSize(12)
{
	FILE* file;
	UInt32 filesize;
	
	// Open file
	file = _wfopen(filepath.GetAbs().ToWStringData(), L"rb");
	
	// Get file size
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	
	// Get the data
	fontFileData = new Byte[filesize];
	fread(fontFileData, filesize, 1, file);
	
	FT_Error err;
	if (err = FT_New_Memory_Face(gd->GetFT_Library(), (FT_Byte*)fontFileData,
		filesize, 0, &face))
		throw Exception(Text("FT_New_Memory_Face() failed"), err);

	if (err = FT_Set_Pixel_Sizes(face, 0, fontSize))
		throw Exception(Text("FT_Set_Char_Size() failed"), err);
}

FreeType2Font::FreeType2Font(Mako::GenericGraphicsDevice *gd, InputStream* istream)
: gd(gd), fontFileData(nullptr), fontSize(12)
{
	istream->Hold();
	/*
   *   base ::
   *     For memory-based streams, this is the address of the first stream
   *     byte in memory.  This field should always be set to NULL for
   *     disk-based streams.
   *
   *   size ::
   *     The stream size in bytes.
   *
   *   pos ::
   *     The current position within the stream.
   *
   *   descriptor ::
   *     This field is a union that can hold an integer or a pointer.  It is
   *     used by stream implementations to store file descriptors or `FILE*'
   *     pointers.
   *
   *   pathname ::
   *     This field is completely ignored by FreeType.  However, it is often
   *     useful during debugging to use it to store the stream's fileName
   *     (where available).
   *
   *   read ::
   *     The stream's input function.
   *
   *   close ::
   *     The stream;s close function.
   *
   *   memory ::
   *     The memory manager to use to preload frames.  This is set
   *     internally by FreeType and shouldn't be touched by stream
   *     implementations.
   *
   *   cursor ::
   *     This field is set and used internally by FreeType when parsing
   *     frames.
   *
   *   limit ::
   *     This field is set and used internally by FreeType when parsing
   *     frames.
   */
	UInt8* dat = new UInt8[istream->GetSize()];

	FT_StreamRec stream;
	stream.base               = dat;
	stream.close              = FTStreamCloseFunc;
	stream.descriptor.pointer = istream;
	stream.pos                = 0;
	stream.read               = FTStreamIoFunc;
	stream.size               = istream->GetSize();

	FT_Open_Args openArgs;
	openArgs.memory_base = nullptr;
	openArgs.memory_size = 0;
	openArgs.stream      = &stream;
	openArgs.flags       = FT_OPEN_STREAM;
	openArgs.pathname    = nullptr;
	
	FT_Error err;
 	if (err = FT_Open_Face(gd->GetFT_Library(), &openArgs, 0, &face))
		throw Exception(Text("FT_Open_Face() failed"), err);

	istream->Drop();

	if (err = FT_Set_Pixel_Sizes(face, 0, fontSize))
		throw Exception(Text("FT_Set_Char_Size() failed"), err);
}

FreeType2Font::~FreeType2Font()
{
	typedef Map<StringChar, Map<UInt, Texture*> >::iterator GlyphsIt;
	for (GlyphsIt it = cachedGlyphs.begin(); it != cachedGlyphs.end(); ++it)
	{
		typedef Map<UInt, Texture*>::iterator TexsIt;
		for (TexsIt it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
		{
			(*it2).second->Drop();
		}
	}
	FT_Done_Face(face);
	delete [] fontFileData;
}

Texture* FreeType2Font::Rasterize(const String& text)
{
	TextureCreationParams texparams;
	texparams.size.x = 0;
	texparams.size.y = 0;
	texparams.format = CF_R8G8B8A8;

	for (UInt i = 0; i < text.GetLength(); ++i)
	{
		FT_Error err;

		FT_GlyphSlot slot = face->glyph;

		// load glyph image into the slot (erase previous one)
		if (err = FT_Load_Char(face, text[i], FT_LOAD_DEFAULT))
			throw Exception(Text("FT_Load_Char() failed"), err);

		texparams.size.x += slot->advance.x/64;
		texparams.size.y = Max(texparams.size.y, (UInt)slot->metrics.height/64);
	}

	// Allocate image data
	UInt32* data = new UInt32[texparams.size.x * texparams.size.y];
	texparams.data = static_cast<void*>(data);
	memset(data, 0, sizeof(UInt32*) * texparams.size.x * texparams.size.y);

	// Position of pen
	Int penX = 0, penY = 0;
	for (UInt i = 0; i < text.GetLength(); ++i)
	{
		FT_Error err;

		FT_GlyphSlot slot = face->glyph;

		// load glyph image into the slot (erase previous one)
		if (err = FT_Load_Char(face, text[i], FT_LOAD_RENDER))
			throw Exception(Text("FT_Load_Char() failed"), err);

		penX += slot->bitmap_left;
		//penY -= (face->bbox.yMax/64 - slot->bitmap_top)/2;

		UInt colorStride = GetColorStride(CF_R8G8B8A8)/8;
		for (UInt x = 0; x < static_cast<UInt>(slot->bitmap.width); ++x)
		{
			for (UInt y = 0; y < static_cast<UInt>(slot->bitmap.rows); ++y)
			{
				UInt datax = x + penX;
				UInt datay = y + penY;
				if (datax >= texparams.size.x || datay >= texparams.size.y)
					continue;
				UInt dataOffset = datay*texparams.size.x + datax;
				UInt bmpOffset  = y*(slot->bitmap.width) + x;

				unsigned char val = slot->bitmap.buffer[bmpOffset];

				data[dataOffset] = (val << 24) | (val << 16) | (val << 8) | (val);
			}
		}
		penX -= slot->bitmap_left;
		//penY += (face->bbox.yMax/64 - slot->bitmap_top)/2;

		penX += slot->advance.x/64;
		penY += slot->advance.y/64;

	}


	return gd->CreateTexture(texparams);
}

GraphicsDevice* FreeType2Font::GetGraphicsDevice()
{ return gd; }

unsigned long FreeType2Font::FTStreamIoFunc(FT_Stream      stream,
											unsigned long  offset,
											unsigned char* buffer,
											unsigned long  count)
{
	if (count == 0)
		return 0;
	InputStream* istream = (InputStream*)stream->descriptor.pointer;
	istream->ReadTo(buffer, count);
	return count;
}

void FreeType2Font::FTStreamCloseFunc(FT_Stream stream)
{}

#if 0
const Size2d& FreeType2Font::GetGlyphSize(const StringChar& glyph,
										  const UInt fontSize) const
{
	if (cachedGlyphsInfo[glyph][fontSize].size != 0)
		return cachedGlyphsInfo[glyph][fontSize].size;

	FT_Error err;
	
	if (err = FT_Set_Char_Size(face, 50 * fontSize, 50 * fontSize, 100, 0))
		throw Exception(Text("FT_Set_Char_Size() failed"), err);

	FT_GlyphSlot slot = face->glyph;

	// load glyph image into the slot (erase previous one)
	if (err = FT_Load_Char(face, glyph, FT_LOAD_DEFAULT))
		throw Exception(Text("FT_Load_Char() failed"), err);

	GlyphInfo& gi = cachedGlyphsInfo[glyph][fontSize];
	gi.offset =	Vec2di(0, -slot->bitmap_top);
	gi.size = Size2d(slot->bitmap.width, slot->bitmap.rows);
	return gi.size;
}


Texture* FreeType2Font::GetGlyphTexture(const StringChar& glyph,
										const UInt fontSize) const
{
	if (cachedGlyphs[glyph][fontSize])
		return cachedGlyphs[glyph][fontSize];
	
	FT_Error err;
	
	if (err = FT_Set_Char_Size(face, 50 * fontSize, 0, 100, 0))
		throw Exception(Text("FT_Set_Char_Size() failed"), err);

	FT_GlyphSlot slot = face->glyph;

	// load glyph image into the slot (erase previous one)
	if (err = FT_Load_Char(face, glyph, FT_LOAD_RENDER))
		throw Exception(Text("FT_Load_Char() failed"), err);

	GlyphInfo& gi = cachedGlyphsInfo[glyph][fontSize];
	if (gi.size == 0)
	{
		gi.size = Size2d(slot->bitmap.width, slot->bitmap.rows);
		gi.offset = Vec2di(0, -slot->bitmap_top);
	}

	UInt colorStride = GetColorStride(CF_R8G8B8A8)/8;
	UInt8* data = new UInt8[colorStride *
		                    slot->bitmap.width *
							slot->bitmap.rows];
	for (UInt x = 0; x < static_cast<UInt>(slot->bitmap.width); ++x)
	{
		for (UInt y = 0; y < static_cast<UInt>(slot->bitmap.rows); ++y)
		{
			UInt dataOffset = ((y * colorStride) * slot->bitmap.width) + (x * colorStride);
			UInt bmpOffset  = (y * slot->bitmap.width + x);
			//memcpy(data + dataOffset, slot->bitmap.buffer + bmpOffset, 4);
			data[dataOffset + 0] = slot->bitmap.buffer[bmpOffset];
			data[dataOffset + 1] = slot->bitmap.buffer[bmpOffset];
			data[dataOffset + 2] = slot->bitmap.buffer[bmpOffset];
			data[dataOffset + 3] = slot->bitmap.buffer[bmpOffset];
		}
	}

	//slot->bitmap
	TextureCreationParams texparams;
	texparams.data   = data;
	texparams.size.x = slot->bitmap.width;
	texparams.size.y = slot->bitmap.rows;
	texparams.format = CF_R8G8B8A8;
	Texture* t = cachedGlyphs[glyph][fontSize] = gd->CreateTexture(texparams);
	t->Hold();
	return t;
}

const Vec2di& FreeType2Font::GetGlyphOffset(const StringChar& glyph,
											 UInt fontSize) const
{
	GlyphInfo& gi = cachedGlyphsInfo[glyph][fontSize];
	if (gi.size == 0)
	{
		FT_Error err;


		if (err = FT_Set_Char_Size(face, 50 * fontSize, 0, 100, 0))
			throw Exception(Text("FT_Set_Char_Size() failed"), err);

		FT_GlyphSlot slot = face->glyph;

		// load glyph image into the slot (erase previous one)
		if (err = FT_Load_Char(face, glyph, FT_LOAD_DEFAULT))
			throw Exception(Text("FT_Load_Char() failed"), err);

		gi.size = Size2d(slot->bitmap.width, slot->bitmap.rows);
		gi.offset = Vec2di(0, -slot->bitmap_top);
	}

	return gi.offset;
}

#endif

MAKO_END_NAMESPACE