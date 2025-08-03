#include "ServerListCliClass.h"
#include "../../mainproc/GameMain.h"
#include "../../ErrorList/errorlist.h"
//----------------------------------------------------------------------------------------------

bool		LBServerList::OnRecv		( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData )
{
    return _Parent->_OnPacket( dwNetID , dwPackSize , pPacketData );
}
void		LBServerList::OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode )
{
    _Parent->_OnConnectFailed( dwFailedCode );
}
void		LBServerList::OnConnect		( unsigned long dwNetID )
{
    _Parent->_OnConnected( dwNetID );
}
void		LBServerList::OnDisconnect	( unsigned long dwNetID )
{
    _Parent->_OnDisconnected( dwNetID );
}
CEventObj*	LBServerList::OnAccept		( unsigned long dwNetID )
{
    return this;
}
//-----------------------------------------------------------------------------------
// ��l�� Client �P Server �� CallBack�禡
//-----------------------------------------------------------------------------------
void	ServerListCliClass::InitPGCallBack( )
{
    int		i;
    //callback �禡�}�C�X�l��
    for( i = 0 ; i <EM_ServerListNet_Packet_Count ; i++ )
    {
        _PGCallBack[i] = &ServerListCliClass::_PGxUnKnoew;
    }
	_PGCallBack[EM_ServerListNet_StoC_ServerListResult]			= &ServerListCliClass::_ServerListNet_StoC_ServerListResult;
	_PGCallBack[EM_ServerListNet_StoC_ServerListFailed]			= &ServerListCliClass::_ServerListNet_StoC_ServerListFailed;
	_PGCallBack[EM_ServerListNet_StoC_GameRoleCount]			= &ServerListCliClass::_ServerListNet_StoC_GameRoleCount;
	_PGCallBack[EM_ServerListNet_StoC_ResetPassword]			= &ServerListCliClass::_ServerListNet_StoC_ResetPassword;
	_PGCallBack[EM_ServerListNet_StoC_ResetPasswordResult]		= &ServerListCliClass::_ServerListNet_StoC_ResetPasswordResult;
	_PGCallBack[EM_ServerListNet_StoC_ServerListFailedData]		= &ServerListCliClass::_ServerListNet_StoC_ServerListFailedData;
}
//----------------------------------------------------------------------------------------------
void	ServerListCliClass::_OnConnected( DWORD NetID )
{
	_IsReady = true;
	_NetID = NetID;
	_Listener->OnConnect( );

	//�n�DServerList�T��
	ServerListNet_CtoS_ServerListRequest Send;
	
	strncpy( Send.Account  , _Account.c_str() , sizeof(Send.Account) );
	strncpy( Send.Password , _Password.c_str() , sizeof(Send.Password) );
	Send.KickAccount = true;
	Send.HashedPassword = _HashedPassword;

	_BNet->Send( _NetID , sizeof(Send) , &Send );
	_SrvListInfo.clear();
}
//----------------------------------------------------------------------------------------------
void	ServerListCliClass::_OnDisconnected( DWORD id )
{
	_IsReady = false;
    Print( LV1 , "_OnDisconnected" , "ServerList �_�u" );
	_Listener->OnDisConnect( );

}
//----------------------------------------------------------------------------------------------
bool	ServerListCliClass::_OnPacket( DWORD NetID , DWORD Size , void* Data )
{
    ServerListNet_SysNull* PG = ( ServerListNet_SysNull *) Data;

     if(EM_ServerListNet_Packet_Count <= (UINT32)PG->Command )
     {
         Print( LV2 , "_OnPacket" ,"ServerListCliClass  �ʥ]cmd=%d�L�j " , NetID , PG->Command );
         return false;
     }
     //�I�sCallback �B�z
     (this->*_PGCallBack[PG->Command])( NetID , PG , Size );

    //------------------------------------------------------------------------------
    return true;
}
//----------------------------------------------------------------------------------------------
void	ServerListCliClass::_OnConnectFailed( int id )
{
	_IsReady = false;
	_Listener->OnConnectFailed( );
}
//----------------------------------------------------------------------------------------------
void ServerListCliClass::Init()
{
	InitPGCallBack( );
	_BListener  = NEW LBServerList( this );
	_BNet       = CreateNetBridge( );
	_IsReady	= false;
	_NetID		= -1;
}
//----------------------------------------------------------------------------------------------
ServerListCliClass::ServerListCliClass( )
{
	_BNet = NULL;
	_BListener = NULL;
	_Listener = NULL;
	_HashedPassword = false;
}
//----------------------------------------------------------------------------------------------
ServerListCliClass::~ServerListCliClass( )
{
	if ( _BNet )
		DeleteNetBridge( _BNet );

	if ( _BListener )
		delete _BListener;
}
//----------------------------------------------------------------------------------------------
void	ServerListCliClass::Connect( char* IP , int Port , string Account , string Password, bool HashedPassword ,  ServerListSCliListener* Listener  )
{
	_Listener = Listener;
    _Port	= Port;
    _IP		= IP;
	_Account = Account;
	_Password = Password;
	_HashedPassword = HashedPassword;

    int IPNum = ConvertIP( IP );
	g_pLogList->AddMessage( "Connecting to serverlist %s:%d",IP, Port );
    _BNet->Connect( IPNum , Port , _BListener );	

}
//----------------------------------------------------------------------------------------------
//�w�ɳB�z
void	ServerListCliClass::Process( )
{
    //_BNet->Flush();	// 2007/10/09 �b�̥~�h�Τ@ WaitAll flush
}
//----------------------------------------------------------------------------------------------
void    ServerListCliClass::_ServerListNet_StoC_ServerListResult      ( int NetID , ServerListNet_SysNull* Data , int Size )
{
	ServerListNet_StoC_ServerListResult* PG = (ServerListNet_StoC_ServerListResult*)Data;

	_Account = PG->Account;

	ServerListInfoStruct Temp;
	Temp.Name			 = PG->ServerName;
	Temp.IP				 = PG->IP_DNS;
	Temp.Port			 = PG->Port;
	Temp.UpdateTime		 = PG->UpdateTime;
	Temp.WorldID		 = PG->WorldID;	
	Temp.ServerStatus	 = PG->ServerStatus;
	Temp.ServerListMode  = PG->ServerlistMode;
	Temp.TotalPlayerRate = PG->TotalPlayerRate;

	for( int i = 0 ; i < PG->ParallelZoneCount ; i++ )
	{
		Temp.Loading.push_back( PG->PlayerRate[i] );
	}

	_SrvListInfo.push_back( Temp );

	if( (int)_SrvListInfo.size() == PG->TotalCount )
	{
		_Listener->OnRevServerList( PG->LastLoginWorldID , PG->Account , PG->Country , PG->IPNum , PG->Age , PG->ValidTime , PG->VivoxLicenseTime , _SrvListInfo  );
		_SrvListInfo.clear();
	}


	//�o�e�t�θ�T��Serverlist
	ServerListNet_CtoS_ClientSystemInfo Send;

	strncpy( Send.Account  , _Account.c_str() , sizeof(Send.Account) );
	strncpy( Send.MacAddress , g_pGameMain->GetMac() , sizeof(Send.MacAddress) );
	Send.DisplayCardVendorID = g_ruEngine->GetAdapterVendorID();
	Send.DisplayCardDeviceID = g_ruEngine->GetAdapterDeviceID();
	char szOS[256] = {0};
	if( !g_pGameMain->GetOSDisplayString( szOS , 256) )
	{
		sprintf(szOS, "Unknown OS");
	}
	strncpy( Send.OSINFO , szOS , sizeof(Send.OSINFO) );

	_BNet->Send( _NetID , sizeof(Send) , &Send );
}
//----------------------------------------------------------------------------------------------
void ServerListCliClass::_ServerListNet_StoC_ResetPassword( int NetID , ServerListNet_SysNull* Data , int Size )
{
	ServerListNet_StoC_ResetPassword* PG = (ServerListNet_StoC_ResetPassword*)Data;

	if (_Listener) {
		_Listener->OnResetPassword();
	}



	/*
	// for debug test
	ServerListNet_CtoS_ResetPasswordResult	Packet;
	
	sprintf( Packet.Account,		"hui01" );
	sprintf( Packet.OldPassword,	"hui" );
	sprintf( Packet.NewPassword,	"hui" );

	_BNet->Send( NetID, sizeof( Packet), &Packet );
	*/
}
//----------------------------------------------------------------------------------------------
void ServerListCliClass::_ServerListNet_StoC_ResetPasswordResult( int NetID , ServerListNet_SysNull* PG , int Size )
{
	int a = 3;
}
//----------------------------------------------------------------------------------------------
void    ServerListCliClass::_ServerListNet_StoC_ServerListFailed      ( int NetID , ServerListNet_SysNull* Data , int Size )
{
	ServerListNet_StoC_ServerListFailed* PG = (ServerListNet_StoC_ServerListFailed*)Data;
	_Listener->OnError( PG->Result );
	_BNet->Close( _NetID );
}
//----------------------------------------------------------------------------------------------
void    ServerListCliClass::_ServerListNet_StoC_GameRoleCount      ( int NetID , ServerListNet_SysNull* Data , int Size )
{
	ServerListNet_StoC_GameRoleCount* PG = (ServerListNet_StoC_GameRoleCount*)Data;
	_Listener->OnGameRoleCount( PG->RoleCount , PG->DataSize , PG->FileData );

//	_BNet->Close( _NetID );
}
//----------------------------------------------------------------------------------------------
void ServerListCliClass::_ServerListNet_StoC_ServerListFailedData( int NetID , ServerListNet_SysNull* Data , int Size )
{
	ServerListNet_StoC_ServerListFailedData* PG = (ServerListNet_StoC_ServerListFailedData*)Data;
	_Listener->OnErrorData( PG->Result , &(PG->Data) , sizeof(PG->Data) );
}
//----------------------------------------------------------------------------------------------
//�n�DServerList ���
void	ServerListCliClass::ServerListRequest( )
{
	ServerListNet_CtoS_ServerListRequest Send;

	strncpy( Send.Account  , _Account.c_str() , sizeof(Send.Account) );
	strncpy( Send.Password , _Password.c_str() , sizeof(Send.Password) );
	Send.KickAccount = false;

	_BNet->Send( _NetID , sizeof(Send) , &Send );

//	_SrvListInfo.clear();
}

//�]�wVivox���P�N�ɶ�
void	ServerListCliClass::SetVivoxLicenseTime( )
{
	ServerListNet_CtoS_SetVivoxLicenseTime Send;
	_BNet->Send( _NetID , sizeof(Send) , &Send );
}

//�����s�u
void	ServerListCliClass::NetClose( )
{
	_BNet->Close( _NetID );
}