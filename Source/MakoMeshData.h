#pragma once
#include "MakoReferenceCounted.h"
#include "MakoVertex.h"
#include "MakoMap.h"
#include "MakoMaterial.h"
#include "MakoHardwareBuffer.h"

MAKO_BEGIN_NAMESPACE

//! This enum identifies 3d primitive types. This is used
//! for drawing.
//! Protip: the values in PRIMITIVE_TYPE match the values
//! corrosponding to D3D primitive types.
enum PRIMITIVE_TYPE
{
	//! The point list primitive treats each vertex as an individual point.
	//! Each point primitive consists of a single vertex. If there are vertices 
	//! A, B, C, and D, then 4 point primitives will be drawn, as defined 
	//! by A, B, C, and D.
	//!
	//! The following illustration depicts a rendered point list:
	//! \image html PT_POINTLIST.gif
	//! <BR>
	PT_POINTLIST    = 1,

	//! The line list primitive specifies a series of independent line segments.
	//! Each primitive consists of two vertices. If there are vertices 
	//! A, B, C and D, then 2 line segments will be drawn, as defined by 
	//! (A, B) and (C, D).
	//!
	//! The following illustration depicts a rendered line list:
	//! \image html PT_LINELIST.gif
	//! <BR>
	PT_LINELIST	    = 2,

	//! The line strip primitive specifies a series of interconnected line 
	//! segments. The first two vertices specify a line segment in the same way 
	//! as the line list primitive. However, each additional vertex is treated 
	//! as the endpoint of another line segment with the vertex preceding it. 
	//! Therefore, if there are vertices A, B, C and D, three line segments will 
	//! be drawn as defined by (A,B), (B,C) and (C,D).
	//!
	//! The following illustration depicts a rendered line strip:
	//! \image html PT_LINESTRIP.gif
	//! <BR>
	PT_LINESTRIP    = 3,

	//! The triangle list primitive specifies a series of independent triangles. 
	//! Each primitive consists of three vertices. If there are vertices 
	//! A, B, C, D, E and F, then 2 triangles will be drawn (A,B,C) and (D,E,F).
	//!
	//! The following illustration depicts a rendered triangle list:
	//! \image html PT_TRIANGLELIST.gif
	//! <BR>
	PT_TRIANGLELIST  = 4,

	//! The triangle strip primitive specifies a series of interconnected triangles. 
	//! The first triangle is specified according to the first 3 vertices as in 
	//! a triangle list. The remaining triangles are specified by a shared edge 
	//! with the previous triangle and one new vertex. If there are vertices 
	//! A, B, C, D, E, and F, then 4 triangles will be drawn: (A,B,C), (B, D, C), 
	//! (C, D, E), and (D, F, E). Note that the second and third vertices of 
	//! the second and fourth triangles are reversed. The triangles (B, D, C) 
	//! and (D, F, E) are used rather than (B, C, D) and (D, E, F) to preserve 
	//! the "winding order" of the triangles. All of the triangles a strip are wound 
	//! in the same direction.
	//!
	//! The following illustration depicts a rendered triangle strip:
	//! \image html PT_TRIANGLESTRIP2.gif
	//! <BR>
	PT_TRIANGLESTRIP = 5,

	//! The triangle fan primitive also specifies a series of interconnected triangles. 
	//! Like the triangle strip, the first triangle is specified by the first 3 
	//! vertices. Each successive vertex is moved into the third vertex position and 
	//! the vertex that was in the third position is moved into the second. The first 
	//! vertex never changes. If there are vertices A, B, C, D, E, and F, then 4 triangles 
	//! will be drawn: (A, B, C), (A, C, D), (A, D, E) and (A, E, F).
	//!
	//! The following illustration depicts a rendered triangle fan:
	//! \image html PT_TRIANGLEFAN2.gif
	//! <BR>
	PT_TRIANGLEFAN  = 6,
	
	PT_ENUM_LENGTH  = 6
};

//! An enum indifying various options for the usage for a MeshData. This
//! is used in optimization.
enum MESH_BUFFER_USAGE_OPTION
{
	//! Set to indicate that the vertex buffer requires dynamic memory use.
	//! This is useful for drivers because it enables them to decide where 
	//! to place the buffer. In general, static vertex buffers are placed 
	//! in video memory and dynamic vertex buffers are placed in AGP memory. 
	//! Note that there is no separate static use. If you do not specify 
	//! MBUO_VBDYNAMIC, the vertex buffer is made static. This usage option
	//! is enabled for MeshDatas in an AnimatedMesh.
	MBUO_VERTEX_BUFFER_DYNAMIC = 1 << 0
};

// Forward declaration
class MeshData;

//! This class is used in conjunction with MeshData to store vertex data
//! in GPU memory- decreasing rendering time. For every implementation of
//! GraphicsDevice, there should be a corrosponding VertexHardwareBuffer
//! implementation.
class VertexHardwareBuffer : public HardwareBuffer
{
public:
	//! Get the parent of the MeshData it belongs to
	virtual MeshData* GetParent() = 0;

	virtual ~VertexHardwareBuffer() {}
};


//! This struct represents the parameters to create a MeshData.
struct MeshDataCreationParams
{
	MAKO_INLINE MeshDataCreationParams() 
	: vertices(nullptr), flags(0) {}

	//! The vertices to be put in the MeshData
	void* vertices;
	//! The number of vertices to be put in the MeshData
	UInt32 numVertices;
	//! The vertex type of the vertices. Usually VT_STANDARD
	VERTEX_TYPE vertexType;
	
	// The number of primitives that this MeshData describes.
	UInt32 numPrimitives;
	//! The primtive type that this MeshData describes.
	PRIMITIVE_TYPE primitiveType;
	
	//! This paramter is a combination (bit mask) of MESH_BUFFER_USAGE_OPTION
	//! elements
	UInt32 flags;

	//! This Map outlines the materials for the mesh buffer. The renderer 
	//! will iterate through all the entries in the map. It will render
	//! the mesh the number of vertices until the key, in which it will switch
	//! materials. If the map only contains one entry which is <0, new NullMaterial>
	//! then the whole MeshData will be drawn with only NullMaterial. This flexible
	//! system allows for multi-sub object materials.
	Map<UInt32, Material*> materials;
};


//! A MeshData is a class that represents a set of vertices (and optionally indices)
//! representing a shape that is rendered following it's primitive type. By default,
//! it also stores it's mesh information in GPU memory, with VertexHardwareBuffer.
class MeshData : public ReferenceCounted
{
private:
	Map<UInt32, Material*> subMaterials;

	void* vertices;
	UInt32 numVertices;
	VERTEX_TYPE vertexType;
	
	UInt32 numPrimitives;
	PRIMITIVE_TYPE primitiveType;

	VertexHardwareBuffer* hvb;

	UInt32 flags;
public:
	MAKO_API MeshData(GraphicsDevice* gd, const MeshDataCreationParams& params);
	MAKO_API ~MeshData();

	//! Get the VertexHardwareBuffer for this MeshData.
	//! \return The VertexHardwareBuffer for this MeshData, if it exists.
	//! this method will return a nullptr if a VertexHardwareBuffer has not
	//! been initialized for this object. This will commonly be the case if
	//! this MeshData is being used not for rendering, such as being used for
	//! collision.
	MAKO_INLINE VertexHardwareBuffer* GetVertexHardwareBuffer()
	{ return hvb; }

	//! Check if this MeshData is a IndexedMeshData.
	//! \return If the MeshData is a IndexedMeshData. It is safe to downcast
	//! this to an IndexedMeshData if true.
	virtual bool IsIndexed() const
	{ return false; }
	
	//! Get the usage of this MeshData.
	//! \return A bitmask, combination of the MESH_BUFFER_USAGE_OPTION flags.
	MAKO_INLINE UInt32 GetFlags() const
	{ return flags; }

	//! Retreives the type of vertices handled in this
	//! MeshData. This attribute cannot change.
	MAKO_INLINE VERTEX_TYPE GetVertexType() const
	{ return vertexType; }
	
	//! Get the number of vertices in this MeshData
	MAKO_INLINE UInt32 GetNumVertices() const
	{ return numVertices; }
	
	//! Get the vertices in this MeshData
	MAKO_INLINE const void* GetVertices() const
	{ return vertices; }
	
	//! Get the vertices in this MeshData
	MAKO_INLINE void* GetVertices()
	{ return vertices; }
	
	//! Get the type of primitive this MeshData renders
	//! \return The type of primitive this MeshData renders
	MAKO_INLINE PRIMITIVE_TYPE GetPrimitiveType() const
	{ return primitiveType; }
	
	//! Get the number of primitives drawn by this
	//! MeshData
	//! \return the number of primitives drawn by this
	//! MeshData
	MAKO_INLINE UInt32 GetNumPrimitives() const
	{ return numPrimitives; }

	//! At the primitive position, the renderer switches the material to m.
	MAKO_INLINE void SetSubMaterial(Material* m, UInt32 primitive)
	{
		if (subMaterials[primitive])
			subMaterials[primitive]->Drop();
		subMaterials[primitive] = m;
		m->Hold();
	}
	
	//! Get the sub Materials
	//! \return A map containing the primitive count as the key and the material
	//! as the data.
	MAKO_INLINE const Map<UInt32, Material*>& GetSubMaterials() const
	{ return subMaterials; }

	//! Get the sub Materials
	//! \return A map containing the primitive count as the key and the material
	//! as the data.
	MAKO_INLINE Map<UInt32, Material*>& GetSubMaterials()
	{ return subMaterials; }

	//! Clears all sub materials, then sets the new material as drawing from
	//! primitive 0
	MAKO_API void SetMaterial(Material* m);
};

MAKO_END_NAMESPACE