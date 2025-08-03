#ifndef MING_BASE_ARRAY_H
#define MING_BASE_ARRAY_H

#include <base\base_base.h>
#include <base/base_assert.h>

namespace base
{


/** 
 * Flexible array. Arrays are meant to be used
 * as part of higher abstraction level objects so 
 * they don't have functionality for reference counting.
 *
 * Note that Array has important semantic difference to for
 * example std::vector: Array does NOT call destructors when
 * array is resized to smaller one, but it uses assign operator
 * with default (empty) array element. This behaviour
 * might seem weird, but it is more often useful than not, since
 * for example you don't want memory to be released too easily 
 * if you have for example Array of Arrays. And you shouldn't
 * use too big objects in flexible arrays anyway so in practice
 * this shouldn't harm you in any case.
 *
 * @param T Type of array element. Copy and default ctor should be nothrow.
 * 
 * @ingroup base
 */

template <class T> class  Array
{
public:
	/** 
	 * Creates an empty array. 
	 */
	Array() :
		m_a(0), m_size(0), m_cap(0)
	{
	}

	/** 
	 * Creates an array of specified size. 
	 * @exception OutOfMemoryException
	 */
	explicit Array( int size, const T& defaultvalue=T() ) :
		m_a(0), m_size(0), m_cap(0)
	{
		resize( size, defaultvalue );
	}

	/** 
	 * Copy by value. 
	 */
	Array( const Array<T>& other ) :
		m_a(0), m_size(0), m_cap(0)
	{
		*this = other;
	}

	///
	~Array()
	{
		
		delete[] m_a;
		
	}

	/**
	 * Swaps two arrays.
	 */
	void swap( Array<T>& other );

	/** 
	 * Copy by value. 
	 * @exception OutOfMemoryException
	 */
	Array<T>& operator=( const Array<T>& other );

	/** 
	 * Returns ith element from the array. 
	 */
	inline T& operator[]( int index )
	{
		assert( index >= 0 && index < m_size );
		return m_a[index];
	}

	/** 
	 * Adds an element to the end of the array. 
	 * @exception OutOfMemoryException
	 */
	void add( const T& item );

	/** 
	 * Adds an element before specified index. 
	 * @exception OutOfMemoryException
	 */
	void add( int index, const T& item );

	/**
	 * Removes an element at specified index.
	 */
	void remove( int index );

	/**
	 * Removes range of elements at specified index.
	 * @param begin First element to remove.
	 * @param end One beyond the last element to remove.
	 */
	void remove( int begin, int end );

	/** 
	 * Sets number of elements in the array. 
	 * @exception OutOfMemoryException
	 */
	void resize( int size, const T& defaultvalue=T() );

	/** 
	 * Sets number of elements in the array to 0. 
	 */
	void clear()
	{
		resize( 0 );
	}

	/** 
	 * Returns pointer to the beginning of the array (inclusive). 
	 */
	inline T* begin()
	{
		return m_a;
	}

	/** 
	 * Returns pointer to the end of the array (exclusive). 
	 */
	inline T* end()
	{
		return m_a+m_size;
	}

	/** 
	 * Returns first element in the array (inclusive).
	 */
	inline T& first()
	{
		assert( m_size > 0 );
		return *m_a;
	}

	/** 
	 * Returns last element in the array (inclusive).
	 */
	inline T& last()
	{
		assert( m_size > 0 );
		return m_a[m_size-1];
	}
	
	/** 
	 * Returns ith element from the array. 
	 */
	inline const T& operator[]( int index ) const
	{
		assert( index >= 0 && index < m_size );
		return m_a[index];
	}

	/** 
	 * Returns number of elements in the array. 
	 */
	inline int size() const
	{
		return m_size;
	}

	/** 
	 * Returns pointer to the beginning of the array (inclusive).
	 */
	inline const T* begin() const
	{
		return m_a;
	}

	/** 
	 * Returns pointer to the end of the array (exclusive).
	 */
	inline const T* end() const
	{
		return m_a+m_size;
	}

	/** 
	 * Returns first element in the array (inclusive).
	 */
	inline const T& first() const
	{
		assert( m_size > 0 );
		return *m_a;
	}

	/** 
	 * Returns last element in the array (inclusive).
	 */
	inline const T& last() const
	{
		assert( m_size > 0 );
		return m_a[m_size-1];
	}

	/**
	 * Returns index of the item in the array or -1 if not found.
	 */
	int indexOf( const T& item ) const;

	int addToSorted(const T& item)
	{
		int left = 0, right = size(); 
		while (left != right)
		{
			int mid = (left + right) / 2;
			const T& midValue = (*this)[mid];
			if (item == midValue)
			{
				right = mid + 1;
				break;
			}
			if (item < midValue)
				right = mid;
			else
				left = mid + 1;
		}
		add(right, item);
		return right;
	}

	int indexOfSorted(const T& item) const
	{
		int left = 0, right = size(); 
		while (left != right)
		{
			int mid = (left + right) / 2;
			const T& midValue = (*this)[mid];
			if (item == midValue)
				return mid;
			if (item < midValue)
				right = mid;
			else
				left = mid + 1;
		}
		return -1;
	}

private:
	T*		m_a;
	int		m_size;
	int		m_cap;

	/**
	 * Allocates buffer for n elements. 
	 * @exception OutOfMemoryException
	 */
	void realloc( int size);

	/** 
	 * Copy in normal order.
	 */
	void copy( T* dst, const T* src, int count );

	/** 
	 * Copy in reverse order.
	 */
	void rcopy( T* dst, const T* src, int count );
};


#include <base/base_Array.inl>


} // base


#endif // MING_BASE_ARRAY_H

// MinG
