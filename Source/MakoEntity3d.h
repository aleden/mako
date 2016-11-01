#pragma once

#include "MakoCommon.h"
#include "MakoMeshSceneNode.h"
#include "MakoPhysics3dDevice.h"

MAKO_BEGIN_NAMESPACE

struct Entity3dCreationParams : public Scene3dNodeCreationParams,
								public Physics3dSceneCreationParams
{
	Mesh* mesh;
	Physics3dActor* actor;
};

//! An entity represents a mesh that's orientation is 
//! controlled by a physics actor.
class Entity3d : public MeshSceneNode
{
private:
	Physics3dActor* actor;
	bool isFirstFrame;
public:
	MAKO_INLINE Entity3d(Mesh* mesh,
						 Physics3dActor* actor,
						 const Scale3d& scale,
						 bool isDynamic)
	: MeshSceneNode(mesh, actor->GetPosition(), actor->GetRotation(), scale, isDynamic),
	  isFirstFrame(true), actor(actor)
	{ actor->Hold(); }
	
	MAKO_INLINE virtual ~Entity3d()
	{ actor->Drop(); }
	
	MAKO_INLINE virtual void Update()
	{
		MeshSceneNode::Update();
		
		if (IsDynamic())
		{
			SetPosition(actor->GetPosition());
			SetRotation(actor->GetRotation());
		}
	}
	
	MAKO_INLINE virtual void Draw(GraphicsDevice* gd)
	{
		MeshSceneNode::Draw(gd);
	}
	
	MAKO_INLINE virtual Physics3dActor* GetActor()
	{ return actor; }

	MAKO_INLINE void SetScale(const Size3d& scale)
	{ return ; }

};

MAKO_END_NAMESPACE