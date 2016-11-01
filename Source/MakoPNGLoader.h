#pragma once
#include "MakoCommon.h"
#include "MakoTextureLoader.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class Application;

class PNGLoader : public TextureLoader
{
public:
	MAKO_INLINE PNGLoader() {}
	MAKO_INLINE ~PNGLoader() {}

	bool IsLoadableFileExt(const String& ext) const
	{ return ext == Text("png"); }

	Texture* Load(InputStream* stream);
};

MAKO_END_NAMESPACE