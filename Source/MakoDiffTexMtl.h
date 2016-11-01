#pragma once
#include "MakoCgMtl.h"
#include "MakoCgParameters.h"

MAKO_BEGIN_NAMESPACE

//! This material uses the 0th tcoord channel
class DiffTexMtl : public CgMtl
{
private:
	Float4x4CgParam* modelViewProjCgParam;
	Sampler2dCgParam* difftexCgParam;
	UInt8 texCoordChannel;
	Texture* tex;
public:
	MAKO_API DiffTexMtl(Texture* tex, UInt8 tCoordChannel = 0);	
	MAKO_API ~DiffTexMtl();
	
	MAKO_INLINE Int32 GetType() const
	{ return MTLT_DIFF_TEX; }

	MAKO_INLINE void SetTexCoordChannel(UInt8 tCoordChannel)
	{ texCoordChannel = tCoordChannel; }

	MAKO_INLINE UInt8 GetTexCoordChannel() const
	{ return texCoordChannel; }

	MAKO_INLINE Texture* GetDiffuseTexture() const
	{ return tex; }

	void Bind(GraphicsDevice* gd) const;
};

MAKO_END_NAMESPACE