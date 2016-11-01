#define DEBUG_MEMORY_LEAKS
#if (defined (DEBUG) || defined (_DEBUG)) && defined (DEBUG_MEMORY_LEAKS)
#include <vld.h>
#endif
#include <Mako.h>
using namespace Mako;

class EarthMtl : public CgMtl
{
private:
	Float4x4CgParam* modelViewProjCgParam;
	Sampler2dCgParam* diffTexCgParam;
	Sampler2dCgParam* glowTexCgParam;

	Texture* diffTex;
	Texture* glowTex;
public:
	EarthMtl(Texture* diff, Texture* glow)
		: CgMtl(diff->GetGraphicsDevice()->GetCgDevice()->LoadShaderFromFile(Text("EarthShader.cg"),
		  "EarthShaderv", "EarthShaderf")), diffTex(diff), glowTex(glow)
	{
		modelViewProjCgParam = (Float4x4CgParam*)GetCgShader()->GetParameterByName("modelViewProj",
		CPT_FLOAT4X4, CPC_VERTEX_FUNCT);
		modelViewProjCgParam->Hold();

		diffTexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName("diffTex",
			CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
		diffTexCgParam->Hold();

		glowTexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName("glowTex",
			CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
		glowTexCgParam->Hold();
	}

	~EarthMtl()
	{
		modelViewProjCgParam->Drop();
		diffTexCgParam->Drop();
		glowTexCgParam->Drop();
	}

	void Bind(GraphicsDevice* gd) const
	{
		CgMtl::Bind(gd);

		GetCgShader()->BeginEditingParameters();

		modelViewProjCgParam->SetValue((gd->GetTransform(TS_PROJECTION) *
										gd->GetTransform(TS_VIEW) *
										gd->GetTransform(TS_WORLD)).GetTransposed());
		diffTexCgParam->SetValue(diffTex);
		glowTexCgParam->SetValue(glowTex);

		GetCgShader()->EndEditingParameters();
	}
};


class EarthDemo : public SimpleApplication
{
public:
	OSDevice* os;
	GraphicsDevice* gd;
	AudioDevice* ad;

	MeshSceneNode* earthNode;
	MeshData* starsMeshData;
	Camera* cam;
	EarthMtl* earthMtl;

	Texture* diffTex, * glowTex;

	Float32 distanceFromEarth;

	void Initialize();
	void Frame();
	void Shutdown();
};

class MouseBtnEvtRvr : public MouseButtonEventReceiver
{
private:
	EarthDemo* app;
public:
	MAKO_INLINE MouseBtnEvtRvr(EarthDemo* app) : app(app) {}
	MAKO_INLINE ~MouseBtnEvtRvr() {}

	void OnEvent(const MouseButtonEvent* event);
};

class MouseMoveEvtRvr : public MouseMoveEventReceiver
{
private:
	EarthDemo* app;
public:
	MAKO_INLINE MouseMoveEvtRvr(EarthDemo* app) : app(app) {}
	MAKO_INLINE ~MouseMoveEvtRvr() {}

	void OnEvent(const MouseMoveEvent* event);
};

class MouseWheelEvtRvr : public MouseWheelEventReceiver
{
private:
	EarthDemo* app;
public:
	MAKO_INLINE MouseWheelEvtRvr(EarthDemo* app) : app(app) {}
	MAKO_INLINE ~MouseWheelEvtRvr() {}

	void OnEvent(const MouseWheelEvent* event);
};

class KeyEvtRvr : public KeyEventReceiver
{
private:
	EarthDemo* app;
public:
	MAKO_INLINE KeyEvtRvr(EarthDemo* app) : app(app) {}
	MAKO_INLINE ~KeyEvtRvr() {}

	void OnEvent(const KeyEvent* event);
};

void EarthDemo::Initialize()
{
	starsMeshData = nullptr;

	GraphicsCreationParams creationparams;
	creationparams.wndTitle   = Text("Earth Visualization");
//#define FULLSCREEN
#ifndef FULLSCREEN
	creationparams.wndSize    = Size2d(640, 480);
	creationparams.fullscreen = false;
#else
	creationparams.wndSize    = Size2d(1280, 800);
	creationparams.fullscreen = true;
#endif
	FS()->AddDirectory(Text("../Media"));

	InitGraphics(creationparams);

	AddEventReceiver(new MouseBtnEvtRvr(this));
	AddEventReceiver(new MouseMoveEvtRvr(this));
	AddEventReceiver(new MouseWheelEvtRvr(this));
	AddEventReceiver(new KeyEvtRvr(this));

	gd = GetGraphicsDevice();
	ad = GetAudioDevice();
	os = GetOSDevice();

	distanceFromEarth = -25.f;

	cam = GetActive3dScene()->GetCamera();

	os->SetMouseLocked(true);
	gd->SetBackgroundColor(Color(0, 0, 0, 255));

	GetActive3dScene()->GetCamera()->SetFarViewPlane(10000.f);

	diffTex = gd->LoadTextureFromFile(Text("EarthDiffuse.jpg"));
	diffTex->Hold();
	glowTex = gd->LoadTextureFromFile(Text("EarthGlow.jpg"));
	glowTex->Hold();

	earthMtl = new EarthMtl(diffTex, glowTex);
	earthMtl->Hold();

	MeshSceneNodeCreationParams msncp;
	msncp.mesh = GetMeshManipulator()->MakeSphere(10.f, 50, 50);
	msncp.mesh->GetSubMesh(0)->SetMaterial(earthMtl);

	GetActive3dScene()->Add(earthNode = new MeshSceneNode(msncp));
	earthNode->Hold();

	Color starColor(255, 255, 255, 255);
	const UInt32 numStars = 1000;
	const Float32 starRange = 700.f;
	const Float32 minimalDist = 450.f;

	ArrayList<StandardVertex> starPositions(numStars);

	for (UInt32 i = 0; i < numStars; ++i)
	{
		Pos3d starPos;
		do
		{
			starPos = Pos3d(RandFloat(-starRange, starRange),
				            RandFloat(-starRange, starRange),
							RandFloat(-starRange, starRange));
		} while (Distance(starPos, Pos3d(0, 0, 0)) > minimalDist);
		
		starPositions[i] = StandardVertex(starPos);
	}


	MeshDataCreationParams mdcp;
	mdcp.materials[0]  = new ColorMtl(gd, starColor);
	mdcp.numPrimitives = starPositions.size();
	mdcp.numVertices   = starPositions.size();
	mdcp.primitiveType = PT_POINTLIST;
	mdcp.vertexType    = VT_STANDARD;
	mdcp.vertices      = &starPositions[0];
	starsMeshData = gd->CreateMeshData(mdcp);
	starsMeshData->Hold();
}

void EarthDemo::Frame()
{
	cam->SetPosition(Forward(Pos3d(0), cam->GetRotation(), distanceFromEarth));
	gd->SetTransform(Matrix4f(), TS_WORLD);
	gd->DrawMeshData(starsMeshData);
}

void EarthDemo::Shutdown()
{
	if (starsMeshData) starsMeshData->Drop();
	if (diffTex) diffTex->Drop();
	if (glowTex) glowTex->Drop();
	if (earthNode) earthNode->Drop();
	if (earthMtl) earthMtl->Drop();

	wprintf(GetConsole()->GetContents().ToWStringData());
	system("pause");
}

void MouseBtnEvtRvr::OnEvent(const MouseButtonEvent* event) {}

void MouseMoveEvtRvr::OnEvent(const MouseMoveEvent* event)
{
	const Float32 rotSpeed = .5f;

	Vec3df rotVecX(0.f, (Float32)event->GetMovement().x, 0.f);
	Vec3df camRot = app->cam->GetRotation();
	if ((camRot + rotVecX*rotSpeed).x > -90.f && (camRot + rotVecX*rotSpeed).x < 90.f)
		app->cam->SetRotation(camRot + rotVecX*rotSpeed);
	
	Vec3df rotVecY((Float32)event->GetMovement().y, 0.f, 0.f);
	camRot = app->cam->GetRotation();
	if ((camRot + rotVecY*rotSpeed).x > -90 && (camRot + rotVecY*rotSpeed).x < 90)
		app->cam->SetRotation(camRot + rotVecY*rotSpeed);
}

void MouseWheelEvtRvr::OnEvent(const MouseWheelEvent* event)
{
	if (app->distanceFromEarth + static_cast<Float32>(event->GetMovement())/70.f < -10.f)
		app->distanceFromEarth += static_cast<Float32>(event->GetMovement())/70.f;
}

void KeyEvtRvr::OnEvent(const KeyEvent* event)
{
	if (!event->IsDown())
		return ;

	switch (event->GetKey())
	{
	case KEY_UP:
		{
			if (app->distanceFromEarth + 4.5f < -10.f)
				app->distanceFromEarth += 4.5f;
			return;
		}
	case KEY_DOWN:
		{
			if (app->distanceFromEarth - 4.5f < -10.f)
				app->distanceFromEarth -= 4.5f;
			return;
		}
	}
}

RUN_MAKO_APPLICATION(EarthDemo)