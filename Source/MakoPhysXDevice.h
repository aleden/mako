#pragma once
#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoCommon.h"
#include "MakoPhysics3dDevice.h"
#include "MakoReferenceCounted.h"
#include "MakoArrayList.h"
#include "MakoOS.h"
#include <NxPhysics.h>
#include <NxCooking.h>
#include <NxControllerManager.h>
#include <NxCapsuleController.h>
#include <NxUserAllocatorDefault.h> 
//#include "stream.h"

MAKO_BEGIN_NAMESPACE

class Mesh;

class PhysXOutputStream : public NxUserOutputStream
{
public:
	MAKO_REALINLINE PhysXOutputStream() {}
	MAKO_REALINLINE ~PhysXOutputStream() {}
	
	void reportError(NxErrorCode code,
					 const char *message,
					 const char* file,
					 int line);
	NxAssertResponse reportAssertViolation(const char *message,
										   const char *file,
										   int line);
	void print(const char* message);
};

void SetNxActorDescPosAndRot(NxActorDesc& desc,
							 const Pos3d& pos,
							 const Rot3d& rot);

// Forward declaration
class PhysXSceneManager;

class PhysXDevice : public Physics3dDevice
{
private:
	ArrayList<PhysXSceneManager*> scenes;

	bool physXHardwarePresent;
	NxPhysicsSDK* physicsSDK;
	NxControllerManager* controllerManager;

	NxUserAllocatorDefault userAllocDefault;
	PhysXOutputStream outputStream;
	NxCookingInterface* cooker;
public:
	NxPhysicsSDK* GetPhysXSDK() const;
	bool IsPhysXHardwarePresent() const;

	PhysXDevice();
	~PhysXDevice();

	void Update(Float32 timeStep);
	String GetName() const;

	Physics3dScene* AddScene();
	UInt32 GetNumScenes() const;
	Physics3dScene* GetScene(UInt32 index = 0);
	const Physics3dScene* GetScene(UInt32 index) const;
	
	NxUserAllocator* GetPhysXAllocator();
	NxUserOutputStream* GetPhysXOutputStream();
	NxCookingInterface* GetPhysXCookerInterface();

	PHYSICS_3D_DEVICE_TYPE GetType() const
	{ return P3DT_PHYSX; }
};

class PhysXActorContactReport : public NxUserContactReport
{
public:
	MAKO_INLINE PhysXActorContactReport() {}
	MAKO_INLINE ~PhysXActorContactReport() {}
	
	void onContactNotify(NxContactPair& pair, NxU32 events);
};

class PhysXSceneManager : public Physics3dScene
{
private:
	PhysXDevice* device;
	PhysXActorContactReport contactReport;
	Vec3df gravity;
	NxScene* scene;
public:
	PhysXSceneManager(PhysXDevice* device);
	~PhysXSceneManager();

	Physics3dDevice* GetPhysics3dDevice() const;
	
	Physics3dActor*  AddStaticBoxActor(const Size3d& dim,
		                                        const Position3d& pos,
												const Rotation3d& rot);
	Physics3dActor*  AddDynamicBoxActor(const Size3d& dim,
		                                         const Position3d& pos,
												 const Rotation3d& rot);
	Physics3dActor*  AddDynamicSphereActor(const Float32 radius,
		                                            const Position3d& pos,
													const Rotation3d& rot);
	Physics3dActor* AddStaticSphereActor(const Float32 radius,
		                                         const Position3d& pos,
		                                         const Rotation3d& rot);
	Physics3dActor*  AddStaticPlaneActor(Float32 size,
		                                          const Position3d& pos,
												  const Rotation3d& rot);
	Physics3dActor*  AddStaticTriangleMeshActor(const Mesh* mesh,
		                                                 const Position3d& pos,
														 const Rotation3d& rot,
														 const Scale3d& scale);
	NxScene* GetPhysXScene() const;

	void SetGravity(const Vec3df& g);
	const Vec3df& GetGravity() const;

	UInt32 GetNumActors() const;

	Physics3dActor* GetActor(UInt32 index);
	const Physics3dActor* GetActor(UInt32 index) const;
};

class PhysXActor : public Physics3dActor
{
	friend class PhysXActorContactReport;
protected:
	NxActor* actor;
	ArrayList<bool> colEventFlags;
public:
	PhysXActor(PhysXSceneManager* scene,
		       const Position3d& pos,
			   const Rotation3d& rot);
	virtual ~PhysXActor();
	
	virtual void SetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct, bool enabled = true);
	virtual bool GetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct);
	virtual void Update();
	virtual void AddForce(const Vec3df& vel);
};

MAKO_END_NAMESPACE

#endif // MAKO_PHYSX_AVAILABLE