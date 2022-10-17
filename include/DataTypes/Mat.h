#ifndef MAT_H
#define MAT_H

#include "Vec.h"
#include <assert.h>
#include <iostream>

template <int R, int C, class real=float>
class Mat : public FixedArray<VecNoInit<C,real>,R>
{
public:
	enum { N = R*C };

	typedef real Real;
	typedef Vec<C,real> Row;
	typedef VecNoInit<C,real> RowNoInit;
	typedef Vec<R,real> Col;

	Mat()
	{
		clear();
	}

	explicit Mat(NoInit)
	{
	}

	/// Specific constructor with 2 rows.
	Mat(Row r1, Row r2)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
	}

	/// Specific constructor with 3 rows.
	Mat(Row r1, Row r2, Row r3)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
	}

	/// Specific constructor with 4 rows.
	Mat(Row r1, Row r2, Row r3, Row r4)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
	}

	/// Constructor from an element
	explicit Mat(const real& v)
	{
		for( int i=0; i<R; i++ )
			for( int j=0; j<C; j++ )
				this->elems[i][j] = v;
	}

	/// Constructor from another matrix
	template<typename real2>
	Mat(const Mat<R,C,real2>& m)
	{
		std::copy(m.begin(), m.begin()+R, this->begin());
	}

	/// Constructor from an array of elements (stored per line).
	template<typename real2>
	explicit Mat(const real2* p)
	{
		std::copy(p, p+N, this->begin()->begin());
	}

	/// number of rows
	int getNbRows() const
	{
		return R;
	}

	/// number of colums
	int getNbCols() const
	{
		return C;
	}

	/// Sets each element to 0.
	void clear()
	{
		for (int i=0;i<R;i++)
			this->elems[i].clear();
	}

	/// Assignment from an array of elements (stored per line).
	void operator=(const real* p)
	{
		std::copy(p, p+N, this->begin()->begin());
	}

	/// Assignment from another matrix
	template<typename real2> void operator=(const Mat<R,C,real2>& m)
	{
		std::copy(m.begin(), m.begin()+R, this->begin());
	}

	/// Assignment from a matrix of different size.
	template<int R2, int C2> void operator=(const Mat<R2,C2,real>& m)
	{
		std::copy(m.begin(), m.begin()+(R>R2?R2:R), this->begin());
	}

	template<int R2, int C2> void getsub(int R0, int C0, Mat<R2,C2,real>& m) const
	{
		for (int i=0;i<R2;i++)
			for (int j=0;j<C2;j++)
				m[i][j] = this->elems[i+R0][j+C0];
	}

	template<int R2, int C2> void setsub(int R0, int C0, const Mat<R2,C2,real>& m)
	{
		for (int i=0;i<R2;i++)
			for (int j=0;j<C2;j++)
				this->elems[i+R0][j+C0] = m[i][j];
	}

	/// Sets each element to r.
	void fill(real r)
	{
		for (int i=0;i<R;i++)
			this->elems[i].fill(r);
	}

	void setRotationMatrix(Vec3d& axis, Real angle)
	{
		if((R==3)&&(C==3))
		{
			double x,y,z;
			x=axis[0];
			y=axis[1];
			z=axis[2];

			(*this)(0,0)=x*x+(y*y+z*z)*cos(angle);
			(*this)(1,1)=y*y+(x*x+z*z)*cos(angle);
			(*this)(2,2)=z*z+(x*x+y*y)*cos(angle);
			(*this)(0,1)=(1-cos(angle))*x*y+z*sin(angle);
			(*this)(1,0)=(1-cos(angle))*x*y-z*sin(angle);
			(*this)(0,2)=(1-cos(angle))*x*z-y*sin(angle);
			(*this)(2,0)=(1-cos(angle))*z*x+y*sin(angle);
			(*this)(1,2)=(1-cos(angle))*y*z+x*sin(angle);
			(*this)(2,1)=(1-cos(angle))*z*y-x*sin(angle);
		}
		else
			return;
	}

	/// Read-only access to row i.
	const Row& row(int i) const
	{
		return this->elems[i];
	}

	/// Copy of column j.
	Col col(int j) const
	{
		Col c;
		for (int i=0;i<R;i++)
			c[i]=this->elems[i][j];
		return c;
	}

	/// Write access to row i.
	RowNoInit& operator[](int i)
	{
		return this->elems[i];
	}

	/// Read-only access to row i.
	const RowNoInit& operator[](int i) const
	{
		return this->elems[i];
	}

	/// Write acess to row i.
	RowNoInit& operator()(int i)
	{
		return this->elems[i];
	}

	/// Read-only access to row i.
	const RowNoInit& operator()(int i) const
	{
		return this->elems[i];
	}

	/// Write access to element (i,j).
	real& operator()(int i, int j)
	{
		return this->elems[i][j];
	}

	/// Read-only access to element (i,j).
	const real& operator()(int i, int j) const
	{
		return this->elems[i][j];
	}

	/// Cast into a standard C array of lines (read-only).
	const Row* lptr() const
	{
		return this->elems;
	}

	/// Cast into a standard C array of lines.
	Row* lptr()
	{
		return this->elems;
	}

	/// Cast into a standard C array of elements (stored per line) (read-only).
	const real* ptr() const
	{
		return this->elems[0].ptr();;
	}

	/// Cast into a standard C array of elements (stored per line).
	real* ptr()
	{
		return this->elems[0].ptr();
	}

	/// Special access to first line.
	Row& x() { return this->elems[0]; }
	/// Special access to second line.
	Row& y() { return this->elems[1]; }
	/// Special access to third line.
	Row& z() { return this->elems[2]; }
	/// Special access to fourth line.
	Row& w() { return this->elems[3]; }

	/// Special access to first line (read-only).
	const Row& x() const { return this->elems[0]; }
	/// Special access to second line (read-only).
	const Row& y() const { return this->elems[1]; }
	/// Special access to thrid line (read-only).
	const Row& z() const { return this->elems[2]; }
	/// Special access to fourth line (read-only).
	const Row& w() const { return this->elems[3]; }

	/// Set matrix to identity.
	void identity()
	{
		clear();
		for (int i=0;i<R;i++)
			this->elems[i][i]=1;
	}

	/// Set matrix as the transpose of m.
	void transpose(const Mat<C,R,real> &m)
	{
		for (int i=0;i<R;i++)
			for (int j=0;j<C;j++)
				this->elems[i][j]=m[j][i];
	}

	/// Return the transpose of m.
	Mat<C,R,real> transposed() const
	{
		Mat<C,R,real> m(NOINIT);
		for (int i=0;i<R;i++)
			for (int j=0;j<C;j++)
				m[j][i]=this->elems[i][j];
		return m;
	}

	/// Transpose current matrix.
	void transpose()
	{ 
	for (int i=0;i<R;i++)
		for (int j=i+1;j<C;j++)
		{
			real t = this->elems[i][j];
			this->elems[i][j] = this->elems[j][i];
			this->elems[j][i] = t;
		}
	}

	bool operator==(const Mat<R,C,real>& b) const
	{
		for (int i=0;i<R;i++)
			if (!(this->elems[i]==b[i])) return false;
		return true;
	}

	bool operator!=(const Mat<R,C,real>& b) const
	{
		for (int i=0;i<L;i++)
			if (this->elems[i]!=b[i]) return true;
		return false;
	}


	bool isSymetric() const
	{
		for (int i=0;i<R;i++)
			for (int j=i+1;j<C;j++)
				if( fabs( this->elems[i][j] - this->elems[j][i] ) > EQUALITY_THRESHOLD ) return false;
		return true;
	}

	// LINEAR ALGEBRA

	/// Matrix multiplication operator.
	template <int P>
	Mat<R,P,real> operator*(const Mat<C,P,real>& m) const
	{
		Mat<R,P,real> r(NOINIT);
		for(int i=0;i<R;i++)
			for(int j=0;j<P;j++)
			{
				r[i][j]=(*this)[i][0] * m[0][j];
				for(int k=1;k<C;k++)
					r[i][j] += (*this)[i][k] * m[k][j];
			}
			return r;
	}

	/// Matrix addition operator.
	Mat<R,C,real> operator+(const Mat<R,C,real>& m) const
	{
		Mat<R,C,real> r(NOINIT);
		for(int i = 0; i < R; i++)
			r[i] = (*this)[i] + m[i];
		return r;
	}

	/// Matrix subtraction operator.
	Mat<R,C,real> operator-(const Mat<R,C,real>& m) const
	{
		Mat<R,C,real> r(NOINIT);
		for(int i = 0; i < R; i++)
			r[i] = (*this)[i] - m[i];
		return r;
	}

	/// Multiplication operator Matrix * Line.
	Col operator*(const Col& v) const
	{
		Col r(NOINIT);
		for(int i=0;i<R;i++)
		{
			r[i]=(*this)[i][0] * v[0];
			for(int j=1;j<C;j++)
				r[i] += (*this)[i][j] * v[j];
		}
		return r;
	}

	/// Multiplication of the transposed Matrix * Column
	Row multTranspose(const Col& v) const
	{
		Row r(NOINIT);
		for(int i=0;i<C;i++)
		{
			r[i]=(*this)[0][i] * v[0];
			for(int j=1;j<R;j++)
				r[i] += (*this)[j][i] * v[j];
		}
		return r;
	}


	/// Transposed Matrix multiplication operator.
	template <int P>
	Mat<C,P,real> multTranspose(const Mat<R,P,real>& m) const
	{
		Mat<C,P,real> r(NOINIT);
		for(int i=0;i<C;i++)
			for(int j=0;j<P;j++)
			{
				r[i][j]=(*this)[0][i] * m[0][j];
				for(int k=1;k<R;k++)
					r[i][j] += (*this)[k][i] * m[k][j];
			}
			return r;
	}


	/// Scalar multiplication operator.
	Mat<R,C,real> operator*(real f) const
	{
		Mat<R,C,real> r(NOINIT);
		for(int i=0;i<R;i++)
			for(int j=0;j<C;j++)
				r[i][j] = (*this)[i][j] * f;
		return r;
	}

	/// Scalar matrix multiplication operator.
	friend Mat<R,C,real> operator*(real r, const Mat<R,C,real>& m)
	{
		return m*r;
	}

	/// Scalar division operator.
	Mat<R,C,real> operator/(real f) const
	{
		Mat<R,C,real> r(NOINIT);
		for(int i=0;i<R;i++)
			for(int j=0;j<C;j++)
				r[i][j] = (*this)[i][j] / f;
		return r;
	}

	/// Scalar multiplication assignment operator.
	void operator *=(real r)
	{
		for(int i=0;i<R;i++)
			this->elems[i]*=r;
	}

	/// Scalar division assignment operator.
	void operator /=(real r)
	{
		for(int i=0;i<R;i++)
			this->elems[i]/=r;
	}

	/// Addition assignment operator.
	void operator +=(const Mat<R,C,real>& m)
	{
		for(int i=0;i<R;i++)
			this->elems[i]+=m[i];
	}

	/// Substraction assignment operator.
	void operator -=(const Mat<R,C,real>& m)
	{
		for(int i=0;i<R;i++)
			this->elems[i]-=m[i];
	}

	/// Addition of the transposed of m (possible for square matrices only)
	void addTransposed(const Mat<R,C,real>& m)
	{
		for(int i=0;i<R;i++)
			for(int j=0;j<R;j++)
				(*this)[i][j] += m[j][i];
	}

	/// Substraction of the transposed of m (possible for square matrices only)
	void subTransposed(const Mat<R,C,real>& m)
	{
		for(int i=0;i<R;i++)
			for(int j=0;j<R;j++)
				(*this)[i][j] += m[j][i];
	}

	/// Invert matrix m
	bool invert(const Mat<R,C,real>& m)
	{
		return invertMatrix(*this, m);
	}

	static Mat<R,C,real> transformTranslation(const Vec<C-1,real>& t)
	{
		Mat<R,C,real> m;
		m.identity();
		for (int i=0;i<C-1;++i)
			m.elems[i][C-1] = t[i];
		return m;
	}

	static Mat<R,C,real> transformScale(real s)
	{
		Mat<R,C,real> m;
		m.identity();
		for (int i=0;i<C-1;++i)
			m.elems[i][i] = s;
		return m;
	}

	static Mat<R,C,real> transformScale(const Vec<C-1,real>& s)
	{
		Mat<R,C,real> m;
		m.identity();
		for (int i=0;i<C-1;++i)
			m.elems[i][i] = s[i];
		return m;
	}

/*	template<class Quat>
	static Mat<L,C,real> transformRotation(const Quat& q)
	{
		Mat<L,C,real> m;
		m.identity();
		q.toMatrix(m);
		return m;
	}*/

	/// Multiplication operator Matrix * Vector considering the matrix as a transformation.
	Vec<C-1,real> transform(const Vec<C-1,real>& v) const
	{
		Vec<C-1,real> r(NOINIT);
		for(int i=0;i<C-1;i++)
		{
			r[i]=(*this)[i][0] * v[0];
			for(int j=1;j<C-1;j++)
				r[i] += (*this)[i][j] * v[j];
			r[i] += (*this)[i][C-1];
		}
		return r;
	}
};

/// Same as Mat except the values are not initialized by default
template <int R, int C, typename real=float>
class MatNoInit : public Mat<R,C,real>
{
public:
	MatNoInit()
		: Mat<R,C,real>(NOINIT)
	{
	}

	/// Assignment from an array of elements (stored per line).
	void operator=(const real* p)
	{
		this->Mat<R,C,real>::operator=(p);
	}

	/// Assignment from another matrix
	template<int R2, int C2, typename real2> void operator=(const Mat<R2,C2,real2>& m)
	{
		this->Mat<R,C,real>::operator=(m);
	}
};

/// Determinant of a 3x3 matrix.
template<class real>
inline real determinant(const Mat<3,3,real>& m)
{
	return m(0,0)*m(1,1)*m(2,2)
		+ m(1,0)*m(2,1)*m(0,2)
		+ m(2,0)*m(0,1)*m(1,2)
		- m(0,0)*m(2,1)*m(1,2)
		- m(1,0)*m(0,1)*m(2,2)
		- m(2,0)*m(1,1)*m(0,2);
}

/// Determinant of a 2x2 matrix.
template<class real>
inline real determinant(const Mat<2,2,real>& m)
{
	return m(0,0)*m(1,1)
		- m(1,0)*m(0,1);
}

#define MIN_DETERMINANT  1.0e-100

/// Matrix inversion (general case).
template<int S, class real>
bool invertMatrix(Mat<S,S,real>& dest, const Mat<S,S,real>& from)
{
	int i, j, k;
	Vec<S,int> r, c, row, col;

	Mat<S,S,real> m1 = from;
	Mat<S,S,real> m2;
	m2.identity();

	for ( k = 0; k < S; k++ )
	{
		// Choosing the pivot
		real pivot = 0;
		for (i = 0; i < S; i++)
		{
			if (row[i])
				continue;
			for (j = 0; j < S; j++)
			{
				if (col[j])
					continue;
				real t = m1[i][j]; if (t<0) t=-t;
				if ( t > pivot)
				{
					pivot = t;
					r[k] = i;
					c[k] = j;
				}
			}
		}

		if (pivot <= (real) MIN_DETERMINANT)
		{
			printf("Warning: invertMatrix finds too small determinant");
			return false;
		}

		row[r[k]] = col[c[k]] = 1;
		pivot = m1[r[k]][c[k]];

		// Normalization
		m1[r[k]] /= pivot; m1[r[k]][c[k]] = 1;
		m2[r[k]] /= pivot;

		// Reduction
		for (i = 0; i < S; i++)
		{
			if (i != r[k])
			{
				real f = m1[i][c[k]];
				m1[i] -= m1[r[k]]*f; m1[i][c[k]] = 0;
				m2[i] -= m2[r[k]]*f;
			}
		}
	}

	for (i = 0; i < S; i++)
		for (j = 0; j < S; j++)
			if (c[j] == i)
				row[i] = r[j];

	for ( i = 0; i < S; i++ )
		dest[i] = m2[row[i]];

	return true;
}

/// Matrix inversion (special case 3x3).
template<class real>
bool invertMatrix(Mat<3,3,real>& dest, const Mat<3,3,real>& from)
{
	real det=determinant(from);

	if ( -(real) MIN_DETERMINANT<=det && det<=(real) MIN_DETERMINANT){
		printf("Warning: invertMatrix finds too small determinant\n");
		return false;
	}

	dest(0,0)= (from(1,1)*from(2,2) - from(2,1)*from(1,2))/det;
	dest(1,0)= (from(1,2)*from(2,0) - from(2,2)*from(1,0))/det;
	dest(2,0)= (from(1,0)*from(2,1) - from(2,0)*from(1,1))/det;
	dest(0,1)= (from(2,1)*from(0,2) - from(0,1)*from(2,2))/det;
	dest(1,1)= (from(2,2)*from(0,0) - from(0,2)*from(2,0))/det;
	dest(2,1)= (from(2,0)*from(0,1) - from(0,0)*from(2,1))/det;
	dest(0,2)= (from(0,1)*from(1,2) - from(1,1)*from(0,2))/det;
	dest(1,2)= (from(0,2)*from(1,0) - from(1,2)*from(0,0))/det;
	dest(2,2)= (from(0,0)*from(1,1) - from(1,0)*from(0,1))/det;

	return true;
}

/// Matrix inversion (special case 2x2).
template<class real>
bool invertMatrix(Mat<2,2,real>& dest, const Mat<2,2,real>& from)
{
	real det=determinant(from);

	if ( -(real) MIN_DETERMINANT<=det && det<=(real) MIN_DETERMINANT){
		printf("Warning: invertMatrix finds too small determinant\n");
		return false;
	}

	dest(0,0)=  from(1,1)/det;
	dest(0,1)= -from(0,1)/det;
	dest(1,0)= -from(1,0)/det;
	dest(1,1)=  from(0,0)/det;

	return true;
}
#undef MIN_DETERMINANT

typedef Mat<1,1,float> Mat1x1f;
typedef Mat<1,1,double> Mat1x1d;

typedef Mat<2,2,float> Mat2x2f;
typedef Mat<2,2,double> Mat2x2d;

typedef Mat<3,3,float> Mat3x3f;
typedef Mat<3,3,double> Mat3x3d;

typedef Mat<3,4,float> Mat3x4f;
typedef Mat<3,4,double> Mat3x4d;

typedef Mat<4,4,float> Mat4x4f;
typedef Mat<4,4,double> Mat4x4d;

/// return the max of two values
template<class T1,class T2>
inline const T1 S_MAX(const T1 &a, const T2 &b)
{
	return b > a ? (b) : (a);
}

/// return the min of two values
template<class T1,class T2>
inline const T1 S_MIN(const T1 &a, const T2 &b)
{
	return b < a ? (b) : (a);
}

template<class T1,class T2>
inline const T1 S_SIGN(const T1 &a, const T2 &b)
{
	return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

template<class T>
inline const T S_SQR(const T a)
{
	return a*a;
}

template<class T>
inline const T SQR(const T& a)
{
	return a*a;
}

///Computes sqrt(aï¿?+ bï¿? without destructive underflow or overflow.
template <class T1, class T2>
T1 pythag(const T1 a, const T2 b)
{
	T1 absa,absb;
	absa=(T1)fabs(a);
	absb=(T1)fabs(b);
	if (absa > absb) return T1(absa*sqrt(1.0+SQR(absb/absa)));
	else return (T1)(absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
}

/// Compute the SVD decomposition of matrix a (from nr). a is replaced by its pivoted LU decomposition. indx stores pivoting indices.
/** SVD decomposition   a = u.w.vt
\pre a: original matrix, destroyed to become u
\pre w: diagonal vector
\pre v: matrix
*/
template< int m, int n, typename Real>
void svddcmp(Mat<m,n,Real> &a, Vec<n,Real> &w, Mat<n,m,Real> &v)
{
	bool flag;
	int i,its,j,jj,k,l,nm;
	Real anorm,c,f,g,h,s,scale,x,y,z;

	Vec<n,Real> rv1;
	g=scale=anorm=0.0;
	for (i=0;i<n;i++) {
		l=i+2;
		rv1[i]=scale*g;
		g=s=scale=0.0;
		if (i < m) {
			for (k=i;k<m;k++) scale += fabs(a[k][i]);
			if (scale != 0.0) {
				for (k=i;k<m;k++) {
					a[k][i] /= scale;
					s += a[k][i]*a[k][i];
				}
				f=a[i][i];
				g = -S_SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][i]=f-g;
				for (j=l-1;j<n;j++) {
					for (s=0.0,k=i;k<m;k++) s += a[k][i]*a[k][j];
					f=s/h;
					for (k=i;k<m;k++) a[k][j] += f*a[k][i];
				}
				for (k=i;k<m;k++) a[k][i] *= scale;
			}
		}
		w[i]=scale *g;
		g=s=scale=0.0;
		if (i+1 <= m && i != n) {
			for (k=l-1;k<n;k++) scale += fabs(a[i][k]);
			if (scale != 0.0) {
				for (k=l-1;k<n;k++) {
					a[i][k] /= scale;
					s += a[i][k]*a[i][k];
				}
				f=a[i][l-1];
				g = -S_SIGN(sqrt(s),f);
				h=f*g-s;
				a[i][l-1]=f-g;
				for (k=l-1;k<n;k++) rv1[k]=a[i][k]/h;
				for (j=l-1;j<m;j++) {
					for (s=0.0,k=l-1;k<n;k++) s += a[j][k]*a[i][k];
					for (k=l-1;k<n;k++) a[j][k] += s*rv1[k];
				}
				for (k=l-1;k<n;k++) a[i][k] *= scale;
			}
		}
		anorm=S_MAX(anorm,(fabs(w[i])+fabs(rv1[i])));
	}
	for (i=n-1;i>=0;i--) {
		if (i < n-1) {
			if (g != 0.0) {
				for (j=l;j<n;j++)
					v[j][i]=(a[i][j]/a[i][l])/g;
				for (j=l;j<n;j++) {
					for (s=0.0,k=l;k<n;k++) s += a[i][k]*v[k][j];
					for (k=l;k<n;k++) v[k][j] += s*v[k][i];
				}
			}
			for (j=l;j<n;j++) v[i][j]=v[j][i]=0.0;
		}
		v[i][i]=1.0;
		g=rv1[i];
		l=i;
	}
	for (i=S_MIN(m,n)-1;i>=0;i--) {
		l=i+1;
		g=w[i];
		for (j=l;j<n;j++) a[i][j]=0.0;
		if (g != 0.0) {
			g=Real(1.0/g);
			for (j=l;j<n;j++) {
				for (s=0.0,k=l;k<m;k++) s += a[k][i]*a[k][j];
				f=(s/a[i][i])*g;
				for (k=i;k<m;k++) a[k][j] += f*a[k][i];
			}
			for (j=i;j<m;j++) a[j][i] *= g;
		} else for (j=i;j<m;j++) a[j][i]=0.0;
		++a[i][i];
	}
	for (k=n-1;k>=0;k--) {
		for (its=0;its<30;its++) {
			flag=true;
			for (l=k;l>=0;l--) {
				nm=l-1;
				if (fabs(rv1[l])+anorm == anorm) {
					flag=false;
					break;
				}
				if (fabs(w[nm])+anorm == anorm) break;
			}
			if (flag) {
				c=0.0;
				s=1.0;
				for (i=l-1;i<k+1;i++) {
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if (fabs(f)+anorm == anorm) break;
					g=w[i];
					h=pythag(f,g);
					w[i]=h;
					h=Real(1.0/h);
					c=g*h;
					s = -f*h;
					for (j=0;j<m;j++) {
						y=a[j][nm];
						z=a[j][i];
						a[j][nm]=y*c+z*s;
						a[j][i]=z*c-y*s;
					}
				}
			}
			z=w[k];
			if (l == k) {
				if (z < 0.0) {
					w[k] = -z;
					for (j=0;j<n;j++) v[j][k] = -v[j][k];
				}
				break;
			}
			if (its == 29)
			{
				// 				std::cerr<<"Warning: Mat.h :: svddcmp: no convergence in 30 svdcmp iterations"<<std::endl;
				return;
			}
			x=w[l];
			nm=k-1;
			y=w[nm];
			g=rv1[nm];
			h=rv1[k];
			f=Real(((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y));
			g=pythag(f,1.0);
			f=((x-z)*(x+z)+h*((y/(f+S_SIGN(g,f)))-h))/x;
			c=s=1.0;
			for (j=l;j<=nm;j++) {
				i=j+1;
				g=rv1[i];
				y=w[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c+g*s;
				g=g*c-x*s;
				h=y*s;
				y *= c;
				for (jj=0;jj<n;jj++) {
					x=v[jj][j];
					z=v[jj][i];
					v[jj][j]=x*c+z*s;
					v[jj][i]=z*c-x*s;
				}
				z=pythag(f,h);
				w[j]=z;
				if (z) {
					z=(Real)1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g+s*y;
				x=c*y-s*g;
				for (jj=0;jj<m;jj++) {
					y=a[jj][j];
					z=a[jj][i];
					a[jj][j]=y*c+z*s;
					a[jj][i]=z*c-y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			w[k]=x;
		}
	}
}

#endif