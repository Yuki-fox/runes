#ifndef MING_BASE_THROWABLE_H
#define MING_BASE_THROWABLE_H
#include <base/base_base.h>

#include <base/base_Format.h>

#if defined(__SYMBIAN32__) || defined(_WIN32_WCE)
#include <stdio.h>
#define BASE_NOEXCEPTIONS
#else
#define BASE_EXCEPTIONS
#endif


namespace base
{


/**
 * Base class for all exceptions.
 * 
 * @ingroup base
 */
class BASE_API Throwable
{
public:
	/** 
	 * Creates throwable object with no error description. 
	 */
	Throwable();

	/** 
	 * Creates throwable object with error description. 
	 * @exception FormatException If message is not valid.
	 */
	Throwable( const Format& msg );

	/** 
	 * Returns the error message format object. 
	 */
	const Format& getMessage() const;

private:
	Format	m_msg;
};

/** 
 * Wrapper for throwing exceptions.
 * Can be made 'work' also on platforms which do not support exceptions.
 */
template <class T> void throwError( T e )
{
#ifdef BASE_EXCEPTIONS
	throw e;
#else
	const char FNAME[] = "C:\\error.txt";
	FILE* fh = fopen( FNAME, "wt" );
	fprintf( fh, "ERROR: %s\n", e.getMessage().format().c_str() );
	printf( "ERROR: %s\n", e.getMessage().format().c_str() );
	fclose( fh );
	for (;;) {}
#endif
}


} // base


#endif // MING_BASE_THROWABLE_H

// MinG
