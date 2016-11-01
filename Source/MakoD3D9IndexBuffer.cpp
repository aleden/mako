#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9IndexBuffer.h"
#include "MakoException.h"
#include "MakoD3D9Device.h"
#include "MakoApplication.h"
#include "MakoVertex.h"

MAKO_BEGIN_NAMESPACE

/////////////////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
D3D9IndexBuffer::D3D9IndexBuffer(GraphicsDevice* gd, IndexedMeshData* imb)
: d3d9ib(nullptr), imb(imb), gd(gd)
{
	CreateD3D9IB();
	UpdateD3D9IB();
}

D3D9IndexBuffer::~D3D9IndexBuffer()
{ if (d3d9ib) d3d9ib->Release(); }

/////////////////////////////////////////////////////////////////////////////
// Methods

void D3D9IndexBuffer::CreateD3D9IB()
{
	IndexedMeshData* imb = GetParent();

	if (imb->GetNumVertexBufferIndices() == 0)
		throw Exception(Text("There are no indices to create a D3D9 Index Buffer in \
							D3D9VertexBuffer::CreateD3D9IB()."));

	LPDIRECT3DDEVICE9 d3d9dev = static_cast<D3D9Device*>(gd)->GetIDirect3DDevice9();

	UInt32 usage = D3DUSAGE_WRITEONLY;

	if (FAILED(d3d9dev->CreateIndexBuffer
		(
			imb->GetNumVertexBufferIndices() * imb->GetVertexBufferIndexType(),
			usage,
			imb->GetVertexBufferIndexType() == VBIT_16 ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
			D3DPOOL_DEFAULT,
			&d3d9ib,
			nullptr
		)))
		throw Exception(Text("IDirect3DDevice9::CreateIndexBuffer() failed"));
}

void D3D9IndexBuffer::UpdateD3D9IB()
{
	IndexedMeshData* imb = static_cast<IndexedMeshData*>(GetParent());

	VOID* ib;
	// Lock d3d9 indices buffer and load the indices into it
	if (FAILED(d3d9ib->Lock(0, 0, (void**)&ib, 0)))
		throw Exception(Text("IDirect3dIndexBuffer9::Lock() failed."));

	memcpy(ib, imb->GetVertexBufferIndices(), imb->GetNumVertexBufferIndices() * imb->GetVertexBufferIndexType());

	if (FAILED(d3d9ib->Unlock()))
		throw Exception(Text("IDirect3dIndexBuffer9::Unlock() failed."));
}

MAKO_END_NAMESPACE
#endif