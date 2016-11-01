#include "MakoSimpleApplication.h"
#include "MakoWinsockDevice.h"
#include "MakoXAudio2Device.h"
#include "MakoD3D9Device.h"
#include "MakoSimpleKeyEventReceiver.h"
#include "MakoScene3d.h"
#include "MakoScene2d.h"
#include "MakoMeshManipulator.h"
#include "MakoWindowsDevice.h"
#include "MakoException.h"
#include "MakoOSDevice.h"
#include "MakoConsole.h"
#include "MakoPhysXDevice.h"
#include "MakoColorMtl.h"
#include "MakoWireframeMtl.h"
#include "MakoNetworkingDevice.h"
#include "MakoVERSION.h"

MAKO_BEGIN_NAMESPACE

///////////////////////////////////////////////////////////////////////////////////////////
// Initializer Methods/Deconstructor

SimpleApplication::SimpleApplication()
: eventReceivers(ET_ENUM_LENGTH), fpsUpdateCounter(0), fps(0), graphics(nullptr),
  audio(nullptr), phys3d(nullptr), scene3d(nullptr), scene2d(nullptr), rw(nullptr),
  os(nullptr), mm(nullptr), console(nullptr), net(nullptr), fs(nullptr), isRunning(true)
{
	/////////////////////////////////////////////////////////////////////////////
	// Console
	console = new Console();
	console->PrintLn(Text("Mako Game Engine v") + String::From32BitFloat(GetMakoVersion()) + '\n');

	/////////////////////////////////////////////////////////////////////////////
	// OSDevice
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	os = new WindowsDevice();
#endif

	/////////////////////////////////////////////////////////////////////////////
	// FileSystem
	fs = new FileSystem();
}


void SimpleApplication::InitGraphics(const GraphicsCreationParams& p)
{
	/////////////////////////////////////////////////////////////////////////////
	// RenderedWindow
	rw = os->CreateRenderedWindow(p.wndTitle, p.wndSize, p.fullscreen);

	/////////////////////////////////////////////////////////////////////////////
	// GraphicsDevice
#if defined(MAKO_D3D_AVAILABLE) && MAKO_D3D_VER >= MAKO_D3D_VER_9
	graphics = new D3D9Device(p.vsync);
#endif
	console->PrintLn(Text("Initialized Graphics (") + graphics->GetName() + StringChar(')'));
	graphics->SetDefaultMaterial(new ColorMtl(graphics));

	/////////////////////////////////////////////////////////////////////////////
	// Scene3d
	scene3d = new Scene3d();
	scene3d->SetCamera(new Camera);
	scene3d->Hold();
	
	/////////////////////////////////////////////////////////////////////////////
	// Scene2d
	scene2d = new Scene2d();
	scene2d->Hold();
	
	/////////////////////////////////////////////////////////////////////////////
	// MeshManipulator
	mm = new MeshManipulator();
}

void SimpleApplication::InitNetworking(const NetworkingCreationParams& params)
{
	/////////////////////////////////////////////////////////////////////////////
	// NetworkingDevice
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	net = new WinsockDevice();
#endif
	console->PrintLn(Text("Initialized Networking (") + net->GetName() + StringChar(')'));
}

void SimpleApplication::InitPhysics3d(const Physics3dCreationParams& params)
{
	/////////////////////////////////////////////////////////////////////////////
	// Physics3dDevice
#ifdef MAKO_PHYSX_AVAILABLE
	phys3d = new PhysXDevice();
#endif
	console->PrintLn(Text("Initialized 3D Physics (") + phys3d->GetName() + StringChar(')'));
}

void SimpleApplication::InitAudio(const AudioCreationParams& params)
{
	/////////////////////////////////////////////////////////////////////////////
	// AudioDevice
#ifdef MAKO_XAUDIO2_AVAILABLE
	audio = new XAudio2Device();
#endif
	console->PrintLn(Text("Initialized Audio (") + audio->GetName() + StringChar(')'));
}

SimpleApplication::~SimpleApplication()
{
	if (scene3d) scene3d->Drop();
	if (scene2d) scene2d->Drop();
	
	// Array List Linked List Event Receiver Iterator
	typedef ArrayList<LinkedList<EventReceiver*> >::iterator Alllerit;
	// Linked List Event Receiver Iterator
	typedef LinkedList<EventReceiver*>::iterator Llerit;
	for (Alllerit it1 = eventReceivers.begin(); it1 != eventReceivers.end(); ++it1)
	{
		for (Llerit it2 = (*it1).begin(); it2 != (*it1).end();)
		{
			EventReceiver* temp = *it2;
			it2 = (*it1).erase(it2);
			temp->Drop();
		}
	}
	if (net)      delete net;
	if (audio)    delete audio;
	if (os)       delete os;
	if (rw)       delete rw;
	if (phys3d)   delete phys3d;
	if (mm)       delete mm;
	if (graphics) delete graphics;
	if (fs)       delete fs;

#if defined (MAKO_DEBUG_MODE) && MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	if (console) OutputDebugStringW((Text("\n") + console->GetContents() + Text("\n")).ToWStringData());
#endif
	if (console)  delete console;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Methods

void SimpleApplication::SetCommandLineArguments(Int32 argc, Int8** argv)
{
	cmdLnArgs.clear();
	cmdLnArgs.resize(static_cast<UInt32>(argc));
	for (Int32 i = 0; i < argc; ++i)
	{
		cmdLnArgs[i] = ToString(argv[i]);

		// Replace all '\\' with '/'
		for (UInt ci = 0; ci < cmdLnArgs[i].GetLength(); ++ci)
		{
			if (cmdLnArgs[i][ci] == '\\')
				cmdLnArgs[i][ci] = '/';
		}
	}
}

void SimpleApplication::AddEventReceiver(EventReceiver* er)
{
	er->Hold();
	eventReceivers[er->GetTypeOfEventHandled()].push_back(er);
}

void SimpleApplication::RemoveEventReceiver(EventReceiver* er)
{
	EVENT_TYPE type = er->GetTypeOfEventHandled();
	LinkedList<EventReceiver*>::const_iterator iter;
	for(iter  = eventReceivers[type].begin();
		iter != eventReceivers[type].end();
		++iter)
	{
		if ((*iter) == er)
		{
			(*iter)->Drop();
			eventReceivers[type].erase(iter);
			return ;
		}
	}
	console->Log(LL_LOW, Text("The EventReceiver* given to SimpleApplication::"
		L"RemoveEventReceiver() could not be removed because it was not found."));
	return ;
}

void SimpleApplication::PostEvent(Event* e)
{
	LinkedList<EventReceiver*>::const_iterator iter;
	switch (e->GetEventType())
	{
	case ET_KEY:
		{
			for(iter  = eventReceivers[e->GetEventType()].begin();
				iter != eventReceivers[e->GetEventType()].end();
				++iter)
			{
				static_cast<KeyEventReceiver*>(*iter)->OnEvent(static_cast<KeyEvent*>(e));
			}
			return ;
		}
	case ET_MOUSE_BTN:
		{
			for(iter  = eventReceivers[e->GetEventType()].begin();
				iter != eventReceivers[e->GetEventType()].end();
				++iter)
			{
				static_cast<MouseButtonEventReceiver*>(*iter)->OnEvent(static_cast<MouseButtonEvent*>(e));
			}
			return ;
		}
	case ET_MOUSE_MOVE:
		{
			for(iter  = eventReceivers[e->GetEventType()].begin();
				iter != eventReceivers[e->GetEventType()].end();
				++iter)
			{
				static_cast<MouseMoveEventReceiver*>(*iter)->OnEvent(static_cast<MouseMoveEvent*>(e));
			}
			return ;
		}
	case ET_MOUSE_WHEEL:
		{
			for(iter  = eventReceivers[e->GetEventType()].begin();
				iter != eventReceivers[e->GetEventType()].end();
				++iter)
			{
				static_cast<MouseWheelEventReceiver*>(*iter)->OnEvent(static_cast<MouseWheelEvent*>(e));
			}
			return ;
		}
	case ET_PHYSICS_3D_ACTOR_PAIR_EVENT:
		{
			for(iter  = eventReceivers[e->GetEventType()].begin();
				iter != eventReceivers[e->GetEventType()].end();
				++iter)
			{
				static_cast<Physics3dActorPairEventRecevier*>(*iter)->OnEvent(static_cast<Physics3dActorPairEvent*>(e));
			}
			return ;
		}
	}
}

void SimpleApplication::SetScene(Scene3d* scene)
{ this->scene3d->Drop(); this->scene3d = scene; }

#define FPS_DEADZONE_RANGE 2

UInt32                       SimpleApplication::FPSDeadZone(UInt32 fps) const
{ return graphics->IsVSyncEnabled() && fps <= 60 + 2 && fps >= 60 - 2 ? 60 : fps; }

UInt32 SimpleApplication::CalculateFPS() const
{
	UInt32 ct = os->GetChangeInTime();
	return ct == 0 ? 0 : FPSDeadZone(1000 / ct);
}

void SimpleApplication::Quit()
{
	if (rw)
		rw->Close();
	isRunning = false;
}

void SimpleApplication::Run()
{
	isRunning = true;
	while (isRunning)
	{
		if (graphics)
			graphics->BeginScene();

		fpsUpdateCounter += os->GetChangeInTime();
		if (fpsUpdateCounter > 1000)
		{
			fps = CalculateFPS();
			fpsUpdateCounter = 0;
		}
		if (scene3d)
			scene3d->DrawAll();
		if (scene2d)
			scene2d->DrawAll();

		if (os)
			os->Update();
		if (audio)
			audio->Update();
		if (phys3d)
			phys3d->Update(graphics->IsVSyncEnabled() ? .01666f : ((Float32)os->GetChangeInTime())/1000.f );
		if (rw)
			rw->Update();

		Frame();
		if (graphics)
			graphics->EndScene();

	}
}
MAKO_END_NAMESPACE
