#pragma once
#include "MakoCommon.h"
#include "MakoMap.h"
#include "MakoFont.h"
#include "MakoException.h"
#include "MakoFilePath.h"
#include <ft2build.h>
#include FT_FREETYPE_H

MAKO_BEGIN_NAMESPACE

// Forward declarations
class GenericGraphicsDevice;
class InputStream;

class FreeType2Font : public Font
{
private:
	struct GlyphInfo
	{
		Size2d size;
		Vec2di offset;
	};
private:
	FT_Stream s;
	GenericGraphicsDevice* gd;
	FT_Face face;
	void* fontFileData;
	mutable Map<StringChar, Map<UInt, Texture*> > cachedGlyphs;
	mutable Map<StringChar, Map<UInt, GlyphInfo> > cachedGlyphsInfo;

	FontSize fontSize;
public:
	FreeType2Font(GenericGraphicsDevice* gd, const FilePath& filepath);
	FreeType2Font(GenericGraphicsDevice* gd, InputStream* istream);
	~FreeType2Font();

	Texture* Rasterize(const String& text);

	GraphicsDevice* GetGraphicsDevice();

	MAKO_INLINE FontSize GetSize() const
	{ return fontSize; }

	MAKO_INLINE void SetSize(FontSize size)
	{
		fontSize = size;
		FT_Error err;
		if (err = FT_Set_Pixel_Sizes(face, 0, fontSize))
			throw Exception(Text("FT_Set_Char_Size() failed"), err);
	}

	static unsigned long FTStreamIoFunc(FT_Stream      stream,
		                                unsigned long  offset,
										unsigned char* buffer,
										unsigned long  count);
	
	static void FTStreamCloseFunc(FT_Stream stream);
};

MAKO_END_NAMESPACE