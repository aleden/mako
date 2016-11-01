#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class PhysXStaticPlaneActor : public PhysXActor
{
private:
	Float32 size;

	NxTriangleMesh *triangleMesh;
public:
	PhysXStaticPlaneActor(PhysXSceneManager* scene,
						  Float32 size,
						  const Position3d& pos,
						  const Rotation3d& rot);
	~PhysXStaticPlaneActor();

	bool IsDynamic() const
	{ return false; }
};

MAKO_END_NAMESPACE
#endif