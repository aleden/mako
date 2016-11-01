#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include <cg\cgD3D9.h>
#include "MakoGenericCgDevice.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class D3D9Device;
class GraphicsDevice;

class D3D9CgDevice : public GenericCgDevice
{
private:
	D3D9Device* d3d9dev;
	CGprofile cgLatestVertProfile, cgLatestFragProfile;
	//char** vertProfileOptions;
	//char** fragProfileOptions;

	//void CopyOverProfOpts(const char **profOptsSrc, char **profOptsDest);
public:
	D3D9CgDevice(GraphicsDevice* gd);
	~D3D9CgDevice();

	CgShader* CreateShader(const ASCIIString& sourceCode,
		const ASCIIString& vertexProgName,
		const ASCIIString& fragProgName);

	MAKO_INLINE CGprofile GetLatestVertProfile()
	{ return cgLatestVertProfile; }

	MAKO_INLINE CGprofile GetLatestFragProfile()
	{ return cgLatestFragProfile; }

	const char** GetOptimalVertProfOpts();
	const char** GetOptimalFragProfOpts();
};

MAKO_END_NAMESPACE
#endif