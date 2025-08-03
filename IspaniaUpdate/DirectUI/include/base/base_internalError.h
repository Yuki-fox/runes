#ifndef MING_BASE_INTERNALERROR_H
#define MING_BASE_INTERNALERROR_H
#include <base\base_base.h>
#include <base\base_baseTypes.h>
namespace base
{


/** 
 * Throws Exception with 'Internal error at <filename>(<line>): <expr>' message. 
 * @ingroup base
 */
void BASE_API internalError( const SQChar* filename, int line, const SQChar* expr );


} // base


#endif // MING_BASE_INTERNALERROR_H

// MinG
