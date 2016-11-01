#pragma once
#include "MakoCommon.h"
#include "MakoReferenceCounted.h"
#include "MakoString.h"
#include "MakoVec2d.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class GraphicsDevice;
class Texture;

typedef UInt FontSize;

struct GlyphMetrics
{
};

class Font : public ReferenceCounted
{
public:
	//! Rasterize text into a texture that can be drawn.
	//! \param[in] text The text to rasterize. If it contains a new line
	//! character ('\n'), it will treat it as a space character (' ').
	//! \param[in] size The font size to rasterize to
	//! \return The texture of the text
	virtual Texture* Rasterize(const String& text) = 0;

	virtual void SetSize(FontSize size) = 0;
	virtual FontSize GetSize() const = 0;
	
	virtual GraphicsDevice* GetGraphicsDevice() = 0;
};

MAKO_END_NAMESPACE