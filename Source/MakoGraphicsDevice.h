#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoVec3d.h"
#include "MakoVec2d.h"
#include "MakoMeshData.h"
#include "MakoIndexedMeshData.h"
#include "MakoArrayList.h"
#include "MakoReferenceCounted.h"
#include "MakoDevice.h"
#include "MakoMatrix4.h"
#include "MakoMap.h"
#include "MakoColor.h"
#include "MakoTexture.h"
#include "MakoFilePath.h"

MAKO_BEGIN_NAMESPACE

////////////////////////////////////////////////////
// Enums

// Forward declarations
class InputStream;
class Scene3d;
class Mesh;
class Application;
class Material;
class CgDevice;
class Font;

//! Graphics device option
enum GRAPHICS_DEVICE_OPTION
{
	GDO_WIREFRAME,
	GDO_ZBUFFER,
	GDO_ENUM_LENGTH
};

//! Graphics device type
enum GRAPHICS_DEVICE_TYPE
{
#ifdef MAKO_D3D_AVAILABLE
	GDT_D3D9,
#endif
	GDT_ENUM_LENGTH
};

//! Texture filtering is the method used to determine the texture color 
//! for a texture mapped pixel, using the colors of nearby texels 
//! (pixels of the texture). In short, it blends the texture pixels 
//! together by breaking them up into tinier pixels. Another term for 
//! texture filtering is called texture smoothing. There are many methods 
//! of texture filtering, which make different trade-offs between 
//! computational complexity and image quality. Since texture filtering 
//! is an attempt to find a value at some point given a set of discrete 
//! samples at nearby points, it is a form of interpolation.
//! 
//! The default texture filtering used is TFM_BILINEAR.
//!
//! Note: The values of each of these enums are equal to the D3D enum
//! D3DTEXTUREFILTERTYPE's values.
enum TEXTURE_FILTER_MODE
{
	//! No mip-mapping, no texture filtering
	TFM_NONE,
	TFM_POINT,
	TFM_BILINEAR,
	TFM_ANISOTROPIC,
	TFM_ENUM_LENGTH
};

//! Note: The values of each of these enums are equal to the D3D enum
//! D3DTEXTUREADDRESS's values.
enum TEXTURE_ADDRESS_MODE
{
	//! Tile the texture at every integer junction. For example, for 
	//! u values between 0 and 3, the texture is repeated three times; 
	//! no mirroring is performed.
	TAM_WRAP,

	//! Similar to TAD_WRAP, except that the texture is flipped 
	//! at every integer junction. For u values between 0 and 1, for 
	//! example, the texture is addressed normally; between 1 and 2, the 
	//! texture is flipped (mirrored); between 2 and 3, the texture is 
	//! normal again; and so on.	
	TAM_MIRROR,

	//! Texture coordinates outside the range [0.0, 1.0] are set to the 
	//! texture color at 0.0 or 1.0, respectively.
	TAM_CLAMP,

	//! Texture coordinates outside the range [0.0, 1.0] are set to the 
	//! border color.
	TAM_BORDER,

	//! Similar to TAD_MIRROR and TAD_CLAMP. Takes the absolute value 
	//! of the texture coordinate (thus, mirroring around 0), and then 
	//! clamps to the maximum value. The most common usage is for volume 
	//! textures, where support for the full TAD_MIRRORONCE 
	//! texture-addressing mode is not necessary, but the data is symmetric 
	//! around the one axis.
	TAM_MIRRORONCE,
	TAM_ENUM_LENGTH
};


enum MESH_TYPE
{
#if 0
	MT_OBJ,
#endif
	MT_MAKOMESH,
	MT_ENUM_LENGTH
};

enum TEXTURE_TYPE
{
	TT_JPEG,
	TT_PNG,
	TT_ENUM_LENGTH
};

//! Types of images supported by Mako
//! Protip: these values for the enums are
//! identical to the d3d counter-parts.
enum IMAGE_TYPE
{ IMGT_BMP, IMGT_JPG, IMGT_TGA, IMGT_PNG, IMGT_DDS, IMGT_ENUM_LENGTH };

//! Geometry transformation states 
enum TRANSFORMATION_STATE
{
	//! View transformation
	TS_VIEW,
	//! World transformation
	TS_WORLD,
	//! Projection transformation
	TS_PROJECTION,
	TS_ENUM_LENGTH
};

//! This abstract class provides 2d and 3d rendering.
class GraphicsDevice : public Device
{
	friend class MeshData;
	friend class IndexedMeshData;
	friend class Texture;
private:
	//! Create a VertexHardwareBuffer. This is used in the MeshData class. Clients
	//! should not use this function.
	virtual VertexHardwareBuffer* CreateVertexHardwareBuffer(MeshData* parent) = 0;

	//! Create a IndexHardwareBuffer. This is used in the MeshData class. Clients
	//! should not use this function.
	virtual IndexHardwareBuffer* CreateIndexHardwareBuffer(IndexedMeshData* parent) = 0;

	//! Create a TextureHardwareBuffer. This is used in the Texture class. Clients
	//! should not use this function.
	virtual TextureHardwareBuffer* CreateTextureHardwareBuffer(Texture* parent) = 0;
public:
	///////////////////////////////////////////////////////////////////////////////////
	// Core Functions

	//! This is called before any rendering in one frame
	virtual void BeginScene() = 0;
	
	//! This is called after any rendering in one frame
	virtual void EndScene() = 0;
	
	//! This will reset certain properties of a graphics device sepecific
	//! to it's implementation
	virtual void Reset() = 0;
	
	//! Get the graphics device's implementation's type
	virtual GRAPHICS_DEVICE_TYPE GetType() const = 0;

	//! Get the current transformation
	virtual const Matrix4f& GetTransform(TRANSFORMATION_STATE ts) const = 0;
	
	//! Set the current transformation
	virtual void SetTransform(const Matrix4f& mat, TRANSFORMATION_STATE ts) = 0;

	//! Get the CgDevice
	virtual CgDevice* GetCgDevice() const = 0;

	///////////////////////////////////////////////////////////////////////////////////
	// Misc Functions

	//! Get the default material that will be assigned to meshes with no material
	//! specified. By default, the default material is a White ColorMtl.
	virtual Material* GetDefaultMaterial() = 0;

	//! Set the default material that will be assigned to meshes with no material
	//! specified
	//! \param[in] mtl The new default material
	virtual void SetDefaultMaterial(Material* mtl) = 0;

	//! Set the background color when rendering
	//! \param[in] color The color to set to be the background color
	virtual void SetBackgroundColor(const Color& color) = 0;

	//! Create a font from a file. The file must be a TrueType font.
	//! \param[in] filepath The file path of the font file.
	//! \return The newly created Font.
	virtual Font* LoadFontFromFile(const FilePath& filepath) = 0;

	//! Create a font from an InputStream.
	//! \param[in] istream The InputStream to read from in order to generate a 
	//! file
	//! \return The newly created Font.
	virtual Font* LoadFont(InputStream* istream) = 0;

	//! Loads a texture from a file. This overload provides an additional 
	//! parameter to specify the format of the texture file, regardless 
	//! of it's extension.
	//! \param[in] fileName The file path of the texture
	//! \param[in] texType The format of the texture
	//! \return The loaded texture.
	virtual Texture* LoadTextureFromFile(const FilePath& fileName,
		TEXTURE_TYPE texType) = 0;
	
	//! Loads a mesh from a file. This overload provides an additional 
	//! parameter to specify the format of the mesh file, regardless 
	//! of it's extension.
	//! \param[in] fileName The file path of the mesh
	//! \param[in] ml The format of the mesh
	//! \return The loaded mesh.
	virtual Mesh* LoadMeshFromFile(const FilePath& fileName,
		MESH_TYPE mt) = 0;

	//! Loads a texture from a file.This method will auto-detect the
	//! texture file's format by it's extension.
	//! \param[in] fileName The file path of the texture
	//! \return The loaded texture.
	virtual Texture* LoadTextureFromFile(const FilePath& fileName) = 0;
	
	//! Loads a mesh from a file. This method will auto-detect the mesh 
	//! file's format by it's extension.
	//! \param[in] fileName The file path of the mesh
	//! \return The loaded mesh.
	virtual Mesh* LoadMeshFromFile(const FilePath& fileName) = 0;

	//! Set the new texture filtering mode
	//! \param[in] mode The new texture filtering mode
	virtual void SetTextureFilteringMode(TEXTURE_FILTER_MODE mode) = 0;
	
	//! Get the texture filtering mode
	//! \return The filtering mode
	virtual TEXTURE_FILTER_MODE GetTextureFilteringMode() = 0;

	//! Set the texture addressing mode for affecting U coordinates
	virtual void SetTexAddressUMode(TEXTURE_ADDRESS_MODE mode) = 0;
	
	//! Set the texture addressing mode for affecting V coordinates
	virtual void SetTexAddressVMode(TEXTURE_ADDRESS_MODE mode) = 0;

	//! Set the texture addressing mode for affecting U coordinates
	virtual TEXTURE_ADDRESS_MODE GetTexAddressUMode() = 0;
	
	//! Set the texture addressing mode for affecting V coordinates
	virtual TEXTURE_ADDRESS_MODE GetTexAddressVMode() = 0;
	
	//! Set an option to true or false
	//! \param[in] option The option to modify
	//! \param[in] b The new status
	virtual void SetOption(GRAPHICS_DEVICE_OPTION option, bool b) = 0;
	
	//! Check an option's status
	virtual bool GetOption(GRAPHICS_DEVICE_OPTION option) const = 0;

	//! Check if VSync (vertical syncronization) is enabled.
	virtual bool IsVSyncEnabled() const = 0;

	//! Get the GPU vendor info
	//! \todo Change so it works for multiple GPUs
	virtual const String& GetGPUVendorInfo() const = 0;

	//! Take a screenshot of the current screen
	//! \param[in] filePath The file path of the screen shot that it is to 
	//! be saved to.
	virtual void TakeScreenshot(const String& filePath,
		                        IMAGE_TYPE imageType = IMGT_PNG) = 0;
	//! Draws text
	//! \parma[in] text The text to draw
	//! \param[in] font The font to draw the text with
	//! \param[in] pos The screen position of where to draw the text, up-left
	//! centered.
	virtual void Draw2dText(const String& text, Font* font, const Pos2d& pos) = 0;
	
	//! Create a MeshData.
	MAKO_INLINE MeshData* CreateMeshData(const MeshDataCreationParams& params)
	{ return new MeshData(this, params); }

	//! Create an IndexedMeshData.
	MAKO_INLINE MeshData* CreateIndexedMeshData(const IndexedMeshDataCreationParams& params)
	{ return new IndexedMeshData(this, params); }

	//! Create a MeshData.
	MAKO_INLINE Texture* CreateTexture(const TextureCreationParams& params)
	{ return new Texture(this, params); }
	
	///////////////////////////////////////////////////////////////////////////////////
	// Drawing functions
	
	//! Draw a non-indexed mesh buffer
	//! \param[in] mb The mesh buffer to draw
	//! in world space
	virtual void DrawMeshData(MeshData* mb) = 0;

	//! Draw an indexed mesh buffer
	//! \param[in] mb The mesh buffer to draw
	//! in world space
	virtual void DrawIndexedMeshData(IndexedMeshData* mb) = 0;

	//! Draw a 2d texture
	//! \param[in] pos The position, left-up corner centered
	//! \param[in] tex The texture to be drawn
	//! \param[in] rot The rotation
	virtual void Draw2dTexture(const Position2d& pos,
		Texture* tex, const Rotation2d& rot) = 0;

	//! Virtual deconstructor
	virtual ~GraphicsDevice() {}
};

MAKO_END_NAMESPACE