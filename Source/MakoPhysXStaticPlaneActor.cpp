#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXStaticPlaneActor.h"
#include "MakoMatrix4.h"
#include "Makomath.h"
#include "MakoPhysXMemoryBuffer.h"
#include "MakoException.h"
#include "MakoString.h"

#define VEC3DF_TO_NXVEC3(VEC3DF) NxVec3(VEC3DF.x, VEC3DF.y, VEC3DF.z)

MAKO_BEGIN_NAMESPACE

PhysXStaticPlaneActor::PhysXStaticPlaneActor(PhysXSceneManager* scene,
											 Float32 size,
											 const Position3d& pos,
											 const Rotation3d& rot)
											 : PhysXActor(scene, pos, rot)
{
	this->size = size;
	/*1------------------2*\
	|                    |
	|                    |
	|                    |
	|                    |
	|                    |
	|                    |
	\*0------------------3*/
	NxVec3 verts[4];
	verts[0] = NxVec3(-size/2, 0, -size/2);
	verts[1] = NxVec3(-size/2, 0,  size/2);
	verts[2] = NxVec3( size/2, 0,  size/2);
	verts[3] = NxVec3( size/2, 0, -size/2);

	NxU16 indices[6];
	indices[0] = 1;
	indices[1] = 2;
	indices[2] = 0;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	NxCookingInterface* cook = ((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXCookerInterface();
	cook->NxInitCooking(((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXAllocator(),
		((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXOutputStream());

	// Build physical model
	NxTriangleMeshDesc triangleMeshDesc; //  mesh description
	triangleMeshDesc.numVertices         = 4;    
	triangleMeshDesc.numTriangles        = 2;
	triangleMeshDesc.pointStrideBytes    = sizeof(NxVec3);   
	triangleMeshDesc.triangleStrideBytes = 3 * sizeof(NxU16);    
	triangleMeshDesc.points              = (void*)verts;    
	triangleMeshDesc.triangles           = (void*)indices;    
	triangleMeshDesc.flags               = 0;
	triangleMeshDesc.flags              |= NX_MF_16_BIT_INDICES;

	PhysXMemoryWriteBuffer buf;
	if (!cook->NxCookTriangleMesh(triangleMeshDesc, buf))
	{
		cook->NxCloseCooking();
		throw Exception(Text("NxCookingInterface::NxCookTriangleMesh() failed in") + 
			ToString(__FILE__) + Text(" on line ") + String::From32BitInt(__LINE__)); 
	}
	PhysXMemoryReadBuffer readBuffer(buf.data);
	triangleMesh = ((PhysXDevice*)scene->GetPhysics3dDevice())->GetPhysXSDK()->createTriangleMesh(readBuffer);
	
	cook->NxCloseCooking();

	//create triangle mesh instance
	NxTriangleMeshShapeDesc meshShapeDesc;
	meshShapeDesc.meshData = triangleMesh; //See NxTriangleMesh

	NxActorDesc actorDesc;
	actorDesc.shapes.pushBack(&meshShapeDesc);

	SetNxActorDescPosAndRot(actorDesc, pos, rot);

	//actorDesc.userData = physXActorUserData;
	actor = scene->GetPhysXScene()->createActor(actorDesc);
	actor->userData = static_cast<void*>(this);
}

PhysXStaticPlaneActor::~PhysXStaticPlaneActor()
{
	((PhysXSceneManager*)GetPhysics3dScene())->GetPhysXScene()->releaseActor(*actor);
	((PhysXDevice*)GetPhysics3dScene()->GetPhysics3dDevice())->GetPhysXSDK()->releaseTriangleMesh(*triangleMesh);
}

MAKO_END_NAMESPACE
#endif