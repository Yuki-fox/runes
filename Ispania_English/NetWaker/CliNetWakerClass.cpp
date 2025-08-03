
#include "CliNetWakerClass.h"
#include "malloc/MyAlloc.h"

#ifdef _CLIENT
	#include "../ErrorList/errorlist.h"
#endif

#pragma unmanaged
KeyDataClass		CliNetWakerClass::_KeyData;
map< string , vector<char> >	CliNetWakerClass::_Send_CliData;
map< string , vector<char> >*	CliNetWakerClass::_Recvie_CliData = NULL;

//---------------------------------------------------------------------------------
bool		LBSwitch::OnRecv			( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData )
{
    return _Parent->_OnSwitchPacket(  dwNetID, dwPackSize, pPacketData );
}
void		LBSwitch::OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode )
{
    TRACE0( "\nOnSwitchConnectFailed" );
    _Parent->_OnSwitchConnectFailed( dwFailedCode );
}
void		LBSwitch::OnConnect		( unsigned long dwNetID )
{
    TRACE0( "\nOnSwitchConnected" );
    _Parent->_OnSwitchConnected( dwNetID );
}
void		LBSwitch::OnDisconnect	( unsigned long dwNetID )
{
    TRACE0( "\nOnSwitchDisconnected" );
    _Parent->_OnSwitchDisconnected( dwNetID );
}
CEventObj*	LBSwitch::OnAccept		( unsigned long dwNetID )
{
    return this;
}
//---------------------------------------------------------------------------------
bool		LBProxy::OnRecv			( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData )
{
    return _Parent->_OnProxyPacket( dwNetID , dwPackSize , pPacketData );
}
void		LBProxy::OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode )
{
    TRACE0( "\n_OnProxyConnectFailed" );
    _Parent->_OnProxyConnectFailed( dwFailedCode );
}
void		LBProxy::OnConnect		( unsigned long dwNetID )
{
    TRACE0( "\n_OnProxyConnected" );
    _Parent->_OnProxyConnected( dwNetID );
}
void		LBProxy::OnDisconnect	( unsigned long dwNetID )
{
    TRACE0( "\n_OnProxyDisconnected" );
    _Parent->_OnProxyDisconnected( dwNetID );
}
CEventObj*	LBProxy::OnAccept		( unsigned long dwNetID )
{
    return this;
}
//---------------------------------------------------------------------------------
void	CliNetWakerClass::InitPGCallBack()
{

    int		i;
    PGBaseInfo	PGBase;
    //callback 函式陣列出始化
    for( i = 0 ; i <EM_SysNet_Packet_Count ; i++ )
    {
        _PGCallBack[i] = &CliNetWakerClass::_PGxUnKnoew;
        _PGInfo.push_back(PGBase);
    }

    //( Switch )
    _PGInfo[ EM_SysNet_Switch_Cli_LoginInfoRequest ].Type.Switch = true;
    _PGCallBack[ EM_SysNet_Switch_Cli_LoginInfoRequest ]        = &CliNetWakerClass::_PG_LoginInfoRequest;

    _PGInfo[ EM_SysNet_Switch_Cli_ConnectPorxyNotify ].Type.Switch = true;
    _PGCallBack[ EM_SysNet_Switch_Cli_ConnectPorxyNotify ]        = &CliNetWakerClass::_PG_LoginInProxyNotify;

	_PGInfo[ EM_SysNet_Switch_Cli_LoginFailedData ].Type.Switch = true;
	_PGCallBack[ EM_SysNet_Switch_Cli_LoginFailedData ]        = &CliNetWakerClass::_PG_LoginFailedData;

    //( Proxy )
    _PGInfo[ EM_SysNet_Proxy_Cli_LoginInfoRequest ].Type.Proxy = true;
    _PGCallBack[ EM_SysNet_Proxy_Cli_LoginInfoRequest ]        = &CliNetWakerClass::_PG_LoginProxyRequest;

	_PGInfo[ EM_SysNet_Proxy_Cli_LoginInfoOK ].Type.Proxy = true;
	_PGCallBack[ EM_SysNet_Proxy_Cli_LoginInfoOK ]        = &CliNetWakerClass::_PG_LoginProxyInfoOK;

    _PGInfo[ EM_SysNet_Proxy_Cli_GSrvOnConnect ].Type.Proxy = true;
    _PGCallBack[ EM_SysNet_Proxy_Cli_GSrvOnConnect ]        = &CliNetWakerClass::_PGxGSrvOnConnect;

    _PGInfo[ EM_SysNet_Proxy_Cli_GSrvOnDisConnect ].Type.Proxy = true;
    _PGCallBack[ EM_SysNet_Proxy_Cli_GSrvOnDisConnect ]        = &CliNetWakerClass::_PGxGSrvOnDisonnect;

    _PGInfo[ EM_SysNet_Proxy_Cli_DataGSrvToCli ].Type.Proxy = true;
    _PGCallBack[ EM_SysNet_Proxy_Cli_DataGSrvToCli ]        = &CliNetWakerClass::_PGxDataGSrvToCli;

    _PGInfo[ EM_SysNet_Proxy_Cli_NotifyCliIDRelogin].Type.Proxy = true;
    _PGCallBack[ EM_SysNet_Proxy_Cli_NotifyCliIDRelogin ]       = &CliNetWakerClass::_PGxCliRelogin;


}

//----------------------------------------------------------------------------------
void     CliNetWakerClass::_PGxUnKnoew( int NetID,PGSysNULL* PG,int Size)
{
    TRACE2( "\n(Switch:%d)_PGxUnKnoew : Command = %d" , NetID ,  PG->Command );
}
//----------------------------------------------------------------------------------
bool    CliNetWakerClass::SendToSwitch( int Size , void* Data )
{
    _NetBSwitch->Send( _SwitchNetID , Size , Data );
    return true;
}
//-----------------------------------------------------------------------------------
bool    CliNetWakerClass::SendToProxy( int Size , void* Data )
{
    _NetBProxy->Send( _ProxyNetID , Size , Data );
    return true;
}
//-----------------------------------------------------------------------------------
CliNetWakerClass::CliNetWakerClass( )
{
	_ProxyNetID = -1;
	_SwitchNetID = -1;

	_NetBSwitch = NULL;
	_NetBProxy	= NULL;
	_LBSwitch	= NULL;
	_LBProxy	= NULL;

}

void	CliNetWakerClass::Init()
{
	InitPGCallBack();

#ifdef _NB_SINGLECORE_
	_NetBSwitch       = CreateNetBridge( true );
	_NetBProxy        = CreateNetBridge( true );
#else 
	_NetBSwitch       = CreateNetBridge();
	_NetBProxy       = CreateNetBridge();
	//_NetBProxy        = CreateAsio( true );

#endif

	_LBSwitch         = NEW LBSwitch(this);             //資料列舉Class
	_LBProxy          = NEW LBProxy(this);              //資料列舉Class

	_SwitchNetID     = -1;
	_ExitFlag        = false;                   //結束旗標
	_IsReady         = false;                   //登入ok
	_ClientDataKey	 = -1;


	//_NetBSwitch->SetOutPutCB( OutputNetEngine );
	//_NetBProxy->SetOutPutCB( OutputNetEngine );


}

//-----------------------------------------------------------------------------------
CliNetWakerClass::~CliNetWakerClass()
{
#ifdef _NET_DESTROY_NOWAIT_
	if( _NetBSwitch )
		DeleteNetBridge( _NetBSwitch , false );
	if( _NetBProxy )
		DeleteNetBridge( _NetBProxy , false );
#else
	if( _NetBSwitch )
		DeleteNetBridge( _NetBSwitch );
	if( _NetBProxy )
		DeleteNetBridge( _NetBProxy );
#endif

	if( _LBSwitch == NULL )
		delete _LBSwitch;
    if( _LBProxy == NULL )
		delete _LBProxy;
}
//-----------------------------------------------------------------------------------
void	CliNetWakerClass::ConnectSwitch( char* IP,int Port , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword )
{
    Logout( EM_ClientNetCloseType_Relogin );

    strncpy( _Account  , Account  , sizeof( _Account  ) );
    strncpy( _Password , Password , sizeof( _Password ) );
	strncpy( _MacAddress , pszMacAddress , sizeof( _MacAddress ) );
	_HashedPassword = HashedPassword;

	g_pLogList->AddMessage( "Connecting to switch %s:%d",IP, Port );

    int IPNum = ConvertIP( IP );

	if (_NetBSwitch != NULL)
	{
		_NetBSwitch->Connect( IPNum , Port , _LBSwitch );
	}    

    TRACE4( "\nConnect to Switch Ip=%s Port=%d \n   Account=%s PWD=%s\n" , IP , Port , Account , Password );
	

}
//-----------------------------------------------------------------------------------
void	CliNetWakerClass::_ConnectProxy( char* IP , int Port  , char* LastLoginIP , char* LastLoginTime )
{
    //sockaddr_in Addr;

    //MAKE_IP_ADDRESS( (sockaddr_in*)&Addr , IP , Port );    )

	g_pLogList->AddMessage( "Connecting to Proxy %s:%d",IP, Port );

	int IPNum = ConvertIP( IP );
    _NetBProxy->Connect( IPNum , Port , _LBProxy );	

    TRACE2( "\nConnect to Proxy Ip=%s Port=%d \n" , IP , Port );
	_LastLoginIP   = LastLoginIP;
	_LastLoginTime = LastLoginTime;
}
//-----------------------------------------------------------------------------------
//定時處理
bool	CliNetWakerClass::Process( )
{
	_Schedular.Process();
    //_NetBSwitch->Flush(  );	// 2007/10/09 在最外層統一 WaitAll flush
    //_NetBProxy->Flush( );		// 2007/10/09 在最外層統一 WaitAll flush
    return true;
}
//-----------------------------------------------------------------------------------
//登出
bool    CliNetWakerClass::Logout( ClientNetCloseTypeENUM Type )
{
	if( _ProxyNetID != -1 )
	{
		SysNet_Cli_Proxy_NetClose Send;
		Send.NetCloseType = Type;
		SendToProxy( sizeof(Send) , &Send );

		CNetBridge::WaitAll( 500 );
	}

	if (_NetBProxy != NULL)
	{
		_NetBProxy->Close( _ProxyNetID );
	}

	if (_NetBSwitch != NULL)
	{
		_NetBSwitch->Close( _SwitchNetID );
	}

    return true;
}
//-----------------------------------------------------------------------------------
//設定事件回呼函式
void    CliNetWakerClass::SetListener(  NewWakerEventListener*   Listener )
{
    _Listener = Listener;
}
//-----------------------------------------------------------------------------------
bool    CliNetWakerClass::SendToGSrv( int GSrvID , int Size , void* Data )
{
    if( Size > _Max_Cli_Package_Size_ )
    {
        TRACE0( "\n超過 Client 端封包送出的上限大小 Size=%s " );
        return false;
    }
    int PGSize = Size + sizeof(SysNet_Cli_Proxy_DataCliToGSrv);
    char* SendData = new char[ PGSize ];
    SysNet_Cli_Proxy_DataCliToGSrv* Send = (SysNet_Cli_Proxy_DataCliToGSrv*)SendData;
    Send->GSrvID        = GSrvID;

	//////////////////////////////////////////////////////////////////////////
	//client 端夾帶資料　等回傳封包處理時取出
	if( _Send_CliData.size() == 0 )
	{
		Send->ClientDataKey = -1;
	}
	else
	{
		Send->ClientDataKey = _KeyData.PushData( _Send_CliData );
		_Send_CliData.clear();
	}
	//////////////////////////////////////////////////////////////////////////
    Send->Size          = Size;
    Send->Command       = EM_SysNet_Cli_Proxy_DataCliToGSrv;
    memcpy( Send->Data , Data , Size );
    SendToProxy( PGSize , Send );
    
    delete[] SendData;
    return true;
}

DWORD CliNetWakerClass::GetProxyRespondTime()
{
	return _NetBProxy->GetRespondTime( _ProxyNetID );
}

void	CliNetWakerClass::PushClientData( const char* Key , void* Data , int Size )
{
	//_Send_CliData.Init();
//	_Send_CliData.Data = NEW char[Size];
//	_Send_CliData.Size = Size;
//	memcpy( _Send_CliData.Data , Data , Size );
	vector<char>& vecdata = _Send_CliData[Key];
	vecdata.clear();
	vecdata.insert( vecdata.begin() , (char*)(Data) , (char*)Data+Size  );
}
/*
MemoryBlockStruct* CliNetWakerClass::GetClientData( )
{	
//	return _KeyData.GetData( _ClientDataKey );
	return _Recvie_CliData;
}
*/
bool CliNetWakerClass::PopClientData( const char* Key , void* Data , int Size )
{
	if( _Recvie_CliData == NULL )
		return false;

	vector<char>& vecdata = (*_Recvie_CliData)[Key];
	if( vecdata.size() != Size )
		return false;

	memcpy( Data , &vecdata[0] , Size );

	return true;
}

#ifdef _CLIENT
void CliNetWakerClass::OutputNetEngineProxy( int lv, const char* pszMsg )
{
	//g_pLogList->AddMessage( "[P]%s", pszMsg );
}

void CliNetWakerClass::OutputNetEngineSwitch( int lv, const char* pszMsg )
{
	//g_pLogList->AddMessage( "[S]%s", pszMsg );
}

void CliNetWakerClass::OutputNetEngineBGProxy( int lv, const char* pszMsg )
{
	//g_pLogList->AddMessage( "[BP]%s", pszMsg );
}
void CliNetWakerClass::OutputNetEngineBGSwitch( int lv, const char* pszMsg )
{
	//g_pLogList->AddMessage( "[BS]%s", pszMsg );
}
#endif
#pragma managed
