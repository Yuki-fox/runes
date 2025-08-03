#include "base_Pcheader.h"


#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <pthread.h>
#endif


//-----------------------------------------------------------------------------

// select atomic addition operation implementation
#if defined(_WIN32) && defined(_MSC_VER)
	#define ATOMICADD_INTELWIN32
	#pragma warning( disable : 4035 )	// 'no return value'
#elif defined(_WIN32)
	#define ATOMICADD_WIN32
#elif defined(_POSIX_THREADS)
	#define ATOMICADD_PTHREADS
	static pthread_mutex_t s_refCountMutex = PTHREAD_MUTEX_INITIALIZER;
#else
	#error Current platform does not have atomic addition implemented.
#endif

//-----------------------------------------------------------------------------

namespace base
{

	Mutex::Lock::Lock(  Mutex& o )
	{
		//assert( o );	// initMutex has not been called

		m = &o;
		m->lock();
	}

	Mutex::Lock::Lock(  Mutex* o )
	{
		assert( o );	// initMutex has not been called

		m = o;
		m->lock();
	}

	Mutex::Lock::~Lock()
	{
		m->unlock();
	}

class Mutex::MutexImpl
{
public:
	#ifdef _WIN32
	CRITICAL_SECTION	obj;
	#else
	pthread_mutex_t		obj;
	#endif

	MutexImpl()
	{
		#ifdef _WIN32
		InitializeCriticalSection( &obj );
		#else
		pthread_mutex_init( &obj, 0 );
		#endif
	}

	~MutexImpl()
	{
		#ifdef _WIN32
		DeleteCriticalSection( &obj );
		#else
		pthread_mutex_destroy( &obj );
		#endif
	}

	void lock()
	{
		#ifdef _WIN32
		EnterCriticalSection( &obj );
		#else
		pthread_mutex_lock( &obj );
		#endif
	}

	void unlock()
	{
		#ifdef _WIN32
		LeaveCriticalSection( &obj );
		#else
		pthread_mutex_unlock( &obj );
		#endif
	}
};

//-----------------------------------------------------------------------------

Mutex::Mutex()
{
	m_this = new MutexImpl;
}

Mutex::~Mutex()
{
	delete m_this;
}

void Mutex::lock()
{
	m_this->lock();
}

void Mutex::unlock()
{
	m_this->unlock();
}

void Mutex::incrementRC( long* value )
{
	#if defined(ATOMICADD_INTELWIN32)

		__asm
		{
			mov ebx, value
			mov eax, 1
			lock xadd dword ptr [ebx], eax
			inc eax
		}

	#elif defined(ATOMICADD_WIN32)

		InterlockedIncrement( value );

	#elif defined(ATOMICADD_PTHREADS)

		pthread_mutex_lock( &s_refCountMutex );
		long v = ++*x;
		pthread_mutex_unlock( &s_refCountMutex );
		return v;

	#endif
}

long Mutex::decrementRC( long* value )
{
	#if defined(ATOMICADD_INTELWIN32)

		__asm
		{
			mov ebx, value
			mov eax, -1
			lock xadd dword ptr [ebx], eax
			dec eax
		}

	#elif defined(ATOMICADD_WIN32)

		return InterlockedDecrement( value );

	#elif defined(ATOMICADD_PTHREADS)

		pthread_mutex_lock( &s_refCountMutex );
		long v = --*x;
		pthread_mutex_unlock( &s_refCountMutex );
		return v;

	#endif
}

long Mutex::testAndSet( long* value, int newValue )
{
	#ifdef _WIN32

		return InterlockedExchange( value, newValue );

	#else

		pthread_mutex_lock( &s_refCountMutex );
		long v = *value;
		*value = newValue;
		pthread_mutex_unlock( &s_refCountMutex );
		return v;

	#endif
}


} // util
