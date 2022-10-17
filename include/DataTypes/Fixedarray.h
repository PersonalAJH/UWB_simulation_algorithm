/********************************************	
*		Original source code: SOFA			*
*		Modified by Hoeryong Jung			*
*		Date: 2009. 10. 21					*
*		contact: junghl80@kaist.ac.kr		*
/*******************************************/

#ifndef FIXEDARRAY_H
#define FIXEDARRAY_H

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <math.h>

template<class T, unsigned int N>
class FixedArray
{
public:
	T elems[N];

public:
	// type definitions
	typedef T              valueType;
	typedef T*             iterator;
	typedef const T*       constIterator;
	typedef T&             reference;
	typedef const T&       constReference;
	typedef unsigned int   sizeType;
	typedef std::ptrdiff_t differenceType;

	FixedArray()
	{
	}

	/// Specific constructor for 1-element vectors.
	explicit FixedArray(valueType r1)
	{
		this->elems[0]=r1;
	}

	/// Specific constructor for 2-elements vectors.
	FixedArray(valueType r1, valueType r2)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
	}

	/// Specific constructor for 3-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
	}

	/// Specific constructor for 4-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
	}

	/// Specific constructor for 5-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
	}

	/// Specific constructor for 6-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5, valueType r6)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
		this->elems[5]=r6;
	}

	/// Specific constructor for 7-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5, valueType r6, valueType r7)
	{
		this->elems[0]=r1;
		this->elems[1]=r2;
		this->elems[2]=r3;
		this->elems[3]=r4;
		this->elems[4]=r5;
		this->elems[5]=r6;
		this->elems[6]=r7;
	}

	/// Specific constructor for 8-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5, valueType r6, valueType r7, valueType r8)
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
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5, valueType r6, valueType r7, valueType r8, valueType r9)
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

	/// Specific constructor for 10-elements vectors.
	FixedArray(valueType r1, valueType r2, valueType r3, valueType r4, valueType r5, valueType r6, valueType r7, valueType r8, valueType r9, valueType r10)
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
		this->elems[9]=r10;
	}

	// iterator support
	iterator begin()
	{
		return elems;
	}
	constIterator begin() const
	{
		return elems;
	}
	iterator end()
	{
		return elems+N;
	}
	constIterator end() const
	{
		return elems+N;
	}
	
	// operator[]
	reference operator[](sizeType i)
	{
		return elems[i];
	}
	constReference operator[](sizeType i) const
	{
		return elems[i];
	}

	// at() with range check
	reference at(sizeType i)
	{
		rangecheck(i);
		return elems[i];
	}
	constReference at(sizeType i) const
	{
		rangecheck(i);
		return elems[i];
	}

	// front() and back()
	reference front()
	{
		return elems[0];
	}
	constReference front() const
	{
		return elems[0];
	}
	reference back()
	{
		return elems[N-1];
	}
	constReference back() const
	{
		return elems[N-1];
	}

	// size is constant
	static sizeType size()
	{
		return N;
	}
	static bool empty()
	{
		return false;
	}
	static sizeType maxSize()
	{
		return N;
	}

	// swap (note: linear complexity)
	void swap (FixedArray<T,N>& y)
	{
		std::swap_ranges(begin(),end(),y.begin());
	}

	// direct access to data
	const T* data() const
	{
		return elems;
	}

	// assignment with type conversion
	template <typename T2>
	FixedArray<T,N>& operator= (const FixedArray<T2,N>& rhs)
	{
		for (sizeType i=0;i<N;i++)
			elems[i] = (T)rhs[i];
		return *this;
	}

	// assign one value to all elements
	void assign (const T& value)
	{
		for (sizeType i=0;i<N;i++)
			elems[i] = value;
	}

	inline friend std::ostream& operator << (std::ostream& out, const FixedArray<T,N>& a)
	{
		for( sizeType i=0; i<N; i++ )
			out<<a.elems[i]<<" ";
		return out;
	}

	inline friend std::istream& operator >> (std::istream& in, FixedArray<T,N>& a)
	{
		for( sizeType i=0; i<N; i++ )
			in>>a.elems[i];
		return in;
	}

	inline bool operator < (const FixedArray& v ) const {
		for( sizeType i=0; i<N; i++ ){
			if( elems[i]<v[i] )
				return true;  // (*this)<v
			else if( elems[i]>v[i] )
				return false; // (*this)>v
		}
		return false; // (*this)==v
	}

private:

	// check range (may be private because it is static)
	static void rangecheck (sizeType i)
	{
		if (i >= size())
		{
			throw std::range_error("FixedArray");
		}
	}
};

template<class T>
inline FixedArray<T, 2> makeArray(const T& v0, const T& v1)
{
	FixedArray<T, 2> v;
	v[0] = v0;
	v[1] = v1;
	return v;
}

template<class T>
inline FixedArray<T, 3> makeArray(const T& v0, const T& v1, const T& v2)
{
	FixedArray<T, 3> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	return v;
}

template<class T>
inline FixedArray<T, 4> makeArray(const T& v0, const T& v1, const T& v2, const T& v3)
{
	FixedArray<T, 4> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	return v;
}

template<class T>
inline FixedArray<T, 5> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4)
{
	FixedArray<T, 5> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	return v;
}

template<class T>
inline FixedArray<T, 6> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4, const T& v5)
{
	FixedArray<T, 6> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	v[5] = v5;
	return v;
}

template<class T>
inline FixedArray<T, 7> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6)
{
	FixedArray<T, 7> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	v[5] = v5;
	v[6] = v6;
	return v;
}

template<class T>
inline FixedArray<T, 8> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7)
{
	FixedArray<T, 8> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	v[5] = v5;
	v[6] = v6;
	v[7] = v7;
	return v;
}

template<class T>
inline FixedArray<T, 9> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8)
{
	FixedArray<T, 9> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	v[5] = v5;
	v[6] = v6;
	v[7] = v7;
	v[8] = v8;
	return v;
}

template<class T>
inline FixedArray<T, 10> makeArray(const T& v0, const T& v1, const T& v2, const T& v3, const T& v4, const T& v5, const T& v6, const T& v7, const T& v8, const T& v9)
{
	FixedArray<T, 10> v;
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
	v[4] = v4;
	v[5] = v5;
	v[6] = v6;
	v[7] = v7;
	v[8] = v8;
	v[9] = v9;
	return v;
}

#endif