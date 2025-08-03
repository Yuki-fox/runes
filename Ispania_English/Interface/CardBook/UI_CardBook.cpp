#include "../../mainproc/GameMain.h"					// For Obj and Role struct
#include "../../netwaker_member/NetWakerClientInc.h"	// For NetGlobal

#include "../WorldFrames/ItemClipboard.h"
#include "../Debug/DebugMessage.h"
#include "./UI_CardBook.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_CardBook*		g_pUi_CardBook						= NULL;
//-------------------------------------------------------------------
int	LuaFunc_GetString							( lua_State *L );	
int LuaFunc_InitCardInfo						( lua_State *L );
int LuaFunc_GetCardMaxCount						( lua_State *L );
int LuaFunc_GetCardCount						( lua_State *L );
int LuaFunc_GetCardInfo							( lua_State *L );
int LuaFunc_ShowCardImage						( lua_State *L );
int LuaFunc_TakeOutCard							( lua_State *L );
//-------------------------------------------------------------------
CUI_CardBook::CUI_CardBook( CInterface* object ) : CInterfaceBase( object )
{
	g_pUi_CardBook		= this;
	_UI_CardImage		= NULL;
}
// --------------------------------------------------------------------------------------
CUI_CardBook::~CUI_CardBook(void)
{
	g_pUi_CardBook	= NULL;
}
// --------------------------------------------------------------------------------------
void CUI_CardBook::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "LuaFunc_GetString"		,LuaFunc_GetString			);		
		lua_register( l, "LuaFunc_InitCardInfo"		,LuaFunc_InitCardInfo		);		
		lua_register( l, "LuaFunc_GetCardMaxCount"	,LuaFunc_GetCardMaxCount	);		
		lua_register( l, "LuaFunc_GetCardCount"		,LuaFunc_GetCardCount		);		
		lua_register( l, "LuaFunc_GetCardInfo"		,LuaFunc_GetCardInfo		);
		lua_register( l, "LuaFunc_ShowCardImage"	,LuaFunc_ShowCardImage		);
  		lua_register( l, "LuaFunc_TakeOutCard"		,LuaFunc_TakeOutCard		);
	}
}
// --------------------------------------------------------------------------------------
bool	CompMonsterCardPriority( int Card2 , int Card1 )
{
	GameObjDbStructEx* CardObj1		= g_ObjectData->GetObj( Card1 );
	GameObjDbStructEx* CardObj2		= g_ObjectData->GetObj( Card2 );


	if( CardObj1->Rare < CardObj2->Rare )
		return true;
	else	if( CardObj1->Rare > CardObj2->Rare )
		return false;
	else
	{
		if( CardObj1->Item.CardPriorityID == CardObj2->Item.CardPriorityID  )
			return ( Card1 > Card2 );

		return ( CardObj1->Item.CardPriorityID < CardObj2->Item.CardPriorityID );
	}
	
}

void CUI_CardBook::InitCardInfo()
{
	for( int i = 0 ; i < EM_Max_Race ; i++ )
		_MonsterCard[ i ].clear();

	for( int i = 0 ; i < 10000 ; i++ )
	{
		GameObjDbStructEx* CardObj = g_ObjectData->GetObj( i+Def_ObjectClass_Card );
		if( CardObj == NULL )
			continue;
		GameObjDbStructEx* OrgObj = g_ObjectData->GetObj( CardObj->Item.CardOrgObjID );
		if( OrgObj->IsNPC() )
		{
			//依種足分類
			_MonsterCard[ OrgObj->NPC.Race ].push_back( i+Def_ObjectClass_Card );
		}
	}

	for( int i = 0 ; i < EM_Max_Race ; i++ )
	{
		std::sort( _MonsterCard[i].begin() , _MonsterCard[i].end() , CompMonsterCardPriority );
	}

	_UI_CardImage = (CUiModel* )m_interface->GetUiWorld()->FindObject( "CardBookFrameCardImage" );
}

// ----------------------------------------------------------------------------
void CUI_CardBook::Open(  )
{
	SendWorldEvent("CARDBOOKFRAME_OPEN");
}

// ----------------------------------------------------------------------------
void CUI_CardBook::Close()
{
	SendWorldEvent("CARDBOOKFRAME_CLOSE");
}
// ----------------------------------------------------------------------------
void CUI_CardBook::ResetCardInfo( )
{
	SendWorldEvent("CARDBOOKFRAME_UPDATE");
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetString	(  lua_State *L )
{
	const char* Key = lua_tostring( L , 1 );
	const char* Value = g_ObjectData->GetString( Key );
	lua_pushstring( L, Value );						
	return 1;
}
// --------------------------------------------------------------------------------------
int LuaFunc_InitCardInfo						( lua_State *L )
{
	g_pUi_CardBook->InitCardInfo();
	return 0;
}
// --------------------------------------------------------------------------------------
int LuaFunc_GetCardMaxCount						( lua_State *L )
{
	int Race = (int)lua_tonumber( L , 1 );

	if( (unsigned) Race >= EM_Max_Race )
	{
		return 0;
	}

	int	MaxCount = (int)g_pUi_CardBook->_MonsterCard[ Race ].size();
	lua_pushnumber( L, MaxCount );						
	return 1;
}

int LuaFunc_GetCardCount						( lua_State *L )
{
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	int Race = (int)lua_tonumber( L , 1 );
	int Count = 0;

	if( (unsigned) Race >= EM_Max_Race )
		return 0;

	vector<int>& CardList = g_pUi_CardBook->_MonsterCard[ Race ];
	for( int i = 0 ; i < CardList.size() ; i++ )
	{
		if( Role->PlayerBaseData->Card.GetFlag(CardList[i] - Def_ObjectClass_Card) == true )
			Count++;
	}
	lua_pushnumber( L, Count );						
	return 1;
}

int	LuaFunc_GetCardInfo							( lua_State *L )
{
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	int Race = (int)lua_tonumber( L , 1 );
	int ID   = (int)lua_tonumber( L , 2 );

	if( (unsigned) Race >= EM_Max_Race )
		return 0;
	
	vector<int>& CardList = g_pUi_CardBook->_MonsterCard[ Race ];

	if( (unsigned)ID >= CardList.size() )
		return 0;

	int CardID = CardList[ ID ];
	GameObjDbStructEx* CardObjDB = g_ObjectData->GetObj( CardID );
	bool Ret = Role->PlayerBaseData->Card.GetFlag( CardID- Def_ObjectClass_Card);

	lua_pushnumber( L , CardID );
	lua_pushnumber( L , (int)Ret );
	GameObjDbStructEx* CardOrgObjDB = g_ObjectData->GetObj( CardObjDB->Item.CardOrgObjID );
	if( Ret == false )	
		lua_pushstring( L , "? ? ? ? ? ? ? ?" );
	else
		lua_pushstring( L , CardOrgObjDB->GetName() );

	string strShortNote = CNetCli_Script_Child::ReplaceSystemKeyword( CardOrgObjDB->ShortNote );

	lua_pushstring( L , strShortNote.c_str() );

	lua_pushnumber( L , CardOrgObjDB->NPC.Race );
	int Rare = CardObjDB->Rare -2;
	if( Rare < 0 )
		Rare = 0;
	if( Rare > 2)
		Rare = 2;
	lua_pushnumber( L , Rare );
	lua_pushnumber( L , CardOrgObjDB->NPC.ResistType + 1 );


	//附加能力
	GameObjDbStructEx* AddPowerDB =  g_ObjectData->GetObj( 	CardObjDB->Item.CardAddpower);

	char AbilityMsg[512];
	AbilityMsg[0]=0;

	if ( AddPowerDB&&AddPowerDB->Classification == EM_ObjectClass_Attribute)
	{
		GameObjectAttributeStruct*pA=	&AddPowerDB->Attirbute;

		for ( int i = 0; i < Max_WearEqType_Count; i++ )
		{

			char buf[256];
			g_ObjectData->GetWearEqTypeString((WearEqTypeENUM)pA->Ability.EqType[i], (float)pA->Ability.EqTypeValue[i], buf, 256);			

			strcat(AbilityMsg,buf);
			strcat(AbilityMsg," ");
			
		}


	}
	




	lua_pushstring( L , AbilityMsg );
	return 8;
}

int LuaFunc_ShowCardImage						( lua_State *L )
{	
	CUiModel* UI_CardImage = g_pUi_CardBook->_UI_CardImage;
	if( UI_CardImage == NULL )
		return 0;

	RoleDataEx*	Role	= CNetGlobal::RoleData();
	int			CardID	= (int)lua_tonumber( L , 1 );
	GameObjDbStructEx* CardObjDB = g_ObjectData->GetObj( CardID );
	if( CardObjDB == NULL )
		return 0;

	GameObjDbStructEx* CardOrgObjDB = g_ObjectData->GetObj( CardObjDB->Item.CardOrgObjID );
	if( CardOrgObjDB == NULL )
		return 0;

	GameObjDbStructEx* imageDB = CNetGlobal::GetObj(CardOrgObjDB->ImageID);
	if ( imageDB == NULL )
		return 0;

	bool CardFlag = Role->PlayerBaseData->Card.GetFlag( CardID- Def_ObjectClass_Card);

	UI_CardImage->CreateEntity( imageDB->Image.ACTWorld );


	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR	, imageDB->Image.PaperdollHair.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD	, imageDB->Image.PaperdollFace.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND	, imageDB->Image.PaperdollGloves.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT	, imageDB->Image.PaperdollShoes.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO	, imageDB->Image.PaperdollClothes.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK	, imageDB->Image.PaperdollBack.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT	, imageDB->Image.PaperdollBelt.Name );
	UI_CardImage->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.PaperdollShouder.Name );

	// 髮色跟膚色需要特別指定
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.HairColor.Color				 , 0 );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.SkinColor.Color				 , 0 );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET		, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color );
	UI_CardImage->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );

	if( CardFlag )
	{
		UI_CardImage->SetEntityDiffuse( 0xffffffff );
	}
	else
	{
		UI_CardImage->SetEntityDiffuse( 0x10101010 );

		IRuTexture *blackTexture = g_ruResourceManager->LoadTexture("RUSYSTEM_BLACK");

		CRuEntity* pEntity = UI_CardImage->GetRuEntity();

		if ( pEntity )
		{
			IRuMaterial* pMaterial;
			CRuArrayList<IRuMaterial *> materialList;
			if( RuEntity_EnumerateMaterials( pEntity, materialList ) )
			{
				int count = materialList.Count();
				for( int i = 0 ; i < count ; ++i )
				{
					pMaterial = materialList.Get( i );
					pMaterial->SetTexture( ruTEXCHANNEL_GLOWMAP, blackTexture );
				}
			}
		}

		blackTexture->Release();
	}

	
//	UI_CardImage->SetCameraPos( CRuVector3( 100 , 100 ,100 ) ); // 2007/9/7 fuming

	return 0;
}


//取出卡片
int LuaFunc_TakeOutCard( lua_State *L )
{

	int CardObjID = (int)luaL_checknumber( L , 1  );

	NetCli_Item::_TakeOutCardRequest( CardObjID );
	return 0;
}
