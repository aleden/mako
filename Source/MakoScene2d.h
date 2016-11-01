#pragma once
#include "MakoScene2d.h"
#include "MakoScene2dNode.h"
#include "MakoVec3d.h"
#include "MakoCamera.h"
#include "MakoApplication.h"

MAKO_BEGIN_NAMESPACE

class Scene2d : public ReferenceCounted
{
private:
	class Root2dSceneNode : public Scene2dNode
	{
	public:
		MAKO_INLINE Root2dSceneNode() : Scene2dNode(0, 0, 0, true) {}
		MAKO_INLINE ~Root2dSceneNode() {}
	};

	Root2dSceneNode* root;

	// Recursive function
	void UpdateThenDrawChildren_r(Scene2dNode* n);
public:
	MAKO_INLINE Scene2d()
	{
		root = new Root2dSceneNode;
		root->Hold();
	}
	
	MAKO_INLINE ~Scene2d()
	{ root->Drop(); }

	void DrawAll();

	Root2dSceneNode* GetRootNode() const;
};

MAKO_END_NAMESPACE