#include "../mainproc/GameMain.h"
#include "WeekInstance.h"

//********************************************************************
CWeekInstanceFrame* g_pWeekInstanceFrame = NULL;

//********************************************************************
int LuaFunc_WIDisableTag(lua_State* L);
int LuaFunc_WIGetToday(lua_State* L);
int LuaFunc_WIGetShowDay(lua_State* L);
int LuaFunc_WISetShowDay(lua_State* L);
int LuaFunc_WIGetGroupInstanceCount(lua_State* L);
int LuaFunc_WIGetGroupCredit(lua_State* L);
int LuaFunc_WIGetInstance(lua_State* L);
int LuaFunc_WIGetInstanceName(lua_State* L);
int LuaFunc_WIGetInstanceLevel(lua_State* L);
int LuaFunc_WIRequest(lua_State* L);
int LuaFunc_WITeleport(lua_State* L);
int LuaFunc_WIGetResetCost(lua_State* L);
int LuaFunc_WIGetResetCount(lua_State* L);
int LuaFunc_WICheckReset(lua_State* L);
int LuaFunc_WIReset(lua_State* L);
int LuaFunc_WIGetRoleLevel(lua_State* L);

//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CWeekInstanceFrame::CWeekInstanceFrame(CInterface* object)
:CInterfaceBase(object)
{	
	g_pWeekInstanceFrame = this;
	m_Today = EM_WeekInstanceDay_Sunday;
	m_ShowDay = m_Today;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CWeekInstanceFrame::~CWeekInstanceFrame()
{
	g_pWeekInstanceFrame = NULL;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L != NULL)
	{
		lua_register(L, "WIDisableTag"				, LuaFunc_WIDisableTag				);
		lua_register(L, "WIGetToday"				, LuaFunc_WIGetToday				);
		lua_register(L, "WIGetShowDay"				, LuaFunc_WIGetShowDay				);
		lua_register(L, "WISetShowDay"				, LuaFunc_WISetShowDay				);
		lua_register(L, "WIGetGroupInstanceCount"	, LuaFunc_WIGetGroupInstanceCount	);
		lua_register(L, "WIGetGroupCredit"			, LuaFunc_WIGetGroupCredit			);
		lua_register(L, "WIGetInstance"				, LuaFunc_WIGetInstance				);
		lua_register(L, "WIGetInstanceName"			, LuaFunc_WIGetInstanceName			);
		lua_register(L, "WIGetInstanceLevel"		, LuaFunc_WIGetInstanceLevel		);
		lua_register(L, "WIRequest"					, LuaFunc_WIRequest					);
		lua_register(L, "WITeleport"				, LuaFunc_WITeleport				);
		lua_register(L, "WIGetResetCost"			, LuaFunc_WIGetResetCost			);
		lua_register(L, "WIGetResetCount"			, LuaFunc_WIGetResetCount			);
		lua_register(L, "WICheckReset"				, LuaFunc_WICheckReset				);
		lua_register(L, "WIReset"					, LuaFunc_WIReset					);
		lua_register(L, "WIGetRoleLevel"			, LuaFunc_WIGetRoleLevel			);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::Open()
{
	SendWorldEvent(UiEvent_OpenWeekInstanceFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::Close()
{
	SendWorldEvent(UiEvent_CloseWeekInstanceFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WeekInstanceGroup* CWeekInstanceFrame::GetGroup(int id)
{
	WeekInstanceGroup* Result = NULL;

	std::map<int, std::map<int, WeekInstanceGroup>>::iterator itDay = m_Instances.find(m_ShowDay);

	if (itDay != m_Instances.end())
	{
		std::map<int, WeekInstanceGroup>& Groups = itDay->second;
		std::map<int, WeekInstanceGroup>::iterator itGroup = Groups.find(id);

		if (itGroup != Groups.end())
		{
			Result = &itGroup->second;
		}
	}

	return Result;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::RL_WeekInstances(int today, int count, WeekZoneStruct info[])
{
	m_Instances.clear();

	std::map<int, std::vector<GameObjDbStructEx*>> ObjGroups;
	GameObjDbStructEx* pObj = NULL;

	for (int i = 1; i < _DEF_ZONE_BASE_COUNT_; ++i)
	{
		pObj = CNetGlobal::GetObj(i + Def_ObjectClass_Zone);

		if (pObj == NULL)
			continue;

		if (pObj->IsZone() == false)
			continue;

		if (pObj->Zone.MirrorInstanceType <= 0)
			continue;

		ObjGroups[pObj->Zone.MirrorInstanceType].push_back(pObj);
	}

	WeekInstance WI;
	char Buff[128];

	for (int i = 0; i < count; ++i)
	{
		WeekZoneStruct& Info = info[i];

		if ((Info.Day < EM_WeekInstanceDay_Sunday) || (Info.Day > EM_WeekInstanceDay_Saturday))
			continue;

		if ((Info.ZoneGroupID < 1) || (Info.ZoneGroupID > DEF_MAX_WEEK_INS_TYPE))
			continue;

		std::vector<GameObjDbStructEx*>& ObjGroup = ObjGroups[Info.ZoneGroupID];

		WeekInstanceGroup& WIG = m_Instances[Info.Day][Info.ZoneGroupID];

		for (unsigned j = 0; j < ObjGroup.size(); ++j)
		{
			pObj = ObjGroup[j];

			WI.Init();
			WI.ZoneID = (pObj->GUID % Def_ObjectClass_Zone);

			const WorldMapInfoStruct* pInfo = g_ObjectData->GetWorldMapInfoByID(WI.ZoneID);

			if (pInfo != NULL)
			{
				sprintf(Buff, "zone_%s", pInfo->MapName);
				_strupr(Buff);

				WI.Name = g_ObjectData->GetString(Buff);
			}

			WI.Level  = pObj->Zone.MirrorLimitLv;
			WIG.Instances.push_back(WI);
		}
	}

	m_Today = (WeekInstanceDayENUM)today;
	m_ShowDay = m_Today;

	SendWorldEvent(UiEvent_UpdateWeekInstanceFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::RL_WeekInstancesTeleportResult(bool result)
{
	if (result == true)
	{
		SendWorldEvent(UiEvent_CloseWeekInstanceFrame);
	}
	else
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WEEKINSTANCE_TELEPORT_FAIL"));
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CWeekInstanceFrame::RL_WeekInstancesReset(bool result)
{
	if (result == true)
	{
		SendWorldEvent(UiEvent_UpdateWeekInstanceFrame);
		g_pGameMain->SendSystemChatMsg(g_ObjectData->GetString("SYS_WEEKINSTANCE_RESET_OK"));
	}
	else
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WEEKINSTANCE_RESET_FAIL"));
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIDisableTag(lua_State* L)
{
	bool Result  = false;
	bool Flag	 = false;
	bool Disable = false;

	if(g_ObjectData->_DisableTag.size() > 669)
	{
		Disable = g_ObjectData->_DisableTag[669];
	}

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if (pRole != NULL)
	{
		Flag = pRole->BaseData.KeyItem.GetFlag(9372);
	}

	if ((Disable == false) && (Flag == true))
	{
		//顯示
		Result = false;
	}
	else
	{
		//不顯示
		Result = true;
	}

	lua_pushboolean(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetToday(lua_State* L)
{
	int Result = g_pWeekInstanceFrame->Today;
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetShowDay(lua_State* L)
{
	int Result = g_pWeekInstanceFrame->ShowDay;
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WISetShowDay(lua_State* L)
{
	int Day = luaL_checkint(L, 1);
	g_pWeekInstanceFrame->ShowDay = (WeekInstanceDayENUM)Day;
	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetGroupInstanceCount(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1);
	WeekInstanceGroup* pGroup = g_pWeekInstanceFrame->GetGroup(GroupID);
	int Result = (pGroup == NULL) ? 0 : (int)pGroup->Instances.size();
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetGroupCredit(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1) - 1;
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int Credit = (pRole == NULL) ? 0 : pRole->GetValue_Int((RoleValueName_ENUM)(EM_RoleValue_WeekInstnaces+GroupID));
	lua_pushnumber(L, Credit);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetInstance(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1);
	int Index	= luaL_checkint(L, 2) - 1;

	bool Result = false;

	WeekInstanceGroup* pGroup = g_pWeekInstanceFrame->GetGroup(GroupID);

	if (pGroup != NULL)
	{
		if ((Index >= 0) && (Index < (int)pGroup->Instances.size()))
		{
			Result = true;
		}
	}

	lua_pushboolean(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetInstanceName(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1);
	int Index	= luaL_checkint(L, 2) - 1;

	std::string Result = "";

	WeekInstanceGroup* pGroup = g_pWeekInstanceFrame->GetGroup(GroupID);

	if (pGroup != NULL)
	{
		if ((Index >= 0) && (Index < (int)pGroup->Instances.size()))
		{
			Result = pGroup->Instances[Index].Name;
		}
	}

	lua_pushstring(L, Result.c_str());
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetInstanceLevel(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1);
	int Index	= luaL_checkint(L, 2) - 1;

	int Result = 0;

	WeekInstanceGroup* pGroup = g_pWeekInstanceFrame->GetGroup(GroupID);

	if (pGroup != NULL)
	{
		if ((Index >= 0) && (Index < (int)pGroup->Instances.size()))
		{
			Result = pGroup->Instances[Index].Level;
		}
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIRequest(lua_State* L)
{
	NetCli_Other::This->SL_WeekInstancesRequest();
	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WITeleport(lua_State* L)
{
	int GroupID = luaL_checkint(L, 1);
	int Index	= luaL_checkint(L, 2) - 1;

	WeekInstanceGroup* pGroup = g_pWeekInstanceFrame->GetGroup(GroupID);

	if (pGroup != NULL)
	{
		if ((Index >= 0) && (Index < (int)pGroup->Instances.size()))
		{
			WeekInstance& WI = pGroup->Instances[Index];
			NetCli_Other::SL_WeekInstancesTeleport(WI.ZoneID);
		}
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetResetCost(lua_State* L)
{
	int Result = g_ObjectData->GetMoneyKeyValue("WeekInstReset", 0);
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetResetCount(lua_State* L)
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int Result = (pRole == NULL) ? 0 : pRole->GetValue_Int(EM_RoleValue_WeekResetCount);
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WICheckReset(lua_State* L)
{
	bool Result = false;

	if (g_pWeekInstanceFrame->Today == g_pWeekInstanceFrame->ShowDay)
	{
		RoleDataEx* pRole = CNetGlobal::RoleData();

		if (pRole != NULL)
		{
			for(int i = 0; i < DEF_MAX_WEEK_INS_TYPE; ++i)
			{
				int Credit = pRole->GetValue_Int((RoleValueName_ENUM)(EM_RoleValue_WeekInstnaces+i));

				if(Credit != DEF_MAX_WEEK_INS_TYPE_DAY_COUNT)
				{
					Result = true;
					break;
				}
			}
		}
	}

	lua_pushboolean(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIReset(lua_State* L)
{
	RoleDataEx* pRole = CNetGlobal::RoleData();

	if (pRole != NULL)
	{
		int ResetCost = g_ObjectData->GetMoneyKeyValue("WeekInstReset", 0);
		int ResetCount = pRole->GetValue_Int(EM_RoleValue_WeekResetCount);
		int TotalCost = (ResetCount+1)*ResetCost;

		//檢查晶鑽餘額
		if (TotalCost > pRole->BodyMoney_Account())
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNT_ERR1_1"));
		}
		else
		{
			NetCli_Other::This->SL_WeekInstancesReset();
		}
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_WIGetRoleLevel(lua_State* L)
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int Result = (pRole == NULL) ? 0 : pRole->Level();
	lua_pushnumber(L, Result);
	return 1;
}