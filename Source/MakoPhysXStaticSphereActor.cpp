#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXStaticSphereActor.h"

MAKO_BEGIN_NAMESPACE

PhysXStaticSphereActor::PhysXStaticSphereActor(PhysXSceneManager* scene,
											   const Float32 radius,
											   const Position3d& pos,
											   const Rotation3d& rot)
											   : radius(radius), PhysXActor(scene, pos, rot)
{
	NxActorDesc actorDesc;

	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = radius;
	
	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.density         = 1.0f;
	
	SetNxActorDescPosAndRot(actorDesc, pos, rot);
	
	actor = scene->GetPhysXScene()->createActor(actorDesc);
	actor->userData = static_cast<void*>(this);
}

PhysXStaticSphereActor::~PhysXStaticSphereActor()
{ ((PhysXSceneManager*)GetPhysics3dScene())->GetPhysXScene()->releaseActor(*actor); }


MAKO_END_NAMESPACE
#endif