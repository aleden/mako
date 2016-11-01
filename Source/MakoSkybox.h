#pragma once

#include "MakoMeshSceneNode.h"
#include "MakoMesh.h"
#include "MakoArrayList.h"


MAKO_BEGIN_NAMESPACE

class Texture;

struct SkyboxCreationParams
{
	Texture* front;
	Texture* back;
	Texture* right;
	Texture* left;
	Texture* up;
	Texture* down;
};

//! A skybox scene node. A skybox is simply a cube with it's normals flipped, 
//! and a different texture applied to each face. Without lighting, it appears
//! that all around the camera is a seamless sky and not a box.
class Skybox : public MeshSceneNode
{
public:
	//! Constructor taking in all textures
	MAKO_API Skybox(Texture* front,
		   Texture* back,
		   Texture* right,
		   Texture* left,
		   Texture* up,
		   Texture* down);
	MAKO_API Skybox(const SkyboxCreationParams& params);
	
	//! Deconstructor
	MAKO_API ~Skybox();
	
	MAKO_API void Update();

	//! Draws skybox
	MAKO_API void Draw(GraphicsDevice* gd);
	
	//! Sets skybox's position to the active scene's active camera's
	//! position, and sets the skybox's scale to within the near and far
	//! view plane of the camera.
	MAKO_API void PostUpdate();

	MAKO_API void PostDraw();

	MAKO_API void PreDraw();
};

MAKO_END_NAMESPACE