#pragma once
#include "MakoCommon.h"
#include "MakoGraphicsDevice.h"
#include "MakoVec2d.h"
#include "MakoColor.h"
#include "MakoMeshLoader.h"
#include "MakoFileIO.h"
#include "MakoApplication.h"
#include "MakoOSDevice.h"
#include "MakoMemoryStream.h"
#include "MakoTextureLoader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

MAKO_BEGIN_NAMESPACE

class Material;

class GenericGraphicsDevice : public GraphicsDevice
{
private:
	Color backgroundColor;
	MeshLoader* meshLoaders[MT_ENUM_LENGTH];
	TextureLoader* texLoaders[TT_ENUM_LENGTH];
	//! Directly addresses the transformations of states
	Matrix4f transformations[TS_ENUM_LENGTH];
	bool vsync;
	FT_Library library;
protected:
	MAKO_INLINE Matrix4f& GetTransform(TRANSFORMATION_STATE ts)
	{ return transformations[ts]; }
public:
	GenericGraphicsDevice(bool vsync);
	~GenericGraphicsDevice();

	MAKO_INLINE void SetBackgroundColor(const Color& color)
	{ backgroundColor = color; }

	FT_Library GetFT_Library()
	{ return library; }

	void Draw2dText(const String& text, Font* font, const Pos2d& pos);

	Font* LoadFontFromFile(const FilePath& fileName);
	Font* LoadFont(InputStream* istream);

	Mesh* LoadMeshFromFile(const FilePath& fileName);
	Texture* LoadTextureFromFile(const FilePath& fileName);

	Mesh* LoadMeshFromFile(const FilePath& fileName, MESH_TYPE mt);
	Texture* LoadTextureFromFile(const FilePath& fileName, TEXTURE_TYPE texType);

	MAKO_INLINE const Matrix4f& GetTransform(TRANSFORMATION_STATE ts) const
	{ return transformations[ts]; }
	
	virtual void SetTransform(const Matrix4f& mat, TRANSFORMATION_STATE ts)
	{ transformations[ts] = mat; }

	MAKO_INLINE bool IsVSyncEnabled() const
	{ return vsync; }
};

MAKO_END_NAMESPACE