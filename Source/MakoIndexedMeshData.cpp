#include "MakoIndexedMeshData.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

IndexedMeshData::IndexedMeshData(GraphicsDevice* gd,
									 const IndexedMeshDataCreationParams& p)
: MeshData(gd, p), vertBufferIndexType(p.vertBufferIndexType),
  numVertBufferIndices(p.numVertBufferIndices), vertBufferIndices(nullptr), hib(nullptr)
{
	vertBufferIndices = new Byte[numVertBufferIndices * vertBufferIndexType];
	memcpy(vertBufferIndices, p.vertBufferIndices, numVertBufferIndices * vertBufferIndexType);

	hib = gd->CreateIndexHardwareBuffer(this);
}

IndexedMeshData::~IndexedMeshData()
{
	if (vertBufferIndices)
		delete [] vertBufferIndices;
	if (hib)
		delete hib;
}

MAKO_END_NAMESPACE