#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9Device.h"
#include "MakoD3D9Texture.h"
#include "MakoColor.h"
#include "MakoException.h"
#include "MakoD3D9VertexBuffer.h"
#include "MakoD3D9IndexBuffer.h"
#include "MakoMesh.h"
#include "MakoMaterial.h"
#include "MakoApplication.h"
#include "MakoScene3d.h"
#include "MakoCamera.h"
#include "MakoMeshSceneNode.h"
#include "MakoWindowsDevice.h"
#include "MakoString.h"
#include "MakoConsole.h"
#include "Makomath.h"
#include "MakoUtilities.h"
#include "MakoD3D9CgDevice.h"

MAKO_BEGIN_NAMESPACE

//#pragma comment (lib, "dxerr.lib")
//#pragma comment (lib, "dxguid.lib")
//#if (defined (DEBUG) || defined (_DEBUG)) && defined (MAKO_DEBUG_D3D)
//#pragma comment (lib, "d3dx9d.lib")
//#else
//#pragma comment (lib, "d3dx9.lib")
//#endif
//#pragma comment (lib, "d3d9.lib")

/////////////////////////////////////////////////////////////////////////
// Testing d3d function hresults macros
#define LOG_IF_D3D9FUNC_FAILED(thing, functionName)\
	if (FAILED(thing)) APP()->GetConsole()->Log(LL_LOW, ToString(L"IDirect3d9::" functionName L"() failed."));

#define EXC_IF_D3D9FUNC_FAILED(thing, functionName)\
	if (FAILED(thing)) throw Exception(ToString(L"IDirect3d9::" functionName L"() failed."));

#define EXC_IF_D3D9GLOFUNC_FAILED(thing, functionName)\
	if (FAILED(thing)) throw Exception(ToString(functionName L"() failed."));

#define EXC_IF_HR_FAILEDMSG(thing, msg)\
	if (FAILED(thing)) throw Exception(msg);

D3D9Device::D3D9Device(bool vsync)
: deviceOptions(GDO_ENUM_LENGTH), oldTex2dQuadGeometryPos(0, 0), oldTex2dQuadGeometryRot(0.f),
  oldTex2dQuadGeometrySize(0, 0), GenericGraphicsDevice(vsync), defaultmtl(nullptr)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	InitIDirect3dDevice9();
	SetTextureFilteringMode(TFM_BILINEAR);
	InitOptions();
	Init2dDrawingCapability();

	// Default background color is a blue
	SetBackgroundColor(Color(0, 0, 0, 255));

	cgdev = new D3D9CgDevice(this);
}

void D3D9Device::InitIDirect3dDevice9()
{
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	// Discard old frames
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// Set the window to be used by Direct3d
	d3dpp.hDeviceWindow = static_cast<WindowsRenderedWindow*>(APP()->GetRenderedWindow())->GetHWnd();

	// Enable zbuffer
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval   = IsVSyncEnabled() ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	if (!APP()->GetRenderedWindow()->IsFullscreen())
	{
		// Program windowed
		d3dpp.Windowed = TRUE;
	}
	else
	{
		// Program fullscreen, not windowed
		d3dpp.Windowed = FALSE;
		// Set the back buffer format to 32-bit
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		// Set the width of the buffer
		d3dpp.BackBufferWidth = APP()->GetRenderedWindow()->GetWindowSize().x;
		// Set the height of the buffer
		d3dpp.BackBufferHeight = APP()->GetRenderedWindow()->GetWindowSize().y;
	}


	EXC_IF_D3D9FUNC_FAILED(d3d->CreateDevice
		(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		static_cast<WindowsRenderedWindow*>(APP()->GetRenderedWindow())->GetHWnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
		), L"CreateDevice");
}

void D3D9Device::InitOptions()
{
	deviceOptions[GDO_WIREFRAME] = false;
	deviceOptions[GDO_ZBUFFER]   = true;

	// Wireframe is turned off by default.
	// ...

	// Turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void D3D9Device::Init2dDrawingCapability()
{
	EXC_IF_D3D9GLOFUNC_FAILED(D3DXCreateSprite(d3ddev, &sprite), L"D3DXCreateSprite");
}

void D3D9Device::GetVendorInfo()
{
	// Store GPU vendor info
	D3DADAPTER_IDENTIFIER9 dai;
	if (!FAILED(d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &dai)))
	{
		wchar_t tmp[512];

		Int32 Product = HIWORD(dai.DriverVersion.HighPart);
		Int32 Version = LOWORD(dai.DriverVersion.HighPart);
		Int32 SubVersion = HIWORD(dai.DriverVersion.LowPart);
		Int32 Build = LOWORD(dai.DriverVersion.LowPart);

		swprintf_s(tmp, 512, L"%s %s %d.%d.%d.%d", ToString(dai.Description).Trim().GetData(), 
			ToString(dai.Driver).Trim().GetData(), Product, Version,
			SubVersion, Build);

		vendornfo = ToString(tmp);

		//// AssignTo vendor name based on vendor id.
		//switch(dai.Vendord)
		//{
		//case 0x1002 : vendornfo += L" ATI Technologies Inc."; break;
		//case 0x10DE : vendornfo += L" NVIDIA Corporation"; break;
		//case 0x102B : vendornfo += L" Matrox Electronic Systems Ltd."; break;
		//case 0x121A : vendornfo += L" 3dfx Interactive Inc"; break;
		//case 0x5333 : vendornfo += L" S3 Graphics Co., Ltd."; break;
		//case 0x8086 : vendornfo += L" Intel Corporation"; break;
		//default: vendornfo += L" Unknown Vendor Id: "; vendornfo += ToString((UInt32)dai.VendorId); break;
		//}
	}
}

D3D9Device::~D3D9Device()
{
	if (defaultmtl)
		defaultmtl->Drop();
	if (d3ddev)
		d3ddev->SetStreamSource(0, nullptr, 0, 0);
	if (d3ddev)
		d3ddev->SetVertexShader(nullptr);
	if (d3ddev)
		d3ddev->SetPixelShader(nullptr);

	if (cgdev)
		delete cgdev;
	if (sprite)
	{
		sprite->Release();
		sprite = nullptr;
	}
	if (d3ddev)
	{
		d3ddev->Release();
		d3ddev = nullptr;
	}
	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void D3D9Device::SetTransform(const Matrix4f& mat, TRANSFORMATION_STATE ts)
{
	GenericGraphicsDevice::SetTransform(mat, ts);
	
	switch (ts)
	{
	case TS_VIEW:
		{
			////////////////////////////
			// Set the view transform
			LOG_IF_D3D9FUNC_FAILED(d3ddev->SetTransform(D3DTS_VIEW, &D3DXMATRIX(mat.Pointer())), L"SetTransform");
			return ;
		}
	case TS_PROJECTION:
		{
			////////////////////////////
			// Set the projection transform
			LOG_IF_D3D9FUNC_FAILED(d3ddev->SetTransform(D3DTS_PROJECTION, &D3DXMATRIX(mat.Pointer())), L"SetTransform");
			return ;
		}
	case TS_WORLD:
		{
			////////////////////////////
			// Set the world transform
			LOG_IF_D3D9FUNC_FAILED(d3ddev->SetTransform(D3DTS_WORLD, &D3DXMATRIX(mat.Pointer())), L"SetTransform");
			return ;
		}
	}
}

Material* D3D9Device::GetDefaultMaterial()
{ return defaultmtl; }

void D3D9Device::SetDefaultMaterial(Material* mtl)
{ if (defaultmtl) defaultmtl->Drop(); defaultmtl = mtl; defaultmtl->Hold(); }

void D3D9Device::SetOption(GRAPHICS_DEVICE_OPTION option, bool b)
{
	deviceOptions[option] = b;
	switch (option)
	{
	case GDO_WIREFRAME:
		{
			d3ddev->SetRenderState(D3DRS_FILLMODE, b ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
			return ;
		}
	case GDO_ZBUFFER:
		{
			d3ddev->SetRenderState(D3DRS_ZENABLE, b ? D3DZB_TRUE : D3DZB_FALSE);
			return ;
		}
	}
}
void D3D9Device::BeginScene()
{
	// Clear the window to background color
	d3ddev->Clear(0, nullptr, D3DCLEAR_TARGET, backgroundColor.GetB8G8R8A8Format(), 1.0f, 0);
	d3ddev->Clear(0, nullptr, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	LOG_IF_D3D9FUNC_FAILED(d3ddev->BeginScene(), L"BeginScene");

	Vec3df campos, camtarget;
	Float32 fov, nearviewplane, farviewplane;
	campos    = APP()->GetActive3dScene()->GetCamera()->GetPosition();
	camtarget = APP()->GetActive3dScene()->GetCamera()->GetTarget();

	fov           = APP()->GetActive3dScene()->GetCamera()->GetFOV();
	nearviewplane = APP()->GetActive3dScene()->GetCamera()->GetNearViewPlane();
	farviewplane  = APP()->GetActive3dScene()->GetCamera()->GetFarViewPlane();

	////////////////////////////////////////////////////////////////////////////////////////
	// Set the view transform
	
	GetTransform(TS_VIEW).BuildCameraLookAtMatrixLH(campos, camtarget, Vec3df(0.f, 1.f, 0.f));
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetTransform(D3DTS_VIEW, &D3DXMATRIX(GetTransform(TS_VIEW).Pointer())), L"SetTransform");

	///////////////////////////////////////////////////////////////////////////////////////
	// Set the projection transform
	
	GetTransform(TS_PROJECTION).BuildProjectionMatrixPerspectiveFovLH
		(
			fov,
			(Float32)APP()->GetRenderedWindow()->GetWindowSize().x/ // Aspect ratio
			(Float32)APP()->GetRenderedWindow()->GetWindowSize().y,
			nearviewplane,
			farviewplane
		);
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetTransform(D3DTS_PROJECTION, &D3DXMATRIX(GetTransform(TS_PROJECTION).Pointer())), L"SetTransform");
}
void D3D9Device::EndScene()
{
	LOG_IF_D3D9FUNC_FAILED(d3ddev->EndScene(), L"EndScene");
	LOG_IF_D3D9FUNC_FAILED(d3ddev->Present(nullptr, nullptr, nullptr, nullptr), L"Present");
}

void D3D9Device::Reset()
{
	HRESULT hr = d3ddev->Reset(&d3dpp);
	deviceLost = false;
}
void D3D9Device::SetBackgroundColor(const Color& color)
{
	backgroundColor = color;
}

void D3D9Device::TakeScreenshot(const String& filePath, IMAGE_TYPE imageType)
{
	LPDIRECT3DSURFACE9 pFrontBuffer;   
	LOG_IF_D3D9FUNC_FAILED
		(
			d3ddev->CreateOffscreenPlainSurface(640, 480, D3DFMT_R5G6B5, D3DPOOL_SYSTEMMEM, &pFrontBuffer, nullptr),
			L"CreateOffscreenPlainSurface"
		);

	LOG_IF_D3D9FUNC_FAILED
		(
			d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pFrontBuffer),
			L"GetBackBuffer"
		);
	EXC_IF_D3D9GLOFUNC_FAILED
		(
			D3DXSaveSurfaceToFile(filePath.ToWStringData(), static_cast<_D3DXIMAGE_FILEFORMAT>(imageType), pFrontBuffer, nullptr, nullptr),
			L"D3DXSaveSurfaceToFileW"
		);
}

VertexHardwareBuffer* D3D9Device::CreateVertexHardwareBuffer(MeshData* parent)
{ return new D3D9VertexBuffer(this, parent); }

IndexHardwareBuffer* D3D9Device::CreateIndexHardwareBuffer(IndexedMeshData* parent)
{ return new D3D9IndexBuffer(this, parent); }

TextureHardwareBuffer* D3D9Device::CreateTextureHardwareBuffer(Texture* parent)
{ return new D3D9Texture(this, parent); }

void D3D9Device::SetTextureFilteringMode(TEXTURE_FILTER_MODE mode)
{
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, mode), L"SetSamplerState");
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, mode), L"SetSamplerState");
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, mode), L"SetSamplerState");
}

TEXTURE_FILTER_MODE D3D9Device::GetTextureFilteringMode()
{
	DWORD val;
	LOG_IF_D3D9FUNC_FAILED(d3ddev->GetSamplerState(0, D3DSAMP_MINFILTER, &val),
		L"GetSamplerState");
	return static_cast<TEXTURE_FILTER_MODE>(val);
}

void D3D9Device::SetTexAddressUMode(TEXTURE_ADDRESS_MODE mode)
{ LOG_IF_D3D9FUNC_FAILED(d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, mode), L"SetSamplerState"); }

void D3D9Device::SetTexAddressVMode(TEXTURE_ADDRESS_MODE mode)
{ LOG_IF_D3D9FUNC_FAILED(d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, mode), L"SetSamplerState"); }

TEXTURE_ADDRESS_MODE D3D9Device::GetTexAddressUMode()
{
	DWORD val;
	LOG_IF_D3D9FUNC_FAILED(d3ddev->GetSamplerState(0, D3DSAMP_ADDRESSU, &val),
		L"GetSamplerState");
	return static_cast<TEXTURE_ADDRESS_MODE>(val);
}

TEXTURE_ADDRESS_MODE D3D9Device::GetTexAddressVMode()
{
	DWORD val;
	LOG_IF_D3D9FUNC_FAILED(d3ddev->GetSamplerState(0, D3DSAMP_ADDRESSV, &val),
		L"GetSamplerState");
	return static_cast<TEXTURE_ADDRESS_MODE>(val);
}

const String& D3D9Device::GetGPUVendorInfo() const
{ return vendornfo; }

String D3D9Device::GetName() const
{ return String(Text("Direct3D 9")); }

void D3D9Device::DrawMeshData(MeshData* mb)
{
	if (mb->IsIndexed())
		return DrawIndexedMeshData(static_cast<IndexedMeshData*>(mb));

	/*
	sudo code
	Map<UInt32 primitivePosition, Material* material> subMaterials

	UInt32 primitvesDrawn = 0
	for every pair in subMaterials
		SetMaterial(pair.second)
		DrawPrimtives(CalculateVertexPositionFromPrimitiveCount(primitvesDrawn), next_pair.first));
		primitvesDrawn += pair.first
	*/
	
	// Select which vertex format we are using
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetFVF
		(
			static_cast<D3D9VertexBuffer*>(mb->GetVertexHardwareBuffer())->GetFVF()
		), L"SetFVF");

	// Tell D3D9 where the vertices are
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetStreamSource
		(
		0,
		static_cast<D3D9VertexBuffer*>(mb->GetVertexHardwareBuffer())->GetD3D9VertexBuffer(),
		0,
		mb->GetVertexType()
		), L"SetStreamSource");

	const Map<UInt32, Material*>& submats = mb->GetSubMaterials();
	
	UInt32 primsDrawn = 0;
	typedef Map<UInt32, Material*>::const_iterator submatsIt;
	
	for (submatsIt it = submats.begin(); it != submats.end(); ++it)
	{
		(*it).second->Bind(this);
		
		UInt32 vertPosition = CalcVertPosFromPrimCount(primsDrawn, mb->GetPrimitiveType());
		// The MinVertexIndex and NumVertices values are really just hints to help 
		// Direct3D optimize memory access during software vertex processing, 
		// and could simply be set to include the entire vertex buffer at the 
		// price of performance.
		
		// Prims to draw is equal to the next pair's primitive position minus this pair's
		// primitive position
		UInt32 primsToDraw;
		if ((++it) != submats.end())
		{
			--it;
			primsToDraw = (*(++it)).first - (*(--it)).first;
		}
		else
		{
			--it;
			primsToDraw = mb->GetNumPrimitives() - (*it).first;
		}

		LOG_IF_D3D9FUNC_FAILED(d3ddev->DrawPrimitive
		(
			static_cast<D3DPRIMITIVETYPE>(mb->GetPrimitiveType()),
			vertPosition,
			primsToDraw
		), L"DrawIndexedPrimitive");
		
		primsDrawn += primsToDraw;

		(*it).second->UnBind(this);
	}
}

void D3D9Device::DrawIndexedMeshData(IndexedMeshData* mb)
{	/*
	sudo code
	
	Map<UInt32 primitivePosition, Material* material> subMaterials

	// Contents of subMaterials...
	// 0, *
	// 2, *
	// 6, *
	
	totalPrims = 12
	UInt32 primsToDraw = (next_pair.first - pair.first);
	UInt32 primitvesDrawn = 0
	for every pair in subMaterials
		SetMaterial(pair.second)
		DrawIndexedPrimtives(CalculateIndexPositionFromPrimitiveCount(primitvesDrawn), primsToDraw));
		primitvesDrawn += primsToDraw
	*/
	
	// Select which vertex format we are using
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetFVF
		(
		static_cast<D3D9VertexBuffer*>(mb->GetVertexHardwareBuffer())->GetFVF()
		), L"SetFVF");

	// Tell D3D9 where the indices are
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetIndices
		(
			static_cast<D3D9IndexBuffer*>(mb->GetIndexHardwareBuffer())->GetD3D9IndexBuffer()
		), L"SetIndices");

	// Tell D3D9 where the vertices are
	LOG_IF_D3D9FUNC_FAILED(d3ddev->SetStreamSource
		(
			0,
			static_cast<D3D9VertexBuffer*>(mb->GetVertexHardwareBuffer())->GetD3D9VertexBuffer(),
			0,
			mb->GetVertexType()
		), L"SetStreamSource");

	const Map<UInt32, Material*>& submats = mb->GetSubMaterials();
	
	UInt32 primsDrawn = 0;
	typedef Map<UInt32, Material*>::const_iterator submatsIt;
	
	for (submatsIt it = submats.begin(); it != submats.end(); ++it)
	{
		(*it).second->Bind(this);
		
		UInt32 indexPosition = CalcVBIndexPosFromPrimCount(primsDrawn, mb->GetPrimitiveType());
		// The MinVertexIndex and NumVertices values are really just hints to help 
		// Direct3D optimize memory access during software vertex processing, 
		// and could simply be set to include the entire vertex buffer at the 
		// price of performance.
		
		// Prims to draw is equal to the next pair's primitive position minus this pair's
		// primitive position
		UInt32 primsToDraw;
		if ((++it) != submats.end())
		{
			--it;
			primsToDraw = (*(++it)).first - (*(--it)).first;
		}
		else
		{
			--it;
			primsToDraw = mb->GetNumPrimitives() - (*it).first;
		}

		LOG_IF_D3D9FUNC_FAILED(d3ddev->DrawIndexedPrimitive
		(
			static_cast<D3DPRIMITIVETYPE>(mb->GetPrimitiveType()),
			0, // This value is ADDED to the vbindices' values
			0,
			mb->GetNumVertices(),
			indexPosition,
			primsToDraw
			//mb->GetNumPrimitives()
		), L"DrawIndexedPrimitive");
		
		primsDrawn += primsToDraw;

		(*it).second->UnBind(this);
	}
}

void D3D9Device::Draw2dTexture(const Position2d& pos, Texture* tex, const Rotation2d& rot)
{
	EXC_IF_D3D9GLOFUNC_FAILED(sprite->Begin(D3DXSPRITE_ALPHABLEND), L"D3DXSprite::Begin");

	D3DXVECTOR2 d3dvspriteCentre = D3DXVECTOR2((Float32)tex->GetSize().x/2, (Float32)tex->GetSize().y/2);

	// Screen position of the sprite
	D3DXVECTOR2 d3dvpos((Float32)pos.x, (Float32)pos.y);
	D3DXVECTOR2 d3dvscaling(1.0f,1.0f);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat, nullptr, 0.f, &d3dvscaling, &d3dvspriteCentre, rot, &d3dvpos);

	// Tell the sprite about the matrix
	sprite->SetTransform(&mat);

	// Draw the sprite 
	sprite->Draw(static_cast<D3D9Texture*>(tex->GetTextureHardwareBuffer())->GetIDirect3DTexture9(), nullptr, nullptr, nullptr, 0xFFFFFFFF);

	EXC_IF_D3D9GLOFUNC_FAILED(sprite->End(), L"D3DXSprite::End");;
}

void D3D9Device::Draw2dText(const Position2d& pos, const String& text, const Color& c, const UInt32 fontSize,
							const String& font)
{}

MAKO_END_NAMESPACE
#endif