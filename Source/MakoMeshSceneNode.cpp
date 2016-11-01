#include "MakoMeshSceneNode.h"
#include "MakoColorMtl.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include "MakoMesh.h"
#include "MakoMeshData.h"

MAKO_BEGIN_NAMESPACE

// Array-List-Material-Iterator
typedef ArrayList<Material*>::iterator almit;

MeshSceneNode::MeshSceneNode(Mesh* mesh,
							 const Position3d& pos,
							 const Rotation3d& rot,
							 const Scale3d& scale,
							 bool isDynamic)
							 : Scene3dNode(pos, rot, scale, isDynamic), mesh(mesh)
{ mesh->Hold(); }

MeshSceneNode::MeshSceneNode(const MeshSceneNodeCreationParams& p)
: Scene3dNode(p.pos, p.rot, p.scale, p.isDynamic), mesh(p.mesh)
{ mesh->Hold(); }

MeshSceneNode::~MeshSceneNode()
{ if (mesh) mesh->Drop(); }

void MeshSceneNode::Draw(GraphicsDevice* gd)
{
	for (UInt i = 0; i < mesh->GetNumSubMeshes(); ++i)
	{
		gd->SetTransform(GetAbsoluteTransformation(), TS_WORLD);
		gd->DrawMeshData(mesh->GetSubMesh(i));
	}
}

MAKO_END_NAMESPACE