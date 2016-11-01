#pragma once

#include "MakoCommon.h"
#include "MakoReferenceCounted.h"

MAKO_BEGIN_NAMESPACE

enum MATERAL_OPTON
{
	MO_ZBUFFER,
	MO_ENUM_LENGTH
};

enum MATERAL_TYPE
{
	MTLT_WIREFRAME,
	MTLT_CG,
	MTLT_COLOR,
	MTLT_DIFF_TEX,
	MTLT_LIGHTMAPPED_DIFF_TEX,
	MTLT_ENUM_LENGTH
};

class Texture;
class GraphicsDevice;

//! This class defines how something is drawn. For instance,
//! there is a material that draws things with a texture. Inside
//! each implementation of Material contains contains 
class Material : public ReferenceCounted
{
public:
	virtual void Bind(GraphicsDevice* gd) const = 0;
	virtual void UnBind(GraphicsDevice* gd) const {}
	virtual Int32 GetType() const = 0;
};

MAKO_END_NAMESPACE