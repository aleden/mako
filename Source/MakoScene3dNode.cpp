#include "MakoScene3dNode.h"
#include "MakoVec3d.h"
#include "MakoApplication.h"
#include "MakoScene3d.h"
#include "MakoMaterial.h"

MAKO_BEGIN_NAMESPACE

// Linked-List-Scene-3d-Node-Iterator
typedef LinkedList<Scene3dNode*>::iterator lls3dnit;

/////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
Scene3dNode::Scene3dNode(const Position3d& pos,
						 const Rotation3d& rot,
						 const Scale3d& scale,
						 bool isDynamic)
						 : relPos(pos), relRot(rot), relScale(scale), scene(nullptr),
						   parent(nullptr), isDynamic(isDynamic)
{}

Scene3dNode::~Scene3dNode()
{
	for (lls3dnit it = children.begin(); it != children.end(); ++it)
		(*it)->Drop();
}

/////////////////////////////////////////////////////////////////
// Methods

void Scene3dNode::UpdateAbsoluteTransformation()
{
	absTransformation = parent ? parent->GetAbsoluteTransformation() * GetTransformation() : GetTransformation();
	for (lls3dnit it = children.begin(); it != children.end(); ++it)
		(*it)->UpdateAbsoluteTransformation();
}

void Scene3dNode::RemoveChild(Scene3dNode* n)
{
	for (lls3dnit it = children.begin(); it != children.end(); ++it)
	{
		if (*it == n)
		{
			Scene3dNode* temp = *it;
			children.erase(it);
			temp->Drop();
			return;
		}
	}
}

Matrix4f Scene3dNode::GetTransformation() const
{
	Matrix4f mat;
	mat.SetRotationDegrees(relRot);
	mat.SetTranslation(relPos);

	if (relScale != Scale3d(1.f,1.f,1.f))
	{
		Matrix4f smat;
		smat.SetScale(relScale);
		mat *= smat;
	}
	return mat;
}

MAKO_END_NAMESPACE