#pragma once

#include "MakoMaterial.h"

MAKO_BEGIN_NAMESPACE

class WireframeMtl : public Material
{
public:
	MAKO_INLINE WireframeMtl() {}
	MAKO_INLINE ~WireframeMtl() {}
	
	MAKO_INLINE Int32 GetType() const
	{ return MTLT_WIREFRAME; }

	MAKO_API void UnBind(GraphicsDevice* gd) const;
	
	MAKO_API void Bind(GraphicsDevice* gd) const;
};

MAKO_END_NAMESPACE