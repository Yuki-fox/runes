#ifndef MING_BASE_DEBUG_H
#define MING_BASE_DEBUG_H

#include <base/base_base.h>
#include <base/base_baseTypes.h>
#include <base/base_Formattable.h>


namespace base
{


/** 
 * Provides support for debug output. Default implementation writes the output to
 * stdout and to Debug Output on Win32 platform.
 *
 * On Windows, debug output can be viewed with IDE debugger or stand-alone 
 * debug output viewer -- there are multiple available for free on Internet. This kind
 * of debug output has several benefits over traditional log files:
 * <ol>
 * <li>You get debug output visible already while running application without file access / reload issues.
 * <li>You always get up-to-date output, instead of accidentally looking at old debug output files.
 * <li>Debug output does not clutter up hard disk unless debug output viewer is active and logging enabled from the viewer.
 * <li>Debug output can be filtered, color coded, etc., marked, depending on debug output viewer application's features.
 * <li>Debug output viewer can provide support for remote debug output viewing on remote PC.
 * </ol>
 * 
 * @ingroup base
 */
class BASE_API Debug
{
public:
	/** Outputs a log message. printf compatible arguments. */
	static void			printf( const SQChar* fmt, ... );

	/** Strips parent paths from full filename. */
	static const SQChar*	stripSourceFilename( const SQChar* fname );
};


} // base


#ifndef DBG_TRACE
#define DBG_TRACE() base::Debug::printf( "%s(%d)\n", base::Debug::stripSourceFilename(_SC(__FILE__)),_SC( __LINE__) )
#endif // DBG_TRACE

#ifndef DBG_OUT
#define DBG_OUT(A) base::Debug::printf( "%s @ %s(%d)\n", (A), base::Debug::stripSourceFilename(_SC(__FILE__)), _SC(__LINE__) )
#endif // DBG_OUT

#ifndef DBG_ONCE
#ifdef __SYMBIAN32__
#define DBG_ONCE(A) 
#else
#define DBG_ONCE(A) {static bool s_once = true; if ( s_once && !(s_once=false) ) A;}
#endif
#endif // DBG_ONCE

#if defined(DEBUG) || defined (_DEBUG)
#	define _LOGINSANE( message ) base::Debug::printf(message);
#else
#	define _LOGINSANE( message )
#endif

#endif // MING_BASE_DEBUG_H

// MinG
