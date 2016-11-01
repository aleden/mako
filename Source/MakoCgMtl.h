#pragma once
#include "MakoMaterial.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class CgShader;

// THE CG CONTEXT MUST BE DESTROYED BEFORE ATTEMPTING TO RELEASE ANY D3D
// OBJECTS
class CgMtl : public Material
{
private:
	CgShader* cgProgram;
public:
	MAKO_API CgMtl(CgShader* cgprog);
	MAKO_API ~CgMtl();

	MAKO_INLINE CgShader* GetCgShader() const
	{ return cgProgram; }

	MAKO_API void Bind(GraphicsDevice* gd) const;
	MAKO_API void UnBind(GraphicsDevice* gd) const;

	Int32 GetType() const
	{ return MTLT_CG; }
};

MAKO_END_NAMESPACE