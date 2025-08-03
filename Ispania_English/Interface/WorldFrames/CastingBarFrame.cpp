#include "../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "CastingBarFrame.h"

CCastingBarFrame*			g_pCastingBarFrame = NULL;

// ----------------------------------------------------------------------------
CCastingBarFrame::CCastingBarFrame(CInterface* object)
	: CInterfaceBase(object)
{
	Clear();

	g_pCastingBarFrame	= this;
}

// ----------------------------------------------------------------------------
CCastingBarFrame::~CCastingBarFrame()
{
	g_pCastingBarFrame	= NULL;
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::RegisterFunc()
{
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::Clear()
{
	m_text.clear();
	m_castingType	= eCastingBar_None;
	m_minTime		= 0.0f;
	m_maxTime		= 0.0f;
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::CastingStart(CastingBarType type, const char* text, float time)
{
	lua_State* L = GetLuaState();

	m_castingType	= type;
	m_text			= (text) ? text : "";
	m_minTime		= (float)g_pGameMain->GetFrameTime();
	m_maxTime		= m_minTime + time;

	lua_pushstring(L, m_text.c_str());
	lua_setglobal(L, "arg1");
	lua_pushnumber(L, time);
	lua_setglobal(L, "arg2");
	SendWorldEvent("CASTING_START");
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::CastingStop(CastingBarType type)
{
	if ( m_castingType == type || type == eCastingBar_All )
	{
		Clear();
		SendWorldEvent("CASTING_STOP");
	}
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::CastingFailed(CastingBarType type)
{
	if ( m_castingType == type || type == eCastingBar_All )
	{
		Clear();
		SendWorldEvent("CASTING_FAILED");
	}
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::CastingDelayed(CastingBarType type, float time)
{
	lua_State* L = GetLuaState();

	if ( m_castingType == type )
	{
		m_minTime += time;
		m_maxTime += time;

		lua_pushnumber(L, time);
		lua_setglobal(L, "arg1");

		SendWorldEvent("CASTING_DELAYED");
	}
}

// ----------------------------------------------------------------------------
void CCastingBarFrame::ChangeZone()
{
	CastingStop(eCastingBar_All);
}