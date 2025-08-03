#include "../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "ClientCountDownBarFrame.h"

CClientCountDownBarFrame*			g_pClientCountDownBarFrame = NULL;

CClientCountDownBarFrame::CClientCountDownBarFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pClientCountDownBarFrame = this;
}

CClientCountDownBarFrame::~CClientCountDownBarFrame()
{
	g_pClientCountDownBarFrame = NULL;
}

void CClientCountDownBarFrame::RegisterFunc()
{

}

void CClientCountDownBarFrame::CountDownStart(const char* text, float time, float nTinme, int iMode)
{
	lua_State* L = GetLuaState();

	lua_pushstring(L, text);
	lua_setglobal(L, "arg1");
	lua_pushnumber(L, time);
	lua_setglobal(L, "arg2");
	lua_pushnumber(L, iMode);
	lua_setglobal(L, "arg3");
	lua_pushnumber(L, nTinme);
	lua_setglobal(L, "arg4");

	SendWorldEvent(UiEvent_ClientCountDownStart);
}

void CClientCountDownBarFrame::CountDownStop()
{
	SendWorldEvent(UiEvent_ClientCountDownStop);
}