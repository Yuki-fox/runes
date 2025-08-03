#include "../../mainproc/GameMain.h"					// For Obj and Role struct
#include "../../netwaker_member/NetWakerClientInc.h"	// For NetGlobal

#include "../CraftFrame/UI_CraftFrame.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ActionFrame.h"
#include "../Debug/DebugMessage.h"
#include "./UI_SkillBook.h"
#include "../ItemLink/ItemLink.h"

// 全域宣告

//-------------------------------------------------------------------
CUI_SkillBook*		g_pUi_SkillBook						= NULL;
int					CUI_SkillBook::m_iSpellingGUID		= 0;
vector< CUI_SkillBook::SpellInfo >		CUI_SkillBook::m_vecSkill[ST_Count];
bool CUI_SkillBook::m_bSpellInfoDirty[ST_Count];
map< int , CUI_SkillBook::SpellInfo > CUI_SkillBook::m_SaveLvPos_SpellInfo_Map[ST_Count];
map< int , int > CUI_SkillBook::m_SkillID_SaveLvPos_Map[ST_Count];
int CUI_SkillBook::m_nPetMagicID;
//-------------------------------------------------------------------
//int			LuaFunc_GetNumSkill_SkillBook				( lua_State *L );
int	LuaFunc_GetNumSkill							( lua_State *L );
int	LuaFunc_GetSkillDetail						( lua_State *L );
int	LuaFunc_GetSkillTotalPointToLV              ( lua_State *L );
int	LuaFunc_DragSkillButton						( lua_State *L );
int	LuaFunc_UseSkill							( lua_State *L );

int	LuaFunc_GetTpExp							( lua_State *L );
int	LuaFunc_GetTotalTpExp						( lua_State *L );

int	LuaFunc_SetSpellPoint						( lua_State *L );
int	LuaFunc_GetVocInfo							( lua_State *L );
int	LuaFunc_GetVocSubInfo						( lua_State *L );
int	LuaFunc_GetSkillCooldown					( lua_State *L );
int	LuaFunc_CastSpellByName						( lua_State *L );

//ItemLink
int	LuaFunc_GetSkillHyperLink					( lua_State *L );

/*
int			LuaFunc_GetSkillPoint_SkillBook				( lua_State *L );
int			LuaFunc_SkillUp_SkillBook					( lua_State *L );
int			LuaFunc_UseSkill_SkillBook					( lua_State *L );
int			LuaFunc_OpenSkillList_SkillBook				( lua_State *L );
int			LuaFunc_GetSkillInfo_SkillList				( lua_State *L );
int			LuaFunc_GetNumMaxSkillInfo_SkillList		( lua_State *L );


*/


//-------------------------------------------------------------------
CUI_SkillBook::CUI_SkillBook( CInterface* object ) : CInterfaceBase( object )
{
	g_pUi_SkillBook		= this;
	//m_iLastSkillIndex	= -1;
	//m_iSpellingGUID		= 0;
 
	m_bSpellInfoDirty[0] = true;
	m_bSpellInfoDirty[1] = true;
	m_bSpellInfoDirty[2] = true;
	m_bSpellInfoDirty[3] = true;
	m_bSpellInfoDirty[4] = true;
}
// --------------------------------------------------------------------------------------
CUI_SkillBook::~CUI_SkillBook(void)
{
	g_pUi_SkillBook	= NULL;
}
// --------------------------------------------------------------------------------------
void CUI_SkillBook::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "GetNumSkill",						LuaFunc_GetNumSkill					);
		lua_register( l, "GetSkillDetail",					LuaFunc_GetSkillDetail				);
		lua_register( l, "GetSkillTotalPointToLV",			LuaFunc_GetSkillTotalPointToLV		);
		lua_register( l, "DragSkillButton",					LuaFunc_DragSkillButton				);
		lua_register( l, "UseSkill",						LuaFunc_UseSkill					);

		lua_register( l, "GetTpExp",						LuaFunc_GetTpExp					);
		lua_register( l, "GetTotalTpExp",					LuaFunc_GetTotalTpExp				);

		lua_register( l, "SetSpellPoint",					LuaFunc_SetSpellPoint				);
		lua_register( l, "GetVocInfo",						LuaFunc_GetVocInfo					);
		lua_register( l, "GetVocSubInfo",					LuaFunc_GetVocSubInfo				);

		lua_register( l, "GetSkillCooldown",				LuaFunc_GetSkillCooldown			);
		lua_register( l, "CastSpellByName",					LuaFunc_CastSpellByName				);

		lua_register( l, "GetSkillHyperLink",				LuaFunc_GetSkillHyperLink			);

		/*
		lua_register( l, "GetSkillPoint_SkillBook",			LuaFunc_GetSkillPoint_SkillBook		);

		lua_register( l, "SkillUp_SkillBook",				LuaFunc_SkillUp_SkillBook			);

		lua_register( l, "OpenSkillList_SkillBook",			LuaFunc_OpenSkillList_SkillBook		);
		lua_register( l, "GetSkillInfo_SkillList",			LuaFunc_GetSkillInfo_SkillList		);
		lua_register( l, "GetNumMaxSkillInfo_SkillList",	LuaFunc_GetNumMaxSkillInfo_SkillList);		
		lua_register( l, "GetSkillIndex_SkillBook",			LuaFunc_GetSkillIndex_SkillBook		);
		lua_register( l, "OnDrag_SkillButton",				LuaFunc_OnDrag_SkillButton			);
		*/
	}
}


void CUI_SkillBook::SkillUpdate( int iType )
{
	if( iType < 0 || iType >= ST_Count )
	{
		m_bSpellInfoDirty[0] = true;
		m_bSpellInfoDirty[1] = true;
		m_bSpellInfoDirty[2] = true;
		m_bSpellInfoDirty[3] = true;
		m_bSpellInfoDirty[4] = true;
	}
	else
	{
		m_bSpellInfoDirty[iType] = true;
	}



	g_pInterface->SendWorldEvent(UiEvent_SkillUpdate);

	// 更新快捷列技能編號為目前學會的最高等級
	g_pActionFrame->UpdateSkillID();
}


// --------------------------------------------------------------------------------------
static int SortSpellInfoCallback( const void* arg1, const void* arg2 )
{
	CUI_SkillBook::SpellInfo* pInfo1 = (CUI_SkillBook::SpellInfo*)arg1;
	CUI_SkillBook::SpellInfo* pInfo2 = (CUI_SkillBook::SpellInfo*)arg2;

	//未習得 比較
	if( !pInfo1->bLearned && !pInfo2->bLearned )
	{
		if( pInfo1->LearnLV > pInfo2->LearnLV )
			return 1;
		if( pInfo1->LearnLV < pInfo2->LearnLV )
			return -1;
		return 0;
	}

	if( !pInfo1->bLearned )
	{
		return 1;
	}

	if( !pInfo2->bLearned )
	{
		return -1;
	}

	//已習得 比較
	if( pInfo1->BaseLearnLV > pInfo2->BaseLearnLV )
	{
		return 1;
	}
	if( pInfo1->BaseLearnLV < pInfo2->BaseLearnLV )
	{
		return -1;
	}

	if( pInfo1->Name > pInfo2->Name )
	{
		return 1;
	}

	if( pInfo1->Name < pInfo2->Name )
	{
		return -1;
	}

	if( pInfo1->LearnLV > pInfo2->LearnLV )
	{
		return 1;
	}

	if( pInfo1->LearnLV < pInfo2->LearnLV )
	{
		return -1;
	}

	return 0;
}

// --------------------------------------------------------------------------------------
vector< CUI_SkillBook::SpellInfo >* CUI_SkillBook::GetSpellInfoVector( int iType )
{
	if ( (unsigned int)iType >= ST_Count )
	{
		return NULL;
	}

	if( false == m_bSpellInfoDirty[iType] )
	{
		return &m_vecSkill[iType];
	}
	

	RoleDataEx* RoleData = CNetGlobal::RoleData();

	int LearnSkillCount = 0;
	JobSkillBaseStruct* LearnSkillTable = NULL;

	int				*pSkill		= NULL;
	int				iMaxSkill	= 0;

	RoleDataEx*		pRole		= CNetGlobal::RoleData();

	float*			pPowerInfo	= NULL;

	CUI_SkillBook::m_vecSkill[ iType ].clear();		

	switch( iType )
	{
		//----------------------------------------------------------------
	case ST_Common: // 綜合技能
		{
			// 事先插入特殊招式
			CUI_SkillBook::SpellInfo Info;

			// 插入攻擊技能
			Info.Reset();
			Info.iGUID    = DF_SKILLID_ATTACK;
			Info.bLearned = true; 

			CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );

			// 檢查回城術
			//if( pRole->CheckKeyItem( DF_SKILLID_PORTOL ) == true )
			//{
			Info.Reset();
			Info.iGUID    = DF_SKILLID_PORTOL;
			Info.bLearned = true;
			CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
			//}

			// 檢查分解技能
			if( pRole->CheckKeyItem( DF_SKILLID_DISENCHANT ) == true )
			{
				Info.Reset();
				Info.iGUID    = DF_SKILLID_DISENCHANT;
				Info.bLearned = true;
				CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
			}

			// PK
			if( g_ObjectData->GetObj( DF_SKILLID_PK ) != NULL && pRole->CheckKeyItem( DF_SKILLID_PK  ) )
			{
				Info.Reset();
				Info.iGUID    = DF_SKILLID_PK;
				Info.bLearned = true;
				CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
			}

			// 檢查角色所會的生活技能
			g_pCraftFrame->ResetCraftInfo();

			for( vector< StructCraftType >::iterator it = g_pCraftFrame->m_vecCraftType.begin(); it != g_pCraftFrame->m_vecCraftType.end(); it++ )
			{
				StructCraftType* pCraftType = &(*it);

				Info.Reset();
				Info.iGUID    = Def_ObjectClass_KeyItem + pCraftType->iCraftSkillID;
				Info.bLearned = true;
				CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );											
			}

			// 新手區傳送
			for( int i = DF_SKILLID_BEGINNERPORTOL_BEGIN; i <= DF_SKILLID_BEGINNERPORTOL_END; i++ )
			{
				if( pRole->CheckKeyItem( i ) == true )
				{
					Info.Reset();
					Info.iGUID    = i;
					Info.bLearned = true;
					CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
				}
			}

			//稱號技能
			GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( CNetGlobal::RoleData()->BaseData.TitleID );
			if( pTitleObj )
			{
				GameObjDbStructEx* pSkillObj = g_ObjectData->GetObj( pTitleObj->Title.SkillID );
				if( pSkillObj )
				{
					Info.Reset();
					Info.iGUID    = pTitleObj->Title.SkillID;
 					Info.iPowerLV = pTitleObj->Title.SkillLv;
					Info.bLearned = true;
					CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
				}
			}

		} break;
	case ST_MainJob: // Main Job
		{
			pSkill		= pRole->PlayerTempData->Skill.MainJob;
			pPowerInfo	= pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].NormalSkill;
			iMaxSkill	= _MAX_NormalSkill_COUNT_;

			GameObjDbStructEx* JobDB = NULL;
			if ( RoleData->BaseData.Voc != -1 )
				JobDB = RoleDataEx::St_GetObjDB( RoleDataEx::GlobalSetting.LearnMagicID + RoleData->BaseData.Voc );

			//GameObjDbStructEx* JobDB = RoleDataEx::St_GetObjDB( Def_ObjectClass_LearnMagic + RoleData->BaseData.Voc );
			//GameObjDbStructEx* JobDB = RoleDataEx::St_GetObjDB( RoleDataEx::GlobalSetting.LearnMagicID + RoleData->BaseData.Voc );
			//assert( JobDB );
			if( !JobDB )
			{
				return 0;
			}

			GameObjectLearnMagicStruct* LearnMagic = &JobDB->LearnMagic;
			LearnSkillCount = LearnMagic->NormalSkillCount;
			LearnSkillTable = LearnMagic->NormalSkill;

		} break;
		//----------------------------------------------------------------
	case ST_SubJob: // Sub Job
		{
			pSkill		= pRole->PlayerTempData->Skill.SubJob;
			pPowerInfo	= pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc_Sub ].NormalSkill;
			iMaxSkill	= _MAX_NormalSkill_COUNT_;

			GameObjDbStructEx* JobDB = NULL;
			if ( RoleData->BaseData.Voc_Sub != -1 )
				JobDB = RoleDataEx::St_GetObjDB( RoleDataEx::GlobalSetting.LearnMagicID + RoleData->BaseData.Voc_Sub );

			//assert( JobDB );
			if( !JobDB )
			{
				return 0;
			}

			GameObjectLearnMagicStruct* LearnMagic = &JobDB->LearnMagic;
			LearnSkillCount = LearnMagic->NormalSkillCount;
			LearnSkillTable = LearnMagic->NormalSkill;
		} break;
		//----------------------------------------------------------------
	case ST_SP: // SP
		{
			pSkill		=  pRole->PlayerTempData->Skill.SP;
			pPowerInfo	= pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].SPSkill;
			iMaxSkill	= _MAX_SPSkill_COUNT_;

			GameObjDbStructEx* JobDB = NULL;
			if ( RoleData->BaseData.Voc != -1 )
				JobDB = RoleDataEx::St_GetObjDB( RoleDataEx::GlobalSetting.LearnMagicID + RoleData->BaseData.Voc );
			
			//assert( JobDB );
			if( !JobDB )
			{
				return 0;
			}

			GameObjectLearnMagicStruct* LearnMagic = &JobDB->LearnMagic;
			LearnSkillCount = LearnMagic->SPSkillCount;
			LearnSkillTable = LearnMagic->SPSkill;

		} break;
		//----------------------------------------------------------------
	case ST_Pet: // Pet
		{
			pSkill		=  pRole->PlayerTempData->Skill.SP;
			pPowerInfo	= pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].SPSkill;
			iMaxSkill	= _MAX_SPSkill_COUNT_;

			GameObjDbStructEx* JobDB = NULL;
			if ( RoleData->BaseData.Voc != -1 )
				JobDB = RoleDataEx::St_GetObjDB( RoleDataEx::GlobalSetting.LearnMagicID + RoleData->BaseData.Voc );

			//assert( JobDB );
			if( !JobDB )
			{
				return 0;
			}

			GameObjectLearnMagicStruct* LearnMagic = &JobDB->LearnMagic;
			LearnSkillCount = LearnMagic->SPSkillCount;
			LearnSkillTable = LearnMagic->SPSkill;

		} break;

	}

	GameObjDbStructEx* pSkillObj;	 
	int ExpLevel;
	CUI_SkillBook::SpellInfo Info;
	
	JobSkillBaseStruct*	pLearnSkill;
	int LearnSkillIndex;
	bool bLearned;
	float fPower;
	int i;

	map<string,int> BaseLearnLVMap;
	map<string,int>::iterator itPos;

	int BaseLearnLV;
	
	//同一位置 只紀錄學會的 SaveLvPos
	map< int , CUI_SkillBook::SpellInfo >& SaveLvPos_SpellInfo_Map = m_SaveLvPos_SpellInfo_Map[iType];
	SaveLvPos_SpellInfo_Map.clear();

	map< int , int >& SkillID_SaveLvPos_Map = m_SkillID_SaveLvPos_Map[iType];
	SkillID_SaveLvPos_Map.clear();

	map< int , int > LearnMagic_ID_Lv_Map;
	
	for( LearnSkillIndex = 0; LearnSkillIndex < LearnSkillCount; ++LearnSkillIndex )
	{
		pLearnSkill = &LearnSkillTable[LearnSkillIndex];

		//pLearnSkill->MagicID;
		//pLearnSkill->LearnLV;

		pSkillObj = g_ObjectData->GetObj( pLearnSkill->MagicID );
		if( !pSkillObj )
		{
			continue;
		}

		//寵物技能
		if( ( iType == ST_Pet && pSkillObj->MagicCol.PetSkillType <= 0 ) ||
			( iType != ST_Pet && pSkillObj->MagicCol.PetSkillType > 0 ) )
		{
			continue;
		}
		
		//是否學會
		bLearned = false;
		fPower = 0.0f;
		for( i = 0; i < LearnSkillCount; ++i )
		{
			if( pSkill[ i ] == pLearnSkill->MagicID )
			{
				fPower = pPowerInfo[ i ];
				bLearned = true;
				break;
			}
		}

		//寵物技能還沒學會 不顯示
		if( iType == ST_Pet && bLearned == false )			
		{
			continue;
		}

		//取得同技能位置的技能 使用
		SkillID_SaveLvPos_Map[pLearnSkill->MagicID] = pLearnSkill->SaveLvPos;

		//keyitem
		if( pLearnSkill->KeyItemID > 0 )
		{
			//有KeyItemID時 未得到KeyItem 或 學習條件未達成 不顯示
			if( false == pRole->CheckKeyItem( pLearnSkill->KeyItemID ) || false == bLearned )
			{
				continue;
			}
		}

		itPos = BaseLearnLVMap.find( pSkillObj->GetName() );
		if( itPos != BaseLearnLVMap.end() )
		{
			BaseLearnLV = itPos->second;
		}
		else
		{
			BaseLearnLVMap[pSkillObj->GetName() ] = pLearnSkill->LearnLV;
			BaseLearnLV = pLearnSkill->LearnLV;
		}
		
		if( bLearned )
		{
			Info.iGUID    = pLearnSkill->MagicID;
			Info.iPowerLV = (int)fPower;

			ExpLevel = int(Info.iPowerLV + pSkillObj->MagicCol.ExpTableRate -1);

			if( ExpLevel <= 0 )
				ExpLevel = 0 ;
			if( ExpLevel >= (int)pRole->JobSkillExpTable.size() )
				ExpLevel = (int)pRole->JobSkillExpTable.size() -1;

			Info.iPowerPointToLV	= pRole->JobSkillExpTable[ ExpLevel ];
			Info.iPowerPoint		= (int) ( ( fPower - ((int)fPower) ) * Info.iPowerPointToLV + ( ((float) 1 /(float)Info.iPowerPointToLV)/2  ) );

		}
		else
		{
			Info.iGUID           = pLearnSkill->MagicID;
			Info.iPowerPoint     = 0;
			Info.iPowerPointToLV = 0;
			Info.iPowerLV        = 0;
			Info.iSpecial        = 0;
		}

		Info.bLearned    = bLearned;
		Info.BaseLearnLV = BaseLearnLV;        // 第1級 習得等級 排序用
		Info.LearnLV     = pLearnSkill->LearnLV;        // 習得等級 排序用
		Info.Name        = pSkillObj->GetName();

		LearnMagic_ID_Lv_Map[Info.iGUID] = Info.iPowerLV;
		for( i = 0 ; i < 2 ; ++i )
		{
			if( pLearnSkill->NeedMagicLv[i] <= 0 ||
				pLearnSkill->NeedMagicLv[i] < LearnMagic_ID_Lv_Map[ pLearnSkill->NeedMagicID[i] ] )
			{
				Info.NeedMagicID[i]  = 0 ;	//需要法術
				Info.NeedMagicLv[i]  = 0 ;	//需要法術等級
			}
			else
			{
				Info.NeedMagicID[i]  = pLearnSkill->NeedMagicID[i] ;	//需要法術
				Info.NeedMagicLv[i]  = pLearnSkill->NeedMagicLv[i] ;	//需要法術等級
			}
		}
		
		//同一位置 只紀錄學會的 SaveLvPos
		if( bLearned )
		{
			SaveLvPos_SpellInfo_Map[pLearnSkill->SaveLvPos] = Info;
		}
		else
		{
			if( SaveLvPos_SpellInfo_Map.find( pLearnSkill->SaveLvPos ) == SaveLvPos_SpellInfo_Map.end() )
			{
				SaveLvPos_SpellInfo_Map[pLearnSkill->SaveLvPos] = Info;
			}
		}

		//CUI_SkillBook::m_vecSkill[ iType ].push_back( Info );
	}

	//同一位置 只紀錄學會的 SaveLvPos
	{
		map< int , CUI_SkillBook::SpellInfo >::iterator itPos = SaveLvPos_SpellInfo_Map.begin();
		map< int , CUI_SkillBook::SpellInfo >::iterator itEnd = SaveLvPos_SpellInfo_Map.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			CUI_SkillBook::m_vecSkill[ iType ].push_back( itPos->second );
		}
	}

	if( CUI_SkillBook::m_vecSkill[ iType ].size() > 0 && iType != 0 )
	{
		//排序
		qsort( &*CUI_SkillBook::m_vecSkill[ iType ].begin() , 
			CUI_SkillBook::m_vecSkill[ iType ].size(), sizeof( CUI_SkillBook::SpellInfo ),
			SortSpellInfoCallback );
	}

	m_bSpellInfoDirty[iType] = false;

	return &m_vecSkill[iType];
}

bool CUI_SkillBook::GetPetSkillInfo(int petMagicID, vector<int>& outSkill)
{
	// SkillUpdate(ST_Pet);

	vector< SpellInfo >* pSpellInfoVector = GetSpellInfoVector( ST_Pet );
	if ( pSpellInfoVector == NULL )
		return false;	

	// 取得寵物技能
	GameObjDbStructEx* petSkillObj = g_ObjectData->GetObj(petMagicID);
	if ( petSkillObj )
	{
		int skillType;
		for ( int i = 0; i < 3; i++ )
		{
			if ( (skillType = petSkillObj->MagicBase.SummonCreature.SkillType[i]) >= 0 )
			{
				vector< SpellInfo >::iterator itPos = pSpellInfoVector->begin();
				vector< SpellInfo >::iterator itEnd = pSpellInfoVector->end();

				// 搜尋技能書類型相同				
				for( ; itPos != itEnd ; ++itPos )
				{
					SpellInfo& info = *itPos;
					GameObjDbStruct* skillOBj = g_ObjectData->GetObj(info.iGUID);
					if ( skillOBj && info.bLearned )
					{
						// 記錄類型相同法術編號
						if ( skillOBj->MagicCol.PetSkillType == skillType )
							outSkill.push_back(info.iGUID);
					}
				}
			}
		}
	}

	return true;
}


bool CUI_SkillBook::IsSkillLearned( int iType, int skillID )
{
	vector< SpellInfo >* pSpellInfoVector = GetSpellInfoVector( iType );
  	if( NULL == pSpellInfoVector )
	{
		return false;
	}

	vector< SpellInfo >::iterator itPos = pSpellInfoVector->begin();
   	vector< SpellInfo >::iterator itEnd = pSpellInfoVector->end();

	int count = (int)pSpellInfoVector->size();

	for( ; itPos != itEnd ; ++itPos )
	{
		SpellInfo& info = *itPos;

		if( info.iGUID == skillID )
		{
			if( info.bLearned )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;	
}

int CUI_SkillBook::GetFinalSkillID( int iType, int skillID )
{
	int finalSkillID = -1;

	if( iType < 0 || iType >= ST_Count )
	{
		return finalSkillID;
	}

	vector< SpellInfo >* pSpellInfoVector = GetSpellInfoVector( iType );
	if( NULL == pSpellInfoVector )
	{
		return finalSkillID;
	}

	map< int , CUI_SkillBook::SpellInfo >& SaveLvPos_SpellInfo_Map = m_SaveLvPos_SpellInfo_Map[iType];
  	map< int , int >& SkillID_SaveLvPos_Map = m_SkillID_SaveLvPos_Map[iType];


	map< int , int >::iterator itSkillID_SaveLvPos = SkillID_SaveLvPos_Map.find( skillID );
	if( itSkillID_SaveLvPos == SkillID_SaveLvPos_Map.end() )
	{
		return finalSkillID;	
	}

	int SaveLvPos = itSkillID_SaveLvPos->second;

 	map< int , CUI_SkillBook::SpellInfo >::iterator itSaveLvPos_SpellInfo = SaveLvPos_SpellInfo_Map.find( SaveLvPos );
	if( itSaveLvPos_SpellInfo == SaveLvPos_SpellInfo_Map.end() )
	{
		return finalSkillID;	
	}

	finalSkillID = itSaveLvPos_SpellInfo->second.iGUID;

	return finalSkillID;

}

int CUI_SkillBook::GetFinalSkillID( int skillID )
{
	int newSkillID = -1;
	for( int i = 0; i < ST_Count; i++ )
	{
		if ( ( newSkillID = GetFinalSkillID( i, skillID ) ) != -1 )
			return newSkillID;
	}
	return -1;
}

CUI_SkillBook::SpellInfo* CUI_SkillBook::GetSkillInfo( int iType, int iIndex )
{
 	vector< SpellInfo >* pSpellInfoVector = CUI_SkillBook::GetSpellInfoVector( iType );
	if( pSpellInfoVector )
	{
		if( iIndex < 0 || iIndex >= pSpellInfoVector->size() )
		{
			return NULL;
		}
		return &(*pSpellInfoVector)[ iIndex ];
	}

	return NULL;
}

// --------------------------------------------------------------------------------------
int	CUI_SkillBook::GetSkillGUID	( int iType, int iIndex )
{
	SpellInfo* pInfo = GetSkillInfo( iType, iIndex );
	if( NULL == pInfo )
	{
		return 0;
	}
	return pInfo->iGUID;
}

const char* g_szUnlearnSkillIconTextureName = "interface\\icons\\skill_unlearn";
// --------------------------------------------------------------------------------------
int LuaFunc_GetSkillDetail( lua_State *L )
{
	int iType  = luaL_checkint( L, 1 ) - 1;
	int iIndex = luaL_checkint( L, 2 ) - 1;

	CUI_SkillBook::SpellInfo* pInfo = CUI_SkillBook::GetSkillInfo( iType, iIndex );
	RoleDataEx* pRole = CNetGlobal::RoleData();

	if( NULL == pInfo )
	{
		return 0;
	}
   

	switch( pInfo->iGUID )
	{


	case DF_SKILLID_ATTACK:
		{
			//GameObjDbStruct*	pBaseSkillObj	= g_ObjectData->GetObj( pInfo->iGUID );

			//int					iSkillLevel		= pBaseSkillObj->MagicCol.MagicLV;
			//GameObjDbStruct*	pSKillObj		= NULL;
			GameObjDbStructEx*	pBaseSkillObj	= g_ObjectData->GetObj( pInfo->iGUID );

			if( pBaseSkillObj == NULL )
				return 0;

			int					iWeaponGUID = 0;
			GameObjDbStruct*	pWeaponObj	= NULL;
			char				szTextureFile[ 256 ];

			CUI_SkillBook::GetAttackIcon( szTextureFile );

			//g_pItemClipboard->SetFile( szTextureFile );
			//g_pItemClipboard->PickupSkill( NULL, iSkillID );

			lua_pushstring( L, pBaseSkillObj->GetName() );						// NAME
			lua_pushnumber( L, 0 );								// LV


			lua_pushstring( L, szTextureFile );			// ICON
			lua_pushnumber( L,	100			);	// 法術種類, 被動主動, 3 是攻擊技能
			lua_pushnumber( L,  0			);							// 法術威力等級
			lua_pushnumber( L,  0			);						// 法術威力點數
			lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
			lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
			lua_pushboolean( L, pInfo->bLearned         );             // 是否學會

			return 9;

		} break;
	case DF_SKILLID_PK:
	case DF_SKILLID_PORTOL:
	case DF_SKILLID_DISENCHANT:
		{
			GameObjDbStructEx*	pBaseSkillObj	= g_ObjectData->GetObj( pInfo->iGUID );
			if( pBaseSkillObj == NULL )
				return 0;

			GameObjDbStruct*	imageDB = CNetGlobal::GetObj( pBaseSkillObj->ImageID );
			if ( imageDB == NULL )
				return 0;

			//int					iSkillLevel		= pBaseSkillObj->MagicCol.MagicLV;
			//GameObjDbStruct*	pSKillObj		= NULL;

			lua_pushstring( L, pBaseSkillObj->GetName() );				// NAME
			lua_pushnumber( L, 0 );									// LV
			lua_pushstring( L, imageDB->Image.ACTField );			// ICON
			lua_pushnumber( L, 100 );	// 法術種類, 被動主動
			lua_pushnumber( L,  0			);							// 法術威力等級
			lua_pushnumber( L,  0			);						// 法術威力點數
			lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
			lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
			lua_pushboolean( L, pInfo->bLearned         );             // 是否學會

			return 9;

		} break;

	default:
		{
			GameObjDbStructEx*	pBaseSkillObj	= g_ObjectData->GetObj( pInfo->iGUID );

			if( pBaseSkillObj == NULL )
				return 0;

			GameObjDbStruct*	imageDB = CNetGlobal::GetObj( pBaseSkillObj->ImageID );
			if ( imageDB == NULL )
				return 0;

			// 回新手村
			if( pInfo->iGUID >= DF_SKILLID_BEGINNERPORTOL_BEGIN && pInfo->iGUID <= DF_SKILLID_BEGINNERPORTOL_END )
			{
				lua_pushstring( L, pBaseSkillObj->GetName() );					// NAME
				lua_pushnumber( L, 0 );										// LV
				lua_pushstring( L, imageDB->Image.ACTField );				// ICON
				lua_pushnumber( L, 100 );	// 法術種類, 被動主動
				lua_pushnumber( L,  0			);							// 法術威力等級
				lua_pushnumber( L,  0			);						// 法術威力點數
				lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
				lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
				lua_pushboolean( L, pInfo->bLearned         );             // 是否學會
				return 9;
			}

			// 生活技能
			if( pBaseSkillObj->Classification == EM_ObjectClass_KeyItem || ( pInfo->iGUID >= ( Def_ObjectClass_KeyItem + EM_SkillValueType_BlackSmith ) && pInfo->iGUID <= ( Def_ObjectClass_KeyItem + EM_SkillValueType_Herblism ) ) )
			{
				GameObjDbStructEx*	pBaseSkillObj	= g_ObjectData->GetObj( pInfo->iGUID );

				lua_pushstring( L, pBaseSkillObj->GetName() );						// NAME
				lua_pushnumber( L, 0 );								// LV
				lua_pushstring( L, imageDB->Image.ACTField );			// ICON
				lua_pushnumber( L, 100 );	// 法術種類, 被動主動
				lua_pushnumber( L,  0			);							// 法術威力等級
				lua_pushnumber( L,  0			);						// 法術威力點數
				lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
				lua_pushnumber( L,  0			);					// 法術威力點數該等級總數
				lua_pushboolean( L, pInfo->bLearned         );             // 是否學會
				return 9;
			}

			// 特殊法術
			{
				int					iSkillLevel		= pBaseSkillObj->MagicCol.MagicLV;
				GameObjDbStruct*	pSKillObj		= NULL;

				lua_pushstring( L, pBaseSkillObj->GetName() );						// NAME
				lua_pushnumber( L, iSkillLevel );								// LV

				if( pInfo->bLearned )
				{
					lua_pushstring( L, imageDB->Image.ACTField );				// ICON
				}
				else
				{
					lua_pushstring( L, g_szUnlearnSkillIconTextureName );			// ICON
				}

				lua_pushnumber( L, (int)pBaseSkillObj->MagicCol.EffectType );	// 法術種類, 被動主動

				//稱號法術
				if( iType == 0 )
				{
					lua_pushnumber( L,  pInfo->iPowerLV );				// 法術威力等級
					lua_pushnumber( L,  0			    );				// 法術威力點數
					lua_pushnumber( L,  0			    );				// 法術威力點數該等級總數
					lua_pushnumber( L,  0			    );				// 法術威力點數該等級總數
					lua_pushboolean( L, pInfo->bLearned );              // 是否學會
					return 9;
				}

				int iEnableToLV = 1;
				int iPowerPointToLV = pInfo->iPowerPointToLV; 

				// 如果人物主職等級小於該技能的威力等級, 則設定威力等級為主職等級, 並設置該法術不能配點
				if( pRole->TempData.Attr.Level <= pInfo->iPowerLV )
				{
					pInfo->iPowerLV = pRole->TempData.Attr.Level;
					iEnableToLV   = 0;
				}
	  
				if( pBaseSkillObj->MagicCol.MaxSkillLv <= pInfo->iPowerLV )
				{
					// 超過可修練等級了
					pInfo->iPowerLV = pBaseSkillObj->MagicCol.MaxSkillLv;
					iEnableToLV   = 0;				
				}

				if( pBaseSkillObj->MagicCol.MaxSkillLv == 0 )
				{
					iPowerPointToLV = 0;
				}

				lua_pushnumber( L, pInfo->iPowerLV         );             // 法術威力等級
				lua_pushnumber( L, pInfo->iPowerPoint      );             // 法術威力點數
				lua_pushnumber( L, iPowerPointToLV         );             // 法術威力點數該等級總數
				lua_pushnumber( L, iEnableToLV             );             // 法術威力點數該等級總數
				lua_pushboolean( L, pInfo->bLearned        );             // 是否學會

				return 9;
			}
		}
	}

	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetSkillTotalPointToLV( lua_State *L )
{
	int iType  = luaL_checkint( L, 1 ) - 1;
	int iIndex = luaL_checkint( L, 2 ) - 1;
	int iLevel = luaL_checkint( L, 3 );
	
	CUI_SkillBook::SpellInfo* pInfo = CUI_SkillBook::GetSkillInfo( iType, iIndex );

	if( NULL == pInfo )
	{
		return 0;
	}

	RoleDataEx* pRole = CNetGlobal::RoleData();
	GameObjDbStruct* MagicObjDB = g_ObjectData->GetObj( pInfo->iGUID );

	if( NULL == MagicObjDB )
	{
		return 0;
	}
   
	// 如果人物主職等級小於該技能的威力等級, 則設定威力等級為主職等級, 並設置該法術不能配點
	if( iLevel >= pRole->TempData.Attr.Level )
	{
		return 0;
	}
	// 超過可修練等級了
	if( iLevel >= MagicObjDB->MagicCol.MaxSkillLv )
	{
		return 0;			
	}

	int ExpMagicLv = iLevel + (int)MagicObjDB->MagicCol.ExpTableRate - 1;

	int BaseNeedExp = 0;

	if ( ExpMagicLv >=0 && ExpMagicLv < RoleDataEx::JobSkillExpTable.size() ) {
		BaseNeedExp = int( RoleDataEx::JobSkillExpTable[ ExpMagicLv ]  );
	}

	//int NeedExp = int( BaseNeedExp  * ( 1 + int( MagicLv ) - MagicLv  ) + 0.9 );

	lua_pushnumber( L, BaseNeedExp );
	return 1;
}


// --------------------------------------------------------------------------------------
int LuaFunc_GetNumSkill	( lua_State *L )
{
	int iType	= luaL_checkint( L, 1 ) - 1;

 	vector< CUI_SkillBook::SpellInfo >* pSpellInfoVector = CUI_SkillBook::GetSpellInfoVector( iType );

	if( NULL == pSpellInfoVector )
	{
		return 0;
	}
	
	lua_pushnumber( L, (lua_Number)pSpellInfoVector->size() );
	return 1;
	
}

// --------------------------------------------------------------------------------------
void CUI_SkillBook::CancleSpelling ()
{
	m_iSpellingGUID		= 0;
	//m_iSpellingIndex	= 0;
}
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
int	LuaFunc_DragSkillButton	( lua_State *L )
{	
	int iType	= luaL_checkint( L, 1 ) - 1;
	int iIndex	= luaL_checkint( L, 2 ) - 1;

	//寵物技能不可拖動
	if( iType == CUI_SkillBook::ST_Pet )
	{
		return 0;
	}

	int iSkillID = CUI_SkillBook::GetSkillGUID( iType, iIndex );

	if( iSkillID != 0 )
	{
		GameObjDbStruct*	pSkillObj	= g_ObjectData->GetObj( iSkillID );

		RoleDataEx*			pRole 			= CNetGlobal::RoleData();

		switch( g_pItemClipboard->GetType() )
		{
		case eClipboardItem_Type_None:			// 拿取物品
			{
				// 拿取法術
				// 檢查可否拿取
				char szTextureFile[ 256 ];
				memset(szTextureFile, 0, sizeof(szTextureFile));

				if( iSkillID == DF_SKILLID_ATTACK )
				{						
					CUI_SkillBook::GetAttackIcon( szTextureFile );
				}
				else
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(pSkillObj->ImageID);
					if ( imageDB )
						sprintf( szTextureFile, imageDB->Image.ACTField );
				}

				g_pItemClipboard->SetFile( szTextureFile );
				g_pItemClipboard->PickupSkill( iSkillID );

			} break;

		default:
			{
				// clear item clipboard data
				g_pItemClipboard->Clear();
			} break;
		}

	}

	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_UseSkill ( lua_State *L )
{	

	int iType	= luaL_checkint( L, 1 ) - 1;
	int iIndex	= luaL_checkint( L, 2 ) - 1;

	int iSkillID = CUI_SkillBook::GetSkillGUID( iType, iIndex );

	if( iSkillID != 0 )
	{
		GameObjDbStruct*	pSkillObj	= g_ObjectData->GetObj( iSkillID );
		CUI_SkillBook::Use( iSkillID );
	}


	return 0;
}

int CUI_SkillBook::Use	( int iSkillID )
{
	switch( iSkillID )
	{

	case DF_SKILLID_PK:
	case DF_SKILLID_PORTOL:
		{
			NetCli_Other::S_SysKeyFunctionRequest( ( iSkillID - Def_ObjectClass_KeyItem ), 0 );
			return 0;
		} break;
	default:
		{
			// 檢查是不是使用生活技能
				if( iSkillID >= ( Def_ObjectClass_KeyItem + EM_SkillValueType_BlackSmith ) && iSkillID <= ( Def_ObjectClass_KeyItem + EM_SkillValueType_Herblism ) )
				{
					int iCount = 1;
					for( vector< StructCraftType >::iterator it = g_pCraftFrame->m_vecCraftType.begin(); it != g_pCraftFrame->m_vecCraftType.end(); it++ )
					{
						StructCraftType* pCraftType = &(*it);
						if( ( pCraftType->iCraftSkillID + Def_ObjectClass_KeyItem ) == iSkillID )
						{
							lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

							lua_pushnumber( L, iCount );
							lua_setglobal( L, "arg1");
							g_pInterface->SendWorldEvent("USE_CRAFTFRAME_SKILL");
							return 0;

						}
						iCount++;
					}
				}

			// 檢查是不是新手傳送術
				if( iSkillID >= DF_SKILLID_BEGINNERPORTOL_BEGIN && iSkillID <= DF_SKILLID_BEGINNERPORTOL_END )
				{
					NetCli_Other::S_SysKeyFunctionRequest( ( iSkillID - Def_ObjectClass_KeyItem ), 0 );
					return 0;
				}
		}
	}

	return g_pGameMain->CastSpellByID(iSkillID);
}
// --------------------------------------------------------------------------------------
bool CUI_SkillBook::WaitSetTarget() 
{ 
	if( m_iSpellingGUID != 0 )
		return true; 
	else
		return false;
}

// --------------------------------------------------------------------------------------
void CUI_SkillBook::UpdateCooldown(int ColdownClass, int ColdownType)
{
	g_pUi_SkillBook->SendWorldEvent("SKILL_UPDATE_COOLDOWN");
}

// --------------------------------------------------------------------------------------
int	LuaFunc_GetTpExp( lua_State *L )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int iTP = pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].TpExp;

	lua_pushnumber( L, iTP );

	return 1;
}



// --------------------------------------------------------------------------------------
int	LuaFunc_GetTotalTpExp( lua_State *L )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int TotalTpExp = pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].TotalTpExp;

	lua_pushnumber( L, TotalTpExp );

	return 1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_SetSpellPoint( lua_State *L )
{
	int			iType		= luaL_checkint( L, 1 ) - 1;
	int			iIndex		= luaL_checkint( L, 2 ) - 1;
	RoleDataEx*	pRole		= CNetGlobal::RoleData();

	int			iSkillID	= CUI_SkillBook::GetSkillGUID( iType, iIndex );

	for( int i = 0 ; i <  _MAX_SPSkill_COUNT_+_MAX_NormalSkill_COUNT_ ; i++ )
	{
		if( pRole->PlayerTempData->Skill.AllSkill[i] == iSkillID )
		{
			NetCli_Magic::SetMagicPoint( iSkillID , i );
			return 0;
		}
	}

    return 0; // add by slime 2010/08/03
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetVocInfo( lua_State *L )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	lua_pushnumber(L, (int)pRole->BaseData.Voc);
	lua_pushstring(L, g_ObjectData->GetClassNameByID(pRole->BaseData.Voc));
	return 2;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetVocSubInfo( lua_State *L )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	lua_pushnumber(L, (int)pRole->BaseData.Voc_Sub);
	lua_pushstring(L, g_ObjectData->GetClassNameByID(pRole->BaseData.Voc_Sub));
	return 2;
}
// --------------------------------------------------------------------------------------
void CUI_SkillBook::UseAttack()
{
	CRoleSprite* target			= g_pGameMain->GetPlayerTarget();
	g_pGameMain->AttackTarget(target);
}
// --------------------------------------------------------------------------------------
void CUI_SkillBook::GetAttackIcon( char* pszIcon )
{
	char szTextureFile[ 256 ];
	memset(szTextureFile, 0, sizeof(szTextureFile));

	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	int					iWeaponGUID = pRole->BaseData.EQ.Item[ EM_EQWearPos_MainHand ].OrgObjID;
	GameObjDbStruct*	pWeaponObj	= NULL;

	if( iWeaponGUID == 0 )
	{
		// 使用空手 ICON							
		sprintf( szTextureFile, "interface//Icons//skill_att.dds" );
	}
	else
	{
		pWeaponObj	= g_ObjectData->GetObj( iWeaponGUID );
		if( pWeaponObj )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(pWeaponObj->ImageID);
			if ( imageDB )
				sprintf( szTextureFile, imageDB->Image.ACTField );
		}
	}

	if( pszIcon != NULL )
	{
		strcpy( pszIcon, szTextureFile );
	}

	//string sStrtemp = szTextureFile;
	//return sStrtemp;
}

// --------------------------------------------------------------------------------------
int	LuaFunc_GetSkillCooldown					( lua_State *L )
{
	float cooldown[2] = { 0.0f, 0.0f };

	int iType	= luaL_checkint( L, 1 ) - 1;
	int iIndex	= luaL_checkint( L, 2 ) - 1;

	int iSkillID	= CUI_SkillBook::GetSkillGUID( iType, iIndex );

	if( iSkillID != 0 )
	{
		CNetGlobal::RoleData()->GetObjectCooldown( iSkillID, cooldown);
	}

	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);

	return 2;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_CastSpellByName(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	CRoleSprite* target = NULL;

	int start = 0;
	int end = 0;
	for ( int i = 0; name[i]; i++ ) {
		if ( name[i] == '(' ) {
			start = i;
		} else if ( name[i] == ')' ) {
			end = i;
		}
	}

	char spellName[256];
	int level = 100;

	// 名稱
	if ( start > 1 ) {
		strncpy(spellName, name, min(start, 255));
	} else {
		strncpy(spellName, name, 255);
	}

	// 等級
	if ( end > start && end > 1 )
	{
		level = atoi(name + start + 1);
	}

	// 對象
	if ( lua_isstring(L, 2) )
	{
		target = g_pGameMain->GetUnitSprite(lua_tostring(L, 2));
	}	

	g_pGameMain->CastSpellByName(spellName, level, target);
	return 0;
}

// --------------------------------------------------------------------------------------
int	LuaFunc_GetSkillHyperLink( lua_State *L )
{
	int iType		= luaL_checkint( L, 1 ) - 1;
	int iIndex		= luaL_checkint( L, 2 ) - 1;

	int GUID = CUI_SkillBook::GetSkillGUID( iType, iIndex );

	int skillpoint = (int)CNetGlobal::RoleData()->GetSkillPoint( GUID );
 
	string link;
	if( !SkillGUID_To_HyperLink( GUID, skillpoint, link ) )
	{
		return 0;
	}
	lua_pushstring(L, link.c_str() );
	return 1;
}