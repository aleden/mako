#pragma once

#include "MakoScene3dNode.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

class Camera : public Scene3dNode
{
private:
	Position3d target;
	Float32 fov, nearViewPlane, farViewPlane;
public:
	MAKO_INLINE Camera
		(
			const Position3d& pos    = Pos3d(),
			const Position3d& target = Pos3d(),
			Float32 fov              = DEGTORAD * 45.f,
			Float32 nearViewPlane    = .1f,
			Float32 farViewPlane     = 100.f
		)
		: Scene3dNode(pos, Rot3d(), Scale3d(1.f)), target(target), fov(fov),
		  nearViewPlane(nearViewPlane), farViewPlane(farViewPlane) {}
	
	MAKO_INLINE virtual ~Camera() {}
	
	MAKO_INLINE Float32 GetFOV() const
	{ return fov; }

	MAKO_INLINE void SetFOV(Float32 fov)
	{ this->fov = fov; }
	
	MAKO_INLINE Float32 GetNearViewPlane() const
	{ return nearViewPlane; }

	MAKO_INLINE void SetNearViewPlane(Float32 f)
	{ nearViewPlane = f; }
	
	MAKO_INLINE Float32 GetFarViewPlane() const
	{ return farViewPlane; }

	MAKO_INLINE void SetFarViewPlane(Float32 f)
	{ farViewPlane = f; }

	MAKO_INLINE const Position3d& GetTarget() const
	{ return target; }

	MAKO_INLINE void SetTarget(const Position3d& target)
	{ this->target = target; }
};

MAKO_END_NAMESPACE