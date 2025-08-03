#ifndef MING_BASE_ICONVERTER_H
#define MING_BASE_ICONVERTER_H

#include <base/base_Base.h>
#include <base/base_RefCount.h>

#define INTERFACE_IConverter(T) \
	public:\
	virtual bool	decode( const void* src, const void* srcend, int* srcbytes, int* dst ) const ##T\
	virtual bool	encode( void* dst, void* dstend, int* dstbytes, int src ) const ##T

#define BASE_IConverter INTERFACE_IConverter(=0;)
#define DERIVD_IConverter INTERFACE_IConverter(;)

namespace base
{


/**
 * Character encoding/decoding interface.
 * Converts to/from Unicode code points.
 *
 * 
 * @ingroup base
 */
class BASE_API IConverter :	public RefCount
{
public:
	BASE_IConverter;
};


} // base


#endif // MING_BASE_ICONVERTER_H

// MinG
