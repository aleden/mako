#if 0
#pragma once

#include "MakoMeshLoader.h"

MAKO_BEGIN_NAMESPACE

class Application;

// Meshloader capable of loading obj meshes.
class ObjMeshLoader : public MeshLoader
{
private:
	Application* app;
public:
	ObjMeshLoader(Application* app);
	virtual ~ObjMeshLoader();

	// Loads a wavefront mesh from file
	virtual Mesh* Load(InputStream* stream);

private:
	void GoNextLine(const Int8*& buf, const Int8* const bufEnd);
	void GoFirstWord(const Int8*& buf, const Int8* const bufEnd, bool acrossNewlines = true);
	void GoAndCopyNextWord(Int8* outBuf, const Int8*& inBuf, UInt32 outBufLength, const Int8* const pBufEnd);
	Vec3df ReadVec3df(const Int8*& bufPtr, const Int8* const bufEnd);
	Vec2df ReadVec2df(const Int8*& bufPtr, const Int8* const bufEnd);
	// Skip current word and stop at beginning of next one
	void GoNextWord(const Int8*& buf, const Int8* const bufEnd, bool acrossNewlines = true);
	// Copies the current word from the inBuf to the outBuf
	UInt32 CopyWord(Int8* outBuf, const Int8* const inBuf, UInt32 outBufLength, const Int8* const bufEnd);
};

MAKO_END_NAMESPACE
#endif