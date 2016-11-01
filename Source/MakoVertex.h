#pragma once

#include "MakoCommon.h"
#include "MakoVec2d.h"
#include "MakoVec3d.h"
#include "MakoReferenceCounted.h"
#include "MakoColor.h"

MAKO_BEGIN_NAMESPACE

// Each enum's value in VERTEX_TYPE is equal
// to it's corrosponding vertex class' size in bytes.
enum VERTEX_TYPE
{
	// Float32's: position(3), tcoord0(2) = 5
	VT_STANDARD = sizeof(Float32) * 5,
	// Float32's: position(3), tcoords(4) = 7
	VT_T2 = sizeof(Float32) * 7,
	VT_ENUM_LENGTH = 2
};

class Vertex
{
public:
#ifdef MAKO_VERTEX_NORMALS
	virtual const Vec3df& GetNormal() const = 0;
#endif
	//virtual const Color&  GetColor() const = 0;

	virtual const Position3d& GetPosition() const = 0;
	virtual const Vec2df& GetTCoord0(UInt8 index = 1) const = 0;
	virtual VERTEX_TYPE GetType() const = 0;


	virtual void SetPosition(const Position3d& p) = 0;
	virtual void SetTCoord0(const Vec2df& tc, UInt8 index = 1) = 0;
};

MAKO_END_NAMESPACE
