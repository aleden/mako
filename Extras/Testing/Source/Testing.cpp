#define DEBUG_MEMORY_LEAKS
#if (defined (DEBUG) || defined (_DEBUG)) && defined (DEBUG_MEMORY_LEAKS)
#include <vld.h>
#endif

//#define MAKO_NO_HANDLE_EXC

#include <Mako.h>
using namespace Mako;

//#define TEST_SPRITES
//#define TEST_FONT
class ExampleApplication : public Mako::SimpleApplication
{
public:
	OSDevice* os;
	GraphicsDevice* gd;
	AudioDevice* ad;

	FPSCamera* cam;

#ifdef TEST_SPRITES
	Sprite* sprite0, * sprite1;
#endif

	Texture* tex1x, * tex2x;

	Font* font;

	void Initialize();
	void Frame();
	void Shutdown();
};


class SpaceshipMtl : public CgMtl
{
private:
	Float4x4CgParam* modelViewProjCgParam;
	Sampler2dCgParam* diffTexCgParam;
	Sampler2dCgParam* glowTexCgParam;

	Texture* diffTex;
	Texture* glowTex;
public:
	SpaceshipMtl()
		: CgMtl(APP()->GD()->GetCgDevice()->LoadShaderFromFile(Text("Spaceship.cg"), ASCIIText("Spaceshipv"),
		ASCIIText("Spaceshipf"))), diffTex(APP()->GD()->LoadTextureFromFile(Text("SpaceshipDiff2.jpg"))),
		glowTex(APP()->GD()->LoadTextureFromFile(Text("SpaceshipSpec2.jpg")))
	{
		diffTex->Hold();
		glowTex->Hold();

		modelViewProjCgParam = (Float4x4CgParam*)GetCgShader()->GetParameterByName("modelViewProj",
			CPT_FLOAT4X4, CPC_VERTEX_FUNCT);
		modelViewProjCgParam->Hold();

		diffTexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName("diffTex",
			CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
		diffTexCgParam->Hold();

		glowTexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName("specTex",
			CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
		glowTexCgParam->Hold();
	}

	~SpaceshipMtl()
	{
		modelViewProjCgParam->Drop();
		diffTexCgParam->Drop();
		glowTexCgParam->Drop();

		diffTex->Drop();
		glowTex->Drop();
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

class FlyingSpaceship : public MeshSceneNode
{
	Float32 FlyRadius;
	Float32 FlyHeight;
	Float32 speed;
	Vec3df lastAdd, v;
public:
	FlyingSpaceship(const Pos3d& p = Pos3d(0.f, 25.f, 0.f))
		: MeshSceneNode(APP()->GD()->LoadMeshFromFile(Text("Spaceship.MAKOMESH")), p),
		FlyRadius(30.f), FlyHeight(0.f), speed(20.f)
	{
		GetMesh()->GetSubMesh(0)->SetMaterial(new SpaceshipMtl);
	}

	void Update()
	{
#if 1
		Vec3df theta = Vec3df(APP()->OS()->GetElapsedTime()/(1000.f-speed),
			APP()->OS()->GetElapsedTime()/(1000.f-speed),
			APP()->OS()->GetElapsedTime()/(1000.f-speed));

		v = Vec3df(Sin<Float32>(theta.x)*FlyRadius,
			Sin<Float32>(theta.y)*FlyHeight,
			Cos<Float32>(theta.z)*FlyRadius);

		Move(v - lastAdd);

		SetRotation(0.f, theta.y*RADTODEG, 0.f);

		lastAdd = v;
#endif
	}
};

class MyKeyEvtRvr : public KeyEventReceiver
{
private:
	ExampleApplication* app;
public:
	MAKO_INLINE MyKeyEvtRvr(ExampleApplication* app) : app(app) {}
	MAKO_INLINE ~MyKeyEvtRvr() {}

	void OnEvent(const KeyEvent* event);
};

class MyMouseBtnEvtRvr : public MouseButtonEventReceiver
{
private:
	ExampleApplication* app;
public:
	MAKO_INLINE MyMouseBtnEvtRvr(ExampleApplication* app) : app(app) {}
	MAKO_INLINE ~MyMouseBtnEvtRvr() {}

	void OnEvent(const MouseButtonEvent* event);
};

class MyMouseWheelEvtRvr : public MouseWheelEventReceiver
{
private:
	ExampleApplication* app;
public:
	MAKO_INLINE MyMouseWheelEvtRvr(ExampleApplication* app) : app(app) {}
	MAKO_INLINE ~MyMouseWheelEvtRvr() {}

	void OnEvent(const MouseWheelEvent* event);
};

class MyPhysics3dPairEvtRvr : public Physics3dActorPairEventRecevier
{
public:
	MAKO_INLINE MyPhysics3dPairEvtRvr() {}
	MAKO_INLINE ~MyPhysics3dPairEvtRvr() {}

	void OnEvent(const Physics3dActorPairEvent* event);
};

void ExampleApplication::Initialize()
{
#ifdef TEST_SPRITES
	sprite0 = sprite1 = nullptr;
#endif

	GraphicsCreationParams creationparams;
	creationparams.wndTitle   = Text("Mako Testing");
	#define FULLSCREEN
#ifndef FULLSCREEN
	creationparams.wndSize    = Size2d(640, 480);
	creationparams.fullscreen = false;
#else
	creationparams.wndSize    = Size2d(1440, 900);
	creationparams.fullscreen = true;
#endif
	InitGraphics(creationparams);
	InitPhysics3d();
	InitAudio();

	gd = GetGraphicsDevice();
#ifdef TEST_AUDIO
	ad = GetAudioDevice();
#endif
	os = GetOSDevice();

	os->SetMouseLocked(true);

	FS()->AddDirectory(Text("../Media"));

	AddEventReceiver(new MyMouseBtnEvtRvr(this));
	AddEventReceiver(new MyPhysics3dPairEvtRvr);
	AddEventReceiver(new MyMouseWheelEvtRvr(this));
	AddEventReceiver(new MyKeyEvtRvr(this));

#ifdef TEST_FONT
	font = gd->LoadFontFromFile(Text("COLLEGEC.TTF"));
	font->SetSize(24);
	font->Hold();
#endif

#ifdef TEST_AUDIO
	AD()->Play2dSound(Text("WorldWithoutWords.wav"));
#endif

	FPSCameraCreationParams fpscamp;
	fpscamp.pos           = Pos3d(0.f, 0.f, 0.f);
	fpscamp.camSpeed      = .01f;
	fpscamp.farViewPlane  = 1000.f;
	fpscamp.nearViewPlane = .5f;
	fpscamp.fov           = 45.f * DEGTORAD;
	cam = new FPSCamera(fpscamp);
	GetActive3dScene()->SetCamera(cam);

	tex1x = GD()->LoadTextureFromFile(Text("spaceship.png"));
	tex1x->Hold();

	tex2x = tex1x->CreateStretched(tex1x->GetSize() * 6);
	tex2x->Hold();

#ifdef TEST_SPRITES
	sprite0 = new Sprite(this, tex2x, 0);
	sprite0->Hold();

	sprite1 = new Sprite(this, tex1x, Pos2d(100, 400));
	sprite1->Hold();
#endif

	GetActive3dScene()->Add(new FlyingSpaceship);

	SkyboxCreationParams skyboxp;
	skyboxp.front = gd->LoadTextureFromFile(Text("skybox2_front.jpg"));
	skyboxp.back  = gd->LoadTextureFromFile(Text("skybox2_back.jpg" ));
	skyboxp.right = gd->LoadTextureFromFile(Text("skybox2_right.jpg"));
	skyboxp.left  = gd->LoadTextureFromFile(Text("skybox2_left.jpg" ));
	skyboxp.up    = gd->LoadTextureFromFile(Text("skybox2_up.jpg"   ));
	skyboxp.down  = gd->LoadTextureFromFile(Text("skybox2_down.jpg" ));
	GetActive3dScene()->Add(new Skybox(skyboxp));

	StaticGeometryCreationParams sgcp;
	sgcp.mesh = gd->LoadMeshFromFile(Text("obj.MAKOMESH"));
	//sgcp.mesh->GetSubMesh(0)->SetMaterial(new DiffTexMtl(skyboxp.front));
	sgcp.p3ds = P3DD()->GetScene();

	StaticGeometry* level = new StaticGeometry(sgcp);
	GetActive3dScene()->Add(level);

	//texture* difftex = gd->LoadTextureFromFile(Text("brick029.jpg"));
	Texture* lmtex = gd->LoadTextureFromFile(Text("lightmap.png"));

#if 1
	typedef Map<UInt32, Material*>::iterator MtlIt;
	Map<UInt32, Material*>& submtls = level->GetMesh()->GetSubMesh(0)->GetSubMaterials();
	for (MtlIt it = submtls.begin(); it != submtls.end(); ++it)
	{
		LightmappedDiffTexMtl* newmtl = 
			new LightmappedDiffTexMtl(static_cast<DiffTexMtl*>((*it).second)->GetDiffuseTexture(),
			lmtex);
		(*it).second->Drop();
		(*it).second = newmtl;
		newmtl->Hold();
	}
#endif
}

void ExampleApplication::Frame()
{
#ifdef TEST_FONT

	String camRot = Text("ROTATION: (") + String::From32BitFloat(cam->GetRotation().x) +
		StringChar(',') + String::From32BitFloat(cam->GetRotation().y) +
		StringChar(',') + String::From32BitFloat(cam->GetRotation().z) + StringChar(')');
	//String camPos = Text("POSITION: (") + String::From32BitFloat(cam->GetPosition().x) +
	//	StringChar(',') + String::From32BitFloat(cam->GetPosition().y) +
	//	StringChar(',') + String::From32BitFloat(cam->GetPosition().z) + StringChar(')');

	//gd->Draw2dText(camPos, font, Pos2d(0, 10));
	gd->Draw2dText(camRot, font, Pos2d(0, 5));


	//gd->Draw2dText(GetConsole()->GetContents(), font, Pos2d(100, 0));
#endif

#ifdef TEST_SPRITES
	if (keyUp)
		sprite0->SetPosition(sprite0->GetPosition() + Pos2d(0, -1));
	if (keyDown)
		sprite0->SetPosition(sprite0->GetPosition() + Pos2d(0, 1));
	if (keyLeft)
		sprite0->SetPosition(sprite0->GetPosition() + Pos2d(-1, 0));
	if (keyRight)
		sprite0->SetPosition(sprite0->GetPosition() + Pos2d(1, 0));
#endif
}

void ExampleApplication::Shutdown()
{
#ifdef TEST_FONT
	if (font) font->Drop();
#endif

#ifdef TEST_SPRITES
	if (sprite0) sprite0->Drop();
	if (sprite1) sprite1->Drop();
#endif

	if (tex1x) tex1x->Drop();
	if (tex2x) tex2x->Drop();

	wprintf(GetConsole()->GetContents().ToWStringData());
	system("pause");
}

void MyMouseBtnEvtRvr::OnEvent(const MouseButtonEvent* event)
{
	// We only want to respond to mouse events where the button has
	// been clicked, not released (or both). 
	if (!event->IsDown())
		return ;

	switch (event->GetButton())
	{
	case MBTN_LEFT:
		{
			DynamicBoxCreationParams p;
			p.p3ds     = app->P3DD()->GetScene();
			p.size     = Size3d(RandFloat(1.5f, 3.f), RandFloat(1.5f, 3.f), RandFloat(1.5f, 3.f));
			p.pos      = Forward(app->cam->GetPosition(), app->cam->GetRotation(), 2.f);
			p.rot      = app->cam->GetRotation();
			p.material = new ColorMtl(app->gd, Color(RandUInt(0, 255), RandUInt(0, 255), RandUInt(0, 255), 255));

			DynamicBox* db = new DynamicBox(p);
			db->GetActor()->SetCollisionEventCondition(CPET_START_TOUCH);
			db->GetActor()->SetCollisionEventCondition(CPET_END_TOUCH);
			db->GetActor()->AddForce((p.pos - app->cam->GetPosition()) * 5000.f);

			app->GetActive3dScene()->Add(db);
			break;
		}
	case MBTN_RIGHT:
		{
			DynamicSphereCreationParams p;
			p.p3ds     = app->P3DD()->GetScene();
			p.radius   = RandFloat(1.2f, 2.5f);
			p.pos      = Forward(app->cam->GetPosition(), app->cam->GetRotation(), 2.f);
			p.rot      = app->cam->GetRotation();
			p.material = new ColorMtl(app->gd, Color(RandUInt(0, 255), RandUInt(0, 255), RandUInt(0, 255), 255));

			DynamicSphere* ds = new DynamicSphere(p);
			ds->GetActor()->AddForce((p.pos - app->cam->GetPosition()) * 10000.f);

			app->GetActive3dScene()->Add(ds);
			break;
		}
	}
}

void MyMouseWheelEvtRvr::OnEvent(const MouseWheelEvent* event)
{
	if (event->GetMovement() < 0)
		app->cam->SetMoveSpeed(app->cam->GetMoveSpeed() + app->cam->GetMoveSpeed()/2);
	else
		app->cam->SetMoveSpeed(app->cam->GetMoveSpeed() - app->cam->GetMoveSpeed()/2);
}

void MyPhysics3dPairEvtRvr::OnEvent(const Physics3dActorPairEvent* event)
{}

void MyKeyEvtRvr::OnEvent(const KeyEvent* event)
{
	if (!event->IsDown())
		return ;

	switch (event->GetKey())
	{
	case KEY_1:
		app->gd->SetOption(GDO_WIREFRAME, app->gd->GetOption(GDO_WIREFRAME) ? false : true);
		return ;
	case KEY_2:
		app->gd->SetOption(GDO_ZBUFFER, app->gd->GetOption(GDO_ZBUFFER) ? false : true);
		return ;
	case KEY_UP:
		{
			app->cam->SetMoveSpeed(app->cam->GetMoveSpeed() + app->cam->GetMoveSpeed()/2);
			return;
		}
	case KEY_DOWN:
		{
			app->cam->SetMoveSpeed(app->cam->GetMoveSpeed() - app->cam->GetMoveSpeed()/2);
			return;
		}
	}
}


RUN_MAKO_APPLICATION(ExampleApplication);