#ifndef EXTVECTYPES_H
#define EXTVECTYPES_H

#include "Vec.h"
#include <iostream>
#include <algorithm>

/// Custom vector allocator class allowing data to be allocated at a specific location (such as for transmission through DMA, PCI-Express, Shared Memory, Network)
template<class T>
class ExtVectorAllocator
{
public:
	typedef T              valueType;
	typedef unsigned int   sizeType;
	virtual ~ExtVectorAllocator() {}
	virtual void resize(valueType*& data, sizeType size, sizeType& maxsize, sizeType& cursize)=0;
	virtual void close(valueType* data)=0;
};

/// Custom vector class.
///
/// This class allows custom buffer allocation while not having any virtual methods using a bridge pattern with ExtVectorAllocator
template<class T>
class ExtVector
{
public:
	typedef T              valueType;
	typedef unsigned int   sizeType;

protected:
	valueType* data;
	sizeType   maxsize;
	sizeType   cursize;
	ExtVectorAllocator<T>* allocator;

public:
	explicit ExtVector(ExtVectorAllocator<T>* alloc = NULL) : data(NULL), maxsize(0), cursize(0), allocator(alloc) {}
	ExtVector(int size, ExtVectorAllocator<T>* alloc) : data(NULL), maxsize(0), cursize(0), allocator(alloc) { resize(size); }
	~ExtVector() { if (allocator) allocator->close(data); }
	void setAllocator(ExtVectorAllocator<T>* alloc)
	{
		if (alloc != allocator)
		{
			if (cursize)
			{
				valueType* oldData = data;
				sizeType size = cursize;
				data = NULL;
				maxsize = 0;
				cursize = 0;
				alloc->resize(data, size, maxsize, cursize);
				std::copy(oldData, oldData+size, data);
				if (allocator)
					allocator->close(oldData);
			}
			allocator = alloc;
		}
	}
	void setData(valueType* d, sizeType s) { data=d; maxsize=s; cursize=s; }
	T* getData() { return this->data; }
	const T* getData() const { return this->data; }

	valueType& operator[](sizeType i) { return data[i]; }
	const valueType& operator[](sizeType i) const { return data[i]; }
	sizeType size() const { return cursize; }
	bool empty() const { return cursize==0; }
	void reserve(sizeType size)
	{
		if (size <= maxsize)
			return;
		sizeType temp = cursize;
		if (allocator)
			allocator->resize(data, size, maxsize, temp);
		else
		{
			std::cerr << "Error: invalid reserve request ("<<size<<">"<<maxsize<<") on external vector without allocator.\n";
		}
	}
	void resize(sizeType size)
	{
		if (size <= maxsize)
			cursize = size;
		else if (allocator)
			allocator->resize(data, size, maxsize, cursize);
		else
		{
			cursize = maxsize;
			std::cerr << "Error: invalid resize request ("<<size<<">"<<maxsize<<") on external vector without allocator.\n";
		}
	}
	void clear()
	{
		resize(0);
	}
	void pushBack(const T& v)
	{
		int i = this->size();
		resize(i+1);
		(*this)[i] = v;
	}
	T* begin() { return getData(); }
	const T* begin() const { return getData(); }
	T* end() { return getData()+size(); }
	const T* end() const { return getData()+size(); }
};

template<class T>
class DefaultAllocator : public ExtVectorAllocator<T>
{
public:
	typedef typename ExtVectorAllocator<T>::valueType valueType;
	typedef typename ExtVectorAllocator<T>::sizeType sizeType;
	virtual void close(valueType* data)
	{
		if (data!=NULL) delete[] data;
		delete this;
	}
	virtual void resize(valueType*& data, sizeType size, sizeType& maxsize, sizeType& cursize)
	{
		if (size > maxsize)
		{
			T* oldData = data;
			maxsize = (size > 2*maxsize ? size : 2*maxsize);
			data = new T[maxsize];
			if (cursize)
			{
				for (sizeType i = 0 ; i < cursize ; ++i)
					data[i] = oldData[i];
			}
			if (oldData!=NULL) delete[] oldData;
		}
		cursize = size;
	}
};

/// Resizable custom vector class using DefaultAllocator
template<class T>
class ResizableExtVector : public ExtVector<T>
{
public:
	typedef typename ExtVector<T>::valueType value_type;
	typedef typename ExtVector<T>::sizeType size_type;
	ResizableExtVector()
		: ExtVector<T>(new DefaultAllocator<T>)
	{
	}
};

#endif