#include "NetCli_Treasure.h"

//-------------------------------------------------------------------
NetCli_Treasure*    NetCli_Treasure::This         = NULL;
vector<ItemFieldStruct>		NetCli_Treasure::s_booty;

//-------------------------------------------------------------------
bool NetCli_Treasure::_Init()
{
	Cli_NetReg( PG_Treasure_LtoC_BaseInfo		);
	Cli_NetReg( PG_Treasure_LtoC_OpenFailed		);
	Cli_NetReg( PG_Treasure_LtoC_Open			);
	Cli_NetReg( PG_Treasure_LtoC_GetItemResult	);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Treasure::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Treasure::_PG_Treasure_LtoC_BaseInfo		( int Sockid , int Size , void* Data )
{
	PG_Treasure_LtoC_BaseInfo* PG = (PG_Treasure_LtoC_BaseInfo*)Data;
	This->R_BaseInfo( PG->ItemGUID , PG->IsEmpty );
}
void NetCli_Treasure::_PG_Treasure_LtoC_OpenFailed		( int Sockid , int Size , void* Data )	
{
	PG_Treasure_LtoC_OpenFailed* PG = (PG_Treasure_LtoC_OpenFailed*)Data;
	This->R_OpenFailed( PG->ItemGUID );
}
void NetCli_Treasure::_PG_Treasure_LtoC_Open			( int Sockid , int Size , void* Data )
{
	PG_Treasure_LtoC_Open* PG = (PG_Treasure_LtoC_Open*)Data;
	This->R_Open( PG->ItemGUID , PG->MaxCount , PG->Items );
}
void NetCli_Treasure::_PG_Treasure_LtoC_GetItemResult	( int Sockid , int Size , void* Data )
{
	PG_Treasure_LtoC_GetItemResult* PG = (PG_Treasure_LtoC_GetItemResult*)Data;
	This->R_GetItemResult( PG->BodyPos , PG->ItemSerial , PG->Result );
}

ItemFieldStruct* NetCli_Treasure::GetBootyItem(int index)
{
	if ( index < 0 || index >= (int)s_booty.size() )
		return NULL;
	return &(s_booty[index]);
}

void NetCli_Treasure::SL_OpenRequest	( int ItemGUID )
{
	PG_Treasure_CtoL_OpenRequest Send;
	Send.ItemGUID = ItemGUID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Treasure::SL_GetItem		( int ItemGUID , int ItemSerial , int BodyPos )
{
	PG_Treasure_CtoL_GetItem Send;
	Send.ItemGUID = ItemGUID;
	Send.ItemSerial = ItemSerial;
	Send.BodyPos = BodyPos;		//要放的位置
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Treasure::SL_Close			( int ItemGUID )
{
	PG_Treasure_CtoL_Close Send;
	Send.ItemGUID = ItemGUID;
	SendToLocal( sizeof(Send) , &Send );
}