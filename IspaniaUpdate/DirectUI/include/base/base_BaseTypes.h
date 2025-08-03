#ifndef MING_BASE_BASETYPES_H
#define MING_BASE_BASETYPES_H


#include <squirrel\squirrel.h>

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;

typedef  unsigned __int64 uint64_t;


typedef  signed char int8_t;
typedef   short int16_t;
typedef   int int32_t;
typedef  __int64 int64_t;
#ifdef _DEBUG
#define INLINE
#else
#define INLINE inline 
#endif

#endif//MING_BASE_BASETYPES_H
