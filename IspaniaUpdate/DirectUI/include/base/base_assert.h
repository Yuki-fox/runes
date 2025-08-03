// we'll allow this file to be included multiple times
// since assert() might be re-defined in many places
#include <base/base_internalError.h>

// by default, we'll be using CRT assert
#include <assert.h>

// check if non-debug assert is requested
#ifdef NDEBUG_ASSERT
#ifdef _DEBUG
#undef assert
#define assert(E) if (E) {} else {base::internalError(_SC(__FILE__),_SC(__LINE__),#E);}
#endif // !_DEBUG
#endif // NDEBUG_ASSERT
