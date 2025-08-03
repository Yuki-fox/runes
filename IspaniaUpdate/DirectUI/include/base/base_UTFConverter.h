#ifndef MING_BASE_UTFCONVERTER_H
#define MING_BASE_UTFCONVERTER_H

#include <base/base_Base.h>
#include <base/base_IConverter.h>


namespace base
{


/**
 * Unicode UTF-data encoding/decoding helper class.
 * Supported encoding schemes are 
 * ASCII-7, UTF-8, UTF-16BE, UTF-16LE, UTF-32BE, UTF-32LE.
 * 
 * @ingroup base
 */
class BASE_API UTFConverter :	public IConverter
{
public:
	/** Encoding/decoding scheme. */
	enum EncodingType
	{
		/** Unsupported/unknown encoding. */
		ENCODING_UNKNOWN,
		/** US/ASCII */
		ENCODING_ASCII7,
		/** UTF-8 */
		ENCODING_UTF8,
		/** UTF-16, platform endian */
		ENCODING_UTF16,
		/** UTF-16 Big Endian */
		ENCODING_UTF16BE,
		/** UTF-16 Little Endian */
		ENCODING_UTF16LE,
		/** UTF-32, platform endian */
		ENCODING_UTF32,
		/** UTF-32 Big Endian */
		ENCODING_UTF32BE,
		/** UTF-32 Little Endian */
		ENCODING_UTF32LE
	};
	
	/** 
	 * Creates an UTF-encoding converter with specified encoding.
	 */
	explicit UTFConverter( EncodingType encoding );

	
	DERIVD_IConverter;

private:
	EncodingType	m_type;
};


} // base


#endif // MING_BASE_UTFCONVERTER_H

// MinG
