#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9CgDevice.h"
#include "MakoD3D9Device.h"
#include "MakoD3D9CgShader.h"

MAKO_BEGIN_NAMESPACE

D3D9CgDevice::D3D9CgDevice(GraphicsDevice* gd)
: GenericCgDevice(gd), d3d9dev(static_cast<D3D9Device*>(gd))
{
	cgD3D9SetDevice(d3d9dev->GetIDirect3DDevice9());

	// Determine the best profiles.
	cgLatestVertProfile = cgD3D9GetLatestVertexProfile();
	cgLatestFragProfile = cgD3D9GetLatestPixelProfile();
}

D3D9CgDevice::~D3D9CgDevice()
{
	DropMakoShaders();
	// YOU MUST!!! DO THIS!!! BEFORE!!! EXITING!!!
	// This bug took 1 1/2 days to fix. FUCK the Cg documentation

	// cgD3D9SetDevice informs the expanded interface of the new D3D device. 
	// This will destroy any D3D resources for programs previously loaded with 
	// cgD3D9LoadProgram and use the new D3D device to recreate them. The 
	// expanded interface will increment the reference count to the D3D device, 
	// so this function must eventually be called with NULL to release that 
	// reference so D3D can be properly shut down. 

	// If device is NULL, all D3D resources for programs previously loaded with 
	// cgD3D9LoadProgram are destroyed. However, these programs are still considered 
	// managed by the expanded interface, so if a new D3D device is set later 
	// these programs will be recreated using the new D3D device. 

	// If a new device is being set, all D3D resources for programs previously 
	// loaded with cgD3D9LoadProgram are rebuilt using the new device. All shadowed 
	// parameters for these programs are maintained across D3D device changes except 
	// texture parameters. Since textures in D3D are bound to a particular D3D device, 
	// these resources cannot be saved across device changes. When these textures are 
	// recreated for the new D3D device, they must be re-bound to the sampler parameter. 

	// Note that calling cgD3D9SetDevice(NULL) does not destroy any core runtime resources 
	// (CGprograms, CGparameters, etc.) used by the expanded interface. These must be 
	// destroyed seperately using cgDestroyProgram and cgDestroyContext. 

	cgD3D9SetDevice(nullptr);
}

CgShader* D3D9CgDevice::CreateShader(const ASCIIString& sourceCode,
									 const ASCIIString& vertexProgName,
									 const ASCIIString& fragProgName)
{ return new D3D9CgShader(this, sourceCode, vertexProgName, fragProgName); }

const char** D3D9CgDevice::GetOptimalFragProfOpts()
{
	// Get the best profile options
	const char** profileOpts;

	profileOpts = cgD3D9GetOptimalOptions(cgLatestFragProfile);

	return profileOpts;
}

const char** D3D9CgDevice::GetOptimalVertProfOpts()
{
	// Get the best profile options
	const char** profileOpts;

	profileOpts = cgD3D9GetOptimalOptions(cgLatestVertProfile);

	return profileOpts;
}


MAKO_END_NAMESPACE

#endif
