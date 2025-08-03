#include ".\base_Semaphore.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <limits.h>
#else
	#include <semaphore.h>
#endif


//-----------------------------------------------------------------------------

namespace base
{


class Semaphore::SemaphoreImpl
{
public:
	#ifdef _WIN32
	HANDLE		obj;
	#else
	sem_t		obj;
	#endif

	SemaphoreImpl( int value )
	{
		#ifdef _WIN32
		obj = CreateSemaphore( 0, value, INT_MAX, 0 );
		#else
		sem_init( &obj, 0, value );
		#endif
	}

	~SemaphoreImpl()
	{
		#ifdef _WIN32
		CloseHandle( obj ); 
		#else
		sem_destroy( &obj );
		#endif
	}

	void wait()
	{
		#ifdef _WIN32
		WaitForSingleObject( obj, INFINITE );
		#else
		sem_wait( &obj );
		#endif
	}

	void signal()
	{
		#ifdef _WIN32
		ReleaseSemaphore( obj, 1, 0 );
		#else
		sem_post( &obj );
		#endif
	}
};

//-----------------------------------------------------------------------------

Semaphore::Semaphore( int value )
{
	m_this = new SemaphoreImpl( value );
}

Semaphore::~Semaphore()
{
	delete m_this;
}

void Semaphore::wait()
{
	m_this->wait();
}

void Semaphore::signal()
{
	m_this->signal();
}


} // base
