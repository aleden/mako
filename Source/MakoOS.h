#pragma once
#include "MakoCommon.h"

#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	//#ifndef WIN32_LEAN_AND_MEAN
	//	#define WIN32_LEAN_AND_MEAN
	//#endif
	#include <windows.h>
#endif