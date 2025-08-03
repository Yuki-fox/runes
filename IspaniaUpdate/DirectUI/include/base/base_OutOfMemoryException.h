#ifndef MING_BASE_OUTOFMEMORYEXCEPTION_H
#define MING_BASE_OUTOFMEMORYEXCEPTION_H


#include <base/base_Exception.h>


namespace base
{


/** 
 * Thrown if application rans out of memory. 
 * @ingroup base
 */
class OutOfMemoryException :	public Exception
{
public:
	OutOfMemoryException();
};


/*
 * Throws OutOfMemoryException. (used to avoid dependencies to header OutOfMemoryException.h) 
 */
BASE_API void throw_OutOfMemoryException();


} // base


#endif // MING_BASE_OUTOFMEMORYEXCEPTION_H

// MinG
