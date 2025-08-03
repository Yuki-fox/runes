#include <Windows.h>
#include "uibase.h"

void lua_pushstring (lua_State *L, const wchar_t *s) {
	if (s == NULL)
		lua_pushnil(L);
	else {
		char* us = WCharToUFT8(s);
		lua_pushlstring(L, us, strlen(us));
		delete [] us;
	}
}

char* WCharToUFT8(const wchar_t *ws) {
	int nLen = WideCharToMultiByte(CP_UTF8, 0, ws, -1, NULL, NULL, NULL, NULL);
	char* us = new char[nLen];
	WideCharToMultiByte(CP_UTF8, 0, ws, -1, us, nLen, NULL, NULL);
	return us;
}