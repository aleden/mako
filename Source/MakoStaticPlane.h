#pragma once

#include "MakoEntity3d.h"
#include "MakoScene3d.h"
#include "MakoApplication.h"
#include "MakoMeshManipulator.h"

MAKO_BEGIN_NAMESPACE

struct StaticPlaneCreationParams : public Scene3dNodeCreationParams,
								   public Physics3dSceneCreationParams,
								   public SingleMaterialCreationParams
{
	MAKO_INLINE StaticPlaneCreationParams() : size(1.f) {}
	Float32 size;
};

//! This node represents a graphical box and a
//! box collision on it.
//! This is a very high level class.
class StaticPlane : public Entity3d
{
public:
	MAKO_INLINE StaticPlane(Physics3dScene* p3ds,
		                    Float32 size,
							const Position3d& pos,
							const Rotation3d& rot,
							Material* material)
							: Entity3d(APP()->GetMeshManipulator()->MakePlane(size),
							  p3ds->AddStaticPlaneActor(size, pos, rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(material); }

	MAKO_INLINE ~StaticPlane() {}
};

MAKO_END_NAMESPACE