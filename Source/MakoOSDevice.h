#pragma once
#include "MakoCommon.h"
#include "MakoVec2d.h"
#include "MakoVec2d.h"
#include "MakoReferenceCounted.h"
#include "MakoString.h"
#include "MakoDevice.h"


MAKO_BEGIN_NAMESPACE

// The RenderedWindow has these duties:
// 1. To initialize a native window depending on the OS
// 2. To send events to the event receiver.
class RenderedWindow
{
public:
	virtual const Size2d& GetWindowSize() const = 0;
	virtual bool IsFullscreen() const = 0;
	virtual const String& GetTitle() const = 0;
	virtual void SetTitle(String& title) = 0;
	virtual void Update() = 0;
	virtual void Close() = 0;

	virtual ~RenderedWindow() {}
};

//! This abstract class OSDevice provides functionalities
//! that are specific to the operating system (use the os'
//! libraries).
class OSDevice : public Device
{
public:
	//! Get the mouse position at any given time
	//! \return The mouse position
	virtual Position2d GetMousePosition() const = 0;
	
	//! Set the mouse position at any given time
	//! \param[in] pos The new mouse position
	virtual void SetMousePosition(const Position2d& pos) = 0;
	
	//! Create the main rendered window. This is not to be called by
	//! users of the Mako Game Engine. It is called internally inside
	//! SimpleApplication::InitializeMako().
	//! \param[in] title The window's name
	//! \param[in] dim The size (resolution) of the window
	//! \param[in] fullscreen Create the window in fullscreen mode?
	//! \param[in] vsync Is vertical sync enabled?
	//! \return The rendered window
	virtual RenderedWindow* CreateRenderedWindow(const String& title,
		const Size2d& dim, bool fullscreen) = 0;

	//! Updates the operating system services. This is not to be
	//! called by users of the Mako Game Engine, and instead is called
	//! inside the game loop of Mako::Application.
	virtual void Update() = 0;
	
	//! Get the change in mouse since the last frame.
	//! \return The change in mouse since the last frame.
	virtual const Vec2di& GetChangeInMouse() const = 0;
	
	//! Get the amount of milli seconds passed since the last frame.
	//! This function is important for most of the operations you
	//! will perform, because it is necessary to include in your
	//! calculations to produce frame-rate independent code.
	//! \return The mount of milli seconds passed since the 
	//! last frame
	virtual UInt32 GetChangeInTime() const = 0;

	//! Reset the elapsed time within OSDevice
	virtual void ResetElapsedTime() = 0;
	
	//! Get the number of milli-seconds since the program was
	//! started. The accuracy of this numbers varies on the impl
	//! \return The elapsed time since the game started.
	virtual UInt32 GetElapsedTime() const = 0;

	//! This does two things. It hides the mouse, and it sets it's
	//! position at the end of each frame to the center of the window.
	//! The value returned by GetChangeInMouse() is the same if
	//! the mouse wasn't locked. This is desireable to enable
	//! in most applications you will make with the Mako Game Engine,
	//! but the mouse shouldn't be locked when the player is in a
	//! menu state.
	//! \param[in] b Should the mouse be locked or not
	virtual void SetMouseLocked(bool b) = 0;

	//! Find out whether a file exists
	//! \param[in] filePath The file path of the file you want to
	//! check
	//! \return True if the file exists, else false
	virtual bool DoesFileExist(const String& filePath) const = 0;

	//! Displays a modal dialog box that contains a "ok" button that closes the window.
	//! This function is a blocking function, and gives control back to the thread when
	//! the user has closed the message box.
	//! \param[in] text The contents of the message box
	//! \param[in] title The title of the message box
	virtual void ShowMessageBox(const String& title, const String& text) const = 0;
	
	//! Empty virtual deconstructor
	virtual ~OSDevice() {}
};

MAKO_END_NAMESPACE
