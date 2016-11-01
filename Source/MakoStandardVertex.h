#pragma once
#include "MakoVertex.h"
#include "MakoVec3d.h"
#include "MakoVec2d.h"
#include "MakoColor.h"
#include "MakoException.h"

MAKO_BEGIN_NAMESPACE

// D3D9 REQUIRES ORDER TO BE FOR VERTICES IN MEMORY:
// 1. POSITION    (Float32 X, Y, Z)
// 2. NORMAL      (Float32 X, Y, Z)
// 2. COLOR       (UInt32 COLOR)
// 3. D3DFVF_TEX1 (Float32 U, V)
// 4. D3DFVF_TEX2 (Float32 U, V)

//! A StandardVertex only has a 0 tcoord channel, and a position.
class StandardVertex // : public Vertex
{
public:
	Position3d pos;
	TCoord tcoord0;

	MAKO_INLINE StandardVertex(const Position3d& pos = Pos3d(0.f),
		                       const TCoord& tcoord = TCoord(0.f))
		: pos(pos), tcoord0(tcoord) {}

	MAKO_INLINE StandardVertex(const StandardVertex& svert)
		: pos(svert.pos), tcoord0(svert.tcoord0) {}

	MAKO_INLINE ~StandardVertex() {}

	MAKO_INLINE bool Equals(const StandardVertex& svert)
	{ return pos == svert.pos && tcoord0 == svert.tcoord0; }

	MAKO_INLINE const Position3d& GetPosition() const
	{ return pos; }

	MAKO_INLINE const TCoord& GetTCoord0() const
	{ return tcoord0; }

	MAKO_INLINE void SetPosition(const Position3d& p)
	{ pos = p; }

	MAKO_INLINE void SetTCoord0(const TCoord& tc)
	{ tcoord0 = tc; }
};

MAKO_END_NAMESPACE