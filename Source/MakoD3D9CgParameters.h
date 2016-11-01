#pragma once
#include "MakoGraphicsDevice.h"
#include "Cg/cg.h"
#include "MakoException.h"
#include "MakoD3D9Texture.h"

MAKO_BEGIN_NAMESPACE

class D3D9Float4CgParam : public Float4CgParam
{
private:
	CGparameter cgp;
public:
	MAKO_INLINE D3D9Float4CgParam(D3D9CgShader* cgprog, const ASCIIString& name, CG_PROGRAM_COMPONENT func)
	{
		cgp = cgGetNamedParameter(func == CPC_FRAGMENT_FUNC ? cgprog->GetFragmentCGProgram() : cgprog->GetVertexCGProgram(),
			name.ToASCII());
		if (!cgp)
			throw Exception(Text("Cg parameter by the name ") + ToString(name.GetData()) + Text(" was not found."));
	}

	MAKO_INLINE ~D3D9Float4CgParam() {}

	MAKO_INLINE void SetValue(Float32 arg1, Float32 arg2, Float32 arg3, Float32 arg4)
	{ cgSetParameter4f(cgp, arg1, arg2, arg3, arg4); }
};

class D3D9Float4x4CgParam : public Float4x4CgParam
{
private:
	CGparameter cgp;
	D3D9CgShader* cgprog;
public:
	MAKO_INLINE D3D9Float4x4CgParam(D3D9CgShader* cgprog, const ASCIIString& name, CG_PROGRAM_COMPONENT func) : cgprog(cgprog)
	{
		cgp = cgGetNamedParameter(func == CPC_FRAGMENT_FUNC ? cgprog->GetFragmentCGProgram() : cgprog->GetVertexCGProgram(),
			name.ToASCII());
		if (!cgp)
			throw Exception(Text("Cg parameter by the name ") + ToString(name.GetData()) + Text(" was not found."));
	}

	MAKO_INLINE ~D3D9Float4x4CgParam() {}
	
	MAKO_INLINE void SetValue(const Matrix4f& data)
	{ cgD3D9SetUniform(cgp, data.Pointer()); }
};
class D3D9Sampler2DCgParam : public Sampler2dCgParam
{
private:
	CGparameter cgp;
	D3D9CgShader* cgprog;
public:
	MAKO_INLINE D3D9Sampler2DCgParam(D3D9CgShader* cgprog, const ASCIIString& name, CG_PROGRAM_COMPONENT func) : cgprog(cgprog)
	{
		cgp = cgGetNamedParameter(func == CPC_FRAGMENT_FUNC ? cgprog->GetFragmentCGProgram() : cgprog->GetVertexCGProgram(),
			name.ToASCII());
		if (!cgp)
			throw Exception(Text("Cg parameter by the name ") + ToString(name.GetData()) + Text(" was not found."));
	}

	MAKO_INLINE ~D3D9Sampler2DCgParam() {}
	
	MAKO_INLINE void SetValue(Texture* tex)
	{
		cgD3D9SetTexture(cgp, static_cast<D3D9Texture*>(tex->GetTextureHardwareBuffer())->GetIDirect3DTexture9());  
	}
};

MAKO_END_NAMESPACE