#include "base_Pcheader.h"



#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment( lib, "winmm" )
#endif

#ifdef PLATFORM_WINCE
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment( lib, "Mmtimer.lib" )
#endif

#include <time.h>



namespace base
{
	
	void InitStringTable(void );
	void FinishStringTable(void );

	void System::initial(void)
	{
		 InitStringTable();

		

	}
 void  System::finish(void)
 {
	 FinishStringTable();



 }
int System::currentTimeMillis()
{
#ifdef _WIN32

	LARGE_INTEGER freq;
	if ( QueryPerformanceFrequency(&freq) )
	{
		LARGE_INTEGER cur;
		QueryPerformanceCounter( &cur );
		if ( freq.QuadPart >= 1000 )
		{
			__int64 msdiv = __int64(freq.QuadPart) / __int64(1000);
			__int64 c = __int64(cur.QuadPart) / msdiv;
			return (int)c;
		}
	}

	return (int)timeGetTime();

#elif defined(PLATFORM_WINCE)

	return (int)timeGetTime();

#elif defined(PLATFORM_PS2)
	
	#warning System::currentTimeMillis not defined on PS2 (returns 0)
	return 0;

#elif defined(PLATFORM_SYMBIAN)
	
	#warning System::currentTimeMillis not defined on Symbian (returns 0)
	return 0;
	
#endif
}


} // base

// MinG
