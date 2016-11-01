#pragma once
#include "MakoCommon.h"
#include "MakoDevice.h"
#include "MakoVec3d.h"
#include "MakoReferenceCounted.h"
#include "MakoMesh.h"

MAKO_BEGIN_NAMESPACE

//! Protip: This enums' values are identical to the PhysX enum 
//! NxContactPairFlag counterparts.
enum CONTACT_PAIR_EVENT_TYPE
{
	CPET_START_TOUCH                 = (1<<1),
	CPET_END_TOUCH                   = (1<<2),
	CPET_TOUCH                       = (1<<3),
	CPET_START_TOUCH_FORCE_THRESHOLD = (1<<8),
	CPET_END_TOUCH_FORCE_THRESHOLD   = (1<<9),
	CPET_TOUCH_FORCE_THRESHOLD       = (1<<10),

	CPET_ALL = CPET_START_TOUCH | CPET_END_TOUCH | CPET_TOUCH | 
	           CPET_START_TOUCH_FORCE_THRESHOLD | CPET_END_TOUCH_FORCE_THRESHOLD |
	           CPET_TOUCH_FORCE_THRESHOLD,

	CPET_ENUM_LENGTH = 7
};

enum COLLISION_SHAPE
{ CS_BOX, CS_PLANE, CS_ENUM_LENGTH };

// Forward declaration
class Physics3dScene;

enum PHYSICS_3D_DEVICE_TYPE
{
#ifdef MAKO_PHYSX_AVAILABLE
	P3DT_PHYSX,
#endif
	P3DT_ENUM_LENGTH
};

//! This device manages 3d physics
class Physics3dDevice : public Device
{
public:
	//! Updates the device. Called each frame.
	virtual void Update(Float32 timeStep) = 0;

	//! Adds a scene
	//! \return The newly added scene
	virtual Physics3dScene* AddScene() = 0;

	//! Get the number of scenes
	//! \return The number of scenes
	virtual UInt32 GetNumScenes() const = 0;

	//! Get a scene
	//! \param[in] index The index of the scene
	//! \return The scene
	virtual Physics3dScene* GetScene(UInt32 index = 0) = 0;

	//! Get a scene
	//! \param[in] index The index of the scene
	//! \return The scene
	virtual const Physics3dScene* GetScene(UInt32 index) const = 0;

	//! Get the actual type of this Device.
	virtual PHYSICS_3D_DEVICE_TYPE GetType() const = 0;

	virtual ~Physics3dDevice() {}
};


//! Physics3dActor is the main simulation object for physics. 
//! The actor is owned by and contained in a Physics3dScene.
//! Physics3dActors cannot be scaled in size. For example, 
//! a 1.f size cube Physics3dActor's size cannot be changed
//! after it's first specification through the constructor.
class Physics3dActor : public ReferenceCounted
{
private:
	Rotation3d rot;
	Position3d pos;
	Physics3dScene* scene;
protected:
	MAKO_API void SetRotation(const Rotation3d& rot);
	MAKO_API void SetPosition(const Position3d& pos);
public:
	MAKO_API Physics3dActor(Physics3dScene* scene,
		                     const Position3d& pos,
							 const Rotation3d& rot);

	MAKO_API virtual ~Physics3dActor();

	//! Get the Physics3dScene that created this actor
	//! \return The Physics3dScene
	MAKO_API Physics3dScene* GetPhysics3dScene() const;
	
	//! Get the position of the actor
	//! \return The position
	MAKO_API const Position3d& GetPosition() const;

	//! Get the rotation of the actor
	//! \return The rotation
	MAKO_API const Rotation3d& GetRotation() const;

	//! Set a condition to be true so that collision event receivers
	//! will be notified when that condition is fulfilled
	//! \param[in] ct The condition
	//! \param[in] enabled If true, events will be posted based on ct. Else, they will not.
	virtual void SetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct, bool enabled = true) = 0;

	//! Check if a condition is true so that collision event receivers
	//! will be notified when that condition is fulfilled
	//! \param[in] ct The condition
	//! \return True if it is enabled, false if not
	virtual bool GetCollisionEventCondition(CONTACT_PAIR_EVENT_TYPE ct) = 0;
	
	//! Tells if the actor is dynamic or not
	//! \return True if the actor is dynamic, false if static
	virtual bool IsDynamic() const = 0;

	//! Add a force to the actor
	//! \param[in] vel The force to be added
	virtual void AddForce(const Vec3df& vel) = 0;

	//! Only used by Physics3dScene to update, and only when
	//! actor is dynamic.
	virtual void Update() = 0;
};

//! A scene is a collection of bodies, constraints, and effectors which can interact. 
//! The scene simulates the behavior of these objects over time. Several scenes may 
//! exist at the same time, but each body, constraint, or effector object is specific
//! to a scene -- they may not be shared.
//!
//! For example, attempting to create a joint in one scene and then using it to attach
//! bodies from a different scene results in undefined behavior.
class Physics3dScene : public ReferenceCounted
{
public:
	//! Get the Physics3dDevice that created this scene
	//! \return The device
	virtual Physics3dDevice* GetPhysics3dDevice() const = 0;

	//! Add a static box actor to this scene
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \param[in] dim The dimension of the box
	//! \return The newly added static box actor
	virtual Physics3dActor*  AddStaticBoxActor(const Size3d& dim,
		                                       const Position3d& pos,
		                                       const Rotation3d& rot) = 0;

	//! Add a dynamic box actor to this scene
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \param[in] dim The dimension of the box
	//! \return The newly added dynamic box actor
	virtual Physics3dActor*  AddDynamicBoxActor(const Size3d& dim,
		                                        const Position3d& pos,
		                                        const Rotation3d& rot) = 0;

	//! Add a dynamic sphere actor to this scene
	//! \param[in] radius The radius of the sphere
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \return The newly added dynamic sphere actor
	virtual Physics3dActor*  AddDynamicSphereActor(const Float32 radius,
		                                           const Position3d& pos,
		                                           const Rotation3d& rot) = 0;
	//! Add a static sphere actor to this scene
	//! \param[in] radius The radius of the sphere
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \return The newly added static sphere actor
	virtual Physics3dActor* AddStaticSphereActor(const Float32 radius,
		                                         const Position3d& pos,
		                                         const Rotation3d& rot) = 0;

	//! Add a static flat plane (square) actor to this scene
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \param[in] size The size of the plane (it's length on all sides)
	//! \return The newly added actor
	virtual Physics3dActor*  AddStaticPlaneActor(Float32 size,
		                                         const Position3d& pos,
		                                         const Rotation3d& rot) = 0;

	//! Add a static triangle mesh actor to this scene. A triangle mesh actor
	//! uses a mesh of triangles to form it's collision shape. This is used a lot
	//! for indoor game environments.
	//! \param[in] mesh The triangle mesh
	//! \param[in] pos The initial position
	//! \param[in] rot The initial rotation
	//! \return The newly added actor
	virtual Physics3dActor*  AddStaticTriangleMeshActor(const Mesh* mesh,
		                                                const Position3d& pos = Pos3d(),
														const Rotation3d& rot = Rot3d(),
														const Scale3d& scale  = Scale3d(1.f)) = 0;
	//! Get the number of actors present in the scene
	//! \return The number of actors
	virtual UInt32 GetNumActors() const = 0;
	
	//! Set the gravity of this scene
	//! \param[in] g The new gravity
	virtual void SetGravity(const Vec3df& g) = 0;
	
	//! Get the gravity of this scene
	//! \return The gravity
	virtual const Vec3df& GetGravity() const = 0;
	
	//! Get an actor from this scene at a certain index
	//! \param[in] index The index of the actor
	//! \return The actor
	virtual Physics3dActor* GetActor(UInt32 index) = 0;
	
	//! Get an actor from this scene at a certain index
	//! \param[in] index The index of the actor
	//! \return The actor
	virtual const Physics3dActor* GetActor(UInt32 index) const = 0;
};

MAKO_END_NAMESPACE

