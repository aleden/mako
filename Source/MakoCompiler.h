#pragma once

#define MAKO_COMPILER_MSVC 1
#define MAKO_COMPILER_GNUC 2
#define MAKO_COMPILER_BORL 3

// Get the compiler type
#if defined (_MSC_VER)
	#define MAKO_COMPILER MAKO_COMPILER_MSVC
#elif defined (__GNUC__)
	#define MAKO_COMPILER MAKO_COMPILER_GNUC
#elif defined (__BORLANDC__)
	#define MAKO_COMPILER MAKO_COMPILER_BORL
#else
	#pragma error "Unknown compiler."
#endif