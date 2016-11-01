#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoD3D9CgShader.h"
#include "MakoD3D9CgDevice.h"
#include "MakoD3D9Device.h"
#include "MakoD3D9CgParameters.h"
#include "MakoException.h"

MAKO_BEGIN_NAMESPACE

D3D9CgShader::D3D9CgShader(CgDevice* cgdevice,
							 const ASCIIString& sourceCode,
							 const ASCIIString& vertexProgName,
							 const ASCIIString& fragProgName)
							 : GenericCgShader(cgdevice, static_cast<D3D9CgDevice*>(cgdevice)->GetCGContext()),
							 d3d9dev(static_cast<D3D9Device*>(GetCgDevice()->GetGraphicsDevice())->GetIDirect3DDevice9())
{
	cgVertexProgram = cgCreateProgram
		(
			GetCGContext(),                                                // Cg runtime context
			CG_SOURCE,                                                     // Program in human-readable form
			sourceCode.ToASCII(),                                    // Source code
			static_cast<D3D9CgDevice*>(cgdevice)->GetLatestVertProfile(),  // Profile
			vertexProgName.ToASCII(),                                // Entry function name
			static_cast<D3D9CgDevice*>(cgdevice)->GetOptimalVertProfOpts() // Compiler options
		);
	CheckForCgCompileError(CPC_VERTEX_FUNCT, GetCGContext());

	cgD3D9LoadProgram(cgVertexProgram, false, 0);

	cgFragmentProgram = cgCreateProgram
		(
			GetCGContext(),                                                // Cg runtime context
			CG_SOURCE,                                                     // Program in human-readable form
			sourceCode.ToASCII(),                                    // Source code
			static_cast<D3D9CgDevice*>(cgdevice)->GetLatestFragProfile(),  // Profile
			fragProgName.ToASCII(),                                  // Entry function name
			static_cast<D3D9CgDevice*>(cgdevice)->GetOptimalFragProfOpts() // Compiler options
		);
	CheckForCgCompileError(CPC_FRAGMENT_FUNC, GetCGContext());

	cgD3D9LoadProgram(cgFragmentProgram, false, 0);
}

void D3D9CgShader::Bind()
{
	cgD3D9BindProgram(cgVertexProgram);
	cgD3D9BindProgram(cgFragmentProgram);
}

void D3D9CgShader::UnBind()
{
	d3d9dev->SetVertexShader(nullptr);
	d3d9dev->SetPixelShader(nullptr);
}

CgParameter* D3D9CgShader::GetParameterByName(const ASCIIString& name, CG_PARAMETER_TYPE type,
											   CG_PROGRAM_COMPONENT func)
{
	switch (type)
	{
	case CPT_FLOAT4:
		return new D3D9Float4CgParam(this, name, func);
	case CPT_FLOAT4X4:
		return new D3D9Float4x4CgParam(this, name, func);
	case CPT_SAMPLER2D:
		return new D3D9Sampler2DCgParam(this, name, func);
	}

	throw Exception(Text("Invalid CG_PARAMETER_TYPE was supplied to CgShader::GetParameterByName()"));
}

MAKO_END_NAMESPACE

#endif