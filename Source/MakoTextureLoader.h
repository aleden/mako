#pragma once
#include "MakoCommon.h"
#include "MakoLoader.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Texture;
class InputStream;

//! This class can load Textures (images) from data
class TextureLoader : public Loader
{
public:
	//! Load a texture from a file
	//! \param[in] filePath The file path of the texture
	virtual Texture* Load(InputStream* stream) = 0;
	
	virtual ~TextureLoader() {}
};

MAKO_END_NAMESPACE