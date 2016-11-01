#pragma once
#include "MakoCommon.h"
#include "MakoArrayList.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class EventReceiver;
class GraphicsDevice;
class AudioDevice;
class Scene3d;
class Scene2d;
class MeshManipulator;
template <typename T>
class Vec2d;
class RenderedWindow;
class OSDevice;
class Event;
class MeshLoader;
class Console;
class Physics3dDevice;
class NetworkingDevice;
class FileSystem;

//! This is the most important class in the Mako Game Engine,
//! it ties together all the devices and other functionalities.
//! To create a Mako game, you must create a class that inherits
//! from an implementation of this. There are some rules that are
//! not syntactically enforced:
//!
//! \li Each implementation of Application must have a constructor
//! that takes zero arguments.
//! \li Each implementation of Application must have a 
class Application
{
public:
	Application();
	virtual ~Application() {}

	//! Gets the graphics device
	//! \return The graphics device
	virtual GraphicsDevice* GetGraphicsDevice() const = 0;

	//! Shorter way of saying in code to get the graphics device. Identical
	//! to GetGraphicsDevice().
	MAKO_REALINLINE GraphicsDevice* GD() const
	{ return GetGraphicsDevice(); }

	//! Gets the NetworkingDevice
	//! \return The NetworkingDevice
	virtual NetworkingDevice* GetNetworkingDevice() const = 0;
	
	//! Shorter way of saying in code to get the networking device. Identical
	//! to GetNetworkingDevice().
	MAKO_REALINLINE NetworkingDevice* ND() const
	{ return GetNetworkingDevice(); }

	//! Gets the audio device
	//! \return The audio device
	virtual AudioDevice* GetAudioDevice() const = 0;

	//! Shorter way of saying in code to get the audio device. Identical
	//! to GetAudioDevice().
	MAKO_REALINLINE AudioDevice* AD() const
	{ return GetAudioDevice(); }
	
	//! Gets the active 3d scene
	//! \return The active 3d scene
	virtual Scene3d* GetActive3dScene() const = 0;

	//! Gets the active 2d scene
	//! \return The active 2d scene
	virtual Scene2d* GetActive2dScene() const = 0;
	
	//! Gets the active 3d physics device
	//! \return The active 3d physics device
	virtual Physics3dDevice* GetPhysics3dDevice() const = 0;

	//! Shorter way of writing in code to get the physics 3d device. Identical
	//! to GetPhysics3dDevice().
	MAKO_REALINLINE Physics3dDevice* P3DD() const
	{ return GetPhysics3dDevice(); }
	
	//! Gets the rendered window
	//! \return The rendered window
	virtual RenderedWindow* GetRenderedWindow() const = 0;

	//! Shorter way of writing in code to get the RenderedWindow. Identical to 
	//! GetRenderedWindow().
	MAKO_REALINLINE RenderedWindow* RW() const
	{ return GetRenderedWindow(); }
	
	//! Gets the operating system services
	//! \return The operating system services
	virtual OSDevice* GetOSDevice() const = 0;

	//! Shorter way of writing in code to get the OS device. Identical to 
	//! GetOSDevice().
	MAKO_REALINLINE OSDevice* OS() const
	{ return GetOSDevice(); }

	//! Get the file system
	virtual FileSystem* GetFileSystem() const = 0;

	//! Shorter way of writing in code to get the file system. Identical to 
	//! GetFileSystem().
	MAKO_REALINLINE FileSystem* FS() const
	{ return GetFileSystem(); }
	
	//! Gets the mesh manipulator
	//! \return The mesh manipulator
	virtual MeshManipulator* GetMeshManipulator() const = 0;

	//! Shorter way of writing in code to get the MeshManipulator. Identical
	//! to GetMeshManipulator().
	MAKO_REALINLINE MeshManipulator* MM() const
	{ return GetMeshManipulator(); }
	
	//! Gets the console
	//! \return The console
	virtual Console* GetConsole() const = 0;

	//! Post an event. All applicable event receivers will receive this event
	//! in OnEvent().
	//! \param[in] e The event to be sent
	virtual void PostEvent(Event* e) = 0;
	
	//! Add a new event receiver. Returns the index of it.
	//! \param[in] er The receiver to be added
	//! \return The index of the reciever
	virtual void AddEventReceiver(EventReceiver* er) = 0;
	
	//! Removes the event receiver, stopping it from receiving events
	//! \param[in] er The event receiver to remove
	virtual void RemoveEventReceiver(EventReceiver* er) = 0;
	
	//! Set the new 3d scene
	//! \param[in] scene The new active 3d scene
	virtual void SetScene(Scene3d* scene) = 0;

	//! Get the FPS (frame rate)
	//! \return The fps
	virtual UInt32 GetFPS() const = 0;

	//! Users of Application must use this method instead of the constructor
	//! in order to intialize user-created stuff.
	MAKO_API MAKO_INLINE virtual void Initialize() {}

	//! Users of SimpleApplication must use this method instead of the deconstructor
	//! in order to deallocate anything that was allocated in Initialize()
	MAKO_API MAKO_INLINE virtual void Shutdown() {}

	//! Starts the Application. This method should not be called inside
	//! the Application. It is called inside the macro MAKO_START_APPLICATION.
	virtual void Run() = 0;

	//! Quits the application
	virtual void Quit() = 0;

	//! Gets whether the application has quitted yet.
	//! \return True if the application has quitted, false if not
	virtual bool HasQuitted() const = 0;

	//! Get the command line arguements
	//! \return The command line arguements
	virtual const ArrayList<String>& GetCmdLnArgs() const = 0;

	//! Get the file path to the program location
	//! \return The file path
	MAKO_INLINE const String& GetProgramLocation() const
	{ return (GetCmdLnArgs())[0]; }

	//! Set the command line arguments. This should not be called by the user.
	//! It is called inside MAKO_RUN_APPLICATION().
	//! \param[in] argc The number of arguments
	//! \param[out] argv The arguments
	virtual void SetCommandLineArguments(Int32 argc, Int8** argv) = 0;
};

extern MAKO_API Application* appinst;

MAKO_REALINLINE Application* APP()
{ return appinst; }

MAKO_END_NAMESPACE