#include "../mainproc/GameMain.h"
#include "PhantomFrame.h"
#include "ItemClipboard.h"
#include "..\SkillBook\UI_SkillBook.h"

//********************************************************************
CPhantomFrame* g_pPhantomFrame = NULL;

//********************************************************************
int LuaFunc_GetObjName(lua_State* L);
int LuaFunc_GetObjImage(lua_State* L);
int LuaFunc_GetObjShortNote(lua_State* L);
int LuaFunc_GetItemQuality(lua_State* L);
int LuaFunc_GetBodyItemCount(lua_State* L);
int LuaFunc_IsMagicBase(lua_State* L);
int LuaFunc_GetAllPhantom(lua_State* L);
int LuaFunc_GetPhantomImage(lua_State* L);
int LuaFunc_GetPhantomRank(lua_State* L);
int LuaFunc_GetPhantomLevel(lua_State* L);
int LuaFunc_GetPhantomMaxLevel(lua_State* L);
int LuaFunc_GetPhantomAbility(lua_State* L);
int LuaFunc_GetEquipPhantom(lua_State* L);
int LuaFunc_GetPhantomRankUpMaterial(lua_State* L);
int LuaFunc_GetPhantomRankUpCost(lua_State* L);
int LuaFunc_GetPhantomLevelUpMaterial(lua_State* L);
int LuaFunc_GetPhantomComboSkill(lua_State* L);
int LuaFunc_PhantomEquip(lua_State* L);
int LuaFunc_PhantomLevelUp(lua_State* L);
int LuaFunc_PhantomRankUp(lua_State* L);
int LuaFunc_PhantomSummon(lua_State* L);
int LuaFunc_PasteMagicCollectToClipboard(lua_State* L);
int LuaFunc_SetPhantomTab(lua_State* L);

//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int _GetPhantomRankUpMaterial(int orgobjid)
{
	int Result = 0;

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(orgobjid);

	if (pObjDB != NULL)
	{
		if (pObjDB->IsPhantom() == true)
		{
			Result = pObjDB->Phantom.RankUpMaterial;
		}
	}

	return Result;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int _GetPhantomRankUpCost(int orgobjid)
{
	int Result = 0;

	int Idx = orgobjid % 1000;
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		if (Idx < MAX_PHANTOM_COUNT)
		{
			int Rank = pRole->PlayerBaseData->Phantom.Info[Idx].Rank;
			Rank++;

			map< int,PhantomRankTableStruct >::iterator it = RoleDataEx::PhantomRank.find(Rank);

			if (it != RoleDataEx::PhantomRank.end())
			{
				PhantomRankTableStruct& RankInfo = it->second;

				Result = RankInfo.NeedFragment;
			}
		}
	}

	return Result;
}

//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CPhantomFrame::CPhantomFrame(CInterface* object)
:CInterfaceBase(object)
{	
	g_pPhantomFrame = this;
	m_ForceShow	= false;
	m_PhantomTab = NULL;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CPhantomFrame::~CPhantomFrame()
{
	g_pPhantomFrame = NULL;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::Update(float elapsedTime)
{
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if (m_PhantomTab != NULL)
	{
		if (m_ForceShow == true)
		{
			if (m_PhantomTab->IsVisible() == false)
			{
				m_PhantomTab->SetVisible(true);
			}
		}
		else
		{
			bool Flag = false;
			bool Disable = false;

			if(g_ObjectData->_DisableTag.size() > 669)
			{
				Disable = g_ObjectData->_DisableTag[669];
			}

			if (Disable == false)
			{
				RoleDataEx*	pRole = CNetGlobal::RoleData();

				if (pRole != NULL)
				{
					Flag = pRole->BaseData.KeyItem.GetFlag(9372);
				}
			}

			if (Flag == true)
			{
				if (m_PhantomTab->IsVisible() == false)
				{
					m_PhantomTab->SetVisible(true);
				}
			}
			else
			{
				if (m_PhantomTab->IsVisible() == true)
				{
					m_PhantomTab->SetVisible(false);
				}
			}
		}
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L != NULL)
	{
		lua_register(L, "GetObjName"					, LuaFunc_GetObjName					);
		lua_register(L, "GetObjImage"					, LuaFunc_GetObjImage					);
		lua_register(L, "GetObjShortNote"				, LuaFunc_GetObjShortNote				);
		lua_register(L, "GetItemQuality"				, LuaFunc_GetItemQuality				);
		lua_register(L, "GetBodyItemCount"				, LuaFunc_GetBodyItemCount				);
		lua_register(L, "IsMagicBase"					, LuaFunc_IsMagicBase					);
		lua_register(L, "GetAllPhantom"					, LuaFunc_GetAllPhantom					);
		lua_register(L, "GetPhantomImage"				, LuaFunc_GetPhantomImage				);
		lua_register(L, "GetPhantomRank"				, LuaFunc_GetPhantomRank				);
		lua_register(L, "GetPhantomLevel"				, LuaFunc_GetPhantomLevel				);
		lua_register(L, "GetPhantomMaxLevel"			, LuaFunc_GetPhantomMaxLevel			);
		lua_register(L, "GetPhantomAbility"				, LuaFunc_GetPhantomAbility				);
		lua_register(L, "GetEquipPhantom"				, LuaFunc_GetEquipPhantom				);
		lua_register(L, "GetPhantomRankUpMaterial"		, LuaFunc_GetPhantomRankUpMaterial		);
		lua_register(L, "GetPhantomRankUpCost"			, LuaFunc_GetPhantomRankUpCost			);
		lua_register(L, "GetPhantomLevelUpMaterial"		, LuaFunc_GetPhantomLevelUpMaterial		);
		lua_register(L, "GetPhantomComboSkill"			, LuaFunc_GetPhantomComboSkill			);
		lua_register(L, "PhantomEquip"					, LuaFunc_PhantomEquip					);
		lua_register(L, "PhantomLevelUp"				, LuaFunc_PhantomLevelUp				);
		lua_register(L, "PhantomRankUp"					, LuaFunc_PhantomRankUp					);
		lua_register(L, "PhantomSummon"					, LuaFunc_PhantomSummon					);
		lua_register(L, "PasteMagicCollectToClipboard"	, LuaFunc_PasteMagicCollectToClipboard	);
		lua_register(L, "SetPhantomTab"					, LuaFunc_SetPhantomTab					);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::Open()
{
	SendWorldEvent(UiEvent_OpenPhantomFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::Close()
{
	SendWorldEvent(UiEvent_ClosePhantomFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_PhantomRankUpResult(bool summon, bool result)
{
	std::string Msg = "";

	if (result == true)
	{
		if (summon == true)
		{			
			Msg = g_ObjectData->GetString("SYS_PHANTOM_SUMMON_OK");

			if (Msg.empty())
			{
				Msg = "SYS_PHANTOM_SUMMON_OK";
			}

			g_pGameMain->SendSystemChatMsg(Msg.c_str());			
		}
		else
		{
			Msg = g_ObjectData->GetString("SYS_PHANTOM_RANK_UP_OK");

			if (Msg.empty())
			{
				Msg = "SYS_PHANTOM_RANK_UP_OK";
			}

			g_pGameMain->SendSystemChatMsg(Msg.c_str());
		}
	}
	else
	{
		if (summon == true)
		{
			Msg = g_ObjectData->GetString("SYS_PHANTOM_SUMMON_FAIL");

			if (Msg.empty())
			{
				Msg = "SYS_PHANTOM_SUMMON_FAIL";
			}

			g_pGameMain->SendWarningMsg(Msg.c_str());
		}
		else
		{
			Msg = g_ObjectData->GetString("SYS_PHANTOM_RANK_UP_FAIL");

			if (Msg.empty())
			{
				Msg = "SYS_PHANTOM_RANK_UP_FAIL";
			}

			g_pGameMain->SendWarningMsg(Msg.c_str());
		}
	}

	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_PhantomLevelUpResult(bool result)
{
	std::string Msg = "";

	if (result == true)
	{
		Msg = g_ObjectData->GetString("SYS_PHANTOM_LEVEL_UP_OK");

		if (Msg.empty())
		{
			Msg = "SYS_PHANTOM_LEVEL_UP_OK";
		}

		g_pGameMain->SendSystemChatMsg(Msg.c_str());
	}
	else
	{
		Msg = g_ObjectData->GetString("SYS_PHANTOM_LEVEL_UP_FAIL");

		if (Msg.empty())
		{
			Msg = "SYS_PHANTOM_LEVEL_UP_FAIL";
		}

		g_pGameMain->SendWarningMsg(Msg.c_str());
	}

	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_PhantomEQResult(bool result)
{
	std::string Msg = "";

	if (result == true)
	{
		Msg = g_ObjectData->GetString("SYS_PHANTOM_EQUIP_OK");

		if (Msg.empty())
		{
			Msg = "SYS_PHANTOM_EQUIP_OK";
		}

		g_pGameMain->SendSystemChatMsg(Msg.c_str());
	}
	else
	{
		Msg = g_ObjectData->GetString("SYS_PHANTOM_EQUIP_FAIL");

		if (Msg.empty())
		{
			Msg = "SYS_PHANTOM_EQUIP_FAIL";
		}

		g_pGameMain->SendWarningMsg(Msg.c_str());
	}

	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_FixPhantom(int phantomid)
{
	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_FixPhantomEQ()
{
	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::RL_OpenPhantomFrame()
{
	m_ForceShow = true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPhantomFrame::R_FixItemInfo_Body()
{
	SendWorldEvent(UiEvent_UpdatePhantomFrame);
	SendWorldEvent(UiEvent_UpdatePhantomBagFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetObjName(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	std::string Result = "";

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		char Buff[64];
		sprintf(Buff, "Sys%d_name", OrgObjID);
		Result = g_ObjectData->GetString(Buff);
	}

	lua_pushstring(L, Result.c_str());
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetObjImage(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	std::string Result = "";

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		GameObjDbStructEx* pImageDB = g_ObjectData->GetObj(pObjDB->ImageID);

		if (pImageDB != NULL)
		{
			if (pImageDB->IsImage() == true)
			{
				Result = pImageDB->Image.ACTField;
			}
		}
	}

	lua_pushstring(L, Result.c_str());
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetObjShortNote(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	std::string Result = "";

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		Result = pObjDB->ShortNote;
	}

	lua_pushstring(L, Result.c_str());

	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetItemQuality(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	int Result = 0;

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		if (pObjDB->IsItem() == true)
		{
			Result = g_ObjectData->GetItemQuality(pObjDB, NULL);
		}
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetBodyItemCount(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	int Result = 0;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if (pRole != NULL)
	{
		Result = pRole->CalBodyItemCount(OrgObjID);
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_IsMagicBase(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	bool Result = false;

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		Result = pObjDB->IsMagicBase();
	}

	lua_pushboolean(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetAllPhantom(lua_State* L)
{
	lua_newtable(L);

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		int OrgObjID = 0;
		int TableKey = 1;

		for (int i = 0; i < MAX_PHANTOM_COUNT; ++i)
		{
			PhantomBase& Info = pRole->PlayerBaseData->Phantom.Info[i];

			OrgObjID = Def_ObjectClass_Phantom + i;

			GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

			if (pObjDB != NULL)
			{
				if (pObjDB->IsPhantom() == true)
				{
					if (pObjDB->Phantom.Release == true)
					{
						lua_pushnumber(L, TableKey);
						lua_pushnumber(L, OrgObjID);
						lua_settable(L, -3);

						TableKey++;
					}
				}
			}
		}
	}

	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomImage(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	std::string Result = "";

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		if (pObjDB->IsPhantom() == true)
		{
			GameObjDbStructEx* pImageDB = g_ObjectData->GetObj(pObjDB->ImageID);

			if (pImageDB != NULL)
			{
				if (pImageDB->IsImage() == true)
				{
					Result = pImageDB->Image.PhantomImg;
				}
			}
		}
	}

	lua_pushstring(L, Result.c_str());
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomRank(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	int Result = 0;
	int Idx = OrgObjID % 1000;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		if (Idx < MAX_PHANTOM_COUNT)
		{
			Result = pRole->PlayerBaseData->Phantom.Info[Idx].Rank;
		}
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomLevel(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	int Result = 0;
	int Idx = OrgObjID % 1000;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		if (Idx < MAX_PHANTOM_COUNT)
		{
			Result = pRole->PlayerBaseData->Phantom.Info[Idx].Lv;
		}
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomMaxLevel(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	int Result = 0;
	int Idx = OrgObjID % 1000;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		if (Idx < MAX_PHANTOM_COUNT)
		{
			int Rank = pRole->PlayerBaseData->Phantom.Info[Idx].Rank;

			for (unsigned i = 0; i < RoleDataEx::PhantomRank.size(); ++i)
			{
				PhantomRankTableStruct& RankInfo = RoleDataEx::PhantomRank[i];

				if (RankInfo.Rank == Rank)
				{
					Result = RankInfo.MaxLevel;
					break;
				}
			}
		}
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomAbility(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	int Idx		 = luaL_checkint(L, 2)-1;

	float		Result1 = 0;
	std::string Result2 = "";

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		if (pObjDB->IsPhantom() == true)
		{
			RoleDataEx*	pRole = CNetGlobal::RoleData();

			if(pRole != NULL)
			{
				int Pos = OrgObjID % 1000;

				if (Pos < MAX_PHANTOM_COUNT)
				{
					WearEqTypeENUM Type = pObjDB->Phantom.abi_Phantom.EqType[Idx];					

					if((Type > 0) && (Type < EM_WearEqType_MaxCount))
					{
						//璸衡そΑ = 把计 * 单 * 单把计 * ( 1 + 顶 * 顶把计 ) * 瞯
						float Value   = (float)pObjDB->Phantom.abi_Phantom.EqTypeValue[Idx];
						float Lv	  = (float)pRole->PlayerBaseData->Phantom.Info[Pos].Lv;
						float Rank	  = (float)pRole->PlayerBaseData->Phantom.Info[Pos].Rank;
						float LvArg   = pRole->FixArg().PhantomLvArg;
						float RankArg = pRole->FixArg().PhantomRankArg;
						float AbiArg  = RoleDataEx::PhantomAbiArg[Type];

						if (Lv == 0)
							Lv = 1;

						if (Rank == 0)
							Rank = 1;

						Result1 = (Value * Lv * LvArg * (1.0f + (Rank * RankArg)) * AbiArg);

						char Buff[256];
						g_ObjectData->GetWearEqTypeString(Type, Result1, Buff, 256);
						Result2 = Buff;
					}
				}
			}
		}
	}

	lua_pushnumber(L, Result1);
	lua_pushstring(L, Result2.c_str());
	return 2;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetEquipPhantom(lua_State* L)
{
	int Result1 = 0;
	int Result2 = 0;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		Result1 = pRole->PlayerBaseData->Phantom.EQ[0];
		Result2 = pRole->PlayerBaseData->Phantom.EQ[1];
	}

	lua_pushnumber(L, Result1);
	lua_pushnumber(L, Result2);
	return 2;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomRankUpMaterial(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	int Result = _GetPhantomRankUpMaterial(OrgObjID);
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomRankUpCost(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	int Result = _GetPhantomRankUpCost(OrgObjID);
	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomLevelUpMaterial(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	lua_newtable(L);

	int Idx = OrgObjID % 1000;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		if (Idx < MAX_PHANTOM_COUNT)
		{
			int Lv = pRole->PlayerBaseData->Phantom.Info[Idx].Lv;
			Lv++;

			GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

			if (pObjDB != NULL)
			{
				if (pObjDB->IsPhantom() == true)
				{
					int Type = pObjDB->Phantom.LevelUpType + 1;

					std::map<int, std::map<int, PhantomLvTableStruct>>::iterator itType = RoleDataEx::PhantomLv.find(Type);

					if (itType !=  RoleDataEx::PhantomLv.end())
					{
						std::map<int, PhantomLvTableStruct>& LvTable = itType->second;
						std::map<int, PhantomLvTableStruct>::iterator itLvTable = LvTable.find(Lv);

						if (itLvTable != LvTable.end())
						{
							PhantomLvTableStruct& LvInfo = itLvTable->second;

							int TableKey = 1;

							for (int j = 0; j < 4; ++j)
							{
								if (LvInfo.Material[j] > 0)
								{
									lua_pushnumber(L, TableKey);
									lua_pushnumber(L, LvInfo.Material[j]);
									lua_settable(L, -3);
									TableKey++;
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_GetPhantomComboSkill(lua_State* L)
{
	int Result = 0;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if (pRole != NULL)
	{
		//key = id + id * 0x100000000
		int EQ1 = pRole->PlayerBaseData->Phantom.EQ[0];
		int EQ2 = pRole->PlayerBaseData->Phantom.EQ[1];

		INT64 TableID = 0;
		if (EQ1 > EQ2)
		{
			TableID = EQ1 + EQ2 * 0x100000000;
		}
		else
		{
			TableID = EQ2 + EQ1 * 0x100000000;
		}

		Result = RoleDataEx::PhantomSkill[TableID];
	}

	lua_pushnumber(L, Result);
	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_PhantomEquip(lua_State* L)
{
	int Slot	 = luaL_checkint(L, 1);
	int OrgObjID = luaL_checkint(L, 2);

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole != NULL)
	{
		int EQ[2];
		EQ[0] = pRole->PlayerBaseData->Phantom.EQ[0];
		EQ[1] = pRole->PlayerBaseData->Phantom.EQ[1];

		switch (Slot)
		{
		case 1:
			EQ[0] = OrgObjID;
			break;
		case 2:
			EQ[1] = OrgObjID;
			break;
		}

		NetCli_Other::SL_PhantomEQ(EQ);
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_PhantomLevelUp(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	NetCli_Other::SL_PhantomLevelUp(OrgObjID);
	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_PhantomRankUp(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	NetCli_Other::SL_PhantomRankUp(OrgObjID, false);
	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_PhantomSummon(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);
	NetCli_Other::SL_PhantomRankUp(OrgObjID, true);
	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_PasteMagicCollectToClipboard(lua_State* L)
{
	int OrgObjID = luaL_checkint(L, 1);

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(OrgObjID);

	if (pObjDB != NULL)
	{
		if (pObjDB->IsMagicCollect() == true)
		{
			ClipboardItemType ItemType = g_pItemClipboard->GetType();

			if (ItemType == eClipboardItem_Type_None)
			{
				g_pItemClipboard->SetFileFromImageID(pObjDB->ImageID);
				g_pItemClipboard->PickupSkill(OrgObjID);
			}
		}
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_SetPhantomTab(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	g_pPhantomFrame->SetPhantomTab(layout);
	return 0;
}