#include <base\base_Thread.h>
#include <base\base_RefCount.h>
#include "base_Thread_impl.h"
#include <base\base_Throwable.h>
#include <base\base_Exception.h>
#include "base_Semaphore.h"
#include <assert.h>


//-----------------------------------------------------------------------------

namespace base
{


class ThreadImpl :	public RefCount
{
public:
	THREAD_H	handle;
	SP(Thread)	obj;
	bool		running;

	ThreadImpl()
	{
		handle	= 0;
		obj		= 0;
		running	= false;
	}

private:
	ThreadImpl( const ThreadImpl& );
	ThreadImpl& operator=( const ThreadImpl& );
};

//-----------------------------------------------------------------------------

static void* runThread( void* arg )
{
	SP(ThreadImpl) thread = reinterpret_cast<ThreadImpl*>(arg);
	assert( thread->obj );
	assert( thread->running );

	try 
	{
		thread->obj->run();
	}
	catch ( ... )
	{
	}

	thread->running = false;
	//thread->obj = 0;
	//thread = 0;
	return 0;
}

//-----------------------------------------------------------------------------

Thread::Thread():MTRefCount(OBJECT_INITMUTEX)
{
	m_this = new ThreadImpl;
}

Thread::~Thread()
{
	assert( !m_this->running );

	if ( m_this->handle )
	{
		Threadimp::detach( m_this->handle );
		m_this->handle = 0;
	}
}

void Thread::start()
{
	assert( !m_this->running );

	if ( m_this->handle )
	{
		Threadimp::detach( m_this->handle );
		m_this->handle = 0;
	}

	m_this->obj = this;
	m_this->running = true;
	if ( Threadimp::create( &m_this->handle, runThread, m_this ) )
	{
		m_this->obj = 0;
		m_this->running = false;
		throw Exception( Format("Failed to create a thread.") );
	}
}
bool	Thread::alive()
{
	return m_this->running;

}
void Thread:: detach(void )
{
	if ( m_this->handle )
	{
		Threadimp::detach( m_this->handle );
		m_this->running=0;
		m_this->handle = 0;
	}

}
void Thread::join()
{
	if ( m_this->handle )
	{
		Threadimp::join( m_this->handle );
		m_this->handle = 0;
	}
}

void Thread::run()
{
	assert( false );	// Thread is used by subclassing
}

void Thread::sleep( long millis )
{
	Threadimp::sleep( millis );
}


} // base
