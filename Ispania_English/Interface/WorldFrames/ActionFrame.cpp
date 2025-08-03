#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "../skillbook/UI_SkillBook.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "CharacterFrame.h"
#include "MacroFrame.h"
#include "EmoteFrame.h"
#include "ActionFrame.h"
#include "SuitSkillFrame.h"
#include "..\TransportBook\TeleportBook.h"
#include "PartnerFrame.h"

#include "../../tools/Common/Com_Function.h"

#define ACTION_SAVE_TIME					300.0f							// 固定時間儲存

int LuaFunc_HasAction					(lua_State* L);
int LuaFunc_PickupAction				(lua_State* L);
int LuaFunc_UseAction					(lua_State* L);
int LuaFunc_GetActionInfo				(lua_State* L);
int LuaFunc_GetActionCooldown			(lua_State* L);
int LuaFunc_GetActionUsable				(lua_State* L);
int LuaFunc_GetActionWearable			(lua_State* L);

// Pet
int	LuaFunc_GetPetActionInfo			(lua_State* L);
int LuaFunc_GetPetActionCooldown		(lua_State* L);
int LuaFunc_UsePetAction				(lua_State* L);

// Extra
int LuaFunc_GetExtraActionInfo			(lua_State* L);
int LuaFunc_GetExtraActionCooldown		(lua_State* L);
int LuaFunc_UseExtraAction				(lua_State* L);

int LuaFunc_SetActionBarSetting			(lua_State* L);
int LuaFunc_GetActionBarSetting			(lua_State* L);
int LuaFunc_SetActionBarLocked			(lua_State* L);
int LuaFunc_GetActionBarLocked			(lua_State* L);
int LuaFunc_SetActionBarHideEmpty		(lua_State* L);
int LuaFunc_GetActionBarHideEmpty		(lua_State* L);
int	LuaFunc_ResetActionSetting			(lua_State* L);

// Implement
int LuaFunc_GetImplementActionInfo		(lua_State* L);
int LuaFunc_GetImplementActionCooldown	(lua_State* L);
int LuaFunc_UseImplementAction			(lua_State* L);

CActionFrame*				g_pActionFrame = NULL;

struct SaveActionHeader
{
	int size;
	int classNum;
};

// ----------------------------------------------------------------------------
CActionFrame::CActionFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pActionFrame = this;

	ClearWaitingSpellID();

	memset(m_items, 0, sizeof(m_items));
	memset(m_petItems, 0, sizeof(m_petItems));

	m_setting.Init();
	m_settingBackup.Init();

	m_lastSaveSettingTime	= 0.0f;

	m_waitingType		= 0;

	m_extraItems.clear();
	m_implementItems.clear();
}

// ----------------------------------------------------------------------------
CActionFrame::~CActionFrame()
{
	g_pActionFrame = NULL;
}

// ----------------------------------------------------------------------------
void CActionFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "HasAction",				LuaFunc_HasAction);		
		lua_register(L, "PickupAction",				LuaFunc_PickupAction);
		lua_register(L, "UseAction",				LuaFunc_UseAction);
		lua_register(L, "GetActionInfo",			LuaFunc_GetActionInfo);		
		lua_register(L, "GetActionCooldown",		LuaFunc_GetActionCooldown);		
		lua_register(L, "GetActionUsable",			LuaFunc_GetActionUsable);
		lua_register(L, "GetActionWearable",		LuaFunc_GetActionWearable);

		lua_register(L, "GetPetActionInfo",			LuaFunc_GetPetActionInfo);
		lua_register(L, "GetPetActionCooldown",		LuaFunc_GetPetActionCooldown);
		lua_register(L, "UsePetAction",				LuaFunc_UsePetAction);

		lua_register(L, "GetExtraActionInfo",		LuaFunc_GetExtraActionInfo);
		lua_register(L, "GetExtraActionCooldown",	LuaFunc_GetExtraActionCooldown);
		lua_register(L, "UseExtraAction",			LuaFunc_UseExtraAction);
		
		lua_register(L, "SetActionBarSetting",		LuaFunc_SetActionBarSetting);
		lua_register(L, "GetActionBarSetting",		LuaFunc_GetActionBarSetting);
		lua_register(L, "SetActionBarLocked",		LuaFunc_SetActionBarLocked);
		lua_register(L, "GetActionBarLocked",		LuaFunc_GetActionBarLocked);
		lua_register(L, "SetActionBarHideEmpty",	LuaFunc_SetActionBarHideEmpty);
		lua_register(L, "GetActionBarHideEmpty",	LuaFunc_GetActionBarHideEmpty);
		lua_register(L, "ResetActionSetting",		LuaFunc_ResetActionSetting);

		lua_register(L, "GetImplementActionInfo",		LuaFunc_GetImplementActionInfo);
		lua_register(L, "GetImplementActionCooldown",	LuaFunc_GetImplementActionCooldown);
		lua_register(L, "UseImplementAction",			LuaFunc_UseImplementAction);
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::DestroyFrames()
{
	Save();

	memset(m_items, 0, sizeof(m_items));

	m_extraItems.clear();
}

// ----------------------------------------------------------------------------
void CActionFrame::Update(float elapsedTime)
{
	for ( int i = 0; i < PETACTIONBAR_MAX_SKILLS; i++ )
	{
		if ( m_petItems[i].coldownTime > 0.0f )
			m_petItems[i].coldownTime -= elapsedTime;

		if ( m_petItems[i].coldownTime < 0.0f )
			m_petItems[i].coldownTime = 0.0f;
	}

	SaveClientSetting();
}

// ----------------------------------------------------------------------------
void CActionFrame::SaveClientSetting(bool Immediately)
{
	if ( memcmp(&m_setting, &m_settingBackup, sizeof(ActionBarSetting)) != 0 )
	{
		if ( Immediately || g_pGameMain->GetFrameTime() - m_lastSaveSettingTime > 5.0f )
		{
			g_pGameMain->SaveClientData(eSaveClientData_ActionBarSetting, sizeof(m_setting), (char*)&m_setting);

			m_settingBackup = m_setting;

			m_lastSaveSettingTime = g_pGameMain->GetFrameTime();
		}
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::Cut(int index)
{
	ActionItem2* item = GetItem(index);
	if ( item )
	{
		GameObjDbStruct* itemDB = NULL;

		switch( item->type )
		{
		case eActionItemType_None:
			break;

		case eActionItemType_Bag:
		case eActionItemType_Equipment:
			if ( (itemDB = CNetGlobal::GetObj(item->objectID)) != NULL )
			{
				g_pItemClipboard->SetFileFromImageID(itemDB->ImageID);
				g_pItemClipboard->PickupAction(item->type, item->objectID, item->createTime, item->serial);
			}
			break;

		case eActionItemType_Skill:
			if ( (itemDB = CNetGlobal::GetObj(item->skillID)) != NULL )
			{
				char fileName[256];
				memset(fileName, 0, sizeof(fileName));

				switch (item->skillID)
				{
				case DF_SKILLID_ATTACK:
					{
						CUI_SkillBook::GetAttackIcon( fileName );
					}
					break;

				default:
					{
						GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
						if ( imageDB )
						{
							strcpy( fileName, imageDB->Image.ACTField );
						}
					}
					break;
				}
				g_pItemClipboard->SetFile(fileName);
				g_pItemClipboard->PickupAction(item->type, item->skillID);
			}
			break;

		case eActionItemType_Macro:
			{
				MacroItem* macroItem = g_pMacroFrame->GetMacroItemById(item->macroID);
				if ( macroItem )
				{
					g_pItemClipboard->SetFile(g_pMacroFrame->GetMacroIcon(macroItem->icon));
					g_pItemClipboard->PickupAction(item->type, item->macroID);
				}
			}
			break;

		case eActionItemType_Emote:
			{
				SEmoteItem* emoteItem = g_pEmoteFrame->GetEmoteItemByID( item->emoteID );
				if ( emoteItem )
				{
					g_pItemClipboard->SetFile( g_pEmoteFrame->GetEmoteIcon( emoteItem ) );
					g_pItemClipboard->PickupAction(item->type, item->emoteID);
				}
			}
			break;

		case eActionItemType_Teleport:
			{
				LocationSaveStruct*Item=g_pTeleportBook->GetItem(item->teleportID);
				
				if ( Item )
				{
					char temp[512];
					g_pTeleportBook->GetItemIcon( Item->IconType,temp ) ;
					g_pItemClipboard->SetFile(temp );
					g_pItemClipboard->PickupAction(item->type, item->teleportID);
				}
			}
			break;

		case eActionItemType_SuitSkill:
			if ( (itemDB = CNetGlobal::GetObj(item->suitskillID)) != NULL )
			{
				g_pItemClipboard->SetFileFromImageID(itemDB->ImageID);				
				g_pItemClipboard->PickupAction(item->type, item->suitskillID);
			}
			break;	
		case eActionItemType_SkillPlate:
			if( (itemDB = CNetGlobal::GetObj(item->skillplateID)) != NULL)
			{
				g_pItemClipboard->SetFileFromImageID(itemDB->ImageID);				
				g_pItemClipboard->PickupAction(item->type, item->skillplateID);
			}
			break;

		case eActionItemType_HappyPartner:
			{
				g_pPartnerFrame->PickupAction(item->PartnerMode,item->PartnerPos);
			}
			break;

		case eActionItemType_CoolSuit:
			{
				g_pCharacterFrame->PickupCoolSuit( item->coolSuitIndex );
			}
			break;
		}

	}
	memset(item, 0, sizeof(ActionItem));	
}

// ----------------------------------------------------------------------------
void CActionFrame::Paste(int index)
{
	ActionItem2* action = GetItem(index);
	GameObjDbStructEx* itemDB;
	if ( action )
	{
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
		ClipboardItemType itemType = g_pItemClipboard->GetType();

		ItemFieldStruct* item = NULL;
		switch(itemType)
		{	
		case eClipboardItem_Type_None:			// 拿取物品
			Cut(index);
			SlotChanged(index);
			return;
		case eClipboardItem_Type_SkillPlate:
		case eClipboardItem_Type_Bag:			// 背包 -> 快捷
		case eClipboardItem_Type_Equipment:		// 裝備 -> 快捷
		case eClipboardItem_Type_Action:		// 快捷 -> 快捷
		case eClipboardItem_Type_Skill:			// 技能 -> 快捷
		case eClipboardItem_Type_Macro:			// 巨集 -> 快捷
		case eClipboardItem_Type_Emote:			// emote -> 快捷
		case eClipboardItem_Type_Teleport:		// 傳送鈕 -> 快捷
		case eClipboardItem_Type_SuitSkill:		// 套裝技能 -> 快捷
		case eClipboardItem_Type_HappyPartner:		// 夥伴 -> 快捷
		case eClipboardItem_Type_CoolSuit:		// 酷套裝 -> 快捷
  			if ( action->type != eActionItemType_None )
				Cut(index);
			else
				g_pItemClipboard->Clear();

			switch (itemType)
			{
			case eClipboardItem_Type_Bag:			// 背包 -> 快捷
				if ( (item = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos)) != NULL )
				{
					action->type		= eActionItemType_Bag;
					action->objectID	= item->OrgObjID;
					action->createTime	= item->CreateTime;
					action->serial		= item->Serial;
				}
				break;

			case eClipboardItem_Type_Equipment:		// 裝備 -> 快捷
				if ( (item = CNetGlobal::RoleData()->GetEqItem(itemInfo.equipment.pos)) != NULL )
				{
					action->type		= eActionItemType_Equipment;
					action->objectID	= item->OrgObjID;
					action->createTime	= item->CreateTime;
					action->serial		= item->Serial;
				}
				break;

			case eClipboardItem_Type_Action:		// 快捷 -> 快捷
				action->type = itemInfo.action.type;
				switch (action->type)
				{
				case eActionItemType_None:
					break;

				case eActionItemType_Bag:
				case eActionItemType_Equipment:
					action->objectID	= itemInfo.action.id;
					action->createTime	= itemInfo.action.arg[0];
					action->serial		= itemInfo.action.arg[1];
					break;

				case eActionItemType_Skill:
					action->skillID	= itemInfo.action.id;
					break;

				case eActionItemType_Macro:
					action->macroID	= itemInfo.action.id;
					break;

				case eActionItemType_Emote:
					action->emoteID	= itemInfo.action.id;
					break;
				case eActionItemType_Teleport:
					action->teleportID	=itemInfo.action.id;
					break;
				case eActionItemType_SuitSkill:
					action->suitskillID = itemInfo.action.id;
					break;
				case eActionItemType_SkillPlate:
					action->skillplateID = itemInfo.action.id;
					break;

				case eActionItemType_HappyPartner: // 夥伴 -> 快捷
					action->PartnerPos	=itemInfo.action.arg[0];
					action->PartnerMode	=itemInfo.action.id ;
					break;
				case eActionItemType_CoolSuit:
					action->coolSuitIndex	= itemInfo.action.id;
					break;

				}
				break;

			case eClipboardItem_Type_Skill:			// 技能 -> 快捷
				action->type		= eActionItemType_Skill;
				action->skillID		= itemInfo.skill.skillID;
				break;

			case eClipboardItem_Type_Macro:			// 巨集 -> 快捷
				action->type		= eActionItemType_Macro;
				action->macroID		= itemInfo.macro.id;
				break;

			case eClipboardItem_Type_Emote:			// emote -> 快捷
				action->type		= eActionItemType_Emote;
				action->emoteID		= itemInfo.emote.id;
				break;
			case eClipboardItem_Type_Teleport:			// 傳點 -> 快捷
				action->type		= eActionItemType_Teleport;
				action->teleportID	= itemInfo.teleport.pos;
				break;

			case eClipboardItem_Type_SuitSkill:		// 套裝技能 -> 快捷
				itemDB= CNetGlobal::GetObj(itemInfo.SuitSkill.skillID);
				if( itemDB->MagicCol.EffectType != EM_Magic_Eq )
				{
					action->type		= eActionItemType_SuitSkill;
					action->suitskillID	= itemInfo.SuitSkill.skillID;
				}
				else{
					g_pItemClipboard->Clear();
				}
				break;
			
			case eClipboardItem_Type_SkillPlate:
				itemDB= CNetGlobal::GetObj(itemInfo.SkillPlate.skillID);
				if( itemDB->MagicCol.EffectType != EM_Magic_Eq )
				{
					action->type		= eActionItemType_SkillPlate;
					action->suitskillID	= itemInfo.SkillPlate.skillID;
					g_pSuitSkillFrame->SetSkillPlateOldID(itemInfo.SkillPlate.skillID , itemInfo.SkillPlate.pos);	
				}	
				else{
					g_pItemClipboard->Clear();
				}
				break;

			case eClipboardItem_Type_HappyPartner:			// 夥伴 -> 快捷
				action->type		= eActionItemType_HappyPartner;
				action->PartnerMode	= itemInfo.partner.mode;
				action->PartnerPos	= itemInfo.partner.pos;
				break;

			case eClipboardItem_Type_CoolSuit: //酷套裝 -> 快捷
				action->type = eActionItemType_CoolSuit;
				action->coolSuitIndex = itemInfo.CoolSuit.index;
			}
			SlotChanged(index);
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::Use(int index)
{
	int count,i,k=0;
	if ( g_pGameMain->IsUIUpdate() )
		return;
	/*
	if ( IsUpdate() )
		return;
	*/

	ActionItem2* item = GetItem(index);
	if ( item )
	{		
		RoleDataEx* roleData = CNetGlobal::RoleData();

		//if ( !g_pItemClipboard->IsEmpty() )
		//	Paste(index);
		//else
		{
			switch(item->type)
			{
			case eActionItemType_None:
				break;

			case eActionItemType_Bag:
			case eActionItemType_Equipment:
				{					
					GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->objectID);
					if ( itemDB )
					{
						int position = -1;
						int type = 0;				// 0背包 1裝備
						
						// 非堆疊物品,並且有物品唯一編號
						if ( itemDB->MaxHeap <= 1 && item->createTime != 0 )
						{
							ItemFieldStruct* itemField = NULL;

							// 搜尋裝備
							if ( (position = g_pCharacterFrame->SearchEquip(item->createTime, item->serial)) > 0 ) {
								type = 1;
							} else if ( (position = g_pBagFrame->SearchItem(item->createTime, item->serial)) > 0 ) {
								type = 0;
							}
						}

						// 若物品索引編號為不存在
						if ( position == -1 ) {
							// 使用物品唯一編號搜尋
							position = g_pBagFrame->GetIndex(item->objectID);
						}

						// 物品是否存在
						if ( position == -1 )
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USE_NOT_EXIST"));
						}
						else
						{
							switch(type)
							{
							// 背包
							case 0:
								g_pBagFrame->Use(position);
								break;

							// 裝備
							case 1:
								g_pCharacterFrame->Use(position);
								break;
							}
						}
					}
				}
				break;

			case eActionItemType_Skill:
				g_pUi_SkillBook->Use( item->skillID );
				break;

			case eActionItemType_Macro:
				g_pMacroFrame->UseMacro( item->macroID );
				break;

			case eActionItemType_Emote:
				g_pEmoteFrame->DoEmote( item->emoteID );
				break;
			case eActionItemType_Teleport:
				if ( g_pTeleportBook->CanUseTeleport(item->teleportID))
					g_pTeleportBook->DoTelePortCasting( item->teleportID );
				break;
			case eActionItemType_HappyPartner:
				g_pPartnerFrame->CallPartner( item->PartnerMode,item->PartnerPos );
				break;


			case eActionItemType_SuitSkill:
				count = CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.MaxEqSkillCount;
				for( i = 0 ; i < count ; i++ )
				{
					if( item->suitskillID == CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.EQ[i] )
					{
						g_pUi_SkillBook->Use( item->suitskillID );
						k = 1;
						break;
					}
				}
				if( k == 0)
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_LIST_WRONG"));
				}  //WARRING_SKILL_LIST_WRONG
				//顯示紅色訊息'此技能尚未裝備於套裝技能盤中，無法使用'
				break;

			case eActionItemType_SkillPlate:
				count = CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.MaxEqSkillCount;
				
				for( i = 0 ; i < count ; i++ )
				{
					if( item->suitskillID == CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.EQ[i] )
					{
						g_pUi_SkillBook->Use( item->skillplateID );
						k = 1;
						break;
					}
				}
				if( k == 0)
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_LIST_WRONG"));
				}
				//顯示紅色訊息'此技能尚未裝備於套裝技能盤中，無法使用'
				break;

			case eActionItemType_CoolSuit:
				g_pCharacterFrame->SetCoolSuitIndex( item->coolSuitIndex );
				break;
			}
		}
		SlotChanged(index);
	}
}

// ----------------------------------------------------------------------------
ActionItem2* CActionFrame::GetItem(int index)
{
	if (index < 0 || index >= ACTION_BAR_MAX_ITEMS)
		return NULL;

	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	int voc;
	if ( sprite )
		voc = sprite->GetVocation();
	else
		voc = CNetGlobal::RoleData()->BaseData.Voc;

	return &m_items[voc][index];
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdateCooldown(int ColdownClass, int ColdownType)
{
	SendWorldEvent("ACTIONBAR_UPDATE_COOLDOWN");
}

// ----------------------------------------------------------------------------
void CActionFrame::DefaultAction()
{
	GameObjDbStruct* itemDB = NULL;
	for ( int i = 0; i < ACTION_BAR_MAX_CLASS; i++ )
	{
		int count = 1;

		// 一般攻擊
		m_items[i][0].type = eActionItemType_Skill;
		m_items[i][0].skillID = DF_SKILLID_ATTACK;		

		itemDB = CNetGlobal::GetObj(Def_ObjectClass_LearnMagic + i);
		if ( itemDB )
		{
			for ( int j = 0; j < itemDB->LearnMagic.NormalSkillCount; j++ )
			{
				JobSkillBaseStruct& jobSkill = itemDB->LearnMagic.NormalSkill[j];
				if ( jobSkill.LearnLV <= 1 && jobSkill.KeyItemID == 0 )
				{
					GameObjDbStruct* magicDB = CNetGlobal::GetObj(jobSkill.MagicID);
					if ( magicDB && magicDB->MagicCol.EffectType != EM_Magic_Eq && magicDB->MagicCol.PetSkillType == -1 )
					{
						m_items[i][count].type = eActionItemType_Skill;
						m_items[i][count].skillID = jobSkill.MagicID;
						count++;
					}
				}
			}

			for ( int j = 0; j < itemDB->LearnMagic.SPSkillCount; j++ )
			{
				JobSkillBaseStruct& jobSkill = itemDB->LearnMagic.SPSkill[j];
				if ( jobSkill.LearnLV <= 1 && jobSkill.KeyItemID == 0 )
				{
					GameObjDbStruct* magicDB = CNetGlobal::GetObj(jobSkill.MagicID);
					if ( magicDB && magicDB->MagicCol.EffectType != EM_Magic_Eq && magicDB->MagicCol.PetSkillType == -1 )
					{
						m_items[i][count].type = eActionItemType_Skill;
						m_items[i][count].skillID = jobSkill.MagicID;
						count++;
					}
				}
			}
		}	

		count = 11;

		// 搜尋建角基本物品資訊
		int index = Def_ObjectClass_CreateRole;
		itemDB = CNetGlobal::GetObj(index);
		while ( itemDB ) {
			if ( itemDB->CreateRole.Job == i ) {
				for ( int n = 0; n < 5; n++ ) {
					int id = itemDB->CreateRole.ItemID[n];
					if ( id <= 0 )
						continue;
					
					if ( id > DF_SKILLID_ATTACK ) {
						m_items[i][count].type = eActionItemType_Skill;
						m_items[i][count].skillID = id;
					} else {
						m_items[i][count].type = eActionItemType_Bag;
						m_items[i][count].objectID = id;
					}					
					count--;
				}				
				break;
			}

			index++;
			itemDB = CNetGlobal::GetObj(index);
		}
	}
}

// ----------------------------------------------------------------------------
// 要求 Server 儲存角色資料
void CActionFrame::SaveClientData()
{
	// 產生暫存資料
	CTempFile tempFile(65536);

	int version, itemSize;	

	// 寫入識別編號
	version = ACTION_ITEM_VERSION;
	tempFile.Write(&version, sizeof(int));

	// 寫入結構大小
	itemSize = sizeof(m_items);
	tempFile.Write(&itemSize, sizeof(int));	

	// 儲存整筆資料
	tempFile.Write(m_items, sizeof(m_items));
	
	g_pGameMain->SaveClientData(eSaveClientData_Action, tempFile.GetPosition(), tempFile.GetData());

	// 將最後存檔時間去除
	SetSaveUpdateTime(0.0f);
}

// ----------------------------------------------------------------------------
// 讀取角色資料
void CActionFrame::LoadClientData(int type, int size, char* data)
{
	// 熱鍵資料
	if ( type == eSaveClientData_Action )
	{
		// 產生暫存資料
		CTempFile tempFile(data,size);

		int version, itemSize;

		// 比對版本編號以及結構大小
		tempFile.Read(&version, sizeof(int));
		tempFile.Read(&itemSize, sizeof(int));
		if ( version != ACTION_ITEM_VERSION || itemSize != sizeof(m_items) )
			return;

		// 讀取熱鍵整筆結構
		tempFile.Read(m_items, sizeof(m_items));
	}

	// 快捷設定
	if ( type == eSaveClientData_ActionBarSetting )
	{
		memcpy(&m_setting, data, min(size, sizeof(ActionBarSetting)));

		m_settingBackup = m_setting;
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::Save()
{	
	if ( m_interface == NULL || m_interface->IsLoadVariables() == false )
		return;
	
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"Action.bsd");
	wstring localName = GetLocalPathW(L"Action.bsd");

	// 儲存共用資料			

	// 記錄資料	

	// 關閉檔案	

	if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
	{
		int size;
		int version;
		int classNum;
		int itemNum;

		// 第二版
		{
			// 寫入識別編號
			version = ACTION_ITEM_VERSION;
			fwrite(&version, sizeof(int), 1, fptr);

			// 寫入結構大小
			size = sizeof(ActionItem2);
			fwrite(&size, sizeof(int), 1, fptr);

			// 寫入職業個數
			classNum = ACTION_BAR_MAX_CLASS;
			fwrite(&classNum, sizeof(int), 1, fptr);

			// 寫入一個職業最大按鈕個數
			itemNum = ACTION_BAR_MAX_ITEMS;
			fwrite(&itemNum, sizeof(int), 1, fptr);

			// 儲存整筆資料
			fwrite(m_items, size, classNum * itemNum, fptr);
		}

		// 關閉檔案
		fclose(fptr);
	}

	fptr = NULL;
	localName = GetLocalPathW(L"ActionSetting.bsd");
	if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
	{
		int size;
		int version;

		// 寫入識別編號
		version = ACTION_ITEM_VERSION;
		fwrite(&version, sizeof(int), 1, fptr);

		// 寫入結構大小
		size = sizeof(ActionBarSetting);
		fwrite(&size, sizeof(int), 1, fptr);

		// 寫入資料大小
		fwrite(&m_setting, sizeof(ActionBarSetting), 1, fptr);

		// 關閉檔案
		fclose(fptr);
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::Load()
{
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"Action.bsd");
	wstring localName = GetLocalPathW(L"Action.bsd");	

	// 讀取共用資料

	// 記錄資料	

	// 關閉檔案

	// 讀取本地資料
	if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
	{
		int size;
		int version;

		fread(&version, sizeof(int), 1, fptr);
		fread(&size, sizeof(int), 1, fptr);

		// 第二版
		if ( version == ACTION_ITEM_VERSION && size == sizeof(ActionItem2) )
		{
			int classNum;
			int itemNum;

			// 取得最大職業與最大按鈕個數
			fread(&classNum, sizeof(int), 1, fptr);
			fread(&itemNum, sizeof(int), 1, fptr);

			for ( int i = 0; i < classNum; i ++ )
			{
				for ( int j = 0; j < itemNum; j++ )
				{
					// 讀取一筆筆資料
					fread(&m_items[i][j], sizeof(ActionItem2), 1, fptr);
				}
			}
		}
		// 第一版(結構需要轉換)
		else
		{
			int classNum = 16;
			int itemNum = 80;

			fseek(fptr, 0, SEEK_SET);
			fread(&size, sizeof(int), 1, fptr);
			
			if ( size == sizeof(ActionItem) * classNum * itemNum )
			{
				ActionItem item;
				for ( int i = 0; i < classNum; i++ )
				{
					for ( int j = 0; j < itemNum; j++ )
					{
						fread(&item, sizeof(ActionItem), 1, fptr);
						m_items[i][j].type = item.type;
						switch ( item.type )
						{
						case eActionItemType_None:
							memset(m_items[i][j]._value, 0, sizeof(m_items[i][j]._value));
							break;

						case eActionItemType_Bag:
						case eActionItemType_Equipment:
							m_items[i][j].objectID		= item.objectID;
							m_items[i][j].createTime	= 0;
							m_items[i][j].serial		= 0;
							break;

						case eActionItemType_Skill:
							m_items[i][j].skillID		= item.skillID;
							break;

						case eActionItemType_Macro:
							m_items[i][j].macroID		= item.macroID;
							break;

						case eActionItemType_Emote:
							m_items[i][j].emoteID		= item.emoteID;
							break;
						case eActionItemType_Teleport:
							m_items[i][j].teleportID	= item.teleportID;
							break;
					
						}
					}
				}
			}
		}

		// 關閉檔案
		fclose(fptr);
	}

	// 由 Lua 載入設定值
	lua_State* L = GetLuaState();
	if ( L )
	{
		luaL_dostring(L,
			"if ( ActionBarInfo ) then \
				for i = 1, 4 do \
					SetActionBarSetting(i, ActionBarInfo[i].visible, ActionBarInfo[i].count, ActionBarInfo[i].row) \
					SetActionBarLocked(ACTIONBUTTON_LOCKED) \
					SetActionBarHideEmpty(ACTIONBUTTON_HIDDEN) \
				end \
			 end"
			);
	}

	fptr = NULL;
	localName = GetLocalPathW(L"ActionSetting.bsd");
	if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
	{
		int size;
		int version;

		// 寫入識別編號
		fread(&version, sizeof(int), 1, fptr);
		if ( version == ACTION_ITEM_VERSION )
		{
			// 寫入結構大小
			fread(&size, sizeof(int), 1, fptr);

			// 寫入資料大小
			fread(&m_setting, min(sizeof(ActionBarSetting), size), 1, fptr);
		}

		// 關閉檔案
		fclose(fptr);
	}

	// 如果未在伺服器上存檔
	SetSaveUpdateTime();
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdatePetSkill(int petMagicID)
{
	vector<int> m_petSkill;
	int curr = 0;

	memset(m_petItems, 0, sizeof(m_petItems));

	// 基本命令
	m_petItems[curr++].skillID = EM_PET_STOP;
	m_petItems[curr++].skillID = EM_PET_FOLLOW;
	m_petItems[curr++].skillID = EM_PET_ATTACK;
	m_petItems[curr++].skillID = EM_PET_STRICKBACK;	
	//m_petItems[3].skillID = EM_PET_ATTACKOFF;
	//m_petItems[4].skillID = EM_PET_STRICKBACK;	

	// 寵物技能	
	g_pUi_SkillBook->GetPetSkillInfo( petMagicID, m_petSkill );

	m_petAllColdown = 0.0f;
	for ( int i = 0; i < (int)m_petSkill.size(); i++ )
	{
		m_petItems[curr + i].coldownTime	= 0.0f;
		m_petItems[curr + i].autoSpell		= false;
		m_petItems[curr + i].skillID		= m_petSkill[i];
	}
}

// ----------------------------------------------------------------------------
PetActionItem* CActionFrame::GetPetSkill(int index)
{
	if ( index < 0 || index >= PETACTIONBAR_MAX_SKILLS )
		return NULL;
	return m_petItems + index;
}

// ----------------------------------------------------------------------------
void CActionFrame::UsePetItem(int index, bool autoRun)
{
	PetActionItem* petItem = GetPetSkill(index);
	if ( petItem )
	{
		CRoleSprite* target = g_pGameMain->GetPlayerTarget();
		int petGUID	= g_pGameMain->GetPetGUID();
		int targetID = (target) ? target->GetIdentify() : 0;

		switch (petItem->skillID)
		{
		case EM_PET_STOP:
			if ( autoRun == false )
				NetCli_Magic::PetCommand(petGUID, EM_PET_STOP, targetID, 0);
			break;

		case EM_PET_FOLLOW:
		case EM_PET_ATTACKOFF:
			if ( autoRun == false ) {
				NetCli_Magic::PetCommand(petGUID, EM_PET_ATTACKOFF, targetID, 0);
				NetCli_Magic::PetCommand(petGUID, EM_PET_FOLLOW, targetID, 0);
			}
			break;

		case EM_PET_ATTACK:
			if ( autoRun == false )
				NetCli_Magic::PetCommand(petGUID, EM_PET_ATTACK, targetID, 0);
			break;

		case EM_PET_STRICKBACK:
			if ( autoRun == false )
			{
				if ( g_pGameMain->IsPetStrikeBackEnable() )
					NetCli_Magic::PetCommand(petGUID, EM_PET_STRICKBACK, targetID, 0);					
				else
					NetCli_Magic::PetCommand(petGUID, EM_PET_STRICKBACK_OFF, targetID, 0);
			}
			break;

		case EM_PET_GUARD:
			NetCli_Magic::PetCommand(petGUID, EM_PET_GUARD, targetID, 0);
			break;

		default:
			if ( petItem->skillID > 0 )
			{
				if ( autoRun )
				{
					GameObjDbStruct* item = g_ObjectData->GetObj(petItem->skillID);
					if ( item->MagicCol.IsPetCycleMagic )
					{
						NetCli_Magic::PetMagicSwitch(petGUID, petItem->skillID, !petItem->autoSpell);
					}
				}
				else
				{
					NetCli_Magic::PetCommand(petGUID, EM_PET_SPELLMAGIC, targetID, petItem->skillID);
				}
			}
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::SetPetAutoSpellState(int skillID, bool state)
{
	bool needUpdate = false;

	for ( int i = 0; i < PETACTIONBAR_MAX_SKILLS; i++ )
	{
		if ( m_petItems[i].skillID == skillID )
		{
			if ( m_petItems[i].autoSpell != state )
			{
				m_petItems[i].autoSpell = state;
				needUpdate = true;
			}
		}
	}

	if ( needUpdate )
		SendWorldEvent("PET_ACTIONBAR_UPDATE");
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdatePetColdown(int skillID)
{
	for ( int i = 0; i < PETACTIONBAR_MAX_SKILLS; i++ )
	{
		if ( m_petItems[i].skillID == skillID )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(skillID);
			if ( itemDB && itemDB->Classification == EM_ObjectClass_MagicCollect )
			{
				m_petItems[i].coldownTime = (float)itemDB->MagicCol.Coldown.Time;
			}
			break;
		}
	}

	SendWorldEvent("PET_ACTIONBAR_UPDATE_COOLDOWN");
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdateSkillID()
{
	Voc_ENUM voc = CNetGlobal::RoleData()->BaseData.Voc;
	for ( int i = 0; i < ACTION_BAR_MAX_ITEMS; i++ )
	{
		if ( m_items[voc][i].type == eActionItemType_Skill )
		{
			int newSkillID = -1;
			int oldSkillID = m_items[voc][i].skillID;
			if ( (newSkillID = CUI_SkillBook::GetFinalSkillID(oldSkillID)) != -1 )
			{
				if ( oldSkillID != newSkillID )
				{
					m_items[voc][i].skillID = newSkillID;
					SlotChanged(i);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdateSkillAction(int skillId)
{
	Voc_ENUM voc = CNetGlobal::RoleData()->BaseData.Voc;
	for ( int i = 0; i < ACTION_BAR_MAX_ITEMS; i++ )
	{
		if ( m_items[voc][i].type == eActionItemType_Skill && m_items[voc][i].skillID == skillId )
		{
			SlotChanged(i);
		}
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::UpdateExtraSkill()
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player == NULL )
		return;

	int size = player->GetBuffSize();
	RoleBuffInfo* buffInfo = NULL;
	GameObjDbStruct* itemDB;
	
	vector<ExtraActionItem> newAction;	
	for ( int i = 0; i < size; i++ )
	{
		buffInfo = player->GetBuffInfo(i);
		itemDB = g_ObjectData->GetObj(buffInfo->id);

		if ( itemDB && itemDB->MagicBase.Setting.BuffSkill )
		{
			for ( int k = 0; k < _MAX_BUFFSKILL_COUNT_; k++ )
			{
				if ( itemDB->MagicBase.BuffSkill[k] != 0 )
				{
					ExtraActionItem extraItem;
					extraItem.buffId = buffInfo->id;
					extraItem.buffPowerLevel = buffInfo->magicLv;
					extraItem.skillId = itemDB->MagicBase.BuffSkill[k];
					newAction.push_back(extraItem);					
				}
			}
		}
	}

	m_extraItems.clear();
	m_extraItems = newAction;

	/*
	GameObjDbStructEx* NewSkill = g_ObjectData->GetObj( SkillInfo.MagicID );
	if( NewSkill == NULL )
		continue;

	int skillpoint = (int)CNetGlobal::RoleData()->GetSkillPoint( NewSkill->GUID );
	string link;
	if( false == SkillGUID_To_HyperLink( NewSkill->GUID, skillpoint, link ) )
	{
		link = NewSkill->Name;
	}
	g_pChatFrame->SendChatMessageEx( CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("LEVELUP_MESSAGE_SKILL"), link.c_str() );
	*/

	if ( m_extraItems.empty() )
	{
		// 關閉技能列
		SendWorldEvent("EXTRA_ACTIONBAR_HIDE");

		// 如果在等待施法狀態,清除選取
		switch ( g_pGameMain->GetCastSpellState() )
		{
		case CASTSPELL_WAITING_LOCATION_EXTRA:
		case CASTSPELL_WAITING_TARGET_EXTRA:
			g_pGameMain->SetCastSpellState(CASTSPELL_NONE);
			break;
		}

		ClearWaitingSpellID();
	}
	else
	{
		// 更新技能列
		SendWorldEvent("EXTRA_ACTIONBAR_SHOW");
	}
}

// ----------------------------------------------------------------------------
ExtraActionItem* CActionFrame::GetExtraItem(int index)
{
	if ( (unsigned int)index >= (unsigned int)m_extraItems.size() )
		return NULL;
	return &(m_extraItems[index]);
}

// ----------------------------------------------------------------------------
void CActionFrame::UseExtraItem(int index)
{
	ExtraActionItem* extraItem = GetExtraItem(index);
	if ( extraItem )
	{
		GameObjDbStruct* magicObj = g_ObjectData->GetObj(extraItem->skillId);
		if ( magicObj )
		{
			CRoleSprite* player = g_pGameMain->GetPlayer();
			CRoleSprite* target = g_pGameMain->GetPlayerTarget();

			switch (magicObj->MagicCol.TargetType)
			{
			// 對位置
			case EM_Releation_Locatoin:
				m_waitingType = 1;
				m_waitingSpellIndex = index;
				g_pGameMain->SetCastSpellState(CASTSPELL_WAITING_LOCATION_EXTRA, true);
				break;

			// 對自己
			case EM_Releation_Self:
				if ( player )
				{
					NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, player->GetIdentify(), 0, 0, 0);
				}
				break;

			// 友善法術
			case EM_Releation_Member:
			case EM_Releation_Friend:
			case EM_Releation_Friend_NPC:
			case EM_Releation_Player:
			case EM_Releation_GuildPlayer:
			case EM_Releation_CountryPlayer:
			case EM_Releation_Self_Pet:
			case EM_Releation_Member_Pet:				
				if ( target && !target->GetCanAttackState() )
				{
					NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, target->GetIdentify(), 0, 0, 0);
				}
				else
				{
					m_waitingType = 1;
					m_waitingSpellIndex = index;
					g_pGameMain->SetCastSpellState(CASTSPELL_WAITING_TARGET_EXTRA, true);
				}
				break;

			// 對屍體
			case EM_Releation_Corpse:
			case EM_Releation_Npc_Corpse:
			case EM_Releation_Player_Corpse:
				if ( target && target->GetDeadState() )
				{
					NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, target->GetIdentify(), 0, 0, 0);
				}
				else
				{
					m_waitingType = 1;
					m_waitingSpellIndex = index;
					g_pGameMain->SetCastSpellState(CASTSPELL_WAITING_TARGET_EXTRA, true);
				}
				break;

			// 仇佷對象
			case EM_Releation_Enemy:			
				if ( target && target->GetCanAttackState() )
				{
					NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, target->GetIdentify(), 0, 0, 0);
				}
				else
				{
					m_waitingType = 1;
					m_waitingSpellIndex = index;
					g_pGameMain->SetCastSpellState(CASTSPELL_WAITING_TARGET_EXTRA, true);
				}
				break;

			// 不同公會不同國家
			case EM_Releation_NotGuildPlayer:
			case EM_Releation_NoCountryPlayer:
				break;

			case EM_Releation_All:
			default:
				if ( target == NULL )
				{
					m_waitingType = 1;
					m_waitingSpellIndex = index;
					g_pGameMain->SetCastSpellState(CASTSPELL_WAITING_TARGET_EXTRA, true);
				}
				else
				{
					int targetID = (target) ? target->GetIdentify() : 0;
					NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, targetID, 0, 0, 0);
				}
				break;
			}
		}		
	}
}

// --------------------------------------------------------------------------------------
ExtraActionItem* CActionFrame::GetExtraWaitingItem()
{
	if ( m_waitingType == 1 )
	{
		return GetExtraItem(m_waitingSpellIndex);
	}
	return NULL; 
}

// --------------------------------------------------------------------------------------
void CActionFrame::InsertImplementAction(int speller, int skillId, int skillLevel)
{
	ImplementActionItem newItem;
	newItem.speller = speller;
	newItem.skillId = skillId;
	newItem.skillLevel = skillLevel;

	for ( vector<ImplementActionItem>::iterator iter = m_implementItems.begin(); iter != m_implementItems.end(); iter++ )
	{
		if ( *iter == newItem )
			return;
	}

	m_implementItems.push_back(newItem);

	SendWorldEvent("IMPLEMENT_ACTIONBAR_UPDATE");
}

// --------------------------------------------------------------------------------------
void CActionFrame::DeleteImplementAction(int speller, int skillId, int skillLevel)
{
	ImplementActionItem newItem;	
	newItem.speller = speller;
	newItem.skillId = skillId;
	newItem.skillLevel = skillLevel;

	for ( vector<ImplementActionItem>::iterator iter = m_implementItems.begin(); iter != m_implementItems.end(); iter++ )
	{
		if ( *iter == newItem )
		{
			m_implementItems.erase(iter);

			SendWorldEvent("IMPLEMENT_ACTIONBAR_UPDATE");

			return;
		}
	}	
}

// --------------------------------------------------------------------------------------
void CActionFrame::UseImplementAction(int index)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();

	// 預設定最後一個離開載具
	if ( index == 9 )
	{
		if ( player && player->GetImplementSprite() )
		{
			Net_Move::S_DetachObjRequest( player->GetIdentify(), player->GetImplementSprite()->GetIdentify() );
			return;
		}
	}

	// 取得法術資訊
	ImplementActionItem* item = GetImplementActionItem(index);
	if ( item )
	{
		GameObjDbStruct* magicDB = g_ObjectData->GetObj(item->skillId);		
		ExSkillBaseStruct info;
		info.SkillID = item->skillId;
		info.SkillLv = item->skillLevel;
		info.SpellerID = item->speller;

		if ( magicDB )
		{
			CRoleSprite* player = g_pGameMain->GetPlayer();
			CRoleSprite* target = g_pGameMain->GetPlayerTarget();
			CRoleSprite* implement = (player) ? player->GetImplementSprite() : NULL;
			CRuEntity* targetEntity = g_pGameMain->GetTargetEntity();
			float parabolaHeight = g_pGameMain->GetTargetEffectParabolaHeight();						

			GameObjDbStruct* imageDB = g_ObjectData->GetObj(magicDB->ImageID);

			switch (magicDB->MagicCol.TargetType)
			{
			// 對位置
			case EM_Releation_Locatoin:
				if ( targetEntity == NULL )
				{
					if ( imageDB && imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_Parabola || imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_ParabolaMulti )
						g_pGameMain->CreateTargetEffect(TargetLineEffect_Parabola, implement);						
					else
						g_pGameMain->CreateTargetEffect(TargetLineEffect_Line, implement);						
				}
				else
				{
					if ( imageDB && imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_Parabola || imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_ParabolaMulti )
						g_pGameMain->SetTargetEffectType(TargetLineEffect_Parabola, 50);
					else
						g_pGameMain->SetTargetEffectType(TargetLineEffect_Line, 0);

					CRuVector3 position = targetEntity->GetTranslation(ruFRAMETYPE_WORLD);
					CRuVector3 p1 = (implement) ? implement->GetPos() : position;
					float flyTime = g_pGameMain->GetMagicFlyTime(p1, position, magicDB->GUID, (int)g_pGameMain->GetTargetEffectParabolaHeight());
					NetCli_Other::SL_SpellExSkill( 0, position.m_x, position.m_y, position.m_z, info, (int)parabolaHeight, (int)(flyTime * 1000.0f));
				}
				break;

			// 對自己
			case EM_Releation_Self:
				if ( player )
				{
					NetCli_Other::SL_SpellExSkill( item->speller, 0.0f, 0.0f, 0.0f, info, 0, 0);
				}
				break;

			// 只能對載具用
			case EM_Releation_Wagon:
				if ( target == NULL )
				{					
					// 對自己用
					NetCli_Other::SL_SpellExSkill( item->speller, 0.0f, 0.0f, 0.0f, info, 0, 0);
				}
				else if ( target->IsImplement() == false )
				{
					// 目標不是載具
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
				}
				else if ( target->GetCanAttackState() )
				{
					// 目標是敵方
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
				}
				else if ( g_pGameMain->GetDistanceWithPlayer(target) > magicDB->MagicCol.AttackDistance )
				{
					// 超出施法距離
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_101"));
				}
				else
				{
					CRuVector3 p2 = target->GetPos();
					CRuVector3 p1 = (implement) ? implement->GetPos() : p2;					
					float flyTime = g_pGameMain->GetMagicFlyTime(p1, p2, magicDB->GUID);
					NetCli_Other::SL_SpellExSkill( target->GetIdentify(), 0.0f, 0.0f, 0.0f, info, (int)parabolaHeight, (int)(flyTime * 1000.0f) );
				}
				break;

			// 友善法術
			case EM_Releation_Member:
			case EM_Releation_Friend:
			case EM_Releation_Friend_NPC:
			case EM_Releation_Player:
			case EM_Releation_GuildPlayer:
			case EM_Releation_CountryPlayer:
			case EM_Releation_Self_Pet:
			case EM_Releation_Member_Pet:				
				// 請選擇一個目標
				if ( target == NULL )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NOTARGET_ERROR"));
				}
				// 對象是載具
				else if ( target->IsImplement() == true )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_NOT_IMPLEMENT"));
				}
				// 目標錯誤
				else if ( target->GetCanAttackState() )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
				}
				else if ( g_pGameMain->GetDistanceWithPlayer(target) > magicDB->MagicCol.AttackDistance )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_101"));
				}
				else
				{
					CRuVector3 p2 = target->GetPos();
					CRuVector3 p1 = (implement) ? implement->GetPos() : p2;
					float flyTime = g_pGameMain->GetMagicFlyTime(p1, p2, magicDB->GUID);
					NetCli_Other::SL_SpellExSkill( target->GetIdentify(), 0.0f, 0.0f, 0.0f, info, (int)parabolaHeight, (int)(flyTime * 1000.0f) );
				}				
				break;

			// 對屍體
			case EM_Releation_Corpse:
			case EM_Releation_Npc_Corpse:
			case EM_Releation_Player_Corpse:
				break;

			// 仇佷對象
			case EM_Releation_Enemy:
				// 請選擇一個目標
				if ( target == NULL )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NOTARGET_ERROR"));					
				}
				// 目標錯誤
				else if ( target->GetCanAttackState() == false )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
				}
				else if ( g_pGameMain->GetDistanceWithPlayer(target) > magicDB->MagicCol.AttackDistance )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_101"));
				}
				else
				{
					CRuVector3 p2 = target->GetPos();
					CRuVector3 p1 = (implement) ? implement->GetPos() : p2;
					float flyTime = g_pGameMain->GetMagicFlyTime(p1, p2, magicDB->GUID);
					NetCli_Other::SL_SpellExSkill( target->GetIdentify(), 0.0f, 0.0f, 0.0f, info, (int)parabolaHeight, (int)(flyTime * 1000.0f) );
				}
				break;

				// 不同公會不同國家
			case EM_Releation_NotGuildPlayer:
			case EM_Releation_NoCountryPlayer:
				break;

			case EM_Releation_All:
			default:
				// 請選擇一個目標
				if ( target == NULL )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NOTARGET_ERROR"));					
				}
				else if ( g_pGameMain->GetDistanceWithPlayer(target) > magicDB->MagicCol.AttackDistance )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_101"));
				}
				else
				{
					CRuVector3 p2 = target->GetPos();
					CRuVector3 p1 = (implement) ? implement->GetPos() : p2;
					float flyTime = g_pGameMain->GetMagicFlyTime(p1, p2, magicDB->GUID);
					NetCli_Other::SL_SpellExSkill( target->GetIdentify(), 0.0f, 0.0f, 0.0f, info, (int)parabolaHeight, (int)(flyTime * 1000.0f) );
				}
				break;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CActionFrame::ClearImplementAction()
{
	m_implementItems.clear();
}

// --------------------------------------------------------------------------------------
ImplementActionItem* CActionFrame::GetImplementActionItem(int index)
{
	if (index >= 0 && index < m_implementItems.size())
		return &(m_implementItems[index]);
	return NULL;
}

// --------------------------------------------------------------------------------------
void CActionFrame::UseExSkill(int targetId, int x, int y, int z)
{
	switch ( GetWaitingSkillType() )
	{
	case 1:
		{
			ExtraActionItem* extraItem = GetExtraWaitingItem();
			if ( extraItem )
			{
				NetCli_Magic::SL_BuffSkillRequest(extraItem->buffId, extraItem->skillId, targetId, (float)x, (float)y, (float)z);
			}
		}
		break;

	case 2:
		{
			// 取得法術資訊
			ImplementActionItem* item = GetImplementActionItem(m_waitingSpellIndex);
			if ( item )
			{
				ExSkillBaseStruct info;
				info.SkillID = item->skillId;
				info.SkillLv = item->skillLevel;
				info.SpellerID = item->speller;

				NetCli_Other::SL_SpellExSkill(targetId, (float)x, (float)y, (float)z, info, (int)g_pGameMain->GetTargetEffectParabolaHeight(), 0);
			}
		}
		break;
	}

	m_waitingType = 0;
	m_waitingSpellIndex = 0;
}

// --------------------------------------------------------------------------------------
void CActionFrame::SetActionBarSetting(int index, bool enable, int count, int row)
{
	if ( index >= 0 && index < 4 )
	{
		m_setting.bar[index].isEnable = enable;
		m_setting.bar[index].count = count;
		m_setting.bar[index].row = row;
	}
}

// ----------------------------------------------------------------------------
void CActionFrame::ResetActionBarSetting()
{
	m_setting.Init();
}

// ----------------------------------------------------------------------------
void CActionFrame::SlotChanged(int index)
{
	lua_State* L = GetLuaStateWorld();

	lua_pushnumber(L, index + 1);
	lua_setglobal(L, "arg1");
	SendWorldEvent("ACTIONBAR_SLOT_CHANGED");

	SetSaveUpdateTime();
}

// ----------------------------------------------------------------------------
int LuaFunc_HasAction(lua_State* L)
{
	ActionItem2* item = g_pActionFrame->GetItem(luaL_checkint(L, 1) - 1);
	if ( item == NULL || item->type == eActionItemType_None )
		lua_pushboolean(L, false);
	else
		lua_pushboolean(L, true);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_PickupAction(lua_State* L)
{
	g_pActionFrame->Paste(luaL_checkint(L, 1) - 1);		// 拿取物品
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UseAction(lua_State* L)
{
	g_pActionFrame->Use(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionInfo(lua_State* L)
{
	ActionItem2* item = g_pActionFrame->GetItem(luaL_checkint(L, 1) - 1);
	if ( item )
	{
		switch (item->type)
		{
		case eActionItemType_None:
			break;

		case eActionItemType_Bag:
		case eActionItemType_Equipment:
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->objectID);
				if (itemDB)
				{
					bool isWearable = false;
					int count = CNetGlobal::RoleData()->GetBagItemCount(item->objectID);
					char* texture = NULL;

					/*
					if ( itemDB->MaxHeap > 1 ) {
						count = CNetGlobal::RoleData()->GetBagItemCount(item->objectID);
					}
					*/

					// 非堆疊物品,並且有物品唯一編號
					if ( itemDB->MaxHeap <= 1 && item->createTime != 0 )
					{
						if ( g_pCharacterFrame->SearchEquip(item->createTime, item->serial) > 0 ) {
							isWearable = true;
							if ( count <= 1 ) count = 1;
						}
					}

					GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
					if ( imageDB )
					{
						texture = imageDB->Image.ACTField;
					}

					lua_pushstring(L, texture);
					lua_pushstring(L, "");
					lua_pushnumber(L, count);
					lua_pushboolean(L, false);
					lua_pushboolean(L, isWearable);
					return 5;
				}
			}
			break;

		case eActionItemType_Skill:
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->skillID);

				// TODO : GetSkillIcon( SkillID );
				if( item->skillID == DF_SKILLID_ATTACK )
				{
					char szAttackIcon[256];
					CUI_SkillBook::GetAttackIcon( szAttackIcon );

					lua_pushstring( L, szAttackIcon );
					lua_pushstring( L, "" );
					lua_pushnumber( L, 1 );
					lua_pushboolean( L, false );
					lua_pushboolean( L, false );		// wearable					
					lua_pushboolean( L, false );
					//lua_pushboolean( L, g_pGameMain->IsAttackAction() );
					return 6;
				}
				else if (itemDB)
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
					if ( imageDB )
						lua_pushstring( L, imageDB->Image.ACTField );
					else
						lua_pushstring( L, "" );

					lua_pushstring( L, "" );
					lua_pushnumber( L, 1 );
					lua_pushboolean( L, false );
					lua_pushboolean( L, false );		// wearable
					lua_pushboolean( L, g_pGameMain->IsCycleSpellID(item->skillID) );
					return 6;
				}
			}
			break;

		case eActionItemType_Macro:
			{
				MacroItem* macroItem = g_pMacroFrame->GetMacroItemById(item->macroID);
				if ( macroItem )
				{
					lua_pushstring(L, g_pMacroFrame->GetMacroIcon(macroItem->icon));
					lua_pushstring(L, macroItem->name);
					lua_pushnumber(L, 1);
					lua_pushboolean(L, false);
					return 4;
				}
			}
			break;

		case eActionItemType_Emote:
			{
				SEmoteItem* emoteItem = g_pEmoteFrame->GetEmoteItemByID( item->emoteID );
				if ( emoteItem )
				{
					lua_pushstring(L, g_pEmoteFrame->GetEmoteIcon( emoteItem ) );
					lua_pushstring(L, emoteItem->pName );
					lua_pushnumber(L, 1);
					lua_pushboolean(L, false);
					return 4;
				}
			}
			break;
		case eActionItemType_Teleport:
			{
				LocationSaveStruct* Item =g_pTeleportBook->GetItem(item->teleportID);
				
				if ( Item )
				{
					char temp[512];
					g_pTeleportBook->GetItemIcon( Item->IconType,temp ) ;

					lua_pushstring(L,temp);
					lua_pushstring(L, Item->Note);
					lua_pushnumber(L, 1);
					lua_pushboolean(L, false);
					return 4;
				}
			}
			break;

		case eActionItemType_HappyPartner:
			{
			return 	g_pPartnerFrame->GetActionInfo(item->PartnerMode,item->PartnerPos);

			
			}
			break;

		case eActionItemType_SuitSkill:
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->suitskillID);
				if (itemDB)
				{
					// ICON 
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
					if ( imageDB )
						lua_pushstring( L, imageDB->Image.ACTField );
					else
						lua_pushstring( L, "" );

					lua_pushstring( L, "" );			// 名稱
					lua_pushnumber( L, 1 );				// 數量
					lua_pushboolean( L, false );		// 
					lua_pushboolean( L, false );		// wearable
					lua_pushboolean( L, g_pGameMain->IsCycleSpellID(item->suitskillID) );
					return 6;
				}

			}
			break;

		case eActionItemType_SkillPlate:
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->skillplateID);
				if (itemDB)
				{
					// ICON 
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
					if ( imageDB )
						lua_pushstring( L, imageDB->Image.ACTField );
					else
						lua_pushstring( L, "" );

					lua_pushstring( L, "" );			// 名稱
					lua_pushnumber( L, 1 );				// 數量
					lua_pushboolean( L, false );		// 
					lua_pushboolean( L, false );		// wearable
					lua_pushboolean( L, g_pGameMain->IsCycleSpellID(item->skillplateID) );
					return 6;
				}
			}
			break;

		case eActionItemType_CoolSuit:
			{
				//item->coolSuitIndex

				string texture = g_pCharacterFrame->GetCoolSuitPageTexture( item->coolSuitIndex );

				PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
				bool isAction = PlayerBaseData->CoolSuitID == item->coolSuitIndex;

				string name = g_ObjectData->GetString( "SYS_COOLSUIT_PAGE_TIPS" );
				name = ReplaceKeyString( name, "<number>", item->coolSuitIndex + 1 );

				lua_pushstring( L, texture.c_str() );
				lua_pushstring( L, name.c_str() );
				lua_pushnumber( L, 1 );
				lua_pushboolean( L, false );
				lua_pushboolean( L, false );		// wearable
				lua_pushboolean( L, isAction );

				return 6;

			}

		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionCooldown(lua_State* L)
{
	ActionItem2* item = g_pActionFrame->GetItem(luaL_checkint(L, 1) - 1);
	float cooldown[2];

	cooldown[0] = 0.0f;
	cooldown[1] = 0.0f;	
	if ( item )
	{
		switch (item->type)
		{
		case eActionItemType_Bag:
		case eActionItemType_Equipment:
			CNetGlobal::RoleData()->GetObjectCooldown(item->objectID, cooldown);
			break;

		case eActionItemType_Skill:
			CNetGlobal::RoleData()->GetObjectCooldown(item->skillID, cooldown);
			break;

		case eActionItemType_Macro:
			break;

		case eActionItemType_Emote:
			break;

		case eActionItemType_Teleport:
			break;
		case eActionItemType_HappyPartner:
			break;
		case eActionItemType_SuitSkill:
			CNetGlobal::RoleData()->GetObjectCooldown(item->suitskillID, cooldown);
			break;

		case eActionItemType_SkillPlate:
			CNetGlobal::RoleData()->GetObjectCooldown(item->skillplateID, cooldown);
			break;
		}
	}
	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionUsable(lua_State* L)
{
	ActionItem2* item = g_pActionFrame->GetItem(luaL_checkint(L, 1) - 1);
	bool isUsable = true;
	if ( item )
	{
		switch (item->type)
		{
		case eActionItemType_Bag:
		case eActionItemType_Equipment:
			break;

		case eActionItemType_Skill:
			isUsable = g_pGameMain->CanUseSkill(item->skillID) == 0;
			break;

		case eActionItemType_Macro:
			break;

		case eActionItemType_Emote:
			break;

		case eActionItemType_Teleport:
			isUsable = g_pTeleportBook->CanUseTeleport(item->teleportID);
			break;

		case eActionItemType_HappyPartner:
			isUsable = g_pPartnerFrame->CanUsePartner(item->PartnerMode,item->PartnerPos);
			break;

		case eActionItemType_SuitSkill:
			isUsable = g_pGameMain->CanUseSkill(item->suitskillID) == 0;
			break;

		case eActionItemType_SkillPlate:
			isUsable = g_pGameMain->CanUseSkill(item->skillplateID) == 0;
			break;
		}
	}
	lua_pushboolean(L, isUsable);
	return 1;
}

// ----------------------------------------------------------------------------
// 取得裝備是否為目前穿著裝備
// 回傳 true 已裝備  false未裝備(應該在背包內)
int LuaFunc_GetActionWearable(lua_State* L)
{
	ActionItem2* item = g_pActionFrame->GetItem(luaL_checkint(L, 1) - 1);
	bool isWearable = false;

	// 裝備
	if ( item && (item->type == eActionItemType_Equipment || item->type == eActionItemType_Bag) )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->objectID);
		if (itemDB)
		{
			// 非堆疊物品,並且有物品唯一編號
			if ( itemDB->MaxHeap <= 1 && item->createTime != 0 )
			{
				if ( g_pCharacterFrame->SearchEquip(item->createTime, item->serial) > 0 )
					isWearable = true;
			}
		}
	}
	lua_pushboolean(L, isWearable);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPetActionInfo(lua_State* L)
{
	PetActionItem *petItem = g_pActionFrame->GetPetSkill(luaL_checkint(L, 1) - 1);
	if ( petItem )
	{
		const char* name = NULL;
		bool cycleEnable = false;
		bool canAutoRun = false;

		switch (petItem->skillID)
		{
		case EM_PET_STOP:
			name = "interface\\icons\\pet_skill_stop.dds";
			cycleEnable = g_pGameMain->IsPetMovable() == true;
			break;
		case EM_PET_FOLLOW:
			name = "interface\\icons\\pet_skill_follow.dds";
			cycleEnable = g_pGameMain->IsPetMovable() == false;
			break;
		case EM_PET_ATTACK:
			name = "interface\\icons\\pet_skill_attack.dds";
			cycleEnable = false;
			break;
		case EM_PET_ATTACKOFF:
			name = "interface\\icons\\pet_skill_stop_attack.dds";
			cycleEnable = false;
			break;
		case EM_PET_STRICKBACK:
			name = "interface\\icons\\pet_skill_protect.dds";
			cycleEnable = g_pGameMain->IsPetStrikeBackEnable() == false;
			break;
		case EM_PET_GUARD:
			name = "interface\\icons\\emo_point.dds";
			cycleEnable = false;
			break;
		default:
			{
				GameObjDbStructEx* obj = g_ObjectData->GetObj(petItem->skillID);
				if ( obj )
				{
					GameObjDbStructEx* imageDB = g_ObjectData->GetObj(obj->ImageID);
					if ( imageDB )
						name	= imageDB->Image.ACTField;
					
					cycleEnable	= petItem->autoSpell;
					canAutoRun	= obj->MagicCol.IsPetCycleMagic;
				}
			}
		}

		lua_pushstring(L, name);
		lua_pushboolean(L, cycleEnable);
		lua_pushboolean(L, canAutoRun);
		return 3;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetActionCooldown(lua_State* L)
{
	// 0總時間量 1剩於時間
	float coldown[2] = { 0.0f, 0.0f };

	PetActionItem *petItem = g_pActionFrame->GetPetSkill(luaL_checkint(L, 1) - 1);
	if ( petItem )
	{
		switch (petItem->skillID)
		{
		case EM_PET_STOP:
		case EM_PET_FOLLOW:			
		case EM_PET_ATTACK:			
		case EM_PET_ATTACKOFF:			
		case EM_PET_STRICKBACK:			
		case EM_PET_GUARD:
			break;

		default:
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(petItem->skillID);
				if ( itemDB && itemDB->Classification == EM_ObjectClass_MagicCollect )
				{
					coldown[0] = (float)itemDB->MagicCol.Coldown.Time;
					coldown[1] = petItem->coldownTime;
				}
			}
		}
	}

	lua_pushnumber(L, coldown[0]);
	lua_pushnumber(L, coldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_UsePetAction(lua_State* L)
{
	g_pActionFrame->UsePetItem(luaL_checkint(L, 1) - 1, (lua_toboolean(L, 2) != 0));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetExtraActionInfo(lua_State* L)
{
	ExtraActionItem* item = g_pActionFrame->GetExtraItem(luaL_checkint(L, 1) - 1);
	if ( item )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->skillId);
		if ( itemDB )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB )
			{
				lua_pushstring(L, imageDB->Image.ACTField);			// ICON 圖示
				return 1;
			}
		}
	}
	return 0;	
}

// ----------------------------------------------------------------------------
int LuaFunc_GetExtraActionCooldown(lua_State* L)
{
	ExtraActionItem* item = g_pActionFrame->GetExtraItem(luaL_checkint(L, 1) - 1);
	float cooldown[2];

	cooldown[0] = 0.0f;
	cooldown[1] = 0.0f;	
	if ( item )
		CNetGlobal::RoleData()->GetObjectCooldown(item->skillId, cooldown);

	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_UseExtraAction(lua_State* L)
{
	g_pActionFrame->UseExtraItem(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetActionBarSetting(lua_State* L)
{	
	int index = luaL_checkint(L, 1) - 1;
	bool enable = (lua_toboolean(L, 2) != 0);
	int count = luaL_checkint(L, 3);
	int row = luaL_checkint(L, 4);

	g_pActionFrame->SetActionBarSetting(index, enable, count, row);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionBarSetting(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	if ( index >= 0 && index < 4 )
	{
		ActionBarSetting& setting = g_pActionFrame->GetActionBarSetting();
		lua_pushboolean(L, setting.bar[index].isEnable);
		lua_pushnumber(L, setting.bar[index].count);
		lua_pushnumber(L, setting.bar[index].row);
		return 3;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetActionBarLocked(lua_State* L)
{
	g_pActionFrame->SetActionBarLocked((lua_toboolean(L, 1) != 0));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionBarLocked(lua_State* L)
{
	lua_pushboolean(L, g_pActionFrame->GetActionBarLocked());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetActionBarHideEmpty(lua_State* L)
{
	g_pActionFrame->SetActionBarHideEmpty((lua_toboolean(L, 1) != 0));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetActionBarHideEmpty(lua_State* L)
{
	lua_pushboolean(L, g_pActionFrame->GetActionBarHideEmpty());
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_ResetActionSetting(lua_State* L)
{
	g_pActionFrame->ResetActionBarSetting();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetImplementActionInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	// 最後一個
	if ( index == 9 )
	{
		lua_pushstring(L, "Interface\\Icons\\dismount");			// ICON 圖示
		return 1;
	}
	else
	{
		ImplementActionItem* item = g_pActionFrame->GetImplementActionItem(index);
		if ( item )
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->skillId);
			if ( itemDB )
			{
				GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
				if ( imageDB )
				{
					lua_pushstring(L, imageDB->Image.ACTField);			// ICON 圖示
					return 1;
				}
			}
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetImplementActionCooldown(lua_State* L)
{
	// 0總時間量 1剩於時間
	float coldown[2] = { 0.0f, 0.0f };

	ImplementActionItem* item = g_pActionFrame->GetImplementActionItem(luaL_checkint(L, 1) - 1);	
	if ( item )
	{
		MagicColdownStruct* magicColdown = NetCli_MoveChild::GetImplementMagicColdown(item->speller);
		if ( magicColdown )
		{
			RoleDataEx::GetMagicCooldown(*magicColdown, item->skillId, coldown);
		}
	}

	lua_pushnumber(L, coldown[0]);
	lua_pushnumber(L, coldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_UseImplementAction(lua_State* L)
{
	g_pActionFrame->UseImplementAction(luaL_checkint(L, 1) - 1);
	return 0;
}

