#pragma once
#include "MakoVec3d.h"
#include "MakoReferenceCounted.h"
#include "MakoScene3dNode.h"
#include "MakoArrayList.h"
#include "MakoMath.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Application;
class Mesh;
class Material;
class Texture;
class Physics3dActor;
class Camera;
class Entity3d;
class MeshSceneNode;
class Skybox;
class FPSCamera;
class StaticGeometry;
class Physics3dScene;
class DynamicBox;
class DynamicSphere;
class StaticSphere;
class StaticBox;
class StaticPlane;

//! The Scene3d manages Scene3dNodes. All Scene nodes can be created only here.
//! There is a always growing list of scene nodes for lots of purposes. 
//! A scene node is a node in the hierachical scene graph. Every scene node
//! may have children, which are other scene nodes. Children move relative
//! the their parents position. If the parent of a node is not visible, its
//! children won't be visible, too. In this way, it is for example easily
//! possible to attach a light to a moving car or to place a walking
//! character on a moving platform on a moving ship.
class Scene3d : public ReferenceCounted
{
private:
	Scene3dNode* root;
	Camera* cam;

	// Recursive functions
	void UpdateNodes_r(Scene3dNode* n);
	void PostUpdateNodes_r(Scene3dNode* n);
	void DrawNodes_r(Scene3dNode* n);
public:
	//! Constructor
	Scene3d();

	//! Deconstructor
	~Scene3d();

	//! Adds a node under the root
	MAKO_INLINE void Add(Scene3dNode* node)
	{ root->AddChild(node); }

	//! Not called by a user of the Mako Game Engine. Called inside the
	//! game loop of Mako::Application.
	MAKO_API void DrawAll();

	//! Get the root scene node of this scene
	//! \return The root scene node
	MAKO_INLINE Scene3dNode* GetRootNode() const
	{ return root; }

	//! Get the active camera for this scene.
	//! \return The camera
	MAKO_INLINE Camera* GetCamera() const
	{ return cam; }
	
	//! Set the active Camera for this scene
	//! \param[in] cam The new active camera
	MAKO_API void SetCamera(Camera* cam);
};

MAKO_END_NAMESPACE