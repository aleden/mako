#pragma once
#include "MakoEntity3d.h"
#include "MakoScene3d.h"
#include "MakoApplication.h"
#include "MakoMeshManipulator.h"
#include "MakoMesh.h"
#include "MakoMeshData.h"

MAKO_BEGIN_NAMESPACE

struct StaticSphereCreationParams : public Scene3dNodeCreationParams,
									public Physics3dSceneCreationParams,
									public SingleMaterialCreationParams
{
	MAKO_INLINE StaticSphereCreationParams() : radius(1.f) {}
	
	Float32 radius;
};

//! This node represents a graphical box and a
//! box collision on it.
//! This is a very high level class.
class StaticSphere : public Entity3d
{
public:
	MAKO_INLINE StaticSphere(Physics3dScene* p3ds,
		                     Float32 radius,
							 const Position3d& pos,
							 const Rotation3d& rot,
							 Material* material)
							 : Entity3d(APP()->GetMeshManipulator()->MakeSphere(radius),
							   p3ds->AddStaticSphereActor(radius, pos, rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(material); }
	
	MAKO_INLINE ~StaticSphere() {}
};

MAKO_END_NAMESPACE