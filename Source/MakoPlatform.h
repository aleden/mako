#pragma once

#define MAKO_PLATFORM_WIN32 1
#define MAKO_PLATFORM_LINUX 2
#define MAKO_PLATFORM_APPLE 3

// Get the platform to compile for
#if defined (__WIN32__) || defined (_WIN32) || defined (WIN32)
	#define MAKO_PLATFORM MAKO_PLATFORM_WIN32
#elif defined (__APPLE_CC__)
	#define MAKO_PLATFORM MAKO_PLATFORM_APPLE
#else
	#define MAKO_PLATFORM MAKO_PLATFORM_LINUX
#endif

#define MAKO_LITTLE_ENDIAN 1
#define MAKO_BIG_ENDIAN 2

#if defined (__hppa__) || defined (__m68k__) || defined (mc68000) || defined (_M_M68K) || (defined (__MIPS__) && defined (__MISPEB__)) || defined (__ppc__) || defined (__POWERPC__) || defined (_M_PPC) || defined (__sparc__)
	#define MAKO_ENDIAN MAKO_BIG_ENDIAN
#else
	#define MAKO_ENDIAN MAKO_LITTLE_ENDIAN
#endif