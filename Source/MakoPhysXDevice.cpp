#include "MakoCommon.h"
#ifdef MAKO_PHYSX_AVAILABLE
#include "MakoPhysXDevice.h"
#include "MakoException.h"
#include "MakoApplication.h"
#include "MakoConsole.h"
#include "MakoString.h"
#include "MakoString.h"
#include "MakoMatrix4.h"
#include "MakoPhysXStaticBoxActor.h"
#include "MakoPhysXDynamicBoxActor.h"
#include "MakoPhysXStaticPlaneActor.h"
#include "MakoPhysXStaticTriangleMeshActor.h"
#include "MakoPhysXDynamicSphereActor.h"
#include "MakoPhysXStaticSphereActor.h"
#include "MakoEvents.h"

#define VEC3DF_TO_NXVEC3(VEC3DF) NxVec3(VEC3DF.x, VEC3DF.y, VEC3DF.z)

MAKO_BEGIN_NAMESPACE

typedef ArrayList<PhysXSceneManager*>::iterator scenesit;
////////////////////////////////////////////////////////////////////////////////////////////////////
// PhysXOutputStream

void PhysXOutputStream::reportError(NxErrorCode code,
									const char* message,
									const char* file,
									int line)
{
	//this should be routed to the application
	//specific error handling. If this gets hit
	//then you are in most cases using the SDK
	//wrong and you need to debug your code!
	//however, code may just be a warning or
	//information.
	APP()->GetConsole()->Log(LL_LOW, 
		ToString(message) + Text(" in ") + ToString(file) + Text(" line ")  + String::From32BitInt(line));
}

NxAssertResponse PhysXOutputStream::reportAssertViolation(const char* message,
														  const char* file,
														  int line)
{
	//this should not get hit by
	// a properly debugged SDK!
	throw Exception(ToString(message) + Text(" in ") + ToString(file) + Text(" line ")  +
		String::From32BitInt(line));
}

void PhysXOutputStream::print(const char* message)
{ APP()->GetConsole()->PrintLn(ToString(message)); }

void SetNxActorDescPosAndRot(NxActorDesc& actorDesc,
							 const Pos3d& pos,
							 const Rot3d& rot)
{
	actorDesc.globalPose.t = NxVec3(pos.x, pos.y, pos.z);
	//Some math here to convert from rotation in degrees to PhysX matrix
	Matrix4f mat;

	mat.SetRotationDegrees(rot);
	NxMat33 m;
	//m.setColumnMajorStride4(&mat.Pointer()[0]);
	m.setColumnMajorStride4(mat.Pointer());
	actorDesc.globalPose.M = m;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PhysXDevice
PhysXDevice::PhysXDevice()
{
	// Create the SDK
	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, &userAllocDefault, &outputStream);
	if(!physicsSDK)
		throw Exception(Text("NxCreatePhysicsSDK() failed, is the NVIDIA PhysX System Software installed?"));

	physicsSDK->setParameter(NX_SKIN_WIDTH, 0.02f); // usually 0.02
	physicsSDK->setParameter(NX_CONTINUOUS_CD, 1);  // enable CCD

	physXHardwarePresent = (physicsSDK->getHWVersion() != NX_HW_VERSION_NONE);

	scenes.push_back(new PhysXSceneManager(this));

	// Note: The controller manager is supposed to be a singleton. n other words, each 
	// application should take the appropriate steps to ensure only one instance of 
	// NxControllerManager is created Create one controller for each movable character
	controllerManager = NxCreateControllerManager(&userAllocDefault);
	cooker = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
}

PhysXDevice::~PhysXDevice()
{
	if (controllerManager)
		NxReleaseControllerManager(controllerManager);
	
	// We do this if someone added physics scenes but did not hold
	// them once, for whatever reason. Physics3dDevice s do not
	// hold scenes that they add. This is because if they had, there would
	// be no way to delete these scenes until when the program has finished.
	// This can be undesireable, so this code will delete those scenes if they
	// have not been deleted before.
	for (scenesit it = scenes.begin(); it != scenes.end(); ++it)
	{
		(*it)->Hold();
		(*it)->Drop();
	}

	if(physicsSDK)
		physicsSDK->release();
}

void PhysXDevice::Update(Float32 timeStep)
{
	for (scenesit it = scenes.begin(); it != scenes.end(); ++it)
	{
		// The order of function calls here have been a source of hanging in the past

		// Start collision and dynamics for delta time since the last frame
		((*it)->GetPhysXScene())->simulate(timeStep);
		((*it)->GetPhysXScene())->flushStream();
		//while (!scene->fetchResults(NX_RGD_BODY_FNSHED, false));
		((*it)->GetPhysXScene())->fetchResults(NX_RIGID_BODY_FINISHED, true);

		for (UInt i = 0; i < (*it)->GetNumActors(); ++i)
			(*it)->GetActor(i)->Update();
	}
	controllerManager->updateControllers();
}

Physics3dScene* PhysXDevice::AddScene()
{
	PhysXSceneManager* t = new PhysXSceneManager(this);
	scenes.push_back(t);
	return t;
}

UInt32 PhysXDevice::GetNumScenes() const
{ return scenes.size(); }

Physics3dScene* PhysXDevice::GetScene(UInt32 index)
{ return scenes[index]; }

const Physics3dScene* PhysXDevice::GetScene(UInt32 index) const
{ return scenes[index]; }

NxUserAllocator* PhysXDevice::GetPhysXAllocator()
{ return &userAllocDefault; }

NxUserOutputStream* PhysXDevice::GetPhysXOutputStream()
{ return &outputStream; }

NxCookingInterface* PhysXDevice::GetPhysXCookerInterface()
{ return cooker; }

String PhysXDevice::GetName() const
{ return String(Text("PhysX")); }

NxPhysicsSDK* PhysXDevice::GetPhysXSDK() const
{ return physicsSDK; }

bool PhysXDevice::IsPhysXHardwarePresent() const
{ return physXHardwarePresent; }

////////////////////////////////////////////////////////////////////////////////////////////////////
// PhysXContactReport
void PhysXActorContactReport::onContactNotify(NxContactPair &pair, NxU32 events)
{
	if (pair.isDeletedActor[0] || pair.isDeletedActor[1])
		return;

	// NX_NOTIFY_ON_START_TOUCH, 
	// NX_NOTIFY_ON_END_TOUCH, 
	// NX_NOTIFY_ON_TOUCH, 
	// NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD, 
	// NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD, 
	// NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD, 
	// NX_NOTIFY_ON_IMPACT, //unimplemented!
	// NX_NOTIFY_ON_ROLL,   //unimplemented!
	// NX_NOTIFY_ON_SLIDE,  //unimplemented!

	// CPET_START_TOUCH                 = (1<<1),
	// CPET_END_TOUCH                   = (1<<2),
	// CPET_TOUCH                       = (1<<3),
	// CPET_START_TOUCH_FORCE_THRESHOLD = (1<<8),
	// CPET_END_TOUCH_FORCE_THRESHOLD   = (1<<9),
	// CPET_TOUCH_FORCE_THRESHOLD       = (1<<10),
	// 
	// CPET_ALL = CPET_START_TOUCH | CPET_END_TOUCH | CPET_TOUCH | 
	//           CPET_START_TOUCH_FORCE_THRESHOLD | CPET_END_TOUCH_FORCE_THRESHOLD |
	//           CPET_TOUCH_FORCE_THRESHOLD,
	// 
	// CPET_ENUM_LENGTH = 7

	PhysXActor* actor1 = static_cast<PhysXActor*>(pair.actors[0]->userData);
	PhysXActor* actor2 = static_cast<PhysXActor*>(pair.actors[1]->userData);
	
	if (events & NX_NOTIFY_ON_START_TOUCH)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_START_TOUCH));
	
	if (events & NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_START_TOUCH_FORCE_THRESHOLD));
	
	if (events & NX_NOTIFY_ON_TOUCH)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_TOUCH));
	
	if (events & NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_TOUCH_FORCE_THRESHOLD));
	
	if (events & NX_NOTIFY_ON_END_TOUCH)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_END_TOUCH));
	
	if (events & NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD)
		APP()->PostEvent(&Physics3dActorPairEvent(actor1, actor2, CPET_END_TOUCH_FORCE_THRESHOLD));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PhysXSceneManager
PhysXSceneManager::PhysXSceneManager(PhysXDevice* device)
: gravity(0.0f,-9.8f*2,0.0f)
{
	this->device = device;

	// Create the scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity = VEC3DF_TO_NXVEC3(gravity);

	// Run in hardware (when there is a PhysX card installed)
	sceneDesc.simType = device->IsPhysXHardwarePresent() ? NX_SIMULATION_HW : NX_SIMULATION_SW;

	scene = device->GetPhysXSDK()->createScene(sceneDesc);

	if(!scene)
		throw Exception(Text("NxPhysicsSDK::createScene() failed."));

	scene->setUserContactReport(&contactReport);

	//Give default material reasonable values
	NxMaterial* defaultMaterial = scene->getMaterialFromIndex(0);
	//defaultMaterial->setRestitution(0.01f);
	//defaultMaterial->setStaticFriction(0.5f);
	//defaultMaterial->setDynamicFriction(0.35f);

	defaultMaterial->setRestitution(0.01f);
	defaultMaterial->setStaticFriction(1.0f);//static friction can be higher than 1.
	defaultMaterial->setDynamicFriction(.7f); //dynamic friction should not be higher than 1.

	//#ifdef DEBUG
	//	((NxFoundationSDK*)physicsSDK)->getRemoteDebugger()->connect("localhost");
	//#endif
}

PhysXSceneManager::~PhysXSceneManager()
{
	NxActor** arrayActors = scene->getActors();
	UInt32 numActors = scene->getNbActors();
	// We go through the actors, hold and drop the Physics3dActor instances
	// to see if they were never holded. f they weren't, Drop() returns
	// true that they were deleted. n the deconstructor the instance
	// will release it's PhysX actor from the scene. Thus, arrayActors
	// could be invalid, and numActors is definetely invalid because
	// there is one less PhysX actor now. So, we do 3 things:
	// 1. we update arrayActors
	// 2. we update numActors
	// 3. we continue without incrementing i (since the array has been shifted
	// one to the left)
	// The reason why we can't just release the scene (which would release
	// all the PhysX actors inside it) is that it's won't alsso Hold() and 
	// Drop() every Physics3dActor which holds that PhysX actor.

	UInt32 i = 0;
	while (i < numActors)
	{
		static_cast<Physics3dActor*>(arrayActors[i]->userData)->Hold();
		if (static_cast<Physics3dActor*>(arrayActors[i]->userData)->Drop())
		{
			arrayActors = scene->getActors();
			numActors = scene->getNbActors();
			continue;
		}
		++i;
	}
	static_cast<PhysXDevice*>(GetPhysics3dDevice())->GetPhysXSDK()->releaseScene(*scene);
}

NxScene* PhysXSceneManager::GetPhysXScene() const
{ return scene; }

void PhysXSceneManager::SetGravity(const Vec3df& g)
{ gravity = g; }

const Vec3df& PhysXSceneManager::GetGravity() const
{ return gravity; }

Physics3dDevice* PhysXSceneManager::GetPhysics3dDevice() const
{ return device; }

Physics3dActor*  PhysXSceneManager::AddStaticBoxActor(const Size3d& dim,
													  const Position3d& pos,
													  const Rotation3d& rot)
{ return new PhysXStaticBoxActor(this, dim, pos, rot); }

Physics3dActor*  PhysXSceneManager::AddDynamicBoxActor(const Size3d& dim,
													   const Position3d& pos,
													   const Rotation3d& rot)
{ return new PhysXDynamicBoxActor(this, dim, pos, rot); }

Physics3dActor*  PhysXSceneManager::AddDynamicSphereActor(const Float32 radius,
														  const Position3d& pos,
														  const Rotation3d& rot)
{ return new PhysXDynamicSphereActor(this, radius, pos, rot); }

Physics3dActor* PhysXSceneManager::AddStaticSphereActor(const Float32 radius,
									 const Position3d& pos,
									 const Rotation3d& rot)
{ return new PhysXStaticSphereActor(this, radius, pos, rot); }


Physics3dActor*  PhysXSceneManager::AddStaticTriangleMeshActor(const Mesh* mesh,
															   const Position3d& pos,
															   const Rotation3d& rot,
															   const Scale3d& scale)
{ return new PhysXStaticTriangleMeshActor(this, mesh, pos, rot, scale); }

Physics3dActor*  PhysXSceneManager::AddStaticPlaneActor(Float32 size,
														const Position3d& pos,
														const Rotation3d& rot)
{ return new PhysXStaticPlaneActor(this, size, pos, rot); }

UInt32 PhysXSceneManager::GetNumActors() const
{ return scene->getNbActors(); }

Physics3dActor* PhysXSceneManager::GetActor(UInt32 index)
{ return static_cast<Physics3dActor*>(((scene->getActors())[index])->userData); }

const Physics3dActor* PhysXSceneManager::GetActor(UInt32 index) const
{ return static_cast<Physics3dActor*>(((scene->getActors())[index])->userData); }

////////////////////////////////////////////////////////////////////////////////////////////////////
// PhysXActor
typedef ArrayList<bool>::iterator albit;

PhysXActor::PhysXActor(PhysXSceneManager* scene,
					   const Position3d& pos,
					   const Rotation3d& rot)
					   : Physics3dActor(scene, pos, rot), colEventFlags(CPET_ENUM_LENGTH)
{
	for (albit it = colEventFlags.begin(); it != colEventFlags.end(); ++it)
		*it = false;
}

PhysXActor::~PhysXActor()
{}

void PhysXActor::Update()
{
	if (!actor)
		return;

	NxMat34 pose = actor->getGlobalPose();

	const NxVec3 apos = pose.t;
	const NxMat33 orient = pose.M;
	Matrix4f mat;
	orient.getColumnMajorStride4(&mat[0]);
	apos.get(&mat[12]);

	mat[3] = mat[7] = mat[11] = 0.0f;
	mat[15] = 1.0f;

	SetPosition(mat.GetTranslation());
	if(/*updateRot*/true)
		SetRotation(mat.GetRotationDegrees());
}

void PhysXActor::AddForce(const Vec3df& vel)
{ if (IsDynamic()) actor->addForce(NxVec3(vel.x, vel.y, vel.z)); }

void PhysXActor::SetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct, bool enabled)
{
	NxU32 flags = actor->getContactReportFlags();
	if (enabled)
		flags |= ct;
	else
		flags &= ~ct;
	actor->setContactReportFlags(flags);
}

bool PhysXActor::GetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct)
{ return (actor->getContactReportFlags() & ct) != 0; }

MAKO_END_NAMESPACE
#endif // MAKO_PHYSX_AVAILABLE