#pragma once
#include "MakoCgMtl.h"
#include "MakoColor.h"
#include "MakoCgParameters.h"

MAKO_BEGIN_NAMESPACE

class ColorMtl : public CgMtl
{
private:
	Float4x4CgParam* modelViewProjCgParam;
	Float4CgParam* colorCgParam;
	Color c;
public:
	MAKO_API ColorMtl(GraphicsDevice* gd,
		const Color& c = Color(0, 0, 255, 255));
	MAKO_API ~ColorMtl();

	MAKO_INLINE const Color& GetColor() const
	{ return c; }

	MAKO_INLINE void SetColor(const Color& c)
	{ this->c = c; }
	
	MAKO_API Int32 GetType() const;
	
	MAKO_API void Bind(GraphicsDevice* gd) const;
};

MAKO_END_NAMESPACE