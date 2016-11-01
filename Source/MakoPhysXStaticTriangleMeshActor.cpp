#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXStaticTriangleMeshActor.h"
#include "MakoMatrix4.h"
#include "Makomath.h"
#include "MakoPhysXMemoryBuffer.h"
#include "MakoException.h"
#include "MakoString.h"
#include "MakoMesh.h"
#include "MakoMeshData.h"
#include "MakoIndexedMeshData.h"
#include "MakoArrayList.h"
#include "MakoVertex.h"
#include "MakoStandardVertex.h"
#include "MakoT2Vertex.h"

#define VEC3DF_TO_NXVEC3(VEC3DF) NxVec3(VEC3DF.x, VEC3DF.y, VEC3DF.z)

MAKO_BEGIN_NAMESPACE

PhysXStaticTriangleMeshActor::PhysXStaticTriangleMeshActor(PhysXSceneManager* scene,
														   const Mesh* mesh,
														   const Position3d& pos,
														   const Rotation3d& rot,
														   const Scale3d& scale)
														   : PhysXActor(scene, pos, rot)
{
	UInt32 numIndices = 0, numVerts = 0;
	VERTEX_BUFFER_INDEX_TYPE indexType = VBIT_16;
	// Calculate number of triangles, number of verts in mesh
	for (UInt i = 0; i < mesh->GetNumSubMeshes(); ++i)
	{
		if (!mesh->GetSubMesh(i)->IsIndexed() || mesh->GetSubMesh(i)->GetPrimitiveType() != PT_TRIANGLELIST)
			continue;
		const IndexedMeshData* mb = static_cast<const IndexedMeshData*>(mesh->GetSubMesh(i));
		indexType = Max(mb->GetVertexBufferIndexType(), indexType);

		numVerts += mb->GetNumVertices();
		numIndices += mb->GetNumVertexBufferIndices();
	}
	if (numIndices == 0 || numVerts == 0)
	{
		MAKO_DEBUG_BREAK;
		throw Exception(Text("An invalid mesh was supplied to PhysXStaticTriangleMeshActor::PhysXStaticTriangleMeshActor()"));
	}

	// Final verts
	ArrayList<Pos3d> fverts(numVerts);
	// Final indices
	void* findices;
	
	if (indexType == VBIT_16)
		findices = new UInt16[numIndices];
	else
		findices = new UInt32[numIndices];

	UInt32 numVertsHandled = 0, numIndicesHandled = 0;
	for (UInt i = 0; i < mesh->GetNumSubMeshes(); ++i)
	{
		if (!mesh->GetSubMesh(i)->IsIndexed() || mesh->GetSubMesh(i)->GetPrimitiveType() != PT_TRIANGLELIST)
			continue;

		const IndexedMeshData* mb = static_cast<const IndexedMeshData*>(mesh->GetSubMesh(i));

		// Put verts from each meshbuffer into final verts,
		// offset them
		for (UInt ivb = 0; ivb < mb->GetNumVertices(); ++ivb)
		{
			if (mb->GetVertexType() == VT_STANDARD)
				fverts[ivb + numVertsHandled] = ((static_cast<const StandardVertex*>(mb->GetVertices()))[ivb]).GetPosition() * scale;
			else if (mb->GetVertexType() == VT_T2)
				fverts[ivb + numVertsHandled] = ((static_cast<const T2Vertex*>(mb->GetVertices()))[ivb]).GetPosition() * scale;

		}
		// Put indices from each meshbuffer into final indices,
		// offset their places in the buffer AND their actual position
		// with the number of indices handled so far
		for (UInt iib = 0; iib < mb->GetNumVertexBufferIndices(); ++iib)
		{
			if (indexType == VBIT_16)
				((UInt16*)findices)[iib + numIndicesHandled] =(((UInt16*)mb->GetVertexBufferIndices())[iib]) + numVertsHandled;
			else
				((UInt32*)findices)[iib + numIndicesHandled] = (((UInt32*)mb->GetVertexBufferIndices())[iib]) + numVertsHandled;
		}
		
		numVertsHandled   += mb->GetNumVertices();
		numIndicesHandled += mb->GetNumVertexBufferIndices();

	}

	// Build physical model
	NxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.numVertices         = numVerts;    
	triangleMeshDesc.numTriangles        = numIndices/3;
	triangleMeshDesc.pointStrideBytes    = sizeof(Pos3d);
	triangleMeshDesc.triangleStrideBytes = 3 * indexType;
	triangleMeshDesc.points              = static_cast<void*>(&(fverts[0]));    
	triangleMeshDesc.triangles           = findices;
	triangleMeshDesc.flags               = indexType == VBIT_16 ? NX_MF_16_BIT_INDICES : 0;

	////////////////////////////
	// Init cooking
	NxCookingInterface* cook = ((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXCookerInterface();
	cook->NxInitCooking(((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXAllocator(),
		((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXOutputStream());
	
	PhysXMemoryWriteBuffer buf;
	if (!cook->NxCookTriangleMesh(triangleMeshDesc, buf))
	{
		cook->NxCloseCooking();
		delete [] findices;
		throw Exception(Text("NxCookingInterface::NxCookTriangleMesh() failed in") + 
			ToString(__FILE__) + Text(" on line ") + String::From32BitInt(__LINE__)); 
	}
	PhysXMemoryReadBuffer readBuffer(buf.data);
	triangleMesh = ((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXSDK()->createTriangleMesh(readBuffer);

	////////////////////////////
	// Close cooking
	cook->NxCloseCooking();


	// Create triangle mesh instance
	NxTriangleMeshShapeDesc meshShapeDesc;
	meshShapeDesc.meshData = triangleMesh; //See NxTriangleMesh

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&meshShapeDesc);

	SetNxActorDescPosAndRot(actorDesc, pos, rot);

	actor = scene->GetPhysXScene()->createActor(actorDesc);
	actor->userData = static_cast<void*>(this);

	delete [] findices;
}

PhysXStaticTriangleMeshActor::~PhysXStaticTriangleMeshActor()
{
	((PhysXSceneManager*)GetPhysics3dScene())->GetPhysXScene()->releaseActor(*actor);
	((PhysXDevice*)GetPhysics3dScene()->GetPhysics3dDevice())->GetPhysXSDK()->releaseTriangleMesh(*triangleMesh);
}

MAKO_END_NAMESPACE
#endif