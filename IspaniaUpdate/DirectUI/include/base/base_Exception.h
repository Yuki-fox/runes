#ifndef MING_BASE_EXCEPTION_H
#define MING_BASE_EXCEPTION_H


#include <base/base_Base.h>
#include <base/base_Throwable.h>


namespace base
{


/**
 * Base class for all exceptions that an application might want to handle.
 * 
 * @ingroup base
 */
class BASE_API Exception :	public Throwable
{
public:
	/** Creates an exception with no error description. */
	Exception() {}

	/** Creates an exception with the specified error description. */
	Exception( const base::Format& msg )											: Throwable(msg) {}
};


} // base


#endif // MING_BASE_EXCEPTION_H

// MinG
