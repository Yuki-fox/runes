#include "../GameMain.h"
#include "UIOptionsFrame.h"

// **********************************************************************************************
// Globals

CUIOptionsFrame				*g_pUIOptionsFrame = NULL;
// **********************************************************************************************

int	GetOptionsStatus (lua_State *L);
int	SetOptionsHitMoveState(lua_State *L);
int	SetOptionsAutoDirState(lua_State *L);
int	SetOptionsCameraFollowState(lua_State *L);

void RegisterUIOptionsFrameFunc()
{
	g_pLuaScript->Register("GetOptionsStatus"			, GetOptionsStatus);
	g_pLuaScript->Register("SetOptionsHitMoveState"		, SetOptionsHitMoveState);
	g_pLuaScript->Register("SetOptionsAutoDirState"		, SetOptionsAutoDirState);
	g_pLuaScript->Register("SetOptionsCameraFollowState", SetOptionsCameraFollowState);
}

CUIOptionsFrame::CUIOptionsFrame()
{
	m_isHitMove			= false;
	m_isAutoFaceToFace	= false;
	m_isCameraFollow	= false;
}

CUIOptionsFrame::~CUIOptionsFrame()
{
}

void CUIOptionsFrame::Initial()
{
	if ( g_pUIOptionsFrame == NULL )
	{
		g_pUIOptionsFrame = new CUIOptionsFrame;
	}
}

void CUIOptionsFrame::Release()
{
	SAFE_DELETE( g_pUIOptionsFrame );	
}

int	GetOptionsStatus (lua_State *L)
{
	luaStack l(L);
	l.Push( g_pUIOptionsFrame->m_isHitMove );
	l.Push( g_pUIOptionsFrame->m_isAutoFaceToFace );
	l.Push( g_pUIOptionsFrame->m_isCameraFollow );
	return 3;
}

int	SetOptionsHitMoveState(lua_State *L)
{
	luaStack l(L);
	g_pUIOptionsFrame->m_isHitMove = l.GetParam<bool>(1);
	return 0;
}

int	SetOptionsAutoDirState(lua_State *L)
{
	luaStack l(L);
	g_pUIOptionsFrame->m_isAutoFaceToFace = l.GetParam<bool>(1);
	return 0;
}

int	SetOptionsCameraFollowState(lua_State *L)
{
	luaStack l(L);
	g_pUIOptionsFrame->m_isCameraFollow = l.GetParam<bool>(1);
	return 0;
}