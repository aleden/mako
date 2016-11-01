#pragma once
#include "../../Source/MakoCommon.h"

// #pragma pack(push,1) forces your compiler 
// to pack the structure as byte-aligned. 
#pragma pack (push, 1)
struct MakoMeshHeader
{
	Mako::UInt32 numSubMeshes;
	Mako::UInt32 numStrings;
	
	// Null terminated utf 16 strings
	//UInt16* string;... 
};

enum MAKO_MESH_ENTRY_TYPE
{ MMET_FACE, MMET_DIFFTEXTURE, MMET_COLOR };

struct MakoSubMeshHeader
{
	Mako::UInt32 numFaces;
	Mako::UInt32 numVerts;
	Mako::UInt32 numTCoordChannels;
};

#pragma pack (pop)