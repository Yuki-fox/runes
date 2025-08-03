// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#define _ATL_USE_CSTRING
#define _ATL_USE_CSTRING_FLOAT
#define _ATL_USE_DDX_FLOAT


// Change these values to use different versions
#define WINVER          0x0501
#define _WIN32_WINNT    0x0501
#define _WIN32_IE       0x0600
#define _RICHEDIT_VER   0x0300

#define _ATL_USE_CSTRING
#define _ATL_USE_CSTRING_FLOAT
#define _ATL_USE_DDX_FLOAT

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <atlddx.h>

#include <time.h>
#include <assert.h>


#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

#define CHECKHR(x) {HRESULT hr = x; if (FAILED(hr)) goto CleanUp;}
#define SAFERELEASE(p) {if (p) {(p)->Release(); p = NULL;}}
#define UpdateRar _T("Update.rar")
#define VCP2PZIP _T("vcp2p.zip")
#define VCP2PExe _T("vcp2p.exe")
#define UpdateExe _T("ClientUpdate.exe")
// TODO: reference additional headers your program requires here
