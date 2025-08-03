#include "../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "ExchangeClassFrame.h"

CExchangeClassFrame*			g_pExchangeClassFrame;

int LuaFunc_CloseExchangeClass	(lua_State* L);
int	LuaFunc_GetNumClasses		(lua_State* L);
int LuaFunc_GetPlayerNumClasses	(lua_State* L);
int LuaFunc_GetPlayerClassInfo	(lua_State* L);
int LuaFunc_ExchangeClass		(lua_State* L);

// ----------------------------------------------------------------------------
CExchangeClassFrame::CExchangeClassFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_npc = 0;
	m_locked = false;

	g_pExchangeClassFrame = this;
}

// ----------------------------------------------------------------------------
CExchangeClassFrame::~CExchangeClassFrame()
{
	g_pExchangeClassFrame = NULL;
}

// ----------------------------------------------------------------------------
void CExchangeClassFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "CloseExchangeClass",	LuaFunc_CloseExchangeClass);
		lua_register(L, "GetNumClasses",		LuaFunc_GetNumClasses);
		lua_register(L, "GetPlayerNumClasses",	LuaFunc_GetPlayerNumClasses);
		lua_register(L, "GetPlayerClassInfo",	LuaFunc_GetPlayerClassInfo);
		lua_register(L, "ExchangeClass",		LuaFunc_ExchangeClass);
	}
}

// ----------------------------------------------------------------------------
void CExchangeClassFrame::Update(float elapsedTime)
{
	if ( m_npc )
	{
		if ( g_pGameMain->GetDistanceWithPlayer(m_npc) > _MAX_ROLE_TOUCH_MAXRANGE_ )
		{
			// g_pError->AddMessage(0, 0, "Close JobUI, NPC to far");
			Close();
		}
	}
}

// ----------------------------------------------------------------------------
void CExchangeClassFrame::Open(int npc)
{
	m_npc = npc;

	SendWorldEvent("EXCHANGECLASS_SHOW");
}

// ----------------------------------------------------------------------------
void CExchangeClassFrame::Close()
{
	m_locked = false;

	if ( m_npc )
	{
		m_npc = 0;
		NetCli_RoleValue::S_CloseChangeJobRequest();
	}
}

// ----------------------------------------------------------------------------
void CExchangeClassFrame::ExchangeClass(int mainClass, int subClass)
{
	if ( m_locked )
		return;

	m_locked = true;

	m_changeJob[0] = mainClass;
	m_changeJob[1] = subClass;

	// set class
	NetCli_RoleValue::S_ChangeJobRequest(mainClass, subClass);
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseExchangeClass(lua_State* L)
{
	g_pExchangeClassFrame->Close();
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetNumClasses(lua_State* L)
{
	lua_pushnumber(L, EM_Max_Vocation);
	return 1;
}

// ----------------------------------------------------------------------------
// 取得目前已取得個數與最大可取得個數
int LuaFunc_GetPlayerNumClasses	(lua_State* L)
{
	int count = 0;
	for ( int i = 0; i < EM_Max_Vocation; i++ )
	{
		if ( CNetGlobal::RoleData()->PlayerBaseData->AbilityList[i].Level > 0 )
			count++;
	}

	lua_pushnumber(L, count);
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->VocCount);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerClassInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	bool have = (lua_toboolean(L, 2) != 0);

	if ( index >= 0 && index < EM_Max_Vocation )
	{
		RoleDataEx* data = CNetGlobal::RoleData();
		if ( have )
		{
			for ( int i = 0; i < EM_Max_Vocation; i++ )
			{
				int level = data->PlayerBaseData->AbilityList[i].Level;
				if ( level > 0 )
				{
					if ( index == 0 )
					{
						lua_pushstring(L, g_ObjectData->GetClassNameByID((Voc_ENUM)i));
						lua_pushstring(L, g_ObjectData->GetClassTokenByID((Voc_ENUM)i));
						lua_pushnumber(L, level);
						lua_pushnumber(L, data->PlayerBaseData->AbilityList[i].EXP);
						lua_pushnumber(L, data->JobExpTable[level - 1]);
						lua_pushnumber(L, data->PlayerBaseData->AbilityList[i].DebtExp);
						return 6;
					}
					else
					{
						index--;
					}
				}
			}
		}
		else
		{
			int level = data->PlayerBaseData->AbilityList[index].Level;
			if ( level > 0 )
			{
				lua_pushstring(L, g_ObjectData->GetClassNameByID((Voc_ENUM)index));
				lua_pushstring(L, g_ObjectData->GetClassTokenByID((Voc_ENUM)index));
				lua_pushnumber(L, level);
				lua_pushnumber(L, data->PlayerBaseData->AbilityList[index].EXP);
				lua_pushnumber(L, data->JobExpTable[level - 1]);
				lua_pushnumber(L, data->PlayerBaseData->AbilityList[index].DebtExp);
				return 6;
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ExchangeClass(lua_State* L)
{
	int mainClass = luaL_checkint(L, 1);
	int subClass = EM_Vocation_None;

	if ( lua_isnumber(L, 2) )
		subClass = (int)lua_tonumber(L, 2);
	g_pExchangeClassFrame->ExchangeClass(mainClass - 1, subClass - 1);
	return 0;
}