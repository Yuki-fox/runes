#include "base_Pcheader.h"




namespace base
{
	

	

MTRefCount::MTRefCount(int flags) :
	m_refs(0),m_mutex(0)
{
	if ( flags & OBJECT_INITMUTEX )
		initMutex();
}

MTRefCount::MTRefCount( const MTRefCount& o) :
	m_refs(0),m_mutex(0)
{
	assert( o.m_mutex );	// initMutex has not been called

	m_mutex = o.m_mutex;
	///m_mutex->lock();
}
void MTRefCount::initMutex()
{
	if ( !m_mutex )
		m_mutex = new Mutex;
}
void MTRefCount::addRef()
{
	assert( m_refs >= 0 );

	Mutex::incrementRC( &m_refs );
}

void MTRefCount::release()
{
	assert( m_refs > 0 );

	if ( 0 == Mutex::decrementRC(&m_refs) )
		delete this;
}
MTRefCount::~MTRefCount()
{
	assert( 0 == m_refs ); // ref count of a stack/auto object incremented?
	if ( m_mutex ) 
	{
		delete m_mutex;
		m_mutex = 0;
	}
}

	
} // base

// MinG
