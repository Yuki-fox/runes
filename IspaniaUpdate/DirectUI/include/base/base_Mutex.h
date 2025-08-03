#ifndef MING_BASE_MUTEX_H
#define MING_BASE_MUTEX_H

#include <base/base_base.h>
namespace base
{


/** 
 * Mutual exclusion synchronization object. 
 * 
 */
class BASE_API Mutex
{
public:
	class BASE_API  Lock 
	{ 
		Mutex* m; 
		Lock(); 
		Lock( const Lock& ); 
		Lock& operator=( const Lock& ); 
	public: 
		Lock(  Mutex& o ); 
		Lock(  Mutex* o ); 
		~Lock();
	};

	///
	Mutex();

	///
	~Mutex();

	/** Acquires exclusive access to the mutex. */
	void	lock();

	/** Releases exclusive access to the mutex. */
	void	unlock();

	/** Atomic increment operation for reference counting. */
	static void		incrementRC( long* value );

	/** Atomic decrement operation for reference counting. */
	static long		decrementRC( long* value );

	/** Atomic test-and-set operation. */
	static long		testAndSet( long* value, int newValue=1 );

private:
	class MutexImpl;
	MutexImpl* m_this;

	Mutex( const Mutex& );
	Mutex& operator=( const Mutex& );
};
/* Helper class for synchronizing access to an Object. */


} // base


#endif // MING_BASE_MUTEX_H
