#pragma once
#include "PG_SmallGameCli.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_SmallGame : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_SmallGame* This;
    static int   SmSrvID;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
    static void _PG_SmallGameCli_G1toC_CreateRoomResult ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_LeaveRoom        ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_JoinRoomFalase   ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_JoinRoom         ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_AddRoomPlayer    ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_DelRoomPlayer    ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_Connect          ( int Sockid , int Size , void* Data );
public:    
    NetCli_SmallGame() { This = this; }
    //-------------------------------------------------------------------
    static void CreateRoom         ( char* Title );
    static void LeaveRoomRequest   ( );
    static void JoinRoomRequest    ( int RoomID );
    static void SendToSmallGame    ( int Size , void* Data );
    //-------------------------------------------------------------------
    
    virtual void CreateRoomOK   ( int RoomID , char* Title ) = 0;
    virtual void CreateRoomFalse( ) = 0;
    virtual void LeaveRoom      ( ) = 0;
    virtual void JoinRoomFalase ( int RoomID ) = 0;
    virtual void JoinRoom       ( int RoomID ) = 0;
    virtual void AddRoomPlayer  ( int RoomID , char* Name , char* Account ) = 0;
    virtual void DelRoomPlayer  ( int RoomID , char* Name , char* Account ) = 0;
    //-------------------------------------------------------------------

protected:
    static void _PG_SmallGameCli_G1toC_BeginGame  ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_CardInfo   ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_CardTurn   ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_ClearTable ( int Sockid , int Size , void* Data );
    static void _PG_SmallGameCli_G1toC_EndGame    ( int Sockid , int Size , void* Data );

public:
    static void CardOut    ( int Count , int* Cards );
    static void PlayAgain  ( );  

    virtual void BeginGame ( ) = 0;
    virtual void CardInfo  ( int* Card ) = 0;
    virtual void CardTurn  ( char* Account ) = 0;
    virtual void ClearTable( ) = 0;
    virtual void EndGame   ( ) = 0;

};

