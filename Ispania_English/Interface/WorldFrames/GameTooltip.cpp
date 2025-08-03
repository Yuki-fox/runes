#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../../netwaker_member/net_mail/NetCli_MailChild.h"
#include "../../netwaker_member/net_trade/NetCli_TradeChild.h"
#include "../../netwaker_member/net_other/NetCli_OtherChild.h"
#include "../../mainproc/GameMain.h"
#include "RoleData/ObjectStructEx.h"
#include "../skillbook/UI_SkillBook.h"
#include "../Quest/UI_QuestDetail.h"

#include "../CraftFrame/UI_CraftFrame.h"
#include "../FusionStoneTradeFrame/FusionStoneTradeFrame.h"
#include "../Houses/HousesFrame.h"
#include "BagFrame.h"

#include "..\transportBook\teleportBook.h"
#include "CharacterFrame.h"
#include "InspectFrame.h"
#include "MailFrame.h"
#include "ItemUTIL.h"
#include "TradeFrame.h"
#include "ActionFrame.h"
#include "StoreFrame.h"
#include "AuctionFrame.h"
#include "GameTooltip.h"
#include "MacroFrame.h"
#include "EmoteFrame.h"
#include "MerchantFrame.h"
#include "MerchantFrame.h"
#include "PetFrame.h"
#include "../Map/MapCommon.h"
#include "../RecipeStore/UI_RecipeStore.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "SuitSkillFrame.h"
#include "../../tools/Common/Com_Function.h"
#include "PartnerFrame.h"
#include "RuneExchange.h"
#include "AncillaryTitleFrame.h"



struct AbilityStatistics
{
	void	Clear()
	{
		m_data.clear();
	}

	void	Insert(WearEqTypeENUM type, float value, float scale=1.0f)
	{
		if ( type != EM_WearEqType_None )
		{
			value *= scale;

			map<WearEqTypeENUM, float>::iterator iter = m_data.find(type);
			if ( iter != m_data.end() )
			{
				iter->second += value;
			}
			else
			{
				m_data.insert(make_pair(type, value));
			}
		}
	}

	void	Insert(GameObjectWearEqStruct& ability, float scale=1.0f)
	{
		for ( int i = 0; i < Max_WearEqType_Count; i++ )
		{
			Insert(ability.EqType[i], (float)ability.EqTypeValue[i], scale);
		}
	}

	void	Insert(AbilityStatistics& src)
	{
		for ( map<WearEqTypeENUM, float>::iterator iter = src.m_data.begin(); iter != src.m_data.end(); iter++ )
		{
			Insert(iter->first, iter->second);
		}
	}

	float		Pop(WearEqTypeENUM type)
	{
		float value = 0.0f;
		map<WearEqTypeENUM, float>::iterator iter = m_data.find(type);
		if ( iter != m_data.end() )
		{
			value = iter->second;
			m_data.erase(iter);
		}
		return value;
	}

	bool	Pop(WearEqTypeENUM &type, float &value)
	{
		map<WearEqTypeENUM, float>::iterator iter = m_data.begin();
		if ( iter != m_data.end() )
		{
			type = iter->first;
			value = iter->second;
			m_data.erase(iter);
			return true;
		}
		return false;
	}

	map<WearEqTypeENUM, float> m_data;
};

CGameTooltip*				g_pGameTooltip		= NULL;
CUiColor					g_redColor			= CUiColor( 1.0f, 0.0f, 0.0f, 1.0f );
CUiColor					g_greenColor		= CUiColor( 0.0f, 1.0f, 0.0f, 1.0f );
CUiColor					g_blueColor			= CUiColor( 0.0f, 0.0f, 1.0f, 1.0f );
CUiColor					g_whiteColor		= CUiColor( 1.0f, 1.0f, 1.0f, 1.0f );

//§Þ¯à¤É¯ÅTooltips¨Ï¥Î
int g_GameTooltip_SkillPoint = -1;
float g_tooltipDotRate = 1.0f;
//Åã¥Ü¦ì²ß±o§Þ¯à¥Î Åã¥Ü®§±oµ¥¯Å
//bool g_GameTooltip_Skill_Unlearn = false;
//int g_GameTooltip_Skill_LearnLV = 0;
bool g_GameTooltip_SkillBookMode = false;
CUI_SkillBook::SpellInfo g_GameTooltip_Skill_Info;
float g_gameTooltipBuffTime = 0.0f;
bool g_GameTooltip_IsSetTitle = false; //¦bºÙ¸¹¤¶­± ºÙ¸¹¦WºÙ­nÅã¥Ü???



union {
	int _value;
	struct {
		bool				HideCost			:1;	// ÁôÂÃ»Ý¨D
		bool				HideJob				:1;	// ÁôÂÃÂ¾·~
		bool				HideRank			:1;	// ÁôÂÃ Rank
		bool				HideDistance		:1;	// ÁôÂÃ¶ZÂ÷
		bool				HideCastingTime		:1;	// ÁôÂÃ¬Iªk®É¶¡
		bool				NotRoleSpellTime	:1;	// ¤£¨ú¥Îª±®a¬Iªk®É¶¡
		bool				HideColdown			:1;	// ÁôÂÃ§NÓý®É¶¡
		bool				NotRoleColdown		:1;	// ¤£¨ú¥Îª±®a§NÓý®É¶¡
		bool				HideNote			:1;	// ÁôÂÃ»¡©ú
		bool				HideNeed			:1;	// ÁôÂÃ»Ý¨D
		bool				HideUseLevel		:1;	// ÁôÂÃ¨Ï¥Îµ¥¯Å
	};
}g_GameTooltipMagicColSetting;



static	bool	_TooltipSetItem					(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB, bool showLimit=true);
static	void	TooltipAddPetSkill				(CUiTooltip* tooltip, GameObjDbStructEx* itemDB, bool bNextLevelGray);
static	void	TooltipAddPetSkillName			(CUiTooltip* tooltip, GameObjDbStructEx* itemDB);
static	void	TooltipAddLimitText				(CUiTooltip* tooltip, ItemFieldStruct* item, LimitStruct& limit, bool compareSelf);
static	void	TooltipAddWearEqTypeText		(CUiTooltip* tooltip, WearEqTypeENUM type, float value, bool inborn=false, const char* name=NULL, int abilityCount=1, int forceRarity=0);
static	void	TooltipAddAbilityText			(CUiTooltip* tooltip, GameObjectAttributeStruct& ability, const char* name=NULL, float scale=1.0f);
static	void	TooltipAddRuneAbilityText		(CUiTooltip* tooltip, ItemFieldStruct* item, float scale=1.0f);
static	void	TooltipAddSoulBoundText			(CUiTooltip* tooltip, ItemFieldStruct* item);
static	void	TooltipAddSuitAbilityText		(CUiTooltip* tooltip, GameObjDbStructEx* itemDB);
static	void	TooltipAddSuitSkillText			(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB);
static  bool	TooltipSetAttackSkill			(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB);
static	void	TooltipAddDropAbility			(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB);
// ----------------------------------------------------------------------------------


void		GetMagicSpellCostStr( MagicSpellCostTypeENUM Type , int Value , int SkillPoint , char* OutputStr )
{
	RoleDataEx* role = CNetGlobal::RoleData();
	switch( Type )
	{
	case EM_MagicSpellCostType_None			:		//µL®ø¯Ó
		strcpy(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_NONE"));
		return;
	case EM_MagicSpellCostType_HP			:		//®ø¯Ó HP
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_HP"), Value);
		return;
	case EM_MagicSpellCostType_MP			:		//®ø¯Ó MP
		{
			float TempValue = (float)Value;
			if( RoleDataEx::MPCostPowerRate.size() > (unsigned)SkillPoint )
				TempValue = TempValue * RoleDataEx::MPCostPowerRate[SkillPoint] ;
			TempValue = ( TempValue * ( 100 - role->TempData.Attr.ManaDecrease() )/100 ); 
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_MP"), int(TempValue) );
		}
		return ;
	case EM_MagicSpellCostType_HP_Per		:		//®ø¯Ó HP¦Ê¤À¤ñ
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_HP_PER"), Value);
		return;
	case EM_MagicSpellCostType_MP_Per		:		//®ø¯Ó MP¦Ê¤À¤ñ
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_MP_PER"), Value);
		return;
	case EM_MagicSpellCostType_SP_Warrior	:		//®ø¯Ó SP ¾Ô¤h(-1ªí¥Ü¥þ³¡)
		if( Value == -1 )
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_FURY_ALL"));
		else
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_FURY"), int( Value * ( 100 - role->TempData.Attr.SPWarriorDecrease() )/100 )  );
		break;

	case EM_MagicSpellCostType_SP_Ranger	:		//®ø¯Ó SP ¹C«L(-1ªí¥Ü¥þ³¡)
		if( Value == -1 )
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_CONCENTRATION_ALL"));
		else
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_CONCENTRATION"), int( Value * ( 100 - role->TempData.Attr.SPRangerDecrease() )/100 ) );
		break;

	case EM_MagicSpellCostType_SP_Rogue		:		//®ø¯Ó SP µs¸é(-1ªí¥Ü¥þ³¡)
		if( Value == -1 )
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_VIGOR_ALL"));
		else
			sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_VIGOR"), int( Value * ( 100 - role->TempData.Attr.SPRogueDecrease() )/100 ) );
		break;

	case EM_MagicSpellCostType_StomachPoint	:		//®ø¯Ó ¹¡­¹«×
		break;

	case EM_MagicSpellCostType_Item			:		//®ø¯Ó ª««~
		{
			GameObjDbStructEx* ItemDB = CNetGlobal::GetObj( Value );
			if( ItemDB != NULL )
				strcpy(OutputStr, ItemDB->GetName());
		}
		break;

	case EM_MagicSpellCostType_Ammo_Gun		:		//®ø¯Ó ¤l¼u
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_AMMO"), Value);
		break;

	case EM_MagicSpellCostType_Ammo_Bow		:		//®ø¯Ó ¤}½b¥Ú
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_ARROW"), Value);
		break;

	case EM_MagicSpellCostType_Ammo_CossBow	:		//®ø¯Ó ¤Q¦r¤}½b¥Ú
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_ARROW"), Value);
		break;

	case EM_MagicSpellCostType_Ammo_Throw	:		//®ø¯Ó §ëÂYª«
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_THROW"), Value);
		break;

	case EM_MagicSpellCostType_Ammo_All		:		//®ø¯Ó ©Ò¦³ªº»·µ{¼uÃÄ
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_AMMO_ALL"), Value);
		break;

	case EM_MagicSpellCostType_SoulPoint:
		sprintf(OutputStr, g_ObjectData->GetString("SYS_TOOLTIPS_BURN_SOUL_POINT"), Value);
		break;
	}
}




//·h¥h ItemUTIL GetItemNote 
/*
const string GetItemNote( GameObjDbStruct* ItemDB )
{
	//char	tempStr[128];
	//char	findKey[32];
	const char* findKey = "(Magic)";
	//ÃöÁä¦r (Magic)
	string Str = CNetCli_Script_Child::ReplaceSystemKeyword(ItemDB->ShortNote);
	if( ItemDB == NULL || Str.length() == 0 )
		return "";

	//////////////////////////////////////////////////////////////////////////
	if( ItemDB->Item.UseType == EM_ItemUseType_Magic )
	{		
		if( Str.find( findKey ) != string::npos )
		{
			GameObjDbStructEx* MagicCol = g_ObjectData->GetObj( ItemDB->Item.IncMagic_Onuse );
			if( MagicCol != NULL )
			{
				ReplaceString( Str , findKey , GetMagicColNote( MagicCol , 0 ).c_str() );
			}
		}
	}
*/
	/*
	if( MagicBase->MagicBase.MagicFunc == EM_MAGICFUN_Assist )
	{
	for( int j = 0 ; j < 5 ; j++ )
	{
	sprintf( findKey , "(Buff%d)", j );
	if( MagicBase->MagicBase.Ability.EqTypeValue[j] != 0 && Str.find( findKey ) != string::npos )
	{
	float Power = MagicBase->MagicBase.Ability.EqTypeValue[j] * ( 100 + MagicBase->MagicBase.Ability_SkillLVArg * MagicLv ) / 100;
	if( Power < 0 )
	Power *= -1;
	sprintf( tempStr , "%d" , int( Power)  );
	ReplaceString( Str , findKey , tempStr );
	}
	}

	//Å]ªk¬Þ
	sprintf( findKey , "(Buff-Shield)" );
	if( MagicBase->MagicBase.MagicShield.Point != 0 && Str.find( findKey ) != string::npos )
	{
	float Power = MagicBase->MagicBase.MagicShield.Point * ( 100 + MagicBase->MagicBase.MagicShield.SkillLvArg * MagicLv ) / 100;

	sprintf( tempStr , "%d" , int( Power)  );
	ReplaceString( Str , findKey , tempStr );

	}

	//Dot
	sprintf( findKey , "(Buff-Dot)"  );
	if( MagicBase->MagicBase.DotMagic.Base  != 0 && Str.find( findKey ) != string::npos )
	{
	float Power = MagicBase->MagicBase.DotMagic.Base * ( 100 + MagicBase->MagicBase.DotMagic.DotSkillLVArg * MagicLv ) / 100;
	if( Power < 0 )
	Power *= -1;
	sprintf( tempStr , "%d" , int( Power)  );
	ReplaceString( Str , findKey , tempStr );

	}

	//Time
	sprintf( findKey , "(Buff-Time)" );
	if( MagicBase->MagicBase.EffectTime != 0 && Str.find( findKey ) != string::npos )
	{
	float Power = MagicBase->MagicBase.EffectTime * ( 100 + MagicBase->MagicBase.EffectTime_SkillLVArg * MagicLv ) / 100;
	if( Power < 0 )
	Power *= -1;
	sprintf( tempStr , "%d" , int( Power)  );
	ReplaceString( Str , findKey , tempStr );

	}

	}
	*/
/*
	return Str;
}*/

const string GetMagicBaseNote( GameObjDbStruct* MagicBase , int MagicLv )
{
	char	tempStr[128];
	char	findKey[32];

	//ÃöÁä¦r ??DMG ¶Ë®`¤O 
	string Str = CNetCli_Script_Child::ReplaceSystemKeyword(MagicBase->ShortNote);
	if( MagicBase == NULL || Str.length() == 0 )
		return "";

	//////////////////////////////////////////////////////////////////////////
	if( MagicBase->MagicBase.MagicFunc == EM_MAGICFUN_Assist )
	{
		for( int j = 0 ; j < 5 ; j++ )
		{
			sprintf( findKey , "(Buff%d)", j );
			if( MagicBase->MagicBase.Ability.EqTypeValue[j] != 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.Ability.EqTypeValue[j] * ( 100 + MagicBase->MagicBase.Ability_SkillLVArg * MagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;


				sprintf( tempStr , "%.1f" , Power  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );
			}
		}

		//Å]ªk¬Þ
		sprintf( findKey , "(Buff-Shield)" );
		if( MagicBase->MagicBase.MagicShield.Point != 0 && Str.find( findKey ) != string::npos )
		{
			float Power = MagicBase->MagicBase.MagicShield.Point * ( 100 + MagicBase->MagicBase.MagicShield.SkillLvArg * MagicLv ) / 100;

			sprintf( tempStr , "%.1f" , Power  );
			ItemUTIL::ReplaceString( Str , findKey , tempStr );

		}

		//Dot
		sprintf( findKey , "(Buff-Dot)"  );
		if( MagicBase->MagicBase.DotMagic.Base  != 0 && Str.find( findKey ) != string::npos )
		{
			float Power = MagicBase->MagicBase.DotMagic.Base * ( 100 + MagicBase->MagicBase.DotMagic.DotSkillLVArg * MagicLv ) / 100;
			if( Power < 0 )
				Power *= -1 * g_tooltipDotRate;
			sprintf( tempStr , "%.1f" , Power  );
			ItemUTIL::ReplaceString( Str , findKey , tempStr );

		}

		//Time
		sprintf( findKey , "(Buff-Time)" );
		if( MagicBase->MagicBase.EffectTime != 0 && Str.find( findKey ) != string::npos )
		{
			float Power = MagicBase->MagicBase.EffectTime * ( 100 + MagicBase->MagicBase.EffectTime_SkillLVArg * MagicLv ) / 100;
			if( Power < 0 )
				Power *= -1;
			sprintf( tempStr , "%.1f" , Power  );
			ItemUTIL::ReplaceString( Str , findKey , tempStr );

		}
		//////////////////////////////////////////////////////////////////////////

	}
	return ItemUTIL::GetMagicNoteEx( Str , MagicLv );
	//return Str;
}

// ----------------------------------------------------------------------------------
CGameTooltip::CGameTooltip(CInterface* object)
: CInterfaceBase(object)
{
	m_index			= -1;
	memset(m_unit, 0, sizeof(m_unit));
	m_SysTooltip[0]=NULL;
	m_SysTooltip[1]=NULL;
	g_pGameTooltip = this;

	m_gameTooltip = NULL;
}

// ----------------------------------------------------------------------------------
CGameTooltip::~CGameTooltip()
{
	g_pGameTooltip = NULL;	
	m_gameTooltip = NULL;
}
//------------------------------------------------------------------------------
int LuaFunc_GameTooltip_SetSysTips(lua_State* L);
// ----------------------------------------------------------------------------------
void CGameTooltip::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GameTooltip_SetSysTips",		LuaFunc_GameTooltip_SetSysTips);
	}
}

// ----------------------------------------------------------------------------------
void GameTooltip_OnHide (CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	g_pGameTooltip->Clear();
	g_pGameTooltip->SetNeedUpdateTime(false);
}

// ----------------------------------------------------------------------------------
void CGameTooltip::Load()
{
	m_gameTooltip = (CUiTooltip*)g_pInterface->GetUiWorld()->FindObject("GameTooltip");
	if ( m_gameTooltip )
	{
		m_gameTooltip->m_pfOnHide = GameTooltip_OnHide;
	}
}

// ----------------------------------------------------------------------------------
void CGameTooltip::DestroyFrames()
{
	m_gameTooltip = NULL;
}

// ----------------------------------------------------------------------------------
//void CGameTooltip::Update(float elapsedTime)
//{
//	/*
//	static float lasttime = 0.0f;
//
//	lasttime += elapsedTime;
//	if ( lasttime > 0.5f )
//	{
//		lasttime = 0.0f;
//		switch (m_type)
//		{
//		case eGameTooltip_Equipment:
//		case eGameTooltip_Bag:
//			{
//				CUiTooltip* tooltip = (CUiTooltip*)m_interface->GetUiWorld()->FindObject("GameTooltip");
//				if ( tooltip && tooltip->IsVisible() )
//					SetTooltip(m_type, tooltip, m_index, m_unit);
//			}
//			break;
//		}
//	}
//	*/
//}


bool CGameTooltip::TooltipSetItem( CUiTooltip* tooltip, bool equip, ItemFieldStruct* item, GameObjDbStructEx* itemDB, bool compareSelf )
{
	int ret= _TooltipSetItem(tooltip,item,itemDB,compareSelf);	

	if (!equip)
	{

		if (/*item&&*/itemDB)
		{
			if (itemDB->Classification==EM_ObjectClass_Item)
			{
				RoleDataEx* role = CNetGlobal::RoleData();

				ItemFieldStruct* item1=0,*item2=0;// = role->GetEqItem(Slot);

				switch(itemDB->Item.Class) 
				{

				case EM_Item_Weapon:
					{

						switch(itemDB->Item.WeaponPos)
						{
						case EM_WeaponPos_TwoHand  ://       ,   //¨â¤â
						case EM_WeaponPos_MainHand  ://   ,   //¥D¤â

							item1=role->GetEqItem(EM_EQWearPos_MainHand);
							break;

						case EM_WeaponPos_SecondHand ://     ,   //°Æ¤â
							item1=role->GetEqItem(EM_EQWearPos_SecondHand);
							break;
						case EM_WeaponPos_OneHand  ://       ,   //Âù¤â³£¥i¥H¸Ë³Æ							
							item1=role->GetEqItem(EM_EQWearPos_MainHand);
							if ( item1 == NULL || item1->IsEmpty() )
								item1=role->GetEqItem(EM_EQWearPos_SecondHand);
							else
								item2=role->GetEqItem(EM_EQWearPos_SecondHand);
							break;					

						case EM_WeaponPos_Ammo      ://      ,   //¤l¼u

							item1=role->GetEqItem(EM_EQWearPos_Ammo);
							break;
						case EM_WeaponPos_Bow       ://      ,   //¤}

							item1=role->GetEqItem(EM_EQWearPos_Bow);
							break;
						case EM_WeaponPos_Manufacture://     ,   //¥Í²£¤u¨ã
							item1=role->GetEqItem(EM_EQWearPos_Manufactory);
							break;
						}
					}
					break;

				case EM_Item_Armor:
					{
						switch(itemDB->Item.ArmorPos)
						{
						case EM_ArmorPos_Head      ://  ,   //ÀY
							item1=role->GetEqItem(EM_EQWearPos_Head);
							break;
						case EM_ArmorPos_Gloves	    :// ,   //¤â®M
							item1=role->GetEqItem(EM_EQWearPos_Gloves);
							break;
						case EM_ArmorPos_Shoes	   ://   ,   //¾c¤l
							item1=role->GetEqItem(EM_EQWearPos_Shoes);
							break;
						case EM_ArmorPos_Clothes	 ://     ,   //¤W¦ç
							item1=role->GetEqItem(EM_EQWearPos_Clothes);
							break;
						case EM_ArmorPos_Pants	  ://    ,   //¿Ç¤l
							item1=role->GetEqItem(EM_EQWearPos_Pants);
							break;
						case EM_ArmorPos_Back	  ://    ,   //­I³¡
							item1=role->GetEqItem(EM_EQWearPos_Back);
							break;
						case EM_ArmorPos_Belt	  ://    ,   //¸y±a
							item1=role->GetEqItem(EM_EQWearPos_Belt);
							break;
						case EM_ArmorPos_Shoulder   ://   ,   //ªÓ¥Ò
							item1=role->GetEqItem(EM_EQWearPos_Shoulder);
							break;
						case EM_ArmorPos_Necklace    ://  ,   //¶µÁå 
							item1=role->GetEqItem(EM_EQWearPos_Necklace);
							break;
						case EM_ArmorPos_Ring       ://  ,   //§Ù¤l
							item1=role->GetEqItem(EM_EQWearPos_Ring1);
							if ( item1 == NULL || item1->IsEmpty() )
								item1=role->GetEqItem(EM_EQWearPos_Ring2);
							else
								item2=role->GetEqItem(EM_EQWearPos_Ring2);							
							break;
						case EM_ArmorPos_Earring     ://  ,   //¦Õ¹¢
							item1=role->GetEqItem(EM_EQWearPos_Earring1);
							if ( item1 == NULL || item1->IsEmpty() )
								item1=role->GetEqItem(EM_EQWearPos_Earring2);
							else
								item2=role->GetEqItem(EM_EQWearPos_Earring2);							
							break;
						case EM_ArmorPos_SecondHand  ://  ,   //°Æ¤â
							item1=role->GetEqItem(EM_EQWearPos_SecondHand);
							break;
						case EM_ArmorPos_MagicTool	 :// ,	//ªkÄ_
							item1=role->GetEqItem(EM_EQWearPos_MagicTool1);
							break;
						case EM_ArmorPos_Ornament	 :// ,	//¸Ë¹¢«~
							item1=role->GetEqItem(EM_EQWearPos_Ornament);
							break;
						}
					}
				}

				if (item1 ||item2)
				{
					bool showTip1 = false;
					bool showTip2 = false;
					const char *pcharStr = g_ObjectData->GetString("NOW_EQUIP_MSG");

					if (item1&&!item1->IsEmpty())
					{
						showTip1 = true;
						m_SysTooltip[0]->ClearLines();
						m_SysTooltip[0]->AddLine(pcharStr, 0.9f, 0.9f, 0.4f);
						_TooltipSetItem(m_SysTooltip[0],item1,CNetGlobal::GetObj(item1->OrgObjID), compareSelf);
					}

					if (item2&&!item2->IsEmpty())
					{
						showTip2 = true;
						m_SysTooltip[1]->ClearLines();
						m_SysTooltip[1]->AddLine(pcharStr, 0.9f, 0.9f, 0.4f);
						_TooltipSetItem(m_SysTooltip[1],item2,CNetGlobal::GetObj(item2->OrgObjID), compareSelf);
					}
					m_SysTooltip[0]->SetVisible(showTip1);
					m_SysTooltip[1]->SetVisible(showTip2);
				}
			}
		}
	}
	return (ret) ? true : false;
}

// ----------------------------------------------------------------------------------
void CGameTooltip::SetTooltip(GameTooltipType type, CUiTooltip* tooltip, int index, const char* unit)
{
	m_type		= type;
	m_index		= index;
	strcpy(m_unit, unit);

	g_gameTooltipBuffTime = 0.0f;

	if ( tooltip )
	{
		bool isVisible = false;
		ItemFieldStruct* item;
		ActionItem2* actionItem;

		tooltip->ClearLines();
		if (m_SysTooltip[0])
			m_SysTooltip[0]->SetVisible(0);

		if (m_SysTooltip[1])
			m_SysTooltip[1]->SetVisible(0);

		switch ( type )
		{
		case eGameTooltip_Bag:
			item = CNetGlobal::RoleData()->GetBodyItem(index);
			if ( item && !item->IsEmpty() )
				isVisible = TooltipSetItem( tooltip, false, item, CNetGlobal::GetObj(item->OrgObjID));
			break;

		case eGameTooltip_Bank:
			item = CNetGlobal::RoleData()->GetBankItem(index);
			if ( item && !item->IsEmpty() )
				isVisible = TooltipSetItem( tooltip, false, item, CNetGlobal::GetObj(item->OrgObjID));
			break;

		case eGameTooltip_Equipment:
			item = CNetGlobal::RoleData()->GetEqItem(index);
			if ( item && !item->IsEmpty() )
				isVisible = TooltipSetItem( tooltip, true, item, CNetGlobal::GetObj(item->OrgObjID));
			break;

		case eGameTooltip_Skill:
			{
				GameObjDbStructEx* itemDB = CNetGlobal::GetObj(m_index);
				if( NULL == itemDB )
				{
					return ;
				}

				isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );

   				//int skillPoint = (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID));
				int skillPoint = g_GameTooltip_SkillPoint < 0 ? (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID)) : g_GameTooltip_SkillPoint ;
				if( !itemDB->IsKeyItem() && skillPoint >= 0 && itemDB->MagicCol.MaxSkillLv > skillPoint && 
					( !g_GameTooltip_SkillBookMode || g_GameTooltip_Skill_Info.bLearned ) ) 
				{
					tooltip->AddLine( g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_NEXT_POWER") , 0.5f , 0.5f , 0.5f );
					tooltip->AddLine( ItemUTIL::GetMagicColNote( itemDB , skillPoint + 1 ).c_str(), 0.5f , 0.5f , 0.5f );
				}
			}

			break;

		case eGameTooltip_QuestItem:
			{
				GameObjDbStructEx* pObj = CNetGlobal::GetObj( index );
				if ( pObj )
					isVisible = TooltipSetItem( tooltip, true, NULL, pObj );
			} break;

		case eGameTooltip_Action:
			actionItem = g_pActionFrame->GetItem(m_index);
			if ( actionItem )
			{
				MacroItem* macroItem;
				switch (actionItem->type )
				{
				case eActionItemType_Bag:
				case eActionItemType_Equipment:
					{
						GameObjDbStructEx* itemDB = g_ObjectData->GetObj(actionItem->objectID);
						ItemFieldStruct* itemField = NULL;

						if ( itemDB )
						{
							// «D°ïÅ|ª««~,¨Ã¥B¦³ª««~°ß¤@½s¸¹
							if ( itemDB->MaxHeap <= 1 && actionItem->createTime != 0 )
							{
								int position;

								// ·j´M¸Ë³Æ
								if ( (position = g_pCharacterFrame->SearchEquip(actionItem->createTime, actionItem->serial)) > 0 ) {
									itemField = CNetGlobal::RoleData()->GetEqItem(position);
								} else if ( (position = g_pBagFrame->SearchItem(actionItem->createTime, actionItem->serial)) > 0 ) {
									itemField = CNetGlobal::RoleData()->GetBodyItem(position);
								}
							}
						}
						isVisible = TooltipSetItem( tooltip, false, itemField, itemDB);
					}
					break;

				case eActionItemType_SuitSkill:
				case eActionItemType_SkillPlate:
				case eActionItemType_Skill:
					//if (actionItem->skillID == DF_SKILLID_ATTACK)
					//	isVisible = TooltipSetAttackSkill(m_tooltip, NULL, CNetGlobal::GetObj(actionItem->skillID));
					//else
					isVisible = TooltipSetItem( tooltip, true, NULL, CNetGlobal::GetObj(actionItem->skillID));
					break;
				case eActionItemType_Macro:
					macroItem = g_pMacroFrame->GetMacroItemById(actionItem->macroID);
					if (macroItem)
					{
						tooltip->AddLine(macroItem->name, 1.0f, 1.0f, 1.0f);
						isVisible = true;
					}
					break;
				case eActionItemType_Emote:
					{
						SEmoteItem* emoteItem = g_pEmoteFrame->GetEmoteItemByID( actionItem->emoteID );
						if ( emoteItem )
						{
							char tt[64];
							sprintf( tt , "/%s" , emoteItem->pToken );
							tooltip->AddLine( emoteItem->pName  , 1.0f, 1.0f, 1.0f);
							tooltip->AddLine( tt , 1.0f, 1.0f, 1.0f);
							isVisible = true;
						}
					}
					break;
				case eActionItemType_HappyPartner:
					{
						GameObjDbStructEx* itemDB =g_pPartnerFrame->GetItem( actionItem->PartnerMode, actionItem->PartnerPos);
						if( NULL == itemDB )
						{
							return ;
						}

						isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
					}
					break;
					

				case eActionItemType_Teleport:
					{
						LocationSaveStruct* item =g_pTeleportBook-> GetItem( actionItem->teleportID );
						if ( item )
						{
					
							char ItemName[128];
							CUI_TeleportBook::GetItemName(item,ItemName);	
						
							tooltip->AddLine( ItemName  , 1.0f, 1.0f, 1.0f);
							tooltip->AddLine( item->Note , 1.0f, 1.0f, 1.0f);
							isVisible = true;
						}
					}
					break;
				}
			}
			break;

		case eGameTooltip_Store:
			if ( strcmp(m_unit, "SELL") == 0 )
			{
				GameObjDbStructEx* goods = CNetGlobal::GetObj( g_pStoreFrame->GetItemId(m_index) );
				isVisible = TooltipSetItem( tooltip, false, NULL, goods);
				if ( isVisible && tooltip )
				{
					if ( goods && goods->MaxHeap > 1 )
					{
						tooltip->AddLine(g_ObjectData->GetString("SYS_SHOP_SHIFT_TIP"), 1.0f, 1.0f, 0.0f);
					}
				}
			}
			else if ( strcmp(m_unit, "BUYBACK") == 0 )
			{
				SellItemFieldStruct* sellItem = CNetGlobal::RoleData()->GetSellItem(m_index);
				if ( sellItem )
					isVisible = TooltipSetItem( tooltip, false, &sellItem->Item, CNetGlobal::GetObj(sellItem->Item.OrgObjID));
			}
			break;

		case eGameTooltip_Inbox:
			{
				NetInboxInfo* header = NetCli_MailChild::GetInboxInfo(m_index);
				if (header && !header->IsItemEmpty())
				{
					int itemPos = atoi(unit);
					if ( itemPos >= 0 && itemPos < 10 )
					{
						isVisible = TooltipSetItem( tooltip, true, header->itemData + itemPos, CNetGlobal::GetObj(header->itemData[itemPos].OrgObjID));
					}				
				}
			}
			break;

		case eGameTooltip_SendMailItem:
			{
				ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(g_pMailFrame->GetSendMailItem(index));
				if (item && !item->IsEmpty())
					isVisible = TooltipSetItem( tooltip, true, item, CNetGlobal::GetObj(item->OrgObjID));
			}
			break;

		case eGameTooltip_TradePlayer:
			{
				TradeItem* tradeItem = g_pTradeFrame->GetItem(m_index);
				if ( tradeItem )
				{
					switch ( tradeItem->type )
					{
					case eTradeItemType_None:
						break;

					case eTradeItemType_Bag:
						item = CNetGlobal::RoleData()->GetBodyItem(tradeItem->pos);
						if ( item && !item->IsEmpty() )
							isVisible = TooltipSetItem( tooltip, false, item, CNetGlobal::GetObj(item->OrgObjID));
						break;

					case eTradeItemType_Bank:
						break;

					case eTradeItemType_Equipment:
						break;
					}
				}
			}
			break;

		case eGameTooltip_TradeTarget:
			item = NetCli_TradeChild::GetTradeItem(m_index);
			if ( item && !item->IsEmpty() )
				isVisible = TooltipSetItem( tooltip, false, item, CNetGlobal::GetObj(item->OrgObjID));
			break;

		case eGameTooltip_Auction:
			item = CNetGlobal::RoleData()->GetBodyItem(g_pAuctionFrame->GetAuctionItemIndex());
			if ( item && !item->IsEmpty() )
				isVisible = TooltipSetItem( tooltip, false, item, CNetGlobal::GetObj(item->OrgObjID));
			break;

		case eGameTooltip_AuctionBrowse:
			{
				AuctionBrowseItemInfo* info = NetCli_ACChild::GetBrowseItem(m_index);
				if ( info && !info->data.Item.IsEmpty() )
					isVisible = TooltipSetItem( tooltip, false, &(info->data.Item), CNetGlobal::GetObj(info->data.Item.OrgObjID));
			}			
			break;

		case eGameTooltip_AuctionBid:
			{
				AuctionBuyItemInfo* info = NetCli_ACChild::GetBuyItem(m_index);
				if ( info && !info->data.Item.IsEmpty() )
					isVisible = TooltipSetItem( tooltip, false, &(info->data.Item), CNetGlobal::GetObj(info->data.Item.OrgObjID));
			}
			break;

		case eGameTooltip_AuctionSell:
			{
				AuctionSellItemInfo* info = NetCli_ACChild::GetSellItem(m_index);
				if ( info && !info->data.Item.IsEmpty() )
					isVisible = TooltipSetItem( tooltip, false, &(info->data.Item), CNetGlobal::GetObj(info->data.Item.OrgObjID));
			}
			break;

		case eGameTooltip_UnitBuff:
		case eGameTooltip_UnitDebuff:
			{
				CRoleSprite* sprite = g_pGameMain->GetUnitSprite(m_unit);
				if ( sprite )
				{					
					int buffIndex = -1;
					if ( type == eGameTooltip_UnitBuff )
						buffIndex = sprite->GetBuffHelpfulIndex(m_index);
					else if ( type == eGameTooltip_UnitDebuff )
						buffIndex = sprite->GetBuffHarmfulIndex(m_index);

					if ( buffIndex != -1 )
					{
						RoleBuffInfo* buff = sprite->GetBuffInfo(buffIndex);
						if ( buff ) {
							GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);
							if ( itemDB ) {
								g_tooltipDotRate = buff->dotRate;
								g_GameTooltip_SkillPoint = buff->magicLv;
								if ( itemDB->MagicBase.Setting.ShowBuffTime )
									g_gameTooltipBuffTime = buff->time;
								else
									g_gameTooltipBuffTime = -1.0f;
								isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
								g_tooltipDotRate = 1.0f;
								g_GameTooltip_SkillPoint = -1;								
							}
						}
					}
				}
			}
			break;

		case eGameTooltip_Merchant2:
			{


				DepartmentStoreBaseInfoStruct *item=g_pMerchantFrame->GetItemInfo(-1,m_index);	
				

				if ( item )
				{
					
					isVisible = TooltipSetItem( tooltip, false, &item->Item, CNetGlobal::GetObj(item->Item.OrgObjID));
				}
			}
			break;


		case eGameTooltip_Merchant:
			{

				vector<DepartmentStoreBaseInfoStruct*>&tableList=	g_pMerchantFrame->GetNowItemList();

				if ( m_index >= 0 && m_index < (int)tableList.size() )
				{
					DepartmentStoreBaseInfoStruct& item = *tableList[m_index];
					isVisible = TooltipSetItem( tooltip, false, NULL, CNetGlobal::GetObj(item.Item.OrgObjID));
				}
			}
			break;

		case eGameTooltip_ShoppingCart:
		/*	{
				vector<ShoppingCartItem>& shoppingCart = g_pMerchantFrame->GetShoppingCart();
				if ( m_index >= 0 && m_index < (int)shoppingCart.size() )
				{
					isVisible = TooltipSetItem( tooltip, false, NULL, CNetGlobal::GetObj(shoppingCart[m_index].objectID));
				}
			}*/
			break;

		case eGameTooltip_LootItem:
			{
				PartyRollItem* rollItem = NetCli_PartyChild::GetRollItem(m_index);
				if ( rollItem )
				{
					isVisible = TooltipSetItem( tooltip, false, &(rollItem->item), CNetGlobal::GetObj(rollItem->item.OrgObjID));
				}
			}
			break;

		case eGameTooltip_RecipeStore:
			{
				int  RecipeID = g_pRecipeStore->GetRecipeID(index);
				if ( RecipeID!=-1 )
				{
				//	GameObjDbStruct*	pRecipe	= g_ObjectData->GetObj( RecipeID );

					isVisible = TooltipSetItem( tooltip, false, NULL, CNetGlobal::GetObj(RecipeID));
				}
			}
			break;
		case eGameTooltip_GameBar:
			{
				GameObjDbStructEx* itemDB = CNetGlobal::GetObj(index);
				if( NULL == itemDB )
				{
					return ;
				}

				isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
			}
			break;
		case eGameTooltip_HappyPartner:
			{
				GameObjDbStructEx* itemDB = CNetGlobal::GetObj(index);
				if( NULL == itemDB )
				{
					return ;
				}

				isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
			}
			break;
			

		case eGameTooltip_SuitSkill:
			{
				GameObjDbStructEx* itemDB = CNetGlobal::GetObj(index);
				if( NULL == itemDB )
				{
					return ;
				}

				isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
			}
			break;

		case eGameTooltip_RuneExChange:
			{
				GameObjDbStructEx* itemDB = CNetGlobal::GetObj(index);
				if( NULL == itemDB )
				{
					return ;
				}

				isVisible = TooltipSetItem( tooltip, true, NULL, itemDB );
			}
			break;
		case eGameTooltip_ATFEffect:
			GameObjDbStructEx* TitleObjDB = CNetGlobal::GetObj(index);
			if( TitleObjDB )
			{
					
			}
			break;
		}

		tooltip->SetVisible(isVisible);
	}
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetGameBarItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) ;
	g_pGameTooltip->SetTooltip(eGameTooltip_GameBar, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetBagItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Bag, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetPartnerItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) ;
	g_pGameTooltip->SetTooltip(eGameTooltip_HappyPartner, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetBankItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Bank, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetEquipmentItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Equipment, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetInventoryItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));

	bool visable=true;
	tooltip->ClearLines();
	if ( lua_isstring(L, 2) && lua_isnumber(L, 3) )
	{
		ItemFieldStruct* item = NULL;
		const char* unit = lua_tostring(L, 2);
		int index = (int)lua_tonumber(L, 3);
		bool compareSelf = false;

		if ( strcmp(unit, "player") == 0 )
		{
			compareSelf = true;
			visable=true;
			item = CNetGlobal::RoleData()->GetEqItem(index);
		}
		else if ( strcmp(unit, g_pInspectFrame->GetInspectUnit()) == 0 )
		{
			item = NetCli_OtherChild::GetInspectItemFieldStruct(index);
			visable=false;
		}

		if ( item && !item->IsEmpty() )
		{
			bool Visible = g_pGameTooltip->TooltipSetItem( tooltip, visable,  item, CNetGlobal::GetObj(item->OrgObjID), compareSelf );
			tooltip->SetVisible( Visible );
		}
		else
		{
			char buf[256];
			sprintf(buf, "SYS_EQWEARPOS_%02d", index);
			tooltip->SetText(g_ObjectData->GetString(buf), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetSkillItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int skillType = luaL_checkint(L, 2) - 1;
	int skillID	  = luaL_checkint(L, 3) - 1;

	g_GameTooltip_SkillBookMode = true;
	CUI_SkillBook::SpellInfo* pInfo = g_pUi_SkillBook->GetSkillInfo( skillType, skillID );
	if( NULL == pInfo )
	{
		return 0;
	}

	g_GameTooltip_Skill_Info = *pInfo;
	int iGUID = g_GameTooltip_Skill_Info.iGUID;
	if( iGUID == 0 )
	{
		return 0;
	}

	g_GameTooltip_SkillPoint = pInfo->iPowerLV;
	   
	g_pGameTooltip->SetTooltip(eGameTooltip_Skill, tooltip, iGUID, "");

	g_GameTooltip_SkillPoint = -1;

   	g_GameTooltip_SkillBookMode = false;
	g_GameTooltip_Skill_Info.bLearned = false;
    g_GameTooltip_Skill_Info.LearnLV = -1;

	return 0;	
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetSkillItem_LevelUp(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int skillType	= luaL_checkint(L, 2) - 1;
	int skillID		= luaL_checkint(L, 3) - 1;
	int SkillPoint  = luaL_checkint(L, 4);
	int index		= g_pUi_SkillBook->GetSkillGUID(skillType, skillID);

	g_GameTooltip_SkillPoint = SkillPoint;
	g_pGameTooltip->SetTooltip(eGameTooltip_Skill, tooltip, index, "");
	g_GameTooltip_SkillPoint = -1;

	return 0;	
}


// ----------------------------------------------------------------------------------
int	Tooltip_SetQuestItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int Type		= luaL_checkint(L, 2);
	int ID			= luaL_checkint(L, 3);

	//int index		= g_pUi_SkillBook->GetSkillGUID(skillType, skillID);

	if( Type >= 1 && Type <= 3 )
	{
		if( ID >= 1 && ID <= (int)(g_pQuestDetail->m_vecQuestItem[ ( Type - 1 ) ].size()) )
		{
			CUI_QuestDetail::StructQuestItem&	item		= g_pQuestDetail->m_vecQuestItem[ ( Type - 1 ) ][ ( ID - 1 ) ];			
			g_pGameTooltip->SetTooltip(eGameTooltip_QuestItem, tooltip, item.iItemID, "");
		}

	}	

	return 0;	
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetCraftRequestItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	/*
	int	iCraftType		= luaL_checkint(L, 2);
	int	iCraftSubType		= luaL_checkint(L, 3);
	int	iCraftItemIndex		= luaL_checkint(L, 4);
	int	iCraftRequestItemIndex	= luaL_checkint(L, 5);

	int iItemGUID = g_pCraftFrame->GetCraftRequestItemGUID( iCraftType, iCraftSubType, iCraftItemIndex, iCraftRequestItemIndex );
	*/

	int iRecpiteGUID			= luaL_checkint(L, 2);
	int	iCraftRequestItemIndex	= luaL_checkint(L, 3);
	int iItemGUID				= g_pCraftFrame->GetCraftRequestItemGUID( iRecpiteGUID, iCraftRequestItemIndex );

	if( iItemGUID != 0 )
	{
		g_pGameTooltip->SetTooltip(eGameTooltip_QuestItem, tooltip, iItemGUID, "");
	}
	return 0;	
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetCraftItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));

	/*
	int	iCraftType		= luaL_checkint(L, 2);
	int	iCraftSubType		= luaL_checkint(L, 3);
	int	iCraftItemIndex		= luaL_checkint(L, 4);

	int iItemGUID = g_pCraftFrame->GetCraftItemGUID( iCraftType, iCraftSubType, iCraftItemIndex );
	g_pGameTooltip->SetTooltip(eGameTooltip_QuestItem, tooltip, iItemGUID, "");
	*/

	int iRecpiteGUID			= luaL_checkint(L, 2);
	int iItemGUID				= g_pCraftFrame->GetCraftItemGUID( iRecpiteGUID );
	int iButtonID				= luaL_checkint(L, 3);

	g_pCraftFrame->SetCraftItemField( true, iItemGUID, iButtonID );

	if( iItemGUID != 0 )
	{
		// ²Ä¤G¶µ¥Ø¤~·|§ïÅÜ±¼¸¨ª««~
		if ( iButtonID > 1 )
		{
			int id = g_ObjectData->GetLifeSkillRareExID(iItemGUID, iButtonID - 1);
			if (id != 0)
				iItemGUID = id;
		}
		g_pGameTooltip->SetTooltip(eGameTooltip_QuestItem, tooltip, iItemGUID, "");
	}

	g_pCraftFrame->SetCraftItemField( false );

	return 0;	
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetCraftItemRuneHole(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));

	int					iRecpiteGUID	= luaL_checkint(L, 2);
	bool				bCheck			= false;	

	bCheck = g_pCraftFrame->CheckRecipte( iRecpiteGUID );		// ÀË¬d¤w¾Ç·|ªº²M³æ¤¤¦³µL¸Óª««~

	if( bCheck == true )
	{
		char tmp[512];

		g_pCraftFrame->GetRuneHoleTooltip( tmp, iRecpiteGUID );

		if( strcmp( tmp, "" ) == 0 )
			return 0;

		tooltip->AddLine( g_ObjectData->GetString( "CRAFT_DASHES" ), 1.0f, 1.0f, 1.0f );
		tooltip->AddLine( tmp, 0.78125f, 0.01953125f, 0.96875f );
	}

	return 0;	
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetCraftQueueRequestItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));

	int		iItemGUID		= luaL_checkint(L, 2);
	bool	bCheck			= false;

	bCheck = g_pCraftFrame->CheckRequestItem( iItemGUID );

	if( bCheck == true )
	{
		g_pGameTooltip->SetTooltip(eGameTooltip_QuestItem, tooltip, iItemGUID, "");
	}
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetActionItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Action, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetPetActionItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	PetActionItem* petItem = g_pActionFrame->GetPetSkill(luaL_checkint(L, 2) - 1);
	if ( petItem )
	{
		tooltip->ClearLines();
		switch (petItem->skillID)
		{
		case EM_PET_STOP:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_STOP"), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
			break;

		case EM_PET_FOLLOW:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_FOLLOW"), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
			break;

		case EM_PET_ATTACK:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_ATTACK"), 1.0f, 1.0f, 1.0f);			
			tooltip->SetVisible(true);
			break;

		case EM_PET_ATTACKOFF:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_ATTACKOFF"), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
			break;

		case EM_PET_STRICKBACK:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_STRIKBACK"), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
			break;

			/*
		case EM_PET_GUARD:
			tooltip->SetText(g_ObjectData->GetString("SYS_PET_COMMAND_GUARD"), 1.0f, 1.0f, 1.0f);
			tooltip->SetVisible(true);
			break;
		*/

		default:
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(petItem->skillID);
				if ( itemDB ) {
					bool visible = g_pGameTooltip->TooltipSetItem(tooltip, false, NULL, itemDB);
					tooltip->SetVisible( visible );
				}
			}
			break;
		}
	}
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetStoreItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	const char* state = luaL_checkstring(L, 2);
	int index = luaL_checkint(L, 3) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Store, tooltip, index, state);
	return 0;
}
// ----------------------------------------------------------------------------------
int	Tooltip_SetInboxItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	int itemPos = (lua_isnumber(L, 3)) ? (int)lua_tonumber(L, 3) - 1 : 0;

	char temp[32];
	sprintf_s(temp, 32, "%d", itemPos);
	g_pGameTooltip->SetTooltip(eGameTooltip_Inbox, tooltip, index, temp);
	return 0;
}

// ----------------------------------------------------------------------------------
int	Tooltip_SetSendMailItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = (lua_isnumber(L, 2)) ? (int)lua_tonumber(L, 2) - 1 : 0;
	g_pGameTooltip->SetTooltip(eGameTooltip_SendMailItem, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetTradeTargetItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_TradeTarget, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetTradePlayerItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_TradePlayer, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAuctionItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));	
	g_pGameTooltip->SetTooltip(eGameTooltip_Auction, tooltip, 0, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAuctionBrowseItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2);
	g_pGameTooltip->SetTooltip(eGameTooltip_AuctionBrowse, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAuctionBidItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_AuctionBid, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAuctionSellItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_AuctionSell, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetUnitBuff(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	const char* unit = luaL_checkstring(L, 2);
	int index = luaL_checkint(L, 3) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_UnitBuff, tooltip, index, unit);
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetUnitDebuff(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	const char* unit = luaL_checkstring(L, 2);
	int index = luaL_checkint(L, 3) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_UnitDebuff, tooltip, index, unit);
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetMerchantItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Merchant, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetItemMall(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int guid = luaL_checkint(L, 2) ;
	g_pGameTooltip->SetTooltip(eGameTooltip_Merchant2, tooltip, guid, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetShoppingCartItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_ShoppingCart, tooltip, index, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetLootItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int serial = luaL_checkint(L, 2);
	g_pGameTooltip->SetTooltip(eGameTooltip_LootItem, tooltip, serial, "");
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAssignItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	PartyAssignItem* assignItem = NetCli_PartyChild::GetAssignItem(luaL_checkint(L, 2));	
	if ( assignItem )
	{
		ItemFieldStruct* item = &assignItem->item;
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
		if ( itemDB ) {
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
			tooltip->SetVisible( visible );
		}
	}	
	return  0;
}

// ----------------------------------------------------------------------------------
//ºÙ¸¹»¡©ú
// ----------------------------------------------------------------------------------
int	Tooltip_SetTitle(lua_State* L)
{
 	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
  	tooltip->ClearLines();

	int nTitleID = luaL_checkint(L, 2);

	GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( nTitleID );
 
	if( NULL == pTitleObj || !pTitleObj->IsTitle() )
		return 0;

	g_GameTooltip_IsSetTitle = true;
	bool isVisible = g_pGameTooltip->TooltipSetItem( tooltip, true, NULL, pTitleObj );
	g_GameTooltip_IsSetTitle = false;

	tooltip->SetVisible(isVisible);

	return 0;	
}

// ----------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------
int Tooltip_SetHouseItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	tooltip->ClearLines();
	if ( !lua_isnumber(L, 2) || !lua_isnumber(L, 3) )
	{
		//assert( false );
		return 0;
	}

	ItemFieldStruct* item = NULL;
	int ContainerID = (int)lua_tonumber(L, 2);
	int index = (int)lua_tonumber(L, 3) - 1;
	bool compareSelf = true;

	HouseItemDBStruct* HouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , index );

	if( !HouseItemDB )
	{
		return 0;
	}

	item = &HouseItemDB->Item;

	if ( item && !item->IsEmpty() )
	{
		bool Visible = g_pGameTooltip->TooltipSetItem( tooltip, false,  item, CNetGlobal::GetObj(item->OrgObjID), compareSelf );
		tooltip->SetVisible( Visible );
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetBulletinBoardItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int item = luaL_checkint(L, 2) - 1;
	int index = luaL_checkint(L, 3) - 1;
	vector<ImportBoardBaseStruct>& data = NetCli_ImportBoardChild::s_importBoardList;
	if ( item >= 0 && item < (int)data.size() )
	{
		if ( index >= 0 && index < 5 )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(data[item].Item[index]);
			if ( itemDB ) {
				bool visible = g_pGameTooltip->TooltipSetItem(tooltip, false, NULL, itemDB);
				tooltip->SetVisible( visible );
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetHyperLink(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable( L, 1 ) );
	
	if( lua_isnil( L , 2 ) )
	{
		return 0;
	}


	const char* link = lua_tostring( L, 2 );
	g_pGameTooltip->SetHyperLink( tooltip, link );

	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetHistoryItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	AuctionHistoryItemInfo* info = NetCli_ACChild::GetHistoryItem(luaL_checkint(L, 2) - 1);
	if ( info )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(info->item.OrgObjID);
		if ( itemDB ) {
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, false, &(info->item), itemDB);
			tooltip->SetVisible( visible );
		}
	}
	return  0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAccountBagItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	NetAccountBagItem* info = NetCli_ItemChild::GetAccountBagItemInfo(luaL_checkint(L, 2) - 1);
	if ( info )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(info->item.OrgObjID);
		if ( itemDB ) {
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, false, &(info->item), itemDB);
			tooltip->SetVisible( visible );
		}
	}
	return  0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetBootyItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	ItemFieldStruct* item = NetCli_Treasure::GetBootyItem(luaL_checkint(L, 2) - 1);
	if ( item )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
		if ( itemDB ) {
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
			tooltip->SetVisible( visible );
		}
	}	
	return  0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetPetSkillItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	int position = luaL_checkint(L, 3) - 1;

	tooltip->ClearLines();

	if ( index >= 0 && index < MAX_CultivatePet_Count )
	{
		CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
		PetMagicSkillList* petSkill = g_pPetFrame->GetSkillListInfo(index);

		if ( petSkill )
		{
			PetMagicSkillInfo* skillInfo = petSkill->GetAtIndex(position);
			if ( skillInfo )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(skillInfo->MagicID);

				g_GameTooltip_SkillPoint = skillInfo->PowerLevel;

				TooltipAddPetSkillName(tooltip, itemDB);

				bool bIsLearn = skillInfo->IsLearn;

				// ¾Ç²ß­­¨î
				{
					char tmpL[512];
					char tmpR[512];
					vector<string> limitStr;

					if( bIsLearn )
					{
						TooltipAddPetSkill( tooltip, itemDB, false );

						skillInfo = g_pPetFrame->GetNextLevelSkill( index, skillInfo->MagicID );						

						if ( skillInfo )
						{
							tooltip->AddLine( g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_NEXT_POWER"), 0.5f, 0.5f, 0.5f );
							g_GameTooltip_SkillPoint = skillInfo->PowerLevel;
						}
					}

					if ( skillInfo )
					{
						strcpy(tmpL, g_ObjectData->GetString("SYS_LEARN_LIMIT"));

						// ­«­nª««~
						if ( skillInfo->Limit.KeyItem != 0 )
						{
							GameObjDbStructEx* keyItemDB = g_ObjectData->GetObj(skillInfo->Limit.KeyItem);
							if ( CNetGlobal::RoleData()->CheckKeyItem( skillInfo->Limit.KeyItem ) )
								sprintf_s(tmpR, " %s", keyItemDB->GetName());
							else
								sprintf_s(tmpR, " |cffff0000%s|r", keyItemDB->GetName());
							limitStr.push_back(tmpR);
						}

						// µ¥¯Å
						if ( skillInfo->Limit.Level > 1 )
						{
							if ( skillInfo->Limit.Level <= pet.Lv )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_LEVEL"), skillInfo->Limit.Level);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_LEVEL"), skillInfo->Limit.Level);
							limitStr.push_back(tmpR);
						}

						// §Þ¯àÂI¼Æ
						if ( skillInfo->Limit.Point > 0 )
						{
							if ( skillInfo->Limit.Point <= pet.SkillPoint )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_SKILL_POINT"), skillInfo->Limit.Point);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_SKILL_POINT"), skillInfo->Limit.Point);
							limitStr.push_back(tmpR);
						}

						// STR
						if ( skillInfo->Limit.Str > 0 )
						{
							if ( skillInfo->Limit.Str <= pet.STR )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_ATTRIBUTE_01"), skillInfo->Limit.Str);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_ATTRIBUTE_01"), skillInfo->Limit.Str);
							limitStr.push_back(tmpR);
						}

						// STA
						if ( skillInfo->Limit.Sta > 0 )
						{
							if ( skillInfo->Limit.Sta <= pet.STA )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_ATTRIBUTE_02"), skillInfo->Limit.Sta);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_ATTRIBUTE_02"), skillInfo->Limit.Sta);
							limitStr.push_back(tmpR);
						}

						// INT
						if ( skillInfo->Limit.Int > 0 )
						{
							if ( skillInfo->Limit.Int <= pet.INT )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_ATTRIBUTE_03"), skillInfo->Limit.Int);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_ATTRIBUTE_03"), skillInfo->Limit.Int);
							limitStr.push_back(tmpR);
						}

						// MND
						if ( skillInfo->Limit.Mnd > 0 )
						{
							if ( skillInfo->Limit.Mnd <= pet.MND )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_ATTRIBUTE_04"), skillInfo->Limit.Mnd);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_ATTRIBUTE_04"), skillInfo->Limit.Mnd);
							limitStr.push_back(tmpR);
						}

						// AGI
						if ( skillInfo->Limit.Agi > 0 )
						{
							if ( skillInfo->Limit.Agi <= pet.AGI )
								sprintf_s(tmpR, 512, " %s %d", g_ObjectData->GetString("SYS_ATTRIBUTE_05"), skillInfo->Limit.Agi);
							else
								sprintf_s(tmpR, 512, " |cffff0000%s %d|r", g_ObjectData->GetString("SYS_ATTRIBUTE_05"), skillInfo->Limit.Agi);
							limitStr.push_back(tmpR);
						}					

					int count = 0;
					for ( vector<string>::iterator iter = limitStr.begin(); iter != limitStr.end(); iter++, count++ )
					{
						if ( count > 0 )
							strcat(tmpL, ",");
						strcat(tmpL, iter->c_str());
					}

					if( bIsLearn )
						tooltip->AddLine(tmpL, 0.5f, 0.5f, 0.5f);
					else
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
					}
				}

				if ( skillInfo )
				{
					TooltipAddPetSkill(tooltip, itemDB, bIsLearn);
				}

				tooltip->SetVisible(true);

				g_GameTooltip_SkillPoint = -1;
			}
		}

	}	
	return  0;
}

// ----------------------------------------------------------------------------
int Tooltip_SetPetTalentSkillItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int petIndex		= luaL_checkint(L, 2) - 1;
	int skillIndex		= luaL_checkint(L, 3) - 1;

	if( petIndex >= 0 && petIndex < MAX_CultivatePet_Count && skillIndex < _MAX_NPC_SPELL_MAGIC_COUNT_ )
	{
		CultivatePetStruct&		pet		= CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[petIndex];
		GameObjDbStruct*		pObj	= g_ObjectData->GetObj( pet.PetOrgID );

		if( pObj->Classification == EM_ObjectClass_NPC && pObj->NPC.Spell[skillIndex].MagicID != 0 )
		{
			g_GameTooltip_SkillPoint = pObj->NPC.Spell[skillIndex].MagicLv;

			tooltip->ClearLines();
			bool Visible = g_pGameTooltip->TooltipSetItem( tooltip, true, NULL, CNetGlobal::GetObj( pObj->NPC.Spell[skillIndex].MagicID ) );
			tooltip->SetVisible( Visible );

			g_GameTooltip_SkillPoint = -1;
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetAbility(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int DBID = luaL_checkint(L, 2);

	tooltip->ClearLines();

	GameObjDbStructEx* inherent = CNetGlobal::GetObj( DBID );
	if ( NULL == inherent || inherent->Classification != EM_ObjectClass_Attribute )
	{
		return 0;
	}

	tooltip->AddLine( inherent->GetName(), 1.0f , 1.0f , 1.0f );
	
	// ¯à¤O
	TooltipAddAbilityText(tooltip, inherent->Attirbute, "");
	tooltip->SetVisible(true);

	return  0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetPetFeedFoodItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(g_pPetFrame->GetFeedFoodIndex());
	if ( item && item->IsEmpty() == false )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
		if ( itemDB )
		{
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
			tooltip->SetVisible( visible );
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetPetFeedFoodItemTmp(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index = luaL_checkint(L, 2) - 1;
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(g_pPetFrame->GetFeedFoodTmpIndex(index));
	if ( item && item->IsEmpty() == false )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
		if ( itemDB )
		{
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
			tooltip->SetVisible( visible );
		}
	}

	return 0;
}




// ----------------------------------------------------------------------------------
// arg1 Ãdª«¯Á¤Þ­Ó¼Æ
// arg2 Ãdª«
int Tooltip_SetPetEquipmentItem	(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index = luaL_checkint(L, 2) - 1;
	if ( index >=0 && index < MAX_CultivatePet_Count )
	{
		int position = CPetFrame::GetPetEQTypeENUM(luaL_checkstring(L, 3));
		if ( position < EM_CultivatePetEQType_MAX )
		{
			ItemFieldStruct* item = &(CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index].EQ[position]);
			if ( item && item->IsEmpty() == false )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
				if ( itemDB )
				{
					bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
					tooltip->SetVisible( visible );
				}
			}
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetPetProductItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index		= luaL_checkint(L, 2) - 1;
	int position	= lua_isnumber(L, 3) ? (int)lua_tonumber(L, 3) - 1 : -1;
	int itemIndex	= lua_isnumber(L, 4) ? (int)lua_tonumber(L, 4) - 1 : 0;

	GameObjDbStructEx* itemDB = g_pPetFrame->GetCraftProductItem(index, position, itemIndex);
	if ( itemDB )
	{
		bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, NULL, itemDB);
		tooltip->SetVisible( visible );
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetImplementActionItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index = luaL_checkint(L, 2) - 1;
	if ( index == 9 )
	{
		tooltip->SetText(g_ObjectData->GetString("SYS_EXIT_IMPLEMENT"), 1, 1, 1);
		tooltip->SetVisible( true );
	}
	else
	{
		ImplementActionItem* item = g_pActionFrame->GetImplementActionItem(index);
		if ( item )
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->skillId);
			if ( itemDB )
			{
				g_GameTooltipMagicColSetting._value = 0;
				g_GameTooltipMagicColSetting.HideDistance = true;
				g_GameTooltipMagicColSetting.NotRoleSpellTime = true;
				g_GameTooltipMagicColSetting.NotRoleColdown = true;
				bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, NULL, itemDB);
				g_GameTooltipMagicColSetting._value = 0;
				tooltip->SetVisible( visible );
			}
		}
	}
	
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetSuitSkillEquipmentItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index = luaL_checkint(L, 2);

	SoubleItemLocked* equipItem = g_pSuitSkillFrame->GetSoubleItem(index);
	if ( equipItem )
	{
		ItemFieldStruct* item = equipItem->GetItemFieldStruct();
		if ( item )
		{
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, g_ObjectData->GetObj(item->OrgObjID));
			tooltip->SetVisible( visible );
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetSuitSkillSkillItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	int index = luaL_checkint(L, 2) - 1;
	int suitId = g_pSuitSkillFrame->GetSuitID();

	GameObjDbStruct* suitDB = g_ObjectData->GetObj(suitId);
	if ( suitDB && index >= 0 && index < 4 )
	{
		bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, NULL, g_ObjectData->GetObj(suitDB->Suit.SuitSkillID[index]));
		tooltip->SetVisible( visible );
	}
	return 0;
}

// ----------------------------------------------------------------------------------
bool _TooltipSetItem(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB, bool compareSelf)
{
	if (tooltip == NULL || itemDB == NULL )
		return false;

 	CUiColor color, colorL, colorR, durableColor, tierColor;
	char tmpL[512];
	char tmpR[512];
	char tmpA[512];
	const char *leftText, *rightText;
	AbilityStatistics abilityBase, abilityRefine, abilityInherent;
	WearEqTypeENUM wearType;
	float durableScale = 1.0f;
	float value[2];
	float wearValue;
	ItemFieldStruct *pCraftItemField = g_pCraftFrame->GetCraftItemField();

	int starValue = 0;	

	bool needUpdateFrame = false;

	switch ( itemDB->Classification )
	{
	case EM_ObjectClass_Treasure:
		{
			tooltip->AddLine(itemDB->GetName(), 1.0f , 1.0f , 1.0f );

			// ª««~¸Ô²Ó»¡©ú
			if ( itemDB->ShortNote[0] )
				tooltip->AddLine( ItemUTIL::GetItemNote( itemDB ).c_str() , 0.0f, 0.75f, 0.95f);
		}
		break;

	case EM_ObjectClass_Item:
		// ­pºâ­@¤[
		switch (itemDB->Item.Class)
		{
		case EM_Item_Weapon:
		case EM_Item_Armor:
			if ( item )
			{
				//durableScale = (float)item->Durable / (itemDB->Item.Durable * item->Quality);
				//durableScale = (float)item->Durable / (float)(itemDB->Item.Durable * 100 );
				int orgDurable = ( itemDB->Item.Durable * item->Quality ) / 100;

				if ( item->Durable/100 > 100 || item->Durable/100 > orgDurable )
				{
					durableColor.SetColor(0xFF00FF00);
					durableScale = 1.2f;
				}
				else if ( item->Durable/100 > orgDurable/2 )
				{
					durableColor.SetColor(0xFFFFFFFF);
					durableScale = 1.0f;
				}
				else if ( item->Durable/100 > orgDurable/5 )
				{
					durableColor.SetColor(0xFFFFFF00);
					durableScale = 0.8f;
				}
				else
				{
					durableColor.SetColor(0xFFFF0000);
					durableScale = 0.2f;
				}
			}
			break;
		}

		// ¨ú±oª««~¦WºÙ
		if ( !RoleDataEx::GetItemFieldName(item, tmpL, MAX_PATH) )
			strcpy(tmpL, itemDB->GetName());		

		// µ}¦³«×
		if ( pCraftItemField )
			color = g_ObjectData->GetItemQualityColor(g_ObjectData->GetItemQuality(itemDB, pCraftItemField));
		else
			color = g_ObjectData->GetItemQualityColor(g_ObjectData->GetItemQuality(itemDB, item));

		// °ò¥»¯à¤O		
		value[0] = 10.0f;
		if ( item )
		{
			switch (itemDB->Item.Class)
			{
			case EM_Item_Weapon:
			case EM_Item_Armor:
				if ( itemDB->MaxHeap <= 1 )
				{
					value[0] = (float)item->PowerQuality;
				}
				break;
			}			
		}
		abilityBase.Insert(itemDB->Item.Ability , value[0]/10.0f * durableScale);

		// ½Äµ¥¼Æ­È
		if ( item && item->Level > 0 )
		{
			sprintf(tmpR, "%s + %d", tmpL, item->Level);
			strcpy(tmpL, tmpR);		

			// ­pºâ½Äµ¥ªþ¥[¯à¤O­È
			GameObjDbStruct* refine = g_ObjectData->GetObj(itemDB->Item.RefineTableID + item->Level - 1);
			if ( refine != NULL )
			{
				abilityRefine.Insert(refine->EqRefineAttribute.Ability , durableScale);

				int ExEqPowerRate = refine->EqRefineAttribute.ExEqPowerRate;
				// ÀË¬d¬O§_¦³¥[ÃB¥~°ò¥»¯à¤O
				if( ExEqPowerRate != 0 )
				{

					float fAddDmg = 0.0f;
					float fAddMDmg = 0.0f;
					float fAddDef = 0.0f;
					float fAddMDef = 0.0f;
					RoleDataEx::GetRareEffect( itemDB , item->Rare , fAddDmg , fAddMDmg , fAddDef , fAddMDef );

					GameObjectWearEqStruct AbilityBK = itemDB->Item.Ability;

					for( int i = 0 ; i < Max_WearEqType_Count ; i++ )
					{
						if( AbilityBK.EqType[i] == EM_WearEqType_DMG )
						{
							AbilityBK.EqTypeValue[i]+= (int)fAddDmg;
							fAddDmg = 0;
						}
						else if( AbilityBK.EqType[i] == EM_WearEqType_MDMG )
						{
							AbilityBK.EqTypeValue[i]+= (int)fAddMDmg;
							fAddMDmg = 0;
						}
						else if( AbilityBK.EqType[i] == EM_WearEqType_DEF )
						{
							AbilityBK.EqTypeValue[i]+= (int)fAddDef;
							fAddDef = 0;
						}
						else if( AbilityBK.EqType[i] == EM_WearEqType_MDEF )
						{
							AbilityBK.EqTypeValue[i]+= (int)fAddMDef;
							fAddMDef = 0;
						}
						else
						{
							AbilityBK.EqTypeValue[i] = 0;
							continue;
						}

						AbilityBK.EqTypeValue[i] = AbilityBK.EqTypeValue[i] * ExEqPowerRate / 100;
						/*
						if(		AbilityBK.EqType[i] != EM_WearEqType_DMG
							&&	AbilityBK.EqType[i] != EM_WearEqType_MDMG
							&&	AbilityBK.EqType[i] != EM_WearEqType_DEF
							&&	AbilityBK.EqType[i] != EM_WearEqType_MDEF )
						{
							AbilityBK.EqTypeValue[i] = 0;
						}
						else
						{
							if( AbilityBK.EqType[i] == EM_WearEqType_DMG )
								AbilityBK.EqTypeValue[i]+= fAddDmg;
							else if( AbilityBK.EqType[i] == EM_WearEqType_MDMG )
								AbilityBK.EqTypeValue[i]+= fAddMDmg;
							else if( AbilityBK.EqType[i] == EM_WearEqType_DEF )
								AbilityBK.EqTypeValue[i]+= fAddDef;
							else if( AbilityBK.EqType[i] == EM_WearEqType_MDEF )
								AbilityBK.EqTypeValue[i]+= fAddMDef;
							else
								AbilityBK.EqTypeValue[i] = 0;


							AbilityBK.EqTypeValue[i] = AbilityBK.EqTypeValue[i] * ExEqPowerRate / 100;
						}
						*/
					}
					//TempData.Attr.AddAbility( &AbilityBK , EM_Armor_None , Pos , PowerRate_Durable );	
					abilityRefine.Insert(AbilityBK , durableScale);
				}

			}
			
			
		}

		// ­pºâ¬Pµ¥
		starValue += itemDB->Item.StarValue;
		if( item )
		{
			int maxCount = 6;
			switch ( itemDB->Item.Class )
			{
			case EM_Item_Normal:
			case EM_Item_Money:
				maxCount = 0;
				break;
			}

			// ¦³6ºØ¯à¤O
			for ( int i = 0; i < maxCount; i++ )
			{
				GameObjDbStruct* inherent = CNetGlobal::GetObj(item->Inherent(i));
				if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
				{
					starValue += inherent->Attirbute.StarValue;
				}
			}
		}
		tooltip->AddLine(tmpL, color.m_r, color.m_g, color.m_b, false, starValue);
		
		/*
		// Ä~©Óª«¥ó¸ê®Æ
		if( item )
		{
			// ¦³6ºØ¯à¤O
			for ( int i = 0; i < 6; i++ )
			{
				GameObjDbStruct* inherent = CNetGlobal::GetObj(item->Inherent(i));
				if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
				{
					abilityInherent.Insert(inherent->Attirbute.Ability);
				}
			}
		}
		*/

		// ¨¾Ãz
		if ( item )
		{
			switch (itemDB->Item.Class)
			{
			case EM_Item_Weapon:
			case EM_Item_Armor:
				if ( item->Mode.Protect ) {
					tooltip->AddLine(g_ObjectData->GetString("SYS_ITEM_PROTECT"), 1.0f, 1.0f, 1.0f);
				}
				break;
			}
		}

		/*
		// ¬O§_¥i¥H©î¸Ñ
		{
			GameObjDbStruct* recipe = g_ObjectData->GetObj(itemDB->Item.SrcRecipe);
			if ( recipe != NULL )
				tooltip->AddLine(g_ObjectData->GetString("SYS_ITEM_CAN_DISSOLUTION"), 0.99f, 0.8f, 0.23f);			
		}
		*/

		// ¸T¨î
		if ( item )
			TooltipAddSoulBoundText(tooltip, item);

		// ®M¸Ë§Þ¯àÂê©wª¬ºA
		if ( item && item->Mode.SuitSkill )
			tooltip->AddLine(g_ObjectData->GetString("SYS_SUITSKILL_LOCKED"), 1.0f, 0.0f, 0.0f );

		// °Ó«°ª««~
		if ( itemDB->Mode.DepartmentStore )
			tooltip->AddLine(g_ObjectData->GetString("SYS_ACCOUNT_ITEM"), 0.99f, 0.8f, 0.23f);

		// PK«OÅ@
		if ( item && item->Mode.PkProtect )
			tooltip->AddLine(g_ObjectData->GetString("SYS_PKPROTECT"), 1.0f, 1.0f, 1.0f);

		// PK¤£·|±¼¸¨
		if ( itemDB->Mode.PkNoDrop )
			tooltip->AddLine(g_ObjectData->GetString("SYS_NOTDROP"), 1.0f, 1.0f, 1.0f);

		// ¤£¥iºë·Ò
		if ( itemDB->Mode.DisableRefine )
			tooltip->AddLine(g_ObjectData->GetString("GAMETOOLTIP_DISABLEREFINE"), 1.0f, 1.0f, 1.0f);

		// ¤£¥iºë·Ò
		if ( itemDB->Mode.DisablePowerQuality )
			tooltip->AddLine(g_ObjectData->GetString("GAMETOOLTIP_DISABLEPOWERQUALITY"), 1.0f, 1.0f, 1.0f);

		// ¤£¥i¨Ï¥ÎÅ]¤ÛÄ_²°
		if ( itemDB->Mode.DisableMagicBox )
			tooltip->AddLine(g_ObjectData->GetString("GAMETOOLTIP_DISABLEMAGICBOX"), 1.0f, 1.0f, 1.0f);

		if ( itemDB->Mode.DisableDigHode )
			tooltip->AddLine(g_ObjectData->GetString("GAMETOOLTIP_DISABLEDIGHODE"), 1.0f, 1.0f, 1.0f);

		// ¸Ñ°£­­¨î
		{
			if ( item )
			{
				compareSelf = !item->Mode.IgnoreLimitLv;
			}
		}

		// ª««~°ß¤@
		if ( itemDB->Mode.Unique )
		{
			tooltip->AddLine(g_ObjectData->GetString("SYS_UNIQUE"), 0.99f, 0.8f, 0.23f);
		}

		// ¨Ï¥Î¦¸¼Æ
		if ( itemDB->Mode.HideCount )
		{
			int count = itemDB->BuyUnit;
			if ( item )
				count = item->Count;

			if ( itemDB->Mode.HideMaxHeap ) {
				sprintf_s(tmpL, 512, "%s %d", g_ObjectData->GetString("SYS_ITEM_USE_COUNT"), count);
			} else {
				sprintf_s(tmpL, 512, "%s %d/%d", g_ObjectData->GetString("SYS_ITEM_USE_COUNT"), count, itemDB->MaxHeap);
			}
			
			tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
		}

		// °ïÅ|¼Æ¶q
		if ( item && item->GetCount() > 1 )
		{
			sprintf_s(tmpL, "%s%d", g_ObjectData->GetString("SYS_STACK"), item->GetCount());
			tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
		}

		// ¦s¦b®É¶¡
		switch ( itemDB->LiveTimeType )
		{
		case EM_ItemLiveTimeType_None:			//¤£·|®ø¥¢
			break;

		case EM_ItemLiveTimeType_RealTime:		//¯u¹ê®É¶¡
			if ( item && item->CreateTime > 0 )
			{
				int second = max(itemDB->LiveTime - ((int)g_pGameMain->GetGameTime() - item->CreateTime) - 1, 0);
				strcpy(tmpL, g_ObjectData->GetString("SYS_ITEM_LIVETIME"));

				if ( second > 86400 ) {	
					// ¤Ñ¼Æ
					sprintf_s(tmpR, 512, " %d %s", second / 86400 + 1, g_ObjectData->GetString("SYS_DAY"));
				} else if ( second > 3600 ) {
					// ¤p®É
					sprintf_s(tmpR, 512, " %d %s", second / 3600 + 1, g_ObjectData->GetString("SYS_HOUR"));
				} else if ( second > 60 ) {
					// ¤ÀÄÁ
					sprintf_s(tmpR, 512, " %d %s", second / 60 + 1, g_ObjectData->GetString("SYS_MINUTE"));
				} else {
					// ¬í
					sprintf_s(tmpR, 512, " %d %s", max(second, 0), g_ObjectData->GetString("SYS_SECOND"));
				}
				strcat(tmpL, tmpR);
				tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
			}
			break;
		}
	 
		switch (itemDB->Item.Class)
		{
		case EM_Item_Normal:
			{				
				// ¨Ï¥Î­­¨î,µ¥¯Å,Â¾·~,ÄÝ©Ê,©Ê§O
				TooltipAddLimitText(tooltip, item, itemDB->Limit, compareSelf);

				// ¥ý¥H°Ó«~¤ÀÃþ,¥¼¦³°Ó«~¤ÀÃþ¦b¥Hª««~¤ÀÃþ
				sprintf(tmpL, "SYS_ITEMTYPE_%02d", itemDB->Item.ItemType);
				leftText = g_ObjectData->GetString(tmpL);
				if ( strlen(leftText) > 0 )
				{
					tooltip->AddLine(leftText, 1.0f, 1.0f, 1.0f);
				}

				//ºØ´Ó
				{
   					bool bIsPlant = false;

					//ºØ¤l
					if( itemDB->IsSeed() )
					{
						bIsPlant = true;
						sprintf(tmpL, "SYS_PLANT_TYPE_%d", itemDB->Item.Plant.Seed.Type );
						const char* szType = g_ObjectData->GetString(tmpL);

						if( NULL == szType ) 
							szType = "";

						const char* szForamt = g_ObjectData->GetString( "SYS_PLANT_ATTRIBUTE" );
						sprintf_s(tmpL, 512, szForamt, szType );
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
					}

					//¬Ö®â
					if( itemDB->IsPot() )
					{
						bIsPlant = true;
					}

					//ªÎ®Æ
					if( itemDB->IsManure() )
					{
						bIsPlant = true;
					}

					if( bIsPlant )
					{
						int SkillLV = (int) CNetGlobal::RoleData()->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Plant );

						const char* szForamt = g_ObjectData->GetString( "SYS_NEED_PLANT_SKILL" );
				
						if( itemDB->Item.Plant.Level > SkillLV )
						{
		  					sprintf_s( tmpR, 512, szForamt, itemDB->Item.Plant.Level );
  							sprintf_s( tmpL, 512,"|cffff2020%s|r", tmpR );
						}
						else
						{
							sprintf_s( tmpL, 512, szForamt, itemDB->Item.Plant.Level );
						}


						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
					}
				}

				// ª««~»ù­È
				int price = (itemDB->Cost + 5) / 10;
				if ( price > 0 && itemDB->Mode.Sell )
				{					
					g_ObjectData->GetMoneyText(price, tmpR, 512);
					sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_ITEM_COST"), tmpR);
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				if ( itemDB->Mode.Sell == false )
				{
					tooltip->AddLine(g_ObjectData->GetString("SYS_CANT_SELL"), 1.0f, 1.0f, 1.0f);
				}

				// ¥D­n¯à¤O
				switch(itemDB->Item.ItemType)
				{
				case EM_ItemType_Plot:
					// ¥ô°È¦WºÙ
					//tooltip->AddLine("¥ô°È¦WºÙ", 1.0f, 1.0f, 1.0f);
					//±N¥ô°ÈÅã¥Ü¦b³Ì«á
					break;

					/*
				case EM_ItemType_Food:
				case EM_ItemType_SweetFood:
				case EM_ItemType_Water:
					// ªk³N®ÄªG»¡©ú
					sprintf(tmpL, "SYS_ITEMTYPE_%02d", itemDB->Item.ItemType);
					tooltip->AddLine(g_ObjectData->GetString(tmpL), 1.0f, 1.0f, 1.0f);
					break;
					*/

				case EM_ItemType_PosRecord:
					// ¶Ç°eÂI¸ê°T
					if ( item )
					{
						GameObjDbStructEx* zoneObj = CNetGlobal::GetObj(Def_ObjectClass_Zone + item->ZoneID);
						if ( zoneObj )
						{
							for ( int i = 0; i < 40; i++ )
							{
								if ( zoneObj->Zone.MapInfoList[i].MapID == item->MapID )
								{
									const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( item->ZoneID );

									float CoordX = 0;
									float CoordY = 0;

									if( pWorldMapInfo )
									{
										SWorldCoordinate WorldCoord;
										SMapCoordinate MapCoord;

										WorldCoord.wdbFile = pWorldMapInfo->wdbFile;
										WorldCoord.PosX = (float)item->X;
										WorldCoord.PosZ = (float)item->Z;

										MapCoord.MapID = pWorldMapInfo->MapID;

										if( Func_TransformCoordinate_WorldToMap( WorldCoord, MapCoord ) )
										{
											CoordX = MapCoord.PosX;
											CoordY = MapCoord.PosZ;
										}
									}

									//char temp[512];
									//sprintf(temp, "ZONE_%s", zoneObj->Zone.MapInfoList[i].MapName);
									//strupr(temp);

									//const char* name = g_ObjectData->GetString(temp);
									//if ( strlen(name) <= 0 )
									//	name = temp;

									const char* name = Func_GetLocalMapName( pWorldMapInfo->MapName ); 

									sprintf(tmpL, "%s (%.1f, %.1f)", name, CoordX * 100 , CoordY * 100 );
									tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);

									//sprintf(tmpL, "%s (%d,%d,%d)", name, item->X, item->Y, item->Z);
									//tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
									break;
								}
							}
						}
					}
					break;

				case EM_ItemType_Lottery:				// ¼Ö³z±m¨÷
					if ( item )
					{
						// ´Á¼Æ
						sprintf(tmpL , "%s %d", g_ObjectData->GetString("SYS_LOTTERY_NO"), item->Lottery.Index);
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);

						//¡u¯S®íÁÊ¶R¡G¿ï¾Ü2­Ó¸¹½X¡v
						if( item->Lottery.Count != 5 )
						{
							sprintf( tmpL, g_ObjectData->GetString("SYS_LOTTERY_SPECIAL"), item->Lottery.Count );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
						}
						
						// ¸¹½X
						strcpy(tmpL, g_ObjectData->GetString("SYS_LOTTERY_NUMBER"));
						for ( int i = 0; i < item->Lottery.Count; i++ )
						{
							sprintf(tmpR, " %d", item->Lottery.SelectID[i]);
							strcat(tmpL, tmpR);
						}
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
					}
					break;
				case EM_ItemType_Furniture: //³Ã­Ñ
					{
						if( itemDB->Item.Furniture_Type == EM_FurnitureType_All )
						{

							if( itemDB->Item.emFurnitureType   != EM_FurnitureType_Normal ||
								itemDB->Item.ExpPoint          != 0 ||
								itemDB->Item.TpPoint           != 0 ||
								itemDB->Item.BlackSmithHQPoint != 0 ||
								itemDB->Item.CarpenterHQPoint  != 0 ||	
								itemDB->Item.MakeArmorHQPoint  != 0 ||	
								itemDB->Item.TailorHQPoint     != 0 ||		
								itemDB->Item.CookHQPoint       != 0 ||		
								itemDB->Item.AlchemyHQPoint    != 0  )
							{
								tooltip->AddLine( g_ObjectData->GetString("FURNITURE_FUNCTION_LIMIT"), 0.99f, 0.8f, 0.23f );
							}
						}
						else if( itemDB->Item.Furniture_Type == EM_FurnitureType_HouseOnly )
						{
							tooltip->AddLine( g_ObjectData->GetString("FURNITURE_HOUSEONLY"), 0.99f, 0.8f, 0.23f );	
						}
						else if( itemDB->Item.Furniture_Type == EM_FurnitureType_GuildOnly )
						{
							tooltip->AddLine( g_ObjectData->GetString("FURNITURE_GUILDONLY"), 0.99f, 0.8f, 0.23f );	
						}
												
						if( itemDB->Item.ExpPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_EXPPOINT"), itemDB->Item.ExpPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.TpPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_TPPOINT"), itemDB->Item.TpPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.BlackSmithHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_BLACKSMITHHQPOINT"), itemDB->Item.BlackSmithHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.CarpenterHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_CARPENTERHQPOINT"), itemDB->Item.CarpenterHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.MakeArmorHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_MAKEARMORHQPOINT"), itemDB->Item.MakeArmorHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.TailorHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_TAILORHQPOINT"), itemDB->Item.TailorHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.CookHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_COOKHQPOINT"), itemDB->Item.CookHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.AlchemyHQPoint > 0  )
						{
							sprintf(tmpL , "%s %d", g_ObjectData->GetString("FURNITURE_ALCHEMYHQPOINT"), itemDB->Item.AlchemyHQPoint );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
					}
					break;

				case EM_ItemType_SpecialRune:		//¯S®í²Å¤å
					break;

				case EM_ItemType_SmeltStone:		//º²¸Ñ¥Û
					break;
				
				case EM_ItemType_Packet:			//¥]»qª««~
					if ( item )
					{
						tooltip->AddLine(g_ObjectData->GetString("SYS_TOOLTIP_ITEM_PACKET"), 1.0f, 1.0f, 1.0f);
						for ( int i = 0; i < 8; i++ )
						{
							int itemID = item->IntAbility[i] >> 8;
							int count = item->IntAbility[i] & 0xFF;

							GameObjDbStructEx* packetItem = g_ObjectData->GetObj(itemID);
							if ( packetItem )
							{
								// ¨ú±oª««~¦WºÙ
								strcpy(tmpL, packetItem->GetName());
								if ( count > 1 )
								{
									sprintf_s(tmpR, 512, " x %d", count);
									strcat(tmpL, tmpR);
								}

								// µ}¦³«×
								color = g_ObjectData->GetItemQualityColor(g_ObjectData->GetItemQuality(packetItem, NULL));
								tooltip->AddLine(tmpL, color.m_r, color.m_g, color.m_b);
							}
						}
					}
					break;

				case EM_ItemType_Pet:				// Ãdª«³J
					if( item )
					{
						tooltip->AddLine( " ", 1.0f, 1.0f, 1.0f );	

						if( item->Pet.PetOrgID )
						{
							GameObjDbStruct* pObj = g_ObjectData->GetObj( item->Pet.PetOrgID );

							if( pObj )
							{
								sprintf( tmpL, "%s%s", g_ObjectData->GetString("PET_KIND"), pObj->Name );
								tooltip->AddLine( tmpL, 1.0f, 1.0f, 1.0f );	
							}							
						}

						sprintf( tmpL, "%s%d", g_ObjectData->GetString("PET_LEVEL"), item->Pet.Lv );
						tooltip->AddLine( tmpL, 1.0f, 1.0f, 1.0f );	

						if( itemDB->Item.Pet.Property >= 0 && itemDB->Item.Pet.Property <= 6 )
						{
							char buf[256];
							sprintf_s( buf, 256, "SYS_MAGIC_TYPE%02d", itemDB->Item.Pet.Property + 1);
							sprintf( tmpL, "%s%s", g_ObjectData->GetString("PET_PROPERTY"), g_ObjectData->GetString(buf) );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						if( itemDB->Item.Pet.Property == -1 )
						{
							char buf[256];
							sprintf_s( buf, 256, "SYS_MAGIC_TYPE%02d", itemDB->Item.Pet.Property );
							sprintf( tmpL, "%s%s", g_ObjectData->GetString("PET_PROPERTY"), g_ObjectData->GetString("SYS_TOOLTIPS_BURN_NONE") );
							tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);	
						}
						

						sprintf( tmpL, "%s%d", g_ObjectData->GetString("PET_TOOLTIP_TALENT"), item->Pet.Talent );
						tooltip->AddLine( tmpL, 1.0f, 1.0f, 1.0f );	

						sprintf( tmpL, g_ObjectData->GetString("PET_GROWTH"), itemDB->Item.Pet.STR, itemDB->Item.Pet.STA,
								 itemDB->Item.Pet.AGI, itemDB->Item.Pet.INT, itemDB->Item.Pet.MND );
						tooltip->AddLine( tmpL, 1.0f, 1.0f, 1.0f );

						tooltip->AddLine( " ", 1.0f, 1.0f, 1.0f );
					}
					break;

				case EM_ItemType_Egg:				// Âà³Jª««~(³Ì¤j8­Ó)
					if ( item )
					{
						tooltip->AddLine(g_ObjectData->GetString("SYS_TOOLTIP_ITEM_EGG"), 1.0f, 1.0f, 1.0f);
						for ( int i = 0; i < 8; i++ )
						{
							int itemID = item->IntAbility[i] >> 8;

							GameObjDbStructEx* packetItem = g_ObjectData->GetObj(itemID);
							if ( packetItem )
							{
								// ¨ú±oª««~¦WºÙ
								sprintf_s(tmpL, 512, "[%s]", packetItem->GetName());

								// µ}¦³«×
								color = g_ObjectData->GetItemQualityColor(g_ObjectData->GetItemQuality(packetItem, NULL));
								tooltip->AddLine(tmpL, color.m_r, color.m_g, color.m_b);
							}
						}
					}
					break;
				}

				switch (itemDB->Item.ItemType)
				{
				case EM_ItemType_Plot:				// ¼@±¡ª««~
				case EM_ItemType_Money:				// ª÷¿ú
				case EM_ItemType_PosRecord:			// ¦ì¸m°O¿ý
				case EM_ItemType_Lottery:			// ¼Ö³z±m¨÷
				case EM_ItemType_Packet:			// ¥]»qª««~
				case EM_ItemType_Egg:				// Âà³Jª««~
				case EM_ItemType_Furniture:			// ®a­Ñ
				case EM_ItemType_Seed:            	// ºØ¤l
				case EM_ItemType_Flowerpot:       	// ªá¬Ö
				case EM_ItemType_SummonHorse:		// ¬V¦â§¤ÃM
				case EM_ItemType_Pet:				// Ãdª«ª««~
				case EM_ItemType_Pet_Tools:			// Ãdª«¤u¨ã
				case EM_ItemType_Pet_Furniture:		// Ãdª«³Ã­Ñ
				case EM_ItemType_Pet_Food:			// Ãdª«­¹ª«
					break;

				default:
					// Ä~©Ó¯à¤O
					if ( item )
					{
						// ¦³6ºØ¯à¤O
						for ( int i = 0; i < 6; i++ )
						{
							GameObjDbStructEx* inherent = CNetGlobal::GetObj(item->Inherent(i));
							if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
							{
								// ¯à¤O
								TooltipAddAbilityText(tooltip, inherent->Attirbute, inherent->GetName());
							}
						}
					}
					else
					{
						TooltipAddDropAbility(tooltip, item, itemDB);
					}
				}

				// ª««~¸Ô²Ó»¡©ú
				if ( itemDB->ShortNote[0] )
					tooltip->AddLine( ItemUTIL::GetItemNote( itemDB ).c_str() , 0.0f, 0.75f, 0.95f);

				//¬Ö®â
				if( itemDB->IsPot() && item && item->Plant.OrgSeedID > 0 )
				{
					GameObjDbStructEx* itemDBSeed = CNetGlobal::GetObj( item->Plant.OrgSeedID + Def_ObjectClass_Item );

					if ( itemDBSeed )
					{
						float fGrowRate   = (float)( (double)item->Plant.GrowRate / 100 ); if( fGrowRate > 100 ) fGrowRate = 100;
						float fHealthRate = (float)( (double)item->Plant.HealthPoint / 100 );

						string strState;

						if( item->Plant.IsDead )
						{
							strState = g_ObjectData->GetString( "SYS_TOOLTIP_PLANT_STATE_DEAD" );
						}
						else if( item->Plant.IsLock )
						{
							strState = g_ObjectData->GetString( "SYS_TOOLTIP_PLANT_STATE_LOCK" );
						}
						else if( item->Plant.GrowRate / 10000 > 1 )
						{
							strState = g_ObjectData->GetString( "SYS_TOOLTIP_PLANT_STATE_RIPE" );
						}
						else
						{
							strState = g_ObjectData->GetString( "SYS_TOOLTIP_PLANT_STATE_GROW" );
						}

						//SYS_TOOLTIP_PLANT_INFO_FORMAT <seedname><grow><health><state> 
						string strTemp = g_ObjectData->GetString( "SYS_TOOLTIP_PLANT_INFO_FORMAT" );

						strTemp = ReplaceKeyString( strTemp, "<seedname>", itemDBSeed->GetName() );
						strTemp = ReplaceKeyString( strTemp, "<grow>"    , (int)fGrowRate );
						strTemp = ReplaceKeyString( strTemp, "<health>"  , (int)fHealthRate );
						strTemp = ReplaceKeyString( strTemp, "<state>"   , strState );	 

						tooltip->AddLine( strTemp.c_str(), 1.0f, 1.0f, 1.0f);
					}
				}

				//¬O§_¬°¨­¤W¥ô°Èªºª««~
				if( item )
				{
					PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
					int iQuestID;
					
					int ItemIndex;
					for( int QuestIndex = 0 ; QuestIndex<_MAX_QUEST_COUNT_ ; ++QuestIndex )
					{
						iQuestID = pBaseData->Quest.QuestLog[QuestIndex].iQuestID;
						if( iQuestID == 0 )
							continue;


						GameObjDbStructEx* pQuestObj = g_ObjectData->GetObj( iQuestID );
						GameObjectQuestDetail* pQuestDetail	= NULL;

						if( pQuestObj )
						{
							pQuestDetail = &( pQuestObj->QuestDetail );

							for( ItemIndex = 0 ; ItemIndex < _MAX_SIZE_QUEST_CONDITION ; ++ItemIndex )
							{
								if( pQuestDetail->iRequest_ItemID[ItemIndex] == 0 )
								{
									break;
								}
								if( pQuestDetail->iRequest_ItemID[ItemIndex] == itemDB->GUID )
								{
									int count = CNetGlobal::RoleData()->Sc_GetItemNumFromRole( item->OrgObjID );
									sprintf( tmpL, g_ObjectData->GetString("SYS_TOOLTIP_QUEST_TEXT"), pQuestObj->GetName() , count, pQuestDetail->iRequest_ItemVal[ItemIndex] ) ;
									tooltip->AddLine(tmpL, 1.0f, 0.0f, 0.0f);
									break;
								}
							}
						}
					}
				}
			}
			break;

		case EM_Item_Weapon:
			{
				// ¶¥¼h
				int tier = ( itemDB->Limit.Level -1 ) / 20 + 1;
				if ( item ) {
					if ( item->PowerQuality > 10 )
						tierColor.SetColor(0xFF00FF00);
					else if ( item->PowerQuality < 10 )
						tierColor.SetColor(0xFFFF0000);
					else
						tierColor.SetColor(0xFFFFFFFF);
					tier += max(item->PowerQuality - 10, 0);
				}
				sprintf( tmpL, g_ObjectData->GetString("SYS_TOOLTIP_RUNE_LEVEL"), tier );

				// ­@¤[«×
				if ( item && itemDB->Item.WeaponPos != EM_WeaponPos_Ammo && item->Mode.ClientCreate == false )
				{
					/*
					int extraDurable = (itemDB->Item.Durable * item->Quality / 100) - itemDB->Item.Durable;
					if ( extraDurable > 0 )
						sprintf( tmpR, "%s %d/%d|cff00ff00+%d|r", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable, extraDurable);
					else if ( extraDurable < 0 )
						sprintf( tmpR, "%s %d/%d|cffff0000%d|r", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable, extraDurable);
					else
						sprintf( tmpR, "%s %d/%d", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable);
					*/
					
					sprintf( tmpR, "%s %d/%d", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable*item->Quality/100);
					tooltip->AddDoubleLine(tmpL, tmpR, tierColor.m_r, tierColor.m_g, tierColor.m_b, durableColor.m_r, durableColor.m_g, durableColor.m_b);

					// ¯à¤O¥[¦¨
					strcpy( tmpL, " " );
					if ( durableScale > 1.0f )
					{
						sprintf( tmpR, "%s |cff00ff00+%d%%|r", g_ObjectData->GetString("SYS_POWER_MODIFY"), (int)(durableScale * 100) - 100);
						tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					}
					else if ( durableScale < 1.0f )
					{
						sprintf( tmpR, "%s |cffff0000%d%%|r", g_ObjectData->GetString("SYS_POWER_MODIFY"), (int)(durableScale * 100) - 100);
						tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
				else
				{
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}

				// ¨Ï¥Î­­¨î,µ¥¯Å,Â¾·~,ÄÝ©Ê,©Ê§O
				TooltipAddLimitText(tooltip, item, itemDB->Limit, compareSelf);

				// »Å¸Ë
				if ( itemDB->Mode.SpecialSuit )
				{
					strcpy(tmpL, g_ObjectData->GetString("SYS_SPECIALSUIT"));
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				
				// ªZ¾¹Ãþ«¬¤Î³¡¦ì
				sprintf(tmpL, "SYS_WEAPON_TYPE%02d", itemDB->Item.WeaponType);
				leftText = g_ObjectData->GetString(tmpL);				
				sprintf(tmpR, "SYS_WEAPON_POS%02d", itemDB->Item.WeaponPos);
				rightText = g_ObjectData->GetString(tmpR);
				if ( compareSelf && !CNetGlobal::RoleData()->CheckEquipmentWearSkill(itemDB) )
					tooltip->AddDoubleLine(leftText, rightText, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
				else
					tooltip->AddDoubleLine(leftText, rightText, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

				// µ}¦³«×¼W¥[ªº¯à¤O
				float fAddDmg = 0.0f;
				float fAddMDmg = 0.0f;
				float fAddDef = 0.0f;
				float fAddMDef = 0.0f;
				float fTotalDmg = 0.0f;

				// §ðÀ»³t«×
				float speed[2];
				speed[0] = abilityBase.Pop(EM_WearEqType_AtkSpeed);
				speed[1] = abilityRefine.Pop(EM_WearEqType_AtkSpeed);
				float atkspeed = itemDB->Item.AttackSpeed / 10.0f * ( 1.0f + (speed[0] + speed[1]) / 100.0f ) + 0.01f;

				if( pCraftItemField )
				{
					RoleDataEx* pRole = CNetGlobal::RoleData();
					pRole->GetRareEffect( itemDB , pCraftItemField->Rare , fAddDmg , fAddMDmg , fAddDef , fAddMDef );
				}
				else if( item )
				{
					RoleDataEx* pRole = CNetGlobal::RoleData();
					pRole->GetRareEffect( itemDB , item->Rare , fAddDmg , fAddMDmg , fAddDef , fAddMDef );
				}

				// §ðÀ»¤O
				value[0] = abilityBase.Pop(EM_WearEqType_DMG);													// base
				value[1] = abilityRefine.Pop(EM_WearEqType_DMG);												// extra
				//value[1] = abilityRefine.Pop(EM_WearEqType_DMG) + abilityInherent.Pop(EM_WearEqType_DMG);		// extra

				colorL = g_whiteColor;
				ObjectDataClass::IntegerString(value[0] + value[1] + fAddDmg, tmpA, 512);
				fTotalDmg = value[0] + value[1] + fAddDmg;

				if ( value[1] > 0 )
					sprintf(tmpL, "%s |cff00ff00%s (%s %.1f)|r", g_ObjectData->GetString("SYS_WEAPON_DMG"), tmpA, g_ObjectData->GetString("SYS_WEAPON_DPS"), fTotalDmg/atkspeed);
				else if ( value[1] < 0 )
					sprintf(tmpL, "%s |cffff0000%s (%s %.1f)|r", g_ObjectData->GetString("SYS_WEAPON_DMG"), tmpA, g_ObjectData->GetString("SYS_WEAPON_DPS"), fTotalDmg/atkspeed);
				else
					sprintf(tmpL, "%s %s (%s %.1f)", g_ObjectData->GetString("SYS_WEAPON_DMG"), tmpA, g_ObjectData->GetString("SYS_WEAPON_DPS"), fTotalDmg/atkspeed);

				// §ðÀ»³t«×
				value[0] = speed[0];
				value[1] = speed[1];
				//value[1] = abilityRefine.Pop(EM_WearEqType_AtkSpeed) + abilityInherent.Pop(EM_WearEqType_AtkSpeed);

				// ¤l¼u
				if ( itemDB->Item.WeaponPos == EM_WeaponPos_Ammo )
				{
					tooltip->AddLine(tmpL, colorL.m_r, colorL.m_g, colorL.m_b);
				}
				else
				{					
					colorR = g_whiteColor;
					ObjectDataClass::IntegerString(atkspeed, tmpA, 512);
					if ( value[1] > 0 )
						sprintf(tmpR, "%s |cff00ff00%s|r", g_ObjectData->GetString("SYS_WEAPON_ATKSPEED"), tmpA);
					else if ( value[1] < 0 )
						sprintf(tmpR, "%s |cffff0000%s|r", g_ObjectData->GetString("SYS_WEAPON_ATKSPEED"), tmpA);
					else
						sprintf(tmpR, "%s %s", g_ObjectData->GetString("SYS_WEAPON_ATKSPEED"), tmpA);

					tooltip->AddDoubleLine(tmpL, tmpR, colorL.m_r, colorL.m_g, colorL.m_b, colorR.m_r, colorR.m_g, colorR.m_b);
				}				

				// Å]ªk§ðÀ»¤O
				value[0] = abilityBase.Pop(EM_WearEqType_MDMG);
				value[1] = abilityRefine.Pop(EM_WearEqType_MDMG);
				//value[1] = abilityRefine.Pop(EM_WearEqType_MATK) + abilityInherent.Pop(EM_WearEqType_MATK);
				if ( value[0] + value[1] != 0 )
				{
					if ( value[1] > 0 )
						colorL = g_greenColor;
					else if ( value[1] < 0 )
						colorL = g_redColor;
					else
						colorL = g_whiteColor;
					ObjectDataClass::IntegerString(value[0] + value[1] + fAddMDmg, tmpA, 512);
					sprintf( tmpL, "%s %s", g_ObjectData->GetString("SYS_WEAPON_MDMG"), tmpA );
					tooltip->AddLine(tmpL, colorL.m_r, colorL.m_g, colorL.m_b);
				}				

				// ª««~»ù­È
				int price = (itemDB->Cost + 5) / 10;
				if ( price > 0 && itemDB->Mode.Sell )
				{					
					g_ObjectData->GetMoneyText(price, tmpR, 512);
					sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_ITEM_COST"), tmpR);
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				if ( itemDB->Mode.Sell == false )
				{
					tooltip->AddLine(g_ObjectData->GetString("SYS_CANT_SELL"), 1.0f, 1.0f, 1.0f);
				}


				// ­×²z¶O¥Î
				if ( item && g_pStoreFrame->GetRepairState() )
				{
					int repairMoney = RoleDataEx::CalRepairWeapon(item, NULL);
					if ( repairMoney < 0 )
					{
						g_ObjectData->GetMoneyText(-repairMoney, tmpR, 512);
						sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_REPAIR_COST"), tmpR);
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 0.0f);
					}
				}

				// °ò¥»¯à¤O + ½Äµ¥¯à¤O
				abilityBase.Insert(abilityRefine);
				while ( abilityBase.Pop(wearType, wearValue) )
				{
					TooltipAddWearEqTypeText(tooltip, wearType, wearValue);
				}

				// ªþ¥[¯à¤O
				if( item )
				{
					// Ä~©Ó¯à¤O
					for ( int i = 0; i < 6; i++ )
					{
						GameObjDbStructEx* inherent = CNetGlobal::GetObj(item->Inherent(i));
						if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
						{
							// ¯à¤O
							TooltipAddAbilityText(tooltip, inherent->Attirbute, inherent->GetName(), durableScale);
						}
					}

					// ²Å¤å¯à¤O
					int runeVolume = RoleDataEx::GetRuneTotalVolume(item);
					if ( runeVolume > 0 )
					{
						sprintf( tmpL, "%s (%d/%d)", g_ObjectData->GetString("SYS_ITEM_RUNE"), runeVolume - item->RuneVolume, runeVolume );
						tooltip->AddLine(tmpL, 0.74f, 0.18f, 1.0f);

						// ²Å¤å¯à¤O»¡©ú
						TooltipAddRuneAbilityText(tooltip, item, durableScale);
					}
				}
				else
				{
					TooltipAddDropAbility(tooltip, item, itemDB);
				}

				// ª««~¸Ô²Ó»¡©ú
				if ( itemDB->ShortNote[0] )
				{
					tooltip->AddLine( ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f);
				}

				// ®M¸Ë¦Cªí
				if ( itemDB->Item.SuitID != 0 )
				{
					tooltip->AddSeparator();

					GameObjDbStructEx* suit = CNetGlobal::GetObj(itemDB->Item.SuitID);
					if ( suit && suit->Classification == EM_ObjectClass_Suit )
						TooltipAddSuitAbilityText(tooltip, suit);
				}
			}
			break;

		case EM_Item_Armor:
			{
				// ¶¥¼h
				int tier = ( itemDB->Limit.Level -1 ) / 20 + 1;
				if ( item )
				{
					if ( item->PowerQuality > 10 )
						tierColor.SetColor(0xFF00FF00);
					else if ( item->PowerQuality < 10 )
						tierColor.SetColor(0xFFFF0000);
					else
						tierColor.SetColor(0xFFFFFFFF);
					tier += max(item->PowerQuality - 10, 0);
				}
				sprintf( tmpL, g_ObjectData->GetString("SYS_TOOLTIP_RUNE_LEVEL"), tier );

				// ­@¤[«×
				if ( item && item->Mode.ClientCreate == false )
				{
					/*
					int extraDurable = (itemDB->Item.Durable * item->Quality / 100) - itemDB->Item.Durable;
					if ( extraDurable > 0 )
						sprintf( tmpR, "%s %d/%d|cff00ff00+%d|r", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable, extraDurable);
					else if ( extraDurable < 0 )
						sprintf( tmpR, "%s %d/%d|cffff0000%d|r", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable, extraDurable);
					else
						sprintf( tmpR, "%s %d/%d", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable);
					*/

					sprintf( tmpR, "%s %d/%d", g_ObjectData->GetString("SYS_ITEM_DURABLE"), item->Durable/100, itemDB->Item.Durable*item->Quality/100);
					tooltip->AddDoubleLine(tmpL, tmpR, tierColor.m_r, tierColor.m_g, tierColor.m_b, durableColor.m_r, durableColor.m_g, durableColor.m_b);

					// ªkÄ_Ãþ¤£Åã¥Ü
					if ( itemDB->Item.ArmorPos != EM_ArmorPos_MagicTool )
					{
						// ¯à¤O¥[¦¨
						strcpy( tmpL, " " );
						if ( durableScale > 1.0f )
						{
							sprintf( tmpR, "%s |cff00ff00+%d%%|r", g_ObjectData->GetString("SYS_POWER_MODIFY"), (int)(durableScale * 100) - 100);
							tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
						}
						else if ( durableScale < 1.0f )
						{
							sprintf( tmpR, "%s |cffff0000%d%%|r", g_ObjectData->GetString("SYS_POWER_MODIFY"), (int)(durableScale * 100) - 100);
							tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
						}
					}
				}
				else
				{
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}

				// ¨Ï¥Î­­¨î,µ¥¯Å,Â¾·~,ÄÝ©Ê,©Ê§O
				TooltipAddLimitText(tooltip, item, itemDB->Limit, compareSelf);

				// ¨¾¨ãÃþ«¬¤Î³¡¦ì
				// sprintf(tmpL, "%s |cff00ff00%d|r", g_ObjectData->GetString("SYS_WEAPON_DMG"), value[0] + value[1]);	

				
				// »Å¸Ë
				if ( itemDB->Mode.SpecialSuit )
				{
					strcpy(tmpL, g_ObjectData->GetString("SYS_SPECIALSUIT"));
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}

				// ªZ¾¹Ãþ«¬¤Î³¡¦ì
				sprintf(tmpL, "SYS_ARMORTYPE_%02d", itemDB->Item.ArmorType);
				leftText = g_ObjectData->GetString(tmpL);
				sprintf(tmpR, "SYS_ARMORPOS_%02d", itemDB->Item.ArmorPos);
				rightText = g_ObjectData->GetString(tmpR);
				if ( compareSelf && !CNetGlobal::RoleData()->CheckEquipmentWearSkill(itemDB) ) {
					tooltip->AddDoubleLine(leftText, rightText, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
				} else {
					tooltip->AddDoubleLine(leftText, rightText, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}				

				if ( itemDB->Item.ArmorPos != EM_ArmorPos_MagicTool )
				{
					float physicsDefence = 0.0f;
					float magicDefence = 0.0f;

					// µ}¦³«×¼W¥[ªº¯à¤O
					float fAddDmg = 0.0f;
					float fAddMDmg = 0.0f;
					float fAddDef = 0.0f;
					float fAddMDef = 0.0f;

					if( pCraftItemField )
					{
						RoleDataEx* pRole = CNetGlobal::RoleData();						
						pRole->GetRareEffect( itemDB , pCraftItemField->Rare , fAddDmg , fAddMDmg , fAddDef , fAddMDef );
					}
					else if( item )
					{
						RoleDataEx* pRole = CNetGlobal::RoleData();
						pRole->GetRareEffect( itemDB , item->Rare , fAddDmg , fAddMDmg , fAddDef , fAddMDef );
					}

					// ¨¾¿m¤O
					value[0] = abilityBase.Pop(EM_WearEqType_DEF);													// base
					value[1] = abilityRefine.Pop(EM_WearEqType_DEF);												// extra

					switch (itemDB->Item.ArmorPos)
					{
					case EM_ArmorPos_Necklace:	//¶µÁå 
					case EM_ArmorPos_Ring:		//§Ù¤l
					case EM_ArmorPos_Earring:	//¦Õ¹¢
						physicsDefence = value[0] + value[1];
						break;

					default:
						physicsDefence = value[1];
						break;
					}

					//value[1] = abilityRefine.Pop(EM_WearEqType_DEF) + abilityInherent.Pop(EM_WearEqType_DEF);		// extra
					ObjectDataClass::IntegerString(value[0] + value[1] + fAddDef, tmpA, 512);
					colorL = g_whiteColor;					
					if ( physicsDefence > 0.0f )
						sprintf(tmpL, "%s |cff00ff00%s|r", g_ObjectData->GetString("SYS_ARMOR_DEF"), tmpA);
					else if ( physicsDefence < 0.0f )
						sprintf(tmpL, "%s |cffff0000%s|r", g_ObjectData->GetString("SYS_ARMOR_DEF"), tmpA);
					else
						sprintf(tmpL, "%s %s", g_ObjectData->GetString("SYS_ARMOR_DEF"), tmpA);

					// Å]ªk¨¾¿m¤O
					value[0] = abilityBase.Pop(EM_WearEqType_MDEF);													// base
					value[1] = abilityRefine.Pop(EM_WearEqType_MDEF);												// extra

					switch (itemDB->Item.ArmorPos)
					{
					case EM_ArmorPos_Necklace:	//¶µÁå 
					case EM_ArmorPos_Ring:		//§Ù¤l
					case EM_ArmorPos_Earring:	//¦Õ¹¢
						magicDefence = value[0] + value[1];
						break;

					default:
						magicDefence = value[1];
						break;
					}

					//value[1] = abilityRefine.Pop(EM_WearEqType_MDEF) + abilityInherent.Pop(EM_WearEqType_MDEF);		// extra
					ObjectDataClass::IntegerString(value[0] + value[1] + fAddMDef, tmpA, 512);
					colorR = g_whiteColor;
					if ( magicDefence > 0.0f )
						sprintf(tmpR, "%s |cff00ff00%s|r", g_ObjectData->GetString("SYS_ARMOR_MDEF"), tmpA);
					else if ( magicDefence < 0.0f )
						sprintf(tmpR, "%s |cffff0000%s|r", g_ObjectData->GetString("SYS_ARMOR_MDEF"), tmpA);
					else
						sprintf(tmpR, "%s %s", g_ObjectData->GetString("SYS_ARMOR_MDEF"), tmpA);
					tooltip->AddDoubleLine(tmpL, tmpR, colorL.m_r, colorL.m_g, colorL.m_b, colorR.m_r, colorR.m_g, colorR.m_b);
				}				

				// ª««~»ù­È
				int price = (itemDB->Cost + 5) / 10;
				if ( price > 0 && itemDB->Mode.Sell )
				{					
					g_ObjectData->GetMoneyText(price, tmpR, 512);
					sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_ITEM_COST"), tmpR);
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				if ( itemDB->Mode.Sell == false )
				{
					tooltip->AddLine(g_ObjectData->GetString("SYS_CANT_SELL"), 1.0f, 1.0f, 1.0f);
				}

				// ­×²z¶O¥Î
				if ( item && g_pStoreFrame->GetRepairState() )
				{
					int repairMoney = RoleDataEx::CalRepairWeapon(item, NULL);
					if ( repairMoney < 0 )
					{
						g_ObjectData->GetMoneyText(-repairMoney, tmpR, 512);
						sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_REPAIR_COST"), tmpR);
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 0.0f);
					}
				}

				// °ò¥»¯à¤O + ½Äµ¥¯à¤O
				abilityBase.Insert(abilityRefine);
				while ( abilityBase.Pop(wearType, wearValue) )
				{
					TooltipAddWearEqTypeText(tooltip, wearType, wearValue);
				}

				// ªþ¥[¯à¤O
				if( item )
				{
					// Ä~©Ó¯à¤O
					for ( int i = 0; i < 6; i++ )
					{
						GameObjDbStructEx* inherent = CNetGlobal::GetObj(item->Inherent(i));
						if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
						{
							// ¯à¤O
							TooltipAddAbilityText(tooltip, inherent->Attirbute, inherent->GetName(), durableScale);
						}
					}

					// ²Å¤å¯à¤O
					int runeVolume = RoleDataEx::GetRuneTotalVolume(item);
					if ( runeVolume > 0 )
					{
						sprintf( tmpL, "%s (%d/%d)", g_ObjectData->GetString("SYS_ITEM_RUNE"), runeVolume - item->RuneVolume, runeVolume );
						tooltip->AddLine(tmpL, 0.74f, 0.18f, 1.0f);

						// ²Å¤å¯à¤O»¡©ú
						TooltipAddRuneAbilityText(tooltip, item, durableScale);
					}
				}
				else
				{
					TooltipAddDropAbility(tooltip, item, itemDB);
				}

				// ª««~¸Ô²Ó»¡©ú
				if ( itemDB->ShortNote[0] )
				{
					tooltip->AddLine( ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f );
				}				

				// ®M¸Ë¦Cªí
				if ( itemDB->Item.SuitID != 0 )
				{
					tooltip->AddSeparator();

					GameObjDbStructEx* suit = CNetGlobal::GetObj(itemDB->Item.SuitID);
					if ( suit && suit->Classification == EM_ObjectClass_Suit )
						TooltipAddSuitAbilityText(tooltip, suit);
				}
			}
			break;

		case EM_Item_Rune:
			{
				// ¹ï¶H¥Ø¼Ðµ¥¯Å
				sprintf(tmpL, g_ObjectData->GetString("SYS_TOOLTIP_RUNE_LEVEL"), itemDB->Item.Level + 1);
				tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);

				// ¨Ï¥Î­­¨î,µ¥¯Å,Â¾·~,ÄÝ©Ê,©Ê§O
				//TooltipAddLimitText(tooltip, itemDB->Limit);

				// Ãþ«¬¤Î®e¶q
				memset( tmpL, 0, sizeof(tmpL) );
				switch ( itemDB->Item.RuneType )
				{
				case EM_RuneType_Manufacture:
					strcat( tmpL, g_ObjectData->GetString("RUNE_CRAFT") );
					break;
				case EM_RuneType_Weapon:
					strcat( tmpL, g_ObjectData->GetString("RUNE_WEAPON") );
					break;
				case EM_RuneType_Armor:
					strcat( tmpL, g_ObjectData->GetString("RUNE_ARMOR") );
					break;
				case EM_RuneType_Head:
					strcat( tmpL, g_ObjectData->GetString("RUNE_HEAD") );
					break;
				case EM_RuneType_Body:
					strcat( tmpL, g_ObjectData->GetString("RUNE_CHEST") );
					break;
				case EM_RuneType_Pant:
					strcat( tmpL, g_ObjectData->GetString("RUNE_LEG") );
					break;
				case EM_RuneType_Special:
					strcat( tmpL, g_ObjectData->GetString("RUNE_SPECIAL") );
					break;
				case EM_RuneType_Ornament:
					strcat( tmpL, g_ObjectData->GetString("RUNE_ORNAMENT") );
					break;
				case EM_RuneType_Shield:
					strcat( tmpL, g_ObjectData->GetString("RUNE_SHIELD") );
					break;
				case EM_RuneType_MagicTools:
					strcat( tmpL, g_ObjectData->GetString("RUNE_MAGICTOOLS") );
					break;
				}
				tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				/*
				®e¶q¤£Åã¥Ü
				sprintf( tmpR, "%s %d", g_ObjectData->GetString("SYS_RUNE_VOLUME"), itemDB->Item.Volume );
				tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				*/

				// ª««~»ù­È
				int price = (itemDB->Cost + 5) / 10;
				if ( price > 0 )
				{					
					g_ObjectData->GetMoneyText(price, tmpR, 512);
					sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_ITEM_COST"), tmpR);
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}

				// ¯à¤O
				for ( int i = 0; i < Max_WearEqType_Count; i++)
					TooltipAddWearEqTypeText(tooltip, itemDB->Item.Ability.EqType[i], (float)itemDB->Item.Ability.EqTypeValue[i]);

				// ª««~¸Ô²Ó»¡©ú
				if ( itemDB->ShortNote[0] )
				{
					tooltip->AddLine( ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f );
				}
			}
			break;

		case EM_Item_Card:
			// ¯à¤O
			if ( itemDB->Item.CardAddpower != 0 )
			{
				// ªþ¥[¯à¤O
				GameObjDbStructEx* ability = CNetGlobal::GetObj(itemDB->Item.CardAddpower);
				if ( ability && ability->Classification == EM_ObjectClass_Attribute )
				{
					const char* abilityStr = g_ObjectData->GetString("SYS_CARD_ABILITY");
					if ( abilityStr && abilityStr[0] )
					{
						tooltip->AddLine(abilityStr, 1.0f, 1.0f, 1.0f);
					}

					TooltipAddAbilityText(tooltip, ability->Attirbute, ability->GetName(), 1.0f);
				}
			}

			// ª««~¸Ô²Ó»¡©ú
			if ( itemDB->ShortNote[0] )
			{
				tooltip->AddLine( ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f);
			}
			break;
		}
		break;
	case EM_ObjectClass_KeyItem: // ­«­nª««~, ³q±`¬O¯S®í GUID ¦p Attack
		{
			/*
			if ( itemDB->GUID == DF_SKILLID_ATTACK || itemDB->GUID == DF_SKILLID_PORTOL || itemDB->GUID == DF_SKILLID_DISENCHANT )
			{
			tooltip->AddLine(itemDB->Name, 1.0f, 1.0f, 1.0f);
			tooltip->AddLine(itemDB->ShortNote, 0.0f, 0.75f, 0.95f);
			}
			*/

			tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);
			tooltip->AddLine(ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f);

		} break;

	case EM_ObjectClass_Title: // ºÙ¸¹
		{
			//tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);
			//tooltip->AddLine(ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f);


			char tmp[MAX_PATH];
			//char text[MAX_PATH] = "";

			GameObjDbStructEx* pTitleObj = itemDB;
			BaseRoleData* pBaseData = &CNetGlobal::RoleData()->BaseData;
			TempRoleData* pTempData = &CNetGlobal::RoleData()->TempData;

			string strName;
			if( g_GameTooltip_IsSetTitle )
			{
				int index = pTitleObj->GUID - Def_ObjectClass_Title;
				if( CNetGlobal::RoleData()->PlayerBaseData->Title.GetFlag(index) )
				{
					strName = pTitleObj->GetName();
				}
				else
				{
					strName = g_ObjectData->GetString( "SYS_TITLE_UNGET" );
				}
			}
			else
			{
				strName = pTitleObj->GetName();
			}

			tooltip->AddLine( strName.c_str() , 1 , 1 , 1 );

			// ªþ¥[¯à¤O
			GameObjectWearEqStruct& Ability = pTitleObj->Title.Ability;
			for ( int i = 0; i < Max_WearEqType_Count; i++ )
				TooltipAddWearEqTypeText(tooltip, Ability.EqType[i], (float)Ability.EqTypeValue[i]);

			// ªþ¥[§Þ¯à
			GameObjDbStructEx* pSkillObj = g_ObjectData->GetObj( pTitleObj->Title.SkillID );
			if( pSkillObj )
			{
				string strSkillName;
				if( pTitleObj->Title.SkillLv > 0 )
				{
					strSkillName = Com_FormatString( "%s+%d", pSkillObj->Name, pTitleObj->Title.SkillLv );
				}
				else
				{
					strSkillName = pSkillObj->Name;
				}

				string strTemp = g_ObjectData->GetString( "TITLE_TIP_SKILL_FORMAT" );
				strTemp = ReplaceKeyString( strTemp, "<skill>", strSkillName.c_str() );

				tooltip->AddLine( strTemp.c_str() , 1 , 1 , 1 );
			}

			//­­¨î
			if( pTitleObj->Title.Type == EM_TitleNameType_Famous )
			{
				//if( pTitleObj->Title.Famous > pBaseData->Famous )
				//{
				//	sprintf(tmp, "Famous |cffff2020%d|r", pTitleObj->Title.Famous );
				//}
				//else
				//{	
				//	sprintf(tmp, "Famous ", pTitleObj->Title.Famous );
				//}
				sprintf(tmp, "Famous ", pTitleObj->Title.Famous );
			}
			else if( pTitleObj->Title.Type == EM_TitleNameType_Job )
			{
				//¥DÂ¾
				if( pTitleObj->Title.MainJob != EM_Vocation_None )
				{
					const char* className = g_ObjectData->GetClassNameByID( pTitleObj->Title.MainJob );

					if( pTitleObj->Title.MainJob != pBaseData->Voc )
					{
						sprintf(tmp, "|cffff2020%s Lv%d|r", className , pTitleObj->Title.MainJobLV );				
					}
					else
					{
						if( pTitleObj->Title.MainJobLV > pTempData->Attr.Level )
						{
							sprintf(tmp, "%s |cffff2020Lv%d|r", className , pTitleObj->Title.MainJobLV );
						}
						else
						{
							sprintf(tmp, "%s Lv%d", className , pTitleObj->Title.MainJobLV );
						}
					}


					//°ÆÂ¾
					if( pTitleObj->Title.SubJob != EM_Vocation_None )
					{
						const char* className = g_ObjectData->GetClassNameByID( pTitleObj->Title.SubJob );

						if( pTitleObj->Title.SubJob != pBaseData->Voc_Sub )
						{
							sprintf(tmp, "%s , |cffff2020%s Lv%d|r", tmp , className , pTitleObj->Title.SubJobLV );				
						}
						else
						{
							if( pTitleObj->Title.SubJobLV > pTempData->Attr.Level_Sub )
							{
								sprintf(tmp, "%s , %s |cffff2020Lv%d|r", tmp , className , pTitleObj->Title.SubJobLV );
							}
							else
							{
								sprintf(tmp, "%s , %s Lv%d", tmp , className , pTitleObj->Title.SubJobLV );
							}
						}
					}

					tooltip->AddLine( tmp , 1 , 1 , 1 );
				}
			}

			if( pTitleObj->ShortNote && strlen( pTitleObj->ShortNote ) > 0 ) 
			{
				string strNote = CNetCli_Script_Child::ReplaceSystemKeyword( pTitleObj->ShortNote );
				tooltip->AddLine( strNote.c_str() , 1 , 1 , 1 );
			}
		
  		} break;
	case EM_ObjectClass_QuestDetail: // ¥ô°È
		{
			tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);
			//tooltip->AddLine(ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 0.75f, 0.95f);
			GameObjectQuestDetail *pQuest = &( itemDB->QuestDetail );

			string sString = g_pQuestDetail->ReplaceKeyWord( pQuest->szQuest_Desc );

			tooltip->AddLine( sString.c_str(), 1.0f, 1.0f, 1.0f );
		} break;

	case EM_ObjectClass_MagicCollect:
		{
			RoleDataEx* pRoleDataEx = CNetGlobal::RoleData();

			// ¦WºÙ
			//int skillPoint = (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID));
			int skillPoint = g_GameTooltip_SkillPoint < 0 ? (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID)) : g_GameTooltip_SkillPoint ;

			if ( skillPoint > 0 )
				sprintf(tmpL, "%s+%d", itemDB->GetName(), skillPoint);
			else
				strcpy(tmpL, itemDB->GetName());

			// Rank(¥Ñ1¶}©l)
			if( itemDB->MagicCol.MagicLV > 0 )
			{
				// GameTooltipHyperLink ¥k¤W¨¤·|¦h1­ÓÃö³¬«ö¯Ã
				if( strcmp( tooltip->GetName() , "GameTooltipHyperLink" ) == 0 )
				{
					sprintf_s(tmpR, 512, "%s%d   ", g_ObjectData->GetString("SYS_MAGIC_LEVEL"), itemDB->MagicCol.MagicLV);
				}
				else
				{
					sprintf_s(tmpR, 512, "%s%d", g_ObjectData->GetString("SYS_MAGIC_LEVEL"), itemDB->MagicCol.MagicLV);
				}
			}
			else
			{
				tmpR[0] = '\0';
			}


			// §Þ¯à¦WºÙ¥H¤ÎÃC¦â
			switch ( itemDB->MagicCol.EffectType )
			{
				// ¥D°Ê§Þ¯à
			case EM_Magic_Magic:
			case EM_Magic_Phy:
				if ( itemDB->MagicCol.JobInfo.IsJobLimit )
					tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 0.96f, 0.4f, 0.99f, 0.8f, 0.23f);
				else
					tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 0.99f, 0.8f, 0.23f);
				break;

				// ³Q°Ê§Þ¯à
			case EM_Magic_Eq:
				tooltip->AddDoubleLine(tmpL, tmpR, 0.19f, 0.57f, 0.4f, 0.99f, 0.8f, 0.23f);				
				break;
			}

			for ( int i = 0; i < 2; i++ )
			{
				if ( itemDB->MagicCol.Need[i].Type == EM_MagicSpellNeedType_Self_MainJob )
				{
					itemDB->MagicCol.JobInfo.Job = (Voc_ENUM)itemDB->MagicCol.Need[i].Value;
					itemDB->MagicCol.JobInfo.IsJobLimit = true;
				}
			}			

			if ( itemDB->MagicCol.JobInfo.Job != EM_Vocation_None && g_GameTooltipMagicColSetting.HideJob == false )
			{
				colorL.SetColor(0xFFFFFFFF);
				colorR.SetColor(0xFFFFFFFF);

				// Â¾·~¸ò±MÄÝ§Þ¯à§P§O
				if ( itemDB->MagicCol.JobInfo.IsJobLimit )
				{
					strcpy(tmpR, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_CLASSONLY"));
					if ( pRoleDataEx->TempData.Attr.Voc != itemDB->MagicCol.JobInfo.Job )
						colorL.SetColor(0xFFFF0000);
					else
						colorL.SetColor(0xFFFFFFFF);
				}
				else
				{
					strcpy(tmpR, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_NORMAL"));
					if ( pRoleDataEx->TempData.Attr.Voc != itemDB->MagicCol.JobInfo.Job && pRoleDataEx->TempData.Attr.Voc_Sub != itemDB->MagicCol.JobInfo.Job )
						colorL.SetColor(0xFFFF0000);
					else
						colorL.SetColor(0xFFFFFFFF);
				}
				sprintf(tmpL, "%s %s", g_ObjectData->GetClassNameByID(itemDB->MagicCol.JobInfo.Job), tmpR );

				memset(tmpR, 0, sizeof(tmpR));
				if ( itemDB->MagicCol.LimitLv > 1 && g_GameTooltipMagicColSetting.HideUseLevel == false )
				{
					sprintf_s(tmpR, 512, "%s %d", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_LIMITLV"), itemDB->MagicCol.LimitLv );
					if ( pRoleDataEx->TempData.Attr.Level < itemDB->MagicCol.LimitLv )
						colorR.SetColor(0xFFFF0000);
					else
						colorR.SetColor(0xFFFFFFFF);
				}

				if ( tmpR[0] )
					tooltip->AddDoubleLine(tmpL, tmpR, colorL.m_r, colorL.m_g, colorL.m_b, colorR.m_r, colorR.m_g, colorR.m_b);
				else
					tooltip->AddLine(tmpL, colorL.m_r, colorL.m_g, colorL.m_b);

				/*
				if( g_GameTooltip_SkillBookMode )
				{
					if ( tmpR[0] )
						tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					else
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				else if ( skillPoint == -1 )
				{
					if ( tmpR[0] )
						tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
					else
						tooltip->AddLine(tmpL, 1.0f, 0.0f, 0.0f);
				}
				else
				{
					if ( tmpR[0] )
						tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
					else
						tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				*/
			}
			else if ( itemDB->MagicCol.LimitLv > 1 && g_GameTooltipMagicColSetting.HideUseLevel == false )
			{
				sprintf_s(tmpL, 512, "%s %d", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_LIMITLV"), itemDB->MagicCol.LimitLv );
				tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
			}

			memset(tmpL, 0, sizeof(tmpL));	

			if ( itemDB->MagicCol.EffectType == EM_Magic_Magic || itemDB->MagicCol.EffectType == EM_Magic_Phy )
			{
				memset(tmpL, 0, sizeof(tmpL));
				memset(tmpR, 0, sizeof(tmpR));

				if ( g_GameTooltipMagicColSetting.HideCost == false ) 
				{
					// ®ø¯Ó
					sprintf_s(tmpL, 512, "%s ", g_ObjectData->GetString("SYS_TOOLTIPS_BURN"));
					GetMagicSpellCostStr( itemDB->MagicCol.Cost[0].Type , itemDB->MagicCol.Cost[0].Value , skillPoint , tmpR );
					strcat(tmpL, tmpR);
					if ( itemDB->MagicCol.Cost[1].Type != EM_MagicSpellCostType_None )
					{
						GetMagicSpellCostStr( itemDB->MagicCol.Cost[1].Type , itemDB->MagicCol.Cost[1].Value , skillPoint , tmpR );
						strcat(tmpL, ", ");
						strcat(tmpL, tmpR);
					}
					memset(tmpR, 0, sizeof(tmpR));
				}

				float AttackDistance = 0;
				if ( g_GameTooltipMagicColSetting.HideDistance == false )
				{
					if( itemDB->MagicCol.Flag.ReferenceWeapon_Bow )
					{
						AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Bow_AttackRange;
					}
					else if( itemDB->MagicCol.Flag.ReferenceWeapon_Throw )
					{
						AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Throw_AttackRange;
					}

					AttackDistance = float( itemDB->MagicCol.AttackDistance ) + AttackDistance;

					if( itemDB->MagicCol.TargetType == EM_Releation_Self )
						AttackDistance = 0;
				}

				// ¬Iªk¶ZÂ÷
				if ( AttackDistance > 0 )
					sprintf( tmpR, "%s %d", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_DISTANCE"), int( AttackDistance ) );

				if ( AttackDistance > 0 ||
					itemDB->MagicCol.Cost[0].Type != EM_MagicSpellCostType_None ||
					itemDB->MagicCol.Cost[1].Type != EM_MagicSpellCostType_None )
				{
					tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}

				// ¬Iªk®É¶¡
				memset(tmpL, 0, sizeof(tmpL));
				memset(tmpR, 0, sizeof(tmpR));
				float SpellTime = itemDB->MagicCol.SpellTime;
				if ( g_GameTooltipMagicColSetting.NotRoleSpellTime == false )
				{
					SpellTime = SpellTime * CNetGlobal::RoleData()->TempData.Attr.MagicSpellSpeed();
				}				
				if ( SpellTime > 0.1f )
					sprintf(tmpL, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_SPELL_TIME"), SpellTime);
				else
					strcpy(tmpL, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_BLINK"));

				// §NÓý®É¶¡				
				float coldownTime = (float)itemDB->MagicCol.Coldown.Time;
				if ( g_GameTooltipMagicColSetting.NotRoleColdown == false )
				{
					coldownTime = CNetGlobal::RoleData()->GetColdownTime((GameObjDbStructEx*)itemDB);
				}

				if ( coldownTime > 0.0f )
				{
					char timeText[512];
					int second = (int)coldownTime;
					if ( second > 86400 ) {	
						// ¤Ñ¼Æ
						if ( (second % 86400) / 3600 > 0 )
							sprintf_s(timeText, 512, "%d %s %d %s", second / 86400, g_ObjectData->GetString("SYS_DAY"), (second % 86400) / 3600, g_ObjectData->GetString("SYS_HOUR"));
						else
							sprintf_s(timeText, 512, "%d %s", second / 86400, g_ObjectData->GetString("SYS_DAY"));
					} else if ( second > 3600 ) {
						// ¤p®É
						if ( (second % 3600) / 60 > 0 )
							sprintf_s(timeText, 512, "%d %s %d %s", second / 3600, g_ObjectData->GetString("SYS_HOUR"), (second % 3600) / 60, g_ObjectData->GetString("SYS_MINUTE"));
						else
							sprintf_s(timeText, 512, "%d %s", second / 3600, g_ObjectData->GetString("SYS_HOUR"));						
					} else if ( second > 60 ) {
						// ¤ÀÄÁ
						if ( second % 60 > 0  )
							sprintf_s(timeText, 512, "%d %s %d %s", second / 60, g_ObjectData->GetString("SYS_MINUTE"), second % 60, g_ObjectData->GetString("SYS_SECOND"));
						else
							sprintf_s(timeText, 512, "%d %s", second / 60, g_ObjectData->GetString("SYS_MINUTE"));
					} else {
						// ¬í
						sprintf_s(timeText, 512, "%d %s", max(second, 0), g_ObjectData->GetString("SYS_SECOND"));
					}
					//g_ObjectData->SecondsToTimeAbbrev(coldownTime, timeText);
					sprintf(tmpR, "%s %s", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_COLDOWN_TIME"), timeText);
				}
				else
					memset( tmpR, 0, sizeof(tmpR) );

				if ( strlen(tmpL) > 0 || strlen(tmpR) > 0 )
				{
					if( strlen(tmpL) == 0 )
					{
						tmpL[0] = ' ';
						tmpL[1] = 0;
					}

					tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
				}
			}

			// ª««~¸Ô²Ó»¡©ú
			if ( itemDB->ShortNote[0] && g_GameTooltipMagicColSetting.HideNote == false )
			{
				tooltip->AddLine( ItemUTIL::GetMagicColNote(itemDB, skillPoint).c_str() , 0.0f, 0.75f, 0.95f);
			}		

			if( g_GameTooltip_SkillBookMode && !g_GameTooltip_Skill_Info.bLearned && itemDB->MagicCol.JobInfo.Job != EM_Vocation_None )
			{

				const char* szNeed = g_ObjectData->GetString("SYS_SKILL_NEED");
		
				if( ( itemDB->MagicCol.JobInfo.Job == pRoleDataEx->BaseData.Voc && g_GameTooltip_Skill_Info.LearnLV <= pRoleDataEx->Level() ) ||
					( itemDB->MagicCol.JobInfo.Job == pRoleDataEx->BaseData.Voc_Sub && g_GameTooltip_Skill_Info.LearnLV <= pRoleDataEx->Level_Sub() ) )
				{

				}
				else
				{
					sprintf(tmpL, "%s %s lv%d", 
						szNeed,
						g_ObjectData->GetClassNameByID(itemDB->MagicCol.JobInfo.Job), 
						g_GameTooltip_Skill_Info.LearnLV );

					tooltip->AddLine(tmpL, 1.0f, 0.0f, 0.0f);
				}
	
				int NeedMagicIndex;

				for( NeedMagicIndex = 0 ; NeedMagicIndex < 2 ; ++NeedMagicIndex )
				{
					if( g_GameTooltip_Skill_Info.NeedMagicLv[NeedMagicIndex] <= 0 )
						continue;

					if( g_GameTooltip_Skill_Info.NeedMagicID[NeedMagicIndex] <= 0 )
						continue;

					GameObjDbStructEx* pSkillObj = g_ObjectData->GetObj( g_GameTooltip_Skill_Info.NeedMagicID[NeedMagicIndex] );
					if( NULL == pSkillObj )
						continue;

					if( pSkillObj->MagicCol.MagicLV > 0 )
					{
						sprintf(tmpL, "%s %s(%s%d) +%d", 
							szNeed, 
							pSkillObj->GetName(),  
							g_ObjectData->GetString("SYS_MAGIC_LEVEL"),
							pSkillObj->MagicCol.MagicLV, 
							g_GameTooltip_Skill_Info.NeedMagicLv[NeedMagicIndex] );
					}
					else
					{
						sprintf(tmpL, "%s %s +%d", 
							szNeed, 
							pSkillObj->GetName(),  
							g_GameTooltip_Skill_Info.NeedMagicLv[NeedMagicIndex] );
					}
					
					tooltip->AddLine(tmpL, 1.0f, 0.0f, 0.0f);
				}
			}

		}
		break;

	case EM_ObjectClass_MagicBase:
		{
			//int skillPoint = (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID));			

			// »Ý­n­«·s§ó·s®É¶¡
			needUpdateFrame = true;

			int powerLv = g_GameTooltip_SkillPoint;
			if ( powerLv < 0 )
				powerLv = (int)(CNetGlobal::RoleData()->GetBuffPowerLv(itemDB->GUID));

			// ¦WºÙ
			tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);

			// BuffÃþ«¬
			if ( itemDB->MagicBase.MagicFunc == EM_MAGICFUN_Assist )
			{
				char temp[1024];
				bool boFirst = true;

				memset(temp, 0, 1024);
				for (int i = 0; i < 32; i++ )
				{
					if ( (itemDB->MagicBase.MagicGroupSet >> i) & 0x01 )
					{
						char buf[256];
						sprintf_s(buf, 256, "SYS_MAGIC_TYPE%02d", i + 1);

						if ( boFirst )
						{
							strcpy(temp, g_ObjectData->GetString(buf));
						}
						else
						{
							strcat(temp, " ");
							strcat(temp, g_ObjectData->GetString(buf));
						}
						boFirst = false;
					}
				}

				if ( strlen(temp) > 0 )
					tooltip->AddLine(temp, 1.0f, 1.0f, 1.0f);
			}

			// Buff³Ñ¾l®É¶¡
			if ( g_gameTooltipBuffTime > 0.0f && g_gameTooltipBuffTime < 3153600.0f )
			{
				int second = (int)g_gameTooltipBuffTime;

				strcpy(tmpL, g_ObjectData->GetString("SYS_REMAINTIME"));

				if ( second > 86400 ) {	
					// ¤Ñ¼Æ
					if ( (second % 86400) / 3600 > 0 )
						sprintf_s(tmpR, 512, "%d %s %d %s", second / 86400, g_ObjectData->GetString("SYS_DAY"), (second % 86400) / 3600, g_ObjectData->GetString("SYS_HOUR"));
					else
						sprintf_s(tmpR, 512, "%d %s", second / 86400, g_ObjectData->GetString("SYS_DAY"));
				} else if ( second > 3600 ) {
					// ¤p®É
					if ( (second % 3600) / 60 > 0 )
						sprintf_s(tmpR, 512, "%d %s %d %s", second / 3600, g_ObjectData->GetString("SYS_HOUR"), (second % 3600) / 60, g_ObjectData->GetString("SYS_MINUTE"));
					else
						sprintf_s(tmpR, 512, "%d %s", second / 3600, g_ObjectData->GetString("SYS_HOUR"));						
				} else if ( second > 60 ) {
					// ¤ÀÄÁ
					if ( second % 60 > 0  )
						sprintf_s(tmpR, 512, "%d %s %d %s", second / 60, g_ObjectData->GetString("SYS_MINUTE"), second % 60, g_ObjectData->GetString("SYS_SECOND"));
					else
						sprintf_s(tmpR, 512, "%d %s", second / 60, g_ObjectData->GetString("SYS_MINUTE"));
				} else {
					// ¬í
					sprintf_s(tmpR, 512, "%d %s", max(second, 0), g_ObjectData->GetString("SYS_SECOND"));
				}
				strcat(tmpL, tmpR);
				tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
			}

			tooltip->AddLine(GetMagicBaseNote(itemDB, powerLv).c_str(), 1.0f, 1.0f, 1.0f);
			//tooltip->AddLine( GetMagicBaseNote( itemDB , powerLv ).c_str() , 1.0f, 1.0f, 1.0f);
		}
		break;

	case EM_ObjectClass_Recipe:
		{
			// µ}¦³«×
			color = g_ObjectData->GetItemQualityColor(g_ObjectData->GetItemQuality(itemDB, item));

			// ¦WºÙ
			tooltip->AddLine(itemDB->GetName(), color.m_r, color.m_g, color.m_b);

			// ¤w¸g¾Ç·|¸Ó°t¤è
			if ( CNetGlobal::RoleData()->CheckKeyItem(itemDB->Recipe.Request.KeyItemID) )
			{
				tooltip->AddLine(g_ObjectData->GetString("SYS_ALREADY_LEARN"), 1.0f, 0.0f, 0.0f);
			}

			if( itemDB->Recipe.Request.Skill != EM_SkillValueType_None )
			{
				int requestCount = 0;
				/*
				int SkillLV = int( CNetGlobal::RoleData()->TempData.Attr.Fin.SkillValue.GetSkill( itemDB->Recipe.Request.Skill ) );				
				switch( itemDB->Recipe.Request.Skill )
				{
				case EM_SkillValueType_BlackSmith:		// ¥´ÅK
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_BLACKSMITH");
					break;
				case EM_SkillValueType_Carpenter:		// ¤ì¤u
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_CARPENTER");
					break;
				case EM_SkillValueType_MakeArmor:		// »s¥Ò
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_MAKEARMOR");
					break;
				case EM_SkillValueType_Tailor:			// µôÁ_
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_TAILOR");
					break;
				case EM_SkillValueType_Cook:			// ²i¶¹
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_COOK");
					break;
				case EM_SkillValueType_Alchemy:			// ·Òª÷
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_ALCHEMY");
					break;
				case EM_SkillValueType_Mining:			// «õÄq
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_MINING");
					break;
				case EM_SkillValueType_Lumbering:		// ¬å§÷
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_LUMBERING");
					break;
				case EM_SkillValueType_Herblism:		// ±ÄÃÄ
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_HERBLISM");
					break;
				case EM_SkillValueType_Fishing:			// ³¨³½
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_FISHING");
					break;
				case EM_SkillValueType_Plant:			// ºØ´Ó
					leftText = g_ObjectData->GetString("SYS_SKILLNAME_PLANT");
					break;
				default:
					leftText = "";
				}				

				if( leftText != NULL )
				{
					if( SkillLV >= itemDB->Recipe.Request.SkillLV )
						sprintf( tmpL, "%s %s %s %d", g_ObjectData->GetString("TOOLTIPS_LIMIT_USE"), leftText, g_ObjectData->GetString("SYS_LEVEL"), itemDB->Recipe.Request.SkillLV );
					else
						sprintf( tmpL, "%s |cffff2020%s %s %d", g_ObjectData->GetString("TOOLTIPS_LIMIT_USE"), leftText, g_ObjectData->GetString("SYS_LEVEL"), itemDB->Recipe.Request.SkillLV );
				}
				tooltip->AddLine( tmpL , 1.0f, 1.0f, 1.0f);
				*/

				sprintf(tmpL, "%s ", g_ObjectData->GetString("SYS_RECIPE_REQUEST_STUFF"));
				for ( int i = 0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
				{
					GameObjDbStructEx* stuff = g_ObjectData->GetObj(itemDB->Recipe.Source[i].OrgObjID);
					if ( stuff )
					{
						if ( requestCount > 0 )
							strcat(tmpL, ", ");
						sprintf(tmpR, "%s*%d", stuff->GetName(), max(itemDB->Recipe.Source[i].Count, 1));
						strcat(tmpL, tmpR);
						requestCount++;
					}
				}
				tooltip->AddLine(tmpL , 1.0f, 1.0f, 1.0f);

				// ª««~»ù­È
				int price = (itemDB->Cost + 5) / 10;
				if ( price > 0 && itemDB->Mode.Sell )
				{					
					g_ObjectData->GetMoneyText(price, tmpR, 512);
					sprintf_s(tmpL, 512, "%s%s", g_ObjectData->GetString("SYS_ITEM_COST"), tmpR);
					tooltip->AddLine(tmpL, 1.0f, 1.0f, 1.0f);
				}
				if ( itemDB->Mode.Sell == false )
				{
					tooltip->AddLine(g_ObjectData->GetString("SYS_CANT_SELL"), 1.0f, 1.0f, 1.0f);
				}

				// Åã¥Üª««~¼Æ­È
				{
					GameObjDbStructEx* recipeItemDB = g_ObjectData->GetObj(itemDB->Recipe.ItemAll[0].OrgObjID);
					if ( recipeItemDB )
					{
						tooltip->AddLine(" ", 0.0f, 0.0f, 0.0f);
						_TooltipSetItem(tooltip, NULL, recipeItemDB, false);
					}
				}
			}
		}
		break;
	}

	if ( item && item->GetCount() > 1 ) {
		tooltip->AddLine(g_ObjectData->GetString("SYS_TOOLTIP_SEPARATE_TEXT"), 1.0f, 1.0f, 1.0f);
	}

	if ( strcmp(tooltip->GetName(), "GameTooltip") == 0 )
	{
		g_pGameTooltip->SetNeedUpdateTime(needUpdateFrame);
	}

	return true;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetExtraActionItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();

	ExtraActionItem* item = g_pActionFrame->GetExtraItem(luaL_checkint(L, 2) - 1);
	if ( item )
	{		
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->skillId);
		if ( itemDB ) {
			g_GameTooltip_SkillBookMode = true;
			g_GameTooltip_Skill_Info.bLearned = true;
			g_GameTooltip_SkillPoint = 0;
			bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, NULL, itemDB);
			tooltip->SetVisible( visible );
			g_GameTooltip_SkillPoint = -1;
			g_GameTooltip_SkillBookMode = false;
			g_GameTooltip_Skill_Info.bLearned = false;
		}
	}
	return  0;
}

// ----------------------------------------------------------------------------------
void TooltipAddPetSkillName(CUiTooltip* tooltip, GameObjDbStructEx* itemDB)
{
	char tmpL[512];
	char tmpR[512];
	int count ;
	if ( tooltip == NULL || itemDB == NULL )
		return;

	// ¦WºÙ
	int skillPoint = g_GameTooltip_SkillPoint;
	if ( skillPoint > 0 )
	{
		count = skillPoint + 1 ;
		sprintf_s( tmpL, "%s+%d", itemDB->GetName(), count );
	}
	else
	{
		sprintf_s( tmpL, "%s+%d", itemDB->GetName(), 1 );
		//strcpy(tmpL, itemDB->GetName() );
	}
	// Rank(¥Ñ1¶}©l)
	if( itemDB->MagicCol.MagicLV > 0 )
	{
		// GameTooltipHyperLink ¥k¤W¨¤·|¦h1­ÓÃö³¬«ö¯Ã
		if( strcmp( tooltip->GetName() , "GameTooltipHyperLink" ) == 0 )
		{
			sprintf_s(tmpR, 512, "%s%d   ", g_ObjectData->GetString("SYS_MAGIC_LEVEL"), itemDB->MagicCol.MagicLV);
		}
		else
		{
			sprintf_s(tmpR, 512, "%s%d", g_ObjectData->GetString("SYS_MAGIC_LEVEL"), itemDB->MagicCol.MagicLV);
		}
	}
	else
	{
		tmpR[0] = '\0';
	}

	// §Þ¯à¦WºÙ¥H¤ÎÃC¦â
	switch ( itemDB->MagicCol.EffectType )
	{
		// ¥D°Ê§Þ¯à
	case EM_Magic_Magic:
	case EM_Magic_Phy:
		if ( itemDB->MagicCol.JobInfo.IsJobLimit )
			tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 0.96f, 0.4f, 0.99f, 0.8f, 0.23f);
		else
			tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 0.99f, 0.8f, 0.23f);
		break;

		// ³Q°Ê§Þ¯à
	case EM_Magic_Eq:
		tooltip->AddDoubleLine(tmpL, tmpR, 0.19f, 0.57f, 0.4f, 0.99f, 0.8f, 0.23f);
		break;
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddPetSkill(CUiTooltip* tooltip, GameObjDbStructEx* itemDB, bool bNextLevelGray)
{
	char tmpL[512];
	char tmpR[512];

	if ( tooltip == NULL || itemDB == NULL )
		return;

	int skillPoint = g_GameTooltip_SkillPoint;

	memset(tmpL, 0, sizeof(tmpL));
	memset(tmpR, 0, sizeof(tmpR));
	if ( itemDB->MagicCol.EffectType == EM_Magic_Magic || itemDB->MagicCol.EffectType == EM_Magic_Phy )
	{
		// ®ø¯Ó
		sprintf_s(tmpL, 512, "%s ", g_ObjectData->GetString("SYS_TOOLTIPS_BURN"));
		GetMagicSpellCostStr( itemDB->MagicCol.Cost[0].Type , itemDB->MagicCol.Cost[0].Value , skillPoint , tmpR );
		strcat(tmpL, tmpR);
		if ( itemDB->MagicCol.Cost[1].Type != EM_MagicSpellCostType_None )
		{
			GetMagicSpellCostStr( itemDB->MagicCol.Cost[1].Type , itemDB->MagicCol.Cost[1].Value , skillPoint , tmpR );
			strcat(tmpL, ", ");
			strcat(tmpL, tmpR);
		}
		memset(tmpR, 0, sizeof(tmpR));

		float	AttackDistance = 0;

		if( itemDB->MagicCol.Flag.ReferenceWeapon_Bow )
		{
			AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Bow_AttackRange;
		}
		else if( itemDB->MagicCol.Flag.ReferenceWeapon_Throw )
		{
			AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Throw_AttackRange;
		}

		AttackDistance = float( itemDB->MagicCol.AttackDistance ) + AttackDistance;

		if( itemDB->MagicCol.TargetType == EM_Releation_Self )
			AttackDistance = 0;

		// ¬Iªk¶ZÂ÷
		if ( AttackDistance > 0 )
			sprintf( tmpR, "%s %d", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_DISTANCE"), int( AttackDistance ) );

		if ( AttackDistance > 0 ||
			itemDB->MagicCol.Cost[0].Type != EM_MagicSpellCostType_None ||
			itemDB->MagicCol.Cost[1].Type != EM_MagicSpellCostType_None )
		{
			tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		float SpellTime = itemDB->MagicCol.SpellTime;
		// SpellTime = SpellTime * CNetGlobal::RoleData()->TempData.Attr.MagicSpellSpeed();

		// ¬Iªk®É¶¡
		if ( SpellTime > 0.1f )
			sprintf(tmpL, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_SPELL_TIME"), SpellTime);
		else
			strcpy(tmpL, g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_BLINK"));

		// §Nƒj®É¶¡
		float coldownTime = CNetGlobal::RoleData()->GetColdownTime((GameObjDbStructEx*)itemDB);
		if ( coldownTime > 0.0f )
		{
			char timeText[256];
			g_ObjectData->SecondsToTimeAbbrev(coldownTime, timeText);
			sprintf(tmpR, "%s %s", g_ObjectData->GetString("SYS_TOOLTIP_MAGIC_COLDOWN_TIME"), timeText);
		}
		else
			memset( tmpR, 0, sizeof(tmpR) );

		if ( strlen(tmpL) > 0 || strlen(tmpR) > 0 )
		{
			if( strlen(tmpL) == 0 )
			{
				tmpL[0] = ' ';
				tmpL[1] = 0;
			}

			tooltip->AddDoubleLine(tmpL, tmpR, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		}

	}

	// ª««~¸Ô²Ó»¡©ú
	if ( itemDB->ShortNote[0] )
	{
		if( bNextLevelGray )
			tooltip->AddLine(ItemUTIL:: GetMagicColNote( itemDB , skillPoint ).c_str() , 0.5f, 0.5f, 0.5f);
		else
			tooltip->AddLine(ItemUTIL:: GetMagicColNote( itemDB , skillPoint ).c_str() , 0.0f, 0.75f, 0.95f);
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddLimitText(CUiTooltip* tooltip, ItemFieldStruct* item, LimitStruct& limit, bool compareSelf)
{
	char tmp[MAX_PATH];
	char text[MAX_PATH];
	bool haveLimit = false;	

	sprintf(text, "%s ", g_ObjectData->GetString("TOOLTIP_LIMIT_TEXT"));

	// µ¥¯Å
	if ( limit.Level > 1 )
	{		
		const char* levelStr = g_ObjectData->GetString("SYS_LEVEL");		
		if ( CNetGlobal::RoleData()->TempData.Attr.Level < limit.Level && compareSelf )
			sprintf(tmp, "|cffff2020%s %d|r", levelStr, limit.Level);
		else
			sprintf(tmp, "%s %d", levelStr, limit.Level);

		strcat(text, tmp);
		haveLimit = true;
	}

	// Â¾·~
	if ( limit.Voc._Voc != -1 )
	{
		bool canWear = false;

		// ÀË¬d¥DÂ¾¬O§_²Å¦X±ø¥ó
		if ( !compareSelf )
			canWear = true;
		else
		{
			for ( int i = 0; i < 16; i++ )
			{
				if ( limit.Voc._Voc & (0x01 << i) && CNetGlobal::RoleData()->TempData.Attr.Voc == i )
					canWear = true;
			}
		}

		for ( int i = 0; i < 16; i++ )
		{
			if ( limit.Voc._Voc & (0x01 << i) )
			{
				const char* className = g_ObjectData->GetClassNameByID((Voc_ENUM)i);

				if ( haveLimit )
					strcat(text, ", ");

				if ( canWear )
					sprintf(tmp, "%s", className);
				else
					sprintf(tmp, "|cffff2020%s|r", className);

				strcat(text, tmp);
				haveLimit = true;
			}
		}
	}

	SexFlagStruct sex = limit.Sex;

	// ÀË¬d¬O§_¦³¹Ï§Î¤W­­¨î
	if ( item )
	{
		GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->ImageObjectID);
		if ( itemDB )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB )
			{
				if ( imageDB->Image.LimitSex._Sex != 0 && imageDB->Image.LimitSex._Sex != -1 )
				{
					sex.Boy &= imageDB->Image.LimitSex.Boy;
					sex.Girl &= imageDB->Image.LimitSex.Girl;
				}
			}
		}
	}

	// ©Ê§O
	if ( sex._Sex != 0 && sex._Sex != -1 )
	{
		bool canWear = false;
		if ( !compareSelf )
			canWear = true;
		else
		{
			switch ( CNetGlobal::RoleData()->BaseData.Sex )
			{
			case EM_Sex_Boy:
				if ( sex.Boy )
					canWear = true;
				break;

			case EM_Sex_Girl:
				if ( sex.Girl )
					canWear = true;
				break;
			}
		}

		if ( sex.Boy )
		{
			if ( haveLimit )
				strcat(text, ", ");

			if ( canWear )
				sprintf(tmp, "%s", g_ObjectData->GetString("SYS_SEX_BOY"));
			else
				sprintf(tmp, "|cffff2020%s|r", g_ObjectData->GetString("SYS_SEX_BOY"));
			strcat(text, tmp);
			haveLimit = true;
		}

		if ( sex.Girl )
		{
			if ( haveLimit )
				strcat(text, ", ");

			if ( canWear )
				sprintf(tmp, "%s", g_ObjectData->GetString("SYS_SEX_GIRL"));
			else
				sprintf(tmp, "|cffff2020%s|r", g_ObjectData->GetString("SYS_SEX_GIRL"));
			strcat(text, tmp);
			haveLimit = true;
		}
	}

	int limit_value[5];
	int value[5];

	limit_value[0] = limit.STR;
	limit_value[1] = limit.STA;
	limit_value[2] = limit.INT;
	limit_value[3] = limit.MND;
	limit_value[4] = limit.AGI;
	value[0] = CNetGlobal::RoleData()->TempData.Attr.Ability->STR;
	value[1] = CNetGlobal::RoleData()->TempData.Attr.Ability->STA;
	value[2] = CNetGlobal::RoleData()->TempData.Attr.Ability->INT;
	value[3] = CNetGlobal::RoleData()->TempData.Attr.Ability->MND;
	value[4] = CNetGlobal::RoleData()->TempData.Attr.Ability->AGI;
	for ( int i = 0; i < 5; i++ )
	{
		if ( limit_value[i] > 0 )
		{			
			bool canWear = false;
			if ( !compareSelf )
				canWear = true;
			else
			{
				if ( value[i] >= limit_value[i] )
					canWear = true;
				else
					canWear = false;
			}

			if ( haveLimit )
				strcat(text, ", ");

			sprintf_s(tmp, MAX_PATH, "SYS_ATTRIBUTE_%2d", i);
			if ( canWear )
				sprintf(tmp, "%s", g_ObjectData->GetString(tmp));
			else
				sprintf(tmp, "|cffff2020%s|r", g_ObjectData->GetString(tmp));

			strcat(text, tmp);
			haveLimit = true;
		}
	}

	if ( haveLimit )
	{
		tooltip->AddLine(text, 1.0f, 1.0f, 1.0f);
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddRuneAbilityText(CUiTooltip* tooltip, ItemFieldStruct* item, float scale)
{
	for ( int i = 0; i < 4; i++ )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->Rune(i));
		if ( itemDB && itemDB->Item.Class == EM_Item_Rune )
		{
			/*
			// ¦WºÙ¤Î®e¶q
			sprintf(tmpL, "%s(%d)", itemDB->Name, itemDB->Item.Volume);
			tooltip->AddLine(tmpL, 0.74f, 0.18f, 1.0f);
			*/

			// ¯à¤O
			for ( int i = 0; i < Max_WearEqType_Count; i++ )
			{
				const char* name = NULL;
				if ( i == 0 )
					name = itemDB->GetName();
				TooltipAddWearEqTypeText(tooltip, itemDB->Item.Ability.EqType[i], itemDB->Item.Ability.EqTypeValue[i] * scale, false, name);
			}

			// »¡©ú
			if ( itemDB->ShortNote[0] != 0 )
			{
				tooltip->AddLine(ItemUTIL::GetItemNote(itemDB).c_str(), 0.0f, 1.0f, 0.0f, true);
			}
		}
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddWearEqTypeText(CUiTooltip* tooltip, WearEqTypeENUM type, float value, bool inborn, const char* name, int abilityCount, int forceRarity)
{
	if ( type == EM_WearEqType_None || value == 0 )
		return;

	char buf[256];
	float r, g, b;	

	switch (type)
	{
	case EM_WearEqType_AtkSpeed:				// "§ðÀ»³t«×(%)"
	case EM_WearEqType_SpellSpeed_AllMagic:		// "¬Iªk³t«× ©Ò¦³(%)"
	case EM_WearEqType_AttackSpeed_AllRange:	// "»·µ{ªZ¾¹³t«×(%) ©Ò¦³"
	case EM_WearEqType_AttackSpeed_Bow:			// "»·µ{ªZ¾¹³t«×(%) ¤}"
	case EM_WearEqType_AttackSpeed_Cossbow:		// "»·µ{ªZ¾¹³t«×(%) ¤Q¦r¤}"
	case EM_WearEqType_AttackSpeed_Gun:			// "»·µ{ªZ¾¹³t«×(%) ºj" 
	case EM_WearEqType_AttackSpeed_AllWeapon:	// "ªñ¾ÔªZ¾¹³t«×(%) ©Ò¦³"
	case EM_WearEqType_AttackSpeed_Unarmed:		// "ªñ¾ÔªZ¾¹³t«×(%) ªÅ¤â"
	case EM_WearEqType_AttackSpeed_Blade:		// "ªñ¾ÔªZ¾¹³t«×(%) ¼C"
	case EM_WearEqType_AttackSpeed_Dagger:		// "ªñ¾ÔªZ¾¹³t«×(%) ¤P­º"
	case EM_WearEqType_AttackSpeed_Wand:		// "ªñ¾ÔªZ¾¹³t«×(%) Åv§ú"
	case EM_WearEqType_AttackSpeed_Axe:			// "ªñ¾ÔªZ¾¹³t«×(%) ©ò"
	case EM_WearEqType_AttackSpeed_Bludgeon:	// "ªñ¾ÔªZ¾¹³t«×(%) Âñ´Ò´Î"
	case EM_WearEqType_AttackSpeed_Claymore:	// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤â¼C"
	case EM_WearEqType_AttackSpeed_Staff:		// "ªñ¾ÔªZ¾¹³t«×(%) §ú"
	case EM_WearEqType_AttackSpeed_2H_Axe:		// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤â©ò"
	case EM_WearEqType_AttackSpeed_2H_Hammer:	// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤âÂñ"
	case EM_WearEqType_AttackSpeed_Polearm:		// "ªñ¾ÔªZ¾¹³t«×(%) ºj(ªø¥Ù)"
	case EM_WearEqType_PlayerDefRate:			// "(¦u)³Qª±®a(orª±®aÃdª«)§ðÀ»«Â¤O¥[´î",
	case EM_WearEqType_NPCDefRate:				// "(¦u)³QNPC§ðÀ»«Â¤O¥[´î",
	case EM_WearEqType_AEMagicDefRate:			// "(¦u)³QACªk³N§ðÀ»«Â¤O¥[´î",
		value = -value;
		break;
	}

	g_ObjectData->GetWearEqTypeString(type, value, buf, 256);
	if ( value < 0.0f )
	{
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
	else
	{
		switch (forceRarity)
		{
		case 1:
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
			break;

		case 2:
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;
			break;

		case 3:
			r = 0.94f;
			g = 0.38f;
			b = 0.05f;
			break;

		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			r = 0.94f;
			g = 0.08f;
			b = 0.88f;
			break;		

		default:
			if ( inborn )
			{
				r = 1.0f;
				g = 1.0f;
				b = 0.0f;

				if ( abilityCount == 3 )
				{
					r = 0.94f;
					g = 0.38f;
					b = 0.05f;
				}
				else if ( abilityCount == 4 )
				{
					r = 0.94f;
					g = 0.08f;
					b = 0.88f;
				}
			}
			else
			{
				r = 0.0f;
				g = 1.0f;
				b = 0.0f;
			}
			break;
		}
	}

	if ( name == NULL || name[0] == 0 )
	{
		tooltip->AddLine(buf, r, g, b);
	}
	else
	{
		tooltip->AddDoubleLine(buf, name, r, g, b, 1.0f, 1.0f, 1.0f);
	}	
}

// ----------------------------------------------------------------------------------
void TooltipAddAbilityText(CUiTooltip* tooltip, GameObjectAttributeStruct& ability, const char* name, float scale)
{	
	int abilityCount = 0;
	for ( int i = 0; i < Max_WearEqType_Count; i++)
	{
		if ( ability.Ability.EqType[i] != EM_WearEqType_None && ability.Ability.EqTypeValue[i] != 0 )
			abilityCount++;
	}

	for ( int i = 0; i < Max_WearEqType_Count; i++ )
	{
		TooltipAddWearEqTypeText(tooltip, ability.Ability.EqType[i], ability.Ability.EqTypeValue[i] * scale, ability.IsBirthAbility, name, abilityCount, ability.Rarity);
		name = NULL;
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddSoulBoundText(CUiTooltip* tooltip, ItemFieldStruct* item)
{
	/*
	if ( item->Mode.Trade == false && item->Mode.EQSoulBound == false)
	tooltip->AddLine(g_ObjectData->GetString("TOOLTIP_SOULBOUND_ALREADY"), 0.99f, 0.8f, 0.23f);
	else 
	*/

	bool lock = false;
	bool trade = false;
	bool soulBound = false;

	if ( item == NULL || item->IsEmpty() )
		return;

	if ( item->Mode._Mode == -1 ) {
		GameObjDbStructEx* OrgDB = g_ObjectData->GetObj( item->OrgObjID );
		if( OrgDB != NULL )
		{
			trade = !(OrgDB->Mode.PickupBound);
			if( OrgDB->IsArmor() || OrgDB->IsWeapon() ) {
				soulBound = OrgDB->Mode.EQSoulBound;
			} else {
				soulBound = false;
			}
		}
	} else {
		lock = item->Mode.ItemLock;
		trade = item->Mode.Trade;
		soulBound = item->Mode.EQSoulBound;
	}	

	if ( lock ) {
		tooltip->AddLine(g_ObjectData->GetString("TOOLTIP_LOCKED_ITEM"), 0.99f, 0.8f, 0.23f);
	} else if ( !trade ) {
		tooltip->AddLine(g_ObjectData->GetString("TOOLTIP_SOULBOUND_ALREADY"), 0.99f, 0.8f, 0.23f);
	} else if ( soulBound ) {
		tooltip->AddLine(g_ObjectData->GetString("TOOLTIP_SOULBOUND_EQ"), 0.99f, 0.8f, 0.23f);
	}
}

// ----------------------------------------------------------------------------------
void TooltipAddSuitAbilityText(CUiTooltip* tooltip, GameObjDbStructEx* itemDB)
{
	char tmpL[512];
	char tmpR[512];
	set<int> suit;	
	int count = 0;

	// ÀË¬d®M¸Ë¥ó¼Æ
	for ( int i = 0; i < itemDB->Suit.TotalCount; i++ )
	{
		for ( int j = 0; j < EM_EQWearPos_MaxCount; j++ )
		{
			int objectID = CNetGlobal::RoleData()->BaseData.EQ.Item[j].OrgObjID;
			if ( objectID == itemDB->Suit.SuitList[i] )
			{
				GameObjDbStructEx* baseItemDB = g_ObjectData->GetObj(objectID);
				if ( CNetGlobal::RoleData()->CanWearObject(baseItemDB, j) ) {
					suit.insert(i);
					break;
				}
			}
		}
	}

	count = (int)suit.size();
	sprintf_s(tmpL, 512, "%s (%d/%d)", itemDB->GetName(), suit.size(), itemDB->Suit.TotalCount);
	tooltip->AddLine(tmpL, 0.62f, 0.46f, 0.3f);

	memset(tmpL, 0, sizeof(tmpL));
	memset(tmpR, 0, sizeof(tmpR));
	// Åã¥Ü®M¸Ë¦U³¡¦ì¦WºÙ
	for ( int i = 0; i < itemDB->Suit.TotalCount; i++ )
	{
		GameObjDbStructEx* suitUnit = CNetGlobal::GetObj(itemDB->Suit.SuitList[i]);
		if ( suitUnit )
		{
			if ( suit.find(i) != suit.end() )
			{
				sprintf_s(tmpL, 512, "  %s", suitUnit->GetName());
				tooltip->AddLine(tmpL, 0.62f, 0.46f, 0.3f);
			}
			else
			{
				sprintf_s(tmpL, 512, "  %s", suitUnit->GetName());
				tooltip->AddLine(tmpL, 0.65f, 0.65f, 0.65f);
			}
		}
	}

	map<int, int> suitAbility;
	for ( int index = 0; index <= itemDB->Suit.TotalCount - 2; index++ )
	{
		if ( itemDB->Suit.SuitBase[index].EqType[0] != EM_WearEqType_None ||
			itemDB->Suit.SuitBase[index].EqType[1] != EM_WearEqType_None ||
			itemDB->Suit.SuitBase[index].EqType[2] != EM_WearEqType_None )
		{
			bool haveItem = false;
			sprintf_s(tmpL, 512, "%d %s " , index+2, g_ObjectData->GetString("SYS_PIECE"));
			for ( int i = 0; i < 10; i++ )
			{
				WearEqTypeENUM type		= itemDB->Suit.SuitValue[index].EqType[i];
				int value				= itemDB->Suit.SuitValue[index].EqTypeValue[i];			

				if ( type != EM_WearEqType_None && value != 0 )
				{
					switch (type)
					{
					case EM_WearEqType_AtkSpeed:				// "§ðÀ»³t«×(%)"
					case EM_WearEqType_SpellSpeed_AllMagic:		// "¬Iªk³t«× ©Ò¦³(%)"
					case EM_WearEqType_AttackSpeed_AllRange:	// "»·µ{ªZ¾¹³t«×(%) ©Ò¦³"
					case EM_WearEqType_AttackSpeed_Bow:			// "»·µ{ªZ¾¹³t«×(%) ¤}"
					case EM_WearEqType_AttackSpeed_Cossbow:		// "»·µ{ªZ¾¹³t«×(%) ¤Q¦r¤}"
					case EM_WearEqType_AttackSpeed_Gun:			// "»·µ{ªZ¾¹³t«×(%) ºj" 
					case EM_WearEqType_AttackSpeed_AllWeapon:	// "ªñ¾ÔªZ¾¹³t«×(%) ©Ò¦³"
					case EM_WearEqType_AttackSpeed_Unarmed:		// "ªñ¾ÔªZ¾¹³t«×(%) ªÅ¤â"
					case EM_WearEqType_AttackSpeed_Blade:		// "ªñ¾ÔªZ¾¹³t«×(%) ¼C"
					case EM_WearEqType_AttackSpeed_Dagger:		// "ªñ¾ÔªZ¾¹³t«×(%) ¤P­º"
					case EM_WearEqType_AttackSpeed_Wand:		// "ªñ¾ÔªZ¾¹³t«×(%) Åv§ú"
					case EM_WearEqType_AttackSpeed_Axe:			// "ªñ¾ÔªZ¾¹³t«×(%) ©ò"
					case EM_WearEqType_AttackSpeed_Bludgeon:	// "ªñ¾ÔªZ¾¹³t«×(%) Âñ´Ò´Î"
					case EM_WearEqType_AttackSpeed_Claymore:	// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤â¼C"
					case EM_WearEqType_AttackSpeed_Staff:		// "ªñ¾ÔªZ¾¹³t«×(%) §ú"
					case EM_WearEqType_AttackSpeed_2H_Axe:		// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤â©ò"
					case EM_WearEqType_AttackSpeed_2H_Hammer:	// "ªñ¾ÔªZ¾¹³t«×(%) Âù¤âÂñ"
					case EM_WearEqType_AttackSpeed_Polearm:		// "ªñ¾ÔªZ¾¹³t«×(%) ºj(ªø¥Ù)"
					case EM_WearEqType_PlayerDefRate:			// "(¦u)³Qª±®a(orª±®aÃdª«)§ðÀ»«Â¤O¥[´î"
					case EM_WearEqType_NPCDefRate:				// "(¦u)³QNPC§ðÀ»«Â¤O¥[´î"
					case EM_WearEqType_AEMagicDefRate:			// "(¦u)³QACªk³N§ðÀ»«Â¤O¥[´î"
						value = -value;
						break;
					}

					int v = value;
					map<int, int>::iterator iter = suitAbility.find(type);
					if ( iter == suitAbility.end() ) {
						suitAbility[type] = value;
					} else if ( value != iter->second ) {
						v = value - iter->second;
						iter->second = value;
					} else {
						v = 0;
					}

					if ( v != 0 ) {
						g_ObjectData->GetWearEqTypeString(type, (float)v, tmpR, MAX_PATH);
						if ( haveItem )
							strcat(tmpL, ", ");
						haveItem = true;
						strcat(tmpL, tmpR);
					}
				}
			}
			if ( index <= count - 2 )
				tooltip->AddLine(tmpL, 0.0f, 1.0f, 0.0f);
			else
				tooltip->AddLine(tmpL, 0.65f, 0.65f, 0.65f);
		}
	}

	bool firstSuitSkill = true;
	// ®M¸Ë§Þ¯à
	for ( int i = 0; i < 4; i++ )
	{
		string tempStr;
		if ( itemDB->Suit.SuitSkillID[i] != 0 )
		{
			GameObjDbStructEx* objDB = CNetGlobal::GetObj(itemDB->Suit.SuitSkillID[i]);
			if ( objDB && objDB->Classification == EM_ObjectClass_MagicCollect )
			{
				if ( firstSuitSkill )
				{
					tempStr = g_ObjectData->GetString("SYS_SUIT_SKILL");
					string newStr = ReplaceKeyString(tempStr, "<SUITCOUNT>", itemDB->Suit.TotalCount);						
					tooltip->AddLine(newStr.c_str(), 1.0f, 1.0f, 1.0f);
					firstSuitSkill = false;
				}

				string needJob;
				
				if ( objDB->MagicCol.Need[0].Type == EM_MagicSpellNeedType_Self_MainJob )
					needJob = g_ObjectData->GetClassNameByID((Voc_ENUM)objDB->MagicCol.Need[0].Value);
				else if ( objDB->MagicCol.Need[1].Type == EM_MagicSpellNeedType_Self_MainJob )
					needJob = g_ObjectData->GetClassNameByID((Voc_ENUM)objDB->MagicCol.Need[1].Value);
				else
					needJob = g_ObjectData->GetString("SYS_TOOLTIP_SUIT_ANY_CLASS");

				tempStr = g_ObjectData->GetString("SYS_TOOLTIP_SUIT_SKILL");
				tempStr = ReplaceKeyString( tempStr, "<SKILLNAME>", objDB->GetName() );
				tempStr = ReplaceKeyString( tempStr, "<CLASSNAME>", needJob );

				tooltip->AddLine(tempStr.c_str(), 1.0f, 1.0f, 0.0f);
			}
		}
	}

	/*
	// ¨ú±o³Ì¤Ö®M¸Ë¥[¦¨¥ó¼Æ
	for ( int curr = count-2; curr >= 0; curr-- )
	{
		if ( itemDB->Suit.SuitBase[curr].EqType[0] != EM_WearEqType_None ||
			itemDB->Suit.SuitBase[curr].EqType[1] != EM_WearEqType_None ||
			itemDB->Suit.SuitBase[curr].EqType[2] != EM_WearEqType_None )
		{
			bool haveItem = false;
			sprintf(tmpL, "%d%s " , curr+2, g_ObjectData->GetString("SYS_PIECE"));
			for ( int i = 0; i < 10; i++ )
			{
				WearEqTypeENUM type		= itemDB->Suit.SuitValue[curr].EqType[i];
				int value				= itemDB->Suit.SuitValue[curr].EqTypeValue[i];
				if ( type != EM_WearEqType_None && value != 0 )
				{					
					g_ObjectData->GetWearEqTypeString(type, value, tmpR, MAX_PATH);
					if ( haveItem )
						strcat(tmpL, ", ");
					haveItem = true;
					strcat(tmpL, tmpR);
				}
			}
			tooltip->AddLine(tmpL, 0.0f, 1.0f, 0.0f);
			break;
		}
	}
	
	//for ( int curr = count-1; curr >= 0 && curr <= itemDB->Suit.TotalCount-2; curr++ )
	{
		// ¤U¤@¶¥®M¸Ë¥[¦¨
		for ( int curr = count-1; curr >= 0 && curr <= itemDB->Suit.TotalCount-2; curr++ )
		{
			if ( itemDB->Suit.SuitBase[curr].EqType[0] != EM_WearEqType_None ||
				itemDB->Suit.SuitBase[curr].EqType[1] != EM_WearEqType_None ||
				itemDB->Suit.SuitBase[curr].EqType[2] != EM_WearEqType_None )
			{
				bool haveItem = false;
				sprintf(tmpL, "%d%s " , curr+2, g_ObjectData->GetString("SYS_PIECE"));
				for ( int i = 0; i < 10; i++ )
				{
					WearEqTypeENUM type		= itemDB->Suit.SuitValue[curr].EqType[i];
					int value				= itemDB->Suit.SuitValue[curr].EqTypeValue[i];
					if ( type != EM_WearEqType_None && value != 0 )
					{
						if ( haveItem )
							strcat(tmpL, ", ");
						haveItem = true;

						g_ObjectData->GetWearEqTypeString(type, value, tmpR, MAX_PATH);
						strcat(tmpL, tmpR);
					}
				}
				tooltip->AddLine(tmpL, 0.65f, 0.65f, 0.65f);
				break;
			}
		}
	}
	*/	
}

// ----------------------------------------------------------------------------------
void TooltipAddSuitSkillText(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB)
{
	
}

// ----------------------------------------------------------------------------------
bool TooltipSetAttackSkill( CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB )
{
	tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);
	tooltip->AddLine(itemDB->GetName(), 1.0f, 1.0f, 1.0f);

	return true;
}
// ----------------------------------------------------------------------------------
void TooltipAddDropAbility(CUiTooltip* tooltip, ItemFieldStruct* item, GameObjDbStructEx* itemDB)
{	
	int abilityCount = 0;
	int realCount = 0;
	int displayAtttibuteCount = 0;
	for ( int i = 0; i < _MAX_DROP_ABILITY_COUNT_; i++ )
	{
		if ( itemDB->Item.DropItemAbility[i].OrgID != 0 )
			realCount++;
		if ( itemDB->Item.DropItemAbility[i].Rate == 100000 )
			abilityCount++;
	}

	// ¹ê»Úªþ¥[¾÷²v¤j©ó¿ï¾Ü¾÷²v
	if ( abilityCount > itemDB->Item.SelectBaseAttrCount )
	{
		tooltip->AddLine(g_ObjectData->GetString("SYS_RANDOM_ABILITY_TEXT"), 1.0f, 1.0f, 1.0f);
	}

	for ( int i = 0; i < _MAX_DROP_ABILITY_COUNT_; i++ )
	{
		if ( itemDB->Item.DropItemAbility[i].Rate == 100000 )
		{
			GameObjDbStructEx* inherent = CNetGlobal::GetObj(itemDB->Item.DropItemAbility[i].OrgID);
			if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
			{	
				displayAtttibuteCount++;

				// ¯à¤O
				TooltipAddAbilityText(tooltip, inherent->Attirbute, inherent->GetName() );
			}
		}
	}

	if ( abilityCount > displayAtttibuteCount )
	{
		tooltip->AddLine(g_ObjectData->GetString("SYS_APPEND_RANDOM_ABILITY"), 1.0f, 1.0f, 1.0f);
	}
	else if ( (abilityCount < itemDB->Item.SelectBaseAttrCount && realCount > 0) )
	{
		tooltip->AddLine(g_ObjectData->GetString("SYS_APPEND_ABILITY_TEXT"), 1.0f, 1.0f, 1.0f);
	}
}

void CGameTooltip::SetHyperLink( CUiTooltip *tooltip,const char* link )
{
	if( NULL == tooltip || NULL == link )
		return;
	
	tooltip->ClearLines();	


	string type ; 
	string data ; 
	string name ;
	if( !ParseHyperlink( link , type , data , name ) )
	{
		return ;
	}

	if( type == "item" )
	{
		ItemFieldStruct* pItemField; 
		GameObjDbStructEx* pItemDB;
		if( !ItemLink_To_ItemField( link, pItemField, pItemDB ) )
		{
			tooltip->SetVisible( false );
			return ;
		}
		bool Visible = this->TooltipSetItem( tooltip, false, pItemField, pItemDB, false );
		tooltip->SetVisible( Visible );
	}
	else if( type == "skill" )
	{
		int GUID; // = atoi( data.c_str() );
		int skillpoint = 0; 

		if( 2 != sscanf( data.c_str() , "%d %d" , &GUID , &skillpoint ) )
		{
			return;
		}
		g_GameTooltip_SkillPoint = skillpoint;

		bool Visible = TooltipSetItem( tooltip, true, NULL, CNetGlobal::GetObj( GUID ) );
 		tooltip->SetVisible( Visible );

		g_GameTooltip_SkillPoint = -1;

	}
	else if( type == "quest" )
	{
		int GUID; // = atoi( data.c_str() );

		if( 1 != sscanf( data.c_str() , "%x" , &GUID ) )
		{
			return;
		}

		bool Visible = TooltipSetItem( tooltip, true, NULL, CNetGlobal::GetObj( GUID ) );
		tooltip->SetVisible( Visible );
	}
}

void CGameTooltip::SetSysTooltips(CUiTooltip* tooltip, int index)
{
	if (index>=0&&index<2)

		m_SysTooltip[index]=tooltip;
}

//-----------------------------------------------------------
void CGameTooltip::Update(float elapsedTime)
{
	if ( m_boUpdate )
	{
		CUiTooltip* tooltip = m_gameTooltip;

		switch ( m_type )
		{
		case eGameTooltip_UnitBuff:
		case eGameTooltip_UnitDebuff:
			if ( tooltip && tooltip->IsVisible() )
			{
				SetTooltip(m_type, tooltip, m_index, m_unit);
				return;
			}
		}

		m_boUpdate = false;
	}
}

//-----------------------------------------------------------
int LuaFunc_GameTooltip_SetSysTips(lua_State* L)
{

	CUiTooltip* tooptips = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	int index = luaL_checkint(L, 2);

	if (g_pGameTooltip)

		g_pGameTooltip->SetSysTooltips(tooptips,index);



	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetRecipeStoreItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_RecipeStore, tooltip, index, "");
	return 0;
}
// ----------------------------------------------------------------------------------
int Tooltip_SetFusionStone(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));

	int iAbilityId[3];
	iAbilityId[0] = luaL_checkint( L, 2 );
	iAbilityId[1] = luaL_checkint( L, 3 );
	iAbilityId[2] = luaL_checkint( L, 4 );

	GameObjDbStructEx* pObj = CNetGlobal::GetObj( g_pFusionStoneTradeFrame->GetStoneID() );

	if( pObj )
	{
		bool isVisible = false;
		ItemFieldStruct item;

		item.Init();
		item.OrgObjID = g_pFusionStoneTradeFrame->GetStoneID();
		item.Inherent( 0, iAbilityId[0] );
		item.Inherent( 1, iAbilityId[1] );
		item.Inherent( 2, iAbilityId[2] );
		item.Mode.Trade = true;

		tooltip->ClearLines();
		
		isVisible = g_pGameTooltip->TooltipSetItem( tooltip, true, &item, pObj );

		tooltip->SetVisible( isVisible );
	}

	return 0;
}
int Tooltip_SetSuitSkillPlateItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	int SkillID = CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.EQ[index];
	if(SkillID)
	{
		g_pGameTooltip->SetTooltip(eGameTooltip_SuitSkill, tooltip, SkillID, "");
	}
	return 0 ;
}
int Tooltip_SetSuitSkillListItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2);
	int SkillID = CNetGlobal::RoleData()->PlayerBaseData->SuitSkill.LeanSkillList[index].SkillID;
	if(SkillID)
	{
		g_pGameTooltip->SetTooltip(eGameTooltip_SuitSkill, tooltip, SkillID, "");
	}
	return 0 ;
}
int Tooltip_SetRuneExChangeItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int index = luaL_checkint(L, 2) - 1;
	g_pGameTooltip->SetTooltip(eGameTooltip_Bag, tooltip, g_pRuneExchangeFrame->RE_GetPos(index), "");
	return 0;
}
int Tooltip_SetATFUseItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();
	int idx = luaL_checkint(L, 2) ;

	int pos = g_pAncillaryTitleFrame->GetATFUseItemPos(idx);

	if( pos >= 60)
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(pos);
		if ( item && item->IsEmpty() == false )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
			if ( itemDB )
			{
				bool visible = g_pGameTooltip->TooltipSetItem(tooltip, true, item, itemDB);
				tooltip->SetVisible( visible );
			}
		}
	}
	return 0;
}
int Tooltip_SetATFTitleItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	
	int idx = luaL_checkint(L, 2) - 1;
	int TitleID = g_pAncillaryTitleFrame->GetATFItem(idx);

	GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( TitleID );

	if( NULL == pTitleObj || !pTitleObj->IsTitle() )
		return 0;
	
	tooltip->ClearLines();
	bool isVisible = g_pGameTooltip->TooltipSetItem( tooltip, true, NULL, pTitleObj );
	tooltip->SetVisible(isVisible);

	return 0;
}
int Tooltip_SetATFTitleTipItem(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	tooltip->ClearLines();
    int idx = luaL_checkint(L, 2);
	int type = luaL_checkint(L, 3);
	int rare = luaL_checkint(L, 4);
	int nTitleID ;

	RoleDataEx* Owner = CNetGlobal::RoleData();
	float Famous = 0;
	if(Owner)
	{
		Famous = Owner->PlayerBaseData->TitleSys.Famous;
	}
	if( type == 0)
	{
		nTitleID = idx;
	}
	else
	{
		nTitleID = g_pAncillaryTitleFrame->GetATFItem( idx - 1 );
	}

	GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( nTitleID );

	if( NULL == pTitleObj || !pTitleObj->IsTitle() )
		return 0;

	bool Visible = false;
	// ªþ¥[§Þ¯à
	if( pTitleObj )
	{
		char szText[128];
		for( int i =0 ; i < Max_WearEqType_Count ; i++ )
		{
			if( pTitleObj->Title.AbilityEx.EqType[i] != 0 &&
				pTitleObj->Title.AbilityEx.EqTypeValue != 0 )  
			{
				sprintf( szText , "SYS_WEAREQTYPE_%d", pTitleObj->Title.AbilityEx.EqType[i] );
				if( pTitleObj->Title.AbilityEx.EqTypeValue[i] >=0 )
				{
					sprintf( szText ,  "+%d %s", pTitleObj->Title.AbilityEx.EqTypeValue[i] , g_ObjectData->GetString( szText ) );
				}
				else
				{
					sprintf( szText ,  "%d %s", pTitleObj->Title.AbilityEx.EqTypeValue[i] , g_ObjectData->GetString( szText ) );
				}
				if( Famous <= 0 && type != 0)
				{
					tooltip->AddLine( szText , 1 , 0 , 0 );
					tooltip->AddLine( pTitleObj->Title.TitleSysNote , 1 , 0 , 0 );
				}
				else
				{
					tooltip->AddLine( szText , 0 , 1 , 0 );
					tooltip->AddLine( pTitleObj->Title.TitleSysNote , 0 , 1 , 0 );
				}
				Visible = true;
			}
		}
	}
	
	if( type == 0 )
	{
		if( rare <= Owner->PlayerBaseData->TitleSys.Lv )
		{
			tooltip->AddLine( g_ObjectData->GetString( "ATF_SET_TITLE_REQUEST_II" ) , 1 , 1 , 1 );	
		}
		else
		{
			tooltip->AddLine( g_ObjectData->GetString( "ATF_RARE_LV_NOT_ENOUGH" ) , 1 , 1 , 1 );	
		}
		Visible = true;
	}

	if( Famous <= 0)
	{
		tooltip->AddLine( g_ObjectData->GetString( "ATF_USE_ITEM_WARNING04" ) , 1 , 0 , 0 );	
	}	


	tooltip->SetVisible(Visible);
	return 0;
}
int Tooltip_SetItemDB(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int OrgObjID = luaL_checkint(L, 2);

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( OrgObjID );

	if( (pObjDB != NULL) && (pObjDB->IsItem() == true) )
	{
		tooltip->ClearLines();
		bool isVisible = g_pGameTooltip->TooltipSetItem( tooltip, false, NULL, pObjDB );
		tooltip->SetVisible(isVisible);
	}

	return 0;
}

int Tooltip_SetMagicBaseDB(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int OrgObjID = luaL_checkint(L, 2);

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( OrgObjID );

	if( (pObjDB != NULL) && (pObjDB->IsMagicBase() == true) )
	{
		tooltip->ClearLines();
		bool isVisible = g_pGameTooltip->TooltipSetItem( tooltip, true, NULL, pObjDB );
		tooltip->SetVisible(isVisible);
	}

	return 0;
}

int Tooltip_SetMagicCollectDB(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	int OrgObjID = luaL_checkint(L, 2);

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( OrgObjID );

	if( (pObjDB != NULL) && (pObjDB->IsMagicCollect() == true) )
	{
		tooltip->ClearLines();
		bool isVisible = g_pGameTooltip->TooltipSetItem( tooltip, true, NULL, pObjDB );
		tooltip->SetVisible(isVisible);
	}

	return 0;
}