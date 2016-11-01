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

//! A T2Vertex only has a 0th and 1st tcoord channel, and a position.
class T2Vertex // : public Vertex
{
public:
	Position3d pos;
	TCoord tcoord0;
	TCoord tcoord1;

	MAKO_INLINE T2Vertex(const Position3d& pos = Pos3d(0.f),
		const TCoord& tcoord0 = TCoord(0.f), const TCoord& tcoord1 = TCoord(0.f))
		: pos(pos), tcoord0(tcoord0), tcoord1(tcoord1) {}

	MAKO_INLINE T2Vertex(const T2Vertex& t2vert)
		: pos(t2vert.pos), tcoord0(t2vert.tcoord0), tcoord1(t2vert.tcoord1) {}

	MAKO_INLINE ~T2Vertex() {}

	MAKO_INLINE bool operator == (const T2Vertex& t2vert)
	{  return pos == t2vert.pos && tcoord0 == t2vert.tcoord0 && tcoord1 == t2vert.tcoord1; }

	MAKO_INLINE const Position3d& GetPosition() const
	{ return pos; }

	MAKO_INLINE void SetPosition(const Position3d& p)
	{ pos = p; }

	MAKO_INLINE const TCoord& GetTCoord0() const
	{ return tcoord0; }

	MAKO_INLINE const TCoord& GetTCoord1() const
	{ return tcoord1; }

	MAKO_INLINE void SetTCoord0(const TCoord& tc)
	{ tcoord0 = tc; }

	MAKO_INLINE void SetTCoord1(const TCoord& tc)
	{ tcoord1 = tc; }
};

MAKO_END_NAMESPACE