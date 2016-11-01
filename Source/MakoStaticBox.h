#pragma once

#include "MakoEntity3d.h"
#include "MakoScene3d.h"
#include "MakoApplication.h"
#include "MakoMeshManipulator.h"

MAKO_BEGIN_NAMESPACE

struct StaticBoxCreationParams : public Scene3dNodeCreationParams,
								 public Physics3dSceneCreationParams,
								 public SingleMaterialCreationParams
{
	MAKO_INLINE StaticBoxCreationParams() : size(1.f) {}
	Size3d size;
};

//! This node represents a graphical box and a
//! box collision on it.
//! This is a very high level class.
class StaticBox : public Entity3d
{
public:
	MAKO_INLINE StaticBox(Physics3dScene* p3ds,
						  const Size3d& size,
						  const Position3d& pos,
						  const Rotation3d& rot,
						  Material* material)
						  : Entity3d(APP()->GetMeshManipulator()->MakeBox(size),
						    p3ds->AddStaticBoxActor(size, pos, rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(material); }

	MAKO_INLINE ~StaticBox() {}
};

MAKO_END_NAMESPACE