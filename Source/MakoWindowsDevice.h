#pragma once
#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoOS.h"
#include "MakoOSDevice.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

class Application;
class D3D9Device;

class WindowsRenderedWindow : public RenderedWindow
{
private:
	Vec2dui dim;
	bool fullscreen, vsync;
	String title;
	HWND hWnd;
	HINSTANCE hInstance;
	const String className;
public:
	WindowsRenderedWindow
		(
			const String& title,
			const Size2d& dim = Vec2dui(800,600),
			bool fullscreen = false
		);
	~WindowsRenderedWindow();

	const Size2d& GetWindowSize() const;
	bool IsFullscreen() const;
	const String& GetTitle() const;
	void SetTitle(String& title);
	void Update();
	void Close();

	HWND GetHWnd() const;
private:
	void InitializeWindow();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class WindowsDevice : public OSDevice
{
	friend class WindowsRenderedWindow;
	friend class D3D9Device;
private:
	LARGE_INTEGER highPerformanceFreq;
	bool firstFrame, mouseLocked, multiCore, highPerformanceTimerSupport;
	Vec2di newMouse, oldMouse, changeMouse, lockedMousePos;
	UInt32 newTime, oldTime, changeTime, startTime; // In milliseconds
	String versionName;
public:
	WindowsDevice();
	~WindowsDevice();

	Position2d GetMousePosition() const;
	void SetMousePosition(const Position2d& pos);
	RenderedWindow* CreateRenderedWindow(const String& title,
		const Size2d& dim,
		bool fullscreen);
	void Update();
	const Vec2di& GetChangeInMouse() const;

	void ResetElapsedTime();
	UInt32 GetElapsedTime() const;
	UInt32 GetChangeInTime() const;
	void SetMouseLocked(bool b);

	HWND GetHWnd(WindowsRenderedWindow* wnd) const;

	bool DoesFileExist(const String& filePath) const;

	void ShowMessageBox(const String& title, const String& text) const;

	String GetName() const
	{ return Text("Windows"); }
private:
	// Get the time in milliseconds of the system.
	UInt32 GetTime() const;
};

MAKO_END_NAMESPACE
#endif