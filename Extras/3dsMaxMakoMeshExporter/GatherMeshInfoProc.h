#pragma once
#include "MakoMeshExporter.h"
#include <map>
#include <vector>
#include <list>
#include "makomeshtypes.h"
#include "../../Source/MakoColor.h"

// numTCoordVertsChannel##...
// faces

// The concept of a face is that it holds no actual vertex data or tcoord
// data, but instead holds the indices into the vertex and tcoord arrays
// that form a face (triangle). Each face is 3 vertices (with each vertex
// encapsulating tcoord information).
struct MakoFace
{
	Mako::UInt32 v1;
	Mako::UInt32 v2;
	Mako::UInt32 v3;

	// tcoords associated with v1
	std::vector<Mako::UInt32> tcoords1;

	// tcoords associated with v2
	std::vector<Mako::UInt32> tcoords2;

	// tcoords associated with v3
	std::vector<Mako::UInt32> tcoords3;
};

struct Vec3df
{
	inline Vec3df(float x, float y, float z) : x(x), y(y), z(z) {}
	inline Vec3df() : x(0.f), y(0.f), z(0.f) {}
	inline Vec3df(const Point3& p) : x(p.x), y(p.y), z(p.z) {}

	inline ~Vec3df() {}

	float x, y, z;
};

struct Vec2df
{
	inline Vec2df(float x, float y) : x(x), y(y) {}
	inline Vec2df() : x(0.f), y(0.f) {}
	inline Vec2df(const Point2& p) : x(p.x), y(p.y) {}

	inline ~Vec2df() {}
	
	float x, y;
};

enum MAKO_MESH_MTL_TYPE
{ MMMT_DIFFTEXTURE, MMMT_COLOR };

struct MakoSubMeshMtl
{
	virtual MAKO_MESH_MTL_TYPE GetType() = 0;
};

struct MakoSubMeshDiffTextureMtl : public MakoSubMeshMtl
{
	MakoSubMeshDiffTextureMtl(Mako::UInt32 strTableIndex)
		: textureFilePathIndex(strTableIndex) {}

	// Index into string table, relative path.
	Mako::UInt32 textureFilePathIndex;

	MAKO_MESH_MTL_TYPE GetType()
	{ return MMMT_DIFFTEXTURE; }
};

struct MakoSubMeshColorMtl : public MakoSubMeshMtl
{
	// Color.
	Mako::Color c;

	MAKO_MESH_MTL_TYPE GetType()
	{ return MMMT_COLOR; }
};


// You cannot have a sub mesh with texture coordinates in channel 1 and 3.
// You are allowed to in 3ds Max, however. So, when this is encountered,
// The sub mesh will have those texturecoordinates in channel 0 and 1 instead.
// (you can't have 0 and 2 either).
struct SubMeshInfo
{
	// Vertices
	std::vector<Vec3df> vertices;
	
	// Number of TCoord vertices per channel, with vector index
	// as tcoord channel and element as number of tcoord verts
	std::vector<Mako::UInt32> numtcoords;
	
	// TCoordVertices, with each index corrosponding to it's
	// texture coordinates channel
	// The pointer to a Point3 is not an array. It is one Point3.
	// This is done to prevent the unnecessary copying of memory.
	std::vector< std::vector<Vec2df> > tcoords;
	
	// Faces of vertices
	// The pointer to a Face is not an array. It is one Face.
	// This is done to prevent the unnecessary copying of memory.
	std::vector<MakoFace> faces;

	// Sub materials.
	// Key: The face count that the material starts at
	// Val: The mtl that the face is rendered with
	std::map<Mako::UInt32, MakoSubMeshMtl*> subTextures;
};

struct MeshInfo
{
	// This contains a list of the indices into the stringTable
	// which point at the textures that the user may want to copy
	// to the export dir.
	std::vector<Mako::String> texturesToCopy;

	// Mako::String table
	std::vector<Mako::String> stringTable;

	// There will be a sub mesh for each texture mapping channel
	// that is used in the scene. The SubMeshes are dynamically
	// allocated.
	std::vector<SubMeshInfo*> subMeshes;
};

class GatherMeshInfoProc : public ITreeEnumProc
{
private:
	Interface* ip;
	MeshInfo& meshinfo;
public:
	inline GatherMeshInfoProc(Interface* ip, OUT MeshInfo& meshinfo)
		: ip(ip), meshinfo(meshinfo) {}
	
	inline ~GatherMeshInfoProc() {}

	static void BuildOnMeshInfo(Interface* ip, INode* node, MeshInfo& meshinfo);

	virtual int callback(INode *node);
};

TriObject* GetTriObjectFromNode(INode *node, Interface *ip, int &deleteIt);