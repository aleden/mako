#include "MakoCgMtl.h"
#include "MakoException.h"
#include "MakoString.h"
#include "MakoD3D9Device.h"
#include "MakoCgShader.h"

#ifdef MAKO_D3D_AVAILABLE
#include <cg\cgD3D9.h>
#endif

MAKO_BEGIN_NAMESPACE

CgMtl::CgMtl(CgShader* cgprog) : cgProgram(cgprog)
{ cgProgram->Hold(); }

CgMtl::~CgMtl()
{ cgProgram->Drop(); }


void CgMtl::Bind(GraphicsDevice* gd) const
{ cgProgram->Bind(); }

void CgMtl::UnBind(GraphicsDevice* gd) const
{ cgProgram->UnBind(); }

MAKO_END_NAMESPACE

/*

	cgContext = cgCreateContext();
	CheckForCgError(Text("creating Cg context"));

	cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);
	CheckForCgError(Text("using cgSetParameterSettingMode()"));

	const char **profileOpts;

	/////////////////////////////////////////////////////////////////
	// Vertex program

	// Determine the best profiles once a device to be set.
	if (gd->GetType() == GDT_D3D9)
	{
#ifdef MAKO_D3D_AVAILABLE
		cgD3D9SetDevice(static_cast<D3D9Device*>(gd)->GetIDirect3DDevice9());
		cgVertexProfile = cgD3D9GetLatestVertexProfile();
#endif
		CheckForCgError(Text("getting latest vertex profile"));

#ifdef MAKO_D3D_AVAILABLE
		profileOpts = cgD3D9GetOptimalOptions(cgVertexProfile);
#endif
		CheckForCgError(Text("getting latest vertex profile options"));
	}

	cgVertexProgram = cgCreateProgram
		(
			cgContext,                       // Cg runtime context
			CG_SOURCE,                       // Program in human-readable form
			source.ToASCII(),          // Source code
			cgVertexProfile,                 // Profile
			vertexProgName.ToASCII(),  // Entry function name
			profileOpts                      // Compiler options
		);
	CheckForCgError(Text("creating vertex program"));

	if (gd->GetType() == GDT_D3D9)
	{
#ifdef MAKO_D3D_AVAILABLE
		cgD3D9LoadProgram(cgVertexProgram, false, 0);
		CheckForCgError(Text("using cgD3D9LoadProgram"));
#endif
	}

	/////////////////////////////////////////////////////////////////
	// Fragment program

	// Determine the best profile once a device to be set.	
	if (gd->GetType() == GDT_D3D9)
	{
#ifdef MAKO_D3D_AVAILABLE
		cgFragmentProfile = cgD3D9GetLatestPixelProfile();
#endif
		CheckForCgError(Text("getting latest fragment profile"));

#ifdef MAKO_D3D_AVAILABLE
		profileOpts = cgD3D9GetOptimalOptions(cgFragmentProfile);
#endif
		CheckForCgError(Text("getting latest fragment profile options"));
	}

	cgFragmentProgram = cgCreateProgram
		(
			cgContext,                    // Cg runtime context
			CG_SOURCE,                    // Program in human-readable form
			source.ToASCII(),       // Source code
			cgFragmentProfile,            // Profile
			fragProgName.ToASCII(), // Entry function name
			profileOpts                   // Compiler options
		);
	CheckForCgError(Text("creating fragment program from file"));

	if (gd->GetType() == GDT_D3D9)
	{
#ifdef MAKO_D3D_AVAILABLE
		cgD3D9LoadProgram(cgFragmentProgram, false, 0);
		CheckForCgError(Text("using cgD3D9LoadProgram"));
#endif
	}
*/