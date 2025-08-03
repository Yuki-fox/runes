#ifndef __Net_GSrvConnectCHILD_H__
#define __Net_GSrvConnectCHILD_H__

#include "Net_GSrvConnect.h"

#pragma unmanaged

class Net_GSrvConnectChild : public Net_GSrvConnect
{
public:
    static bool Init();
    static bool Release();

    //-------------------------------------------------------------------
    //  ¨Æ¥óÄ~©Ó¹ê°
    //-------------------------------------------------------------------
    virtual void PlayerItemID           ( int ItemID );
    virtual void ZoneInfo               ( int ZoneID , int GSrvID );
    virtual void PlayerData             ( BaseRoleData *BaseRole );
    virtual void PrepareChangeZone      ();
    virtual void ChangeZoneOK           ();
 
};

#pragma managed

#endif //__Net_GSrvConnectCHILD_H__