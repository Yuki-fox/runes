#ifndef MING_BASE_SEMAPHORE_H
#define MING_BASE_SEMAPHORE_H

//#include <base/base_base.h>
namespace base
{


/** 
 * Semaphore synchronization object. 
 * 
 */
class /*LANG_API*/ Semaphore
{
public:
	/** Constructs semaphore with specified initial value. */
	Semaphore( int value=0 );

	///
	~Semaphore();

	/** Waits for the semaphore (P primitive). */
	void	wait();

	/** Signals the semaphore (V primitive). */
	void	signal();

private:
	class SemaphoreImpl;
	SemaphoreImpl* m_this;

	Semaphore( const Semaphore& );
	Semaphore& operator=( const Semaphore& );
};


} // base


#endif // MING_BASE_SEMAPHORE_H
