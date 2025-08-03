#include "base_Pcheader.h"





namespace base
{


bool Character::isDigit( int cp )
{
	if ( cp < int(0x80) )
		return 0 != scisdigit( (SQChar)cp );
	else
		return false;
}

bool Character::isLetter( int cp )
{
	if ( cp < int(0x80) )
		return 0 != scisalpha( (SQChar)cp );
	else
		return false;
}

bool Character::isLetterOrDigit( int cp )
{
	return isLetter(cp) || isDigit(cp);
}

bool Character::isLowerCase( int cp )
{
	if ( cp < int(0x80) )
		return 0 != islower( (SQChar)cp );
	else
		return false;
}

bool Character::isUpperCase( int cp )
{
	if ( cp < int(0x80) )
		return 0 != isupper( (SQChar)cp );
	else
		return false;
}

bool Character::isWhitespace( int cp )
{
	if ( cp < int(0x80) )
		return 0 != scisspace( (SQChar)cp );
	else
		return false;
}


} // base

// MinG
