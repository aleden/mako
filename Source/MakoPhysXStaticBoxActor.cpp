#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXStaticBoxActor.h"
#include "MakoMatrix4.h"

MAKO_BEGIN_NAMESPACE

PhysXStaticBoxActor::PhysXStaticBoxActor(PhysXSceneManager* scene,
										 const Size3d& dim,
										 const Position3d& pos,
										 const Rotation3d& rot)
							 : PhysXActor(scene, pos, rot), dim(dim)
{
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.angularDamping = 0.2f;

	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(dim.x/2, dim.y/2, dim.z/2);
	
	actorDesc.shapes.pushBack(&boxDesc);
	actorDesc.density = .65f;

	SetNxActorDescPosAndRot(actorDesc, pos, rot);

	actor = scene->GetPhysXScene()->createActor(actorDesc);
	actor->userData = static_cast<void*>(this);
}

PhysXStaticBoxActor::~PhysXStaticBoxActor()
{ ((PhysXSceneManager*)GetPhysics3dScene())->GetPhysXScene()->releaseActor(*actor); }

MAKO_END_NAMESPACE
#endif