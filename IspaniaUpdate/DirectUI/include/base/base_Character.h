#ifndef MING_BASE_CHARACTER_H
#define MING_BASE_CHARACTER_H

#include <base\base_base.h>
namespace base
{


/** 
 * Character type information.
 * 
 * @ingroup base
 */
class BASE_API Character
{
public:
	/** Returns true if the code point is a Unicode digit. */
	static bool		isDigit( int cp );

	/** Returns true if the code point is a Unicode letter. */
	static bool		isLetter( int cp );

	/** Returns true if the code point is a Unicode letter or digit. */
	static bool		isLetterOrDigit( int cp );

	/** Returns true if the code point is a Unicode lower-case. */
	static bool		isLowerCase( int cp );

	/** Returns true if the code point is a Unicode upper-case. */
	static bool		isUpperCase( int cp );

	/** Returns true if the code point is a Unicode whitespace. */
	static bool		isWhitespace( int cp );
};


} // base


#endif // MING_BASE_CHARACTER_H

// MinG
