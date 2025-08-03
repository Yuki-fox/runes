#include "base_Pcheader.h"

//#include <base/base_Debug.h>
//#include <base/base_pp.h>


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif



// Maximum length of a single debug message.
#define MAX_MSG_LEN 1000


namespace base
{
	


void Debug::printf( const SQChar* fmt, ... )
{
	// format variable arguments
	const unsigned MAX_MSG = 10240*2;
	SQChar msg[MAX_MSG+4];
	va_list marker;
	va_start( marker, fmt );
	scvsprintf( msg, fmt, marker );
	va_end( marker );
	
	//
	int len=scstrlen(msg);
	if (len>4096) 
		len=len;
	assert( len < MAX_MSG ); // too long debug message
	msg[len]=SQChar('\n');
	msg[len+1]=0;
#if defined(PLATFORM_SYMBIAN)
	FILE* fh = fopen( "C:\\log.txt", "at" );
	if ( fh != 0 )
	{
		fprintf( fh, "%s", msg );
		fclose( fh );
	}
#elif defined(_WIN32)
	OutputDebugString( msg );
#elif defined(PLATFORM_WINCE)
	FILE* fh = fopen( "log.txt", "at" );
	if ( fh != 0 )
	{
		fprintf( fh, "%s", msg );
		fclose( fh );
	}
	::printf( msg );
#else
	::printf( msg );
#endif
}

const SQChar*	Debug::stripSourceFilename( const SQChar* fname )
{
	const SQChar* sz = scstrchr( fname, SQChar('/') );
	if ( sz != 0 )
	{
		while ( sz > fname )
		{
			--sz;
			if ( *sz == '/' )
			{
				++sz;
				break;
			}
		}
		return sz;
	}

	return fname;
}


} // base

// MinG
