#ifndef MING_BASE_PTR_H
#define MING_BASE_PTR_H


namespace base
{


/**
 * Smart pointer to an object of class T.
 * T must implement addReference() and release().
 * @see Object
 * 
 * @ingroup base
 */
template <class T> class SmartPtr
{
public:
	/** Null pointer. */
	SmartPtr()													{m_o = 0;}

	/** Releases reference to the object. */
	~SmartPtr()													{if ( m_o ) m_o->release();}

	/** Increments object reference count and stores the pointer to an object. */
	SmartPtr( const SmartPtr<T>& other )								{T* o = other.ptr(); if ( o ) o->addRef(); m_o = o;}

	/** Increments object reference count and stores the pointer to an object. */
	SmartPtr( T* other )											{if ( other ) other->addRef(); m_o = other;}

	/** 
	 * Releases old reference if any, increments other object reference 
	 * count and stores the new reference. 
	 */
	SmartPtr<T>& operator=( const SmartPtr<T>& other )				{T* o = other.ptr(); if ( o ) o->addRef(); if ( m_o ) m_o->release(); m_o = o; return *this;}


	SmartPtr<T>& operator=(  T* o )				{if ( o ) o->addRef(); if ( m_o ) m_o->release(); m_o = o; return *this;}

	//SPtr<T>& operator=( T* o )				{if ( o ) o->addReference(); if ( m_o ) m_o->release(); m_o = o; return *this;}

	/** Return reference to the object. */
	T&		operator*() const								{return *m_o;}

	/** Access member of the object. */
	T*		operator->() const								{return m_o;}

	/** Returns pointer to the object. */
//	operator ==(const SPtr<T>& other) const					{return m_o==other.m_o;}
//	operator <(const SPtr<T>& other) const					{return m_o<other.m_o;}

	operator T*() const										{return m_o;}

	/** Returns pointer to the object. */
	T*		ptr() const										{return m_o;}

private:
	T* m_o;
};


} // base


/** Smart pointer to an object which implements addReference() and release(). */
#define SP( MYCLASS ) base::SmartPtr< MYCLASS >


#define   SMART_POINTER(Class)    \
  typedef  base::SmartPtr<Class> Class##SP;
#endif // MING_BASE_PTR_H

// MinG
