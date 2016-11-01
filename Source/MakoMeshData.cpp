#include "MakoMeshData.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
MeshData::MeshData(GraphicsDevice* gd, const MeshDataCreationParams& p)
: numVertices(p.numVertices), vertexType(p.vertexType), numPrimitives(p.numPrimitives), 
  primitiveType(p.primitiveType), flags(p.flags), vertices(nullptr), hvb(nullptr),
  subMaterials(p.materials)
{
	vertices = new Byte[numVertices * vertexType];
	memcpy(vertices, p.vertices, numVertices * vertexType);

	if (subMaterials.empty())
	{
		subMaterials[0] = gd->GetDefaultMaterial();
		subMaterials[0]->Hold();
	}
	else
	{
		typedef Map<UInt32, Material*>::iterator SubMtlsIt;
		for (SubMtlsIt it = subMaterials.begin(); it != subMaterials.end(); ++it)
			(*it).second->Hold();
	}

	hvb = gd->CreateVertexHardwareBuffer(this);
}

MeshData::~MeshData()
{
	typedef Map<UInt32, Material*>::iterator SubMtlsIt;
	for (SubMtlsIt it = subMaterials.begin(); it != subMaterials.end(); ++it)
		(*it).second->Drop();
	
	if (vertices)
		delete [] vertices;

	if (hvb)
		delete hvb;
}

////////////////////////////////////////////////////////////////////////////////////////
// Member functions
void MeshData::SetMaterial(Material* m)
{
	typedef Map<UInt32, Material*>::iterator SubMtlsIt;
	for (SubMtlsIt it = subMaterials.begin(); it != subMaterials.end(); ++it)
		(*it).second->Drop();
	subMaterials.clear();

	m->Hold();
	subMaterials[0] = m;
}

MAKO_END_NAMESPACE