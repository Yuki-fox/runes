#include "CliNetWakerClass.h"
//#include "../ErrorList/errorlist.h"
#pragma unmanaged
//---------------------------------------------------------------------------------
void	CliNetWakerClass::_OnProxyConnected        ( DWORD NetID )
{
    _ProxyNetID = NetID;
	_NetBProxy->SetSocketNoDelay( NetID );
//    _Listener->OnLogin( "" );
    _IsReady = true;
}
void	CliNetWakerClass::_OnProxyDisconnected     ( DWORD NetID )
{
    set<int>::iterator Iter;

    if( _SrvConnect.size() != 0 )
    {
        for( Iter = _SrvConnect.begin() ; Iter != _SrvConnect.end() ; Iter++ )
        {
            _Listener->OnGSrvDisconnected( *Iter );   
        }

        _SrvConnect.clear();
    }

    if( _IsReady == true )
        _Listener->OnLogout( );
    
    _IsReady     = false;
    _ProxyNetID = -1;
}
bool	CliNetWakerClass::_OnProxyPacket           ( DWORD NetID, DWORD Size , void* Data )
{
//    int		Size;
//    void	*Data;

    //取得封包資料
//    ObjNetGetBuffer(hIncomingData,Size,Data);
    PGSysNULL* PG = ( PGSysNULL *)Data;

    //------------------------------------------------------------------------------


    //檢查是否為系統封包
    //if( PG->SysIdentify == _Sys_Package_Number_ )
    {
        if(_PGInfo.size() <= (UINT32)PG->Command )
        {
            TRACE2("\n(Proxy:%d)_OnProxyPacket : 封包cmd=%d過大" , NetID , PG->Command);
            return false;
        }

        //權限檢查
        PGBaseInfo *PGinfo = &( _PGInfo[ PG->Command ] );

        //封包大小檢查
		if(		PGinfo->MaxPGSize < Size 
			||	PGinfo->MinPGSize > Size  )
        {
            TRACE3("\n(Proxy:%d)_OnProxyPacket : 封包Size=%d過大 cmd=%d" , NetID , Size , PG->Command );
            return false;
        }

        //來源檢查
        if( PGinfo->Type.Proxy == false )
        {
            TRACE2("\n(Proxy:%d)_OnProxyPacket : 封包來源有問題 (PGinfo->Type.Proxy == false ??) cmd=%d" , NetID , PG->Command);
            return false;
        }

		//g_pLogList->AddMessage( "PG = %d, size = %d", PG->Command, Size );

        //呼叫Callback 處理
        (this->*_PGCallBack[ PG->Command ])( NetID , PG , Size );
    }

    return true;
}
void    CliNetWakerClass::_OnProxyConnectFailed    ( DWORD NetID )
{
    _Listener->OnLoginFailed( EM_NetLogFailed_ProxyLoginFailed );
    
}
//---------------------------------------------------------------------------------
//登入資料要求
void    CliNetWakerClass::_PG_LoginProxyRequest( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Cli_Proxy_LoginInfo PG_LoginInfo;
    PG_LoginInfo.CliID = _CliID;
    SendToProxy( sizeof( PG_LoginInfo ) , &PG_LoginInfo );
}

void    CliNetWakerClass::_PG_LoginProxyInfoOK( int NetID , PGSysNULL* PG , int Size )
{
    _Listener->OnLogin( "" );
}
//---------------------------------------------------------------------------------
//與 GSrv成立連線
void    CliNetWakerClass::_PGxGSrvOnConnect( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_GSrvOnConnect* GSrvOnConnect = (SysNet_Proxy_Cli_GSrvOnConnect*)PG;
    if( _SrvConnect.find( GSrvOnConnect->GSrvID ) != _SrvConnect.end() )
    {
        TRACE2( "\n_PGxGSrvOnConnect : 連線已存在???? GSrvID=%d Name=%s" , GSrvOnConnect->GSrvID , GSrvOnConnect->GSrvName );
        return;
    }
    _SrvConnect.insert( GSrvOnConnect->GSrvID );
    _Listener->OnGSrvConnected( GSrvOnConnect->GSrvID , GSrvOnConnect->GSrvName );
}
//---------------------------------------------------------------------------------
//與 GSrv斷線
void    CliNetWakerClass::_PGxGSrvOnDisonnect( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_GSrvOnDisConnect* GSrvOnDisConnect = (SysNet_Proxy_Cli_GSrvOnDisConnect*)PG;

    if( _SrvConnect.find( GSrvOnDisConnect->GSrvID ) == _SrvConnect.end() )
    {
        TRACE1( "\n_PGxGSrvOnDisonnect : 連線不存在???? GSrvID=%d " , GSrvOnDisConnect->GSrvID );
        return;
    }
    _SrvConnect.erase( GSrvOnDisConnect->GSrvID );
    _Listener->OnGSrvDisconnected( GSrvOnDisConnect->GSrvID );
}
//---------------------------------------------------------------------------------
//收到 GSrv 轉送給 Client 的封包
void    CliNetWakerClass::_PGxDataGSrvToCli( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_DataGSrvToCli* DataGSrvToCli = (SysNet_Proxy_Cli_DataGSrvToCli*)PG;

	//g_pLogList->AddMessage( "PGSrv = %d, size = %d, key = %d", DataGSrvToCli->GSrvID, DataGSrvToCli->Size, _ClientDataKey );

	if( DataGSrvToCli->Size == 0 )
	{
		_KeyData.PopData( _ClientDataKey );
		_ClientDataKey = DataGSrvToCli->ClientDataKey;
		return;
	}
	_Recvie_CliData = _KeyData.GetData( _ClientDataKey );



    _Listener->OnGSrvPacket( DataGSrvToCli->GSrvID , DataGSrvToCli->Size , DataGSrvToCli->Data );
	_Recvie_CliData = NULL;
}
//---------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//有其他的玩家用此帳號重新登入
void    CliNetWakerClass::_PGxCliRelogin( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_NotifyCliIDRelogin* CliRelogin = (SysNet_Proxy_Cli_NotifyCliIDRelogin*)PG;
    _Listener->OnCliReloginByOther( CliRelogin->IPNum );
}
//-----------------------------------------------------------------------------------
#pragma managed