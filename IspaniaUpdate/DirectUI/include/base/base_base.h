#ifndef _MING_BASE_BASE_H
#define _MING_BASE_BASE_H
#include <suppressWarnings.h>


#ifdef MINGCORE_EXPORTS 
#define BASE_API __declspec(dllexport)
#else
#ifdef MINGCORE_USRDLL
#define BASE_API __declspec(dllimport)
#else 
#define BASE_API
#endif
#endif

#endif//_MING_BASE_BASE_H
