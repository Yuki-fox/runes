#include "base_Pcheader.h"




namespace base
{


	Throwable::Throwable()
	{
	}

	Throwable::Throwable( const base::Format& msg ) :
	m_msg( msg )
	{
		SQChar buf[1024];
		msg.format( buf, sizeof(buf) );
		Debug::printf( _SC("Created Throwable( \"%s\" )\n"), buf );
	}

	const base::Format& Throwable::getMessage() const
	{
		return m_msg;
	}


} // base

// MinG
