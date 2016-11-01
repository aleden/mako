#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9Device.h"
#include "MakoD3D9Texture.h"
#include "MakoException.h"
#include "MakoApplication.h"
#include "MakoConsole.h"
#include <d3dx9.h>

MAKO_BEGIN_NAMESPACE

D3D9Texture::D3D9Texture(D3D9Device* gd, Texture* texture)
: gd(gd), parent(texture)
{
	D3DFORMAT format;
	switch (texture->GetColorFormat())
	{
	case CF_R8G8B8A8:
		format = D3DFMT_A8R8G8B8;
		break;
	}

	HRESULT hr;
	if (FAILED(hr = gd->GetIDirect3DDevice9()->CreateTexture
		(
			texture->GetSize().x,
			texture->GetSize().y,
			1,
			D3DUSAGE_AUTOGENMIPMAP,
			format,
			D3DPOOL_MANAGED,
			&d3d9tex,
			nullptr
		)))
		throw Exception(Text("IDirect3DDevice9::CreateTexture() failed"), hr);

	Update();
}

D3D9Texture::~D3D9Texture()
{
	if (d3d9tex)
		d3d9tex->Release();
}

void D3D9Texture::Update()
{
	D3DLOCKED_RECT rect;
	if (FAILED(d3d9tex->LockRect(0, &rect, nullptr, 0)))
		throw Exception(Text("IDirect3DTexture9::LockRect() failed"));

	switch (parent->GetColorFormat())
	{
	case CF_R8G8B8A8:
		{
			for (UInt32* out = (UInt32*)rect.pBits, * in = (UInt32*)parent->GetImageData(); in != parent->GetImageDataEnd(); ++in, ++out)
				*out = (((UByte*)in)[3] << 24) | (((UByte*)in)[0] << 16) | (((UByte*)in)[1] << 8) | (((UByte*)in)[2]);
			break;
		}
	default:
		{
			throw Exception(Text("Failed to identify color format in D3D9Texture::Update()."));
		}
	}

	if (FAILED(d3d9tex->UnlockRect(0)))
		throw Exception(Text("IDirect3DTexture9::UnlockRect() failed"));

	d3d9tex->GenerateMipSubLevels();
}

MAKO_END_NAMESPACE
#endif