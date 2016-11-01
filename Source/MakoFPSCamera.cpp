#include "MakoFPSCamera.h"
#include "Makomath.h"
#include "MakoApplication.h"
#include "MakoOSDevice.h"

MAKO_BEGIN_NAMESPACE

FPSCamera::FPSCamera(const Position3d& pos,
					 const Rotation3d& rot,
					 Float32 camSpeed,
					 Float32 camRotateSpeed,
					 Float32 fov,
					 Float32 nearViewPlane,
					 Float32 farViewPlane)
					 : Camera(pos, Pos3d(0), fov, nearViewPlane, farViewPlane),
					   camSpeed(camSpeed), camRotateSpeed(camRotateSpeed), keyDownW(false),
					   keyDownS(false), keyDownA(false), keyDownD(false)
{
	SetRotation(rot);

	child = new FPSCameraKeyReceiver(this);
	APP()->AddEventReceiver(child);
}

FPSCamera::FPSCamera(const FPSCameraCreationParams& p)
: Camera(p.pos, Pos3d(0), p.fov, p.nearViewPlane, p.farViewPlane),
  camSpeed(p.camSpeed), camRotateSpeed(p.camRotateSpeed), keyDownW(false),
  keyDownS(false), keyDownA(false), keyDownD(false)
{
	SetRotation(p.rot);

	child = new FPSCameraKeyReceiver(this);
	APP()->AddEventReceiver(child);
}

FPSCamera::~FPSCamera()
{ APP()->RemoveEventReceiver(child); }

void FPSCamera::Update()
{
	if (keyDownW)
		SetPosition
		(
			Forward
			(
				GetPosition(),
				GetRotation(),
				camSpeed * APP()->GetOSDevice()->GetChangeInTime()
			)
		);
	if (keyDownS)
		SetPosition
		(
			Forward
			(
				GetPosition(),
				GetRotation(),
				-camSpeed * APP()->GetOSDevice()->GetChangeInTime()
			)
		);
	if (keyDownA)
		SetPosition
		(
			Sideways
			(
				GetPosition(),
				GetRotation(),
				-camSpeed * APP()->GetOSDevice()->GetChangeInTime()
			)
		);
	if (keyDownD)
		SetPosition
		(
			Sideways
			(
				GetPosition(),
				GetRotation(),
				camSpeed * APP()->GetOSDevice()->GetChangeInTime()
			)
		);

	Vec3df rotVecX
		(
			0.f,
			static_cast<Float32>(APP()->GetOSDevice()->GetChangeInMouse().x),
			0.f
		);
	Vec3df camRot = GetRotation();
	if ((camRot + rotVecX * camRotateSpeed).x > -90 && (camRot + rotVecX * camRotateSpeed).x < 90)
		SetRotation(camRot + rotVecX * camRotateSpeed);
	
	Vec3df rotVecY
		(
			static_cast<Float32>(APP()->GetOSDevice()->GetChangeInMouse().y),
			0.f,
			0.f
		);
	camRot = GetRotation();
	if ((camRot + rotVecY * camRotateSpeed).x > -90 && (camRot + rotVecY * camRotateSpeed).x < 90)
		SetRotation(camRot + rotVecY * camRotateSpeed);

	SetTarget
		(
			Forward
			(
				GetPosition(),
				GetRotation(),
				10.f
			)
		);
}

FPSCamera::FPSCameraKeyReceiver::FPSCameraKeyReceiver(FPSCamera* parent)
{ this->parent = parent; }

FPSCamera::FPSCameraKeyReceiver::~FPSCameraKeyReceiver() {}

void FPSCamera::FPSCameraKeyReceiver::OnEvent(const KeyEvent* ke)
{
	switch (ke->GetKey())
	{
	case KEY_W:
		parent->keyDownW = ke->IsDown();
		break;
	case KEY_S:
		parent->keyDownS = ke->IsDown();
		break;
	case KEY_A:
		parent->keyDownA = ke->IsDown();
		break;
	case KEY_D:
		parent->keyDownD = ke->IsDown();
		break;
	}
}


MAKO_END_NAMESPACE