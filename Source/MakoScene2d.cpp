#include "MakoScene2d.h"

MAKO_BEGIN_NAMESPACE

Scene2d::Root2dSceneNode* Scene2d::GetRootNode() const
{ return root; }

void Scene2d::DrawAll()
{
	UpdateThenDrawChildren_r(root);
}

void Scene2d::UpdateThenDrawChildren_r(Scene2dNode* n)
{
	if (n->GetNumberOfChildren() == 0)
		return ;

	for (UInt16 i = 0; i < n->GetNumberOfChildren(); i++)
	{
		n->GetChild(i)->Update();
		n->GetChild(i)->Draw();
		UpdateThenDrawChildren_r(n->GetChild(i));
	}
}

MAKO_END_NAMESPACE

