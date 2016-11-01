#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class PhysXDynamicBoxActor : public PhysXActor
{
private:
	Size3d dim;
public:
	PhysXDynamicBoxActor(PhysXSceneManager* scene,
		                 const Size3d& dim,
						 const Position3d& pos,
						 const Rotation3d& rot);

	~PhysXDynamicBoxActor();

	bool IsDynamic() const
	{ return true; }
};

MAKO_END_NAMESPACE
#endif