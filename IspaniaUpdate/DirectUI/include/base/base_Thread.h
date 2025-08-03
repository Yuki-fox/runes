#ifndef MING_BASE_THREAD_H
#define MING_BASE_THREAD_H


#include <base/base_MTRefCount.h>


namespace base
{


class ThreadImpl;


/**
 * Thread of execution in the program.
 * As starting a thread involves reference counting you should
 * always construct classes derived from Thread with operator new.
 * 
 */
class BASE_API Thread :	public MTRefCount
{
public:
	///
	Thread();

	///
	virtual ~Thread();

	/**
	 * Starts the executing the thread. 
	 * The result is that two threads are running concurrently - 
	 * the current thread (which returns from the call to the start method)
	 * and the other thread (which executed run method of the derived class).
	 * Increments reference count of this object before the thread starts
	 * and decrements reference count after the thread has terminated.
	 * (so the thread object does not need to be referenced by the
	 * user after this method returns)
	 * @exception Exception
	 */
	void	start();

	/**
	 * Waits for the thread to die.
	 */
	void	join();

	bool	alive();
	void detach(void );
	/**
	 * Performs actual work of the thread.
	 * Derived classes override this method.
	 * Called by the start() method of Thread class.
	 */ 
	virtual void	run();

	/**
	 * Temporarily cease execution of current thread 
	 * for the specified number of milliseconds.
	 */
	static void		sleep( long millis );

private:
	SP(ThreadImpl) m_this;

	Thread( const Thread& );
	Thread& operator=( const Thread& );
};


} // base


#endif // MING_BASE_THREAD_H

