#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoGenericGraphicsDevice.h"
#include "MakoVec2d.h"
#include "MakoColor.h"
#include "MakoOS.h"
#include <d3d9.h>
#include <d3dx9.h>

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Material;
class Application;
class CgDevice;

class D3D9Device : public GenericGraphicsDevice
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	D3DPRESENT_PARAMETERS d3dpp;

	Color backgroundColor;
	ArrayList<bool> deviceOptions;

	LPD3DXSPRITE sprite;

	bool deviceLost;
	String vendornfo;

	Material* defaultmtl;

	CgDevice* cgdev;

	Position2d oldTex2dQuadGeometryPos;
	Rotation2d oldTex2dQuadGeometryRot;
	Size2d oldTex2dQuadGeometrySize;
public:
	D3D9Device(bool vsync);
	~D3D9Device();

	MAKO_INLINE LPDIRECT3DDEVICE9 GetIDirect3DDevice9() const { return d3ddev;   }
	MAKO_INLINE LPDIRECT3D9 GetIDirect3D9()             const { return d3d;      }
	MAKO_INLINE GRAPHICS_DEVICE_TYPE GetType()          const { return GDT_D3D9; }

	void BeginScene();
	void EndScene();
	void Reset();

	CgDevice* GetCgDevice() const
	{ return cgdev; }

	Material* GetDefaultMaterial();
	void SetDefaultMaterial(Material* mtl);

	MAKO_INLINE void SetBackgroundColor(const Color& color);

	void SetOption(GRAPHICS_DEVICE_OPTION option, bool b);
	
	MAKO_INLINE bool GetOption(GRAPHICS_DEVICE_OPTION option) const
	{ return deviceOptions[option]; }

	void SetTransform(const Matrix4f& mat, TRANSFORMATION_STATE ts);

	void SetTextureFilteringMode(TEXTURE_FILTER_MODE mode);
	TEXTURE_FILTER_MODE GetTextureFilteringMode();

	void SetTexAddressUMode(TEXTURE_ADDRESS_MODE mode);
	void SetTexAddressVMode(TEXTURE_ADDRESS_MODE mode);
	TEXTURE_ADDRESS_MODE GetTexAddressUMode();
	TEXTURE_ADDRESS_MODE GetTexAddressVMode();

	const String& GetGPUVendorInfo() const;

	String GetName() const;

	VertexHardwareBuffer* CreateVertexHardwareBuffer(MeshData* parent);
	IndexHardwareBuffer* CreateIndexHardwareBuffer(IndexedMeshData* parent);
	TextureHardwareBuffer* CreateTextureHardwareBuffer(Texture* parent);

	void DrawMeshData(MeshData* mb);
	void DrawIndexedMeshData(IndexedMeshData* mb);

	MeshData* CreateMeshData(const MeshDataCreationParams& params);
	MeshData* CreateIndexedMeshData(const IndexedMeshDataCreationParams& params);

	void Draw2dText(const Position2d& pos, const String& text, const Color& c,
		const UInt32 fontSize, const String& font);

	void Draw2dTexture(const Position2d& pos,
		Texture* tex, const Rotation2d& rot = Rot2d(0.f));

	void TakeScreenshot(const String& filePath, IMAGE_TYPE imageType = IMGT_PNG);
private:
	void SetTex2dQuadGeometry(const Position2d& pos, const Size2d& size, 
		const Rotation2d rot = Rot2d(0.f));

	void InitOptions();
	void InitIDirect3dDevice9();
	void Init2dDrawingCapability();
	void GetVendorInfo();
};

MAKO_END_NAMESPACE
#endif