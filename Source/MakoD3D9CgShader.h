#pragma once
#include "MakoCommon.h"
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
#include "MakoGenericCgShader.h"
#include <cg\cgD3D9.h>

MAKO_BEGIN_NAMESPACE

class D3D9CgShader : public GenericCgShader
{
private:
	LPDIRECT3DDEVICE9 d3d9dev;
public:
	D3D9CgShader(CgDevice* cgdevice,
		const ASCIIString& sourceCode,
		const ASCIIString& vertexProgName,
		const ASCIIString& fragProgName);
	MAKO_INLINE ~D3D9CgShader() {}

	CgParameter* GetParameterByName(const ASCIIString& name,
		CG_PARAMETER_TYPE type, CG_PROGRAM_COMPONENT func);

	void Bind();
	void UnBind();
};

MAKO_END_NAMESPACE
#endif