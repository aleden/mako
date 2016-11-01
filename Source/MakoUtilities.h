#pragma once
#include "MakoCommon.h"
#include "MakoMeshData.h"

MAKO_BEGIN_NAMESPACE


////////////////////////////////////////////////////////////////////////
// Mesh methods

//! Calculate the vertex buffer index position in an IndexedMeshData
//! given the number of primitives and the primitive type.
UInt32 CalcVBIndexPosFromPrimCount(UInt32 numPrimitives, PRIMITIVE_TYPE type);

//! Calculate the vertex position in an MeshData given the number 
//! of primitives and the primitive type.
UInt32 CalcVertPosFromPrimCount(UInt32 numPrimitives, PRIMITIVE_TYPE type);


MAKO_END_NAMESPACE