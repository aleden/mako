#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDynamicSphereActor.h"

MAKO_BEGIN_NAMESPACE

PhysXDynamicSphereActor::PhysXDynamicSphereActor(PhysXSceneManager* scene,
												 const Float32 radius,
												 const Position3d& pos,
												 const Rotation3d& rot)
												 : radius(radius), PhysXActor(scene, pos, rot)
{
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.2f;
	
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = radius;
	
	actorDesc.shapes.pushBack(&sphereDesc);
	actorDesc.body            = &bodyDesc;
	actorDesc.density         = 1.0f;
	
	SetNxActorDescPosAndRot(actorDesc, pos, rot);
	
	actor = scene->GetPhysXScene()->createActor(actorDesc);
	actor->userData = static_cast<void*>(this);
}

PhysXDynamicSphereActor::~PhysXDynamicSphereActor()
{ ((PhysXSceneManager*)GetPhysics3dScene())->GetPhysXScene()->releaseActor(*actor); }


MAKO_END_NAMESPACE
#endif