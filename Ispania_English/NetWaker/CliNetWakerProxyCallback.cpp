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

    //���o�ʥ]���
//    ObjNetGetBuffer(hIncomingData,Size,Data);
    PGSysNULL* PG = ( PGSysNULL *)Data;

    //------------------------------------------------------------------------------


    //�ˬd�O�_���t�Ϋʥ]
    //if( PG->SysIdentify == _Sys_Package_Number_ )
    {
        if(_PGInfo.size() <= (UINT32)PG->Command )
        {
            TRACE2("\n(Proxy:%d)_OnProxyPacket : �ʥ]cmd=%d�L�j" , NetID , PG->Command);
            return false;
        }

        //�v���ˬd
        PGBaseInfo *PGinfo = &( _PGInfo[ PG->Command ] );

        //�ʥ]�j�p�ˬd
		if(		PGinfo->MaxPGSize < Size 
			||	PGinfo->MinPGSize > Size  )
        {
            TRACE3("\n(Proxy:%d)_OnProxyPacket : �ʥ]Size=%d�L�j cmd=%d" , NetID , Size , PG->Command );
            return false;
        }

        //�ӷ��ˬd
        if( PGinfo->Type.Proxy == false )
        {
            TRACE2("\n(Proxy:%d)_OnProxyPacket : �ʥ]�ӷ������D (PGinfo->Type.Proxy == false ??) cmd=%d" , NetID , PG->Command);
            return false;
        }

		//g_pLogList->AddMessage( "PG = %d, size = %d", PG->Command, Size );

        //�I�sCallback �B�z
        (this->*_PGCallBack[ PG->Command ])( NetID , PG , Size );
    }

    return true;
}
void    CliNetWakerClass::_OnProxyConnectFailed    ( DWORD NetID )
{
    _Listener->OnLoginFailed( EM_NetLogFailed_ProxyLoginFailed );
    
}
//---------------------------------------------------------------------------------
//�n�J��ƭn�D
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
//�P GSrv���߳s�u
void    CliNetWakerClass::_PGxGSrvOnConnect( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_GSrvOnConnect* GSrvOnConnect = (SysNet_Proxy_Cli_GSrvOnConnect*)PG;
    if( _SrvConnect.find( GSrvOnConnect->GSrvID ) != _SrvConnect.end() )
    {
        TRACE2( "\n_PGxGSrvOnConnect : �s�u�w�s�b???? GSrvID=%d Name=%s" , GSrvOnConnect->GSrvID , GSrvOnConnect->GSrvName );
        return;
    }
    _SrvConnect.insert( GSrvOnConnect->GSrvID );
    _Listener->OnGSrvConnected( GSrvOnConnect->GSrvID , GSrvOnConnect->GSrvName );
}
//---------------------------------------------------------------------------------
//�P GSrv�_�u
void    CliNetWakerClass::_PGxGSrvOnDisonnect( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_GSrvOnDisConnect* GSrvOnDisConnect = (SysNet_Proxy_Cli_GSrvOnDisConnect*)PG;

    if( _SrvConnect.find( GSrvOnDisConnect->GSrvID ) == _SrvConnect.end() )
    {
        TRACE1( "\n_PGxGSrvOnDisonnect : �s�u���s�b???? GSrvID=%d " , GSrvOnDisConnect->GSrvID );
        return;
    }
    _SrvConnect.erase( GSrvOnDisConnect->GSrvID );
    _Listener->OnGSrvDisconnected( GSrvOnDisConnect->GSrvID );
}
//---------------------------------------------------------------------------------
//���� GSrv ��e�� Client ���ʥ]
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
//����L�����a�Φ��b�����s�n�J
void    CliNetWakerClass::_PGxCliRelogin( int NetID , PGSysNULL* PG , int Size )
{
    SysNet_Proxy_Cli_NotifyCliIDRelogin* CliRelogin = (SysNet_Proxy_Cli_NotifyCliIDRelogin*)PG;
    _Listener->OnCliReloginByOther( CliRelogin->IPNum );
}
//-----------------------------------------------------------------------------------
#pragma managed