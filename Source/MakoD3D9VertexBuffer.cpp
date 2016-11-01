#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9VertexBuffer.h"
#include "MakoException.h"
#include "MakoD3D9Device.h"
#include "MakoApplication.h"
#include "MakoVertex.h"

MAKO_BEGIN_NAMESPACE

/////////////////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
D3D9VertexBuffer::D3D9VertexBuffer(GraphicsDevice* gd, MeshData* mb)
: mb(mb), gd(gd), d3d9vb(nullptr)
{
	if (mb->GetNumVertices() == 0)
		throw Exception(Text("There are no vertices to create a D3D9 VB \
							 in D3D9VertexBuffer::CreateD3D9VB()."));

	LPDIRECT3DDEVICE9 d3d9dev = static_cast<D3D9Device*>(gd)->GetIDirect3DDevice9();
	
	UInt32 usage = 0;
	if (mb->GetFlags() & MBUO_VERTEX_BUFFER_DYNAMIC)
		usage |= D3DUSAGE_DYNAMIC;
	usage |= D3DUSAGE_WRITEONLY;

	if (FAILED(d3d9dev->CreateVertexBuffer
		(
			mb->GetNumVertices() * mb->GetVertexType(),
			usage,
			GetFVF(),
			D3DPOOL_DEFAULT,
			&d3d9vb,
			nullptr
		)))
		throw Exception(Text("IDirect3dDevice9::CreateVertexBuffer() failed"));

	Update();
}

D3D9VertexBuffer::~D3D9VertexBuffer()
{ if (d3d9vb) d3d9vb->Release(); }

/////////////////////////////////////////////////////////////////////////////
// Methods

void D3D9VertexBuffer::Update()
{
	VOID* vb;

	// Lock t_buffer and load the vertices into it
	
	// Can specify D3DLOCK_DISCARD or D3DLOCK_NOOVERWRITE for only Vertex Buffers created with D3DUSAGE_DYNAMIC
	if (FAILED(d3d9vb->Lock(0, 0, (void**)&vb, mb->GetFlags() & MBUO_VERTEX_BUFFER_DYNAMIC ? D3DLOCK_DISCARD : 0)))
		throw Exception(Text("IDirect3dVertexBuffer9::Lock() failed."));

	// D3D9 Vertex Buffer order of data
	// 1. Position          (FLOAT, FLOAT, FLOAT)
	// 2. RHW               (?)
	// 3. Blending weights  (?)
	// 4. Vertex Normal     (FLOAT, FLOAT, FLOAT)
	// 5. Vertex point size (?)
	// 6. Diffuse colour    (DWORD) [A8R8G8B8]
	// 7. Specular colour   (?)
	// 8. Texture co-ords   (FLOAT, FLOAT)

	memcpy(vb, mb->GetVertices(), mb->GetNumVertices() * mb->GetVertexType());

	//Int8* address = static_cast<Int8*>(pVoid);
	//// Loop to load vertices into VB
	//for (UInt16 i = 0; i < verts.size(); i++)
	//{
	//	// POSTON
	//	memcpy(address, &verts[i]->GetPosition().x, sizeof(Float32));
	//	address += sizeof(Float32);
	//	memcpy(address, &verts[i]->GetPosition().y, sizeof(Float32));
	//	address += sizeof(Float32);
	//	memcpy(address, &verts[i]->GetPosition().z, sizeof(Float32));
	//	address += sizeof(Float32);

	//	#ifdef MAKO_VERTEX_NORMALS
	//	// NORMAL
	//	memcpy(address, &verts[i].GetNormal().x, sizeof(Float32));
	//	address += sizeof(Float32);
	//	memcpy(address, &verts[i].GetNormal().y, sizeof(Float32));
	//	address += sizeof(Float32);
	//	memcpy(address, &verts[i].GetNormal().z, sizeof(Float32));
	//	address += sizeof(Float32);
	//	#endif

	//	//// VERTEX COLOR
	//	//UInt32 c = verts[i].GetColor().GetA8R8G8B8Format();
	//	//memcpy(address, &c, sizeof(UInt32));
	//	//address += sizeof(UInt32);

	//	// TEX COORDS
	//	if (verts[i]->GetType() == VT_STANDARD)
	//	{
	//		memcpy(address, &verts[i]->GetTCoord0(1).x, sizeof(Float32));
	//		address += sizeof(Float32);
	//		memcpy(address, &verts[i]->GetTCoord0(1).y, sizeof(Float32));
	//		address += sizeof(Float32);
	//	}
	//}

	if (FAILED(d3d9vb->Unlock()))
		throw Exception(Text("IDirect3dVertexBuffer9::Unlock() failed."));
}

DWORD D3D9VertexBuffer::GetFVF()
{
	DWORD fvf = D3DFVF_XYZ;
	if (mb->GetVertexType() == VT_STANDARD)
		fvf |= D3DFVF_TEX1;
	else if (mb->GetVertexType() == VT_T2)
		fvf |= D3DFVF_TEX2;
	return fvf;
}

MAKO_END_NAMESPACE
#endif