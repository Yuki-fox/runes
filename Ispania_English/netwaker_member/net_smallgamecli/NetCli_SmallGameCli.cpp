#include "NetCli_SmallGameCli.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_SmallGame*   NetCli_SmallGame::This          = NULL;
int                 NetCli_SmallGame::SmSrvID       = -1;
//-------------------------------------------------------------------
bool NetCli_SmallGame::_Init()
{
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_CreateRoomResult , _PG_SmallGameCli_G1toC_CreateRoomResult   );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_LeaveRoom        , _PG_SmallGameCli_G1toC_LeaveRoom          );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_JoinRoomFalase   , _PG_SmallGameCli_G1toC_JoinRoomFalase     );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_JoinRoom         , _PG_SmallGameCli_G1toC_JoinRoom           );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_AddRoomPlayer    , _PG_SmallGameCli_G1toC_AddRoomPlayer      );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_DelRoomPlayer    , _PG_SmallGameCli_G1toC_DelRoomPlayer      );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_Connect          , _PG_SmallGameCli_G1toC_Connect            );

    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_BeginGame  , _PG_SmallGameCli_G1toC_BeginGame   );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_CardInfo   , _PG_SmallGameCli_G1toC_CardInfo    );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_CardTurn   , _PG_SmallGameCli_G1toC_CardTurn    );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_ClearTable , _PG_SmallGameCli_G1toC_ClearTable  );
    _Net.RegOnSrvPacketFunction	( EM_PG_SmallGameCli_G1toC_EndGame    , _PG_SmallGameCli_G1toC_EndGame     );

    return false;
}
//-------------------------------------------------------------------
bool NetCli_SmallGame::_Release()
{
    return false;
}
//-------------------------------------------------------------------
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_CreateRoomResult    ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_CreateRoomResult* PG = (PG_SmallGameCli_G1toC_CreateRoomResult*)Data;

    if( PG->Result == false )
    {
        This->CreateRoomOK( PG->RoomID , PG->Title );
    }
    else
    {
        This->CreateRoomFalse( );
    }
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_LeaveRoom           ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_LeaveRoom* PG = (PG_SmallGameCli_G1toC_LeaveRoom*)Data;

    This->LeaveRoom( );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_JoinRoomFalase      ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_JoinRoomFalase* PG = (PG_SmallGameCli_G1toC_JoinRoomFalase*)Data;
    
    This->JoinRoomFalase( PG->RoomID );

}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_JoinRoom            ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_JoinRoom* PG = (PG_SmallGameCli_G1toC_JoinRoom*)Data;
    This->JoinRoom( PG->RoomID );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_AddRoomPlayer       ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_AddRoomPlayer* PG = (PG_SmallGameCli_G1toC_AddRoomPlayer*)Data;
    This->AddRoomPlayer( PG->RoomID , PG->Name , PG->Account );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_DelRoomPlayer       ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_DelRoomPlayer* PG = (PG_SmallGameCli_G1toC_DelRoomPlayer*)Data;
    This->DelRoomPlayer( PG->RoomID , PG->Name , PG->Account );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_Connect          ( int Sockid , int Size , void* Data )
{
    SmSrvID    = Sockid;
}
//----------------------------------------------------------------------------------------------------------------------------------
void NetCli_SmallGame::CreateRoom         ( char* Title )
{
    PG_SmallGameCli_CtoG1_CreateRoom Send;
    strncpy( Send.Title , Title , sizeof( Send.Title ) );
    SendToSmallGame( sizeof( Send ) , &Send );
}
void NetCli_SmallGame::LeaveRoomRequest   ( )
{
    PG_SmallGameCli_CtoG1_LeaveRoomRequest Send;
    SendToSmallGame( sizeof( Send ) , &Send );
}
void NetCli_SmallGame::JoinRoomRequest    ( int RoomID )
{
    PG_SmallGameCli_CtoG1_JoinRoomRequest Send;
    Send.RoomID = RoomID;
    SendToSmallGame( sizeof( Send ) , &Send );
}
void NetCli_SmallGame::SendToSmallGame    ( int Size , void* Data )
{
    _Net.SendToSrv( SmSrvID , Size , Data );
}
//----------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_BeginGame  ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_BeginGame* PG = (PG_SmallGameCli_G1toC_BeginGame*)Data;
    This->BeginGame( );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_CardInfo   ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_CardInfo* PG = (PG_SmallGameCli_G1toC_CardInfo*)Data;
    This->CardInfo( PG->Card );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_CardTurn   ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_CardTurn* PG = (PG_SmallGameCli_G1toC_CardTurn*)Data;
    This->CardTurn( PG->Account );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_ClearTable ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_ClearTable* PG = (PG_SmallGameCli_G1toC_ClearTable*)Data;
    This->ClearTable( );
}
void NetCli_SmallGame::_PG_SmallGameCli_G1toC_EndGame    ( int Sockid , int Size , void* Data )
{
    PG_SmallGameCli_G1toC_EndGame* PG = (PG_SmallGameCli_G1toC_EndGame*)Data;
    This->EndGame( );
}
//----------------------------------------------------------------------------------------------------------------------------------
void NetCli_SmallGame::CardOut    ( int Count , int* Cards )
{
    PG_SmallGameCli_CtoG1_CardOut Send;
    memcpy( Send.Card , Cards , sizeof(Send.Card) );
    Send.Count = Count;

    SendToSmallGame( sizeof(Send) , &Send );
}

void NetCli_SmallGame::PlayAgain  ( )  
{
    PG_SmallGameCli_CtoG1_PlayAgain Send;
    SendToSmallGame( sizeof(Send) , &Send );
}
//----------------------------------------------------------------------------------------------------------------------------------