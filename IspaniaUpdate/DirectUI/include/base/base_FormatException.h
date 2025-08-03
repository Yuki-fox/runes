#ifndef MING_BASE_FORMATEXCEPTION_H
#define MING_BASE_FORMATEXCEPTION_H


#include <base/base_Exception.h>


namespace base
{


/**
 * Exception thrown when string formatting error occurs.
 * 
 * @ingroup base
 */
class FormatException :
	public Exception
{
public:
	/** Creates an exception with the specified error description. */
	FormatException( const base::Format& msg )						: Exception(msg) {}
};


} // base


#endif // MING_BASE_FORMATEXCEPTION_H

// MinG
