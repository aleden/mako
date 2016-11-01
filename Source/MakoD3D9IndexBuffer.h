#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoCommon.h"
#include "MakoOS.h"
#include <d3d9.h>
#include "MakoD3D9VertexBuffer.h"
#include "MakoColorMtl.h"
#include "MakoColor.h"
#include "MakoArrayList.h"
#include "MakoVertex.h"
#include "MakoMap.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class GraphicsDevice;

class D3D9IndexBuffer : public IndexHardwareBuffer
{
private:
	IndexedMeshData* imb;
	GraphicsDevice* gd;

	LPDIRECT3DINDEXBUFFER9 d3d9ib;
	void CreateD3D9IB();
	void UpdateD3D9IB();
public:
	D3D9IndexBuffer(GraphicsDevice* gd, IndexedMeshData* imb);
	~D3D9IndexBuffer();

	MAKO_INLINE const LPDIRECT3DINDEXBUFFER9 GetD3D9IndexBuffer() const
	{ return d3d9ib; }

	void Update()
	{ UpdateD3D9IB(); }

	IndexedMeshData* GetParent()
	{ return imb; }
};

MAKO_END_NAMESPACE
#endif