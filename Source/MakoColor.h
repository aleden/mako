#pragma once
#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

//! Class for representing a 32 bit color
class R8G8B8A8Color
{
private:
	UInt8 r, g, b, a;
public:
	//! Constructor taking in number
	//! \param[in] n All color attributes are set to n
	MAKO_INLINE R8G8B8A8Color(UInt8 n = 255)
		: r(n), g(n), b(n), a(n) {}

	//! Copy constructor
	MAKO_INLINE R8G8B8A8Color(const R8G8B8A8Color& c)
		: r(c.r), g(c.g), b(c.b), a(c.a) {}

	//! Constructor taking in all color attributes
	//! \param[in] r The red value
	//! \param[in] g The green value
	//! \param[in] b The blue value
	//! \param[in] a The alpha (opacity) value
	MAKO_INLINE R8G8B8A8Color(UInt8 r, UInt8 g, UInt8 b, UInt8 a)
		: r(r), g(g), b(b), a(a) {}
	
	//! Empty deconstructor
	MAKO_INLINE ~R8G8B8A8Color() {}

	//! Add a color to this color
	//! \param[in] c Other color for this color to be added to
	//! \return New color
	MAKO_INLINE R8G8B8A8Color operator + (const R8G8B8A8Color& c) const
	{ return R8G8B8A8Color(r + c.r, g + c.g, b + c.b, a + c.a); }

	
	//! Set this color equal to other color
	//! \param[in] c Other color for this color to be set to
	//! \return This color
	MAKO_INLINE R8G8B8A8Color& operator = (const R8G8B8A8Color& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
		return *this;
	}

	//! Set this color's attributes equal to a number
	//! \param[in] n The number that all the attributes of this
	//! color will be set to
	MAKO_INLINE R8G8B8A8Color& operator = (UInt8 n)
	{ r = g = b = a = n; }
	
	//! Set the red attribute of this color
	//! \param[in] r The amount of red
	MAKO_INLINE void SetR(UInt8 r)
	{ this->r = r; }

	//! Set the green attribute of this color
	//! \param[in] g The amount of green
	MAKO_INLINE void SetG(UInt8 g)
	{ this->g = g; }
	
	//! Set the blue attribute of this color
	//! \param[in] b The amount of blue
	MAKO_INLINE void SetB(UInt8 b)
	{ this->b = b; }
	
	//! Set the alpha (opacity) attribute of this color
	//! \param[in] a The amount of alpha (opacity)
	MAKO_INLINE void SetA(UInt8 a)
	{ this->a = a; }

	//! Get the red attribute of this color
	//! \return The red attribute
	MAKO_INLINE UInt8 GetR() const
	{ return r; }

	//! Get the green attribute of this color
	//! \return The green attribute
	MAKO_INLINE UInt8 GetG() const
	{ return g; }

	//! Get the blue attribute of this color
	//! \return The blue attribute
	MAKO_INLINE UInt8 GetB() const
	{ return b; }

	//! Get the alpha (opacity) attribute of this color
	//! \return The alpha (opacity) attribute
	MAKO_INLINE UInt8 GetA() const
	{ return a; }

	//! Get the color in A8R8G8B8 format
	//! \return A8R8G8B8 format
	MAKO_INLINE UInt32 GetA8R8G8B8Format() const
	{
#if MAKO_ENDIAN == MAKO_LITTLE_ENDIAN
		return (b << 24) | (g << 16) | (r << 8) | (a);
#else
		return (a << 24) | (r << 16) | (g << 8) | (b);
#endif
	}
	//! Get the color in B8G8R8A8 format
	//! \return B8G8R8A8 format
	MAKO_INLINE UInt32 GetB8G8R8A8Format() const
	{
#if MAKO_ENDIAN == MAKO_LITTLE_ENDIAN
		return (a << 24) | (r << 16) | (g << 8) | (b);
#else
		return (b << 24) | (g << 16) | (r << 8) | (a);
#endif
	}
	//! Get the color in R8G8B8A8 format
	//! \return R8G8B8A8 format
	MAKO_INLINE UInt32 GetR8G8B8A8Format() const
	{
#if MAKO_ENDIAN == MAKO_LITTLE_ENDIAN
		return (a << 24) | (b << 16) | (g << 8) | (r);
#else
		return (r << 24) | (g << 16) | (b << 8) | (a);
#endif
	}
};

//! The standard color format used by the Mako engine.
typedef R8G8B8A8Color Color;

MAKO_END_NAMESPACE