#pragma once
#include "MakoGraphicsDevice.h"
#include "MakoEntity3d.h"
#include "MakoScene3d.h"
#include "MakoApplication.h"
#include "MakoMeshManipulator.h"
#include "MakoMeshData.h"

MAKO_BEGIN_NAMESPACE

struct DynamicBoxCreationParams : public Scene3dNodeCreationParams,
								  public Physics3dSceneCreationParams,
								  public SingleMaterialCreationParams
{
	MAKO_INLINE DynamicBoxCreationParams() : size(1.f) {}
	Size3d size;
};


//! This node represents a graphical box and a
//! box collision on it.
//! This is a very high level class.
class DynamicBox : public Entity3d
{
public:
	MAKO_INLINE DynamicBox(Physics3dScene* p3ds,
						   const Size3d& size,
						   const Position3d& pos,
						   const Rotation3d& rot,
						   Material* material = nullptr)
						   : Entity3d(APP()->GetMeshManipulator()->MakeBox(size),
						     p3ds->AddDynamicBoxActor(size, pos, rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(material); }
	
	MAKO_INLINE DynamicBox(const DynamicBoxCreationParams& p)
		: Entity3d(APP()->GetMeshManipulator()->MakeBox(p.size),
		  p.p3ds->AddDynamicBoxActor(p.size, p.pos, p.rot), Scale3d(1.f), true)
	{ GetMesh()->GetSubMesh(0)->SetMaterial(p.material); }

	MAKO_INLINE ~DynamicBox() {}
};

MAKO_END_NAMESPACE