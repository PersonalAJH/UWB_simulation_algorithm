/********************************************	
*		Original source code: SOFA			*
*		Modified by Hoeryong Jung			*
*		Date: 2009. 10. 21					*
*		contact: junghl80@kaist.ac.kr		*
/*******************************************/

#ifndef VEC_H
#define VEC_H

#include "FixedArray.h"
#include <functional>

#define EQUALITY_THRESHOLD 1e-6

enum NoInit { NOINIT }; ///< use when calling Vec or Mat constructor to skip initialization of values to 0

template <unsigned int N, typename real=float>
class Vec : public FixedArray<real,N>
{
public:

	/// Default constructor: sets all values to 0.
	Vec()
	{
		this->clear();
	}

	/// Fast constructor: no initialization
	explicit Vec(NoInit)
	{
	}

	/// Specific constructor for 1-element vectors.
	explicit Vec(real r1)
	{
		this->elems[0]=r1;
	}

	/// Specific constructor for 1-element vectors.
	void operator=(real r1)
	{
		this->elems[0]=r1;
	}

	/// Specific constructor for 2-elements vectors.
	Vec(real r1, real r2)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
	}

	/// Specific constructor for 3-elements vectors.
	Vec(real r1, real r2, real r3)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
	}

	/// Specific constructor for 4-elements vectors.
	Vec(real r1, real r2, real r3, real r4)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
	}

	/// Specific constructor for 6-elements vectors.
	Vec(real r1, real r2, real r3, real r4, real r5, real r6)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
		this->elems[5]=r6;
	}

	/// Specific constructor for 8-elements vectors.
	Vec(real r1, real r2, real r3, real r4, real r5, real r6, real r7, real r8)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
		this->elems[5]=r6;
		this->elems[6]=r7;
		this->elems[7]=r8;
	}

	/// Specific constructor for 9-elements vectors.
	Vec(real r1, real r2, real r3, real r4, real r5, real r6, real r7, real r8, real r9)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
		this->elems[5]=r6;
		this->elems[6]=r7;
		this->elems[7]=r8;
		this->elems[8]=r9;
	}

	/// Constructor from an N-1 elements vector and an additional value (added at the end).
	Vec(const Vec<N-1,real>& v, real r1)
	{
		for(int i=0;i<N-1;i++)
			this->elems[i] = v[i];
		this->elems[N-1]=r1;
	}

	template<typename real2>
	Vec(const Vec<N, real2>& p)
	{
		for(int i=0;i<N;i++)
			this->elems[i] = (real)p(i);
	}

	/// Constructor from an array of values.
	template<typename real2>
	explicit Vec(const real2* p)
	{
		//std::copy(p, p+N, this->begin());
		for(int i=0;i<N;i++)
			this->elems[i] = (real)p[i];
	}

	/// Special access to first element.
	real& x()
	{
		return this->elems[0];
	}
	/// Special access to second element.
	real& y()
	{
		return this->elems[1];
	}
	/// Special access to third element.
	real& z()
	{
		return this->elems[2];
	}
	/// Special access to fourth element.
	real& w()
	{
		return this->elems[3];
	}

	/// Special const access to first element.
	const real& x() const
	{
		return this->elems[0];
	}
	/// Special const access to second element.
	const real& y() const
	{
		return this->elems[1];
	}
	/// Special const access to third element.
	const real& z() const
	{
		return this->elems[2];
	}
	/// Special const access to fourth element.
	const real& w() const
	{
		return this->elems[3];
	}

	/// Assignment operator from an array of values.
	template<typename real2>
	void operator=(const real2* p)
	{
		//std::copy(p, p+N, this->begin());
		for(int i=0;i<N;i++)
			this->elems[i] = (real)p[i];
	}

	/// Assignment from a vector with different dimensions.
	template<int M, typename real2>
	void operator=(const Vec<M,real2>& v)
	{
		//std::copy(v.begin(), v.begin()+(N>M?M:N), this->begin());
		for(int i=0;i<(N>M?M:N);i++)
			this->elems[i] = (real)v(i);
	}

	/// Sets every element to 0.
	void clear()
	{
		this->assign(real());
	}

	/// Sets every element to r.
	void fill(real r)
	{
		this->assign(r);
	}

	/// Access to i-th element.
	real& operator()(int i)
	{
		return this->elems[i];
	}

	/// Const access to i-th element.
	const real& operator()(int i) const
	{
		return this->elems[i];
	}

	/// Cast into a const array of values.
	const real* ptr() const
	{
		return this->elems;
	}

	/// Cast into an array of values.
	real* ptr()
	{
		return this->elems;
	}

	/// Multiplication by a scalar f.
	template<class real2>
	Vec<N,real> operator*(real2 f) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i] = this->elems[i]*(real)f;
		return r;
	}

	/// On-place multiplication by a scalar f.
	template<class real2>
	void operator*=(real2 f)
	{
		for (int i=0;i<N;i++)
			this->elems[i]*=(real)f;
	}

	/// Division by a scalar f.
	template<class real2>
	Vec<N,real> operator/(real2 f) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i] = this->elems[i]/(real)f;
		return r;
	}

	/// On-place division by a scalar f.
	template<class real2>
	void operator/=(real2 f)
	{
		for (int i=0;i<N;i++)
			this->elems[i]/=(real)f;
	}

	/// Dot product.
	template<class real2>
	real operator*(const Vec<N,real2>& v) const
	{
		real r = (real)(this->elems[0]*v[0]);
		for (int i=1;i<N;i++)
			r += (real)(this->elems[i]*v[i]);
		return r;
	}

	/// linear product.
	template<class real2>
	Vec<N,real> linearProduct(const Vec<N,real2>& v) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i]=this->elems[i]*(real)v[i];
		return r;
	}

	/// linear division.
	template<class real2>
	Vec<N,real> linearDivision(const Vec<N,real2>& v) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i]=this->elems[i]/(real)v[i];
		return r;
	}

	/// Vector addition.
	template<class real2>
	Vec<N,real> operator+(const Vec<N,real2>& v) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i]=this->elems[i]+(real)v[i];
		return r;
	}
	
	/// On-place vector addition.
	template<class real2>
	void operator+=(const Vec<N,real2>& v)
	{
		for (int i=0;i<N;i++)
			this->elems[i]+=(real)v[i];
	}

	/// Vector subtraction.
	template<class real2>
	Vec<N,real> operator-(const Vec<N,real2>& v) const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i]=this->elems[i]-(real)v[i];
		return r;
	}

	/// On-place vector subtraction.
	template<class real2>
	void operator-=(const Vec<N,real2>& v)
	{
		for (int i=0;i<N;i++)
			this->elems[i]-=(real)v[i];
	}

	/// Vector negation.
	Vec<N,real> operator-() const
	{
		Vec<N,real> r(NOINIT);
		for (int i=0;i<N;i++)
			r[i]=-this->elems[i];
		return r;
	}

	/// Squared norm.
	real norm2() const
	{
		real r = this->elems[0]*this->elems[0];
		for (int i=1;i<N;i++)
			r += this->elems[i]*this->elems[i];
		return r;
	}

	/// Euclidean norm.
	real norm() const
	{
		return sqrt(norm2());
	}

	/// Normalize the vector.
	void normalize()
	{
		real r = norm();
		if (r>1e-10)
			for (int i=0;i<N;i++)
				this->elems[i]/=r;
	}

	Vec cross( const Vec& b ) const
	{
		return Vec(
			(*this)[1]*b[2] - (*this)[2]*b[1],
			(*this)[2]*b[0] - (*this)[0]*b[2],
			(*this)[0]*b[1] - (*this)[1]*b[0]
		);
	}

	/// sum of all elements of the vector
	real sum() const
	{
		real sum = 0.0;
		for (int i=0;i<N;i++)
			sum += this->elems[i];
		return sum;
	}

	/// @name Tests operators
	/// @{

	bool operator==(const Vec& b) const
	{
		for (int i=0;i<N;i++)
			if ( fabs( (float)(this->elems[i] - b[i]) ) > EQUALITY_THRESHOLD ) return false;
		return true;
	}

	bool operator!=(const Vec& b) const
	{
		for (int i=0;i<N;i++)
			if ( fabs( (float)(this->elems[i] - b[i]) ) > EQUALITY_THRESHOLD ) return true;
		return false;
	}
};

/// Same as Vec except the values are not initialized by default
template <int N, typename real=float>
class VecNoInit : public Vec<N,real>
{
public:
	VecNoInit() : Vec<N,real>(NOINIT)
	{
	}

	/// Assignment operator from an array of values.
	template<typename real2>
	void operator=(const real2* p)
	{
		this->Vec<N,real>::operator=(p);
	}

	/// Assignment from a vector with different dimensions.
	template<int M, typename real2>
	void operator=(const Vec<M,real2>& v)
	{
		this->Vec<N,real>::operator=(v);
	}

	/// Scalar vector multiplication operator.
	friend Vec<N,real> operator*(real r, const Vec<N,real>& v)
	{
		return v*r;
	}
};

/// Cross product for 3-elements vectors.
template<typename real>
inline Vec<3,real> cross(const Vec<3,real>& a, const Vec<3,real>& b)
{
	return Vec<3,real>(a.y()*b.z() - a.z()*b.y(),
		a.z()*b.x() - a.x()*b.z(),
		a.x()*b.y() - a.y()*b.x());
}

/// Cross product for 2-elements vectors.
template <typename real>
real cross(const Vec<2,real>& a, const Vec<2,real>& b )
{
	return a[0]*b[1] - a[1]*b[0];
}

/// Dot product (alias for operator*)
template<int N,typename real>
inline real dot(const Vec<N,real>& a, const Vec<N,real>& b)
{
	return a*b;
}

typedef Vec<1,float> Vec1f;
typedef Vec<1,double> Vec1d;


typedef Vec<2,float> Vec2f;
typedef Vec<2,double> Vec2d;
typedef Vec<2,int> Vec2i;


typedef Vec<3,float> Vec3f;
typedef Vec<3,double> Vec3d;
typedef Vec<3,int> Vec3i;


typedef Vec<4,float> Vec4f;
typedef Vec<4,double> Vec4d;
typedef Vec<4,int> Vec4i;


typedef Vec<6,float> Vec6f;
typedef Vec<6,double> Vec6d;

typedef Vec<6,float> Vec6f;
typedef Vec<6,double> Vec6d;
typedef Vec<6,int> Vec6i;

#endif
