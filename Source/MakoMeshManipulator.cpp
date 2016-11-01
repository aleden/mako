#include "MakoStandardVertex.h"
#include "MakoApplication.h"
#include "MakoSimpleMesh.h"
#include "MakoVec3d.h"
#include "MakoVec2d.h"
#include "MakoException.h"
#include "MakoMeshManipulator.h"
#include "MakoArrayList.h"
#include "Makomath.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

Mesh* MeshManipulator::MakeSphere(Float32 radius, UInt32 polyCountX, UInt32 polyCountY)
{
	// thanks to Alfaz93 who made his code available for Irrlicht on which
	// this one is based!

	// we are creating the sphere mesh here.

	if (polyCountX < 2)
		polyCountX = 2;
	if (polyCountY < 2)
		polyCountY = 2;
	if (polyCountX * polyCountY > 32767) // prevent UInt16 overflow
	{
		if (polyCountX > polyCountY) // prevent UInt16 overflow
			polyCountX = 32767/polyCountY-1;
		else
			polyCountY = 32767/(polyCountX+1);
	}

	UInt32 polyCountXPitch = polyCountX+1; // get to same vertex on next level
	
	ArrayList<StandardVertex> verts((polyCountXPitch * polyCountY) + 2);
	ArrayList<UInt16> indices((polyCountX * polyCountY) * 6);

	UInt32 i = 0;
	UInt32 level = 0;

	for (UInt p1 = 0; p1 < polyCountY-1; ++p1)
	{
		//main quads, top to bottom
		for (UInt p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			const UInt32 curr = level + p2;
			indices[i] = curr + polyCountXPitch;
			indices[++i] = curr;
			indices[++i] = curr + 1;
			indices[++i] = curr + polyCountXPitch;
			indices[++i] = curr+1;
			indices[++i] = curr + 1 + polyCountXPitch;
			++i;
		}

		// the connectors from front to end
		indices[i] = level + polyCountX - 1 + polyCountXPitch;
		indices[++i] = level + polyCountX - 1;
		indices[++i] = level + polyCountX;
		++i;

		indices[i] = level + polyCountX - 1 + polyCountXPitch;
		indices[++i] = level + polyCountX;
		indices[++i] = level + polyCountX + polyCountXPitch;
		++i;
		level += polyCountXPitch;
	}

	const UInt32 polyCountSq = polyCountXPitch * polyCountY; // top point
	const UInt32 polyCountSq1 = polyCountSq + 1; // bottom point
	const UInt32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

	for (UInt p2 = 0; p2 < polyCountX - 1; ++p2)
	{
		// create triangles which are at the top of the sphere

		indices[i] = polyCountSq;
		indices[++i] = p2 + 1;
		indices[++i] = p2;
		++i;

		// create triangles which are at the bottom of the sphere

		indices[i] = polyCountSqM1 + p2;
		indices[++i] = polyCountSqM1 + p2 + 1;
		indices[++i] = polyCountSq1;
		++i;
	}

	// create final triangle which is at the top of the sphere

	indices[i] = polyCountSq;
	indices[++i] = polyCountX;
	indices[++i] = polyCountX-1;
	++i;

	// create final triangle which is at the bottom of the sphere

	indices[i] = polyCountSqM1 + polyCountX - 1;
	indices[++i] = polyCountSqM1;
	indices[++i] = polyCountSq1;

	// calculate the angle which separates all points in a circle
	const Float64 AngleX = 2 * PI / polyCountX;
	const Float64 AngleY = PI / polyCountY;

	i = 0;
	Float64 axz;

	// we don't start at 0.

	Float64 ay = 0;//AngleY / 2;

	for (UInt y = 0; y < polyCountY; ++y)
	{
		ay += AngleY;
		const Float64 sinay = sin(ay);
		axz = 0;

		// calculate the necessary vertices without the doubled one
		for (UInt xz = 0;xz < polyCountX; ++xz)
		{
			// calculate points position

			Pos3d pos(static_cast<Float32>(radius * cos(axz) * sinay),
				      static_cast<Float32>(radius * cos(ay)),
					  static_cast<Float32>(radius * sin(axz) * sinay));
			// for spheres the normal is the position
			Vec3df normal(pos);
			normal.Normalize();

			// calculate texture coordinates via sphere mapping
			// tu is the same on each level, so only calculate once
			Float32 tu = 0.5f;
			if (y == 0)
			{
				if (normal.y != -1.0f && normal.y != 1.0f)
					tu = static_cast<Float32>(acos(Clamp(normal.x/sinay, -1.0, 1.0)) * 0.5 * RECIPROCAL_PI64);
				if (normal.z < 0.0f)
					tu=1-tu;
			}
			else
				tu = verts[i-polyCountXPitch].GetTCoord0().x;
			
			verts[i] = StandardVertex(Pos3d(pos.x, pos.y, pos.z),
				                          Vec2df(tu, static_cast<Float32>(ay * RECIPROCAL_PI64)));
			++i;
			axz += AngleX;
		}
		// This is the doubled vertex on the initial position
		verts[i] = StandardVertex(verts[i-polyCountX]);
		verts[i].SetTCoord0(Vec2df(1.f, verts[i].GetTCoord0().y));
		++i;
	}

	// the vertex at the top of the sphere
	verts[i] = StandardVertex(Pos3d(0.0f, radius, 0.0f), Vec2df(0.5f, 0.0f));

	// the vertex at the bottom of the sphere
	++i;
	verts[i] = StandardVertex(Pos3d(0.0f, -radius, 0.0f), Vec2df(0.5f, 1.0f));

	SimpleMesh* mesh = new SimpleMesh;

	IndexedMeshDataCreationParams p;
	p.numPrimitives        = indices.size()/3;
	p.numVertBufferIndices = indices.size();
	p.numVertices          = verts.size();
	p.primitiveType        = PT_TRIANGLELIST;
	p.vertBufferIndexType  = VBIT_16;
	p.vertexType           = VT_STANDARD;
	p.vertBufferIndices    = static_cast<void*>(&indices[0]);
	p.vertices             = static_cast<void*>(&verts[0]);
	
	mesh->AddSubMesh(APP()->GD()->CreateIndexedMeshData(p));
	return mesh;
}

Mesh* MeshManipulator::MakeSkySphere(Float32 radius, UInt32 polyCountX, UInt32 polyCountY)
{
	// thanks to Alfaz93 who made his code available for Irrlicht on which
	// this one is based!

	// we are creating the sphere mesh here.

	if (polyCountX < 2)
		polyCountX = 2;
	if (polyCountY < 2)
		polyCountY = 2;
	if (polyCountX * polyCountY > 32767) // prevent UInt16 overflow
	{
		if (polyCountX > polyCountY) // prevent UInt16 overflow
			polyCountX = 32767/polyCountY-1;
		else
			polyCountY = 32767/(polyCountX+1);
	}

	UInt32 polyCountXPitch = polyCountX+1; // get to same vertex on next level
	
	ArrayList<StandardVertex> verts((polyCountXPitch * polyCountY) + 2);
	ArrayList<UInt16> indices((polyCountX * polyCountY) * 6);

	UInt32 i = 0;
	UInt32 level = 0;

	for (UInt p1 = 0; p1 < polyCountY-1; ++p1)
	{
		//main quads, top to bottom
		for (UInt p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			const UInt32 curr = level + p2;
			indices[i] = curr + polyCountXPitch;
			indices[++i] = curr;
			indices[++i] = curr + 1;
			indices[++i] = curr + polyCountXPitch;
			indices[++i] = curr+1;
			indices[++i] = curr + 1 + polyCountXPitch;
			++i;
		}

		// the connectors from front to end
		indices[i] = level + polyCountX - 1 + polyCountXPitch;
		indices[++i] = level + polyCountX - 1;
		indices[++i] = level + polyCountX;
		++i;

		indices[i] = level + polyCountX - 1 + polyCountXPitch;
		indices[++i] = level + polyCountX;
		indices[++i] = level + polyCountX + polyCountXPitch;
		++i;
		level += polyCountXPitch;
	}

	const UInt32 polyCountSq = polyCountXPitch * polyCountY; // top point
	const UInt32 polyCountSq1 = polyCountSq + 1; // bottom point
	const UInt32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

	for (UInt p2 = 0; p2 < polyCountX - 1; ++p2)
	{
		// create triangles which are at the top of the sphere

		indices[i] = polyCountSq;
		indices[++i] = p2 + 1;
		indices[++i] = p2;
		++i;

		// create triangles which are at the bottom of the sphere

		indices[i] = polyCountSqM1 + p2;
		indices[++i] = polyCountSqM1 + p2 + 1;
		indices[++i] = polyCountSq1;
		++i;
	}

	// create final triangle which is at the top of the sphere

	indices[i] = polyCountSq;
	indices[++i] = polyCountX;
	indices[++i] = polyCountX-1;
	++i;

	// create final triangle which is at the bottom of the sphere

	indices[i] = polyCountSqM1 + polyCountX - 1;
	indices[++i] = polyCountSqM1;
	indices[++i] = polyCountSq1;

	// calculate the angle which separates all points in a circle
	const Float64 AngleX = 2 * PI / polyCountX;
	const Float64 AngleY = PI / polyCountY;

	i = 0;
	Float64 axz;

	// we don't start at 0.

	Float64 ay = 0;//AngleY / 2;

	for (UInt y = 0; y < polyCountY; ++y)
	{
		ay += AngleY;
		const Float64 sinay = sin(ay);
		axz = 0;

		// calculate the necessary vertices without the doubled one
		for (UInt xz = 0;xz < polyCountX; ++xz)
		{
			// calculate points position

			Pos3d pos(static_cast<Float32>(radius * cos(axz) * sinay),
				      static_cast<Float32>(radius * cos(ay)),
					  static_cast<Float32>(radius * sin(axz) * sinay));
			// for spheres the normal is the position
			Vec3df normal(pos);
			normal.Normalize();

			// calculate texture coordinates via sphere mapping
			// tu is the same on each level, so only calculate once
			Float32 tu = 0.5f;
			if (y == 0)
			{
				if (normal.y != -1.0f && normal.y != 1.0f)
					tu = static_cast<Float32>(acos(Clamp(normal.x/sinay, -1.0, 1.0)) * 0.5 * RECIPROCAL_PI64);
				if (normal.z < 0.0f)
					tu=1-tu;
			}
			else
				tu = verts[i-polyCountXPitch].GetTCoord0().x;
			
			verts[i] = StandardVertex(Pos3d(pos.x, pos.y, pos.z),
				                          Vec2df(tu, static_cast<Float32>(ay * RECIPROCAL_PI64)));
			++i;
			axz += AngleX;
		}
		// This is the doubled vertex on the initial position
		verts[i] = StandardVertex(verts[i-polyCountX]);
		verts[i].SetTCoord0(Vec2df(1.f, verts[i].GetTCoord0().y));
		++i;
	}

	// the vertex at the top of the sphere
	verts[i] = StandardVertex(Pos3d(0.0f, radius, 0.0f), Vec2df(0.5f, 0.0f));

	// the vertex at the bottom of the sphere
	++i;
	verts[i] = StandardVertex(Pos3d(0.0f, -radius, 0.0f), Vec2df(0.5f, 1.0f));

	for (UInt i = 0; i < indices.size(); i += 3)
	{
		UInt16 old0 = indices[i + 0];
		UInt16 old1 = indices[i + 1];
		UInt16 old2 = indices[i + 2];

		indices[i] = old2;
		indices[i + 1] = old1;
		indices[i + 2] = old0;
	}

	SimpleMesh* mesh = new SimpleMesh;

	IndexedMeshDataCreationParams p;
	p.numPrimitives        = indices.size()/3;
	p.numVertBufferIndices = indices.size();
	p.numVertices          = verts.size();
	p.primitiveType        = PT_TRIANGLELIST;
	p.vertBufferIndexType  = VBIT_16;
	p.vertexType           = VT_STANDARD;
	p.vertBufferIndices    = static_cast<void*>(&indices[0]);
	p.vertices             = static_cast<void*>(&verts[0]);
	
	mesh->AddSubMesh(APP()->GD()->CreateIndexedMeshData(p));
	return mesh;
}

Mesh* MeshManipulator::MakeCube(Float32 sizeOfCube)
{ return MakeBox(sizeOfCube); }

Mesh* MeshManipulator::MakeBox(const Size3d& dim)
{
	/*
	  /0-------3
	 / |      /|
	/  |     / |
	---------  |
	|  |    |  |
	|  1____|__2
	| /	    |  /
	|/	    | /
	---------/
	*/

	const Float32 sizex = dim.x/2;
	const Float32 sizey = dim.y/2;
	const Float32 sizez = dim.z/2;

	ArrayList<StandardVertex> verts(24);
	ArrayList<UInt16> indices(36);
	
	verts[0] = StandardVertex(Vec3df(-sizex, sizey, sizez), Vec2df(1.f, 0.f));
	verts[1] = StandardVertex(Vec3df(-sizex,-sizey, sizez), Vec2df(1.f, 1.f));
	verts[2] = StandardVertex(Vec3df( sizex,-sizey, sizez), Vec2df(0.f, 1.f));
	verts[3] = StandardVertex(Vec3df( sizex, sizey, sizez), Vec2df(0.f, 0.f));
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;
	
	/*
	  /--------2
	 / |      /|
	/  |     / |
	--------0  |
	|  |    |  |
	|  |____|__3
	| /	    |  /
	|/	    | /
	--------1/
	*/

	verts[0 + 4] = StandardVertex(Vec3df( sizex, sizey,-sizez), Vec2df(0.f, 0.f));
	verts[1 + 4] = StandardVertex(Vec3df( sizex,-sizey,-sizez), Vec2df(0.f, 1.f));
	verts[2 + 4] = StandardVertex(Vec3df( sizex, sizey, sizez), Vec2df(1.f, 0.f));
	verts[3 + 4] = StandardVertex(Vec3df( sizex,-sizey, sizez), Vec2df(1.f, 1.f));
	
	indices[0 + 6] = 3 + 4;
	indices[1 + 6] = 1 + 4;
	indices[2 + 6] = 0 + 4;
	indices[3 + 6] = 3 + 4;
	indices[4 + 6] = 0 + 4;
	indices[5 + 6] = 2 + 4;
	

	/*
	  /--------/
	 / |      /|
	/  |     / |
	1-------2  |
	|  |    |  |
	|  |____|__|
	| /	    |  /
	|/	    | /
	0-------3/
	*/

	verts[0 + 8] = StandardVertex(Vec3df(-sizex,-sizey,-sizez), Vec2df(0.f, 1.f));
	verts[1 + 8] = StandardVertex(Vec3df(-sizex, sizey,-sizez), Vec2df(0.f, 0.f));
	verts[2 + 8] = StandardVertex(Vec3df( sizex, sizey,-sizez), Vec2df(1.f, 0.f));
	verts[3 + 8] = StandardVertex(Vec3df( sizex,-sizey,-sizez), Vec2df(1.f, 1.f));
	
	// Order indices in counter-clockwise order so the normals are flipped.
	indices[0 + 12] = 3 + 8;
	indices[1 + 12] = 1 + 8;
	indices[2 + 12] = 2 + 8;
	indices[3 + 12] = 0 + 8;
	indices[4 + 12] = 1 + 8;
	indices[5 + 12] = 3 + 8;
	
	/*
	  /3-------/
	 / |      /|
	/  |     / |
	0--------  |
	|  |    |  |
	|  2____|__|
	| /	    |  /
	1/	    | /
	---------/
	*/

	verts[0 + 12] = StandardVertex(Vec3df(-sizex, sizey,-sizez), Vec2df(1.f, 0.f));
	verts[1 + 12] = StandardVertex(Vec3df(-sizex,-sizey,-sizez), Vec2df(1.f, 1.f));
	verts[2 + 12] = StandardVertex(Vec3df(-sizex,-sizey, sizez), Vec2df(0.f, 1.f));
	verts[3 + 12] = StandardVertex(Vec3df(-sizex, sizey, sizez), Vec2df(0.f, 0.f));
	
	// Order indices in counter-clockwise order so the normals are flipped.
	indices[0 + 18] = 1 + 12;
	indices[1 + 18] = 2 + 12;
	indices[2 + 18] = 0 + 12;
	indices[3 + 18] = 0 + 12;
	indices[4 + 18] = 2 + 12;
	indices[5 + 18] = 3 + 12;
	
	/*
	  /0-------3
	 / |      /|
	/  |     / |
	1-------2  |
	|  |    |  |
	|  |____|__|
	| /	    |  /
	|/	    | /
	---------/		 
	*/

	verts[0 + 16] = StandardVertex(Vec3df(-sizex, sizey, sizez), Vec2df(0.f, 0.f));
	verts[1 + 16] = StandardVertex(Vec3df(-sizex, sizey,-sizez), Vec2df(0.f, 1.f));
	verts[2 + 16] = StandardVertex(Vec3df( sizex, sizey,-sizez), Vec2df(1.f, 1.f));
	verts[3 + 16] = StandardVertex(Vec3df( sizex, sizey, sizez), Vec2df(1.f, 0.f));
	
	// Order indices in counter-clockwise order so the normals are flipped.
	indices[0 + 24] = 0 + 16;
	indices[1 + 24] = 3 + 16;
	indices[2 + 24] = 2 + 16;
	indices[3 + 24] = 1 + 16;
	indices[4 + 24] = 0 + 16;
	indices[5 + 24] = 2 + 16;
	
	/*
	  /|-------/
	 / |      /|
	/  |     / |
	|--------  |
	|  |    |  |
	|  0____|__3
	| /	    |  /
	|/	    | /
	1-------2/
	*/
	
	verts[0 + 20] = StandardVertex(Vec3df(-sizex,-sizey, sizez), Vec2df(1.f, 0.f));
	verts[1 + 20] = StandardVertex(Vec3df(-sizex,-sizey,-sizez), Vec2df(1.f, 1.f));
	verts[2 + 20] = StandardVertex(Vec3df( sizex,-sizey,-sizez), Vec2df(0.f, 1.f));
	verts[3 + 20] = StandardVertex(Vec3df( sizex,-sizey, sizez), Vec2df(0.f, 0.f));
	
	indices[0 + 30] = 0 + 20;
	indices[1 + 30] = 1 + 20;
	indices[2 + 30] = 3 + 20;
	indices[3 + 30] = 3 + 20;
	indices[4 + 30] = 1 + 20;
	indices[5 + 30] = 2 + 20;
	
	SimpleMesh* mesh = new SimpleMesh;

	IndexedMeshDataCreationParams p;
	p.numPrimitives        = indices.size()/3;
	p.numVertBufferIndices = indices.size();
	p.numVertices          = verts.size();
	p.primitiveType        = PT_TRIANGLELIST;
	p.vertBufferIndexType  = VBIT_16;
	p.vertexType           = VT_STANDARD;
	p.vertBufferIndices    = static_cast<void*>(&indices[0]);
	p.vertices             = static_cast<void*>(&verts[0]);

	mesh->AddSubMesh(APP()->GD()->CreateIndexedMeshData(p));
	return mesh;
}

Mesh* MeshManipulator::MakeSkybox()
{
	// The skybox generated is a cube with the normals flipped. SimpleMesh will contain 6 sub meshes
	// (meshbuffers). One for each face of the cube, because each face has a unique texture.
	const Float32 size = 1.f; // size is the max value for a float / 10

	/*

	  /0-------3
	 / |      /|
	/  |     / |
	---------  |
	|  |    |  |
	|  1____|__2
	| /	    |  /
	|/	    | /
	---------/	 
			 
	*/

	ArrayList<StandardVertex> verts(24);
	ArrayList<UInt16> indices(36);

	ArrayList<StandardVertex>::iterator vptr = verts.begin();
	ArrayList<UInt16>::iterator iptr = indices.begin();
	
	(*vptr++) = StandardVertex(Vec3df(-size, size, size), Vec2df(0.f, 0.f)); // 0
	(*vptr++) = StandardVertex(Vec3df(-size,-size, size), Vec2df(0.f, 1.f)); // 1
	(*vptr++) = StandardVertex(Vec3df( size,-size, size), Vec2df(1.f, 1.f)); // 2
	(*vptr++) = StandardVertex(Vec3df( size, size, size), Vec2df(1.f, 0.f)); // 3

	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 3;
	(*iptr++) = 1;
	(*iptr++) = 0;
	(*iptr++) = 3;
	(*iptr++) = 2;
	(*iptr++) = 1;

	//tri->AddSubMesh(APP()->GetGraphicsDevice()->CreateMeshData(verts, indices, 2, PT_TRIANGLELIST));

	//verts.clear();
	//indices.clear();

	/*

	  /--------2
	 / |      /|
	/  |     / |
	--------0  |
	|  |    |  |
	|  |____|__3
	| /	    |  /
	|/	    | /
	--------1/	 
			 
	*/

	(*vptr++) = StandardVertex(Vec3df( size, size,-size), Vec2df(1.f, 0.f)); // 0
	(*vptr++) = StandardVertex(Vec3df( size,-size,-size), Vec2df(1.f, 1.f)); // 1
	(*vptr++) = StandardVertex(Vec3df( size, size, size), Vec2df(0.f, 0.f)); // 2
	(*vptr++) = StandardVertex(Vec3df( size,-size, size), Vec2df(0.f, 1.f)); // 3


	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 0 + 4;
	(*iptr++) = 1 + 4;
	(*iptr++) = 3 + 4;
	(*iptr++) = 2 + 4;
	(*iptr++) = 0 + 4;
	(*iptr++) = 3 + 4;
	
	//tri->AddSubMesh(APP()->GetGraphicsDevice()->CreateMeshData(verts, indices, 2, PT_TRIANGLELIST));

	//verts.clear();
	//indices.clear();



	/*

	  /--------/
	 / |      /|
	/  |     / |
	1-------2  |
	|  |    |  |
	|  |____|__|
	| /	    |  /
	|/	    | /
	0-------3/

	*/

	(*vptr++) = StandardVertex(Vec3df(-size,-size,-size), Vec2df(1.f, 1.f)); // 0
	(*vptr++) = StandardVertex(Vec3df(-size, size,-size), Vec2df(1.f, 0.f)); // 1
	(*vptr++) = StandardVertex(Vec3df( size, size,-size), Vec2df(0.f, 0.f)); // 2
	(*vptr++) = StandardVertex(Vec3df( size,-size,-size), Vec2df(0.f, 1.f)); // 3

	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 2 + 8;
	(*iptr++) = 1 + 8;
	(*iptr++) = 3 + 8;
	(*iptr++) = 3 + 8;
	(*iptr++) = 1 + 8;
	(*iptr++) = 0 + 8;
	
	//tri->AddSubMesh(APP()->GetGraphicsDevice()->CreateMeshData(verts, indices, 2, PT_TRIANGLELIST));

	//verts.clear();
	//indices.clear();


	/*

	  /3-------/
	 / |      /|
	/  |     / |
	0--------  |
	|  |    |  |
	|  2____|__|
	| /	    |  /
	1/	    | /
	---------/	 
			 
	*/

	(*vptr++) = StandardVertex(Vec3df(-size, size,-size), Vec2df(0.f, 0.f)); // 0
	(*vptr++) = StandardVertex(Vec3df(-size,-size,-size), Vec2df(0.f, 1.f)); // 1
	(*vptr++) = StandardVertex(Vec3df(-size,-size, size), Vec2df(1.f, 1.f)); // 2
	(*vptr++) = StandardVertex(Vec3df(-size, size, size), Vec2df(1.f, 0.f)); // 3


	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 0 + 12;
	(*iptr++) = 2 + 12;
	(*iptr++) = 1 + 12;
	(*iptr++) = 3 + 12;
	(*iptr++) = 2 + 12;
	(*iptr++) = 0 + 12;
	
	//tri->AddSubMesh(APP()->GetGraphicsDevice()->CreateMeshData(verts, indices, 2, PT_TRIANGLELIST));

	//verts.clear();
	//indices.clear();

	/*

	  /0-------3
	 / |      /|
	/  |     / |
	1-------2  |
	|  |    |  |
	|  |____|__|
	| /	    |  /
	|/	    | /
	---------/	 
			 
	*/

	(*vptr++) = StandardVertex(Vec3df(-size, size, size), Vec2df(0.f, 0.f)); // 0
	(*vptr++) = StandardVertex(Vec3df(-size, size,-size), Vec2df(1.f, 0.f)); // 1
	(*vptr++) = StandardVertex(Vec3df( size, size,-size), Vec2df(1.f, 1.f)); // 2
	(*vptr++) = StandardVertex(Vec3df( size, size, size), Vec2df(0.f, 1.f)); // 3


	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 2 + 16;
	(*iptr++) = 3 + 16;
	(*iptr++) = 0 + 16;
	(*iptr++) = 2 + 16;
	(*iptr++) = 0 + 16;
	(*iptr++) = 1 + 16;
	
	//tri->AddSubMesh(APP()->GetGraphicsDevice()->CreateMeshData(verts, indices, 2, PT_TRIANGLELIST));

	//verts.clear();
	//indices.clear();

	/*

	  /|-------/
	 / |      /|
	/  |     / |
	|--------  |
	|  |    |  |
	|  0____|__3
	| /	    |  /
	|/	    | /
	1-------2/
	
	*/
	
	(*vptr++) = StandardVertex(Vec3df(-size,-size,-size), Vec2df(1.f, 1.f)); // 0
	(*vptr++) = StandardVertex(Vec3df(-size,-size, size), Vec2df(0.f, 1.f)); // 1
	(*vptr++) = StandardVertex(Vec3df( size,-size, size), Vec2df(0.f, 0.f)); // 2
	(*vptr++) = StandardVertex(Vec3df( size,-size,-size), Vec2df(1.f, 0.f)); // 3


	// Order indices in counter-clockwise order so the normals are flipped.
	(*iptr++) = 0 + 20;
	(*iptr++) = 1 + 20;
	(*iptr++) = 3 + 20;
	(*iptr++) = 3 + 20;
	(*iptr++) = 1 + 20;
	(*iptr++) = 2 + 20;
	
	SimpleMesh* mesh = new SimpleMesh;
	IndexedMeshDataCreationParams p;
	p.numPrimitives        = indices.size()/3;
	p.numVertBufferIndices = indices.size();
	p.numVertices          = verts.size();
	p.primitiveType        = PT_TRIANGLELIST;
	p.vertBufferIndexType  = VBIT_16;
	p.vertexType           = VT_STANDARD;
	p.vertBufferIndices    = static_cast<void*>(&indices[0]);
	p.vertices             = static_cast<void*>(&verts[0]);

	mesh->AddSubMesh(APP()->GD()->CreateIndexedMeshData(p));
	return mesh;
}

Mesh* MeshManipulator::MakePlane(Float32 size, Float32 textureCoordsScale)
{
	if (size/2 <= 0)
		throw Exception(Text("The size/2 given to MeshManipulator::MakePlane() was less than or equal to zero."));

	ArrayList<StandardVertex> verts(4);
	verts[0] = StandardVertex(Vec3df(-size/2, 0, -size/2), Vec2df(0.f, 1.f));
	verts[1] = StandardVertex(Vec3df(-size/2, 0,  size/2), Vec2df(0.f, 0.f));
	verts[2] = StandardVertex(Vec3df( size/2, 0, -size/2), Vec2df(1.f, 1.f));
	verts[3] = StandardVertex(Vec3df( size/2, 0,  size/2), Vec2df(1.f, 0.f));

	SimpleMesh* mesh = new SimpleMesh();
	for (UInt8 i = 0; i < verts.size(); i++)
		verts[i].SetTCoord0(verts[i].GetTCoord0() * textureCoordsScale);
	
	MeshDataCreationParams p;
	p.numPrimitives        = 2;
	p.numVertices          = verts.size();
	p.primitiveType        = PT_TRIANGLESTRIP;
	p.vertexType           = VT_STANDARD;
	p.vertices             = static_cast<void*>(&verts[0]);
	
	mesh->AddSubMesh(APP()->GD()->CreateMeshData(p));
	return mesh;
}


MAKO_END_NAMESPACE