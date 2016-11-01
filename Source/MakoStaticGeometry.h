#pragma once

#include "MakoCommon.h"
#include "MakoEntity3d.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Physics3dScene;

struct StaticGeometryCreationParams : public Scene3dNodeCreationParams,
									  public Physics3dSceneCreationParams
{ Mesh* mesh; };

//! This node represents a graphical mesh and a
//! triangle collision on it.
//! This is a very high level class.
class StaticGeometry : public Entity3d
{
public:
	MAKO_INLINE StaticGeometry(Physics3dScene* p3ds,
							   Mesh* mesh,
							   const Position3d& pos,
							   const Rotation3d& rot,
							   const Scale3d& scale)
	: Entity3d(mesh, p3ds->AddStaticTriangleMeshActor(mesh, pos, rot, scale), scale, false)
	{}

	MAKO_INLINE StaticGeometry(const StaticGeometryCreationParams& p)
	: Entity3d(p.mesh, p.p3ds->AddStaticTriangleMeshActor(p.mesh, p.pos, p.rot, p.scale), p.scale, false)
	{}

	MAKO_INLINE ~StaticGeometry() {}
};

MAKO_END_NAMESPACE
