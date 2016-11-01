#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoReferenceCounted.h"
#include "MakoMatrix4.h"

MAKO_BEGIN_NAMESPACE

const ASCIIString defaultCgVertProgName = "MainV";
const ASCIIString defaultCgFragProgName = "MainF";

// Forward declaration
class CgDevice;
class Texture;

enum CG_PARAMETER_TYPE
{ CPT_FLOAT4, CPT_FLOAT4X4, CPT_SAMPLER2D, CPT_ENUM_LENGTH };

//! This represents a uniform parameter in a Cg program
class CgParameter : public ReferenceCounted
{
public:
	virtual CG_PARAMETER_TYPE GetType() const = 0;
};

class Float4CgParam : public CgParameter
{
public:
	virtual void SetValue(Float32 arg1, Float32 arg2, Float32 arg3, Float32 arg4) = 0;
	MAKO_INLINE CG_PARAMETER_TYPE GetType() const
	{ return CPT_FLOAT4; }
};

class Float4x4CgParam : public CgParameter
{
public:
	virtual void SetValue(const Matrix4f& data) = 0;
	MAKO_INLINE CG_PARAMETER_TYPE GetType() const
	{ return CPT_FLOAT4X4; }
};

class Sampler2dCgParam : public CgParameter
{
public:
	virtual void SetValue(Texture* tex) = 0;
	MAKO_INLINE CG_PARAMETER_TYPE GetType() const
	{ return CPT_SAMPLER2D; }
};

MAKO_END_NAMESPACE