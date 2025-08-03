#include "NetCli_GSrvConnectChild.h"
#include "../NetWakerClientInc.h"
#pragma unmanaged
//-----------------------------------------------------------------
//-----------------------------------------------------------------
bool    Net_GSrvConnectChild::Init()
{
    Net_GSrvConnect::_Init();

    if( This != NULL)
        return false;

    This = new Net_GSrvConnectChild;

    return true;
}
//-----------------------------------------------------------------
bool    Net_GSrvConnectChild::Release()
{
    if( This == NULL )
        return false;

    delete This;
    This = NULL;

    Net_GSrvConnectChild::_Release();
    return true;
    
}
//-----------------------------------------------------------------
void Net_GSrvConnectChild::PlayerItemID           ( int ItemID )
{
    CNetGlobal::Print( Def_PrintLV1,"\nPlayerItemID" );
}
void Net_GSrvConnectChild::ZoneInfo               ( int ZoneID , int GSrvID )
{
    CNetGlobal::Print( Def_PrintLV1,"\nZoneInfo" );
    SetSrvID_Local( GSrvID );
}
void Net_GSrvConnectChild::PlayerData             ( BaseRoleData *BaseRole )
{
    CNetGlobal::Print( Def_PrintLV1,"\nPlayerData" );
}
void Net_GSrvConnectChild::PrepareChangeZone      ( )
{
    CNetGlobal::Print( Def_PrintLV1,"\nPrepareChangeZone" );
}
void Net_GSrvConnectChild::ChangeZoneOK           ( )
{
    CNetGlobal::Print( Def_PrintLV1,"\nChangeZoneOK" );
}
//-----------------------------------------------------------------
#pragma managed