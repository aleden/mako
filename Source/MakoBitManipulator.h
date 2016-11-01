#pragma once
#include "MakoCommon.h"
#include "Makomath.h"

MAKO_BEGIN_NAMESPACE

class BitManipulator
{
public:
	//! Check if bit is set.
	//! \param[in] value The value to check if the bit is
	//! at position bit
	//! \param[in] bit The bit index to check
	//! \return True if the bit is 1 and false if the bit is 0
	template <typename T>
	static MAKO_INLINE bool IsBitSet(const T& value, UInt8 bit)
	{ return (value & Pow(2, static_cast<UInt32>(bit))) != 0; }

	//! Flip a bit in a number
	//! \param[out] value The value who's bit will be
	//! flipped.
	//! \param[in] bit The bit index specifying which bit
	//! is to be flipped.
	template <typename T>
	static MAKO_INLINE void FlipBit(T& value, UInt8 bit)
	{ value = value ^ 1 << bit; }
};

MAKO_END_NAMESPACE
