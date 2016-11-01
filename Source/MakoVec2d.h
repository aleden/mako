#pragma once
#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

// #pragma pack(push,1) and #pragma pack(pop) force your compiler 
// to pack the structure as byte-aligned. 
#pragma pack (push, 1)

//! Class for representing a 2d vector. It is guaranteed that
//! this class will have only the fields x, and y and in
//! that order, and it will be byte-aligned.
template <typename T>
class Vec2d
{
public:
	/////////////////////////////////////////////////////
	// Fields

	//! The x value
	T x;
	//! The y value
	T y;

	
	////////////////////////////////////////////////////
	// Constructor(s)/Deconstructor

	//! Empty constructors, sets fields to zero
	MAKO_INLINE Vec2d()
	{ x = y = static_cast<T>(0); }
	
	//! Constructor taking in n
	//! \param[in] n The value x and y will be set to
	MAKO_INLINE Vec2d(const T& n)
	{ x = y = n; }

	//! Constructor taking in x and y
	//! \param[in] x The initial x value
	//! \param[in] y The initial y value
	MAKO_INLINE Vec2d(const T& x, const T& y)
	{ this->x = x; this->y = y; }
	
	//! Constructor taking in a vector
	//! \param[in] v The vector to be set to
	MAKO_INLINE Vec2d(const Vec2d<T>& v)
	{ (*this) = v; }
	
	//! Empty deconstructor
	~Vec2d() {}

	/////////////////////////////////////////////////////
	// Misc functions

	////! Converts the vector to a string
	////! \return The converted vector
	//MAKO_INLINE String Vec2d::ToString() const
	//{ return (String(L"(") + String(x) + L"," + String(y) + L")"); }

	/////////////////////////////////////////////////////
	// Vector math functions which only require one
	// vector

	//! Computes the length of the vector
	//! \return The length of the vector
	MAKO_INLINE T Length() const
	{ return sqrt((x*x) + (y*y)); }

	//! Normalizes this vector
	void Normalize()
	{
		T len = Length();
		if (len == static_cast<T>(0))
			return ;
		x /= len;
		y /= len;
	}

	//! Makes a normalized copy of the vector
	//! \return The normalized version of the vector
	Vec2d<T> Normalized() const
	{
		T len = Length();
		return Length() == 0 ? Vec2d<T>(0) : Vec2d<T>(x/len, y/len);
	}

	/////////////////////////////////////////////////////
	// Comparison/Assignment overloads

	//! Sets the second vector equal to this vector
	//! \param[in] v Sets the vector equal to v
	//! \return this vector
	MAKO_INLINE Vec2d<T>& operator = (const Vec2d& v)
	{ x = v.x; y = v.y; return *this; }

	//! Sets the number to all the fields of this vector
	//! \param[in] n This is set to all the fields
	//! \return this vector
	MAKO_INLINE Vec2d<T>& operator = (const T& n)
	{ x = n; y = n; return *this; }

	//! Tests if the number is equal to all the fields 
	//! of this vector
	//! \param[in] rhs The number
	//! \return True if equal, false if not
	MAKO_INLINE bool operator == (const T& rhs) const
	{ return (x == rhs && y == rhs); }

	//! Tests if the other vector is equal to this vector
	//! \param[in] rhs The other vector
	//! \return True if equal, false if not
	MAKO_INLINE bool operator == (const Vec2d<T>& rhs) const
	{ return (x == rhs.x && y == rhs.y); }

	//! Tests if the number is not equal to any of the fields 
	//! of this vector
	//! \param[in] rhs The number
	//! \return True if equal, false if not
	MAKO_INLINE bool operator != (const T& rhs) const
	{ return (x != rhs || y != rhs); }

	//! Tests if the other vector is not equal to this vector
	//! \param[in] rhs The other vector
	//! \return True if equal, false if not
	MAKO_INLINE bool operator != (const Vec2d<T>& rhs) const
	{ return (x != rhs.x || y != rhs.y); }

	/////////////////////////////////////////////////////
	// Minus overloads

	//! Subtracts the other vector from this vector into 
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The subtracted vector
	MAKO_INLINE Vec2d<T> operator - (const Vec2d<T>& rhs) const
	{ return Vec2d<T>(x - rhs.x, y - rhs.y); }

	//! Subtracts the number from all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The subtracted vector
	MAKO_INLINE Vec2d<T> operator - (const T& rhs) const
	{ return Vec2d<T>(x - rhs.x, y - rhs.y); }

	//! Subtracts the other vector from this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator -= (const Vec2d<T>& rhs)
	{ x -= rhs.x; y -= rhs.y; return *this; }

	//! Subtracts the number from all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator -= (const T& rhs)
	{ x -= rhs; y -= rhs; return *this; }

	/////////////////////////////////////////////////////
	// Plus overloads

	//! Adds the other vector to this vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The added vector
	MAKO_INLINE Vec2d<T> operator + (const Vec2d<T>& rhs) const
	{ return Vec2d<T>(x + rhs.x, y + rhs.y); }

	//! Adds the number to all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The added vector
	MAKO_INLINE Vec2d<T> operator + (const T& rhs) const
	{ return Vec2d<T>(x + rhs, y + rhs); }

	//! Adds the other vector to this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator += (const Vec2d<T>& rhs)
	{ x += rhs.x; y += rhs.y; return *this; }

	//! Adds the number to all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator += (const T& rhs)
	{ x += rhs; y += rhs; return *this; }

	/////////////////////////////////////////////////////
	// Multiply overloads

	//! Multiplies the other vector and this vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The multiplied vector
	MAKO_INLINE Vec2d<T> operator * (const Vec2d<T>& rhs) const
	{ return Vec2d<T>(x * rhs.x, y * rhs.y); }

	//! Mutiplies the number to all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The multiplied vector
	MAKO_INLINE Vec2d<T> operator * (const T& rhs) const
	{ return Vec2d<T>(x * rhs, y * rhs); }

	//! Mutiplies the other vector to this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator *= (const Vec2d<T>& rhs)
	{ x *= rhs.x; y *= rhs.y; return *this; }

	//! Mutiplies the number to all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator *= (const T& rhs)
	{ x *= rhs; y *= rhs; return *this; }

	/////////////////////////////////////////////////////
	// Divide overloads

	//! Divides this vector by the other vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The divided vector
	MAKO_INLINE Vec2d<T> operator / (const Vec2d<T>& rhs) const
	{ return Vec2d<T>(x / rhs.x, y / rhs.y); }

	//! Divides all this vector's fields by the number
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The divided vector
	MAKO_INLINE Vec2d<T> operator / (const T& rhs) const
	{ return Vec2d<T>(x / rhs, y / rhs); }

	//! Divides this vector by the other vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator /= (const Vec2d<T>& rhs)
	{ x /= rhs.x; y /= rhs.y; return *this; }

	//! Divides all the fields of this vector by the number
	//! \param[in] rhs Other number
	//! \return This vector
	MAKO_INLINE Vec2d<T>& operator /= (const T& rhs)
	{ x /= rhs; y /= rhs; return *this; }
};

#pragma pack (pop)

typedef Vec2d<Float32> Vec2df;
typedef Vec2d<Int> Vec2di;
typedef Vec2d<UInt> Vec2dui;

typedef Vec2di     Position2d;
typedef Position2d Pos2d;
typedef Vec2df     Scale2d;
typedef Vec2dui    Size2d;
typedef Vec2df TCoord;

MAKO_END_NAMESPACE
