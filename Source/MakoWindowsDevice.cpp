#include "MakoCommon.h"
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
#include "MakoOS.h"
#include "MakoWindowsDevice.h"
#include "Makoevents.h"
#include "MakoApplication.h"
#include "MakoException.h"
#include "MakoBitManipulator.h"
#include "MakoConsole.h"

MAKO_BEGIN_NAMESPACE

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Advapi32.lib")

WindowsDevice::WindowsDevice()
: firstFrame(true), mouseLocked(false), newMouse(0), oldMouse(0), changeMouse(0),
  newTime(0), oldTime(0), changeTime(0), lockedMousePos(500, 500)
{
	// disable hires timer on multiple core systems, bios bugs result in bad hires timers.
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	multiCore = (sysinfo.dwNumberOfProcessors > 1);	

	highPerformanceTimerSupport = QueryPerformanceFrequency(&highPerformanceFreq) != FALSE;

	startTime = GetTime();
}

WindowsDevice::~WindowsDevice()
{}

Vec2di WindowsDevice::GetMousePosition() const
{
	POINT p; GetCursorPos(&p);
	return Vec2di(static_cast<Int16>(p.x), static_cast<Int16>(p.y));
}

void WindowsDevice::SetMousePosition(const Position2d& pos)
{ SetCursorPos(pos.x, pos.y); }

void WindowsDevice::SetMouseLocked(bool b)
{ mouseLocked = b; ShowCursor(!b); }

HWND WindowsDevice::GetHWnd(WindowsRenderedWindow* wnd) const
{ return wnd->GetHWnd(); }

bool WindowsDevice::DoesFileExist(const String& filePath) const
{ return GetFileAttributesW(filePath.ToWStringData()) != INVALID_FILE_ATTRIBUTES; }

void WindowsDevice::Update()
{
	if (firstFrame)
	{
		oldTime  = newTime  = GetTime();
		oldMouse = newMouse = GetMousePosition();
		firstFrame = false;
	}
	else
	{
		newTime  = GetTime();
		newMouse = GetMousePosition();
	}

	if (mouseLocked)
	{
		changeMouse = (newMouse - oldMouse);
		SetMousePosition(lockedMousePos);
		oldMouse = newMouse + (lockedMousePos - newMouse);
	}
	else
	{
		changeMouse = newMouse - oldMouse;
		oldMouse = newMouse;
	}
	changeTime = newTime - oldTime;
	oldTime = newTime;
}
const Vec2di& WindowsDevice::GetChangeInMouse() const
{ return changeMouse; }

void WindowsDevice::ResetElapsedTime()
{ startTime = GetTime(); }

UInt32 WindowsDevice::GetElapsedTime() const
{ return GetTime() - startTime; }

UInt32 WindowsDevice::GetChangeInTime() const
{ return changeTime; }

void WindowsDevice::ShowMessageBox(const String& title, const String& text) const
{ ::MessageBoxW(nullptr, text.ToWStringData(), title.ToWStringData(), 0); }

UInt32 WindowsDevice::GetTime() const
{
	if (highPerformanceTimerSupport)
	{
		// Avoid potential timing inaccuracies across multiple cores by 
		// temporarily setting the affinity of this process to one core.
		DWORD_PTR affinityMask;
		if (multiCore)
			affinityMask = SetThreadAffinityMask(GetCurrentThread(), 1); 

		LARGE_INTEGER nTime;
		BOOL queriedOK = QueryPerformanceCounter(&nTime);

		// Restore the true affinity.
		if (multiCore)
			SetThreadAffinityMask(GetCurrentThread(), affinityMask);

		if (queriedOK)
			return UInt32(nTime.QuadPart * 1000 / highPerformanceFreq.QuadPart);
		else
			APP()->GetConsole()->Log(LL_LOW, Text("QueryPerformanceCounter() failed in WindowsDevice::GetTime()"));
	}
	return timeGetTime();
}

RenderedWindow* WindowsDevice::CreateRenderedWindow(const String& title,
													const Size2d& dim, 
													bool fullscreen)
{ return new WindowsRenderedWindow(title, dim, fullscreen); }

// WindowsRenderedWindow
WindowsRenderedWindow::WindowsRenderedWindow(const String& title,
											 const Size2d& dim,
											 bool fullscreen)
											 : className(Text("MAKOWNDOW")),
											 title(title), dim(dim),
											 fullscreen(fullscreen)
{
	hInstance = (HINSTANCE)GetModuleHandle(nullptr);

	InitializeWindow();
}
WindowsRenderedWindow::~WindowsRenderedWindow()
{}

const Size2d& WindowsRenderedWindow::GetWindowSize() const
{ return dim; }

bool WindowsRenderedWindow::IsFullscreen() const
{ return fullscreen;}

const String& WindowsRenderedWindow::GetTitle() const
{ return title; }

void WindowsRenderedWindow::SetTitle(String& title)
{ this->title = title; SetWindowTextW(hWnd, title.ToWStringData()); }

HWND WindowsRenderedWindow::GetHWnd() const
{ return hWnd; }

void WindowsRenderedWindow::InitializeWindow()
{
	WNDCLASSEX wc;

	// Clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// Fill in the struct with the needed information
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	if (!fullscreen)
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = className.ToWStringData();

	// If there is an icon, load it
	wc.hIcon = (HICON)LoadImage(hInstance, L"mako.ico", IMAGE_ICON, 0,0, LR_LOADFROMFILE); 

	// Register the main window class
	RegisterClassEx(&wc);

	// calculate client size
	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = dim.x;
	clientSize.bottom = dim.y;
	
	DWORD style = WS_POPUP;
	
	if (!fullscreen)
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	
	AdjustWindowRect(&clientSize, style, FALSE);
	
	const Int32 realWidth = clientSize.right - clientSize.left;
	const Int32 realHeight = clientSize.bottom - clientSize.top;

	Int32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	Int32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

	if (fullscreen)
	{
		windowLeft = 0;
		windowTop = 0;
	}

	// Create window
	hWnd = CreateWindowW
		(
			className.ToWStringData(),
			title.ToWStringData(),
			style,
			windowLeft,
			windowTop,
			realWidth,
			realHeight,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

	if (!hWnd)
		throw Exception(Text("Error in creating window, HWND invalid."));

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// fix ugly ATI driver bugs. Thanks to ariaci
	MoveWindow(hWnd, windowLeft, windowTop, realWidth, realHeight, TRUE);
}

LRESULT CALLBACK WindowsRenderedWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsDevice* wd;

	switch(msg)
	{
	case WM_CREATE:
		{
			wd = static_cast<WindowsDevice*>(APP()->GetOSDevice());
			return 0;
		}
	case WM_KEYDOWN:
		{

			// Bit 30
			// Specifies the previous key state. The value is 1 if the key is 
			// down before the message is sent, or it is zero if the key is up.
			if (BitManipulator::IsBitSet(lParam, 30))
				return 0; // Key repeat

			APP()->PostEvent(&KeyEvent(static_cast<KEY>(wParam), true));
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			APP()->PostEvent(&MouseMoveEvent(wd->GetChangeInMouse()));
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			APP()->PostEvent(&MouseWheelEvent(GET_WHEEL_DELTA_WPARAM(wParam)));
			return 0;
		}
	case WM_KEYUP:
		{
			APP()->PostEvent(&KeyEvent(static_cast<KEY>(wParam), false));
			return 0;
		}
	case WM_RBUTTONDOWN:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_RIGHT, true));
			return 0;
		}
	case WM_RBUTTONUP:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_RIGHT, false));
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_LEFT, true));
			return 0;
		}
	case WM_LBUTTONUP:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_LEFT, false));
			return 0;
		}
	case WM_MBUTTONDOWN:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_MIDDLE, true));
			return 0;
		}
	case WM_MBUTTONUP:
		{
			APP()->PostEvent(&MouseButtonEvent(MBTN_MIDDLE, false));
			return 0;
		}
	case WM_DESTROY:
		{
			if (APP())
				APP()->Quit();
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void WindowsRenderedWindow::Update()
{
	// This struct holds Windows event messages
	MSG msg;

	// f a message is available, the return value of PeekMessage() is nonzero.
	// There may be more than one message in the queue, so a loop is required or
	// else there is a delay for keyboard input.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
	{
		if (msg.message == WM_QUIT)
		{
			return ;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowsRenderedWindow::Close()
{ PostQuitMessage(0); }


MAKO_END_NAMESPACE
#endif // MAKO_PLATFORM == MAKO_PLATFORM_WIN32