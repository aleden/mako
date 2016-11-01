#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class PhysXStaticBoxActor : public PhysXActor
{
private:
	Vec3df dim;
public:
	PhysXStaticBoxActor(PhysXSceneManager* scene,
				  const Size3d& dim,
				  const Position3d& pos,
				  const Rotation3d& rot);
	~PhysXStaticBoxActor();

	bool IsDynamic() const
	{ return false; }
};

MAKO_END_NAMESPACE
#endif