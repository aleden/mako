#pragma once
#include "MakoCommon.h"
#include "MakoVec3d.h"
#include "MakoMatrix4.h"
#include <float.h> // For FLT_MAX
#include <limits.h> // For INT_MAX / UINT_MAX
#include <cstdlib>

MAKO_BEGIN_NAMESPACE

#ifdef MAKO_FAST_MATH
	const Float32 ROUNDING_ERROR_32 = 0.00005f;
	const Float64 ROUNDING_ERROR_64 = 0.000005;
#else
	const Float32 ROUNDING_ERROR_32 = 0.000001f;
	const Float64 ROUNDING_ERROR_64 = 0.00000001;
#endif

#ifdef PI // make sure we don't collide with a define
#undef PI
#endif

//! Constant for PI.
const Float32 PI = 3.14159265359f;

//! Constant for reciprocal of PI.
const Float32 RECIPROCAL_PI	= 1.f/PI;

//! Constant for half of PI.
const Float32 HALF_PI = PI/2.f;

#ifdef PI64 // make sure we don't collide with a define
#undef PI64
#endif

//! Constant for 64bit PI.
const Float64 PI64 = 3.1415926535897932384626433832795028841971693993751;

//! Constant for 64bit reciprocal of PI.
const Float64 RECIPROCAL_PI64 = 1.0/PI64;

//! 32bit Constant for converting from degrees to radians
const Float32 DEGTORAD = PI / 180.0f;

//! 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
const Float32 RADTODEG = 180.0f / PI;

//! 64bit constant for converting from degrees to radians (formally known as GRAD_PI2)
const Float64 DEGTORAD64 = PI64 / 180.0;

//! 64bit constant for converting from radians to degrees
const Float64 RADTODEG64 = 180.0 / PI64;

template <typename T>
MAKO_INLINE T Sin(const T& n)
{ return sin(n); }

template <typename T>
MAKO_INLINE T Cos(const T& n)
{ return cos(n); }

template <typename T>
MAKO_INLINE T Tan(const T& n)
{ return tan(n); }

template <typename T>
MAKO_INLINE T ArcSin(const T& n)
{ return asin(n); }

template <typename T>
MAKO_INLINE T ArcCos(const T& n)
{ return acos(n); }

template <typename T>
MAKO_INLINE T ArcTan(const T& n)
{ return atan(n); }

//! Utility function to convert a radian value to degrees
//! Provided as it can be clearer to write RadToDeg(X) than RADTODEG * X
//! \param[in] radians The radians value to convert to degrees.
MAKO_INLINE Float32 RadToDeg(Float32 radians)
{ return RADTODEG * radians; }

//! Utility function to convert a radian value to degrees
//! Provided as it can be clearer to write RadToDeg(X) than RADTODEG * X
//! \param[in] radians	The radians value to convert to degrees.
MAKO_INLINE Float64 RadToDeg(Float64 radians)
{ return RADTODEG64 * radians; }

//! Utility function to convert a degrees value to radians
//! Provided as it can be clearer to write DegToRad(X) than DEGTORAD * X
//! \param[in] degrees	The degrees value to convert to radians.
MAKO_INLINE Float32 DegToRad(Float32 degrees)
{ return DEGTORAD * degrees; }

//! Utility function to convert a degrees value to radians
//! Provided as it can be clearer to write DegToRad(X) than DEGTORAD * X
//! \param[in] degrees	The degrees value to convert to radians.
MAKO_INLINE Float64 DegToRad(Float64 degrees)
{ return DEGTORAD64 * degrees; }

//! This function computes the absolute value of anything that has 
//! implemented the '<' and '-' (opposite, not minus) operator.
template <typename T>
MAKO_INLINE T Abs(const T& number)
{ return number < static_cast<T>(0) ? -number : number; }

//! This function computes an integral power of a number
//! \param[in] number The number
//! \param[in] power The integral power
//! \return The computed result
template <typename T>
MAKO_INLINE T Pow(const T& number, UInt32 power)
{
	T r = static_cast<T>(1);
	for (unsigned int i = 0; i < power; i++)
		r *= number;
	return r;
}

//! This function computes a floating point power of a number
//! \param[in] number The number
//! \param[in] power The floating point power
//! \return The computed result
template <typename T>
MAKO_INLINE T Pow(const T& number, Float32 power)
{ return static_cast<T>(powf(static_cast<Float32>(number), power)); }

//! Generates a random float
//! \param[in] a The start of the range
//! \param[in] b The end of the range
//! \return The random float between a and b
MAKO_INLINE Float32 RandFloat(Float32 a, Float32 b)
{ return static_cast<Float32>(a + (((Float32)rand())/((Float32)RAND_MAX)) * (b-a)); }

//! Generates a random unsigned integer
//! \param[in] a The start of the range
//! \param[in] b The end of the range
//! \return The random number between a and b
MAKO_INLINE UInt32 RandUInt(UInt32 a, UInt32 b)
{ return static_cast<UInt32>(a + (((Float32)rand())/((Float32)RAND_MAX)) * (b-a)); }

//! returns minimum of two values.
template <typename T>
MAKO_INLINE const T& Min(const T& a, const T& b)
{ return a < b ? a : b; }

//! Returns minimum of three values.
template <typename T>
MAKO_INLINE const T& Min(const T& a, const T& b, const T& c)
{ return a < b ? Min(a, c) : Min(b, c); }

//! Returns maximum of two values.
template <typename T>
MAKO_INLINE const T& Max(const T& a, const T& b)
{ return a < b ? b : a; }

//! Returns maximum of three values.
template <typename T>
MAKO_INLINE const T& Max(const T& a, const T& b, const T& c)
{ return a < b ? Max(b, c) : Max(a, c); }

//! clamps a value between low and high
template <typename T>
MAKO_INLINE const T Clamp(const T& value, const T& low, const T& high)
{ return Min(Max(value, low), high); }

//! This is for the fast atof function.
// We write [16] here instead of [] to work around a swig bug
const Float32 fast_atof_table[16] =
{
	0.f,
	0.1f,
	0.01f,
	0.001f,
	0.0001f,
	0.00001f,
	0.000001f,
	0.0000001f,
	0.00000001f,
	0.000000001f,
	0.0000000001f,
	0.00000000001f,
	0.000000000001f,
	0.0000000000001f,
	0.00000000000001f,
	0.000000000000001f
};

//! Convert a simple string of base 10 digits into a signed 32 bit integer.
//! \param[in] in The string of digits to convert. Only a leading - or + followed 
//!					by digits 0 to 9 will be considered.  Parsing stops at the
//!					first non-digit.
//! \param[out] out (optional) f provided, it will be set to point at the first
//!					 character not used in the calculation.
//! \return The signed integer value of the digits. f the string specifies too many
//!			digits to encode in an Int32 then +INT_MAX or -INT_MAX will be returned.
MAKO_INLINE Int32 strtol10(const char* in, const char** out=0)
{
	if(!in)
		return 0;

	bool negative = false;
	if('-' == *in)
	{
		negative = true;
		++in;
	}
	else if('+' == *in)
		++in;

	UInt32 unsignedValue = 0;

	while ( ( *in >= '0') && ( *in <= '9' ))
	{
		unsignedValue = ( unsignedValue * 10 ) + ( *in - '0' );
		++in;

		if(unsignedValue > (UInt32)INT_MAX)
		{
			unsignedValue = (UInt32)INT_MAX;
			break;
		}
	}
	if (out)
		*out = in;

	if(negative)
		return -((Int32)unsignedValue);
	else
		return (Int32)unsignedValue;
}

//! Converts a sequence of digits into a whole positive floating point value.
//! Only digits 0 to 9 are parsed.  Parsing stops at any other character, 
//! including sign characters or a decimal point.
//! \param[in] in: the sequence of digits to convert.
//! \param[out] out: (optional) will be set to point at the first non-converted character.
//! \return The whole positive floating point representation of the digit sequence.
MAKO_INLINE Float32 strtof10(const Int8* in, const Int8** out = 0)
{
	if(out)
		*out = in;

	if(!in)
		return 0.f;

	static const UInt32 MAX_SAFE_U32_VALUE = UINT_MAX / 10 - 10;
	Float32 floatValue = 0.f;
	UInt32 intValue = 0;

	// Use integer arithmetic for as long as possible, for speed
	// and precision.
	while ( ( *in >= '0') && ( *in <= '9' ) )
	{
		// f it looks like we're going to overflow, bail out
		// now and start using floating point.
		if(intValue >= MAX_SAFE_U32_VALUE)
			break;

		intValue = ( intValue * 10) + ( *in - '0' );
		++in;
	}

	floatValue = (Float32)intValue;

	// f there are any digits left to parse, then we need to use 
	// floating point arithmetic from here.
	while ( ( *in >= '0') && ( *in <= '9' ) )
	{
		floatValue = ( floatValue * 10.f ) + (Float32)( *in - '0' );
		++in;
		if(floatValue > FLT_MAX) // Just give up.
			break;
	}

	if(out)
		*out = in;

	return floatValue;
}

//! Provides a fast function for converting a string into a float.
//! This is not guaranteed to be as accurate as atof(), but is 
//! approximately 6 to 8 times as fast.
//! \param[in] in The string to convert. 
//! \param[out] out The resultant float will be written here.
//! \return A pointer to the first character in the string that wasn't
//!         use to create the float value.
MAKO_INLINE const Int8* fast_atof_move(const Int8* in, Float32 & out)
{
	// Please run this regression test when making any modifications to this function:
	// https://sourceforge.net/tracker/download.php?group_id=74339&atid=540676&file_id=298968&aid=1865300

	out = 0.f;
	if(!in)
		return 0;

	bool negative = false;
	if(*in == '-')
	{
		negative = true;
		++in;
	}

	Float32 value = strtof10 ( in, &in );

	if (*in == '.')
	{
		++in;

		const char * afterDecimal = in;
		Float32 decimal = strtof10 ( in, &afterDecimal );
		decimal *= fast_atof_table[afterDecimal - in];

		value += decimal;

		in = afterDecimal;
	}

	if ('e' == *in || 'E' == *in)
	{
		++in;
		// Assume that the exponent is a whole number.
		// strtol10() will deal with both + and - signs,
		// but cast to (Float32) to prevent overflow at FLT_MAX
		value *= (Float32)::pow(10.0f, (Float32)strtol10(in, &in));
	}

	if(negative)
		out = -value;
	else
		out = value;

	return in;
}

//! Convert a c-string (char*) to a floating point number. Provides a fast 
//! function for converting a string into a float. This is not guaranteed to 
//! be as accurate as atof(), but is approximately 6 to 8 times as fast.
//! \param[in] floatAsString: The string to convert.
//! \return The converted c-string
MAKO_INLINE Float32 FastCStringDataToS32(const Int8* floatAsString)
{
	float ret;
	fast_atof_move(floatAsString, ret);
	return ret;
}

MAKO_INLINE Int32 CStringDataToS32(const Int8* intAsString)
{ return atoi(intAsString); }

//! Finds the reciprocal of a number
//! \param[in] f The number
//! \return The reciprocal
MAKO_INLINE Float32 Reciprocal(Float32 f)
{ return 1.f / f; }

//! Finds the reciprocal of a number
//! \param[in] f The number
//! \return The reciprocal
MAKO_INLINE Float64 Reciprocal(Float64 f)
{ return 1.f / f; }

//! Returns the inverse square root
//! \param[in] x The floating point value to calculate the inv sqrt of
MAKO_INLINE Float32 InverseSquareRoot(Float32 x)
{
#ifdef MAKO_FAST_MATH
    Float32 xhalf = 0.5f*x;
    Int32 i = *(Int32*)&x;
    i = 0x5f3759df - (i>>1);
    x = *(Float32*)&i;
    return x*(1.5f - xhalf*x*x);
#else
	return 1.f / sqrt(x);
#endif
}

//! Checks if two 64-bit floats equal each other, taking possible rounding errors 
//! into account
//! \param[in] a The first value
//! \param[in] b The second value
//! \param[in] tolerance The type of tolerance
//! \return True if a equals b, taking possible rounding errors into account
MAKO_INLINE bool Equals(const Float64 a, const Float64 b, const Float64 tolerance = ROUNDING_ERROR_64)
{ return (a + tolerance >= b) && (a - tolerance <= b); }

//! Checks if two 32-bit floats equal each other, taking possible rounding errors 
//! into account
//! \param[in] a The first value
//! \param[in] b The second value
//! \param[in] tolerance The type of tolerance
//! \return True if a equals b, taking possible rounding errors into account
MAKO_INLINE bool Equals(const Float32 a, const Float32 b, const Float32 tolerance = ROUNDING_ERROR_32)
{ return (a + tolerance >= b) && (a - tolerance <= b); }

//! Checks if a value equals zero, taking rounding errors into account
//! \param[in] a The value
//! \param[in] tolerance The tolerance
//! \return True if a value equals zero, taking rounding errors into account
MAKO_INLINE bool IsZero(const Float32 a, const Float32 tolerance = ROUNDING_ERROR_32)
{ return fabsf(a) <= tolerance; }

template <typename T>
T Average(const T& a, const T& b)
{ return a+b/2; }

/////////////////////////////////////////////////////
// Vector math functions for functions which require
// 2 or more vectors

//! Given a position, rotation, and distance ahead this function calculates
//! an absolute position forward
//! \param[in] pos The position
//! \param[out] rot The rotation
//! \param[in] distance The distance
//! \return The computed position
MAKO_INLINE Vec3d<Float32> Forward(const Vec3d<Float32>& pos,
								   const Vec3d<Float32>& rot,
								   const Float32 distance)
{
	Float32 rady = rot.y * DEGTORAD, radx = rot.x * DEGTORAD;
	Float32 siny = Sin<Float32>(rady);
	Float32 cosy = Cos<Float32>(rady);

	Float32 sinx = Sin<Float32>(radx);
	Float32 cosx = Cos<Float32>(radx);

	return pos + Vec3d<Float32>(siny*cosx, -sinx, cosy*cosx)*distance;
}

//! Given a position, rotation, and distance ahead this function calculates
//! an absolute position forward
//! \param[in] pos The position
//! \param[out] rot The rotation
//! \param[in] distance The distance
//! \return The computed position
MAKO_INLINE Vec3d<Float64> Forward(const Vec3d<Float64>& pos,
								   const Vec3d<Float64>& rot,
								   const Float64& distance)
{
	Float64 rady = rot.y * DEGTORAD64, radx = rot.x * DEGTORAD64;

	Float64 siny = Sin<Float64>(rady);
	Float64 cosy = Cos<Float64>(rady);

	Float64 sinx = Sin<Float64>(radx);
	Float64 cosx = Cos<Float64>(radx);

	return pos + Vec3d<Float64>(siny*cosx, -sinx, cosy*cosx)*distance;
}

//! Given a position, rotation, and distance ahead to be calculated, this function
//! computes an absolute position sideways
//! \param[in] pos The position
//! \param[in] rot The rotation
//! \param[in] distance The distance
//! \return The computed position
MAKO_INLINE Vec3d<Float32> Sideways(const Vec3d<Float32>& pos,
									const Vec3d<Float32>& rot,
									const Float32 distance)
{
	Float32 newry = (rot.y+90.f) * DEGTORAD;
	Float32 siny = Sin<Float32>(newry);
	Float32 cosy = Cos<Float32>(newry);

	return pos + Vec3d<Float32>(siny, 0, cosy)*distance;
}

//! Given a position, rotation, and distance ahead to be calculated, this function
//! computes an absolute position sideways
//! \param[in] pos The position
//! \param[in] rot The rotation
//! \param[in] distance The distance
//! \return The computed position
MAKO_INLINE Vec3d<Float64> Sideways(const Vec3d<Float64>& pos,
									const Vec3d<Float64>& rot,
									const Float64& distance)
{
	Float64 newry = (rot.y+90.0) * DEGTORAD64;
	Float64 siny = Sin<Float64>(newry);
	Float64 cosy = Cos<Float64>(newry);

	return pos + Vec3d<Float64>(siny, 0, cosy)*distance;
}

//! Finds the distance between two vectors
//! \param[in] a The first vector
//! \param[in] b The second vector
//! \return The distance
template <typename T>
MAKO_INLINE T Distance(const Vec3d<T>& a, const Vec3d<T>& b)
{ return (a-b).Length(); }

//! Computes the dot product of two vectors
//! \param[in] a The first vector
//! \param[in] b The second vector
//! \return The dot product
template <typename T>
MAKO_INLINE T DotProduct(const Vec3d<T>& a, const Vec3d<T>& b)
{ return a.x*b.x + a.y*b.y + a.z*b.z; }

//! Computes the cross product of two vectors
//! \param[in] a The first vector
//! \param[in] b The second vector
//! \return The cross product
template <typename T>
MAKO_INLINE Vec3d<T> CrossProduct(const Vec3d<T>& a, const Vec3d<T>& b)
{ return Vec3d<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }

//! Computes the angle between two vectors
//! \param[in] a The first vector
//! \param[in] b The second vector
//! \return The angle between a and b
template <typename T>
MAKO_INLINE T AngleBetween(const Vec3d<T>& a, const Vec3d<T>& b)
{ return acos((DotProduct(a, b)) / (a.Length()*b.Length())); }

MAKO_END_NAMESPACE
