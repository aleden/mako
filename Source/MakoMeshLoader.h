#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoStandardVertex.h"
#include "MakoLoader.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Mesh;
class InputStream;

//! This class can load Meshes from data
class MeshLoader : public Loader
{
public:
	//! Load a static mesh from a file
	//! \param[in] filePath The file path of the mesh
	virtual Mesh* Load(InputStream* stream) = 0;
	
	virtual ~MeshLoader() {}
};

MAKO_END_NAMESPACE