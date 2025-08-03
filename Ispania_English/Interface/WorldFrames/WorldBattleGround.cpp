#include "WorldBattleGround.h"
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/net_instance/NetCli_InstanceChild.h"
 
//****************************************************************************
CWorldBattleGround*	g_pWorldBattleGround = NULL;

int LuaFunc_WBG_GetScore(lua_State* L);
int LuaFunc_WBG_GetRate(lua_State* L);
int LuaFunc_WBG_Open(lua_State* L);
int LuaFunc_WBG_Close(lua_State* L);
//****************************************************************************
CWorldBattleGround::CWorldBattleGround(CInterface* object): CInterfaceBase(object)
{
	g_pWorldBattleGround = this;
	m_IsVisible = -1;
	m_SettleAccountsTime = 0;			//�h�[����
	m_RewardTime = 0;
	m_ChangeWaitTime = 30;
	m_Rate = -20;
}
//****************************************************************************
CWorldBattleGround::~CWorldBattleGround()
{
	g_pWorldBattleGround = NULL;
}
//****************************************************************************
void CWorldBattleGround::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{	
		lua_register(L, "WBG_GetScore",			LuaFunc_WBG_GetScore	);
		lua_register(L, "WBG_GetRate",			LuaFunc_WBG_GetRate	);
		lua_register(L, "WBG_Open",				LuaFunc_WBG_Open		);
		lua_register(L, "WBG_Close",			LuaFunc_WBG_Close		);
	}
}
//****************************************************************************
void CWorldBattleGround::Update(float elapsedTime)
{
	float temp = m_ChangeWaitTime;
	m_ChangeWaitTime -= elapsedTime;

	if( m_ChangeWaitTime <= 0 )
	{
		g_pWorldBattleGround->WBGOpen();
		g_pWorldBattleGround->SetWBGChangTime();

	}
}
//****************************************************************************
void CWorldBattleGround::WBGOpen()
{
	const char* temp;
	temp = "SYS_WORLD_EVENT_END_TIME";
	Net_Instance::SL_GetWorldVar( temp );		//�Z���U�����⪺�ɶ��G
	
	temp = "SYS_WORLD_EVENT_INFLUENCE_SCORE";
	Net_Instance::SL_GetWorldVar( temp );		//�դO��G

	temp = "SYS_WORLD_EVENT_PRIZE_TIME";
	Net_Instance::SL_GetWorldVar( temp );		//���y�ɶ��G

	g_pWorldBattleGround->m_IsVisible = 0;
}
//****************************************************************************
void CWorldBattleGround::WBGClose()
{
	g_pWorldBattleGround->m_IsVisible = 1;
}
//****************************************************************************
void CWorldBattleGround::R_GetWorldVarResult(const char* pszVarName, int iVarValue)
{
	
	if( strcmp(pszVarName, "SYS_WORLD_EVENT_END_TIME") == 0)//�Z���U�����⪺�ɶ�
	{
		m_SettleAccountsTime = iVarValue;	

		lua_State *L=GetLuaState();
		lua_pushnumber(L, iVarValue);
		lua_setglobal( L, "arg1" );
		g_pInterface->SendWorldEvent("WBG_FRAME_END_TIMEUPDATE");
	}
	
	else if(strcmp(pszVarName, "SYS_WORLD_EVENT_INFLUENCE_SCORE") == 0)  //�դO��
	{	 
		m_Rate = iVarValue;
		lua_State *L=GetLuaState();
		lua_pushnumber(L, iVarValue);
		lua_setglobal( L, "arg1" );
		g_pInterface->SendWorldEvent("WBG_FRAME_INFLUENCE_SCOREUPDATE");
	}
	
	else if(strcmp(pszVarName, "SYS_WORLD_EVENT_PRIZE_TIME") == 0)	 //���y�ɶ�  
	{	
		m_RewardTime = iVarValue;
		lua_State *L=GetLuaState();
		lua_pushnumber(L, iVarValue);
		lua_setglobal( L, "arg1" );
		g_pInterface->SendWorldEvent("WBG_FRAME__PRIZE_TIMEUPDATE");
	}
}				
//****************************************************************************
int LuaFunc_WBG_Open(lua_State* L)
{
	g_pWorldBattleGround->WBGOpen();
	return 0;
}
//****************************************************************************
int LuaFunc_WBG_Close(lua_State* L)
{
	g_pWorldBattleGround->WBGClose();
	return 0;
}
//****************************************************************************
int LuaFunc_WBG_GetScore(lua_State* L)
{	 
	int keyItem;
	char tt[256];
	sprintf( tt, "IncursivePoint" );
	keyItem = g_ObjectData->GetSysKeyValue(tt);
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->IncursivePoint);

	lua_pushnumber(L, keyItem);
	return 2;
}
int LuaFunc_WBG_GetRate(lua_State* L)
{	 
	lua_pushnumber(L, g_pWorldBattleGround->m_Rate);
	return 1;
}