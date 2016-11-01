#pragma once
#include "MakoCommon.h"
#include "MakoVec3d.h"
#include "MakoVec2d.h"
#include "MakoMath.h"

MAKO_BEGIN_NAMESPACE

//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
//! The matrix is a D3D style matrix, row major with translations in the 4th row.
//! This class has been taken from the Irrlicht Graphics Engine, and has been lightly
//! edited.
template <typename T>
class Matrix4
{
public:

	//! Constructor Flags
	enum CONSTRUCTION_STYLE
	{
		CS_NOTHING,
		CS_COPY,
		CS_IDENTITY,
		CS_TRANSPOSED,
		CS_INVERSE,
		CS_INVERSE_TRANSPOSED
	};

	//! Default constructor
	//! \param[in] constructor Choose the initialization style
	Matrix4(CONSTRUCTION_STYLE constructor = CS_IDENTITY);
	
	//! Copy constructor
	//! \param[in] other Other matrix to copy from
	//! \param[in] constructor Choose the initialization style
	Matrix4(const Matrix4<T>& other, CONSTRUCTION_STYLE constructor = CS_COPY);

	//! Simple operator for directly accessing every element of the matrix.
	MAKO_INLINE T& operator() (const Int32 row, const Int32 col)
	{ definitelyIdentityMatrix = false; return M[row * 4 + col]; }

	//! Simple operator for directly accessing every element of the matrix.
	MAKO_INLINE const T& operator() (const Int32 row, const Int32 col) const
	{ return M[row * 4 + col]; }

	//! Simple operator for linearly accessing every element of the matrix.
	MAKO_INLINE T& operator[] (UInt32 index)
	{ definitelyIdentityMatrix = false; return M[index]; }

	//! Simple operator for linearly accessing every element of the matrix.
	MAKO_INLINE const T& operator[] (UInt32 index) const
	{ return M[index]; }

	//! Sets this matrix equal to the other matrix.
	MAKO_INLINE Matrix4<T>& operator = (const Matrix4<T> &other);

	//! Sets all elements of this matrix to the value.
	MAKO_INLINE Matrix4<T>& operator = (const T& scalar);

	//! Returns pointer to internal array
	MAKO_INLINE const T* Pointer() const
	{ return M; }
	
	//! Returns pointer to internal array
	MAKO_INLINE T* Pointer()
	{ definitelyIdentityMatrix = false; return M; }

	//! Returns true if other matrix is equal to this matrix.
	bool operator == (const Matrix4<T> &other) const;

	//! Returns true if other matrix is not equal to this matrix.
	bool operator != (const Matrix4<T> &other) const;

	//! Add another matrix.
	Matrix4<T> operator + (const Matrix4<T>& other) const;

	//! Add another matrix.
	Matrix4<T>& operator += (const Matrix4<T>& other);

	//! Subtract another matrix.
	Matrix4<T> operator - (const Matrix4<T>& other) const;

	//! Subtract another matrix.
	Matrix4<T>& operator -= (const Matrix4<T>& other);

	//! set this matrix to the product of two matrices
	MAKO_INLINE Matrix4<T>& SetByproduct(const Matrix4<T>& other_a,const Matrix4<T>& other_b );

	//! Set this matrix to the product of two matrices, no optimization used.
	//! use it if you know you never have a identity matrix
	Matrix4<T>& SetByproductNoCheck(const Matrix4<T>& other_a,const Matrix4<T>& other_b);

	//! Multiply by another matrix.
	Matrix4<T> operator * (const Matrix4<T>& other) const;

	//! Multiply by another matrix.
	Matrix4<T>& operator *= (const Matrix4<T>& other);

	//! Multiply by scalar.
	Matrix4<T> operator * (const T& scalar) const;

	//! Multiply by scalar.
	Matrix4<T>& operator *= (const T& scalar);

	//! Set matrix to identity.
	MAKO_INLINE Matrix4<T>& MakeIdentity();

	//! Returns true if the matrix is the identity matrix
	MAKO_INLINE bool IsIdentity() const;

	//! Returns true if the matrix is the identity matrix
	bool IsIdentityIntegerBase() const;

	//! Set the translation of the current matrix. Will erase any previous values.
	Matrix4<T>& SetTranslation(const Vec3d<T>& translation);

	//! Gets the current translation
	Vec3d<T> GetTranslation() const;

	//! Set the inverse translation of the current matrix. Will erase any previous values.
	Matrix4<T>& SetInverseTranslation(const Vec3d<T>& translation);

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	MAKO_INLINE Matrix4<T>& SetRotationRadians(const Vec3d<T>& rotation);

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	Matrix4<T>& SetRotationDegrees(const Vec3d<T>& rotation);

	//! Returns the rotation, as set by setRotation().
	//! This code was orginally written by by Chev.
	Vec3d<T> GetRotationDegrees() const;

	//! Make an inverted rotation matrix from Euler angles.
	//!The 4th row and column are unmodified.
	MAKO_INLINE Matrix4<T>& SetInverseRotationRadians(const Vec3d<T>& rotation);

	//! Make an inverted rotation matrix from Euler angles.
	//!The 4th row and column are unmodified.
	Matrix4<T>& SetInverseRotationDegrees(const Vec3d<T>& rotation);

	//! Set Scale
	Matrix4<T>& SetScale(const Vec3d<T>& scale);

	//! Set Scale
	MAKO_INLINE Matrix4<T>& SetScale(const T scale)
	{ return SetScale(Vec3d<T>(scale,scale,scale)); }

	//! Get Scale
	Vec3d<T> GetScale() const;

	//! Translate a vector by the inverse of the translation part of this matrix.
	void InverseTranslateVect(Vec3df& vect) const;

	//! Rotate a vector by the inverse of the rotation part of this matrix.
	void InverseRotateVect(Vec3df& vect) const;

	//! Rotate a vector by the rotation part of this matrix.
	void RotateVect(Vec3df& vect) const;

	//! An alternate transform vector method, writing into a second vector
	void RotateVect(Vec3df& out, const Vec3df& in) const;

	//! An alternate transform vector method, writing into an array of 3 floats
	void RotateVect(T *out,const Vec3df &in) const;

	//! Transforms the vector by this matrix
	void TransformVect(Vec3df& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void TransformVect(Vec3df& out, const Vec3df& in) const;

	//! An alternate transform vector method, writing into an array of 4 floats
	void TransformVect(T *out,const Vec3df &in) const;

	//! Translate a vector by the translation part of this matrix.
	void TranslateVect(Vec3df& vect) const;

	//! Multiplies this matrix by a 1x4 matrix
	void MultiplyWith1x4Matrix(T* matrix) const;

	//! Calculates inverse of matrix. Slow.
	//! \return Returns false if there is no inverse matrix.
	bool MakeInverse();

	//! Inverts a primitive matrix which only contains a translation and a rotation
	//! \param[out] out Where result matrix is written to.
	bool GetInversePrimitive(Matrix4<T>& out) const;

	//! Gets the inversed matrix of this one
	//! \param[out] out: where result matrix is written to.
	//! \return Returns false if there is no inverse matrix.
	bool GetInverse(Matrix4<T>& out) const;

	//! Builds a right-handed perspective projection matrix based on a field of view
	Matrix4<T>& BuildProjectionMatrixPerspectiveFovRH(Float32 fieldOfViewRadians, Float32 aspectRatio, Float32 zNear, Float32 zFar);

	//! Builds a left-handed perspective projection matrix based on a field of view
	Matrix4<T>& BuildProjectionMatrixPerspectiveFovLH(Float32 fieldOfViewRadians, Float32 aspectRatio, Float32 zNear, Float32 zFar);

	//! Builds a right-handed perspective projection matrix.
	Matrix4<T>& BuildProjectionMatrixPerspectiveRH(Float32 widthOfViewVolume, Float32 heightOfViewVolume, Float32 zNear, Float32 zFar);

	//! Builds a left-handed perspective projection matrix.
	Matrix4<T>& BuildProjectionMatrixPerspectiveLH(Float32 widthOfViewVolume, Float32 heightOfViewVolume, Float32 zNear, Float32 zFar);

	//! Builds a left-handed orthogonal projection matrix.
	Matrix4<T>& BuildProjectionMatrixOrthoLH(Float32 widthOfViewVolume, Float32 heightOfViewVolume, Float32 zNear, Float32 zFar);

	//! Builds a right-handed orthogonal projection matrix.
	Matrix4<T>& BuildProjectionMatrixOrthoRH(Float32 widthOfViewVolume, Float32 heightOfViewVolume, Float32 zNear, Float32 zFar);

	//! Builds a left-handed look-at matrix.
	Matrix4<T>& BuildCameraLookAtMatrixLH(
		const Position3d& position,
		const Vec3df& target,
		const Vec3df& upVector);

	//! Builds a right-handed look-at matrix.
	Matrix4<T>& BuildCameraLookAtMatrixRH(
		const Position3d& position,
		const Vec3df& target,
		const Vec3df& upVector);

	//! Creates a new matrix as interpolated matrix from two other ones.
	//! \param[in] b: other matrix to Interpolate with
	//! \param[in] time: Must be a value between 0 and 1.
	Matrix4<T> Interpolate(const Matrix4<T>& b, Float32 time) const;

	//! Gets transposed matrix
	Matrix4<T> GetTransposed() const;

	//! Gets transposed matrix
	MAKO_INLINE void GetTransposed( Matrix4<T>& dest ) const;

	//! construct 2d Texture transformations
	//! rotate about center, scale, and transform.
	//! Set to a texture transformation matrix with the given parameters.
	Matrix4<T>& BuildTextureTransform(Float32 rotateRad,
		const Vec2df &rotatecenter,
		const Vec2df &translate,
		const Vec2df &scale);

	//! Set texture transformation rotation
	//! Rotate about z axis, recenter at (0.5,0.5).
	//! Doesn't clear other elements than those affected
	//! \param[in] radAngle Angle in radians
	//! \return Altered matrix */
	Matrix4<T>& SetTextureRotationCenter(Float32 radAngle);

	//! Set texture transformation translation
	//! Doesn't clear other elements than those affected.
	//! \param[in] x Offset on x axis
	//! \param[in] y Offset on y axis
	//! \return Altered matrix
	Matrix4<T>& SetTextureTranslate(Float32 x, Float32 y);

	//! Set texture transformation translation, using a transposed representation
	//! Doesn't clear other elements than those affected.
	//! \param[in] x Offset on x axis
	//! \param[in] y Offset on y axis
	//! \return Altered matrix
	Matrix4<T>& SetTextureTranslateTransposed(Float32 x, Float32 y);

	//! Set texture transformation scale
	//! Doesn't clear other elements than those affected.
	//! \param[in] sx Scale factor on x axis
	//! \param[in] sy Scale factor on y axis
	//! \return Altered matrix.
	Matrix4<T>& SetTextureScale(Float32 sx, Float32 sy);

	//! Set texture transformation scale, and recenter at (0.5,0.5)
	//! Doesn't clear other elements than those affected.
	//! \param[in] sx Scale factor on x axis
	//! \param[in] sy Scale factor on y axis
	//! \return Altered matrix.
	Matrix4<T>& SetTextureScaleCenter( Float32 sx, Float32 sy );

	//! Sets all matrix data members at once
	Matrix4<T>& SetM(const T* data);

	//! Sets if the matrix is definitely identity matrix
	void SetDefinitelyIdentityMatrix( bool isDefinitelyIdentityMatrix);

	//! Gets if the matrix is definitely identity matrix
	bool GetDefinitelyIdentityMatrix() const;

private:
	//! Matrix data, stored in row-major order
	T M[16];
	//! Flag is this matrix is identity matrix
	mutable bool definitelyIdentityMatrix;
};

// Default constructor
template <typename T>
MAKO_INLINE Matrix4<T>::Matrix4(CONSTRUCTION_STYLE constructor)
: definitelyIdentityMatrix(false)
{
	switch (constructor)
	{
	case CS_NOTHING:
	case CS_COPY:
		break;
	case CS_IDENTITY:
	case CS_INVERSE:
	default:
		MakeIdentity();
		break;
	}
}

// Copy constructor
template <typename T>
MAKO_INLINE Matrix4<T>::Matrix4(const Matrix4<T>& other, CONSTRUCTION_STYLE constructor)
: definitelyIdentityMatrix(false)
{
	switch (constructor)
	{
	case CS_IDENTITY:
		MakeIdentity();
		break;
	case CS_NOTHING:
		break;
	case CS_COPY:
		*this = other;
		break;
	case CS_TRANSPOSED:
		other.GetTransposed(*this);
		break;
	case CS_INVERSE:
		if (!other.GetInverse(*this))
			memset(M, 0, 16*sizeof(T));
		break;
	case CS_INVERSE_TRANSPOSED:
		if (!other.GetInverse(*this))
			memset(M, 0, 16*sizeof(T));
		else
			*this=GetTransposed();
		break;
	}
}

//! Add another matrix.
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::operator + (const Matrix4<T>& other) const
{
	Matrix4<T> temp(CS_NOTHING);

	temp[0] = M[0]+other[0];
	temp[1] = M[1]+other[1];
	temp[2] = M[2]+other[2];
	temp[3] = M[3]+other[3];
	temp[4] = M[4]+other[4];
	temp[5] = M[5]+other[5];
	temp[6] = M[6]+other[6];
	temp[7] = M[7]+other[7];
	temp[8] = M[8]+other[8];
	temp[9] = M[9]+other[9];
	temp[10] = M[10]+other[10];
	temp[11] = M[11]+other[11];
	temp[12] = M[12]+other[12];
	temp[13] = M[13]+other[13];
	temp[14] = M[14]+other[14];
	temp[15] = M[15]+other[15];

	return temp;
}

//! Add another matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator += (const Matrix4<T>& other)
{
	M[0]+=other[0];
	M[1]+=other[1];
	M[2]+=other[2];
	M[3]+=other[3];
	M[4]+=other[4];
	M[5]+=other[5];
	M[6]+=other[6];
	M[7]+=other[7];
	M[8]+=other[8];
	M[9]+=other[9];
	M[10]+=other[10];
	M[11]+=other[11];
	M[12]+=other[12];
	M[13]+=other[13];
	M[14]+=other[14];
	M[15]+=other[15];

	return *this;
}

//! Subtract another matrix.
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::operator - (const Matrix4<T>& other) const
{
	Matrix4<T> temp(CS_NOTHING);

	temp[0] = M[0]-other[0];
	temp[1] = M[1]-other[1];
	temp[2] = M[2]-other[2];
	temp[3] = M[3]-other[3];
	temp[4] = M[4]-other[4];
	temp[5] = M[5]-other[5];
	temp[6] = M[6]-other[6];
	temp[7] = M[7]-other[7];
	temp[8] = M[8]-other[8];
	temp[9] = M[9]-other[9];
	temp[10] = M[10]-other[10];
	temp[11] = M[11]-other[11];
	temp[12] = M[12]-other[12];
	temp[13] = M[13]-other[13];
	temp[14] = M[14]-other[14];
	temp[15] = M[15]-other[15];

	return temp;
}

//! Subtract another matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator -= (const Matrix4<T>& other)
{
	M[0]-=other[0];
	M[1]-=other[1];
	M[2]-=other[2];
	M[3]-=other[3];
	M[4]-=other[4];
	M[5]-=other[5];
	M[6]-=other[6];
	M[7]-=other[7];
	M[8]-=other[8];
	M[9]-=other[9];
	M[10]-=other[10];
	M[11]-=other[11];
	M[12]-=other[12];
	M[13]-=other[13];
	M[14]-=other[14];
	M[15]-=other[15];

	return *this;
}

//! Multiply by scalar.
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::operator * (const T& scalar) const
{
	Matrix4<T> temp(CS_NOTHING);

	temp[0] = M[0]*scalar;
	temp[1] = M[1]*scalar;
	temp[2] = M[2]*scalar;
	temp[3] = M[3]*scalar;
	temp[4] = M[4]*scalar;
	temp[5] = M[5]*scalar;
	temp[6] = M[6]*scalar;
	temp[7] = M[7]*scalar;
	temp[8] = M[8]*scalar;
	temp[9] = M[9]*scalar;
	temp[10] = M[10]*scalar;
	temp[11] = M[11]*scalar;
	temp[12] = M[12]*scalar;
	temp[13] = M[13]*scalar;
	temp[14] = M[14]*scalar;
	temp[15] = M[15]*scalar;

	return temp;
}

//! Multiply by scalar.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator *= (const T& scalar)
{
	M[0]*=scalar;
	M[1]*=scalar;
	M[2]*=scalar;
	M[3]*=scalar;
	M[4]*=scalar;
	M[5]*=scalar;
	M[6]*=scalar;
	M[7]*=scalar;
	M[8]*=scalar;
	M[9]*=scalar;
	M[10]*=scalar;
	M[11]*=scalar;
	M[12]*=scalar;
	M[13]*=scalar;
	M[14]*=scalar;
	M[15]*=scalar;

	return *this;
}

//! Multiply by another matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator *= (const Matrix4<T>& other)
{
	// do checks on your own in order to avoid copy creation
	if ( !other.IsIdentity() )
	{
		if ( this->IsIdentity() )
		{
			return (*this = other);
		}
		else
		{
			Matrix4<T> temp ( *this );
			return SetByproductNoCheck( temp, other );
		}
	}
	return *this;
}

//! multiply by another matrix
// set this matrix to the product of two other matrices
// goal is to reduce stack use and copy
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetByproductNoCheck(const Matrix4<T>& other_a, const Matrix4<T>& other_b)
{
	const T *m1 = other_a.M;
	const T *m2 = other_b.M;

	M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	definitelyIdentityMatrix = false;
	return *this;
}


//! multiply by another matrix
// set this matrix to the product of two other matrices
// goal is to reduce stack use and copy
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetByproduct(const Matrix4<T>& other_a, const Matrix4<T>& other_b)
{
	if (other_a.IsIdentity ())
		return (*this = other_b);
	else
		if (other_b.IsIdentity ())
			return (*this = other_a);
		else
			return SetByproductNoCheck(other_a,other_b);
}

//! multiply by another matrix
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::operator * (const Matrix4<T>& m2) const
{
	// Testing purpose..
	if (this->IsIdentity())
		return m2;
	if (m2.IsIdentity())
		return *this;

	Matrix4<T> m3(CS_NOTHING);

	const T *m1 = M;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	return m3;
}



template <typename T>
MAKO_INLINE Vec3d<T> Matrix4<T>::GetTranslation() const
{ return Vec3d<T>(M[12], M[13], M[14]); }


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTranslation(const Vec3d<T>& translation)
{
	M[12] = translation.x;
	M[13] = translation.y;
	M[14] = translation.z;
	definitelyIdentityMatrix = false;
	return *this;
}

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetInverseTranslation(const Vec3d<T>& translation)
{
	M[12] = -translation.x;
	M[13] = -translation.y;
	M[14] = -translation.z;
	definitelyIdentityMatrix = false;
	return *this;
}

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetScale(const Vec3d<T>& scale)
{
	M[0] = scale.x;
	M[5] = scale.y;
	M[10] = scale.z;
	definitelyIdentityMatrix = false;
	return *this;
}

template <typename T>
MAKO_INLINE Vec3d<T> Matrix4<T>::GetScale() const
{ return Vec3d<T>(M[0],M[5],M[10]); }

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetRotationDegrees(const Vec3d<T>& rotation)
{ return SetRotationRadians(rotation * DEGTORAD); }

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetInverseRotationDegrees(const Vec3d<T>& rotation)
{ return SetInverseRotationRadians(rotation * DEGTORAD); }

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetRotationRadians(const Vec3d<T>& rotation)
{
	const Float64 cr = Cos(rotation.x);
	const Float64 sr = Sin(rotation.x);
	const Float64 cp = Cos(rotation.y);
	const Float64 sp = Sin(rotation.y);
	const Float64 cy = Cos(rotation.z);
	const Float64 sy = Sin(rotation.z);

	M[0] = (T)(cp * cy);
	M[1] = (T)(cp * sy);
	M[2] = (T)(-sp);

	const Float64 srsp = sr*sp;
	const Float64 crsp = cr*sp;

	M[4] = (T)(srsp*cy - cr*sy);
	M[5] = (T)(srsp*sy + cr*cy);
	M[6] = (T)(sr*cp);

	M[8] = (T)(crsp*cy + sr*sy);
	M[9] = (T)(crsp*sy - sr*cy);
	M[10] = (T)(cr*cp);
	definitelyIdentityMatrix = false;
	return *this;
}

//! Returns a rotation that is equivalent to that set by SetRotationDegrees().
//! This code was sent in by Chev.  Note that it does not necessarily return
//! the *same* Euler angles as those set by SetRotationDegrees(), but the rotation will
//! be equivalent, i.e. will have the same result when used to rotate a vector or node.
template <typename T>
MAKO_INLINE Vec3d<T> Matrix4<T>::GetRotationDegrees() const
{
	const Matrix4<T> &mat = *this;

	Float64 Y = -asin(mat(0,2));
	const Float64 C = Cos(Y);
	Y *= RADTODEG64;

	Float64 rotx, roty, X, Z;

	if (fabs(C) > ROUNDING_ERROR_64)
	{
		const T invC = (T)(1.0/C);
		rotx = mat(2,2) * invC;
		roty = mat(1,2) * invC;
		X = atan2(roty, rotx) * RADTODEG64;
		rotx = mat(0,0) * invC;
		roty = mat(0,1) * invC;
		Z = atan2(roty, rotx) * RADTODEG64;
	}
	else
	{
		X = 0.0;
		rotx = mat(1,1);
		roty = -mat(1,0);
		Z = atan2(roty, rotx) * RADTODEG64;
	}

	// fix values that get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if (X < 0.0) X += 360.0;
	if (Y < 0.0) Y += 360.0;
	if (Z < 0.0) Z += 360.0;

	return Vec3d<T>((T)X,(T)Y,(T)Z);
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetInverseRotationRadians(const Vec3d<T>& rotation)
{
	Float64 cr = Cos(rotation.x);
	Float64 sr = Sin(rotation.x);
	Float64 cp = Cos(rotation.y);
	Float64 sp = Sin(rotation.y);
	Float64 cy = Cos(rotation.z);
	Float64 sy = Sin(rotation.z);

	M[0] = (T)(cp*cy);
	M[4] = (T)(cp*sy);
	M[8] = (T)(-sp);

	Float64 srsp = sr*sp;
	Float64 crsp = cr*sp;

	M[1] = (T)(srsp*cy-cr*sy);
	M[5] = (T)(srsp*sy+cr*cy);
	M[9] = (T)(sr*cp);

	M[2] = (T)(crsp*cy+sr*sy);
	M[6] = (T)(crsp*sy-sr*cy);
	M[10] = (T)(cr*cp);
	definitelyIdentityMatrix = false;
	return *this;
}


/*!
*/
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::MakeIdentity()
{
	memset(M, 0, 16*sizeof(T));
	M[0] = M[5] = M[10] = M[15] = (T)1;
	definitelyIdentityMatrix = true;
	return *this;
}


/*
check identity with epsilon
solve floating range problems..
*/
template <typename T>
MAKO_INLINE bool Matrix4<T>::IsIdentity() const
{
	if (definitelyIdentityMatrix)
		return true;
	if (!Equals(M[ 0], (T)1) ||
		!Equals(M[ 5], (T)1) ||
		!Equals(M[10], (T)1) ||
		!Equals(M[15], (T)1))
		return false;

	for (Int32 i=0; i<4; ++i)
		for (Int32 j=0; j<4; ++j)
			if ((j != i) && (!IsZero((*this)(i,j))))
				return false;

	definitelyIdentityMatrix = true;
	return true;
}

/*
doesn't solve floating range problems..
but takes care on +/- 0 on translation because we are changing it..
reducing floating point branches
but it needs the floats in memory..
*/
template <typename T>
MAKO_INLINE bool Matrix4<T>::IsIdentityIntegerBase() const
{
	if (definitelyIdentityMatrix)
		return true;
	if(R(M[0]) != F32_VALUE_1)  return false;
	if(R(M[1]) != 0)            return false;
	if(R(M[2]) != 0)            return false;
	if(R(M[3]) != 0)            return false;

	if(R(M[4]) != 0)            return false;
	if(R(M[5]) != F32_VALUE_1)  return false;
	if(R(M[6]) != 0)            return false;
	if(R(M[7]) != 0)            return false;

	if(R(M[8]) != 0)            return false;
	if(R(M[9]) != 0)            return false;
	if(R(M[10]) != F32_VALUE_1) return false;
	if(R(M[11]) != 0)           return false;

	if(R(M[12]) != 0)           return false;
	if(R(M[13]) != 0)           return false;
	if(R(M[13]) != 0)           return false;
	if(R(M[15]) != F32_VALUE_1)	return false;
	definitelyIdentityMatrix = true;
	return true;
}


template <typename T>
MAKO_INLINE void Matrix4<T>::RotateVect(Vec3df& vect) const
{
	Vec3df tmp = vect;
	vect.x = tmp.x*M[0] + tmp.y*M[4] + tmp.z*M[8];
	vect.y = tmp.x*M[1] + tmp.y*M[5] + tmp.z*M[9];
	vect.z = tmp.x*M[2] + tmp.y*M[6] + tmp.z*M[10];
}

//! An alternate transform vector method, writing into a second vector
template <typename T>
MAKO_INLINE void Matrix4<T>::RotateVect(Vec3df& out, const Vec3df& in) const
{
	out.x = in.x*M[0] + in.y*M[4] + in.z*M[8];
	out.y = in.x*M[1] + in.y*M[5] + in.z*M[9];
	out.z = in.x*M[2] + in.y*M[6] + in.z*M[10];
}

//! An alternate transform vector method, writing into an array of 3 floats
template <typename T>
MAKO_INLINE void Matrix4<T>::RotateVect(T *out, const Vec3df& in) const
{
	out[0] = in.x*M[0] + in.y*M[4] + in.z*M[8];
	out[1] = in.x*M[1] + in.y*M[5] + in.z*M[9];
	out[2] = in.x*M[2] + in.y*M[6] + in.z*M[10];
}

template <typename T>
MAKO_INLINE void Matrix4<T>::InverseRotateVect(Vec3df& vect) const
{
	Vec3df tmp = vect;
	vect.x = tmp.x*M[0] + tmp.y*M[1] + tmp.z*M[2];
	vect.y = tmp.x*M[4] + tmp.y*M[5] + tmp.z*M[6];
	vect.z = tmp.x*M[8] + tmp.y*M[9] + tmp.z*M[10];
}

template <typename T>
MAKO_INLINE void Matrix4<T>::TransformVect(Vec3df& vect) const
{
	Float32 vector[3];

	vector[0] = vect.x*M[0] + vect.y*M[4] + vect.z*M[8] + M[12];
	vector[1] = vect.x*M[1] + vect.y*M[5] + vect.z*M[9] + M[13];
	vector[2] = vect.x*M[2] + vect.y*M[6] + vect.z*M[10] + M[14];

	vect.x = vector[0];
	vect.y = vector[1];
	vect.z = vector[2];
}

template <typename T>
MAKO_INLINE void Matrix4<T>::TransformVect(Vec3df& out, const Vec3df& in) const
{
	out.x = in.x*M[0] + in.y*M[4] + in.z*M[8] + M[12];
	out.y = in.x*M[1] + in.y*M[5] + in.z*M[9] + M[13];
	out.z = in.x*M[2] + in.y*M[6] + in.z*M[10] + M[14];
}


template <typename T>
MAKO_INLINE void Matrix4<T>::TransformVect(T *out, const Vec3df &in) const
{
	out[0] = in.x*M[0] + in.y*M[4] + in.z*M[8] + M[12];
	out[1] = in.x*M[1] + in.y*M[5] + in.z*M[9] + M[13];
	out[2] = in.x*M[2] + in.y*M[6] + in.z*M[10] + M[14];
	out[3] = in.x*M[3] + in.y*M[7] + in.z*M[11] + M[15];
}


//! Multiplies this matrix by a 1x4 matrix
template <typename T>
MAKO_INLINE void Matrix4<T>::MultiplyWith1x4Matrix(T* matrix) const
{
	/*
	0  1  2  3
	4  5  6  7
	8  9  10 11
	12 13 14 15
	*/

	T mat[4];
	mat[0] = matrix[0];
	mat[1] = matrix[1];
	mat[2] = matrix[2];
	mat[3] = matrix[3];

	matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
	matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
	matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
	matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
}

template <typename T>
MAKO_INLINE void Matrix4<T>::InverseTranslateVect(Vec3df& vect) const
{
	vect.x = vect.x-M[12];
	vect.y = vect.y-M[13];
	vect.z = vect.z-M[14];
}

template <typename T>
MAKO_INLINE void Matrix4<T>::TranslateVect(Vec3df& vect) const
{
	vect.x = vect.x+M[12];
	vect.y = vect.y+M[13];
	vect.z = vect.z+M[14];
}


template <typename T>
MAKO_INLINE bool Matrix4<T>::GetInverse(Matrix4<T>& out) const
{
	/// Calculates the inverse of this Matrix
	/// The inverse is calculated using Cramers rule.
	/// f no inverse exists then 'false' is returned.

	if (this->IsIdentity())
	{
		out=*this;
		return true;
	}

	const Matrix4<T> &m = *this;

	Float32 d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
		(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
		(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
		(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
		(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
		(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

	if( IsZero ( d ) )
		return false;

	d = Reciprocal ( d );

	out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
		m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
		m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
	out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
		m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
		m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
	out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
		m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
		m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
	out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
		m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
		m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
	out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
		m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
		m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
	out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
		m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
		m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
	out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
		m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
		m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
	out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
		m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
		m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
	out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
		m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
		m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
	out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
		m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
		m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
	out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
		m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
		m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
	out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
		m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
		m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
	out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
		m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
		m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
	out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
		m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
		m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
	out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
		m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
		m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
	out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
		m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
		m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
	out.definitelyIdentityMatrix = definitelyIdentityMatrix;
	return true;
}


//! Inverts a primitive matrix which only contains a translation and a rotation
//! \param[out] out: where result matrix is written to.
template <typename T>
MAKO_INLINE bool Matrix4<T>::GetInversePrimitive (Matrix4<T>& out) const
{
	out.M[0 ] = M[0];
	out.M[1 ] = M[4];
	out.M[2 ] = M[8];
	out.M[3 ] = 0;

	out.M[4 ] = M[1];
	out.M[5 ] = M[5];
	out.M[6 ] = M[9];
	out.M[7 ] = 0;

	out.M[8 ] = M[2];
	out.M[9 ] = M[6];
	out.M[10] = M[10];
	out.M[11] = 0;

	out.M[12] = (T)-(M[12]*M[0] + M[13]*M[1] + M[14]*M[2]);
	out.M[13] = (T)-(M[12]*M[4] + M[13]*M[5] + M[14]*M[6]);
	out.M[14] = (T)-(M[12]*M[8] + M[13]*M[9] + M[14]*M[10]);
	out.M[15] = 1;
	out.definitelyIdentityMatrix = definitelyIdentityMatrix;
	return true;
}

/*!
*/
template <typename T>
MAKO_INLINE bool Matrix4<T>::MakeInverse()
{
	if (definitelyIdentityMatrix)
		return true;

	Matrix4<T> temp ( CS_NOTHING );

	if (GetInverse(temp))
	{
		*this = temp;
		return true;
	}

	return false;
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator = (const Matrix4<T> &other)
{
	if (this == &other)
		return *this;
	memcpy(M, other.M, 16 * sizeof(T));
	definitelyIdentityMatrix = other.definitelyIdentityMatrix;
	return *this;
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::operator = (const T& scalar)
{
	for (Int32 i = 0; i < 16; ++i)
		M[i]=scalar;
	definitelyIdentityMatrix = false;
	return *this;
}


template <typename T>
MAKO_INLINE bool Matrix4<T>::operator == (const Matrix4<T> &other) const
{
	if (definitelyIdentityMatrix && other.definitelyIdentityMatrix)
		return true;
	for (Int32 i = 0; i < 16; ++i)
		if (M[i] != other.M[i])
			return false;

	return true;
}


template <typename T>
MAKO_INLINE bool Matrix4<T>::operator != (const Matrix4<T> &other) const
{
	return !(*this == other);
}


// Builds a right-handed perspective projection matrix based on a field of view
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixPerspectiveFovRH(Float32 fieldOfViewRadians,
																		  Float32 aspectRatio,
																		  Float32 zNear,
																		  Float32 zFar)
{
	const Float64 h = 1.0/tan(fieldOfViewRadians/2.0);
	const T w = h / aspectRatio;

	M[0] = w;
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)h;
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(zFar/(zNear-zFar)); // DirectX version
	//		M[10] = (T)(zFar+zNear/(zNear-zFar)); // OpenGL version
	M[11] = -1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(zNear*zFar/(zNear-zFar)); // DirectX version
	//		M[14] = (T)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
	M[15] = 0;
	definitelyIdentityMatrix = false;
	return *this;
}


// Builds a left-handed perspective projection matrix based on a field of view
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixPerspectiveFovLH(Float32 fieldOfViewRadians,
																		  Float32 aspectRatio,
																		  Float32 zNear,
																		  Float32 zFar)
{
	const Float64 h = 1.0/tan(fieldOfViewRadians/2.0);
	const T w = (T)(h / aspectRatio);

	M[0] = w;
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)h;
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(zFar/(zFar-zNear));
	M[11] = 1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(-zNear*zFar/(zFar-zNear));
	M[15] = 0;
	definitelyIdentityMatrix = false;
	return *this;
}


// Builds a left-handed orthogonal projection matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixOrthoLH(Float32 widthOfViewVolume,
																 Float32 heightOfViewVolume,
																 Float32 zNear,
																 Float32 zFar)
{
	M[0] = (T)(2/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)(2/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(1/(zFar-zNear));
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(zNear/(zNear-zFar));
	M[15] = 1;
	definitelyIdentityMatrix = false;
	return *this;
}


// Builds a right-handed orthogonal projection matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixOrthoRH(Float32 widthOfViewVolume,
																 Float32 heightOfViewVolume,
																 Float32 zNear,
																 Float32 zFar)
{
	M[0] = (T)(2/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)(2/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(1/(zNear-zFar));
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(zNear/(zNear-zFar));
	M[15] = -1;
	definitelyIdentityMatrix = false;
	return *this;
}


// Builds a right-handed perspective projection matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixPerspectiveRH(Float32 widthOfViewVolume,
																	   Float32 heightOfViewVolume,
																	   Float32 zNear,
																	   Float32 zFar)
{
	M[0] = (T)(2*zNear/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)(2*zNear/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(zFar/(zNear-zFar));
	M[11] = -1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(zNear*zFar/(zNear-zFar));
	M[15] = 0;
	definitelyIdentityMatrix = false;
	return *this;
}


//! Builds a left-handed perspective projection matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildProjectionMatrixPerspectiveLH(Float32 widthOfViewVolume,
																	   Float32 heightOfViewVolume,
																	   Float32 zNear,
																	   Float32 zFar)
{
	M[0] = (T)(2*zNear/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (T)(2*zNear/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (T)(zFar/(zFar-zNear));
	M[11] = 1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (T)(zNear*zFar/(zNear-zFar));
	M[15] = 0;
	definitelyIdentityMatrix = false;
	return *this;
}

//! Builds a left-handed look-at matrix.
//! \return This matrix
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildCameraLookAtMatrixLH(const Position3d& position,
															  const Vec3df& target,
															  const Vec3df& upVector)
{
	Vec3df zaxis = target - position;
	zaxis.Normalize();

	Vec3df xaxis = CrossProduct(upVector, zaxis);
	xaxis.Normalize();

	Vec3df yaxis = CrossProduct(zaxis, xaxis);

	M[0] = (T)xaxis.x;
	M[1] = (T)yaxis.x;
	M[2] = (T)zaxis.x;
	M[3] = 0;

	M[4] = (T)xaxis.y;
	M[5] = (T)yaxis.y;
	M[6] = (T)zaxis.y;
	M[7] = 0;

	M[8] = (T)xaxis.z;
	M[9] = (T)yaxis.z;
	M[10] = (T)zaxis.z;
	M[11] = 0;

	M[12] = (T)-DotProduct(xaxis, position);
	M[13] = (T)-DotProduct(yaxis, position);
	M[14] = (T)-DotProduct(zaxis, position);
	M[15] = 1;
	definitelyIdentityMatrix = false;
	return *this;
}


//! Builds a right-handed look-at matrix.
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildCameraLookAtMatrixRH(const Position3d& position,
															  const Vec3df& target,
															  const Vec3df& upVector)
{
	Vec3df zaxis = position - target;
	zaxis.normalize();

	Vec3df xaxis = upVector.crossProduct(zaxis);
	xaxis.normalize();

	Vec3df yaxis = zaxis.crossProduct(xaxis);

	M[0] = (T)xaxis.x;
	M[1] = (T)yaxis.x;
	M[2] = (T)zaxis.x;
	M[3] = 0;

	M[4] = (T)xaxis.y;
	M[5] = (T)yaxis.y;
	M[6] = (T)zaxis.y;
	M[7] = 0;

	M[8] = (T)xaxis.z;
	M[9] = (T)yaxis.z;
	M[10] = (T)zaxis.z;
	M[11] = 0;

	M[12] = (T)-xaxis.dotProduct(position);
	M[13] = (T)-yaxis.dotProduct(position);
	M[14] = (T)-zaxis.dotProduct(position);
	M[15] = 1;
	definitelyIdentityMatrix = false;
	return *this;
}


// creates a new matrix as interpolated matrix from this and the passed one.
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::Interpolate(const Matrix4<T>& b, Float32 time) const
{
	Matrix4<T> mat ( CS_NOTHING );

	for (UInt i=0; i < 16; i += 4)
	{
		mat.M[i+0] = (T)(M[i+0] + ( b.M[i+0] - M[i+0] ) * time);
		mat.M[i+1] = (T)(M[i+1] + ( b.M[i+1] - M[i+1] ) * time);
		mat.M[i+2] = (T)(M[i+2] + ( b.M[i+2] - M[i+2] ) * time);
		mat.M[i+3] = (T)(M[i+3] + ( b.M[i+3] - M[i+3] ) * time);
	}
	return mat;
}


// returns transposed matrix
template <typename T>
MAKO_INLINE Matrix4<T> Matrix4<T>::GetTransposed() const
{
	Matrix4<T> t(CS_NOTHING);
	GetTransposed(t);
	return t;
}


// returns transposed matrix
template <typename T>
MAKO_INLINE void Matrix4<T>::GetTransposed(Matrix4<T>& o) const
{
	o[ 0] = M[ 0];
	o[ 1] = M[ 4];
	o[ 2] = M[ 8];
	o[ 3] = M[12];

	o[ 4] = M[ 1];
	o[ 5] = M[ 5];
	o[ 6] = M[ 9];
	o[ 7] = M[13];

	o[ 8] = M[ 2];
	o[ 9] = M[ 6];
	o[10] = M[10];
	o[11] = M[14];

	o[12] = M[ 3];
	o[13] = M[ 7];
	o[14] = M[11];
	o[15] = M[15];
	o.definitelyIdentityMatrix=definitelyIdentityMatrix;
}

/*!
Generate texture coordinates as linear functions so that:
u = Ux*x + Uy*y + Uz*z + Uw
v = Vx*x + Vy*y + Vz*z + Vw
The matrix M for this case is:
Ux  Vx  0  0
Uy  Vy  0  0
Uz  Vz  0  0
Uw  Vw  0  0
*/


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::BuildTextureTransform(Float32 rotateRad,
														  const Vec2df &rotatecenter,
														  const Vec2df &translate,
														  const Vec2df &scale)
{
	const Float32 c = cosf(rotateRad);
	const Float32 s = sinf(rotateRad);

	M[0] = (T)(c * scale.x);
	M[1] = (T)(s * scale.y);
	M[2] = 0;
	M[3] = 0;

	M[4] = (T)(-s * scale.x);
	M[5] = (T)(c * scale.y);
	M[6] = 0;
	M[7] = 0;

	M[8] = (T)(c * scale.x * rotatecenter.x + -s * rotatecenter.y + translate.x);
	M[9] = (T)(s * scale.y * rotatecenter.x +  c * rotatecenter.y + translate.y);
	M[10] = 1;
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = 0;
	M[15] = 1;
	definitelyIdentityMatrix = false;
	return *this;
}


// rotate about z axis, center ( 0.5, 0.5 )
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTextureRotationCenter(Float32 rotateRad)
{
	const Float32 c = cosf(rotateRad);
	const Float32 s = sinf(rotateRad);
	M[0] = (T)c;
	M[1] = (T)s;

	M[4] = (T)-s;
	M[5] = (T)c;

	M[8] = (T)(0.5f * ( s - c) + 0.5f);
	M[9] = (T)(-0.5f * ( s + c) + 0.5f);
	definitelyIdentityMatrix = definitelyIdentityMatrix && (rotateRad == 0.0f);
	return *this;
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTextureTranslate(Float32 x, Float32 y)
{
	M[8] = (T)x;
	M[9] = (T)y;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (x == 0.0f) && (y == 0.0f);
	return *this;
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTextureTranslateTransposed(Float32 x, Float32 y)
{
	M[2] = (T)x;
	M[6] = (T)y;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (x == 0.0f) && (y == 0.0f) ;
	return *this;
}

template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTextureScale(Float32 sx, Float32 sy)
{
	M[0] = (T)sx;
	M[5] = (T)sy;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (sx == 1.0f) && (sy == 1.0f);
	return *this;
}


template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetTextureScaleCenter(Float32 sx, Float32 sy)
{
	M[0] = (T)sx;
	M[5] = (T)sy;
	M[8] = (T)(0.5f - 0.5f * sx);
	M[9] = (T)(0.5f - 0.5f * sy);
	definitelyIdentityMatrix = definitelyIdentityMatrix && (sx == 1.0f) && (sy == 1.0f);
	return *this;
}


// sets all matrix data members at once
template <typename T>
MAKO_INLINE Matrix4<T>& Matrix4<T>::SetM(const T* data)
{
	memcpy(M,data, 16*sizeof(T));

	definitelyIdentityMatrix = false;
	return *this;
}


// sets if the matrix is definitely identity matrix
template <typename T>
MAKO_INLINE void Matrix4<T>::SetDefinitelyIdentityMatrix(bool isDefinitelyIdentityMatrix)
{ definitelyIdentityMatrix = isDefinitelyIdentityMatrix; }


// gets if the matrix is definitely identity matrix
template <typename T>
MAKO_INLINE bool Matrix4<T>::GetDefinitelyIdentityMatrix() const
{ return definitelyIdentityMatrix; }


// Multiply by scalar.
template <typename T>
MAKO_INLINE Matrix4<T> operator*(const T scalar, const Matrix4<T>& mat)
{ return mat*scalar; }


//! Typedef for Float32 matrix
typedef Matrix4<Float32> Matrix4f;

MAKO_END_NAMESPACE
