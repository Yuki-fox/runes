
#include	"CliNetWaker.h"
#include	"debuglog/errormessage.h"
#ifdef _CLIENT
	#include	"../ErrorList/errorlist.h"
#endif
#pragma unmanaged

void	NetListener::OnLogin				( string info )
{
    _Parent->_OnLogin( info );
}
void	NetListener::OnLogout			    ( )
{
    _Parent->_OnLogout( );
}
void	NetListener::OnLoginFailed		    ( SYSLogFailedENUM type )
{
    _Parent->_OnLoginFailed( type );
}

void	NetListener::OnLoginFailedData( SYSLogFailedENUM type , void* data, unsigned long size )
{
	_Parent->_OnLoginFailedData( type, data, size);
}

void    NetListener::OnGSrvConnected        ( int id , string GSrvName)
{
    _Parent->_OnGSrvConnected( id , GSrvName );
}
void    NetListener::OnGSrvDisconnected     ( int id )
{
    _Parent->_OnGSrvDisconnected( id );
}
bool    NetListener::OnGSrvPacket           ( int id, int size , void* data )	
{
    return _Parent->_OnGSrvPacket( id , size , (PacketBase*)data );
}

void	NetListener::OnServerMsg			( char* Msg)
{
    _Parent->_OnServerMsg( Msg );
}

void    NetListener::OnCliReloginByOther ( int IPNum )
{
    _Parent->_OnCliReloginByOther( IPNum );
}
//------------------------------------------------------------------------------------------

void	CliNetWaker::_OnLogin				( string info )
{
    TRACE1( "\n_OnLogin : info=%s 登入" , info.c_str() );
    for( myUInt32 i = 0 ; i < _LoginFunctionList.size() ; i++ )
    {
        _LoginFunctionList[i]( info );
    }
}
void	CliNetWaker::_OnLogout				( )
{
    TRACE0( "\n_OnLogout  登出" );
    for( myUInt32 i = 0 ; i < _LogoutFunctionList.size() ; i++ )
    {
        _LogoutFunctionList[i]( );
    }
}
void	CliNetWaker::_OnLoginFailed		( SYSLogFailedENUM type )
{
    TRACE0( "\n_OnLoginFailed  登入失敗" );
    for( myUInt32 i = 0 ; i < _LoginFailFunctionList.size() ; i++ )
    {
        _LoginFailFunctionList[i]( type );
    }
}

void	CliNetWaker::_OnLoginFailedData( SYSLogFailedENUM type, void* data, unsigned long size )
{
	for( myUInt32 i = 0 ; i < _LoginFailDataFunctionList.size() ; i++ )
	{
		_LoginFailDataFunctionList[i]( type, data, size);
	}
}


void	CliNetWaker::_OnGSrvConnected	    ( int id , string GSrvName )
{
    TRACE2( "\n_OnGSrvConnected  有GSrv成立聯線 id=%d GSrvName=%s" , id , GSrvName.c_str() );

    for( myUInt32 i = 0 ; i < _SrvConnectFunctionList.size() ; i++ )
    {
        _SrvConnectFunctionList[i]( id , GSrvName );
    }
}
void	CliNetWaker::_OnGSrvDisconnected	( int id )
{
    TRACE1( "\n_OnGSrvDisconnected  有GSrv斷線 id=%d " , id );
    for( myUInt32 i = 0 ; i < _SrvDisconnectFunctionList.size() ; i++ )
    {
        _SrvDisconnectFunctionList[i]( id );
    }
}

int  CliNetWaker::_PacketDelaySchedular		( SchedularInfo* Obj , void* InputClass )
{
	CliNetWaker* This = (CliNetWaker*)InputClass;
	int	size = Obj->GetNumber("size");
	int	id	 = Obj->GetNumber("id");
	char* data = (char*)Obj->GetUINT64( "data" );
	PacketBase* PG = (PacketBase*)data;

	OnSrvPacketFunctionBase PGFunc;
	PGFunc = This->_SrvPacketFunctionList[ PG->Command ];
	PGFunc( id , size , data );
	
	delete data;
	return 0;
}

bool	CliNetWaker::_OnGSrvPacket			( int id , int size , PacketBase* data )
{
    PGSysNULL*    PG = (PGSysNULL*)data;

    if( (unsigned)data->Command >= _SrvPacketFunctionList.size() )
    {
        TRACE1("\n_OnSrvPacket 封包號碼過大?? ( %d )", PG->Command );
        return false;
    }
    OnSrvPacketFunctionBase PGFunc;

    PGFunc = _SrvPacketFunctionList[ data->Command ];

    if( PGFunc == NULL )
    {
        TRACE1("\n_OnSrvPacket 此封包尚未註冊?? ( %d )", PG->Command );
        return false;
    }

	//g_pLogList->AddMessage( "_OnGSrvPacket = %d, size = %d", PG->Command , size );

	if(		_SrvPacketDelay.size() > (unsigned)id 
		&&	_SrvPacketDelay[PG->Command] != 0 )
	{
		char* NewData = new char[ size];
		
		memcpy( NewData , data , size );

		_Net->Schedular()->Push( _PacketDelaySchedular , _SrvPacketDelay[PG->Command] , this 
			, "data"	, EM_ValueType_uint64	, NewData	
			, "id"		, EM_ValueType_int	, id		
			, "size"	, EM_ValueType_int	, size		
			, NULL  );
	}
	else
	{
		PGFunc( id , size , data );
	}

    return true;
}

void	CliNetWaker::_OnServerMsg			( char*	Msg )
{
    TRACE1( "\n_OnServerMsg  Msg = %s " , Msg );
}
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------
//GateWay 資料交換處理
bool	CliNetWaker::Process( )
{
    _Net->Process( );
    return !_ExitFlag;
}
//------------------------------------------------------------------------------------------
//連到 Switch Server
void CliNetWaker::Connect( char* SwitchIP , int SwitchPort , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword )
{
    //設定 GSrv 的名稱
    _Net->ConnectSwitch( SwitchIP , SwitchPort , Account , Password, pszMacAddress, HashedPassword );

}
//------------------------------------------------------------------------------------------
CliNetWaker::CliNetWaker( )
{
    _ExitFlag     = false;
    _ProcTime     = 10;
    _Listener     = new NetListener( this );

    _Net = new CliNetWakerClass;
    _Net->SetListener( _Listener );
  
}
//------------------------------------------------------------------------------------------
CliNetWaker::~CliNetWaker( )
{
	delete _Net;
	delete _Listener;
}
//------------------------------------------------------------------------------------------
//把自己Logout
void CliNetWaker::Logout( ClientNetCloseTypeENUM Type )
{
	#ifdef _CLIENT
		g_pLogList->AddMessage( "CliNetWaker::Logout[%d]", (int)Type );
	#endif
    _Net->Logout( Type );
}
//***********************************************************************************
//登錄事件的函式 (ret true 為成 false 為失敗)
bool    CliNetWaker::RegOnLoginFunction			( OnLoginFunctionBase Func )
{
    _LoginFunctionList.push_back( Func );
    return true;
}
bool    CliNetWaker::RegOnLogoutFunction			( OnLogoutFunctionBase Func )
{
    _LogoutFunctionList.push_back( Func );
    return true;
}
bool    CliNetWaker::RegOnLoginFailFunction		( OnLoginFailFunctionBase Func ) 
{
    _LoginFailFunctionList.push_back( Func );
    return true;
}

bool CliNetWaker::RegOnLoginFailDataFunction( OnLoginFailDataFunctionBase Func )
{
	_LoginFailDataFunctionList.push_back(Func);
	return true;
}

bool    CliNetWaker::RegOnSrvConnectFunction		( OnSrvConnectFunctionBase Func )
{
    _SrvConnectFunctionList.push_back( Func );
    return true;
}
bool    CliNetWaker::RegOnSrvDisConnectFunction	( OnSrvDisconnectFunctionBase Func )
{
    _SrvDisconnectFunctionList.push_back( Func );
    return true;
}
bool    CliNetWaker::RegOnSrvMsgFunction		(  OnSrvMsgFunctionBase Func)
{
    _SrvMsgFunctionList.push_back( Func );
    return true;
}
//已登入過的封包id不可重覆登入
bool    CliNetWaker::RegOnSrvPacketFunction		( int PGid , OnSrvPacketFunctionBase Func , int DealyTime )
{
	
    if( PGid < 0  )
        return false;

    while(  (int)_SrvPacketFunctionList.size() <= PGid )
	{
        _SrvPacketFunctionList.push_back( NULL );
	}

	while( (int)_SrvPacketDelay.size() <= PGid )
	{
		_SrvPacketDelay.push_back( 0 );
	}

    if( _SrvPacketFunctionList[ PGid ] != NULL )
    {
		char Buf[512];
		sprintf( Buf , "RegOnSrvPacketFunction PGId=%d", PGid  );
		MessageBoxA(NULL , Buf , "Register Error" , MB_OK );
        TRACE1("\n **警告**RegOnSrvPacketFunction PGId=%d 重復註冊 " , PGid );
    }
    _SrvPacketFunctionList[ PGid ] = Func;
	_SrvPacketDelay[ PGid ] = DealyTime;
	
    return true;
}
//------------------------------------------------------------------------------------------
//送封包給Server or Client
bool    CliNetWaker::SendToSrv( int SrvID , int Size , void*	Buf )	
{
    return _Net->SendToGSrv( SrvID , Size , Buf );
}
//------------------------------------------------------------------------------------------
bool    CliNetWaker::RegOnCliReloginByOther		( OnCliReloginByOtherFunctionBase Func )
{
    _CliReloginByOtherFunctionList.push_back( Func );
    return true;
}
//------------------------------------------------------------------------------------------
void	CliNetWaker::_OnCliReloginByOther				( int IPNum )
{
	
    TRACE0( "其他玩家登入此帳號" );
    for( myUInt32 i = 0 ; i < _CliReloginByOtherFunctionList.size() ; i++ )
    {
        _CliReloginByOtherFunctionList[i]( IPNum );
    }
	
}
//------------------------------------------------------------------------------------------

#pragma managed