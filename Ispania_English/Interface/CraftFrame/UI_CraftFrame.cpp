#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/CastingBarFrame.h"
#include "../WorldFrames/ChatFrame.h"
#include "../Houses/HousesFrame.h"
#include "../ItemLink/ItemLink.h"

#include "UI_CraftFrame.h"

CUI_CraftFrame*		g_pCraftFrame	= NULL;
//-------------------------------------------------------------------
int			LuaFunc_GetCraftItemList		( lua_State *L );
int			LuaFunc_GetCraftItemType		( lua_State *L );
int			LuaFunc_GetCraftItem			( lua_State *L );
int			LuaFunc_GetCraftItemInfo		( lua_State *L );
int			LuaFunc_GetCraftCatalogInfo		( lua_State *L );
int			LuaFunc_GetCraftRequestItem		( lua_State *L );
int			LuaFunc_GetMaxCraftItem			( lua_State *L );
int			LuaFunc_CreateCraftItem			( lua_State *L );
int			LuaFunc_GetCraftSubType			( lua_State *L );
int			LuaFunc_GetResourceQuality		( lua_State *L );
int			LuaFunc_GetCountInBagByName		( lua_State *L );
int			LuaFunc_GetCountInBankByName	( lua_State *L );
int			LuaFunc_GetItemType				( lua_State *L );
int			LuaFunc_GetQualityByGUID		( lua_State *L );
int			LuaFunc_GetCraftTypeIndexBySkillIDName	( lua_State *L );
int			LuaFunc_StopCreateCraftItem		( lua_State *L );
int			LuaFunc_IsWeaponOrArmor			( lua_State *L );
int			LuaFunc_GetCraftRecipeLink		( lua_State* L );
int			LuaFunc_GetCraftItemLink		( lua_State* L );


#define  DF_CRAFT_SUPERTOOLSID	7


static int get_Recipe_CoolDown(int dbid)
{
	int colldown=-1;
	RoleDataEx*		pRole		= CNetGlobal::RoleData();
	int count= pRole->PlayerBaseData->RefineColdown.Size();

	for( int i = 0 ; i < count; i++ )
	{
		RefineBuffStruct &rb=pRole->PlayerBaseData->RefineColdown[i];
		if( rb.RecipeID==dbid)
		{			
			colldown=rb.Coldown;
		}		
	}
	return colldown;
}

//-------------------------------------------------------------------
CUI_CraftFrame::CUI_CraftFrame( CInterface* object ) : CInterfaceBase( object )
{
	g_pCraftFrame		= this;
	m_iCraftItem		= 0;
	m_iCraftItemNumber	= 0;
	m_fTimeToCreate		= 0;
	m_iCraftSound		= 0;
	m_iFromQueue		= 0;
	m_bIsValid			= false;
	ZeroMemory( &m_itemField , sizeof( m_itemField ) );
}
// --------------------------------------------------------------------------------------
CUI_CraftFrame::~CUI_CraftFrame(void)
{
	g_pCraftFrame = NULL;
}
//--------------------------------------------------------------------------------------
void CUI_CraftFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "GetCraftItemList",				LuaFunc_GetCraftItemList		);	// ��l�ƨè��o��y�ޯ઺�ƶq
		lua_register( l, "GetCraftItemType",				LuaFunc_GetCraftItemType		);	// ���o��y�ޯ઺�W��
		lua_register( l, "GetCraftItem",					LuaFunc_GetCraftItem			);	// ���o��y���󪺦W�٤Ψ���
		lua_register( l, "GetCraftItemInfo",				LuaFunc_GetCraftItemInfo		);	// ���o��y���󪺦W�٤Ψ���
		lua_register( l, "GetCraftCatalogInfo",				LuaFunc_GetCraftCatalogInfo		);
		lua_register( l, "GetCraftRequestItem",				LuaFunc_GetCraftRequestItem		);
		lua_register( l, "GetMaxCraftItem",					LuaFunc_GetMaxCraftItem			);	// ���o�i�H��y���W���ƶq
		lua_register( l, "CreateCraftItem",					LuaFunc_CreateCraftItem			);	// �}�l�s�y	

		lua_register( l, "GetCraftSubType",					LuaFunc_GetCraftSubType			);	// ���o��y�ޯत�l����������
		lua_register( l, "GetResourceQuality",				LuaFunc_GetResourceQuality		);	// ���o��ƪ��}���ץH�T�w�W�r�C��
		lua_register( l, "GetCountInBagByName",				LuaFunc_GetCountInBagByName		);	// �H���~�W�٨��o��b�I�]�����ƶq
		lua_register( l, "GetCountInBankByName",			LuaFunc_GetCountInBankByName	);	// �H���~�W�٨��o��b�Ȧ椤���ƶq
		lua_register( l, "GetItemType",						LuaFunc_GetItemType				);	// �H���~ID���o���~�ʽ�Ϊ��~�ʽ�r��
		lua_register( l, "GetQualityByGUID",				LuaFunc_GetQualityByGUID		);	// �H���~ID���o���~�}����
		lua_register( l, "GetCraftTypeIndexBySkillIDName",	LuaFunc_GetCraftTypeIndexBySkillIDName	);	// �H�ޯ�ID���o�ޯ�vector��index
		lua_register( l, "StopCreateCraftItem",				LuaFunc_StopCreateCraftItem		);	// ����s�y
		lua_register( l, "IsWeaponOrArmor",					LuaFunc_IsWeaponOrArmor			);	// �P�_�O�_�Z���Ψ���(�ݭn��ܤT�إi��ʦ��~)
		lua_register( l, "GetCraftRecipeLink",				LuaFunc_GetCraftRecipeLink		);	// ���o�t�誺�s���r��
		lua_register( l, "GetCraftItemLink",				LuaFunc_GetCraftItemLink		);	// ���o���~���s���r��
	}
}
//--------------------------------------------------------------------------------------
void CUI_CraftFrame::ResetCraftInfo()
{
	int iStartID	= Def_ObjectClass_Recipe;
	int iEndID		= Def_ObjectClass_Recipe + Def_ObjectClass_Recipe_Total;

	//multimap< int, int >	mapCraft; //�ӧޯ��I��
	map< int, StructCraftType >			mapCraftType;	// �Ƨǻs�y�ޯ�Ϊ� map
	multimap< int, GameObjDbStruct* >	mapCraftItem;	// �ӧޯ��I�Ʊƪ����Ҧ���s�y���~
	RoleDataEx*							pRole				= CNetGlobal::RoleData();
	StructCraftType						CraftType;

	g_pCraftFrame->m_vecCraftType.clear();

	// �j���@���X�اޯ�
	for( int iRecipeID = iStartID; iRecipeID < iEndID; iRecipeID++ )
	{ 
		GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iRecipeID );

		if( pObj == NULL )
			continue;

		// �p�G�S�]�w�ݨD�ޯ�, �h�����J�M��
		if( pObj->Recipe.Request.Skill == 0 )
			continue;

		// �ˬd�O�_�֦��ӭ��n���~
		if(  pObj->Recipe.Request.KeyItemID != 0 )
		{
			if( pRole->CheckKeyItem( pObj->Recipe.Request.KeyItemID ) == false )
				continue;
		}
		else
		{
			continue;
		}



		// �Y���@���s���ǲߧޯ�, �K���J map ��
		if( mapCraftType.find( pObj->Recipe.Request.Skill  ) == mapCraftType.end() )
		{
			//char szBuf[64];
			//sprintf( szBuf, "��)

			CraftType.iCraftSkillID = pObj->Recipe.Request.Skill;


			switch( CraftType.iCraftSkillID )
			{
			case EM_SkillValueType_BlackSmith:	// ���K
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_BLACKSMITH" ); break;

			case EM_SkillValueType_Carpenter:	// ��u
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_CARPENTER" ); break;

			case EM_SkillValueType_MakeArmor:	// �s��
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_MAKEARMOR" ); break;

			case EM_SkillValueType_Tailor:		// ���_
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_TAILOR" ); break;

			case EM_SkillValueType_Cook:		// �i��
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_COOK" ); break;

			case EM_SkillValueType_Alchemy:		// �Ҫ�
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_ALCHEMY" ); break;

			case EM_SkillValueType_Mining:		// ���q
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_MINING" ); break;

			case EM_SkillValueType_Lumbering:		// ���
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_LUMBERING" ); break;

			case EM_SkillValueType_Herblism:		// ����
				CraftType.sSkillName = g_ObjectData->GetString( "SYS_SKILLNAME_HERBLISM" ); break;

			default:
				continue;
				//CraftType.sSkillName = "NONE"; break;
			}

			mapCraftType[ pObj->Recipe.Request.Skill ] = CraftType;
		}

		// ���J map ��, �O���ҷ|������, �H�һݪ��I�ưO��
		mapCraftItem.insert( pair< int, GameObjDbStruct* > ( pObj->Recipe.Request.SkillLV, pObj ) );

	}

	// ��z�ұo�쪺���

	// ��z�ҷ|���ޯ�, �̧ޯබ�ǱƧ�
	for( map< int, StructCraftType >::iterator it = mapCraftType.begin(); it != mapCraftType.end(); it++ )
	{
		g_pCraftFrame->m_vecCraftType.push_back( it->second );
	}

	// ��z�Ҧ����y�����~
	for( vector< StructCraftType >::iterator itType = g_pCraftFrame->m_vecCraftType.begin(); itType != g_pCraftFrame->m_vecCraftType.end(); itType++ )
	{
		int iCraftSkillID = (*itType).iCraftSkillID;

		set< int > setSubType;
		for( multimap< int, GameObjDbStruct* >::iterator it = mapCraftItem.begin(); it != mapCraftItem.end(); it++ )
		{
			GameObjDbStruct*	pObj	= it->second;

			if( pObj->Recipe.Request.Skill == iCraftSkillID )
			{
				//(*itType).mapCraftItem.insert( pair< int, int > ( pObj->Recipe.Request.SkillLV, pObj->GUID ) );
				(*itType).vecCraftItem.push_back( pObj->GUID );
				setSubType.insert( pObj->Recipe.Type );
			}
		}

		for( vector<int>::iterator it = (*itType).vecCraftItem.begin(); it != (*itType).vecCraftItem.end(); it++ )
		{
			GameObjDbStruct* pObj = g_ObjectData->GetObj( *it );
			GameObjDbStructEx* pCraftItem = g_ObjectData->GetObj( pObj->Recipe.Item1_Normal.OrgObjID );

			for( vector<int>::iterator it2 = it + 1; it2 != (*itType).vecCraftItem.end(); it2++ )
			{
				GameObjDbStruct* pObj2 = g_ObjectData->GetObj( *it2 );
				GameObjDbStructEx* pCraftItem2 = g_ObjectData->GetObj( pObj2->Recipe.Item1_Normal.OrgObjID );

				if( pObj->Recipe.Request.SkillLV > pObj2->Recipe.Request.SkillLV )
				{
					int temp = *it;
					*it = *it2;
					*it2 = temp;
					pCraftItem = pCraftItem2;
				}
			}
		}

		for( set<int>::iterator it = setSubType.begin(); it != setSubType.end(); it++ )
		{
			(*itType).vecSubType.push_back( *it );
		}
	}
}

int LuaFunc_GetCraftItemList ( lua_State *L )
{
	//int iIndex	= luaL_checkint( L, 1 ) - 1;

	g_pCraftFrame->ResetCraftInfo();
		
	// �e�X���o�ޯ઺�ƶq

		lua_pushnumber( L, (lua_Number)g_pCraftFrame->m_vecCraftType.size() );			
		return 1;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetCraftItemType ( lua_State *L )
{
	int iType		= luaL_checkint( L, 1 );
	int iTypeSize	= (int)g_pCraftFrame->m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType	CraftType; 
		CraftType = g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ];

		lua_pushnumber( L, CraftType.iCraftSkillID );
		lua_pushstring( L, CraftType.sSkillName.c_str() );		

		return 2;
	}	

	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetCraftItemInfo ( lua_State *L )
{
	int	iObjID		= luaL_checkint( L, 1 );

	bool				bCheck			= g_pCraftFrame->CheckRecipte( iObjID );
	GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iObjID );


	if( bCheck == false || pRecipteObj == NULL )
		return 0;

	

	if( pRecipteObj )
	{
		GameObjDbStructEx*	pCraftItem		= g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );
		int					iMaxCraftItem	= g_pCraftFrame->GetMaxCraftItem( iObjID );	// ���o�i��y���~���W��

		if( pCraftItem )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(pCraftItem->ImageID);

			lua_pushstring( L, pCraftItem->GetName() );

			if ( imageDB )
			{
				lua_pushstring( L, imageDB->Image.ACTField );
			}
			else
			{
				lua_pushnil(L);
			}
			
			lua_pushnumber( L, iMaxCraftItem );
			lua_pushnumber( L, pCraftItem->Rare );
			lua_pushboolean( L, pRecipteObj->Mode.VariableRare == TRUE );

			return 5;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetCraftItem ( lua_State *L )
{
	vector< int >	vecCraftItem;
	
	int iType		= luaL_checkint( L, 1 );
	int	iSubType	= luaL_checkint( L, 2 ) - 1;
	int	iIndex		= luaL_checkint( L, 3 );

	
	CUI_CraftFrame::GetCraftItemList( iType, iSubType, &vecCraftItem );
	
	if( iIndex == -1 )
	{
		int iTotalCount = (int)vecCraftItem.size();
		//return iTotalCount;
		lua_pushnumber( L, iTotalCount );
		return 1;
	}
	else
	{
		int					iObjGUID		= vecCraftItem[ ( iIndex - 1 ) ];

		GameObjDbStructEx*	pRecipteObj		= g_ObjectData->GetObj( iObjGUID );
		GameObjDbStructEx*	pCraftItem		= NULL;

		if( pRecipteObj )
		{
			pCraftItem = g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );
			StructCraftType* pCraftType = &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );

			if( pCraftItem )
			{
				int iSubSize = (int)pCraftType->vecSubType.size();
				RoleDataEx* pRole = CNetGlobal::RoleData();

				for( int i = 0; i < iSubSize; i++ )
				{
					if( pCraftType->vecSubType[i] == pRecipteObj->Recipe.Type )
					{
						lua_pushnumber( L, ( i + 2 ) );
						break;
					}
				}
		
				lua_pushstring( L, pCraftItem->GetName() );

				GameObjDbStruct* imageDB = CNetGlobal::GetObj(pCraftItem->ImageID);
				if ( imageDB )
				{
					lua_pushstring( L, imageDB->Image.ACTField );
				}
				else
				{
					lua_pushnil(L);
				}				

				lua_pushnumber( L, iObjGUID );
				lua_pushnumber( L, pCraftItem->Rare );
				lua_pushnumber( L, pRecipteObj->Recipe.Request.SkillLV );
				float lev= pRole->BaseData.SkillValue.GetSkill(pRecipteObj->Recipe.Request.Skill);
				float SkillLV=(float)pRecipteObj->Recipe.Request.SkillLV;

				if( lev>= SkillLV )
					lua_pushboolean( L, TRUE );
				else
					lua_pushboolean( L,FALSE  );

				int time=-1;
				if (pRecipteObj->Recipe.Coldown>0)
				{
					time=get_Recipe_CoolDown(pRecipteObj->GUID);
				}
					
				lua_pushnumber( L, time );
				lua_pushnumber( L, pRecipteObj->Recipe.Coldown );

				return 9;
			}	
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetCraftCatalogInfo ( lua_State *L )
{
	int iType		= luaL_checkint( L, 1 );
	int	iIndex		= luaL_checkint( L, 2 );

	int iTypeSize	= (int)g_pCraftFrame->m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType* pCraftType = &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );

		int iSubTypeSize	= (int)pCraftType->vecSubType.size();

		int iSubTypeID		= iIndex - 2;
		if( iSubTypeID < iSubTypeSize && iSubTypeID >= 0 )
		{


		}
	}


	lua_pushstring( L, "�|������" );
	return 1;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetCraftRequestItem		( lua_State *L )
{
	int		iObjID		= luaL_checkint( L, 1 );
	int		iIndex		= luaL_checkint( L, 2 );

	bool	bCheck		= g_pCraftFrame->CheckRecipte( iObjID );		// �ˬd�w�Ƿ|���M�椤���L�Ӫ��~

	GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iObjID );

	if( bCheck == false || pObj == NULL )
		return 0;


	// �ˬd�һݭ�Ƽƶq
		int iTotalRequestItem = 0;
		for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
		{
			if( pObj->Recipe.Source[ i ].OrgObjID != 0 )
			{
				iTotalRequestItem++;
			}
		}

	if( iIndex == -1 )
	{
		lua_pushnumber( L, iTotalRequestItem );

		if( pObj->Recipe.RequestCraftObjType == 0 )
		{
			lua_pushstring( L, "" );
		}
		else
		{
			char szToolName[256];
			char szString[256];
			sprintf( szToolName, "CRAFT_TOOLS_%02d", pObj->Recipe.RequestCraftObjType );
			sprintf( szString, g_ObjectData->GetString("CRAFT_STRING_REQTOOLS"),  g_ObjectData->GetString( szToolName ) );

			lua_pushstring( L, szString );
		}

		lua_pushnumber( L, pObj->Recipe.CraftFee );


		return 3;
	}
	else
	if( iIndex <= iTotalRequestItem && iIndex > 0 )
	{
		int					iRequestItemGUID	= pObj->Recipe.Source[ ( iIndex - 1 ) ].OrgObjID;
		GameObjDbStructEx*	pRequestItemObj		= g_ObjectData->GetObj( iRequestItemGUID );
		RoleDataEx*			pRole				= CNetGlobal::RoleData();

		if( pRequestItemObj )
		{
			lua_pushnumber( L, iRequestItemGUID );											// GUID
			lua_pushstring( L, pRequestItemObj->GetName() );								// �W��

			GameObjDbStruct* imageDB = CNetGlobal::GetObj(pRequestItemObj->ImageID);
			if ( imageDB )
			{
				lua_pushstring( L, imageDB->Image.ACTField );						// ICON ��m
			}
			else
			{
				lua_pushnil(L);
			}			

			int iCountOnBody = pRole->CalBodyItemCount( iRequestItemGUID, true );
			lua_pushnumber( L, iCountOnBody );									// �{���ƶq
			lua_pushnumber( L, pObj->Recipe.Source[ ( iIndex - 1 ) ].Count );	// �һݼƶq

			// �p�G�w�Ƿ|���M�椤���Ӫ��~�A���o�M��GUID
			int iResourceRecipeGUID = g_pCraftFrame->GetRecipeGUID( iRequestItemGUID );

			if( iResourceRecipeGUID == 0 )
				return 5;

			// �ˬd�һݭ�ƺ�����
			int iTotalResource = 0;
			GameObjDbStruct* pResourceRecipeObj = g_ObjectData->GetObj( iResourceRecipeGUID );
			for( int i = 0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
			{
				if( pResourceRecipeObj->Recipe.Source[i].OrgObjID != 0 )
				{
					iTotalResource++;
				}
			}

			if( iTotalResource == 1 )
			{
				int iResourceID = pResourceRecipeObj->Recipe.Source[0].OrgObjID;
				int iResourceCountOnBody = pRole->CalBodyItemCount( iResourceID, true );
				lua_pushnumber( L, iResourceCountOnBody );							// �{���ƶq
				lua_pushnumber( L, pResourceRecipeObj->Recipe.Source[0].Count );	// �һݼƶq
				lua_pushnumber( L, iResourceRecipeGUID );	// ��ƪ��t�誺guid
				return 8;
			}

			return 5;

		}

	}

	return 0;


	/*
	int iType		= luaL_checkint( L, 1 );
	int	iSubType	= luaL_checkint( L, 2 ) - 1;
	int	iIndex		= luaL_checkint( L, 3 );
	int iItemIndex	= luaL_checkint( L, 4 );

	int iTypeSize	= g_pCraftFrame->m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType*	pCraftType	= &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );

		int					iItemSize	= 0; // �i��y���~���ƶq
		vector<int>			vecSubCraftItem;
		vector<int>*		pvecCraftItem;

		if( iSubType == 0 )
		{
			// ����
			iItemSize		= pCraftType->vecCraftItem.size();
			pvecCraftItem	= &( pCraftType->vecCraftItem );
		}
		else
		{
			// ��h�F����
			if( iSubType <= pCraftType->vecSubType.size() && iSubType > 0 )
			{
				GameObjDbStruct*	pObj;
				int					iGUID;
				int					iSubTypeID = iSubType - 1;

				for( vector<int>::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++  )
				{
					iGUID = *it;
					pObj = g_ObjectData->GetObj( iGUID );

					if( pObj->Recipe.Type == pCraftType->vecSubType[ iSubTypeID ] )
					{
						vecSubCraftItem.push_back( iGUID );
					}
				}

				iItemSize		= vecSubCraftItem.size();
				pvecCraftItem	= &( vecSubCraftItem );
				

			}
			else
				return 0;
		}

*/

		//if( iIndex <= iItemSize && iIndex > 0 )
		//{

/*
			int					iObjGUID	= (*pvecCraftItem)[ ( iIndex - 1 ) ];
			GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iObjGUID );

			// �ˬd�һݭ�Ƽƶq
			int iTotalRequestItem = 0;
			for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
			{
				if( pObj->Recipe.Source[ i ].OrgObjID != 0 )
				{
					iTotalRequestItem++;
				}
			}

			if( iItemIndex == -1 )
			{
				lua_pushnumber( L, iTotalRequestItem );

				if( pObj->Recipe.RequestCraftObjType == 0 )
				{
					lua_pushstring( L, "" );
				}
				else
				{
					char szToolName[256];
					char szString[256];
					sprintf( szToolName, "CRAFT_TOOLS_%02d", pObj->Recipe.RequestCraftObjType );
					sprintf( szString, g_ObjectData->GetString("CRAFT_STRING_REQTOOLS"),  g_ObjectData->GetString( szToolName ) );

					lua_pushstring( L, szString );
				}


				return 2;
			}
			else
			if( iItemIndex <= iTotalRequestItem && iItemIndex > 0 )
			{
				int					iRequestItemGUID	= pObj->Recipe.Source[ ( iItemIndex - 1 ) ].OrgObjID;
				GameObjDbStruct*	pRequestItemObj		= g_ObjectData->GetObj( iRequestItemGUID );
				RoleDataEx*			pRole				= CNetGlobal::RoleData();

				if( pRequestItemObj )
				{
					lua_pushstring( L, pRequestItemObj->Name );								// �W��
					lua_pushstring( L, pRequestItemObj->ImageObj->ACTField );				// ICON ��m
					
					int iCountOnBody = pRole->CalBodyItemCount( iRequestItemGUID );
					lua_pushnumber( L, iCountOnBody );										// �{���ƶq
					lua_pushnumber( L, pObj->Recipe.Source[ ( iItemIndex - 1 ) ].Count );	// �һݼƶq

					return 4;


				}

			}
		}
	}

	return 0;
	*/
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetMaxCraftItem			( lua_State *L )	// ���o�i��y���󪺤W���q
{
	int iType			= luaL_checkint( L, 1 );
	int iSubType		= luaL_checkint( L, 2 ) - 1;
	int	iIndex			= luaL_checkint( L, 3 );

	int iMaxCraftItem	= g_pCraftFrame->GetMaxCraftItem( iType, iSubType, iIndex );
	
	lua_pushnumber( L, iMaxCraftItem );
	return 1;
}
//--------------------------------------------------------------------------------------
int LuaFunc_CreateCraftItem( lua_State *L ) // ��y���~
{
	int		iObjGUID		= luaL_checkint( L, 1 );
	int		iNumber			= luaL_checkint( L, 2 );
	int		iFrom			= luaL_checkint( L, 3 );
	int		iMaxCraftItem	= g_pCraftFrame->GetMaxCraftItem( iObjGUID );	// ���o�i��y���~���W��
	bool	bCheck			= g_pCraftFrame->CheckRecipte( iObjGUID );		// �ˬd�w�Ƿ|���M�椤���L�Ӫ��~
	bool	bSuccess		= false;

	if( bCheck == true && iObjGUID != 0 && iNumber > 0 && iMaxCraftItem > 0 )
	{	
		bSuccess = g_pCraftFrame->CreateCraftItem( iObjGUID, iNumber, iFrom );
	}

	lua_pushboolean( L, bSuccess );

	return 1;
}
//--------------------------------------------------------------------------------------
int LuaFunc_StopCreateCraftItem( lua_State *L ) // ����s�y���~
{
	g_pCraftFrame->StopCreateCraftItem();
	
	return 0;
}
//--------------------------------------------------------------------------------------
int	CUI_CraftFrame::GetCraftItemGUID( int iType, int iIndex )
{
	int iTypeSize	= (int)m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType*	pCraftType	= &( m_vecCraftType[ ( iType -1 ) ] );
		int					iItemSize	= (int)pCraftType->vecCraftItem.size();

		if( iIndex <= iItemSize && iIndex > 0 )
		{
			int					iObjGUID	= pCraftType->vecCraftItem[ ( iIndex - 1 ) ];
			return iObjGUID;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int	CUI_CraftFrame::GetMaxCraftItem( int iObjGUID )
{
	GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iObjGUID );
	GameObjDbStruct*	pCraftItem		= NULL;
	RoleDataEx*			pRole			= CNetGlobal::RoleData();

	if( pRecipteObj == NULL || pRecipteObj->Classification != EM_ObjectClass_Recipe )
		return 0;

	int	iMaxCraftItem	= -1;

	if( pRecipteObj )
	{
		//pCraftItem = g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );

		// �ˬd�һݭ�Ƽƶq

		for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
		{

			int iSourceItemGUID	= pRecipteObj->Recipe.Source[ i ].OrgObjID;
			int iRequestCount	= max(pRecipteObj->Recipe.Source[ i ].Count, 1);

			if( iSourceItemGUID != 0 )
			{
				int	iCountOnBody		= pRole->CalBodyItemCount( iSourceItemGUID, true );
				int iAvailableNumber	= iCountOnBody / iRequestCount;

				if( iAvailableNumber == 0 )
				{
					iMaxCraftItem = 0;
					break;
				}
				else
				{
					// �u���̤p��
					if( iMaxCraftItem == -1 )
					{
						iMaxCraftItem = iAvailableNumber;
					}
					else
					{
						if( iMaxCraftItem < iAvailableNumber )
							continue;
						else
						{
							iMaxCraftItem = iAvailableNumber;
						}
					}
				}
			}
		}

		if (pRecipteObj->Recipe.Coldown>0)
		{
			if (iMaxCraftItem>1)
				iMaxCraftItem=1;

		}


		if( iMaxCraftItem <= 0 )
			return 0;

		return iMaxCraftItem;
	}

	//lua_pushnumber( L, iMaxCraftItem );
	//return iMaxCraftItem;

	return 0;
}
//--------------------------------------------------------------------------------------
int	CUI_CraftFrame::GetMaxCraftItem( int iType, int iSubType, int iIndex )
{
	vector< int >	vecCraftItem;

	//int iType		= iTypeID;//luaL_checkint( L, 1 );
	//int	iSubType	= iSubTypeID;//luaL_checkint( L, 2 ) - 1;
	//int	iIndex		= luaL_checkint( L, 3 );

	CUI_CraftFrame::GetCraftItemList( iType, iSubType, &vecCraftItem );


	int					iObjGUID		= vecCraftItem[ ( iIndex - 1 ) ];

	GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iObjGUID );
	GameObjDbStruct*	pCraftItem		= NULL;
	RoleDataEx*			pRole			= CNetGlobal::RoleData();

	int					iMaxCraftItem	= -1;

	if( pRecipteObj )
	{
		pCraftItem = g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );

		// �ˬd�һݭ�Ƽƶq
		
		for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
		{

			int iSourceItemGUID	= pRecipteObj->Recipe.Source[ i ].OrgObjID;
			int iRequestCount	= max(pRecipteObj->Recipe.Source[ i ].Count, 1);

			if( iSourceItemGUID != 0 )
			{
				int	iCountOnBody		= pRole->CalBodyItemCount( iSourceItemGUID, true );
				int iAvailableNumber	= iCountOnBody / iRequestCount;


				if( pRecipteObj->Recipe.CraftFee != 0 )
				{
					int iNum = pRole->BodyMoney() / pRecipteObj->Recipe.CraftFee;

					if( iAvailableNumber > iNum )
					{
						iAvailableNumber = iNum;
					}
				}
				

				if( iAvailableNumber == 0 )
				{
					iMaxCraftItem = 0;
					break;
				}
				else
				{
					// �u���̤p��
					if( iMaxCraftItem == -1 )
					{
						iMaxCraftItem = iAvailableNumber;
					}
					else
					{
						if( iMaxCraftItem < iAvailableNumber )
							continue;
						else
						{
							iMaxCraftItem = iAvailableNumber;
						}
					}
				}
			}
		}
	}

	//lua_pushnumber( L, iMaxCraftItem );
	return iMaxCraftItem;




	/*
	
	int iTypeSize	= m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType*	pCraftType	= &( m_vecCraftType[ ( iType -1 ) ] );
		int					iItemSize	= pCraftType->vecCraftItem.size();

		if( iIndex <= iItemSize && iIndex > 0 )
		{
			int					iObjGUID	= pCraftType->vecCraftItem[ ( iIndex - 1 ) ];
			GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iObjGUID );
			RoleDataEx*			pRole		= CNetGlobal::RoleData();

			// �ˬd�һݭ�Ƽƶq
			int	iMaxCraftItem = 0;
			for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
			{

				int iSourceItemGUID	= pObj->Recipe.Source[ i ].OrgObjID;
				int iRequestCount	= pObj->Recipe.Source[ i ].Count;

				if( iSourceItemGUID != 0 )
				{
					int	iCountOnBody		= pRole->CalBodyItemCount( iSourceItemGUID );
					int iAvailableNumber	= iCountOnBody / iRequestCount;

					if( iAvailableNumber == 0 )
					{
						iMaxCraftItem = 0;
						break;
					}
					else
					{
						if( iMaxCraftItem == 0 )
						{
							iMaxCraftItem = iAvailableNumber;
						}
						else
							if( iMaxCraftItem > iAvailableNumber )
							{
								iMaxCraftItem = iAvailableNumber;
								if( iMaxCraftItem > 99 )
									iMaxCraftItem = 99;
							}
					}
				}
			}

			//lua_pushnumber( L, iMaxCraftItem );
			return iMaxCraftItem;
		}
	}
	*/

	return 0;
}
//--------------------------------------------------------------------------------------
void CUI_CraftFrame::StopCreateCraftItem()
{
	g_pCastingBarFrame->CastingFailed( eCastingBar_Crafting );

	m_iCraftItemNumber	= 0;
	m_iCraftItemTotal	= 0;
	m_iCraftItem		= 0;
	m_fTimeToCreate		= 0;

	// ����ʧ@�P�S��
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if( player )
	{
		player->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END );
		player->EndCastingEffect();
	}

	StopCraftSound();
}
//--------------------------------------------------------------------------------------
bool CUI_CraftFrame::CreateCraftItem( int iObjGUID, int iNumber, int iFrom )
{

	RoleDataEx*		pRole	= CNetGlobal::RoleData();
	if( pRole->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
		return false;
	}

	GameObjDbStruct*	pObj	= g_ObjectData->GetObj( iObjGUID );

	if( pObj == NULL )
		return false;

	if( pRole->BaseData.SkillValue.GetSkill( pObj->Recipe.Request.Skill ) < pObj->Recipe.Request.SkillLV )
		return false;

	m_iCraftItem		= iObjGUID;
	m_iCraftItemTotal	= iNumber;
	m_iCraftItemNumber	= 1;
	m_iFromQueue		= iFrom;
	

	// �}�l�ɶ���

	if( pObj->Recipe.CraftFee != 0 )
	{
		if( pRole->BodyMoney() < pObj->Recipe.CraftFee )
		{
			g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "CRAFT_STRING_NOMONEY" ));
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "CRAFT_STRING_NOMONEY" ));
			m_iCraftItem = 0;
			return false;
		}
	}


	// �ˬd��y���~�O�_�ݭn�u��, �Y�ݭn�h�j�M���񪫫~. �ˬd�O�_�����w���~
	if( pObj->Recipe.RequestCraftObjType != 0 )
	{
		bool						bCheck				= false;
		CRoleSprite*				pPlayer				= g_pGameMain->GetPlayer();
		int							iGUID				= 0;
		CRoleSprite*				pRoleSprite			= NULL;
		map<int, CRoleSprite*>&		spriteMap			= g_pRuneDev->GetSpriteMap();

		for( map<int, CRoleSprite*>::iterator it = spriteMap.begin(); it != spriteMap.end(); it++ )
		{
			pRoleSprite = it->second;

			if( pRoleSprite->GetCheckedStatus() == eChecked_QuestObj )
			{
				GameObjDbStructEx* pQuestObj = g_ObjectData->GetObj( pRoleSprite->GetObjectId() );

				if( pQuestObj )
				{
					if( pQuestObj->NPC.QuestObjCraftMode == pObj->Recipe.RequestCraftObjType || pQuestObj->NPC.QuestObjCraftMode == DF_CRAFT_SUPERTOOLSID )
					{
						// ���ۦP������F, �ˬd��̤������Z��
						int iDistance = (int)( pPlayer->GetPos() - pRoleSprite->GetPos() ).Magnitude();

						if( iDistance <= _MAX_ROLE_TOUCH_MAXRANGE_)
						{
							bCheck = true;
							break;
						}						
					}
				}
			}
		}

		//�b�Ыθ̻s�y

		if( g_pHousesFrame && EHousesState_Enter == g_pHousesFrame->GetHousesState() )
		{
			//g_pHousesFrame

			CRoleSprite* pSprite = NULL;

			TD_HouseItemMap &rHouseItemMap = g_pHousesFrame->GetHouseFurnishingList();
			TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
			TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
			for( ; itPos != itEnd ; ++itPos )
			{
				HouseItemDBStruct* pHouseItemDB = itPos->second;

				if( pHouseItemDB->Layout.IsShow == false ||
					pHouseItemDB->Pos >= g_pHousesFrame->m_HouseBaseInfo.MaxItemCount )
				{
					continue;
				}

				GameObjDbStructEx* pItemDB = g_ObjectData->GetObj( pHouseItemDB->Item.OrgObjID );

				if( pItemDB->Item.iCraftObjType == pObj->Recipe.RequestCraftObjType || pItemDB->Item.iCraftObjType == DF_CRAFT_SUPERTOOLSID )
				{
					pSprite = g_pHousesFrame->GetHouseRoleSpriteByItemDBID( itPos->first );

					if( NULL == pSprite || NULL == pSprite->GetRuEntity())
					{
						continue;
					}

					// ���ۦP������F, �ˬd��̤������Z��
					int iDistance = (int)( pPlayer->GetPos() - pSprite->GetPos() ).Magnitude();

					if( iDistance <= _MAX_ROLE_TOUCH_MAXRANGE_)
					{
						bCheck = true;
						break;
					}	
		 		}
			}
  
		}
	
		if( bCheck == false )
		{
			char szToolName[256];
			char szString[256];
			sprintf( szToolName, "CRAFT_TOOLS_%02d", pObj->Recipe.RequestCraftObjType );
			sprintf( szString, g_ObjectData->GetString("CRAFT_STRING_REQTOOLS"),  g_ObjectData->GetString( szToolName ) );
			g_pGameMain->SendSystemMsg( szString );
			m_iCraftItem = 0;
			return false;
		}
	}

	char				szTitle[ 256 ];
	GameObjDbStructEx*	pCraftItem		= NULL;

	// ���X�Q��y���~���W��
	if( pObj )
	{
		
		pCraftItem = g_ObjectData->GetObj( pObj->Recipe.Item1_Normal.OrgObjID );

		
		if( pCraftItem != NULL && m_iCraftItemTotal != 1 )
		{
			sprintf( szTitle, "%s %d / %d", pCraftItem->GetName(), m_iCraftItemNumber, m_iCraftItemTotal );
		}
		else
		{
			strcpy( szTitle, pCraftItem->GetName() );
		}
	}

	// ����U��
	g_pGameMain->Dismount();



	

	// �]�w��y�ɶ�
	m_fTimeToCreate = CaluateCraftTime( pRole, pObj );


	g_pCastingBarFrame->CastingStart(eCastingBar_Crafting, szTitle, m_fTimeToCreate);

	PlayCraftSound( pCraftItem->Recipe.Request.Skill, pObj->Recipe.Item1_Normal.OrgObjID );

	// ����ʧ@�P�S��
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player ) {
		player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_BEGIN);
		player->ClearCastingEffect();
		player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_CRAFT_HAND"), "p_left_hand", false);
		player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_CRAFT_HAND"), "p_right_hand", false);
	}

	return true;
}
//--------------------------------------------------------------------------------------
float CUI_CraftFrame::CaluateCraftTime( RoleDataEx*	pRole, GameObjDbStruct* pObj )
{
	// �]�w��y�ɶ�
	float fTimeToCreate = (float)_TIME_CLICKTOGETITEM;

	if( pRole->TempData.Attr.Mid.Body[ EM_WearEqType_CraftSpeedRate ] != 0 )
	{
		float fRate = pRole->TempData.Attr.Mid.Body[ EM_WearEqType_CraftSpeedRate ] / (float)100;			
		fTimeToCreate	= fTimeToCreate / ( 1 + fRate );
	}

	fTimeToCreate		= fTimeToCreate / 1000;

	if( g_pHousesFrame && EHousesState_Enter == g_pHousesFrame->GetHousesState() )
	{
		int iPoint = 0;
		switch( pObj->Recipe.Request.Skill )
		{
		case EM_SkillValueType_BlackSmith:		iPoint = g_pHousesFrame->m_BlackSmithHQPoint;	break;
		case EM_SkillValueType_Carpenter:		iPoint = g_pHousesFrame->m_CarpenterHQPoint;	break;
		case EM_SkillValueType_MakeArmor:		iPoint = g_pHousesFrame->m_MakeArmorHQPoint;	break;
		case EM_SkillValueType_Tailor:			iPoint = g_pHousesFrame->m_TailorHQPoint;		break;
		case EM_SkillValueType_Cook:			iPoint = g_pHousesFrame->m_CookHQPoint;			break;
		case EM_SkillValueType_Alchemy:			iPoint = g_pHousesFrame->m_AlchemyHQPoint;		break;
		}

		if( iPoint > 1000 )
			iPoint = 1000;

		if( iPoint < 0 )
			iPoint = 0;

		fTimeToCreate = ((float)fTimeToCreate * ( ( 5 - ((float)iPoint * 0.004f) ) / 5 ));

		if( fTimeToCreate < 0 )
		{
			fTimeToCreate = 1;
		}
	}

	return fTimeToCreate;
}
//--------------------------------------------------------------------------------------
void CUI_CraftFrame::Update	(float elapsedTime)
{
	if( m_iCraftItem != 0 )
	{
		GameObjDbStructEx*	pObj	= g_ObjectData->GetObj( m_iCraftItem );
		CRoleSprite*		player	= g_pGameMain->GetPlayer();
		RoleDataEx*			pRole	= CNetGlobal::RoleData();

		if ( player  == NULL )
		{
			StopCraftSound();
			return;
		}

		if( pObj == NULL )
			return;


		// �����ˬd
		// �ˬd���_, �Y���񤤰ʧ@���O���w���ʧ@, ��ʧ@���_�F
			CRuFusion_ActorState actplaying = player->GetActorState(1);
			if( !( actplaying == ruFUSION_ACTORSTATE_CRAFTING_LOOP || actplaying == ruFUSION_ACTORSTATE_CRAFTING_BEGIN ) )
			{
				g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);
				m_iCraftItemNumber	= 0;
				m_iCraftItemTotal	= 0;
				m_iCraftItem		= 0;
				m_fTimeToCreate		= 0;

				// ����ʧ@�P�S��
				CRoleSprite* player = g_pGameMain->GetPlayer();
				if ( player ) {
					//player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END);
					player->EndCastingEffect();
				}

				StopCraftSound();

				if( m_iFromQueue == 0 )
					g_pCraftFrame->SendWorldEvent( "CRAFT_STOP_CREATE" );
				else
					g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_STOP_CREATE" );

				return;
			}


		// �p�G����������, �h���_
			if( pObj->Recipe.CraftFee != 0 )
			{
				if( pRole->BodyMoney() < pObj->Recipe.CraftFee )
				{
					g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "CRAFT_STRING_NOMONEY" ));
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "CRAFT_STRING_NOMONEY" ));

					g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);
					m_iCraftItemNumber	= 0;
					m_iCraftItemTotal	= 0;
					m_iCraftItem		= 0;
					m_fTimeToCreate		= 0;

					// ����ʧ@�P�S��
					CRoleSprite* player = g_pGameMain->GetPlayer();
					if ( player ) 
					{
						player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END);
						player->EndCastingEffect();
					}

					StopCraftSound();

					if( m_iFromQueue == 0 )
						g_pCraftFrame->SendWorldEvent( "CRAFT_STOP_CREATE" );
					else
						g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_NEXT_CREATE" );

					return;
				}
			}

			int pos = pRole->SearchBodyFirstFit( NULL, false );			// ��M�@�ӥi�H���J�I�]����m			

			if( pos == -1 )
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "SYS_BAG_ENOUGH_CAPACITY" ));
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "SYS_BAG_ENOUGH_CAPACITY" ));

				g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);
				m_iCraftItemNumber	= 0;
				m_iCraftItemTotal	= 0;
				m_iCraftItem		= 0;
				m_fTimeToCreate		= 0;				

				// ����ʧ@�P�S��
				CRoleSprite* player = g_pGameMain->GetPlayer();
				if ( player ) 
				{
					player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END);
					player->EndCastingEffect();
				}

				StopCraftSound();

				if( m_iFromQueue == 0 )
					g_pCraftFrame->SendWorldEvent( "CRAFT_STOP_CREATE" );
				else
					g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_NEXT_CREATE" );

				return;			
			}



		m_fTimeToCreate -= elapsedTime;
		
		if( m_fTimeToCreate > 0 )
			return;



			NetCli_RecipeChild::SL_RefineItem( m_iCraftItem );
		//	NetCli_RecipeChild::RL_RefineCooldown( m_iCraftItem );
		


			// �e�X�ƥ� Interface
			//g_pGameMain->GetInterface()->GetUiWorld()->SendEvent( UiEvent_CraftItemDone );

			if( m_iCraftItemNumber == m_iCraftItemTotal )
			{
				if( m_iCraftItemTotal == 0 )
					return;

				g_pCastingBarFrame->CastingStop(eCastingBar_Crafting);

				// ����ʧ@�P�S��
				CRoleSprite* player = g_pGameMain->GetPlayer();
				if ( player ) {
					player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END);
					player->EndCastingEffect();
				}

				m_iCraftItemNumber	= 0;
				m_iCraftItemTotal	= 0;
				m_iCraftItem		= 0;
				m_fTimeToCreate		= 0;				

				StopCraftSound();

				if( m_iFromQueue == 0 )
					g_pCraftFrame->SendWorldEvent( "CRAFT_STOP_CREATE" );
				else
					g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_DELETE_CREATE" );
			}
			else
			{
				m_iCraftItemNumber++;

				// �]�w��y�ɶ�
				m_fTimeToCreate		= g_pCraftFrame->CaluateCraftTime( pRole, pObj );


				char				szTitle[ 256 ];

				GameObjDbStructEx* pCraftItem = g_ObjectData->GetObj( pObj->Recipe.Item1_Normal.OrgObjID );
				sprintf( szTitle, "%s %d / %d", pCraftItem->GetName(), m_iCraftItemNumber, m_iCraftItemTotal );
				
				g_pCastingBarFrame->CastingStop(eCastingBar_Crafting);
				g_pCastingBarFrame->CastingStart(eCastingBar_Crafting, szTitle, m_fTimeToCreate);

				lua_State* L = m_interface->GetUi()->GetLuaState();

				int iRemain = m_iCraftItemTotal - m_iCraftItemNumber + 1;

				lua_pushnumber( L, iRemain );
				lua_setglobal( L, "arg1" );
				if( m_iFromQueue == 0 )
					g_pCraftFrame->SendWorldEvent( "CRAFT_NUMBER_CHANGED" );
				else
					g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_NUMBER_CHANGED" );
			}
	}
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetCraftSubType		( lua_State *L )
{
	int iType			= luaL_checkint( L, 1 );
	int	iIndex			= luaL_checkint( L, 2 );

	int iTypeSize	= (int)g_pCraftFrame->m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType*	pCraftType	= &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );
		int					SubTypeSize	= (int)pCraftType->vecSubType.size() + 1; 

		if( iIndex == -1 )
		{
			lua_pushnumber( L, SubTypeSize );
			return 1;
		}
		else
		{
			if( iIndex <= SubTypeSize && iIndex > 0 )
			{
				if( iIndex == 1 )
				{
					const char* pString =  g_ObjectData->GetString( "CRAFT_TYPELIST_ALL" );
					lua_pushstring( L, pString );
					return 1;
				}
				else
				{
					char szKey[ 256 ];

					//int ID = pCraftType->vecSubType[ ( iIndex - 2 ) ];
					int ID = pCraftType->vecSubType[ ( iIndex - 2 ) ];
					
					sprintf( szKey, "CRAFT_TYPELIST_%02d", ( ID + 1 ) );

					const char* pString =  g_ObjectData->GetString( szKey );

					lua_pushstring( L, pString );
					return 1;
				}
			}
		}
	}

	return 0;
}

int CUI_CraftFrame::GetCraftRequestItemGUID( int iRecipteObjID, int iCraftRequestItemIndex )
{
	bool				bCheck		= CheckRecipte( iRecipteObjID );		// �ˬd�w�Ƿ|���M�椤���L�Ӱt��
	GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iRecipteObjID );

	if( bCheck == false || pObj == NULL )
		return 0;

	// �ˬd�һݭ�Ƽƶq
		int iTotalRequestItem = 0;
		for( int i=0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
		{
			if( pObj->Recipe.Source[ i ].OrgObjID != 0 )
			{
				iTotalRequestItem++;
			}
		}

		if( iCraftRequestItemIndex <= iTotalRequestItem && iCraftRequestItemIndex > 0 && _MAX_RECIPE_SOURCE_COUNT_ > iCraftRequestItemIndex)
		{
			int					iRequestItemGUID	= pObj->Recipe.Source[ ( iCraftRequestItemIndex - 1 ) ].OrgObjID;
			GameObjDbStruct*	pRequestItemObj		= g_ObjectData->GetObj( iRequestItemGUID );

			if( pRequestItemObj )
			{
				return pRequestItemObj->GUID;
			}

		}

	return 0;
}

int CUI_CraftFrame::GetCraftItemGUID( int iRecipteObjID )
{
	bool				bCheck		= CheckRecipte( iRecipteObjID );		// �ˬd�w�Ƿ|���M�椤���L�Ӱt��
	GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iRecipteObjID );

	if( bCheck == false || pObj == NULL )
		return 0;

	if( pObj )
	{
		int					iCraftItemGUID	= pObj->Recipe.Item1_Normal.OrgObjID;

		if( g_ObjectData->GetObj( iCraftItemGUID ) != NULL ) 
			return  iCraftItemGUID;
	}


	return 0;
}

void CUI_CraftFrame::GetCraftItemList ( int iCraftType, int iCraftSubType, vector<int>*	pvecCraftItem )
{
	int iType		= iCraftType;
	int	iSubType	= iCraftSubType;
	//int	iIndex		= iCraftItemIndex;

	int iTypeSize	= (int)g_pCraftFrame->m_vecCraftType.size();

	if( pvecCraftItem == NULL )
		return;

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType* pCraftType = &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );

		int				iItemSize			= 0; // �i��y���~���ƶq
		vector<int>		vecSubCraftItem;
		//vector<int>*	pvecCraftItem;

		if( iSubType == 0 )
		{
			// ����
			iItemSize		= (int)pCraftType->vecCraftItem.size();
			*pvecCraftItem	= pCraftType->vecCraftItem;
		}
		else
		{
			// ��h�F����
			if( iSubType <= pCraftType->vecSubType.size() && iSubType > 0 )
			{
				GameObjDbStruct*	pObj;
				int					iGUID;
				int					iSubTypeID = iSubType - 1;

				for( vector<int>::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++  )
				{
					iGUID = *it;
					pObj = g_ObjectData->GetObj( iGUID );

					if( pObj->Recipe.Type == pCraftType->vecSubType[ iSubTypeID ] )
					{
						vecSubCraftItem.push_back( iGUID );
					}
				}
				iItemSize		= (int)vecSubCraftItem.size();
				*pvecCraftItem	= vecSubCraftItem;
			}
			else
				return;
		}
	}



	return;

}

int CUI_CraftFrame::GetRecipteInfo ( int iCraftType, int iCraftSubType, int iCraftItemIndex )
{
	int iType		= iCraftType;
	int	iSubType	= iCraftSubType;
	int	iIndex		= iCraftItemIndex;

	int iTypeSize	= (int)g_pCraftFrame->m_vecCraftType.size();

	if( iType <= iTypeSize && iType > 0 )
	{
		StructCraftType* pCraftType = &( g_pCraftFrame->m_vecCraftType[ ( iType -1 ) ] );

		int				iItemSize			= 0; // �i��y���~���ƶq
		vector<int>		vecSubCraftItem;
		vector<int>*	pvecCraftItem;

		if( iSubType == 0 )
		{
			// ����
			iItemSize		= (int)pCraftType->vecCraftItem.size();
			pvecCraftItem	= &( pCraftType->vecCraftItem );
		}
		else
		{
			// ��h�F����
			if( iSubType <= pCraftType->vecSubType.size() && iSubType > 0 )
			{
				GameObjDbStruct*	pObj;
				int					iGUID;
				int					iSubTypeID = iSubType - 1;

				for( vector<int>::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++  )
				{
					iGUID = *it;
					pObj = g_ObjectData->GetObj( iGUID );

					if( pObj->Recipe.Type == pCraftType->vecSubType[ iSubTypeID ] )
					{
						vecSubCraftItem.push_back( iGUID );
					}
				}
				iItemSize		= (int)vecSubCraftItem.size();
				pvecCraftItem	= &( vecSubCraftItem );
			}
			else
				return 0;
		}

		if( iIndex == -1 )
		{
			//lua_pushnumber( L, iItemSize );	 
			//return 1;
			return iItemSize;
		}
		else
		if( iIndex <= iItemSize && iIndex > 0 )
		{
			int					iObjGUID		= (*pvecCraftItem)[ ( iIndex - 1 ) ];
			return iObjGUID;

			/*
			GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iObjGUID );
			GameObjDbStruct*	pCraftItem		= NULL;

			if( pRecipteObj )
			{
				pCraftItem = g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );

				if( pCraftItem )
				{
					int iSubSize = pCraftType->vecSubType.size();

					for( int i = 0; i < iSubSize; i++ )
					{
						if( pCraftType->vecSubType[i] == pRecipteObj->Recipe.Type )
						{
							lua_pushnumber( L, ( i + 2 ) );
						}
					}
					//lua_pushnumber( L, ( pRecipteObj->Recipe.Type + 2 ) );		// ����
					//int i
					//lua_pushnumber( L, ( iSubType + 1 ) );
					lua_pushstring( L, pCraftItem->Name );
					lua_pushstring( L, pCraftItem->ImageObj->ACTField );
				}

				return 3;
			}
			*/

		}
	}

	return 0;
}

bool CUI_CraftFrame::CheckRecipte( int iRecipteObjID )
{
	for( vector< StructCraftType >::iterator itType = m_vecCraftType.begin(); itType != m_vecCraftType.end(); itType++ )
	{
		StructCraftType* pCraftType = &( *itType );

		for( vector< int >::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++ )
		{
			if( *it == iRecipteObjID )
				return true;
		}
	}

	return false;
}

bool CUI_CraftFrame::CheckRequestItem( int iRequestItemID )
{
	if( iRequestItemID == 0 )
		return false;

	for( vector< StructCraftType >::iterator itType = m_vecCraftType.begin(); itType != m_vecCraftType.end(); itType++ )
	{
		StructCraftType* pCraftType = &( *itType );

		for( vector< int >::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++ )
		{
			GameObjDbStruct* pObj = g_ObjectData->GetObj( *it );

			for( int i = 0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
			{
				if( pObj->Recipe.Source[ i ].OrgObjID == 0 )
					break;
				else if( pObj->Recipe.Source[ i ].OrgObjID == iRequestItemID )
					return true;
			}
		}
	}

	return false;
}

void CUI_CraftFrame::PlayCraftSound( int iCraftType, int iObjID )
{
}

void CUI_CraftFrame::StopCraftSound()
{
}

void CUI_CraftFrame::SetCraftItemField( bool bIsValid, int iItemGUID, int iButtonID )
{
	m_bIsValid = bIsValid;

	if( !bIsValid )
		return;

	int newItem = g_ObjectData->GetLifeSkillRareExID(iItemGUID, iButtonID - 1);
	if ( newItem != 0 )
	{
		iItemGUID = newItem;
		m_itemField.Rare		= 0;
	}
	else
	{
		m_itemField.Rare		= iButtonID - 1;
	}
	
	m_itemField.OrgObjID		= iItemGUID;
	m_itemField.ImageObjectID	= iItemGUID;	
	m_itemField.OrgQuality 		= m_itemField.Quality				= 100;
	m_itemField.PowerQuality	= 10;
	m_itemField.Durable			= m_itemField.OrgQuality * 100;
}

ItemFieldStruct* CUI_CraftFrame::GetCraftItemField()
{
	if( m_bIsValid )
		return &m_itemField;
	else
		return NULL;
}

int CUI_CraftFrame::GetRecipeGUID ( int iCraftObjID )
{
	for( vector< StructCraftType >::iterator itType = m_vecCraftType.begin(); itType != m_vecCraftType.end(); itType++ )
	{
		StructCraftType* pCraftType = &( *itType );

		for( vector< int >::iterator it = pCraftType->vecCraftItem.begin(); it != pCraftType->vecCraftItem.end(); it++ )
		{
			int iRecipeObjID = *it;
			GameObjDbStruct* pRecipeObj = g_ObjectData->GetObj( iRecipeObjID );

			if( pRecipeObj->Recipe.Item1_Normal.OrgObjID == iCraftObjID )
				return iRecipeObjID;
		}
	}

	return 0;
}

void CUI_CraftFrame::GetRuneHoleTooltip( char *szText, int iRecpiteGUID )
{
	GameObjDbStruct* pRecipeObj = g_ObjectData->GetObj( iRecpiteGUID );

	int flag = 0;

	if( pRecipeObj->Recipe.Item1_Slot1.Rate != 0 )
		flag += 0x0001;
	if( pRecipeObj->Recipe.Item1_Slot2.Rate != 0 )
		flag += 0x0010;
	if( pRecipeObj->Recipe.Item1_Slot3.Rate != 0 )
		flag += 0x0100;
	if( pRecipeObj->Recipe.Item1_Slot4.Rate != 0 )
		flag += 0x1000;

	switch( flag )
	{
	case 0:
		sprintf( szText, "" );
		break;
	case 0x0001:
		sprintf( szText, "%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1 );
		break;
	case 0x0010:
		sprintf( szText, "%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 2 );
		break;
	case 0x0100:
		sprintf( szText, "%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 3 );
		break;
	case 0x1000:
		sprintf( szText, "%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 4 );
		break;
	case 0x0011:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 2 );
		break;
	case 0x0111:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 3 );
		break;
	case 0x1111:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 4 );
		break;
	case 0x0110:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 2, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 3 );
		break;
	case 0x1110:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 2, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 4 );
		break;
	case 0x1100:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 3, g_ObjectData->GetString( "CRAFT_SWUNG_DASH" ), 4 );
		break;
	case 0x0101:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_COMMA" ), 3 );
		break;
	case 0x1001:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_COMMA" ), 4 );
		break;
	case 0x1010:
		sprintf( szText, "%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 2, g_ObjectData->GetString( "CRAFT_COMMA" ), 4 );
		break;
	case 0x1011:
		sprintf( szText, "%s%d%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_COMMA" ), 2, g_ObjectData->GetString( "CRAFT_COMMA" ), 4 );
		break;
	case 0x1101:
		sprintf( szText, "%s%d%s%d%s%d", g_ObjectData->GetString( "CRAFT_RUNEHOLES" ), 1, g_ObjectData->GetString( "CRAFT_COMMA" ), 3, g_ObjectData->GetString( "CRAFT_COMMA" ), 4 );
		break;
	}
}

void CUI_CraftFrame::OpenCraftUI( int lifeSkillID )
{
	lua_State* L = GetLuaState();
	if ( L ) {
		int count = 1;
		for( vector< StructCraftType >::iterator it = m_vecCraftType.begin(); it != m_vecCraftType.end(); it++ )
		{
			StructCraftType* pCraftType = &(*it);
			if( pCraftType->iCraftSkillID + Def_ObjectClass_KeyItem == lifeSkillID )
			{
				lua_pushnumber( L, count );
				lua_setglobal( L, "arg1");
				g_pInterface->SendWorldEvent("USE_CRAFTFRAME_SKILL");
				return;

			}
			count++;
		}
	}

	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_NOTLEARN_SKILL"));
}

int LuaFunc_GetResourceQuality( lua_State *L )
{
	int		iRecipeID	= luaL_checkint( L, 1 );
	int		iIndex		= luaL_checkint( L, 2 );

	bool	bCheck		= g_pCraftFrame->CheckRecipte( iRecipeID );		// �ˬd�w�Ƿ|���M�椤���L�Ӫ��~

	GameObjDbStruct*	pRecipeObj		= g_ObjectData->GetObj( iRecipeID );

	if( bCheck == false || pRecipeObj == NULL )
		return 0;

	// �ˬd�һݭ�Ƽƶq
	int iNumOfResourceKinds = 0;
	for( int i = 0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
	{
		if( pRecipeObj->Recipe.Source[ i ].OrgObjID != 0 )
		{
			iNumOfResourceKinds++;
		}
	}

	if( iIndex <= 0 || iIndex > iNumOfResourceKinds )
		return 0;

	int					iResourceGUID		= pRecipeObj->Recipe.Source[ ( iIndex - 1 ) ].OrgObjID;
	GameObjDbStructEx*	pResourceObj		= g_ObjectData->GetObj( iResourceGUID );

	if( pResourceObj )
		lua_pushnumber( L, pResourceObj->Rare );	// �}����
	else
		lua_pushnumber( L, 0 );

	return 1;
}

int LuaFunc_GetCountInBagByName( lua_State *L )
{
	const char* itemName	= luaL_checkstring(L, 1);
	RoleDataEx* pRole		= CNetGlobal::RoleData();
	int			OrgID		= pRole->GetBodyItemIDByName( itemName );

	if( OrgID == 0 )
		lua_pushnumber( L, 0 );
	else
		lua_pushnumber( L, pRole->CalBodyItemCount( OrgID, true ) );

	return 1;
}

int LuaFunc_GetCountInBankByName( lua_State *L )
{
	const char* itemName	= luaL_checkstring(L, 1);
	RoleDataEx* pRole		= CNetGlobal::RoleData();
	int			OrgID		= pRole->GetBankItemIDByName( itemName );

	if( OrgID == 0 )
		lua_pushnumber( L, 0 );
	else
		lua_pushnumber( L, pRole->CalBankItemCount( OrgID ) );

	return 1;
}

int LuaFunc_GetItemType( lua_State *L )
{
	int					iRecipeID	= luaL_checkint( L, 1 );
	int					iIndex		= luaL_checkint( L, 2 );
	bool				bCheck		= g_pCraftFrame->CheckRecipte( iRecipeID );		// �ˬd�w�Ƿ|���M�椤���L�Ӱt��
	GameObjDbStruct*	pRecipeItem	= g_ObjectData->GetObj( iRecipeID );

	if( bCheck == false || pRecipeItem == NULL )
		return 0;

	// �ˬd�һݭ�Ƽƶq
	int iTotalRequestItem = 0;
	for( int i = 0; i < _MAX_RECIPE_SOURCE_COUNT_; i++ )
	{
		if( pRecipeItem->Recipe.Source[ i ].OrgObjID != 0 )
		{
			iTotalRequestItem++;
		}
	}

	if( iIndex <= iTotalRequestItem && iIndex > 0 )
	{
		int					iRequestItemGUID	= pRecipeItem->Recipe.Source[ ( iIndex - 1 ) ].OrgObjID;
		GameObjDbStructEx*	pRequestItemObj		= g_ObjectData->GetObj( iRequestItemGUID );
		RoleDataEx*			pRole				= CNetGlobal::RoleData();

		if( pRequestItemObj )
		{
			char tmp[512];
			const char *pString;

			sprintf( tmp, "SYS_ITEMTYPE_%02d", pRequestItemObj->Item.ItemType );
			pString = g_ObjectData->GetString( tmp );

			lua_pushnumber( L, pRequestItemObj->Item.ItemType );
			lua_pushstring( L, pString );

			return 2;
		}
	}

	return 0;	
}

int LuaFunc_GetQualityByGUID( lua_State *L )
{
	int					iGUID		= luaL_checkint( L, 1 );
	GameObjDbStructEx*	pItem		= g_ObjectData->GetObj( iGUID );

	if( pItem == NULL )
		lua_pushnumber( L, 0 );
	else
		lua_pushnumber( L, pItem->Rare );

	return 1;
}

int LuaFunc_GetCraftTypeIndexBySkillIDName( lua_State *L )
{
	const char* str					= luaL_checkstring( L, 1 );
	int			type				= g_ObjectData->GetSkillValueTypeENUM( str );
	int			iTypeSize			= (int)g_pCraftFrame->m_vecCraftType.size();

	for( int i = 0; i < iTypeSize; ++i )
	{
		if( type == g_pCraftFrame->m_vecCraftType[i].iCraftSkillID )
		{
			lua_pushnumber( L, i + 1 );
			return 1;
		}
	}

	return 0;
}

int LuaFunc_IsWeaponOrArmor( lua_State *L )
{
	vector< int >	vecCraftItem;
	
	int iType		= luaL_checkint( L, 1 );
	int	iSubType	= luaL_checkint( L, 2 ) - 1;
	int	iIndex		= luaL_checkint( L, 3 );
	
	CUI_CraftFrame::GetCraftItemList( iType, iSubType, &vecCraftItem );
	
	int	iObjGUID	= vecCraftItem[ ( iIndex - 1 ) ];

	GameObjDbStructEx*	pRecipteObj		= g_ObjectData->GetObj( iObjGUID );
	GameObjDbStructEx*	pCraftItem		= NULL;

	if( pRecipteObj )
	{
		pCraftItem = g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );

		if( pCraftItem )
		{
			if( pCraftItem->Item.Class == EM_Item_Weapon || pCraftItem->Item.Class == EM_Item_Armor )
				lua_pushboolean( L, true );
			else
				lua_pushboolean( L, false );

			return 1;
		}
	}

	return 0;
}

int	LuaFunc_GetCraftRecipeLink(lua_State* L)
{
	int	iObjID		= luaL_checkint( L, 1 );

	bool				bCheck			= g_pCraftFrame->CheckRecipte( iObjID );
	GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iObjID );

	if( bCheck == false || pRecipteObj == NULL )
		return 0;

	if( pRecipteObj )
	{
		string link;

		if( !ItemGUID_To_ItemLink( pRecipteObj->GUID, link ) )
		{
			return 0;
		}

		lua_pushstring( L, link.c_str() );

		return 1;
	}

	return 0;
}

int	LuaFunc_GetCraftItemLink(lua_State* L)
{
	int	iRecpiteGUID	= luaL_checkint( L, 1 );
	int iButtonID		= luaL_checkint( L, 2 );

	bool				bCheck			= g_pCraftFrame->CheckRecipte( iRecpiteGUID );
	GameObjDbStruct*	pRecipteObj		= g_ObjectData->GetObj( iRecpiteGUID );

	if( bCheck == false || pRecipteObj == NULL )
		return 0;

	if( pRecipteObj )
	{
		GameObjDbStructEx*	pCraftItem	= g_ObjectData->GetObj( pRecipteObj->Recipe.Item1_Normal.OrgObjID );		

		if( pCraftItem )
		{
			ItemFieldStruct itemField;

			ZeroMemory( &itemField , sizeof( itemField ) );

			itemField.Rare				= iButtonID - 1;
			itemField.Mode.Trade		= true;
			itemField.Mode.ClientCreate	= true;
			itemField.OrgObjID			= pCraftItem->GUID;
			itemField.ImageObjectID		= pCraftItem->GUID;
			itemField.OrgQuality 		= itemField.Quality	= 100;
			itemField.PowerQuality		= 10;
			itemField.Durable			= itemField.OrgQuality * 100;

			string link;

			if( !ItemField_To_ItemLink( &itemField, link, false ) )
			{
				return 0;
			}

			lua_pushstring( L, link.c_str() );

			return 1;
		}		
	}

	return 0;
}