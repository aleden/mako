#pragma once
#include "MakoScene3dNode.h"
#include "MakoVec3d.h"
#include "MakoArrayList.h"
#include "MakoMaterial.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Mesh;

struct MeshSceneNodeCreationParams : public Scene3dNodeCreationParams
{
	MAKO_INLINE MeshSceneNodeCreationParams() : isDynamic(true) {}
	
	Mesh* mesh;
	bool isDynamic;
};

//! This node represents a mesh, and the mesh is drawn
//! every frame at the transformation of the node (GetAbsoluteTransformation()).
class MeshSceneNode : public Scene3dNode
{
private:
	Mesh* mesh;
public:
	//! Constructor
	MAKO_API MeshSceneNode(Mesh* mesh,
		                   const Position3d& pos = Vec3df(0),
						   const Rotation3d& rot = Vec3df(0),
						   const Scale3d& scale  = Scale3d(1.f),
						   bool isDynamic        = true);

	MAKO_API MeshSceneNode(const MeshSceneNodeCreationParams& params);

	//! Virtual deconstructor, drops mesh and mat
	MAKO_API virtual ~MeshSceneNode();
	
	//! Draws the mesh with GraphicsDevice::DrawMeshData().
	MAKO_API virtual void Draw(GraphicsDevice* gd);
	
	//! Get the mesh that this MeshSceneNode draws.
	//! \return The mesh that this MeshSceneNode draws.
	MAKO_INLINE Mesh* GetMesh()
	{ return mesh; }
};

MAKO_END_NAMESPACE