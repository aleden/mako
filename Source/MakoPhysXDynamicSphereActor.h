#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class PhysXDynamicSphereActor : public PhysXActor
{
private:
	Float32 radius;
public:
	PhysXDynamicSphereActor(PhysXSceneManager* scene,
		                    const Float32 radius,
							const Position3d& pos,
							const Rotation3d& rot);
	MAKO_INLINE ~PhysXDynamicSphereActor();

	bool IsDynamic() const
	{ return true; }
};

MAKO_END_NAMESPACE
#endif