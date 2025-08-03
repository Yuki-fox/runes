#include "base_Pcheader.h"
#include <base/base_OutOfMemoryException.h>



namespace base
{


void throw_OutOfMemoryException()
{
	throwError( OutOfMemoryException() );
}
	
OutOfMemoryException::OutOfMemoryException()
{
}


} // base

// MinG
