#pragma once
#include "MakoCgMtl.h"
#include "MakoCgParameters.h"

MAKO_BEGIN_NAMESPACE

//! This uses the 0th tcoord channel for the diffuse map,
//! and the 1st tcoord channel for the lightmap (modulated
//! on top of).
class LightmappedDiffTexMtl : public CgMtl
{
private:
	Float4x4CgParam* modelViewProjCgParam;
	Sampler2dCgParam* difftexCgParam;
	Sampler2dCgParam* lmtexCgParam;
	Texture* difftex;
	Texture* lmtex;
public:
	MAKO_API LightmappedDiffTexMtl(Texture* difftex, Texture* lmtex);
	MAKO_API ~LightmappedDiffTexMtl();

	MAKO_INLINE Int32 GetType() const
	{ return MTLT_LIGHTMAPPED_DIFF_TEX; }
	
	MAKO_API void Bind(GraphicsDevice* gd) const;
};

MAKO_END_NAMESPACE