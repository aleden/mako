#pragma once
#include "MakoMeshData.h"

MAKO_BEGIN_NAMESPACE

//! Specifies the index type (32 bit or 16 bit). Each of the enums'
//! values are equal to the size in BYTES of the index type they
//! corrospond to.
enum VERTEX_BUFFER_INDEX_TYPE
{ VBIT_16 = 16/8, VBIT_32 = 32/8, VBIT_ENUM_LENGTH = 2 };

//! This struct represents the parameters to create an IndexedMeshData.
struct IndexedMeshDataCreationParams : public MeshDataCreationParams
{
	MAKO_INLINE IndexedMeshDataCreationParams()
	: vertBufferIndices(nullptr), vertBufferIndexType(VBIT_16) {}
	
	//! The indices into the vertex buffer describing the draw
	//! order of the vertices. Default: nullptr
	void* vertBufferIndices;

	//! The number of vertex buffer indices
	UInt32 numVertBufferIndices;

	//! The type of the indices. Default: VBIT_16.
	VERTEX_BUFFER_INDEX_TYPE vertBufferIndexType;
};

// Forward declaration
class IndexedMeshData;

//! This class is used in conjunction with MeshData to store vertex buffer index data
//! in GPU memory- increasing rendering time. For every implementation of GraphicsDevice, 
//! there should be a corrosponding IndexHardwareBuffer implementation.
class IndexHardwareBuffer : public HardwareBuffer
{
public:
	//! Get the parent of the MeshData it belongs to
	virtual IndexedMeshData* GetParent() = 0;

	virtual ~IndexHardwareBuffer() {}
};

class IndexedMeshData : public MeshData
{
private:
	void* vertBufferIndices;
	UInt32 numVertBufferIndices;
	VERTEX_BUFFER_INDEX_TYPE vertBufferIndexType;

	IndexHardwareBuffer* hib;
public:
	IndexedMeshData(GraphicsDevice* gd, const IndexedMeshDataCreationParams& params);
	~IndexedMeshData();

	MAKO_INLINE bool IsIndexed() const
	{ return true; }
	
	MAKO_INLINE IndexHardwareBuffer* GetIndexHardwareBuffer()
	{ return hib; }

	//! This will be usually 16 or 32
	//! \return The size of one index in bytes.
	MAKO_INLINE VERTEX_BUFFER_INDEX_TYPE GetVertexBufferIndexType() const
	{ return vertBufferIndexType; }
	
	//! Get the number of indices in this MeshData
	//! \return The number of indices in this MeshData
	MAKO_INLINE UInt32 GetNumVertexBufferIndices() const
	{ return numVertBufferIndices; }
	
	//! Get a pointer to the index buffer
	//! \return The index buffer. To work with it, use GetIndexType()
	//! to find the size of each index, then appropriatly cast the pointer
	//! to UInt32* or UInt16*.
	MAKO_INLINE const void* GetVertexBufferIndices() const
	{ return vertBufferIndices; }
	
	//! Get a pointer to the index buffer
	//! \return The index buffer. To work with it, use GetIndexType()
	//! to find the size of each index, then appropriatly cast the pointer
	//! to UInt32* or UInt16*.
	MAKO_INLINE void* GetVertexBufferIndices()
	{ return vertBufferIndices; }
};

MAKO_END_NAMESPACE