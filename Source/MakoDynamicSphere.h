#pragma once

#include "MakoEntity3d.h"
#include "MakoScene3d.h"
#include "MakoApplication.h"
#include "MakoMeshManipulator.h"
#include "MakoMesh.h"

MAKO_BEGIN_NAMESPACE

struct DynamicSphereCreationParams : public Scene3dNodeCreationParams,
									 public Physics3dSceneCreationParams,
									 public SingleMaterialCreationParams
{
	MAKO_INLINE DynamicSphereCreationParams() : radius(1.f) {}
	
	Float32 radius;
	
};

//! This node represents a graphical box and a
//! box collision on it.
//! This is a very high level class.
class DynamicSphere : public Entity3d
{
public:
	MAKO_INLINE DynamicSphere(Physics3dScene* p3ds,
							  const Float32 radius,
							  const Position3d& pos,
							  const Rotation3d& rot,
							  Material* material)
							  : Entity3d(APP()->GetMeshManipulator()->MakeSphere(radius),
							    p3ds->AddDynamicSphereActor(radius, pos, rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(material); }

	MAKO_INLINE DynamicSphere(const DynamicSphereCreationParams& p)
		: Entity3d(APP()->GetMeshManipulator()->MakeSphere(p.radius),
		  p.p3ds->AddDynamicSphereActor(p.radius, p.pos, p.rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(p.material); }

	MAKO_INLINE ~DynamicSphere() {}
};

MAKO_END_NAMESPACE