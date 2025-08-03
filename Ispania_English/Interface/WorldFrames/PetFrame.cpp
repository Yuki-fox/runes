#include "../netwaker_member/NetWakerClientInc.h"
#include "../netwaker_member/net_cultivatepet/NetCli_CultivatePetChild.h"
#include "../../mainproc/GameMain.h"
#include "ChatFrame.h"
#include "ItemUtil.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "PetFrame.h"
#include "CastingBarFrame.h"
#include "../ItemLink/ItemLink.h"
#include "RoleData/RoleBaseDef.h"
CPetFrame*					g_pPetFrame = NULL;

int LuaFunc_SetPetItem				(lua_State* L);
int LuaFunc_TakeoutPetItem			(lua_State* L);
int LuaFunc_HasPetItem				(lua_State* L);
int LuaFunc_SetPetItemName			(lua_State* L);
int LuaFunc_GetPetItemName			(lua_State* L);
int LuaFunc_GetPetItemExperience	(lua_State* L);
int LuaFunc_GetPetItemLevel			(lua_State* L);
int LuaFunc_GetPetItemSkillPoint	(lua_State* L);
int LuaFunc_GetPetItemProperty		(lua_State* L);
int LuaFunc_GetPetItemGrowth		(lua_State* L);
int LuaFunc_GetPetItemAbility		(lua_State* L);
int LuaFunc_GetPetItemAssist		(lua_State* L);
int LuaFunc_GetPetItemNumSkill		(lua_State* L);
int LuaFunc_GetPetItemSkillInfo		(lua_State* L);
int LuaFunc_IsPetItemSkillLearn		(lua_State* L);
int LuaFunc_PetItemLearnSkill		(lua_State* L);
int LuaFunc_ClickPetCraftItem		(lua_State* L);
int LuaFunc_GetPetEquipmentItem		(lua_State* L);
int LuaFunc_ClickPetFeedItem		(lua_State* L);
int LuaFunc_ClearPetFeedItem		(lua_State* L);
int LuaFunc_GetPetFeedFoodItem		(lua_State* L);
int LuaFunc_PetCraftingStart		(lua_State* L);
int LuaFunc_PetCraftingStop			(lua_State* L);
int LuaFunc_PetCraftHarvest			(lua_State* L);

int LuaFunc_HasPetCraftHarvest		(lua_State* L);

int LuaFunc_GetPetNumCraftItems		(lua_State* L);
int LuaFunc_GetPetCraftItemInfo		(lua_State* L);
int LuaFunc_GetPetLifeSkillInfo		(lua_State* L);
int LuaFunc_IsPetCraftingStart		(lua_State* L);							// 開始製作中

int LuaFunc_GetPetCraftList			(lua_State* L);
int LuaFunc_GetPetNumPossibleProductItems	(lua_State* L);
int LuaFunc_GetPetPossibleProductItemDetail	(lua_State* L);

int LuaFunc_GetPetTalentSkillNum	(lua_State* L);
int LuaFunc_GetPetTalentSkillInfo	(lua_State* L);

int LuaFunc_IsPetSummoned			(lua_State* L);	

int LuaFunc_SummonPet				(lua_State* L);
int LuaFunc_ReturnPet				(lua_State* L);
int LuaFunc_FeedPet					(lua_State* L);
int LuaFunc_TravelPet				(lua_State* L);
int LuaFunc_ReleasePet				(lua_State* L);

int LuaFunc_SendPetEventSelection	(lua_State* L);

int LuaFunc_CheckPetMixResult		(lua_State* L);
int LuaFunc_GetEggIcon				(lua_State* L);
int LuaFunc_GetPetMixModeOneValue	(lua_State* L);
int LuaFunc_SetPetMix				(lua_State* L);
int LuaFunc_GetPetMixModeTwoValue	(lua_State* L);
int LuaFunc_GetPetMixModeSwitchGood	(lua_State* L);
int LuaFunc_GetPetMixDayMergeCount	(lua_State* L);
int LuaFunc_PetIsSummon				(lua_State* L);
int LuaFunc_GetNowAbilityAssist		(lua_State* L);
int LuaFunc_GetNextMergeResetTime	(lua_State* L);
int	LuaFunc_IsPetStarUse			(lua_State* L);
int	LuaFunc_IsPetMixEventErr		(lua_State* L);
int LuaFunc_IsPetCraftProduct		(lua_State* L);
int LuaFunc_GetPetFeedItem			(lua_State* L);
int LuaFunc_SetPetFeedItem			(lua_State* L);
int LuaFunc_IsPetStateTrue			(lua_State* L);
int LuaFunc_ResetPetMergeTime		(lua_State* L);

static float g_fSummonCooldown = 0.0f;
static int	 g_iSummonIndex = -1;

// ----------------------------------------------------------------------------
CultivatePetLifeSkillTypeENUM Func_GetPetSkillTypeENUM(const char* keyStr)
{
	if ( strcmp(keyStr, "MINING") == 0 )
		return EM_CultivatePetLifeSkillType_Mining;
	else if ( strcmp(keyStr, "WOOD") == 0 )
		return EM_CultivatePetLifeSkillType_Lumbering;
	else if ( strcmp(keyStr, "HERB") == 0 )
		return EM_CultivatePetLifeSkillType_Herblism;
	return EM_CultivatePetLifeSkillType_None;
}

// ----------------------------------------------------------------------------
const char* Func_GetPetSkillKeyString(CultivatePetLifeSkillTypeENUM typeENUM)
{
	switch (typeENUM)
	{
	case EM_CultivatePetLifeSkillType_Mining:
		return "MINING";
	case EM_CultivatePetLifeSkillType_Lumbering:
		return "WOOD";
	case EM_CultivatePetLifeSkillType_Herblism:
		return "HERB";
	}
	return "";
}

// ----------------------------------------------------------------------------
CPetFrame::CPetFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pPetFrame = this;

	m_takeoutIndex			= -1;
	m_learnNeedPoint		= 0;
	m_updateNameIndex		= -1;

	m_pickupEQIndex			= -1;			// 已經拿取裝備的寵物編號
	m_pickupEQPosition		= -1;			// 已經拿取的裝備編號

	m_swapItemIndex			= -1;
	m_swapItemType			= -1;

	m_PetMix2				= 0;			
	m_PetMix3				= 0;

	for( int i = 0; i < 6; ++i )
	{
		m_craftingTime[i] = 0.0f;
		m_isStartCrafting[i] = false;
		m_isCancelCrafting[i] = false;
	}

	m_bAlreadyLearned		= false;
}

// ----------------------------------------------------------------------------
CPetFrame::~CPetFrame()
{
	g_pPetFrame = NULL;
}

// ----------------------------------------------------------------------------
void CPetFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "SetPetItem",						LuaFunc_SetPetItem );
		lua_register( L, "TakeoutPetItem",					LuaFunc_TakeoutPetItem );
		lua_register( L, "HasPetItem",						LuaFunc_HasPetItem );
		lua_register( L, "SetPetItemName",					LuaFunc_SetPetItemName );
		lua_register( L, "GetPetItemName",					LuaFunc_GetPetItemName );
		lua_register( L, "GetPetItemExperience",			LuaFunc_GetPetItemExperience );
		lua_register( L, "GetPetItemLevel",					LuaFunc_GetPetItemLevel );
		lua_register( L, "GetPetItemSkillPoint",			LuaFunc_GetPetItemSkillPoint );
		lua_register( L, "GetPetItemProperty",				LuaFunc_GetPetItemProperty );
		lua_register( L, "GetPetItemGrowth",				LuaFunc_GetPetItemGrowth );
		lua_register( L, "GetPetItemAbility",				LuaFunc_GetPetItemAbility );
		lua_register( L, "GetPetItemAssist",				LuaFunc_GetPetItemAssist );
		lua_register( L, "GetPetItemNumSkill",				LuaFunc_GetPetItemNumSkill );
		lua_register( L, "GetPetItemSkillInfo",				LuaFunc_GetPetItemSkillInfo );
		lua_register( L, "IsPetItemSkillLearn",				LuaFunc_IsPetItemSkillLearn );
		lua_register( L, "PetItemLearnSkill",				LuaFunc_PetItemLearnSkill );
		lua_register( L, "ClickPetCraftItem",				LuaFunc_ClickPetCraftItem );
		lua_register( L, "GetPetEquipmentItem",				LuaFunc_GetPetEquipmentItem );
		lua_register( L, "ClickPetFeedItem",				LuaFunc_ClickPetFeedItem );
		lua_register( L, "ClearPetFeedItem",				LuaFunc_ClearPetFeedItem );
		lua_register( L, "GetPetFeedFoodItem",				LuaFunc_GetPetFeedFoodItem );
		lua_register( L, "PetCraftingStart",				LuaFunc_PetCraftingStart );
		lua_register( L, "PetCraftingStop",					LuaFunc_PetCraftingStop );
		lua_register( L, "PetCraftHarvest",					LuaFunc_PetCraftHarvest );
		
		lua_register( L, "HasPetCraftHarvest",				LuaFunc_HasPetCraftHarvest );

		lua_register( L, "GetPetNumCraftItems",				LuaFunc_GetPetNumCraftItems );
		lua_register( L, "GetPetCraftItemInfo",				LuaFunc_GetPetCraftItemInfo );
		lua_register( L, "GetPetLifeSkillInfo",				LuaFunc_GetPetLifeSkillInfo );
		lua_register( L, "IsPetCraftingStart",				LuaFunc_IsPetCraftingStart );

		lua_register( L, "GetPetCraftList",					LuaFunc_GetPetCraftList );
		lua_register( L, "GetPetNumPossibleProductItems",	LuaFunc_GetPetNumPossibleProductItems );
		lua_register( L, "GetPetPossibleProductItemDetail",	LuaFunc_GetPetPossibleProductItemDetail );

		lua_register( L, "GetPetTalentSkillNum",			LuaFunc_GetPetTalentSkillNum );
		lua_register( L, "GetPetTalentSkillInfo",			LuaFunc_GetPetTalentSkillInfo );

		lua_register( L, "IsPetSummoned",					LuaFunc_IsPetSummoned );

		lua_register( L, "SummonPet",						LuaFunc_SummonPet );
		lua_register( L, "ReturnPet",						LuaFunc_ReturnPet );
		lua_register( L, "FeedPet",							LuaFunc_FeedPet );
		lua_register( L, "TravelPet",						LuaFunc_TravelPet );
		lua_register( L, "ReleasePet",						LuaFunc_ReleasePet );
		lua_register( L, "SendPetEventSelection",			LuaFunc_SendPetEventSelection );		

		lua_register( L, "CheckPetMixResult",				LuaFunc_CheckPetMixResult );		
		lua_register( L, "GetEggIcon",						LuaFunc_GetEggIcon );		

		lua_register( L, "GetPetMixModeOneValue",			LuaFunc_GetPetMixModeOneValue );		
		lua_register( L, "SetPetMix",						LuaFunc_SetPetMix );		
		lua_register( L, "GetPetMixModeTwoValue",			LuaFunc_GetPetMixModeTwoValue );		
		lua_register( L, "GetPetMixModeSwitchGood",			LuaFunc_GetPetMixModeSwitchGood );		
		
		lua_register( L, "GetPetMixDayMergeCount",			LuaFunc_GetPetMixDayMergeCount );				
		lua_register( L, "PetIsSummon",						LuaFunc_PetIsSummon );				

		lua_register( L, "GetNowAbilityAssist",				LuaFunc_GetNowAbilityAssist );				
		lua_register( L, "GetNextMergeResetTime",			LuaFunc_GetNextMergeResetTime );				
		lua_register( L, "IsPetStarUse",					LuaFunc_IsPetStarUse );		
		lua_register( L, "IsPetMixEventErr",				LuaFunc_IsPetMixEventErr );		
		lua_register( L, "IsPetCraftProduct",				LuaFunc_IsPetCraftProduct );

		lua_register( L, "GetPetFeedItem",					LuaFunc_GetPetFeedItem );

		lua_register( L, "SetPetFeedItem",					LuaFunc_SetPetFeedItem );
		lua_register( L, "IsPetStateTrue",					LuaFunc_IsPetStateTrue );
		lua_register( L, "ResetPetMergeTime",				LuaFunc_ResetPetMergeTime );
	
		
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::BuildFrames()
{
	m_learnNeedPoint		= 0;
	m_takeoutIndex			= -1;
	m_updateNameIndex		= -1;
	m_feedItemIndex			= -1;
	m_swapItemIndex			= -1;
	m_swapItemType			= -1;
	m_petpremixlevel		= 1;
	InitSkillTopLevel();
}


void CPetFrame::LeaveWorld()
{
	for( int i = 0; i < 6; ++i )
	{
		m_craftingTime[i] = 0.0f;
		m_isStartCrafting[i] = false;
		m_isCancelCrafting[i] = false;
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::Update(float elapsedTime)
{
	if ( m_pickupEQIndex != -1 )
	{
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
		if ( m_pickupEQIndex >= 0 && m_pickupEQIndex < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[m_pickupEQIndex];
			if ( pet.IsEmpty() )
			{
				if ( itemInfo.petEQ.index == m_pickupEQIndex && itemInfo.petEQ.pos == m_pickupEQPosition )
					g_pItemClipboard->Clear();

				m_pickupEQIndex = -1;
				m_pickupEQPosition = -1;
			}
			else if ( pet.Tools.Pos == EM_ItemState_CliDisabled )
			{
				if ( g_pItemClipboard->GetType() != eClipboardItem_Type_PetEQ || itemInfo.petEQ.index != m_pickupEQIndex || itemInfo.petEQ.pos != m_pickupEQPosition )
				{
					pet.Tools.Pos = EM_ItemState_NONE;
					m_pickupEQIndex = -1;
					m_pickupEQPosition = -1;
				}
			}
		}
		else
		{
			m_pickupEQIndex = -1;
			m_pickupEQPosition = -1;
		}
	}

	if ( m_feedItemIndex != -1 )
	{
		g_pBagFrame->Locked( m_feedItemIndex );
	}

	for ( int i = 0; i < 6; i++ )
	{
		m_craftingTime[i] -= elapsedTime;
		if ( m_craftingTime[i] < 0.0f )
			m_craftingTime[i] = 0.0f;
	}

	if( g_fSummonCooldown > 0.0f )
	{
		CRoleSprite* player = g_pGameMain->GetPlayer();

		// 失敗檢查
		// 檢查中斷, 若移動向量不等於0, 或播放中動作不是指定的動作, 表動作中斷了
		if ( player )
		{
			if( player->GetMoveVector() != 0.0f )
			{
				StopSummonPet();

				return;
			}

			CRuFusion_ActorState actplaying = player->GetActorState(3);
			if( !( actplaying >= ruFUSION_ACTORSTATE_CHANNEL_BEGIN && actplaying <= ruFUSION_ACTORSTATE_CHANNEL_END ) )
			{
				StopSummonPet();

				return;
			}
		}

		g_fSummonCooldown -= elapsedTime;

		if( g_fSummonCooldown <= 0.0f )
		{
			g_fSummonCooldown = 0.0f;			

			g_pCastingBarFrame->CastingStop( eCastingBar_PetSummon );

			player->SetActorState( ruFUSION_ACTORSTATE_CHANNEL_END );
			player->EndCastingEffect();

			NetCli_CultivatePet::SL_PetEvent(g_iSummonIndex, 0, EM_CultivatePetCommandType_Summon);

			g_iSummonIndex = -1;

			return;
		}
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::StopSummonPet()
{
	CRoleSprite* player = g_pGameMain->GetPlayer();

	g_fSummonCooldown = 0.0f;
	g_iSummonIndex = -1;

	g_pCastingBarFrame->CastingFailed( eCastingBar_PetSummon );				

	player->SetActorState( ruFUSION_ACTORSTATE_CHANNEL_END );
	player->EndCastingEffect();
}

// ----------------------------------------------------------------------------
bool CPetFrame::IsSummoningPet()
{
	if( g_fSummonCooldown > 0.0f && g_iSummonIndex != -1 )
		return true;

	return false;
}

// ----------------------------------------------------------------------------
void CPetFrame::LearnMagicSkill(int index, int position)
{
	PetLearnSkillResult learnResult = CheckSkillLearnLimit(index, position);

	switch (learnResult)
	{
	case ePetLearnSkill_Level:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_LEVEL"));
		break;

	case ePetLearnSkill_KeyItem:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_KEYITEM"));
		break;

	case ePetLearnSkill_Point:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_POINT"));
		break;

	case ePetLearnSkill_STR:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_STR"));
		break;

	case ePetLearnSkill_STA:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_STA"));
		break;

	case ePetLearnSkill_INT:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_INT"));
		break;

	case ePetLearnSkill_MND:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_MND"));
		break;

	case ePetLearnSkill_AGI:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_AGI"));
		break;

	case ePetLearnSkill_Already:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_ALREADY_MAX"));
		break;

	case ePetLearnSkill_Unknow:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_UNKNOW"));		
		break;

	case ePetLearnSkill_Success:
		{
			PetMagicSkillList* skillList = g_pPetFrame->GetSkillListInfo(index);
			if ( skillList )
			{
				PetMagicSkillInfo* skillInfo = skillList->GetAtIndex(position);
				if ( skillInfo )
				{
					if( m_bAlreadyLearned )
					{
						skillInfo = &m_nextLevelSkill[index][skillInfo->MagicID];
					}

					m_learnNeedPoint = skillInfo->Limit.Point;
					NetCli_CultivatePet::SL_LearnSkill(index, skillInfo->MagicID, skillInfo->PowerLevel);
				}
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::InitSkillTopLevel()
{
	vector<CultivatePetLearnMagicTableStruct>& magicTable = g_ObjectData->GetCultivatePetLearnMagicTable();
	for( vector<CultivatePetLearnMagicTableStruct>::iterator iter = magicTable.begin(); iter != magicTable.end(); iter++ )
	{
		map<int, int>::iterator mapIter = m_topLevelMap.find( iter->MagicID );

		if( mapIter != m_topLevelMap.end() )
		{
			if( iter->PowerLv > mapIter->second )
			{
				mapIter->second = iter->PowerLv;
			}
		}
		else
		{
			m_topLevelMap.insert( make_pair( iter->MagicID, iter->PowerLv ) );
		}
	}
}

// ----------------------------------------------------------------------------
bool CPetFrame::UpdateSkillInfo(int index)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		// 清除技能項目
		m_skillList[index].Clear();
		m_nextLevelSkill[index].clear();

		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		if ( pet.IsEmpty() == false )
		{
			vector<CultivatePetLearnMagicTableStruct> tempMagic;			// 記錄所有條件符合技能資料
			multimap<int, CultivatePetLearnMagicTableStruct> filterMagic;
			vector<CultivatePetLearnMagicTableStruct> nextLevelMagic;

			// 過瀘法術
			vector<CultivatePetLearnMagicTableStruct>& magicTable = g_ObjectData->GetCultivatePetLearnMagicTable();
			for ( vector<CultivatePetLearnMagicTableStruct>::iterator iter = magicTable.begin(); iter != magicTable.end(); iter++ )
			{
				// 踢除不同類型
				if ( iter->Limit.PetProperty != -1 && iter->Limit.PetProperty != pet.Property )
					continue;

				if ( iter->Limit.PetType != -1 && iter->Limit.PetType != pet.PetType )
					continue;

				tempMagic.push_back(*iter);
			}

			// 只保留已學技能中最高等級的以及未學技能中最低等級的
			for ( vector<CultivatePetLearnMagicTableStruct>::iterator iter = tempMagic.begin(); iter != tempMagic.end(); iter++ )
			{
				bool bIsLearned = false;

				for ( int i = 0; i < pet.Magic.Size(); i++ )
				{
					// 判斷是否已經學習
					if ( pet.Magic[i].ID == iter->MagicID )
					{
						if ( iter->PowerLv == pet.Magic[i].Lv )	// 已學技能中最高等級的
						{
							filterMagic.insert(make_pair(iter->MagicID, *iter));
						}
						else if ( iter->PowerLv == pet.Magic[i].Lv + 1 )	// 比已學技能中最高等級的高一級的(下次升級時要學的)
						{
							nextLevelMagic.push_back(*iter);
						}

						bIsLearned = true;
						break;
					}
				}

				if ( bIsLearned == false && iter->PowerLv == 0 )	// 未學技能中最低等級的
				{
					filterMagic.insert(make_pair(iter->MagicID, *iter));
				}
			}			

			PetMagicSkillInfo skillInfo;
			for ( multimap<int, CultivatePetLearnMagicTableStruct>::iterator iter = filterMagic.begin(); iter != filterMagic.end(); iter++ )
			{
				// 記錄法術資料
				skillInfo.MagicID		= iter->second.MagicID;
				skillInfo.Limit.Str		= iter->second.Limit.Str;
				skillInfo.Limit.Sta		= iter->second.Limit.Sta;
				skillInfo.Limit.Int		= iter->second.Limit.Int;
				skillInfo.Limit.Mnd		= iter->second.Limit.Mnd;
				skillInfo.Limit.Agi		= iter->second.Limit.Agi;
				skillInfo.Limit.Level	= iter->second.Limit.Lv;
				skillInfo.Limit.KeyItem = iter->second.Limit.KeyItem;
				skillInfo.Limit.Point	= iter->second.SkillPoint;
				skillInfo.Limit.Lock	= iter->second.Limit.Lock;
				skillInfo.PowerLevel	= iter->second.PowerLv;
				skillInfo.IsLearn		= false;
				skillInfo.IsTopLevel	= false;

				for ( int i = 0; i < pet.Magic.Size(); i++ )
				{
					// 判斷是否已經學習
					if ( pet.Magic[i].ID == iter->second.MagicID )
					{
						skillInfo.IsLearn = true;
					}
				}

				if ( m_topLevelMap[skillInfo.MagicID] == skillInfo.PowerLevel )
					skillInfo.IsTopLevel = true;

				m_skillList[index].Add(skillInfo);
			}

			for ( vector<CultivatePetLearnMagicTableStruct>::iterator iter = nextLevelMagic.begin(); iter != nextLevelMagic.end(); iter++ )
			{
				skillInfo.MagicID		= iter->MagicID;
				skillInfo.Limit.Str		= iter->Limit.Str;
				skillInfo.Limit.Sta		= iter->Limit.Sta;
				skillInfo.Limit.Int		= iter->Limit.Int;
				skillInfo.Limit.Mnd		= iter->Limit.Mnd;
				skillInfo.Limit.Agi		= iter->Limit.Agi;
				skillInfo.Limit.Level	= iter->Limit.Lv;
				skillInfo.Limit.KeyItem = iter->Limit.KeyItem;
				skillInfo.Limit.Point	= iter->SkillPoint;
				skillInfo.Limit.Lock	= iter->Limit.Lock;
				skillInfo.PowerLevel	= iter->PowerLv;
				skillInfo.IsLearn		= false;
				skillInfo.IsTopLevel	= false;
				skillInfo.IsLearn		= true;

				if ( m_topLevelMap[skillInfo.MagicID] == skillInfo.PowerLevel )
					skillInfo.IsTopLevel = true;

				m_nextLevelSkill[index].insert(make_pair(skillInfo.MagicID, skillInfo));
			}

			return true;
		}		
	}
	return false;
}

// ----------------------------------------------------------------------------
PetMagicSkillList* CPetFrame::GetSkillListInfo(int index)
{
	if ( index >= 0 && index < 6 )
		return m_skillList + index;
	return NULL;
}

// ----------------------------------------------------------------------------
PetLearnSkillResult CPetFrame::CheckSkillLearnLimit(int index, int skillIndex)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		PetMagicSkillList* skillList = g_pPetFrame->GetSkillListInfo(index);

		if ( skillList && pet.IsEmpty() == false )
		{
			PetMagicSkillInfo* skillInfo = skillList->GetAtIndex(skillIndex);
			if ( skillInfo )
			{
				m_bAlreadyLearned = false;

				// 是否已經最高級了
				for ( int i = 0; i < pet.Magic.Size(); i++ )
				{
					if ( pet.Magic[i].ID == skillInfo->MagicID && pet.Magic[i].Lv == skillInfo->PowerLevel )
					{
						if ( skillInfo->IsTopLevel )
							return ePetLearnSkill_Already;
						else
						{
							m_bAlreadyLearned = true;
							skillInfo = &m_nextLevelSkill[index][skillInfo->MagicID];
						}

						break;
					}
				}

				if ( skillInfo->Limit.KeyItem != 0 && CNetGlobal::RoleData()->CheckKeyItem( skillInfo->Limit.KeyItem ) == false )
					return ePetLearnSkill_KeyItem;

				if ( skillInfo->Limit.Level > pet.Lv )
					return ePetLearnSkill_Level;

				if ( skillInfo->Limit.Point > pet.SkillPoint )
					return ePetLearnSkill_Point;

				if ( skillInfo->Limit.Str > pet.STR )
					return ePetLearnSkill_STR;

				if ( skillInfo->Limit.Sta > pet.STA )
					return ePetLearnSkill_STA;

				if ( skillInfo->Limit.Int > pet.INT )
					return ePetLearnSkill_INT;

				if ( skillInfo->Limit.Mnd > pet.MND )
					return ePetLearnSkill_MND;

				if ( skillInfo->Limit.Agi > pet.AGI )
					return ePetLearnSkill_AGI;

				return ePetLearnSkill_Success;
			}
		}
	}

	return ePetLearnSkill_Unknow;
}

// ----------------------------------------------------------------------------
PetMagicSkillInfo* CPetFrame::GetNextLevelSkill(int index, int iMagicID)
{
	if( index >= 0 && index < MAX_CultivatePet_Count )
	{
		map<int, PetMagicSkillInfo>::iterator Iter = m_nextLevelSkill[index].find( iMagicID );

		if( Iter != m_nextLevelSkill[index].end() )
			return &m_nextLevelSkill[index][iMagicID];
	}

	return 0;
}

// ----------------------------------------------------------------------------
void CPetFrame::PasteCraftTool(int index)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.IsEmpty() == false )
		{
			if( pet.EventType == EM_CultivatePetCommandType_Summon )
			{			
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_SUMMON_MODE"));
				return;
			}

			ItemFieldStruct& craftTool = pet.Tools;

			ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

			// 拿取與做用物品相同
			if (g_pItemClipboard->GetType() == eClipboardItem_Type_PetEQ && itemInfo.petEQ.index == index && itemInfo.petEQ.pos == EM_CultivatePetEQType_Tools)
			{
				UnlockPickupEQ();
				g_pItemClipboard->Clear();
				return;
			}

			// 物品已鎖定
			if ( craftTool.Pos != EM_ItemState_NONE )
				return;

			switch(g_pItemClipboard->GetType())
			{
			case eClipboardItem_Type_None:					// 拿取物品
				{
					GameObjDbStruct* itemDB = CNetGlobal::GetObj( craftTool.OrgObjID );
					if ( itemDB )
					{
						craftTool.Pos = EM_ItemState_CliDisabled;
						g_pItemClipboard->SetFile(ItemUTIL::GetItemIcon(&craftTool));
						g_pItemClipboard->PickupPetEQ(index, EM_CultivatePetEQType_Tools);
						m_pickupEQIndex		= index;
						m_pickupEQPosition	= EM_CultivatePetEQType_Tools;
						return;
					}
				}
				return;

			case eClipboardItem_Type_Bag:					// 背包 -> 裝備
				{
					SwapItem(index, EM_CultivatePetEQType_Tools, itemInfo.bag.pos, true);
				}
				break;

			case eClipboardItem_Type_Bank:					// 銀行 -> 裝備
			case eClipboardItem_Type_Equipment:				// 裝備 -> 裝備
			case eClipboardItem_Type_House:					// 房屋 -> 裝備
			case eClipboardItem_Type_Trade:
			case eClipboardItem_Type_Action:
			case eClipboardItem_Type_Skill:
			case eClipboardItem_Type_Shop:
			case eClipboardItem_Type_Macro:
			case eClipboardItem_Type_Emote:
				break;
			}
			g_pItemClipboard->Clear();
		}
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::UpdateCreaftInfo(int index)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		m_craftList[index].clear();

		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.Tools.IsEmpty() == false )
		{
			for ( int i = 0; i < g_ObjectData->GetCultivatePetNumLifeSkill(); i++ )
			{
				CultivatePetLifeSkillTableStruct* lifeSkill = g_ObjectData->GetCultivatePetLifeSkill(i);
				if ( lifeSkill->Need.ToolID == pet.Tools.OrgObjID )
				{
					m_craftList[index].push_back(i);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::UnlockPickupEQ()
{
	if ( m_pickupEQIndex != -1 )
	{
		if ( m_pickupEQIndex >= 0 && m_pickupEQIndex < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[m_pickupEQIndex];
			if ( pet.IsEmpty() == false )
			{
				pet.EQ[m_pickupEQPosition].Pos = EM_ItemState_NONE;
			}
		}
	}

	m_pickupEQIndex = -1;
	m_pickupEQPosition = -1;
}

// ----------------------------------------------------------------------------
void CPetFrame::SwapItem(int index, int position, int bagIndex, bool isFromBag)
{
	switch ( position )
	{
	case EM_CultivatePetEQType_Tools:
		{
			// 檢查背包欄位是否為工具
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(bagIndex);
			if ( item && item->IsEmpty() == false )
			{
				bool isTools = false;
				int i = 0;		
				CultivatePetLifeSkillTableStruct* table;
				while ( (table = g_ObjectData->GetCultivatePetLifeSkill(i)) != NULL )
				{
					if ( table->Need.ToolID == item->OrgObjID )
					{
						isTools = true;
						break;
					}
					i++;
				}

				// 是否為工具
				if ( isTools == false )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_NOT_CRAFT_TOOLS"));
					return;
				}
			}
		}
		break;
	case EM_CultivatePetEQType_Source:
		break;
	case EM_CultivatePetEQType_Product1:
		break;
	case EM_CultivatePetEQType_Product2:
		break;
	case EM_CultivatePetEQType_Other0:
		break;
	case EM_CultivatePetEQType_Other1:
		break;
	case EM_CultivatePetEQType_Other2:
		break;
	case EM_CultivatePetEQType_Other3:
		break;
	}

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.IsEmpty() == false && m_swapItemType == -1 )
		{
			ItemFieldStruct& eqItem = pet.EQ[position];
			eqItem.Pos = EM_ItemState_CliDisabled;
			
			NetCli_CultivatePet::SL_SwapItem(index, bagIndex, (CultivatePetEQTypeENUM)position, isFromBag);			
			m_swapItemIndex = index;
			m_swapItemType = position;
		}
	}
}

// ----------------------------------------------------------------------------
// 更換裝備改變
void CPetFrame::UpdateEQItem(int index, int position)
{
	switch (position)
	{
	case EM_CultivatePetEQType_Tools:
		// 更新可以製做清單
		UpdateCreaftInfo(index);
		break;

	case EM_CultivatePetEQType_Source:
	case EM_CultivatePetEQType_Product1:
	case EM_CultivatePetEQType_Product2:
	case EM_CultivatePetEQType_Product3:
	case EM_CultivatePetEQType_Other0:
	case EM_CultivatePetEQType_Other1:
	case EM_CultivatePetEQType_Other2:
	case EM_CultivatePetEQType_Other3:
		break;
	}

	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, index);
		lua_setglobal(L, "arg1");
		lua_pushnumber(L, position);
		lua_setglobal(L, "arg2");
		g_pPetFrame->SendWorldEvent("PET_EQUIPMENT_UPDATE");
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::SetFeedItemIndex(int index)
{
	if ( m_feedItemIndex != index )
	{
		g_pBagFrame->Unlocked(m_feedItemIndex);
		m_feedItemIndex = index;
		SendWorldEvent("PET_FEED_INFO_UPDATE");
	}
}
void CPetFrame::IsPedFoodNull()
{
	BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
	if( m_feedItemIndex != -1 )
	{
		ItemFieldStruct  &iItemBase = bodyItems.ItemBase[m_feedItemIndex - 60];
		if( iItemBase.Count <= 0)
		{		
			g_pPetFrame->SetFeedItemIndex(-1);
		}
	}
}
// ----------------------------------------------------------------------------
void CPetFrame::FeedItemPickup()
{
	int itemIndex = m_feedItemIndex;

	SetFeedItemIndex(-1);

	g_pBagFrame->Cut(itemIndex);
}

// ----------------------------------------------------------------------------
void CPetFrame::FeedItemPutdown(int index)
{
	if ( m_feedItemIndex != -1 )
	{
		// 拿取上一個舊物品
		FeedItemPickup();
	}
	else
	{
		// 更新物品不是背包鎖定
		g_pBagFrame->ClearPickupItemIndex();
	}

	// 鎖定該物品
	g_pBagFrame->Locked(index);

	// 設定販賣物品
	SetFeedItemIndex(index);
}

// ----------------------------------------------------------------------------
void CPetFrame::FeedFood(int index)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.IsEmpty() == false )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(m_feedItemIndex);
			if ( item == NULL || item->IsEmpty() )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_NOT_PET_FOOD"));
				return;
			}

			GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->OrgObjID);
			if ( itemDB == NULL || itemDB->Classification != EM_ObjectClass_Item || itemDB->Item.ItemType != EM_ItemType_Pet_Food )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_NOT_PET_FOOD"));
				return;
			}

			NetCli_CultivatePet::SL_PetEvent(index, m_feedItemIndex, EM_CultivatePetCommandType_Feed);
			return;
		}
	}

	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_NOT_EXIST"));
	return;
}

// ----------------------------------------------------------------------------
CultivatePetEQTypeENUM CPetFrame::GetPetEQTypeENUM(const char* key)
{
	if ( strcmp(key, "tools") == 0 )
		return EM_CultivatePetEQType_Tools;
	if ( strcmp(key, "stuff") == 0 )
		return EM_CultivatePetEQType_Source;
	if ( strcmp(key, "product1") == 0 )
		return EM_CultivatePetEQType_Product1;
	if ( strcmp(key, "product2") == 0 )
		return EM_CultivatePetEQType_Product2;
	if ( strcmp(key, "product3") == 0 )
		return EM_CultivatePetEQType_Product3;
	if ( strcmp(key, "other1") == 0 )
		return EM_CultivatePetEQType_Other0;
	if ( strcmp(key, "other2") == 0 )
		return EM_CultivatePetEQType_Other1;
	if ( strcmp(key, "other3") == 0 )
		return EM_CultivatePetEQType_Other2;
	if ( strcmp(key, "other4") == 0 )
		return EM_CultivatePetEQType_Other3;
	return EM_CultivatePetEQType_MAX;
}

// ----------------------------------------------------------------------------
const char* CPetFrame::GetPetEQTypeKeyString(CultivatePetEQTypeENUM type)
{
	switch (type)
	{
	case EM_CultivatePetEQType_Tools:
		return "tools";
	case EM_CultivatePetEQType_Source:
		return "stuff";
	case EM_CultivatePetEQType_Product1:
		return "product1";
	case EM_CultivatePetEQType_Product2:
		return "product2";
	case EM_CultivatePetEQType_Product3:
		return "product3";
	case EM_CultivatePetEQType_Other0:
		return "other1";
	case EM_CultivatePetEQType_Other1:
		return "other2";
	case EM_CultivatePetEQType_Other2:
		return "other3";
	case EM_CultivatePetEQType_Other3:
		return "other4";
	}
	return "";
}

void CPetFrame::PushPetOK(int index)
{
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");

		SendWorldEvent("PET_EGG_PUSH_OK");
	}
}

void CPetFrame::ReleasePet(int index)
{
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");

		SendWorldEvent("PET_RELEASE_SUCCESS");
	}
}

// ----------------------------------------------------------------------------
GameObjDbStructEx* CPetFrame::GetCraftProductItem(int index, int position, int itemIndex)
{	
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = GetCraftListInfo(index);
		if ( position >= 0 && position < (int)craftList.size() )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(craftList[position]);
			if ( lifeTable )
			{
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(lifeTable->Product.ItemID);		
				if ( itemDB )
				{
					return g_ObjectData->GetObj(itemDB->Treasure.Item[itemIndex].OrgID);					
				}
			}
		}
	}
	return NULL;
}

// ----------------------------------------------------------------------------
void CPetFrame::CraftingStart(int index, float time)
{
	if ( index < 0 && index >= 6 )
		return;

	if ( m_isStartCrafting[index] )
		return;

	m_isStartCrafting[index] = true;
	m_craftingTime[index] = time;

	// 送出事件更新
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, 0.0f);				// 目前時間
		lua_setglobal(L, "arg1");

		lua_pushnumber(L, time);				// 最大時間
		lua_setglobal(L, "arg2");
		
		lua_pushnumber(L, index);	// 寵物index
		lua_setglobal(L, "arg3");

		SendWorldEvent("PET_CRAFTING_START");
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::CraftingStop(int index)
{
	if ( index < 0 && index >= 6 )
		return;

	m_isStartCrafting[index] = false;
	m_craftingTime[index] = 0.0f;
	
	// 送出事件
	SendWorldEvent("PET_CRAFTING_END");
}

// ----------------------------------------------------------------------------
void CPetFrame::CraftingFailed(int index)
{
	if ( index < 0 && index >= 6 )
		return;

	if ( m_isStartCrafting[index] == false && m_isCancelCrafting[index] == false )
		return;

	m_isStartCrafting[index] = false;
	m_isCancelCrafting[index] = false;
	m_craftingTime[index] = 0.0f;

	// 送出事件
	SendWorldEvent("PET_CRAFTING_FAILED");
}

// ----------------------------------------------------------------------------
void CPetFrame::SendCraftingStop(int index)
{
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		if ( m_isStartCrafting[index] == false )
			return;

		m_isCancelCrafting[index] = true;
		NetCli_CultivatePet::SL_PetLifeSkill(index, EM_CultivatePetLifeSkillType_None, 0, 0);
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::UpdateAbility(int index, int eventType)
{
	lua_State* L = g_pPetFrame->GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, index);
		lua_setglobal(L, "arg1");
		SendWorldEvent("PET_FRAME_UPDATE");
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_FixPetBase(int PetPos , CultivatePetStruct& PetBase)
{
	if ( PetPos >= 0 && PetPos < MAX_CultivatePet_Count )
	{
		CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos] = PetBase;
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_LearnSkillResult(bool Result , int PetPos , int MagicID , int MagicLv)
{
	if ( Result == false )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_LEARN_SKILL_FAILED"));
		if ( PetPos >= 0 && PetPos < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos];
			if(pet.EventType == EM_CultivatePetCommandType_Summon)
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_SUMMON"));		
			}
		}
		return;
	}

	const char* magicName = "";
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(MagicID);
	if ( itemDB )
		magicName = itemDB->GetName();

	if ( PetPos >= 0 && PetPos < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos];

		// pet.SkillPoint -= GetLearnNeedPoint();

		// 是否有相同法術
		bool isNew = true;

		for ( int i = 0; i < pet.Magic.Size(); i++ )
		{
			if ( pet.Magic[i].ID == MagicID )
			{
				pet.Magic[i].Lv = MagicLv;
				isNew = false;
				break;
			}
		}

		if ( isNew )
		{
			MagicBaseStruct magic;
			magic.ID = MagicID;
			magic.Lv = MagicLv;
			pet.Magic.Push_Back(magic);
		}

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_LEARN_SKILL_SUCCESS"), magicName);
	}

	CNetGlobal::RoleData()->TempData.UpdateInfo.Recalculate_All = true;

	// 更新技能列表
	UpdateSkillInfo(PetPos);

	// 更新界面
	SendWorldEvent("PET_FRAME_UPDATE");
}

// ----------------------------------------------------------------------------
void CPetFrame::R_PetLifeResult( bool Result , CultivatePetLifeSkillTypeENUM SkillType , int PetPos , int EventTime )
{
	if ( Result == false )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_LIFESKILL_CRAFT_FAILED"));
		return;
	} 

	switch (SkillType)
	{	
	case EM_CultivatePetLifeSkillType_Mining:			// 採礦
	case EM_CultivatePetLifeSkillType_Lumbering:		// 伐木
	case EM_CultivatePetLifeSkillType_Herblism:			// 採草
		// 開始製造
		SendWorldEvent("PET_CRAFT_START");

		CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos].EventType = EM_CultivatePetCommandType_LiftSkill;
		break;
	
	case EM_CultivatePetLifeSkillType_None:
		// 中止生產
		SendWorldEvent("PET_CRAFT_END");

		// 中斷生產進度條
		CraftingFailed(PetPos);

		CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos].EventType = EM_CultivatePetCommandType_None;
		break;
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_GetProductResult( bool Result )
{
	if ( Result == false )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_HARVEST_PRODUCT_FAILED"));
		return;		
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_SwapItemResult(bool Result)
{

	int index	= m_swapItemIndex;
	int type	= m_swapItemType;
	g_pPetFrame->ClearSwapItemIndex();

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( type >= 0 && type < EM_CultivatePetEQType_MAX )
		{
			pet.EQ[type].Pos = EM_ItemState_NONE;
		}
	}

	// 失敗
	if ( Result == false )
	{
		SendWorldEvent("PET_SWAPITEM_FAILD");
	}
	
	else
	{
		SendWorldEvent("PET_SWAPITEM_SUCESS");
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_FixItem(int PetPos , int PetItemPos , ItemFieldStruct& Item)
{
	if ( PetPos >= 0 && PetPos < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos];
		if ( PetItemPos >= 0 && PetItemPos < EM_CultivatePetEQType_MAX )
		{
			pet.EQ[PetItemPos] = Item;			

			// 更新欄位
			UpdateEQItem(PetPos, PetItemPos);
		}
	}
}

// ----------------------------------------------------------------------------
void CPetFrame::R_PetEventProcess(CultivatePetCommandTypeENUM Event)
{

}

// ----------------------------------------------------------------------------
void CPetFrame::R_PetMargeResult(PetMargeResultENUM Result , int PetCount , int PetPos[3] , CultivatePetStruct& PetBase )
{	
	lua_State* L = g_pPetFrame->GetLuaState();
	switch (Result)
	{	
	case EM_PetMargeResult_OK:	
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_SUCESS"));
		if( PetBase.Lv != m_petpremixlevel )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_LEVEL_UP"));
		}
		CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[0]] = PetBase;
		if( PetCount == 2)
		{
			CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[1]].init();
		}
		else
		{
			CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[1]].init();
			CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[2]].init(); 
		}



		lua_pushnumber(L, PetPos[0]);
		lua_setglobal(L, "arg1");
		SendWorldEvent("PET_MIX_SUCESS");	
		break;
	case EM_PetMargeResult_DataErr:		
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_FAILD_01"));

		if(PetCount >= 2)
		{
			if ( ( PetPos[0] >= 0 && PetPos[0] < MAX_CultivatePet_Count ) && ( PetPos[1] >= 0 && PetPos[1] < MAX_CultivatePet_Count ))
			{
				if( PetCount == 3 )
				{
					if(PetPos[3] >= 0 && PetPos[3] < MAX_CultivatePet_Count )
					{
						if( ( CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[0]].EventType == EM_CultivatePetCommandType_Summon ) ||
							( CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[1]].EventType == EM_CultivatePetCommandType_Summon ) ||
							( CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[2]].EventType == EM_CultivatePetCommandType_Summon ) )
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_SUMMON"));		
						}
					}
				}		
				else if(PetCount == 2)
				{
					if( ( CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[0]].EventType == EM_CultivatePetCommandType_Summon ) ||
						( CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos[1]].EventType == EM_CultivatePetCommandType_Summon ) )
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_SUMMON"));		
					}
				}
			}
		}

		
		SendWorldEvent("PET_MIX_FAILED");	
		break;
	case EM_PetMargeResult_LevelErr:	
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_FAILD_02"));
		SendWorldEvent("PET_MIX_FAILED");	
		break;

	case EM_PetMargeResult_MergeCountErr:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_FAILD"));
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD10"));
		SendWorldEvent("PET_MIX_FAILED");	
		break;

	case EM_PetMargeResult_MergeTicketErr:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_FAILD"));
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD8"));
		SendWorldEvent("PET_MIX_FAILED");	
		break;

	case EM_PetMargeResult_EventTypeErr:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_RESULT_FAILD"));
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD12"));
		SendWorldEvent("PET_MIX_FAILED");	
		break;

	}
}
// ----------------------------------------------------------------------------
float CultivatePetStruct::AssistPower( int Type )
{
	GameObjDbStructEx* pPetObj = g_ObjectData->GetObj( ItemOrgID );
	float fPower = 0.0f;

//[ (成長率*等級*(目前資質+目前忠誠度)/160) + (目前屬性值*(目前資質)/30) ] * [ (目前忠誠度+20)/100 ]

	switch( Type )
	{
	case EM_CultivatePetStructValueType_Str: //STR
		fPower = ( pPetObj->Item.Pet.STR * Lv * ( Talent + Loyal ) / 160.0f ) + ( STR * Talent / 30.0f );
		fPower = fPower * ( Loyal + 20 ) / 100.0f ;
//		fPower = pPetObj->Item.Pet.STR * Talent * ( Loyal + 20 ) / 1500.0f + ( STR - pPetObj->Item.Pet.STR * 0.5f * Lv ) * 3;		
		break;
	case EM_CultivatePetStructValueType_Sta: //STA
		fPower = ( pPetObj->Item.Pet.STA * Lv * ( Talent + Loyal ) / 160 ) + ( STA * Talent / 30 );
		fPower = fPower * ( Loyal + 20 ) / 100 ;
//		fPower = pPetObj->Item.Pet.STA * Talent * ( Loyal + 20 ) / 1500.0f + ( STA - pPetObj->Item.Pet.STA * 0.5f * Lv ) * 3;
		break;
	case EM_CultivatePetStructValueType_Agi: //AGI
		fPower = ( pPetObj->Item.Pet.AGI * Lv * ( Talent + Loyal ) / 160 ) + ( AGI * Talent / 30 );
		fPower = fPower * ( Loyal + 20 ) / 100 ;
//		fPower = pPetObj->Item.Pet.AGI * Talent * ( Loyal + 20 ) / 1500.0f + ( AGI - pPetObj->Item.Pet.AGI * 0.5f * Lv ) * 3;
		break;
	case EM_CultivatePetStructValueType_Int: //INT
		fPower = ( pPetObj->Item.Pet.INT * Lv * ( Talent + Loyal ) / 160 ) + ( INT * Talent / 30 );
		fPower = fPower * ( Loyal + 20 ) / 100 ;
//		fPower = pPetObj->Item.Pet.INT * Talent * ( Loyal + 20 ) / 1500.0f + ( INT - pPetObj->Item.Pet.INT * 0.5f * Lv ) * 3;
		break;
	case EM_CultivatePetStructValueType_Mnd: //MND
		fPower = ( pPetObj->Item.Pet.MND * Lv * ( Talent + Loyal ) / 160 ) + ( MND * Talent / 30 );
		fPower = fPower * ( Loyal + 20 ) / 100 ;
//		fPower = pPetObj->Item.Pet.MND * Talent * ( Loyal + 20 ) / 1500.0f + ( MND - pPetObj->Item.Pet.MND * 0.5f * Lv ) * 3;
		break;
	}

	if( fPower < 0.0f )
		fPower = 0.0f;

	return fPower;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetPetItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int bagIndex = g_pBagFrame->GetUseItemIndex();

	g_pBagFrame->UseItemCancel();

	NetCli_CultivatePet::SL_PushPet(index, bagIndex);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_TakeoutPetItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	// 已經鎖定不處理
	if ( g_pPetFrame->GetTakeoutIndex() != -1 )
	{
		return 0;
	}

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		 
		// 空的
		if ( pet.IsEmpty() )
		{
			return 0;
		}

		// 搜尋空格
		int destPos = CNetGlobal::RoleData()->SearchBodyFirstFit(NULL, false);		// 找尋一個可以插入背包的位置

		// 沒有空間
		if ( destPos == -1 )
		{
			// 沒有空間
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BAG_ENOUGH_CAPACITY"));

			return 0;			
		}

		// 鎖定物品
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(destPos);

		item->Pos = EM_ItemState_CliDisabled;

		// Client鎖定 
		g_pPetFrame->SetTakeoutIndex(index);

		NetCli_CultivatePet::SL_PopPet(index, destPos);

		return 0;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_HasPetItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		lua_pushboolean(L, pet.IsEmpty() == false);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemName(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];		

		if ( !pet.IsEmpty() )
		{
			if( strlen( pet.Name.Begin() ) > 0 )
			{
				lua_pushstring(L, pet.Name.Begin());
				return 1;
			}
			else
			{
				GameObjDbStructEx* petDB = g_ObjectData->GetObj(pet.PetOrgID);
				if ( petDB )
				{
					lua_pushstring(L, petDB->GetName());
					return 1;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetPetItemName(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* newName = luaL_checkstring(L, 2);
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( !pet.IsEmpty() )
		{
			if ( g_pPetFrame->GetUpdateNameIndex() != -1 )
			{
				return 0;
			}

			if ( pet.EventType == EM_CultivatePetCommandType_Summon )
				return 0;

			g_pPetFrame->SetUpdateNameIndex(index);

			NetCli_CultivatePet::SL_PetName(index, newName);
		}
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemExperience(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		lua_pushnumber(L, pet.Exp);									// 目前值
		lua_pushnumber(L, RoleDataEx::PetExpTable[pet.Lv]);			// 最大值
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemLevel(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		
		lua_pushnumber(L, pet.Lv);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemSkillPoint(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		lua_pushnumber(L, pet.SkillPoint);						// 目前值
		lua_pushnumber(L, pet.TotalSkillPoint);					// 最大值
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemProperty(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		lua_pushnumber(L, pet.Property + 2);	// 無(0) 地(1) 水(2) 火(3) 風(4) 光(5) 暗(6)

		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemGrowth(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet		= CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		GameObjDbStructEx*	pPetObj	= g_ObjectData->GetObj( pet.ItemOrgID );

		const char* keyStr = luaL_checkstring(L, 2);

		if ( strcmp(keyStr, "STR") == 0 )
		{
			lua_pushnumber(L, pPetObj->Item.Pet.STR);
			return 1;
		}
		else if ( strcmp(keyStr, "STA") == 0 )
		{
			lua_pushnumber(L, pPetObj->Item.Pet.STA);
			return 1;
		}
		else if ( strcmp(keyStr, "AGI") == 0 )
		{
			lua_pushnumber(L, pPetObj->Item.Pet.AGI);
			return 1;
		}
		else if ( strcmp(keyStr, "INT") == 0 )
		{
			lua_pushnumber(L, pPetObj->Item.Pet.INT);
			return 1;
		}
		else if ( strcmp(keyStr, "MND") == 0 )
		{
			lua_pushnumber(L, pPetObj->Item.Pet.MND);
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemAbility(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		const char* keyStr = luaL_checkstring(L, 2);

		if ( strcmp(keyStr, "STR") == 0 )
		{
			lua_pushnumber(L, pet.STR);
			return 1;
		}
		else if ( strcmp(keyStr, "STA") == 0 )
		{
			lua_pushnumber(L, pet.STA);
			return 1;
		}
		else if ( strcmp(keyStr, "AGI") == 0 )
		{
			lua_pushnumber(L, pet.AGI);
			return 1;
		}
		else if ( strcmp(keyStr, "INT") == 0 )
		{
			lua_pushnumber(L, pet.INT);
			return 1;
		}
		else if ( strcmp(keyStr, "MND") == 0 )
		{
			lua_pushnumber(L, pet.MND);
			return 1;
		}
		else if ( strcmp(keyStr, "LOYAL") == 0 )
		{
			lua_pushnumber(L, pet.Loyal);
			return 1;
		}
		else if ( strcmp(keyStr, "HUNGER") == 0 )
		{
			lua_pushnumber(L, pet.Hunger);
			return 1;
		}
		else if ( strcmp(keyStr, "TALENT") == 0 )
		{
			lua_pushnumber(L, pet.Talent);
			return 1;
		}
		else if ( strcmp(keyStr, "UNDERGO") == 0 )
		{
			lua_pushnumber(L, pet.Spirit);
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemAssist(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		const char* keyStr = luaL_checkstring(L, 2);

		if ( strcmp(keyStr, "STR") == 0 )
		{
			lua_pushnumber(L, pet.AssistPower(EM_CultivatePetStructValueType_Str));
			return 1;
		}
		else if ( strcmp(keyStr, "STA") == 0 )
		{
			lua_pushnumber(L, pet.AssistPower(EM_CultivatePetStructValueType_Sta));
			return 1;
		}
		else if ( strcmp(keyStr, "AGI") == 0 )
		{
			lua_pushnumber(L, pet.AssistPower(EM_CultivatePetStructValueType_Agi));
			return 1;
		}
		else if ( strcmp(keyStr, "INT") == 0 )
		{
			lua_pushnumber(L, pet.AssistPower(EM_CultivatePetStructValueType_Int));
			return 1;
		}
		else if ( strcmp(keyStr, "MND") == 0 )
		{
			lua_pushnumber(L, pet.AssistPower(EM_CultivatePetStructValueType_Mnd));
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemNumSkill(lua_State* L)
{
	int size = 0;
	if ( lua_isnumber(L, 1) )
	{
		PetMagicSkillList* item = g_pPetFrame->GetSkillListInfo((int)lua_tonumber(L, 1) - 1);
		if ( item )
			size = item->Size();		
	}
	
	lua_pushnumber(L, size);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetItemSkillInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = luaL_checkint(L, 2) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		PetMagicSkillList* skillList = g_pPetFrame->GetSkillListInfo(index);
		if ( skillList )
		{
			PetMagicSkillInfo* skillInfo = skillList->GetAtIndex(position);
			if ( skillInfo )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(skillInfo->MagicID);
				if ( itemDB )
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
					if ( imageDB )
					{
						bool bCanLearn = ( g_pPetFrame->CheckSkillLearnLimit(index, position) == ePetLearnSkill_Success );

						lua_pushstring(L, itemDB->GetName());					// 法術名稱
						lua_pushstring(L, imageDB->Image.ACTField);				// Icon
						lua_pushboolean(L, skillInfo->IsLearn);					// 是否學會
						lua_pushboolean(L, skillInfo->IsTopLevel);				// 是否最高級別
						lua_pushnumber(L, skillInfo->PowerLevel + 1);			// 目前技能威力等級

						if( skillInfo->IsLearn )
							skillInfo = g_pPetFrame->GetNextLevelSkill( index, skillInfo->MagicID );

						if ( skillInfo )
						{

						}
						else 
							return 5;


						lua_pushnumber(L, skillInfo->Limit.Point);				// 學習所需技能點數
						lua_pushnumber(L, skillInfo->Limit.Level);				// 學習所需等級
						lua_pushboolean(L, bCanLearn);							// 是否可學

						return 8;
					}				
				}			
			}			
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsPetItemSkillLearn(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = luaL_checkint(L, 2) - 1;

	lua_pushboolean(L, g_pPetFrame->CheckSkillLearnLimit(index, position) == ePetLearnSkill_Success);	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_PetItemLearnSkill(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = luaL_checkint(L, 2) - 1;

	g_pPetFrame->LearnMagicSkill(index, position);
	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickPetCraftItem(lua_State* L)
{
	g_pPetFrame->PasteCraftTool(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetEquipmentItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		int position = g_pPetFrame->GetPetEQTypeENUM(luaL_checkstring(L, 2));

		if ( pet.IsEmpty() == false && position >= 0 && position < EM_CultivatePetEQType_MAX )
		{
			ItemFieldStruct& item = pet.EQ[position];
			if ( item.IsEmpty() == false )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item.OrgObjID);
				if ( itemDB )
				{
					lua_pushstring(L, itemDB->GetName());				// 名稱
					lua_pushstring(L, ItemUTIL::GetItemIcon(itemDB));	// 圖示
					lua_pushnumber(L, item.GetCount());					// 個數
					return 3;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickPetFeedItem(lua_State* L)
{
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	switch ( g_pItemClipboard->GetType() )
	{
	// pickup item
	case eClipboardItem_Type_None:
		g_pPetFrame->FeedItemPickup();
		break;

	//
	case eClipboardItem_Type_Bag:

		
		if( itemInfo.bag.pos != NULL )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos);
			if(item->OrgObjID != NULL )
			{
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->OrgObjID);
				if ( itemDB == NULL || itemDB->Classification != EM_ObjectClass_Item || itemDB->Item.ItemType != EM_ItemType_Pet_Food )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_NOT_PET_FOOD"));
					g_pItemClipboard->Clear();
					return 0;
				}
			}
		}

		g_pItemClipboard->Clear();
		g_pPetFrame->FeedItemPutdown(itemInfo.bag.pos);
		break;

	default:
		break;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClearPetFeedItem(lua_State* L)
{
	g_pPetFrame->SetFeedItemIndex(-1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetFeedFoodItem(lua_State* L)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(g_pPetFrame->GetFeedFoodIndex());
	if ( item && item->IsEmpty() == false )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if ( itemDB != NULL )
		{			
			lua_pushstring(L, itemDB->GetName());
			lua_pushstring(L, ItemUTIL::GetItemIcon(item));
			lua_pushnumber(L, item->GetCount());
			return 3;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 開始生產
int LuaFunc_PetCraftingStart(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = luaL_checkint(L, 2) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		// 檢查該寵物是否在被召喚中
		if( g_fSummonCooldown > 0.0f && g_iSummonIndex == index )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_SUMMON_MODE"));
			return 0;
		}

		// 檢查該寵物是否已被召喚出來了
		if( pet.EventType == EM_CultivatePetCommandType_Summon )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_SUMMON_MODE"));
			return 0;
		}

		// 檢查是否有寵物製造中
		for( int i = 0; i < MAX_CultivatePet_Count; ++i )
		{
			if( g_pPetFrame->IsStartCrafting( i ) )
			{
				if( index == i )	// 自己製造中
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_ALREADY_CREATING"));
				else				// 其他寵物製造中
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_ONLY_ONE_CAN_CREATE"));

				return 0;
			}
		}

		// 檢查製造項目是否正確		
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		if ( position >= 0 && position < craftList.size() )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(craftList[position]);
			if ( lifeTable )
			{
				// 檢查是否有正確製造工具
				if ( lifeTable->Need.ToolID != 0 && pet.Tools.OrgObjID != lifeTable->Need.ToolID )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_CRAFT_NEED_TOOL"));
					return 0;
				}

				// 檢查技能限制是否滿足
				if ( pet.LifeSkill[lifeTable->SkillType] < lifeTable->Need.SkillLv )
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_CRAFT_NOT_ENOUGH_SKILL"));
					return 0;
				}

				NetCli_CultivatePet::SL_PetLifeSkill(index, (CultivatePetLifeSkillTypeENUM)lifeTable->SkillType, lifeTable->Lv, craftList[position]);
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 中斷生產
int LuaFunc_PetCraftingStop(lua_State* L)
{
	g_pPetFrame->SendCraftingStop(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
// 收獲
int LuaFunc_PetCraftHarvest(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		// 檢查是否有物品可以收獲
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.IsEmpty() == false )
		{
			// 檢查是否有物品可以拿取
			if ( pet.Product[0].OrgObjID == 0 && pet.Product[1].OrgObjID == 0 && pet.Product[2].OrgObjID == 0 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_EMPTY_PRODUCT"));
				return 0;
			}

			NetCli_CultivatePet::SL_GetProductRequest(index);
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 是否有成品需要收獲
int LuaFunc_HasPetCraftHarvest(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		// 檢查是否有物品可以收獲
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if ( pet.IsEmpty() == false )
		{
			// 檢查是否有物品可以拿取
			if ( pet.Product[0].OrgObjID == 0 && pet.Product[1].OrgObjID == 0 && pet.Product[2].OrgObjID == 0 )
				lua_pushboolean( L, false );
			else
				lua_pushboolean( L, true );

			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetNumCraftItems(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int size = 0;

	g_pPetFrame->UpdateCreaftInfo(index);

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		size = (int)craftList.size();
	}
	lua_pushnumber(L, size);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetCraftItemInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = luaL_checkint(L, 2) - 1;
	int count;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		if ( position >= 0 && position < craftList.size() )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(craftList[position]);
			if ( lifeTable )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(lifeTable->Product.ItemID);
				if ( itemDB )
				{
					lua_pushstring(L, itemDB->GetName());				// 名稱
					lua_pushstring(L, Func_GetPetSkillKeyString((CultivatePetLifeSkillTypeENUM)lifeTable->SkillType));	// 技能類型
					if( lifeTable->Need.SkillLv == 0 )
					{
						count = 1;
						lua_pushnumber(L, count);			// 技能等級
						return 3;
					}
					lua_pushnumber(L, lifeTable->Need.SkillLv);			// 技能等級
					return 3;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetLifeSkillInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		int skillType = Func_GetPetSkillTypeENUM(luaL_checkstring(L, 2));
		if ( skillType != EM_CultivatePetLifeSkillType_None )
		{
			lua_pushnumber(L, pet.LifeSkill[skillType]);
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsPetCraftingStart(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		lua_pushboolean(L, g_pPetFrame->IsStartCrafting(index));
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 取得製作物品名稱
int LuaFunc_GetPetCraftList(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		for ( vector<int>::iterator iter = craftList.begin(); iter != craftList.end(); iter++ )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(*iter);
			if ( lifeTable )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(lifeTable->Product.ItemID);
				if ( itemDB )
				{
					lua_pushstring(L, itemDB->GetName());
				}
				else
				{
					lua_pushstring(L, "Noname");
				}
			}
		}
		return (int)craftList.size();
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 取得製作物可能獲得的物品種類總數
int LuaFunc_GetPetNumPossibleProductItems	(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int position = lua_isnumber(L, 2) ? (int)lua_tonumber(L, 2) - 1 : -1;
	int size = 0;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		if ( position >= 0 && position < (int)craftList.size() )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(craftList[position]);
			if ( lifeTable )
			{
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(lifeTable->Product.ItemID);		
				if ( itemDB )
				{
					size = itemDB->Treasure.ItemCount;
				}
			}
		}
	}
	lua_pushnumber(L, size);
	return 1;
}

// ----------------------------------------------------------------------------
// 取得製作物可能獲得的某一物品資料
int LuaFunc_GetPetPossibleProductItemDetail(lua_State* L)
{
	int index		= luaL_checkint(L, 1) - 1;
	int position	= lua_isnumber(L, 2) ? (int)lua_tonumber(L, 2) - 1 : -1;
	int itemIndex	= lua_isnumber(L, 3) ? (int)lua_tonumber(L, 3) - 1 : 0;

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		vector<int>& craftList = g_pPetFrame->GetCraftListInfo(index);
		if ( position >= 0 && position < (int)craftList.size() )
		{
			CultivatePetLifeSkillTableStruct* lifeTable = g_ObjectData->GetCultivatePetLifeSkill(craftList[position]);
			if ( lifeTable )
			{
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(lifeTable->Product.ItemID);		
				if ( itemDB )
				{
					GameObjDbStructEx* treasureItemDB = g_ObjectData->GetObj(itemDB->Treasure.Item[itemIndex].OrgID);
					if ( treasureItemDB )
					{
						lua_pushstring(L, treasureItemDB->GetName());					// 掉落物名稱
						lua_pushstring(L, ItemUTIL::GetItemIcon(treasureItemDB));		// 掉落物圖形
						lua_pushnumber(L, itemDB->Treasure.Item[itemIndex].DropCount);	// 掉落物數量
						return 3;
					}
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetTalentSkillNum(lua_State* L)
{
	int index		= luaL_checkint(L, 1) - 1;
	int iCount		= 0;

	if( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct&		pet		= CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		GameObjDbStruct*		pObj	= g_ObjectData->GetObj( pet.PetOrgID );

		if( pObj && pObj->Classification == EM_ObjectClass_NPC )
		{
			for( int i = 0; i < _MAX_NPC_SPELL_MAGIC_COUNT_; ++i )
			{
				if( pObj->NPC.Spell[i].MagicID != 0 )
					iCount++;
			}			
		}
	}

	lua_pushnumber( L, iCount );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetTalentSkillInfo(lua_State* L)
{
	int petIndex		= luaL_checkint(L, 1) - 1;
	int skillIndex		= luaL_checkint(L, 2) - 1;

	if( petIndex >= 0 && petIndex < MAX_CultivatePet_Count && skillIndex < _MAX_NPC_SPELL_MAGIC_COUNT_ )
	{
		CultivatePetStruct&		pet		= CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[petIndex];
		GameObjDbStruct*		pObj	= g_ObjectData->GetObj( pet.PetOrgID );

		if( pObj->Classification == EM_ObjectClass_NPC && pObj->NPC.Spell[skillIndex].MagicID != 0 )
		{
			GameObjDbStructEx* skill = g_ObjectData->GetObj( pObj->NPC.Spell[skillIndex].MagicID );

			if( skill )
			{
				GameObjDbStruct* imageDB = CNetGlobal::GetObj( skill->ImageID );

				if( imageDB )
				{
					lua_pushstring( L, imageDB->Image.ACTField );
					return 1;
				}
			}					
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsPetSummoned(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		RoleDataEx* roleData = CNetGlobal::RoleData();
		CultivatePetStruct&	PetBase	= roleData->PlayerBaseData->Pet.Base[ index ];

		lua_pushboolean(L, PetBase.EventType == EM_CultivatePetCommandType_Summon);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SummonPet(lua_State* L)
{

	if ( lua_isnumber(L, 1) )
	{
		int					index	= (int)lua_tonumber(L, 1) - 1;
		CultivatePetStruct& pet		= CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		CRoleSprite*		pSprite	= g_pGameMain->GetPlayer();

		if( !pSprite )
			return 0;

		if( pet.Lv - pSprite->GetLevel() > PET_SUMMON_LEVEL_LIMIT )
		{
			g_pPetFrame->SendWorldEvent( "PET_SUMMON_LEVEL_ERROR" );
			return 0;
		}

		if( g_pPetFrame->IsStartCrafting( index ) )
		{			
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));
			return 0;
		}

		g_pCastingBarFrame->CastingStart(eCastingBar_PetSummon, g_ObjectData->GetString("PET_SUMMONING"), PET_SUMMON_COST_TIME);

		g_fSummonCooldown = PET_SUMMON_COST_TIME;
		g_iSummonIndex = index;

		// 角色動作與特效
		pSprite->SetActorState(ruFUSION_ACTORSTATE_CHANNEL_BEGIN);
		pSprite->ClearCastingEffect();
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ReturnPet(lua_State* L)
{
	if ( lua_isnumber(L, 1) )
	{
		int index = (int)lua_tonumber(L, 1) - 1;
		NetCli_CultivatePet::SL_PetEvent(index, 0, EM_CultivatePetCommandType_Return);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_FeedPet(lua_State* L)
{
	int index = (int)lua_tonumber(L, 1) - 1;

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		if( g_pPetFrame->IsStartCrafting( index ) )
		{			
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));
			return 0;
		}

		if( pet.EventType == EM_CultivatePetCommandType_Summon )
		{			
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_SUMMON_MODE"));
			return 0;
		}

		g_pPetFrame->FeedFood( index );
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_TravelPet(lua_State* L)
{
	if ( lua_isnumber(L, 1) )
	{
		int index = (int)lua_tonumber(L, 1) - 1;
		NetCli_CultivatePet::SL_PetEvent(index, 0, EM_CultivatePetCommandType_Travel);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ReleasePet(lua_State* L)
{
	if ( lua_isnumber(L, 1) )
	{
		int index = (int)lua_tonumber(L, 1) - 1;

		if ( index >= 0 && index < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

			if( g_pPetFrame->IsStartCrafting( index ) )
			{			
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));
				return 0;
			}

			if( pet.EventType == EM_CultivatePetCommandType_Summon )
			{			
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_SUMMON_MODE"));
				return 0;
			}

			NetCli_CultivatePet::SL_PetEvent(index, 0, EM_CultivatePetCommandType_Free);
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int Model_SetPet(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	int index = luaL_checkint(L, 2) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

		GameObjDbStruct* petDB = g_ObjectData->GetObj(pet.PetOrgID);

		if ( petDB )
		{
			GameObjDbStructEx* imageDB = CNetGlobal::GetObj(petDB->ImageID);
			if ( imageDB == NULL )
				return 0;

			// 設定3D模型圖像
			//model->CreateEntity( ItemUTIL::GetItemACTWorld(petDB) );
			model->CreateEntity( imageDB->Image.ACTWorld );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR     , imageDB->Image.PaperdollHair.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , imageDB->Image.PaperdollFace.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.PaperdollHead.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.PaperdollGloves.Name  );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.PaperdollShoes.Name   );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.PaperdollClothes.Name );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.PaperdollPants.Name   );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.PaperdollBack.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.PaperdollBelt.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.PaperdollShouder.Name );

			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.PaperdollHair.MainColor.Color   , imageDB->Image.PaperdollHair.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.PaperdollFace.MainColor.Color   , imageDB->Image.PaperdollFace.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color  );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color   );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color   );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );

		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SendPetEventSelection(lua_State* L)
{
	int iSelection = luaL_checkint( L, 1 );

	if( NetCli_Other::m_DialogType == EM_LuaDialogType_Pet )
		NetCli_Other::S_DialogSelectID( iSelection );

	return 0;
}
// ----------------------------------------------------------------------------
//檢查是否可以融合
int LuaFunc_CheckPetMixResult(lua_State* L)
{
	int index = luaL_checkint(L, 1);
	int pet1index = luaL_checkint(L, 2) - 1;
	int pet2index = luaL_checkint(L, 3) - 1;
	//int num;
	if( index == 0)
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD6")); //選擇重複
		return 0;
	}
	else if ( index == 1 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD5"));  //請先選擇主體
		return 0;
	}
	else if ( index == 2)
	{
		if ( pet1index >= 0 && index < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet1 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet1index];
			CultivatePetStruct& pet2 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet2index];
			if(	pet1.Property  == pet2.Property || pet2.Property == -1 ) //判斷屬性是否相同 或是其中之一是符文精靈
			//if(	pet1.Property == pet2.Property || pet2.Property == 0) //判斷屬性是否相同 或是其中之一是符文精靈
			{
				if( pet2.Lv <= pet1.Lv)	//成?
				{
					return true;
				}
				else
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD2")); //子體寵物不可高於主體寵物5級
					return false;
				}
			}
			else
			{
				if( pet1.Property == -1 && pet2.Property != -1)
			//	if( pet1.Property == 0 && pet2.Property != 0)
				{
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD9"));  //主寵無屬性時，子寵必須無屬性
					return false;			
				}
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD1")); //屬性不同寵物不可融合
				return false;
			}
		}
	}
	else if ( index == 3 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD7")); //子體選擇錯誤
		return false;
	}

	return false;
}

float CPetFrame::CaculatePetMixModeOneCount( int index , float pet1 , float pet2 )   //{[(En2 - En1)*0.8]+En1}
{
	float del , total;
	if( pet2 > pet1 )
	{
		del = ( pet2 - pet1 ) * 0.45f;
		total = del + pet1;
	}
	else
	{
		del = 0;
		total = pet1;
	}

	if( index == 0 )    //del
	{
		return del;
	}
	else		//total
	{
		return total;
	}
	return 0 ;
}
float CPetFrame::CaculatePetMixModeTwoCount	(int index , float pet1 , float pet2 , float pet3 )   //{[(En2 - En1)*0.8]+En1}
{
	float del , total , num1 , num2;   

	if( pet2 > pet1 )
	{
		num1 = ( pet2 - pet1 ) * 0.45f;
	}
	else
	{
		num1 = 0;
	}

	if( pet3 > pet1 )
	{
		num2 = ( pet3 - pet1 ) * 0.45f;
	}
	else
	{
		num2 = 0;
	}

	del = num1 + num2;
	total = pet1 + num1 + num2;

	if( index == 0 )
	{
		return del;
	}
	else
	{
		return total;
	}
	return 0 ;
}
// ----------------------------------------------------------------------------
// 計算單融模式融合資料
int LuaFunc_GetPetMixModeOneValue(lua_State* L)
{
	int pet1index = luaL_checkint(L, 1) - 1;
	int pet2index = luaL_checkint(L, 2) - 1;
	float total , del;
	int itotal , idel  ;
	const char* keyStr = luaL_checkstring(L, 3);

	if ( pet1index >= 0 && pet1index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet1 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet1index];
		if ( pet2index >= 0 && pet2index < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet2 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet2index];
			if ( strcmp(keyStr, "TALENT") == 0 ) //資質   {[(pet2.Talent - pet1.Talent)*0.25]+pet1.Talent}
			{
				
				if( pet2.Talent > pet1.Talent )
				{
					del = ( pet2.Talent - pet1.Talent ) * 0.25f;
					total = del + pet1.Talent;
				}
				else
				{
					del = 0;
					total = pet1.Talent;
				}

				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}
			else if ( strcmp(keyStr, "UNDERGO") == 0 ) //歷練  pet2.Spirit + pet1.Spirit
			{
				total = (float)(pet2.Spirit + pet1.Spirit);
				lua_pushnumber(L, pet2.Spirit);
				lua_pushnumber(L, total);
				return 2;
			}
			else if ( strcmp(keyStr, "EXP") == 0 ) //經驗  1000 / ( 1 + | pet1.Lv - pet2.Lv|)
			{
				itotal = pet1.Lv - pet2.Lv;
				if( itotal < 0 )
				{
					itotal = 0 - itotal;
				}
				itotal = 1000 / ( 1 + itotal ) ;

				itotal = itotal + pet1.Exp;
				idel = itotal - pet1.Exp;
				lua_pushnumber(L, idel);
				lua_pushnumber(L, itotal);

				if( idel > 0)
				{
					lua_pushnumber(L, 0 );
				}
				else
				{
					lua_pushnumber(L, 1 );
				}
				return 3;
			}	
			else if ( strcmp(keyStr, "STR") == 0 ) //力量 {[(En2 - En1)*0.8]+En1}
			{
				del = g_pPetFrame->CaculatePetMixModeOneCount( 0 , pet1.STR , pet2.STR );  
				total = g_pPetFrame->CaculatePetMixModeOneCount( 1 , pet1.STR , pet2.STR );  
				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}	
			else if ( strcmp(keyStr, "STA") == 0 ) //耐力 {[(En2 - En1)*0.8]+En1}
			{
				del = g_pPetFrame->CaculatePetMixModeOneCount( 0 , pet1.STA , pet2.STA );  
				total = g_pPetFrame->CaculatePetMixModeOneCount( 1 , pet1.STA , pet2.STA );  
				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}	  
			else if ( strcmp(keyStr, "AGI") == 0 ) //敏捷 {[(En2 - En1)*0.8]+En1}
			{
				del = g_pPetFrame->CaculatePetMixModeOneCount( 0 , pet1.AGI , pet2.AGI );  
				total = g_pPetFrame->CaculatePetMixModeOneCount( 1 , pet1.AGI , pet2.AGI );  
				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}	
			else if ( strcmp(keyStr, "INT") == 0 ) //智慧 {[(En2 - En1)*0.8]+En1}
			{
				del = g_pPetFrame->CaculatePetMixModeOneCount( 0 , pet1.INT , pet2.INT );  
				total = g_pPetFrame->CaculatePetMixModeOneCount( 1 , pet1.INT , pet2.INT );  
				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}	
			else if ( strcmp(keyStr, "MND") == 0 )  //精神 {[(En2 - En1)*0.8]+En1}
			{
				del = g_pPetFrame->CaculatePetMixModeOneCount( 0 , pet1.MND , pet2.MND );  
				total = g_pPetFrame->CaculatePetMixModeOneCount( 1 , pet1.MND , pet2.MND );  
				lua_pushnumber(L, del );
				lua_pushnumber(L, total );
				return 2;
			}	
		}
		else   //載入資料錯誤
		{
			return 0;
		}
	}
	else	//載入資料錯誤
	{
		return 0;
	}
	return 0;
}


// ----------------------------------------------------------------------------
// 計算雙融模式融合資料
int LuaFunc_GetPetMixModeTwoValue(lua_State* L)
{
	int pet1index = luaL_checkint(L, 1) - 1;
	int pet2index = luaL_checkint(L, 2) - 1;
	int pet3index = luaL_checkint(L, 3) - 1;

    //float temp;
	float total , del , num1 , num2;   
	int itotal , idel , num3 , num4 ;
	const char* keyStr = luaL_checkstring(L, 4);

	if ( pet1index >= 0 && pet1index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet1 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet1index];
		if( pet2index >= 0 && pet2index < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet2 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet2index];
			if( pet3index >= 0 && pet3index < MAX_CultivatePet_Count )
			{
				CultivatePetStruct& pet3 = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[pet3index];

				if ( strcmp(keyStr, "TALENT") == 0 ) //資質   {[(pet2.Talent - pet1.Talent)*0.25]+pet1.Talent}
				{
					num1 = 0; 
					num2 = 0;
					if( pet2.Talent > pet1.Talent )
					{
						num1 = ( pet2.Talent - pet1.Talent ) * 0.25f;
					}
					else
					{
						num1 = 0;
					}

					if( pet3.Talent > pet1.Talent )
					{
						num2 = ( pet3.Talent - pet1.Talent ) * 0.25f;
					}
					else
					{
						num2 = 0;
					}

					del = num1 + num2;
					total = pet1.Talent + del;

					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;
				}
				else if ( strcmp(keyStr, "UNDERGO") == 0 ) //歷練  pet2.Spirit + pet1.Spirit
				{
					total = (float)(pet1.Spirit + pet2.Spirit + pet3.Spirit);
					del = total - pet1.Spirit;
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;
				}
				else if ( strcmp(keyStr, "EXP") == 0 ) //經驗  1000 / ( 1 + | pet1.Lv - pet2.Lv|)
				{
					num3 = pet1.Lv - pet2.Lv;
					if( num3 < 0 )
					{
						num3 = 0 - num3;
					}
					num3 = 1000 / ( 1 + num3 );

					num4 = pet1.Lv - pet3.Lv;
					if( num4 < 0 )
					{
						num4 = 0 - num4;
					}
					num4 = 1000 / ( 1 + num4 );

					itotal = num3 + num4;
					itotal = itotal + pet1.Exp;
					idel = itotal - pet1.Exp;

					lua_pushnumber(L, idel);
					lua_pushnumber(L, itotal);

					if( itotal > pet1.Lv )
					{
						lua_pushnumber(L, 0);
					}
					else
					{
						lua_pushnumber(L, 1);
					}
					return 3;
				}	
				else if ( strcmp(keyStr, "STR") == 0 ) //力量 {[(En2 - En1)*0.8]+En1}
				{
					del = g_pPetFrame->CaculatePetMixModeTwoCount( 0 , pet1.STR ,  pet2.STR ,  pet3.STR );
					total = g_pPetFrame->CaculatePetMixModeTwoCount( 1 , pet1.STR ,  pet2.STR ,  pet3.STR );
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;
				}	
				else if ( strcmp(keyStr, "STA") == 0 ) //耐力 {[(En2 - En1)*0.8]+En1}
				{
					del = g_pPetFrame->CaculatePetMixModeTwoCount( 0 , pet1.STA ,  pet2.STA ,  pet3.STA );
					total = g_pPetFrame->CaculatePetMixModeTwoCount( 1 , pet1.STA ,  pet2.STA ,  pet3.STA );
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;
				}	  
				else if ( strcmp(keyStr, "AGI") == 0 ) //敏捷 {[(En2 - En1)*0.8]+En1}
				{
					del = g_pPetFrame->CaculatePetMixModeTwoCount( 0 , pet1.AGI ,  pet2.AGI ,  pet3.AGI );
					total = g_pPetFrame->CaculatePetMixModeTwoCount( 1 , pet1.AGI ,  pet2.AGI,  pet3.AGI );
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;
				}	
				else if ( strcmp(keyStr, "INT") == 0 ) //智慧 {[(En2 - En1)*0.8]+En1}
				{
					del = g_pPetFrame->CaculatePetMixModeTwoCount( 0 , pet1.INT ,  pet2.INT ,  pet3.INT );
					total = g_pPetFrame->CaculatePetMixModeTwoCount( 1 , pet1.INT ,  pet2.INT,  pet3.INT );
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;		
				}	
				else if ( strcmp(keyStr, "MND") == 0 )  //精神 {[(En2 - En1)*0.8]+En1}
				{
					del = g_pPetFrame->CaculatePetMixModeTwoCount( 0 , pet1.MND ,  pet2.MND ,  pet3.MND );
					total = g_pPetFrame->CaculatePetMixModeTwoCount( 1 , pet1.MND ,  pet2.MND,  pet3.MND );
					lua_pushnumber(L, del);
					lua_pushnumber(L, total);
					return 2;	
				}	
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
	return 0;
}

// ----------------------------------------------------------------------------
//獲得寵物蛋的造型
int LuaFunc_GetEggIcon(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* texture = "";
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		GameObjDbStructEx* pPetObj = g_ObjectData->GetObj( pet.ItemOrgID );
		GameObjDbStructEx* pPetIconObj = CNetGlobal::GetObj(pPetObj->ImageID);
		
		texture = pPetIconObj->Image.ACTField;
		lua_pushstring(L, texture);  
		return 1;	
	}
	return 0; 
}

// ----------------------------------------------------------------------------
int LuaFunc_SetPetMix(lua_State* L)
{
	int petID1 = luaL_checkint(L, 1) - 1;
	int petID2 = luaL_checkint(L, 2) - 1;
	int petID3 = luaL_checkint(L, 3) - 1;

	g_pPetFrame->m_PetMix2 = petID2;
	g_pPetFrame->m_PetMix3 = petID3;

	if ( ( petID1 >= 0 && petID1 < MAX_CultivatePet_Count && petID2 >= 0 && petID2 < MAX_CultivatePet_Count ) && 
		( (petID3 >= 0 && petID3 < MAX_CultivatePet_Count )|| petID3 == -1 ))
	{
		g_pPetFrame->m_petpremixlevel = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[petID1].Lv;
		NetCli_CultivatePet::SL_PetMarge( petID1 , petID2 , petID3 );
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetMixModeSwitchGood(lua_State* L)
{
	int i ;
	BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
	int itemID = g_ObjectData->GetSysKeyValue( "Pet_TriMergeTicket" );
	for( i = 0 ; i < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_ ; i++ )
	{
		if ( bodyItems.ItemBase[i].OrgObjID == itemID )//204922 )
		{
			return true;
		}
	}
	for( i = 0 ; i < _MAX_BODY_EX_COUNT_ ; i++ )
	{
		if ( bodyItems.ItemEx[i].OrgObjID == itemID ) //204922 )
		{
			return true;
		}
	}

	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_MIX_FAILD8"));
	return false;
}

int LuaFunc_GetPetMixDayMergeCount(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int count;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		count = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index].DayMergeCount;
		
		lua_pushnumber(L, PET_MARGE_COUNT_MAX);
		lua_pushnumber(L, count);
		return 2;
	}
	return 0;
}

int LuaFunc_PetIsSummon(lua_State* L)
{
	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IS_SUMMON"));
	return 0;
}

int LuaFunc_GetNowAbilityAssist(lua_State* L)
{
	
	int index = luaL_checkint(L, 1) - 1;
	int num = luaL_checkint(L, 2);
	float iCount , percent;
	int type;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if( pet.Hunger >= 80 )
		{
			percent = 1;
			type = 1;
		}
		else if( pet.Hunger >= 60 && pet.Hunger < 80 )
		{
			percent = 0.8f;
			type = 2;
		}
		else if( pet.Hunger >= 40 && pet.Hunger < 60 )
		{
			percent = 0.6f; 
			type = 3;
		}
		else if( pet.Hunger >= 20 && pet.Hunger < 40)
		{
			percent = 0.4f;
			type = 4;
		}
		else
		{
			percent = 0.2f;
			type = 5;
		}

		if( num == 1 )
		{
			iCount = pet.AssistPower(EM_CultivatePetStructValueType_Str);
		}
		else if ( num == 2 )
		{
			iCount = pet.AssistPower(EM_CultivatePetStructValueType_Agi);
		}
		else if ( num == 3 )
		{
			iCount = pet.AssistPower(EM_CultivatePetStructValueType_Sta);
		}
		else if ( num == 4 )
		{
			iCount = pet.AssistPower(EM_CultivatePetStructValueType_Int);
		}
		else if ( num == 5 )
		{
			iCount = pet.AssistPower(EM_CultivatePetStructValueType_Mnd);
		}
		iCount = iCount * percent;

		lua_pushnumber(L, type);
		lua_pushnumber(L, iCount);
		return 2;
	}

	return 0;
}

int LuaFunc_GetNextMergeResetTime(lua_State* L)
{	
	int ResetTime = CNetGlobal::RoleData()->PlayerBaseData->Pet.NextMergeResetTime;
	
	lua_pushnumber(L, ResetTime);
	return 1;
}

int LuaFunc_IsPetStarUse(lua_State* L)
{	
	int Pos = luaL_checkint(L, 1) - 1;

	switch( Pos )
	{
	case 0:
	case 1:
		return true;
	case 2:
		if( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_PetPage3] < 0 )
			return false;
		else return true;			
	case 3:
		if( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_PetPage4] < 0 )
			return false;
		else return true;
	case 4:
		if( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_PetPage5] < 0 )
			return false;
		else return true;
	case 5:
		if( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_PetPage6] < 0 )
			return false;
		else return true;

	default:
		return false;
	}
	return false;
}
int LuaFunc_IsPetMixEventErr(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if( pet.EventType == EM_CultivatePetCommandType_Summon )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_LEARN_ERROR_SUMMON"));		
			return false;
		}
		return true;
	}

	return false;
}

int LuaFunc_IsPetCraftProduct(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		if( pet.EventType == EM_CultivatePetCommandType_LiftSkill )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PET_ALREADY_CREATING"));		
			return false;
		}
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPetFeedItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) ;
	int count = 0 , i;
	const char* texture = "";
	if( index == -1 )
	{
		BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
		for( i = 0 ; i < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_ ; i++ )
		{
			ItemFieldStruct  &iItemBase = bodyItems.ItemBase[i];
			if(iItemBase.OrgObjID != 0 )
			{
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(iItemBase.OrgObjID);
				if ( itemDB != NULL && itemDB->Item.ItemType == EM_ItemType_Pet_Food )
				{
					if( count <= 9)
					{
						g_pPetFrame->m_petfood[count].ID = iItemBase.OrgObjID;
						g_pPetFrame->m_petfood[count].count = iItemBase.Count;
						g_pPetFrame->m_petfood[count].pos = i;
						count++ ;
					}
				}
			}
		}
		lua_pushnumber(L, count);
		return 1;
	}
	else
	{
		if( g_pPetFrame->m_petfood[index - 1].ID ) 
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(g_pPetFrame->m_petfood[index - 1].ID);
			if ( itemDB != NULL )
			{			
				GameObjDbStructEx* itemDB2 = CNetGlobal::GetObj(itemDB->ImageID);
				texture = itemDB2->Image.ACTField;
			
				lua_pushstring(L, texture);
				lua_pushnumber(L, g_pPetFrame->m_petfood[index - 1].count);
				return 2;
			}
		}
	}
	return 0 ;
}
int LuaFunc_SetPetFeedItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* texture = "";

	BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;

	g_pPetFrame->SetFeedItemIndex( -1 );
	g_pPetFrame->FeedItemPutdown(g_pPetFrame->m_petfood[index].pos + 60);
	
	ItemFieldStruct  &Item = bodyItems.ItemBase[g_pPetFrame->m_petfood[index].pos];

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(g_pPetFrame->m_petfood[index].ID);
	if ( itemDB != NULL )
	{			
		GameObjDbStructEx* itemDB2 = CNetGlobal::GetObj(itemDB->ImageID);
		texture = itemDB2->Image.ACTField;
	
		lua_pushstring(L, texture);
		lua_pushnumber(L, g_pPetFrame->m_petfood[index].count);
		return 2;
	}

	return 0 ;
}
bool CPetFrame::IsPetStateBusy(int index1, int index2, int index3)
{
	if ( index1 >= 0 && index1 < MAX_CultivatePet_Count )
	{
		if( m_isStartCrafting[index1] )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));	
			return false;
		}
	}

	if ( index2 >= 0 && index2 < MAX_CultivatePet_Count )
	{
		if( m_isStartCrafting[index2] )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));	
			return false;
		}
	}

	if ( index3 >= 0 && index3 < MAX_CultivatePet_Count )
	{
		if( m_isStartCrafting[index3] )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("PET_IN_CREATING_STATE"));	
			return false;
		}
	}
	return true;
}

int LuaFunc_IsPetStateTrue(lua_State* L)
{
	int index1 = luaL_checkint(L, 1) ;
	int index2 = luaL_checkint(L, 2) ;
	int index3 = luaL_checkint(L, 3) ;
	if( g_pPetFrame->IsPetStateBusy( index1 , index2 , index3) )
	{
		return true;
	}
	return false;
}

void CPetFrame::ResetMergeTime()
{
	int itime = CNetGlobal::RoleData()->PlayerBaseData->Pet.NextMergeResetTime;
	int jtime = (int)g_pGameMain->GetGameTime();
	if( itime - jtime <= 0 )
	{
		int i ;
		for( i = 0 ; i < MAX_CultivatePet_Count ; i++ )
		{
			CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[i].DayMergeCount = 0;
		}
		g_pInterface->SendWorldEvent("REST_MERGE_TIME");
	}	
}
int LuaFunc_ResetPetMergeTime(lua_State* L)
{
	g_pPetFrame->ResetMergeTime();
	return 0;
}