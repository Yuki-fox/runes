
#ifndef MING_SQ_STD_AUXLIB_H_
#define MING_SQ_STD_AUXLIB_H_
//#include <base\base_base.h>
#ifdef __cplusplus
extern "C" {
#endif

SQUIRREL_API void sqstd_seterrorhandlers(HSQUIRRELVM v);
SQUIRREL_API void sqstd_printcallstack(HSQUIRRELVM v);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* MING_SQ_STD_AUXLIB_H_ */
