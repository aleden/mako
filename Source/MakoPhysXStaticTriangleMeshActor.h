#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class PhysXStaticTriangleMeshActor : public PhysXActor
{
private:
	NxTriangleMesh *triangleMesh;
public:
	PhysXStaticTriangleMeshActor(PhysXSceneManager* scene,
		                         const Mesh* mesh,
								 const Position3d& pos,
								 const Rotation3d& rot,
								 const Scale3d& scale);
	~PhysXStaticTriangleMeshActor();

	bool IsDynamic() const
	{ return false; }
};

MAKO_END_NAMESPACE
#endif