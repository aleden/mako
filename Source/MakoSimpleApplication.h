#pragma once
#include "MakoApplication.h"
#include "MakoString.h"
#include "MakoVec2d.h"
#include "MakoLinkedList.h"
#include "MakoArrayList.h"
#include "MakoGraphicsDevice.h"
#include "MakoAudioDevice.h"
#include "MakoNetworkingDevice.h"
#include "MakoPhysics3dDevice.h"
#include "MakoFileSystem.h"

MAKO_BEGIN_NAMESPACE

//! This struct describes how the GraphicsDevice should be initialized.
struct GraphicsCreationParams
{
	String wndTitle;
	Size2d wndSize;
	bool fullscreen;

	//! Vertical synchronization (v-sync, v-synch, vbl-sync, vbl-synch) refers generally 
	//! to the synchronization of frame changes with the vertical blanking interval. 
	//! Since CRTs were nearly the only common video display technology prior to the 
	//! widespread adoption of LCDs, the frame buffers in computer graphics hardware are 
	//! designed to match the CRT characteristic of drawing images from the top down a 
	//! line at a time by replacing the data of the previous frame in the buffer with that 
	//! of the next frame in a similar fashion. If the frame buffer is updated with a new 
	//! image while the image is being transmitted to the display, the frame buffer gives 
	//! it the current mishmash of both frames, producing a page tearing artifact partway 
	//! down the image.
	//!
	//! Vertical synchronization eliminates this by timing frame buffer fills to coincide 
	//! with the vertical blanking interval, thus ensuring that only whole frames are seen 
	//! on-screen.
	bool vsync;
	GRAPHICS_DEVICE_TYPE deviceType;
	
	//! \param[in] deviceType By default this is set to be auto-detected.
	MAKO_INLINE GraphicsCreationParams(const String& wndTitle = Text("Mako Application"),
		                               const Size2d& wndSize = Size2d(640, 480),
									   bool fullscreen = false,
									   bool vsync = true,
									   GRAPHICS_DEVICE_TYPE deviceType = (GRAPHICS_DEVICE_TYPE)~0)
									   : wndTitle(wndTitle), wndSize(wndSize), fullscreen(fullscreen), vsync(vsync) {}
	MAKO_INLINE ~GraphicsCreationParams() {}
};

struct AudioCreationParams
{
	AUDIO_DEVICE_TYPE deviceType;

	//! \param[in] deviceType By default this is set to be auto-detected.
	MAKO_INLINE AudioCreationParams(AUDIO_DEVICE_TYPE deviceType = (AUDIO_DEVICE_TYPE)~0)
		: deviceType(deviceType) {}
	
	MAKO_INLINE ~AudioCreationParams() {}
};

struct NetworkingCreationParams
{
	NETWORKING_DEVICE_TYPE deviceType;

	//! \param[in] deviceType By default this is set to be auto-detected.
	MAKO_INLINE NetworkingCreationParams(NETWORKING_DEVICE_TYPE deviceType = (NETWORKING_DEVICE_TYPE)~0)
		: deviceType(deviceType) {}
	
	MAKO_INLINE ~NetworkingCreationParams() {}
};

struct Physics3dCreationParams
{
	PHYSICS_3D_DEVICE_TYPE deviceType;

	//! \param[in] deviceType By default this is set to be auto-detected.
	MAKO_INLINE Physics3dCreationParams(PHYSICS_3D_DEVICE_TYPE deviceType = (PHYSICS_3D_DEVICE_TYPE)~0)
		: deviceType(deviceType) {}
	
	MAKO_INLINE ~Physics3dCreationParams() {}
};

//! This is a simple Mako application implementation to inherit from in order
//! to program your game. This implementation is usually used for demos
//! and not actual games.
class SimpleApplication : public Application
{
private:
	GraphicsDevice*    graphics;
	AudioDevice*       audio;
	Physics3dDevice*   phys3d;
	Scene3d*           scene3d;
	Scene2d*           scene2d;	
	RenderedWindow*    rw;
	OSDevice*          os;
	MeshManipulator*   mm;
	Console*           console;
	NetworkingDevice*  net;
	FileSystem*        fs;


	bool isRunning;
	ArrayList<String> cmdLnArgs;
	ArrayList<LinkedList<EventReceiver*> > eventReceivers;
	UInt32 fps;
	UInt32 fpsUpdateCounter;
public:
	//! Constructor for SimpleApplication. This should not be called 
	//! by a user. It is called inside MAKO_RUN_APPLICATION().
	MAKO_API SimpleApplication();
	
	//! Deconstructor for SimpleApplication. Inside this it calls Shutdown().
	MAKO_API virtual ~SimpleApplication();

	//! This function must be called in Initialize() before calling any 
	//! other mako graphics related function calls (textures, meshes, etc.)
	//! \param[in] params Describing how the GraphicsDevice should be initialized.
	MAKO_API virtual void InitGraphics(const GraphicsCreationParams& params = GraphicsCreationParams());

	//! This function must be called in Initialize() before calling any 
	//! other mako audio related function calls.
	//! \param[in] params Describing how the AudioDevice should be initialized.
	MAKO_API virtual void InitAudio(const AudioCreationParams& params = AudioCreationParams());

	//! This function must be called in Initialize() before calling any 
	//! other mako networking related function calls.
	//! \param[in] params Describing how the NetworkingDevice should be initialized.
	MAKO_API virtual void InitNetworking(const NetworkingCreationParams& params = NetworkingCreationParams());

	//! This function must be called in Initialize() before calling any 
	//! other mako 3d physics related function calls (Entity3d nodes, Physics3dActors, etc.)
	//! \param[in] params Describing how the Physics3dDevice should be initialized.
	MAKO_API virtual void InitPhysics3d(const Physics3dCreationParams& params = Physics3dCreationParams());
	
	//! Users of SimpleApplication must implement this method in order
	//! to do something in the game loop. It is called exactly once
	//! every frame.
	MAKO_API MAKO_INLINE virtual void Frame() {}

	MAKO_INLINE GraphicsDevice*   GetGraphicsDevice()   const { return graphics; }
	MAKO_INLINE AudioDevice*      GetAudioDevice()      const { return audio;    }
	MAKO_INLINE Scene3d*          GetActive3dScene()    const { return scene3d;  }
	MAKO_INLINE Scene2d*          GetActive2dScene()    const { return scene2d;  }
	MAKO_INLINE MeshManipulator*  GetMeshManipulator()  const { return mm;       }
	MAKO_INLINE OSDevice*         GetOSDevice()         const { return os;       }
	MAKO_INLINE FileSystem*       GetFileSystem()       const { return fs;       }
	MAKO_INLINE RenderedWindow*   GetRenderedWindow()   const { return rw;       }
	MAKO_INLINE Console*          GetConsole()          const { return console;  }
	MAKO_INLINE Physics3dDevice*  GetPhysics3dDevice()  const { return phys3d;   }
	MAKO_INLINE NetworkingDevice* GetNetworkingDevice() const { return net;      }
	
	MAKO_INLINE const ArrayList<String>& GetCmdLnArgs() const
	{ return cmdLnArgs; }

	//! Not to be called by a client of the Mako library. This is used in the 
	//! RUN_MAKO_APPLICATION macro.
	MAKO_API void SetCommandLineArguments(Int32 argc, Int8** argv);
	
	MAKO_API UInt32 GetFPS() const
	{ return fps; }
	
	MAKO_API void PostEvent(Event* e);
	MAKO_API void AddEventReceiver(EventReceiver* er);
	MAKO_API void RemoveEventReceiver(EventReceiver* er);
	MAKO_API void SetScene(Scene3d* scene);
	
	MAKO_INLINE bool HasQuitted() const
	{ return !isRunning; }
	
	MAKO_API void Quit();
	MAKO_API void Run();
private:
	MAKO_INLINE UInt32 FPSDeadZone(UInt32 fps) const;
	MAKO_INLINE UInt32 CalculateFPS() const;

	void LogToConsoleAllDevicesBeingUsed();
};

MAKO_END_NAMESPACE