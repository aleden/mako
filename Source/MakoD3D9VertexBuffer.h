#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoOS.h"
#include <d3d9.h>
#include "MakoMeshData.h"
#include "MakoColorMtl.h"
#include "MakoColor.h"
#include "MakoArrayList.h"
#include "MakoVertex.h"
#include "MakoMap.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

class D3D9VertexBuffer : public VertexHardwareBuffer
{
private:
	MeshData* mb;
	
	LPDIRECT3DVERTEXBUFFER9 d3d9vb;
protected:
	GraphicsDevice* gd;
private:
	void CreateD3D9VB();
public:
	D3D9VertexBuffer(GraphicsDevice* gd, MeshData* mb);
	~D3D9VertexBuffer();

	DWORD GetFVF();
	MAKO_INLINE const LPDIRECT3DVERTEXBUFFER9 GetD3D9VertexBuffer() const
	{ return d3d9vb; }

	MeshData* GetParent()
	{ return mb; }

	virtual void Update();
};

MAKO_END_NAMESPACE
#endif