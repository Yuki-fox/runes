#include "../../mainproc/GameMain.h"					// For Obj and Role struct
#include "../../netwaker_member/NetWakerClientInc.h"	// For NetGlobal

#include "../WorldFrames/ItemClipboard.h"
#include "../Debug/DebugMessage.h"
#include "./UI_PetBook.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_PetBook*		g_pUi_PetBook						= NULL;
//-------------------------------------------------------------------
//int	LuaFunc_GetString					( lua_State *L );	
int LuaFunc_InitPetBookInfo						( lua_State *L );
int LuaFunc_GetPetBookMaxCount					( lua_State *L );
int LuaFunc_GetPetBookCount						( lua_State *L );
int LuaFunc_GetPetBookInfo						( lua_State *L );
int LuaFunc_ShowPetBookImage					( lua_State *L );

//-------------------------------------------------------------------
CUI_PetBook::CUI_PetBook( CInterface* object ) : CInterfaceBase( object )
{
	g_pUi_PetBook		= this;
	_UI_PetImage		= NULL;
}
// --------------------------------------------------------------------------------------
CUI_PetBook::~CUI_PetBook(void)
{
	g_pUi_PetBook	= NULL;
}
// --------------------------------------------------------------------------------------
void CUI_PetBook::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		
		lua_register( l, "LuaFunc_InitPetBookInfo"		,LuaFunc_InitPetBookInfo		);		
		lua_register( l, "LuaFunc_GetPetBookMaxCount"	,LuaFunc_GetPetBookMaxCount	);		
		lua_register( l, "LuaFunc_GetPetBookCount"		,LuaFunc_GetPetBookCount		);		
		lua_register( l, "LuaFunc_GetPetBookInfo"		,LuaFunc_GetPetBookInfo		);
		lua_register( l, "LuaFunc_ShowPetBookImage"		,LuaFunc_ShowPetBookImage		);
		
	}
}
/*
// --------------------------------------------------------------------------------------
bool	CompMonsterPetPriority( int Pet2 , int Pet1 )
{
	GameObjDbStructEx* petObj1		= g_ObjectData->GetObj( Pet1 );
	GameObjDbStructEx* petObj2		= g_ObjectData->GetObj( Pet2 );


	if( petObj1->Rare < petObj2->Rare )
		return true;
	else	if( petObj1->Rare > petObj2->Rare )
		return false;
	else
	{
		if( petObj1->Item.CardPriorityID == petObj2->Item.CardPriorityID  )
			return ( Pet1 > Pet2 );

		return ( petObj1->Item.CardPriorityID < petObj2->Item.CardPriorityID );
	}

}
*/
void CUI_PetBook::InitPetInfo()
{
	


	

	_UI_PetImage = (CUiModel* )m_interface->GetUiWorld()->FindObject( "PetBookFramePetImage" );
}

// ----------------------------------------------------------------------------
void CUI_PetBook::Open(  )
{
	SendWorldEvent("PETBOOKFRAME_OPEN");
}

// ----------------------------------------------------------------------------
void CUI_PetBook::Close()
{
	SendWorldEvent("PETBOOKFRAME_CLOSE");
}
// ----------------------------------------------------------------------------
void CUI_PetBook::ResetPetInfo( )
{
	SendWorldEvent("PETBOOKFRAME_UPDATE");
}


// --------------------------------------------------------------------------------------
int LuaFunc_InitPetBookInfo						( lua_State *L )
{
	g_pUi_PetBook->InitPetInfo();
	return 0;
}
// --------------------------------------------------------------------------------------
int LuaFunc_GetPetBookMaxCount						( lua_State *L )
{
	int Group = (int)lua_tonumber( L , 1 );

	int	MaxCount = (int)g_ObjectData->_PetCardList.size();//g_pUi_PetBook->_pets[ Race ].size();
	lua_pushnumber( L, MaxCount );						
	return 1;
}

int LuaFunc_GetPetBookCount						( lua_State *L )
{
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	int Group = (int)lua_tonumber( L , 1 );
	int Count = 0;

	if( (unsigned) Group >= EM_Max_Race||Group<0 )
		return 0;

	vector<int>& PetList =  g_ObjectData->_PetCardList;
	for( int i = 0 ; i < PetList.size() ; i++ )
	{
		if( Role->PlayerBaseData->PetCard.GetFlag(i) == true )
			Count++;
	}
	lua_pushnumber( L, Count );						
	return 1;
}

int	LuaFunc_GetPetBookInfo							( lua_State *L )
{
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	int Group = (int)lua_tonumber( L , 1 );
	int ID   = (int)lua_tonumber( L , 2 );

	if( (unsigned) Group >= EM_Max_Race||Group<0 )
		return 0;

	vector<int>& PetList =g_ObjectData->_PetCardList;

	if( (unsigned)ID >= PetList.size() )
		return 0;

	int PetID = PetList[ ID ];


	GameObjDbStructEx* PetObjDB = g_ObjectData->GetObj( PetID );

	
	GameObjDbStructEx* PetNpcObjDB = g_ObjectData->GetObj( PetObjDB->Item.CollectionNPC );
	bool Ret = Role->PlayerBaseData->PetCard.GetFlag( ID);
	if (!PetNpcObjDB)
		return 0;




	lua_pushnumber( L , ID );
	lua_pushnumber( L , (int)Ret );

	if( Ret == false )	
		lua_pushstring( L , "? ? ? ? ? ? ? ?" );
	else
		lua_pushstring( L , PetNpcObjDB->GetName() );
	
	lua_pushnumber( L , PetNpcObjDB->NPC.ResistType );

	
	return 4;
}

int LuaFunc_ShowPetBookImage						( lua_State *L )
{	
	CUiModel* UI_PetImage = g_pUi_PetBook->_UI_PetImage;
	if( UI_PetImage == NULL )
		return 0;

	RoleDataEx*	Role	= CNetGlobal::RoleData();
	int			PetID	= (int)lua_tonumber( L , 1 );

	vector<int>& PetList =g_ObjectData->_PetCardList;

	GameObjDbStructEx* PetObjDB = g_ObjectData->GetObj( PetList[PetID] );
	if( PetObjDB == NULL )
		return 0;

	GameObjDbStructEx* PetNPCObjDB = g_ObjectData->GetObj( PetObjDB->Item.CollectionNPC );
	if( PetNPCObjDB == NULL )
		return 0;



	GameObjDbStructEx* imageDB = g_ObjectData->GetObj( PetNPCObjDB->ImageID);

	if ( imageDB == NULL )
		return 0;

	bool PetFlag = Role->PlayerBaseData->PetCard.GetFlag( PetID);

	UI_PetImage->CreateEntity( imageDB->Image.ACTWorld );


	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR	, imageDB->Image.PaperdollHair.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD	, imageDB->Image.PaperdollFace.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND	, imageDB->Image.PaperdollGloves.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT	, imageDB->Image.PaperdollShoes.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO	, imageDB->Image.PaperdollClothes.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK	, imageDB->Image.PaperdollBack.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT	, imageDB->Image.PaperdollBelt.Name );
	UI_PetImage->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.PaperdollShouder.Name );

	// 髮色跟膚色需要特別指定
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.HairColor.Color				 , 0 );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.SkinColor.Color				 , 0 );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET		, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color );
	UI_PetImage->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );

	if( PetFlag )
	{
		UI_PetImage->SetEntityDiffuse( 0xffffffff );
	}
	else
	{
		UI_PetImage->SetEntityDiffuse( 0x10101010 );

		IRuTexture *blackTexture = g_ruResourceManager->LoadTexture("RUSYSTEM_BLACK");

		CRuEntity* pEntity = UI_PetImage->GetRuEntity();

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


