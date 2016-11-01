#include "MakoScene3d.h"
#include "MakoMeshSceneNode.h"
#include "MakoSkybox.h"
#include "MakoEntity3d.h"
#include "MakoFPSCamera.h"
#include "MakoCamera.h"
#include "MakoStaticGeometry.h"
#include "MakoDynamicBox.h"
#include "MakoDynamicSphere.h"
#include "MakoStaticSphere.h"
#include "MakoStaticBox.h"
#include "MakoStaticPlane.h"

MAKO_BEGIN_NAMESPACE

// Linked-list-const-scene-3d-node-iterator
typedef LinkedList<Scene3dNode*>::const_iterator llcs3dnit;

Scene3d::Scene3d()
: cam(nullptr)
{
	root = new Scene3dNode();
	root->parent = nullptr;
	root->scene  = this;
	root->Hold();
}

Scene3d::~Scene3d()
{ cam->Drop(); root->Drop(); }

void Scene3d::DrawAll()
{
	
	UpdateNodes_r(root);
	root->UpdateAbsoluteTransformation();
	PostUpdateNodes_r(root);
	DrawNodes_r(root);
	
}

void Scene3d::UpdateNodes_r(Scene3dNode* n)
{
	for (llcs3dnit it = n->GetChildren().begin(); it != n->GetChildren().end(); ++it)
	{
		(*it)->Update();
		UpdateNodes_r((*it));
	}
}
void Scene3d::PostUpdateNodes_r(Scene3dNode* n)
{
	for (llcs3dnit it = n->GetChildren().begin(); it != n->GetChildren().end(); ++it)
	{
		(*it)->PostUpdate();
		PostUpdateNodes_r((*it));
	}
}

void Scene3d::DrawNodes_r(Scene3dNode* n)
{
	GraphicsDevice* gd = APP()->GD();
	for (llcs3dnit it = n->GetChildren().begin(); it != n->GetChildren().end(); ++it)
	{
		(*it)->PreDraw();
		(*it)->Draw(gd);
		(*it)->PostDraw();
		DrawNodes_r((*it));
	}
}

void Scene3d::SetCamera(Camera* cam)
{
	if (this->cam)
	{
		this->cam->Drop();
	}
	this->cam = cam;
	this->cam->Hold();

	if (this->cam->parent == nullptr && this->cam->scene == nullptr)
		Add(this->cam);
}


MAKO_END_NAMESPACE