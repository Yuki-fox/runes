#include "NetCli_Recipe.h"

//-------------------------------------------------------------------
NetCli_Recipe*    NetCli_Recipe::This         = NULL;
vector<ItemFieldStruct> NetCli_Recipe::_ItemList;
//-------------------------------------------------------------------
bool NetCli_Recipe::_Init()
{
	Cli_NetReg( PG_Recipe_LtoC_LockResourceAndRefineResult );
	Cli_NetReg( PG_Recipe_LtoC_ProduceItemResult    );
	Cli_NetReg( PG_Recipe_LtoC_RecipeList    );
	Cli_NetReg( PG_Recipe_LtoC_LearnRecipeResult    );


    return true;
}
//-------------------------------------------------------------------
bool NetCli_Recipe::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Recipe::_PG_Recipe_LtoC_RefineCooldown( int Sockid , int Size , void* Data )
{
	PG_Recipe_LtoC_RefineCooldown *  pPacket = (PG_Recipe_LtoC_RefineCooldown *)Data;
	This->RL_RefineCooldown( pPacket->Cooldown );
}
//-------------------------------------------------------------------
void NetCli_Recipe::_PG_Recipe_LtoC_LearnRecipeResult ( int Sockid , int Size , void* Data )
{
	PG_Recipe_LtoC_LearnRecipeResult *  pPacket = (PG_Recipe_LtoC_LearnRecipeResult *)Data;
	This->RL_LearnRecipeResult( pPacket->iRecipeID,(EM_LearnRecipeResult) pPacket->iResult );
}
//-------------------------------------------------------------------
void NetCli_Recipe::_PG_Recipe_LtoC_RecipeList ( int Sockid , int Size , void* Data )
{
	PG_Recipe_LtoC_RecipeList *  pPacket = (PG_Recipe_LtoC_RecipeList *)Data;
	This->RL_RecipeList( &pPacket->RecipeList );
}


//-------------------------------------------------------------------
void NetCli_Recipe::_PG_Recipe_LtoC_LockResourceAndRefineResult ( int Sockid , int Size , void* Data )
{
	PG_Recipe_LtoC_LockResourceAndRefineResult *  PG = (PG_Recipe_LtoC_LockResourceAndRefineResult *)Data;

	switch( PG->Result )
	{
	case EM_RefineResult_OK:				//製造成 
		This->RL_LockResourceAndRefineResultOK( PG->RecipeID , _ItemList  );
		break;

	case EM_RefineResult_Failed:			//製造失敗
		This->RL_LockResourceAndRefineResultFailed( PG->RecipeID , _ItemList  );
		break;
	case EM_RefineResult_ResourceError:		//材料不足
	case EM_RefineResult_RecipeError:		//沒配方單
	case EM_RefineResult_SkillLvError:		//技能等級不足
	case EM_RefineResult_ColdownError:		//冷卻中
		This->RL_LockResourceAndRefineResultError( PG->RecipeID , PG->Result );
		break;
	}

	_ItemList.clear();
	UnlockRecipeResource( );
}
void NetCli_Recipe::_PG_Recipe_LtoC_ProduceItemResult			( int Sockid , int Size , void* Data )
{
	PG_Recipe_LtoC_ProduceItemResult *  PG = (PG_Recipe_LtoC_ProduceItemResult *)Data;
	_ItemList.push_back( PG->Item );

	This->RL_ProduceItemResult(PG);
}

//////////////////////////////////////////////////////////////////////////
//要求鎖住某個配方單( Client 端)
bool NetCli_Recipe::LockRecipeResource( int RecipeID )
{
	RoleDataEx* Owner = CNetGlobal::RoleData();

	GameObjDbStruct* RecipeDB = GetObj( RecipeID );
	if( RecipeDB == NULL )
		return false;

	for( int i = 0 ; i < _MAX_RECIPE_SOURCE_COUNT_ ; i++ )
	{
		GameObjDbStructEx*	SourceDB = GetObj( RecipeDB->Recipe.Source[i].OrgObjID );
		int	Count = RecipeDB->Recipe.Source[i].Count;

		if( SourceDB == NULL )
			continue;

		if( Owner->CalBodyItemCount( SourceDB->GUID ) < Count )
		{
			This->ShowRequestItemError();
			return false ;
		}
	}

	for( int i = 0 ; i < _MAX_RECIPE_SOURCE_COUNT_ ; i++ )
	{
		GameObjDbStructEx*	SourceDB = GetObj( RecipeDB->Recipe.Source[i].OrgObjID );
		int	Count = RecipeDB->Recipe.Source[i].Count;

		if( SourceDB == NULL )
			continue;

		for( int i = 0 ; i < Owner->PlayerBaseData->Body.Count ; i ++ )
		{
			ItemFieldStruct& Item = Owner->PlayerBaseData->Body.Item[i];
			if(		Item.OrgObjID == SourceDB->GUID 
				&&	Item.Pos	  == EM_ItemState_NONE	)
				Item.Pos = EM_RECIPE_RESOURCE;
		}
	}

	
	return true;
}
//要求解開配方單鎖住的物品( Client 端)
void NetCli_Recipe::UnlockRecipeResource( )
{

	RoleDataEx* Owner = CNetGlobal::RoleData();

	for( int i = 0 ; i < Owner->PlayerBaseData->Body.Count ; i ++ )
	{
		ItemFieldStruct& Item = Owner->PlayerBaseData->Body.Item[i];
		if( Item.Pos == EM_RECIPE_RESOURCE	)
			Item.Pos = EM_ItemState_NONE;
	}

}

//要求合成物品
bool  NetCli_Recipe::SL_RefineItem( int RecipeID )
{
	if( LockRecipeResource( RecipeID ) == false )
		return false;

	PG_Recipe_CtoL_LockResourceAndRefineRequest Send;

	Send.RecipeID = RecipeID;

	SendToLocal( sizeof(Send) , &Send );

	return true;	
}

void NetCli_Recipe::SL_LearnRecipe		( int RecipeID )
{
	PG_Recipe_CtoL_LearnRecipe	Packet;

	Packet.iRecipeID	= RecipeID;

	SendToLocal( sizeof( Packet), &Packet );
}