#include "MakoGenericGraphicsDevice.h"
#include "MakoFileIO.h"
#include "MakoObjMeshLoader.h"
#include "MakoMakoMeshLoader.h"
#include "MakoTextureLoader.h"
#include "MakoColorMtl.h"
#include "MakoException.h"
#include "MakoConsole.h"
#include "MakoFileStream.h"
#include "MakoJPEGLoader.h"
#include "MakoPNGLoader.h"
#include "MakoException.h"
#include "MakoFreeType2Font.h"
#include "MakoFileStream.h"
#include "MakoFileSystem.h"

MAKO_BEGIN_NAMESPACE

GenericGraphicsDevice::GenericGraphicsDevice(bool vsync)
: vsync(vsync)
{
#if 0
	meshLoaders[MT_OBJ] = new ObjMeshLoader(APP());
#endif
	meshLoaders[MT_MAKOMESH] = new MakoMeshLoader();

	texLoaders[TT_JPEG] = new JPEGLoader;
	texLoaders[TT_PNG] = new PNGLoader;

	if (FT_Init_FreeType(&library))
	{
		throw Exception(Text("FT_Init_FreeType() failed in \
							 GenericGraphicsDevice::GenericGraphicsDevice()"));
	}
}

GenericGraphicsDevice::~GenericGraphicsDevice()
{
	for (UInt i = 0; i < MT_ENUM_LENGTH; ++i)
		delete meshLoaders[i];

	for (UInt i = 0; i < TT_ENUM_LENGTH; ++i)
		delete texLoaders[i];

	if (FT_Done_FreeType(library))
		throw Exception(Text("FT_Done_FreeType() failed"));
}

Mesh* GenericGraphicsDevice::LoadMeshFromFile(const FilePath& fileName, MESH_TYPE mt)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The mesh file [") + fileName.GetAbs() + Text("] does not exist"));

	FileInputStream* file = new FileInputStream(found);
	file->Hold();
	Mesh* m = meshLoaders[mt]->Load(file);
	file->Drop();

	APP()->GetConsole()->Log(LL_LOW, Text("Loaded mesh [") + found.GetAbs() + StringChar(']'));
	return m;
}

Texture* GenericGraphicsDevice::LoadTextureFromFile(const FilePath& fileName, TEXTURE_TYPE texType)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The texture file [") + fileName.GetAbs() + Text("] does not exist"));

	FileInputStream* file = new FileInputStream(found);
	file->Hold();
	Texture* t = texLoaders[texType]->Load(file);
	file->Drop();

	APP()->GetConsole()->Log(LL_LOW, Text("Loaded texture [") + found.GetAbs() + StringChar(']'));
	return t;
}

Mesh* GenericGraphicsDevice::LoadMeshFromFile(const FilePath& fileName)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The mesh file [") + found.GetAbs() + Text("] does not exist"));

	MESH_TYPE mt = (MESH_TYPE)0xFFFFFFFF;
	String ext = fileName.GetExt();
	ext.LowerCase();

	for (UInt i = 0; i < TT_ENUM_LENGTH; ++i)
	{
		if (meshLoaders[i]->IsLoadableFileExt(ext))
		{
			mt = static_cast<MESH_TYPE>(i);
			break;
		}
	}

	if (mt == 0xFFFFFFFF)
		throw Exception(Text("The file type [") + ext + Text("] is not supported"));

	FileInputStream* file = new FileInputStream(found);
	file->Hold();
	Mesh* m = meshLoaders[mt]->Load(file);
	file->Drop();

	APP()->GetConsole()->Log(LL_LOW, Text("Loaded mesh [") + found.GetAbs() + StringChar(']'));
	return m;
}

Texture* GenericGraphicsDevice::LoadTextureFromFile(const FilePath& fileName)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The texture file [") + fileName.GetAbs() + Text("] does not exist"));

	TEXTURE_TYPE tt = (TEXTURE_TYPE)0xFFFFFFFF;
	String ext = fileName.GetExt();
	ext.LowerCase();

	for (UInt i = 0; i < TT_ENUM_LENGTH; ++i)
	{
		if (texLoaders[i]->IsLoadableFileExt(ext))
		{
			tt = static_cast<TEXTURE_TYPE>(i);
			break;
		}
	}

	if (tt == 0xFFFFFFFF)
		throw Exception(Text("The file type [") + ext + Text("] is not supported"));

	FileInputStream* file = new FileInputStream(found);
	file->Hold();
	Texture* t = texLoaders[tt]->Load(file);
	file->Drop();

	APP()->GetConsole()->Log(LL_LOW, Text("Loaded texture [") + found.GetAbs() + StringChar(']'));
	return t;
}

Font* GenericGraphicsDevice::LoadFontFromFile(const FilePath& fileName)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The font file [") + fileName.GetAbs() + Text("] does not exist"));

	APP()->GetConsole()->Log(LL_LOW, Text("Loaded font [") + found.GetAbs() + StringChar(']'));

	return new FreeType2Font(this, found); //new FileInputStream(filepath));
}

Font* GenericGraphicsDevice::LoadFont(InputStream* istream)
{
	return new FreeType2Font(this, istream);
}

void GenericGraphicsDevice::Draw2dText(const String& text,
									   Font* font,
									   const Pos2d& pos)
{
	Pos2d currpos(pos);
	UInt32 cc = 0; // char count
	for (const StringChar* p = text.GetData(); p != (text.GetData() + text.GetLength() + 1); ++p)
	{
		switch (*p)
		{
		case '\0':
		case '\n':
			{
				if (cc == 0)
					break;

				Texture* t = font->Rasterize(String::FromData(p - cc, cc));
				t->Hold();

				Draw2dTexture(currpos, t, 0.f);

				currpos.y += 15;
				cc = 0;
				t->Drop();
				break;
			}
		default:
			++cc;
		}
	}
}

MAKO_END_NAMESPACE