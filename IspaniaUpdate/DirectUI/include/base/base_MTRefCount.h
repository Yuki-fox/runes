#ifndef MING_BASE_MTREFCOUNT_H
#define MING_BASE_MTREFCOUNT_H

#include <base/base_base.h>
//#include <base/base_pp.h>

#include <base/base_SmartPtr.h>


namespace base
{

class Mutex;
/**
 * Object class has functionality for reference counting.
 * If an Object is created in stack then special care
 * must be taken that the object reference count 
 * is not affected anywhere as it will result in crash
 * when the object goes out of scope in the stack.
 * Note that Object reference count modifying is not 
 * thread safe operation.
 * 
 * @ingroup base
 */
class BASE_API MTRefCount
{

public:
	/* Helper class for synchronizing access to an Object. */
	//class  MutexLock { Mutex* m; MutexLock(); MutexLock( const MutexLock& ); MutexLock& operator=( const MutexLock& ); public: MutexLock( const MTObject& o ); MutexLock( const MTObject* o ); ~MutexLock();};

	enum ObjectFlags
	{
		/** Do not initialize synchronization objects. */
		OBJECT_DEFAULT			= 0,
		/** Initialize synchronization object. */
		OBJECT_INITMUTEX		= 1
	};
	/** 
	 * Initializes reference count to zero. 
	 */
	MTRefCount(int flags=OBJECT_DEFAULT);

	/** 
	 * Initializes reference count to zero. 
	 */
	MTRefCount( const MTRefCount& );

	/** 
	 * Ensures that the reference count is zero. 
	 */
	virtual ~MTRefCount();

	/** 
	 * Does nothing. 
	*/
	MTRefCount&			operator=( const MTRefCount& )		{return *this;}

	void initMutex(void);
	/** 
	 * Increments reference count by one. 
	 * DO NOT USE THIS METHOD ON OBJECTS
	 * WHICH ARE CREATED IN STACK!
	 */
	void			addRef()		;//			{++m_refs;}

	/** 
	 * Decrements reference count and 
	 * uses operator delete to the object when
	 * the count reaches zero. 
	 * DO NOT USE THIS METHOD ON OBJECTS
	 * WHICH ARE CREATED IN STACK!
	 */
	void			release()		;//				{if ( --m_refs == 0 ) delete this;}

	/**
	 * Returns number of references left.
	 * For DEBUG use only.
	 */
	int				references() const				{return m_refs;}
	
	Mutex*	m_mutex;
private:
//	friend class Mutex::Lock;
	long				m_refs;
	
};
#define synchronized( OBJ )		base::Mutex::Lock mutexLock( OBJ )

} // base


#endif // MING_BASE_MTOBJECT_H

// MinG
