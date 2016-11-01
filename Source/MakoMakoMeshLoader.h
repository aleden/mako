#pragma once

#include "MakoMeshLoader.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class Application;

// Meshloader capable of loading obj meshes.
class MakoMeshLoader : public MeshLoader
{
public:
	MAKO_INLINE MakoMeshLoader() {}
	MAKO_INLINE ~MakoMeshLoader() {}

	bool IsLoadableFileExt(const String& ext) const
	{ return ext == Text("makomesh"); }

	// Loads a wavefront mesh from file
	virtual Mesh* Load(InputStream* stream);
};

MAKO_END_NAMESPACE