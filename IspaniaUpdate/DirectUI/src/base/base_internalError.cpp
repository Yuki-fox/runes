#include "base_pcheader.h"
#include <base/base_internalError.h>
#include <base/base_Debug.h>
#include <base/base_Exception.h>
//#include <config.h>


namespace base
{


void base::internalError( const SQChar* filename, int line, const SQChar* expr )
{
	Debug::printf( _SC("INTERNAL ERROR: %s(%d): %s\n"), filename, line, expr );

	String str(_SC("Internal error at {0}({1}): {2}"));
	throwError( Exception( Format(str, filename, line, expr) ) );
}


} // base

// MinG
