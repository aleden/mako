//#include "MakoCommon.h"
//#ifdef MAKO_PLATFORM_WIN32
//#include "MakoOS.h"
//
//
//
//#if defined(DEBUG) || defined(_DEBUG)
////#include <crtdbg.h>
//#include <vld.h>
//#endif
//
//BOOL APENTRY DllMain(HANDLE hModule,
//					  DWORD  reason,
//					  LPVOD lpReserved)
//{
//	switch (reason)
//	{
//	case DLL_PROCESS_ATTACH:
////#if defined(DEBUG) || defined(_DEBUG)
////		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
////#endif
////		break;
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}
//
//#endif

/*! \mainpage Mako Game Engine
 *
 * \image html C:/Dev/Mako/Media/makologo_shorter_fullsize.png
 *
 * \section intro_sec About
 *
 * The Mako Game Engine is a library for creating 3d and 2d games
 * in a single easy to use package.
 *
 * anthonyeden0@gmail.com
 *
 * \section over_sec Overview
 * 
 * The core classes of the engine are:
 *
 * \li Mako::Application
 * \li Mako::AudioDevice
 * \li Mako::GraphicsDevice
 * \li Mako::Physics3dDevice
 * \li Mako::Scene3d
 * \li Mako::Scene2d
 *
 * If you're just starting out with the engine, it's recommended that
 * you look at the tutorials included with the engine.
 *
 */

/*
 * The key class in the Mako Game Engine is Mako::Application. When you use the engine,
 * you must make a class (it could be called MyApplication) that inherits
 * from some implementation of Mako::Application. Once you've done that, you need
 * to call the macro MAKO_RUN_APPLICATION(), just with the name of your engine.
 * RUN_MAKO_APPLICATION() will insert the main() function inside the source file,
 * initialize your main class inside try-catch blocks, and call Run() on your
 * application inside more try-catch blocks. Treat Application::Initialize() as
 * the entry point to your application. Inside Intialize(), once you know how you
 * want your application to be made (window size, fullscreen?, vsync, anti-aliasing?, etc.),
 * you call InitializeMako() with an instance of MakoCreationParameters describing the
 * characteristics of your game.
 */

//! Terminology:
//! FileName: Full file path or relative file name
//! FilePath: Full file path or relative file name