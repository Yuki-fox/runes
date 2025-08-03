#include "base_pcheader.h"
namespace base
{
	static void parseNumberPattern( 
	const String& pattern, int pos,
	int& minIntDigits, int& maxIntDigits,
	int& minFracDigits, int& maxFracDigits,
	bool& fraction, bool& grouping, SQChar& hex )
{
	const int absMaxIntDigits = 20;

	minIntDigits		= 0;
	maxIntDigits		= 0;
	minFracDigits		= 0;
	maxFracDigits		= 0;
	fraction			= false;
	grouping			= false;
	hex					= 0;

	int patternEnd = pos;
	for ( ; patternEnd < pattern.length() ; ++patternEnd )
		if ( pattern.charAt(patternEnd) == SQChar('}') )
			break;

	for ( int i = pos ; i < patternEnd ; ++i )
	{
		SQChar ch = pattern.charAt(i);

		if ( ch == SQChar('x') || ch == SQChar('X') )
		{
			hex = (SQChar)ch;
			break;
		}
		else if ( ch == SQChar('.') )
		{
			fraction = true;
		}
		else if ( ch == SQChar(',') )
		{
			grouping = true;
		}
		else if ( ch == SQChar('0') )
		{
			// compulsory digit
			if ( fraction )
				++minFracDigits, ++maxFracDigits;
			else
				++minIntDigits, maxIntDigits=absMaxIntDigits;
		}
		else if ( ch == SQChar('#') )
		{
			// optional digit
			if ( fraction )
				++maxFracDigits;
			else
				maxIntDigits=absMaxIntDigits;
		}
	}
}


Formattable::Formattable()
{
	m_type = VALUE_NONE;
}

Formattable::Formattable( double value )
{
	m_type = VALUE_DOUBLE;
	m_dbl = value;
}

Formattable::Formattable( const String& value )
{
	m_type = VALUE_STRING;
	m_str = value;
}

Formattable::Formattable( const SQChar* value )
{
	m_type = VALUE_STRING;
	m_str = value;
}
/*
Formattable::Formattable( const char* value )
{
	m_type = VALUE_STRING;
	m_str = value;
}
*/	
Formattable::ValueType Formattable::getType() const
{
	return m_type;
}

double Formattable::doubleValue() const
{
	assert( m_type == VALUE_DOUBLE );
	return m_dbl;
}

String Formattable::stringValue() const
{
	assert( m_type == VALUE_STRING );
	return m_str;
}

int	Formattable::format( SQChar* buffer, int size, const String& pattern, int pos ) const
{
	if ( VALUE_DOUBLE == m_type )
	{
		int minIntDigits, maxIntDigits;
		int minFracDigits, maxFracDigits;
		bool fraction, grouping;
		SQChar hex;
		parseNumberPattern( pattern, pos, minIntDigits, maxIntDigits, minFracDigits, maxFracDigits, fraction, grouping, hex );

		bool chopInt = false;
		SQChar fmt[32];
		if ( hex )
			scsprintf( fmt,_SC( "%%%c"), hex ), chopInt = true;
		else if ( 0 == maxFracDigits && 0 == maxIntDigits )
			scsprintf( fmt, _SC("%%g" ));
		else if ( minIntDigits > 0 )
			scsprintf( fmt, _SC("%%0%i.%if"), minIntDigits, maxFracDigits );
		else
			scsprintf( fmt, _SC("%%.%if"), maxFracDigits );

		SQChar buff[32];
		if ( chopInt )
			scsprintf( buff, fmt, (int)m_dbl );
		else
			scsprintf( buff, fmt, m_dbl );

		int needed = scstrlen(buff);
		int count = needed;
		if ( count > size ) 
			count = size;
		for ( int i = 0 ; i < count ; ++i )
			buffer[i] = buff[i];
		if ( needed < size )
			buffer[needed] = 0;
		else if ( size > 0 )
			buffer[size-1] = 0;
		return needed;
	}
	else if ( VALUE_STRING == m_type )
	{
		int needed = m_str.length();
		int count = needed;
		if ( count > size ) 
			count = size;
		m_str.getChars( 0, count, buffer );
		if ( needed < size )
			buffer[needed] = 0;
		else if ( size > 0 )
			buffer[size-1] = 0;
		return needed;
	}
	return 0;
}


} // base

// MinG
