#pragma once
#include "MakoCompileConfig.h"

#if defined (_DEBUG) || defined (__DEBUG) || defined (DEBUG)
	#define MAKO_DEBUG_MODE
#endif

/////////////////////////////////////////////////
// Defines
#define MAKO_D3D_VER_9 9
#define MAKO_D3D_VER_10 10
#define MAKO_D3D_VER_11 11

// D3D, XAUDIO
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	#define MAKO_D3D_AVAILABLE
	// This is the highest Direct3d version available on the system.
	#define MAKO_D3D_VER MAKO_D3D_VER_9
	#define MAKO_XAUDIO2_AVAILABLE
	#ifdef MAKO_DEBUG_MODE
		#define MAKO_DEBUG_D3D
		#define D3D_DEBUG_INFO
	#endif
#endif

// PHYSX
#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32 || MAKO_PLATFORM == MAKO_PLATFORM_LINUX
	#define MAKO_PHYSX_AVAILABLE
	#ifndef WIN32
		#define WIN32
	#endif
#endif

#define MAKO_OPENGL_AVAILABLE

#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	#define _CRT_SECURE_NO_WARNINGS
#endif

/////////////////////////////////////////////////
// Other defines

#define MAKO_BEGIN_NAMESPACE namespace Mako {
#define MAKO_END_NAMESPACE }

#define MAKO_INLINE inline
#ifdef _MSC_VER
	#define MAKO_REALINLINE __forceinline
#else
	#define MAKO_REALINLINE inline
#endif

#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
	#ifdef MAKO_COMPILING
		#define MAKO_API __declspec(dllexport)
	#else
		#define MAKO_API __declspec(dllimport)
	#endif
#else
	#define MAKO_API
#endif

#ifdef MAKO_DEBUG_MODE
	#define MAKO_DEBUG_BREAK do { _asm int 3 } while (false)
#else
	#define MAKO_DEBUG_BREAK 
#endif

#if !defined (MAKO_CPP_0X) && !defined (nullptr)
	#define nullptr 0
#endif

MAKO_BEGIN_NAMESPACE

/////////////////////////////////////////////////
// Typedefs

#if MAKO_COMPILER == MAKO_COMPILER_MSVC
	//! Guaranteed to be 8 bit signed int
	typedef __int8   Int8;
	
	//! Guaranteed to be 16 bit signed int
	typedef __int16  Int16;
	
	//! Guaranteed to be 32 bit signed int
	typedef __int32  Int32;
	
	//! Guaranteed to be 8 bit unsigned int
	typedef unsigned __int8  UInt8;

	//! Guaranteed to be 16 bit unsigned int
	typedef unsigned __int16 UInt16;
	
	//! Guaranteed to be 32 bit unsigned int
	typedef unsigned __int32 UInt32;
#else
	//! Guaranteed to be 8 bit signed int
	typedef char   Int8;
	
	//! Guaranteed to be 16 bit signed int
	typedef short  Int16;
	
	//! Guaranteed to be 32 bit signed int
	typedef int  Int32;
	
	//! Guaranteed to be 8 bit unsigned int
	typedef unsigned char  UInt8;

	//! Guaranteed to be 16 bit unsigned int
	typedef unsigned short UInt16;
	
	//! Guaranteed to be 32 bit unsigned int
	typedef unsigned int UInt32;
#endif

typedef Int8 Byte;
typedef UInt8 UByte;

typedef unsigned int UInt;
typedef int Int;

//! Guaranteed to be 32 bit float
typedef float Float32;

//! Guaranteed to be 64 bit float
typedef double Float64;

typedef Float32 Rotation2d;
typedef Rotation2d Rot2d;

MAKO_END_NAMESPACE