#pragma once
#include "MakoCommon.h"
#include <math.h>

MAKO_BEGIN_NAMESPACE

// #pragma pack(push,1) and #pragma pack(pop) force your compiler 
// to pack the structure as byte-aligned. 
#pragma pack (push, 1)

//! Class for representing a 3d vector. It is guaranteed that
//! this class will have only the fields x, y, and z, and in
//! that order, and it will be byte aligned.
template <typename T>
class Vec3d
{
public:
	/////////////////////////////////////////////////////
	// Fields
	
	//! The x value
	T x;
	//! The y value
	T y;
	//! The z value
	T z;

	/////////////////////////////////////////////////////
	// Constructor(s)/Deconstructor

	//! Empty constructors, sets fields to zero
	MAKO_INLINE Vec3d()
	{ x = y = z = static_cast<T>(0); }

	//! Constructor taking in n
	//! \param[in] n The values x,y,z will be set to
	MAKO_INLINE Vec3d(const T& n)
	{ x = y = z = n; }

	//! Constructor taking in x,y,z
	//! \param[in] x The initial x value
	//! \param[in] y The initial y value
	//! \param[in] z The initial z value
	MAKO_INLINE Vec3d(const T& x, const T& y, const T& z)
	{ this->x = x; this->y = y; this->z = z; }

	//! Constructor taking in a vector
	//! \param[in] v The vector to be set to
	MAKO_INLINE Vec3d(const Vec3d<T>& v)
	{ (*this) = v; }
	
	//! Empty deconstructor
	MAKO_INLINE ~Vec3d() {}

	/////////////////////////////////////////////////////
	// Misc functions

	//! Converts the vector to a string
	//! \return The converted vector
	//MAKO_INLINE String ToString() const
	//{ return Text("(") + String(x) + Text(",") + y + Text(",") + z + Text(")"); }

	/////////////////////////////////////////////////////
	// Vector math functions which only require one vec

	//! Computes the length of the vector
	//! \return The length of the vector
	MAKO_INLINE T Length() const
	{ return sqrt((x*x) + (y*y) + (z*z)); }

	//! Normalizes this vector
	void Normalize()
	{
		T len = Length();
		if (len == 0)
			return ;
		x/=len;
		y/=len;
		z/=len;
	}

	//! Makes a normalized copy of the vector
	//! \return The normalized version of the vector
	Vec3d<T> Normalized() const
	{
		T len = Length();
		return Length() == 0 ? Vec3d<T>(0) : Vec3d<T>(x/len, y/len, z/len);
	}

	/////////////////////////////////////////////////////
	// Comparison/AssignTo overloads

	//! Sets the second vector equal to this vector
	//! \param[in] v Sets the vector equal to v
	//! \return this vector
	MAKO_INLINE Vec3d<T>& operator = (const Vec3d& v)
	{ x = v.x; y = v.y; z = v.z; return *this; }

	//! Sets the number to all the fields of this vector
	//! \param[in] n This is set to all the fields
	//! \return this vector
	MAKO_INLINE Vec3d<T>& operator = (const T& n)
	{ x = n; y = n; z = n; return *this; }

	//! Tests if the number is equal to all the fields 
	//! of this vector
	//! \param[in] n The number
	//! \return True if equal, false if not
	MAKO_INLINE bool operator == (const T& n) const
	{ return (x == n && y == n && z == n); }

	//! Tests if the other vector is equal to this vector
	//! \param[in] rhs The other vector
	//! \return True if equal, false if not
	MAKO_INLINE bool operator == (const Vec3d<T>& rhs)
	{ return (x == rhs.x && y == rhs.y && z == rhs.z); }

	//! Tests if the number is not equal to any of the fields 
	//! of this vector
	//! \param[in] n The number
	//! \return True if equal, false if not
	MAKO_INLINE bool operator != (const T& n) const
	{ return !(x == n && y == n && z == n); }

	//! Tests if the other vector is not equal to this vector
	//! \param[in] rhs The other vector
	//! \return True if equal, false if not
	MAKO_INLINE bool operator != (const Vec3d<T>& rhs) const
	{ return (x != rhs.x || y != rhs.y || z != rhs.z); }

	/////////////////////////////////////////////////////
	// Minus overloads

	//! Subtracts the other vector from this vector into 
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The subtracted vector
	MAKO_INLINE Vec3d<T> operator - (const Vec3d<T>& rhs) const
	{ return Vec3d<T>(x - rhs.x, y - rhs.y, z - rhs.z); }

	//! Subtracts the number from all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The subtracted vector
	MAKO_INLINE Vec3d<T> operator - (const T& rhs) const
	{ return Vec3d<T>(x - rhs, y - rhs, z - rhs); }

	//! Subtracts the other vector from this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator -= (const Vec3d<T>& rhs)
	{ x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

	//! Subtracts the number from all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator -= (const T& rhs)
	{ x -= rhs; y -= rhs; z -= rhs; return *this; }

	/////////////////////////////////////////////////////
	// Plus overloads

	//! Adds the other vector to this vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The added vector
	MAKO_INLINE Vec3d<T> operator + (const Vec3d<T>& rhs) const
	{ return Vec3d<T>(x + rhs.x, y + rhs.y, z + rhs.z); }

	//! Adds the number to all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The added vector
	MAKO_INLINE Vec3d<T> operator + (const T& rhs) const
	{ return Vec3d<T>(x + rhs, y + rhs, z + rhs); }

	//! Adds the other vector to this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator += (const Vec3d<T>& rhs)
	{ x += rhs.x; y += rhs.y; z += rhs.z; return *this; }

	//! Adds the number to all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator += (const T& rhs)
	{ x += rhs; y += rhs; z += rhs; return *this; }

	/////////////////////////////////////////////////////
	// Multiply overloads

	//! Multiplies the other vector and this vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The multiplied vector
	MAKO_INLINE Vec3d<T> operator * (const Vec3d<T>& rhs) const
	{ return Vec3d<T>(x * rhs.x, y * rhs.y, z * rhs.z); }

	//! Mutiplies the number to all the fields of this vector
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The multiplied vector
	MAKO_INLINE Vec3d<T> operator * (const T& rhs) const
	{ return Vec3d<T>(x * rhs, y * rhs, z * rhs); }

	//! Mutiplies the other vector to this vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator *= (const Vec3d<T>& rhs)
	{ x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }

	//! Mutiplies the number to all the fields of this vector
	//! \param[in] rhs The number
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator *= (const T& rhs)
	{ x *= rhs; y *= rhs; z *= rhs; return *this; }

	/////////////////////////////////////////////////////
	// Divide overloads

	//! Divides this vector by the other vector into
	//! a new vector
	//! \param[in] rhs Other vector
	//! \return The divided vector
	MAKO_INLINE Vec3d<T> operator / (const Vec3d<T>& rhs) const
	{ return Vec3d<T>(x / rhs.x, y / rhs.y, z / rhs.z); }

	//! Divides all this vector's fields by the number
	//! into a new vector
	//! \param[in] rhs The number
	//! \return The divided vector
	MAKO_INLINE Vec3d<T> operator / (const T& rhs) const
	{ return Vec3d<T>(x / rhs, y / rhs, z / rhs); }

	//! Divides this vector by the other vector
	//! \param[in] rhs Other vector
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator /= (const Vec3d<T>& rhs)
	{ x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

	//! Divides all the fields of this vector by the number
	//! \param[in] rhs Other number
	//! \return This vector
	MAKO_INLINE Vec3d<T>& operator /= (const T& rhs)
	{ x /= rhs; y /= rhs; z /= rhs; return *this; }
};

#pragma pack (pop)


typedef Vec3d<Float32> Vec3df;
typedef Vec3d<Int32> Vec3di;
typedef Vec3d<UInt32> Vec3dui;

typedef Vec3df     Position3d;
typedef Vec3df     Rotation3d;
typedef Vec3df     Scale3d;
typedef Vec3df     Size3d;

typedef Position3d Pos3d;
typedef Rotation3d Rot3d;

MAKO_END_NAMESPACE
