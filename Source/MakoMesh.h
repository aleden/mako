#pragma once
#include "MakoCommon.h"
#include "MakoReferenceCounted.h"

MAKO_BEGIN_NAMESPACE

// Forward Declaration
class MeshData;

//! A mesh is nothing more than a collection of MeshDatas.
//! SimpleMesh is a simple implementation of a Mesh.
//! A mesh will have sub meshes in cases where the data's
//! vertices stored have different attributes. For example,
//! if there is a mesh where you have a simple plane and a 
//! box each with only one diffuse texture, then the mesh that
//! contains these can use only one sub mesh. However, if a mesh
//! must contain a box with a diffuse texture and a plane with
//! a diffuse and lightmap texture, there must be a seperate sub
//! mesh for each thing. This is because the lightmapped plane
//! requires two sets of mapping coordinates, while the box only
//! requires one set. In more simplistic terms, a Mesh will contain
//! multiple sub meshes if it has signifigantly different materials
//! in it.
//! \n
//! if a scene from 3ds max is exported with a  terrain that has a 
//! diffuse texture mapping coordinates, and lightmap mapping 
//! coordinates channel, and then a gun with only a diffuse mapping
//! coordinates channel, the gun and the terrain will be two seperate
//! sub meshes because they don't have identical vertex information.
class Mesh : public ReferenceCounted
{
public:
	//! Check if this Mesh is animated.
	//! \return If true, it is safe to downcast this Mesh
	//! to an AnimatedMesh.
	virtual bool IsAnimated() const = 0;	
	
	//! Get the number of sub meshes this Mesh contains.
	//! \return The number of sub meshes this Mesh contains.
	virtual UInt32 GetNumSubMeshes() const = 0;
	
	//! Get a sub mesh from this Mesh
	//! \param[in] index The index of the sub mesh to retreive
	//! \return The MeshData
	virtual const MeshData* GetSubMesh(UInt32 index) const = 0;

	//! Get a sub mesh from this Mesh
	//! \param[in] index The index of the sub mesh to retreive
	//! \return The MeshData
	virtual MeshData* GetSubMesh(UInt32 index) = 0;

	//! Add a sub mesh (MeshData) to this Mesh
	//! \param[in] mb The sub mesh (MeshData) to add
	virtual void AddSubMesh(MeshData* mb) = 0;
};

MAKO_END_NAMESPACE