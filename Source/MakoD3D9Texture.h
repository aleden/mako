#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoTexture.h"
#include "MakoVec2d.h"
#include <d3d9.h>

MAKO_BEGIN_NAMESPACE

// Forward declarations
class GraphicsDevice;
class D3D9Device;

class D3D9Texture : public TextureHardwareBuffer
{
private:
	LPDIRECT3DTEXTURE9 d3d9tex;
	
	D3D9Device* gd;

	Texture* parent;
private:
	void CreateD3D9Texture();
public:
	D3D9Texture(D3D9Device* device, Texture* texture);
	~D3D9Texture();

	MAKO_INLINE Texture* GetParent()
	{ return parent; }

	MAKO_INLINE LPDIRECT3DTEXTURE9 GetIDirect3DTexture9()
	{ return d3d9tex; }

	void Update();
};

MAKO_END_NAMESPACE
#endif