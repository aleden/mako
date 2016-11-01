#pragma once
#include "MakoCamera.h"
#include "Makoevents.h"
#include "MakoVec3d.h"

MAKO_BEGIN_NAMESPACE

struct FPSCameraCreationParams : public Scene3dNodeCreationParams
{
	MAKO_INLINE FPSCameraCreationParams() : camSpeed(0.01f),
		                                    camRotateSpeed(.25f),
											fov(DEGTORAD * 65.f),
											nearViewPlane(.1f),
											farViewPlane(100.f) {}
	Float32 camSpeed;
	Float32 camRotateSpeed;
	//! In radians. To convert degrees to radians, multiply the degrees by DEGTORAD.
	Float32 fov;
	Float32 nearViewPlane;
	Float32 farViewPlane;
};

class FPSCamera : public Camera
{
	bool keyDownW, keyDownS, keyDownA, keyDownD;
	Float32 fov, nearViewPlane, farViewPlane;
	Vec3df target;
	Float32 camSpeed, camRotateSpeed;

	class FPSCameraKeyReceiver : public KeyEventReceiver
	{
	private:
		FPSCamera* parent;
	public:
		FPSCameraKeyReceiver(FPSCamera* parent);
		~FPSCameraKeyReceiver();

		void OnEvent(const KeyEvent* event);
	};

	FPSCameraKeyReceiver* child;
public:
	MAKO_API FPSCamera(const Position3d& pos  = Pos3d(),
		               const Rotation3d& rot  = Rot3d(),
					   Float32 camSpeed       = .01f,
					   Float32 camRotateSpeed = .25f,
					   Float32 fov            = DEGTORAD * 90.f,
					   Float32 nearViewPlane  = .1f,
					   Float32 farViewPlane   = 100.f);

	MAKO_API FPSCamera(const FPSCameraCreationParams& params);

	MAKO_API virtual ~FPSCamera();
	MAKO_API virtual void Update();

	MAKO_INLINE Float32 GetMoveSpeed()
	{ return camSpeed; }

	MAKO_INLINE void SetMoveSpeed(Float32 speed)
	{ camSpeed = speed; }

	MAKO_INLINE Float32 GetRotateSpeed()
	{ return camRotateSpeed; }

	MAKO_INLINE void SetRotateSpeed(Float32 speed)
	{ camRotateSpeed = speed; }
};

MAKO_END_NAMESPACE