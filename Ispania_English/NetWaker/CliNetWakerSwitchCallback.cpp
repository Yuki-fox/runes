#include "CliNetWakerClass.h"

#pragma unmanaged
//----------------------------------------------------------------------------------
void	CliNetWakerClass::_OnSwitchConnected        ( DWORD NetID )
{
    _SwitchNetID = NetID;
}
//----------------------------------------------------------------------------------
void	CliNetWakerClass::_OnSwitchDisconnected     ( DWORD NetID )
{
    _SwitchNetID = -1;
    if( _IsReady == false )
	{
        //_Listener->OnLogout( );
		_Listener->OnLoginFailed( EM_NetLogFailed_SwitchDisconnect );
	}
}
//----------------------------------------------------------------------------------
bool	CliNetWakerClass::_OnSwitchPacket           ( DWORD NetID, DWORD Size , void* Data )
{
//    int		Size;
//    void	*Data;

    //取得封包資料
    //ObjNetGetBuffer( hIncomingData , Size , Data );
    PGSysNULL* PG = ( PGSysNULL *) Data;

    //------------------------------------------------------------------------------

    //檢查是否為系統封包
    //if( PG->SysIdentify == _Sys_Package_Number_ )
    {
        if(_PGInfo.size() <= (UINT32)PG->Command)
        {
            TRACE2("\n(Switch:%d)_OnSwitchPacket : 封包cmd=%d過大 " , NetID , PG->Command );
            return false;
        }

        //權限檢查
        PGBaseInfo *PGinfo = &( _PGInfo[ PG->Command ] );

        //封包大小檢查
        if(		PGinfo->MaxPGSize < Size 
			||	PGinfo->MinPGSize > Size  )
        {
            TRACE3("\n(Switch:%d)_OnSwitchPacket : 封包Size=%d過大 cmd=%d " , NetID , Size , PG->Command );
            return false;
        }

        //來源檢查
        if( PGinfo->Type.Switch == false )
        {
            TRACE2("\n(Switch:%d)_OnSwitchPacket : 封包來源有問題 (PGinfo->Type.Proxy == false ??) cmd=%d " , NetID , PG->Command );
            return false;
        }

        //呼叫Callback 處理
        (this->*_PGCallBack[PG->Command])( NetID , PG , Size );
    }

    
    return true;
}
//----------------------------------------------------------------------------------
void    CliNetWakerClass::_OnSwitchConnectFailed    ( DWORD NetID )
{
    _Listener->OnLoginFailed( EM_NetLogFailed_ConnectFail );
}
//---------------------------------------------------------------------------------
//登入資料要求
void CliNetWakerClass::_PG_LoginInfoRequest( int NetID , PGSysNULL* PG , int Size )
{
    TRACE0( "\n_PG_LoginInfoRequest 要求Client 送登入訊息給 Switch" );
    SysNet_Switch_Cli_LoginInfoRequest* LoginInfoRequest = (SysNet_Switch_Cli_LoginInfoRequest*)PG;
    _CliID = LoginInfoRequest->CliID;
    
    SysNet_Cli_Switch_LoginInfo LoginInfo;
    LoginInfo.CliID = _CliID;
    memcpy( LoginInfo.Account , _Account , sizeof(_Account) );
    memcpy( LoginInfo.Password , _Password , sizeof(_Password) );
	memcpy( LoginInfo.MacAddress , _MacAddress , sizeof(_MacAddress) );
	LoginInfo.HashedPassword = _HashedPassword;

    SendToSwitch( sizeof(LoginInfo) , &LoginInfo );
}
//---------------------------------------------------------------------------------
//登入 Proxy 要求
void CliNetWakerClass::_PG_LoginInProxyNotify( int NetID , PGSysNULL* PG , int Size )
{
    TRACE0( "\n_PG_LoginInProxyNotify" );
    SysNet_Switch_Cli_ConnectPorxyNotify* ConnectPorxyNotify = (SysNet_Switch_Cli_ConnectPorxyNotify*)PG;

    if( ConnectPorxyNotify->Result == EM_NetLogFailed_OK )
    {
        _ConnectProxy( ConnectPorxyNotify->ProxyIPStr , ConnectPorxyNotify->ProxyPort , ConnectPorxyNotify->LastLoginIP , ConnectPorxyNotify->LastLoginTime );
        _ProxyID = ConnectPorxyNotify->ProxyID; 
		_PlayTimeQuota = ConnectPorxyNotify->PlayTimeQuota;
    }
    else
    {
        _Listener->OnLoginFailed( ConnectPorxyNotify->Result );
        _NetBSwitch->Close( NetID );
    }
}
//---------------------------------------------------------------------------------
//
void CliNetWakerClass::_PG_LoginFailedData( int NetID , PGSysNULL* PG , int Size )
{
	SysNet_Switch_Cli_LoginFailedData* FailedData = (SysNet_Switch_Cli_LoginFailedData*)PG;
	 _Listener->OnLoginFailedData(FailedData->Result, &(FailedData->Data), sizeof(FailedData->Data));
}
//---------------------------------------------------------------------------------
#pragma managed