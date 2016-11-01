#pragma once
#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

class Application;

//! This class deals with manipulating/creating meshes.
class MeshManipulator
{
public:
	//! Constructor
	MAKO_INLINE MeshManipulator() {}
	
	//! Empty Deconstructor
	MAKO_INLINE ~MeshManipulator() {}

	//! Make a sphere mesh. This code was taken from the Irrlicht graphics engine 
	//! to create, it (lightly edited). The mesh made from this function has a 
	//! single MeshData, containing standard vertices that have spherical mapped
	//! texture coordinates.
	//! \param[in] radius The radius of the sphere
	//! \param[in] polyCountX (Optional) The number of triangles horizontally
	//! \param[in] polyCountY (Optional) The number of triangles vertically
	MAKO_API Mesh* MakeSphere(Float32 radius, UInt32 polyCountX = 25, UInt32 polyCountY = 25);

	//! Make a sky sphere mesh. This just makes a sphere with it's normals flipped
	//! (similiar to MakeSkybox())
	//! \param[in] radius The radius of the sphere
	//! \param[in] polyCountX (Optional) The number of triangles horizontally
	//! \param[in] polyCountY (Optional) The number of triangles vertically
	MAKO_API Mesh* MakeSkySphere(Float32 radius, UInt32 polyCountX = 25, UInt32 polyCountY = 25);

	//! Create a cube mesh. The resulting mesh has a single MeshData, containing
	//! standard vertices with simple texture mapping coordinates.
	//! \param[in] size The size of the cube
	//! \return The cube mesh
	MAKO_API Mesh* MakeCube(Float32 size);

	//! Create a box mesh. The resulting mesh has a single MeshData, containing
	//! standard vertices with simple texture mapping coordinates.
	//! \param[in] dim The dimensions of the box
	//! \return The box mesh
	MAKO_API Mesh* MakeBox(const Size3d& dim);
	
	//! Create a plane mesh- a square which is laid flat down.
	//! \param[in] size The size of the plane
	//! \param[in] textureCoordsScale (Optional) the scale of the texture coordinates.
	//! \return The plane mesh
	MAKO_API Mesh* MakePlane(Float32 size, Float32 textureCoordsScale = 1.f);
	
	//! Create a skybox mesh. This is simply a cube mesh with a size of 1.f with it's
	//! normals flipped.
	//! \return The skybox mesh
	MAKO_API Mesh* MakeSkybox();
};


MAKO_END_NAMESPACE