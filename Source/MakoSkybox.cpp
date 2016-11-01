#include "MakoSkybox.h"
#include "MakoMeshManipulator.h"
#include "MakoScene3d.h"
#include "MakoCamera.h"
#include "MakoMeshData.h"
#include "MakoDiffTexMtl.h"
#include "MakoColorMtl.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include "MakoOS.h"

MAKO_BEGIN_NAMESPACE

Skybox::Skybox(Texture* front,
			   Texture* back,
			   Texture* right,
			   Texture* left,
			   Texture* up,
			   Texture* down)
			   : MeshSceneNode(APP()->MM()->MakeSkybox())
{
	// 1 - back
	// 2 - right
	// 3 - front
	// 4 - left
	// 5 - up
	// 6 - down
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(back ), 0);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(right), 2);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(front), 4);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(left ), 6);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(up   ), 8);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(down ), 10);
}

Skybox::Skybox(const SkyboxCreationParams& p)
: MeshSceneNode(APP()->MM()->MakeSkybox())
{
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.back ), 0);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.right), 2);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.front), 4);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.left ), 6);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.up   ), 8);
	GetMesh()->GetSubMesh(0)->SetSubMaterial(new DiffTexMtl(p.down ), 10);
}

Skybox::~Skybox() {}

void Skybox::Draw(GraphicsDevice* gd)
{	
	// Clamp texture addressing so there are no artifacts with bilinear+ texture
	// filtering
	TEXTURE_ADDRESS_MODE oldModeU = gd->GetTexAddressUMode();
	TEXTURE_ADDRESS_MODE oldModeV = gd->GetTexAddressVMode();

	gd->SetTexAddressUMode(TAM_CLAMP);
	gd->SetTexAddressVMode(TAM_CLAMP);

	MeshSceneNode::Draw(gd);

	gd->SetTexAddressUMode(oldModeU);
	gd->SetTexAddressVMode(oldModeV);
}

void Skybox::Update()
{

}

void Skybox::PostUpdate()
{
	Camera* camera = GetScene()->GetCamera();
	
	// Set position to be at the camera's to prevent the camera from
	// going through the skybox.
	SetPosition(camera->GetPosition());
}

void Skybox::PreDraw()
{
	Camera* camera = GetScene()->GetCamera();

	//camera->SetFarViewPlane(camera->GetFarViewPlane() * 100.f);

	// Set scale to fit within far view plane.
	SetScale(camera->GetFarViewPlane() / 2.f);
}

void Skybox::PostDraw()
{
	//Camera* camera = APP()->GetActive3dScene()->GetCamera();

	//camera->SetFarViewPlane(camera->GetFarViewPlane() / 100.f);
}

MAKO_END_NAMESPACE