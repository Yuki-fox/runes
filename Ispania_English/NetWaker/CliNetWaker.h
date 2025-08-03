#pragma once
#pragma warning (disable:4786)
#include "CliNetWakerClass.h"

#pragma unmanaged

//***********************************************************************************
// �w�q�^Call �禡
// �Ψӵ��U�禡������ �A �p�G���ƥ�o�ʹN�|�I�s�n�����禡
/*
typedef void (*OnLoginFunctionBase)			    ( string info );
typedef void (*OnLogoutFunctionBase)		    ( );
typedef void (*OnLoginFailFunctionBase)		    ( SYSLogFailedENUM );	

typedef void (*OnSrvConnectFunctionBase)	    ( int id , string GSrvName );	
typedef void (*OnSrvDisconnectFunctionBase)	    ( int id );	
typedef void (*OnSrvPacketFunctionBase)		    ( int id , int size , void* data);	

typedef void (*OnCliConnectFunctionBase)	    ( int id , string accountname );	
typedef void (*OnCliDisconnectFunctionBase)	    ( int id );	
typedef void (*OnSrvMsgFunctionBase)		    ( char* msg);	

typedef void (*OnCliReloginByOtherFunctionBase)	( int IPNum );	
*/
#define OnLoginFunctionBase         boost::function<void ( string info ) >
#define OnLogoutFunctionBase        boost::function<void ( ) >
#define OnLoginFailFunctionBase     boost::function<void ( SYSLogFailedENUM ) >
#define OnLoginFailDataFunctionBase boost::function<void ( SYSLogFailedENUM , void*, unsigned long) >

#define OnSrvConnectFunctionBase    boost::function<void ( int id , string GSrvName ) >
#define OnSrvDisconnectFunctionBase boost::function<void ( int id ) >
#define OnSrvPacketFunctionBase     boost::function<void ( int id , int size , void* data) >

#define OnCliConnectFunctionBase    boost::function<void ( int id , string accountname ) >
#define OnCliDisconnectFunctionBase boost::function<void ( int id ) >
#define OnCliPacketFunctionBase     boost::function<void ( int id , int size , void* data ) >
#define OnSrvMsgFunctionBase        boost::function<void ( char* msg) >

#define OnCliReloginByOtherFunctionBase        boost::function<void (int) >


//-----------------------------------------------------------------------------------
class	CliNetWaker;
//-----------------------------------------------------------------------------------
//CallBack class �]�w
class NetListener: public NewWakerEventListener
{
    CliNetWaker* _Parent;
public:
    NetListener( CliNetWaker* value ){ _Parent = value; };

    void	OnLogin				( string info );
    void	OnLogout			( );
    void	OnLoginFailed		( SYSLogFailedENUM type );
	void	OnLoginFailedData	( SYSLogFailedENUM, void* data, unsigned long size );

    void    OnGSrvConnected     ( int id , string GSrvName);
    void    OnGSrvDisconnected  ( int id );
    bool    OnGSrvPacket        ( int id, int size , void* data );	

    void	OnServerMsg			( char* Msg);

    void    OnCliReloginByOther ( int IPNum );
};
//-----------------------------------------------------------------------------------
class	CliNetWaker 
{
public:
    friend  NetListener;

    CliNetWakerClass*  	 _Net;
    NetListener*         _Listener;
   //-----------------------------------------------------------------------------------
    //���檬�A
    //-----------------------------------------------------------------------------------
    int	    _ExitFlag;

    int      _ProcTime;

    //------------------------------------------------------------------------------------------
    //�����n�J�ƥ�禡�����
    vector< OnLoginFunctionBase >			    _LoginFunctionList;
    vector< OnLogoutFunctionBase >			    _LogoutFunctionList;
    vector< OnLoginFailFunctionBase >		    _LoginFailFunctionList;
	vector< OnLoginFailDataFunctionBase >		_LoginFailDataFunctionList;

    vector< OnSrvConnectFunctionBase >		    _SrvConnectFunctionList;
    vector< OnSrvDisconnectFunctionBase >	    _SrvDisconnectFunctionList;
    vector< OnSrvPacketFunctionBase >		    _SrvPacketFunctionList;
	vector< int >								_SrvPacketDelay;				//����ʥ]�ƥ�

    vector< OnSrvMsgFunctionBase >			    _SrvMsgFunctionList;
    vector< OnCliReloginByOtherFunctionBase >   _CliReloginByOtherFunctionList;


    void	_OnLogin				( string info );
    void	_OnLogout				( );
    void	_OnLoginFailed			( SYSLogFailedENUM type );
	void	_OnLoginFailedData		( SYSLogFailedENUM type, void* data, unsigned long size );

    void	_OnGSrvConnected		( int id , string GSrvName );
    void	_OnGSrvDisconnected	    ( int id );
    bool	_OnGSrvPacket			( int id , int size , PacketBase* data );

    void	_OnServerMsg			( char*	Msg );

	void    _OnCliReloginByOther    ( int IPNum );

	static	int	_PacketDelaySchedular	( SchedularInfo* Obj , void* InputClass );
public:

    CliNetWaker();
    ~CliNetWaker();

	virtual	void Init()	{ _Net->Init(); }

	bool	IsLoginOK()	{ return _Net->IsLoginOK(); }
    //GateWay ��ƥ洫�B�z
    bool	Process( );
    //�s�� Switch Server
    void Connect( char* SwitchIP , int SwitchPort , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword );
    //��ۤvLogout
    void	Logout( ClientNetCloseTypeENUM Type );

	const char*	LastLoginIP()	{ return _Net->LastLoginIP(); }
	const char*	LastLoginTime()	{ return _Net->LastLoginTime(); } 

	int		PlayTimeQuota()		{ return _Net->PlayTimeQuota();}
    //-------------------------------------------------------------------------------------------
    //�e�ʥ]�� Server 
    bool	SendToSrv( int SrvID , int Size , void*	Buf );	
    //-------------------------------------------------------------------------------------------
	int		CliID()	{ return _Net->CliID(); }

	int		GetRespondTime			() { return _Net->GetProxyRespondTime(); }
    
    //�n���ƥ󪺨禡 (ret true ���� false ������)
    bool RegOnLoginFunction			( OnLoginFunctionBase );
    bool RegOnLogoutFunction		( OnLogoutFunctionBase );
    bool RegOnLoginFailFunction		( OnLoginFailFunctionBase ); 
	bool RegOnLoginFailDataFunction	( OnLoginFailDataFunctionBase );

    bool RegOnSrvConnectFunction	( OnSrvConnectFunctionBase );
    bool RegOnSrvDisConnectFunction	( OnSrvDisconnectFunctionBase );
    //�w�n�J�L���ʥ]id���i���еn�J DelayTiem ��� 1/1000��
    bool RegOnSrvPacketFunction		( int PGid , OnSrvPacketFunctionBase , int DelayTime = 0 );

    bool RegOnSrvMsgFunction		(  OnSrvMsgFunctionBase );
    
    bool RegOnCliReloginByOther  ( OnCliReloginByOtherFunctionBase );
    //-------------------------------------------------------------------------------------------
	FunctionSchedularClass*  Schedular() { return _Net->Schedular() ;};
	//static void	PushClientData( void* Data , int Size ) { CliNetWakerClass::PushClientData( Data , Size ); };
	//static MemoryBlockStruct* GetClientData( ) { return CliNetWakerClass::GetClientData(); } ;
	static void	PushClientData( const char* Key , void* Data , int Size ) { CliNetWakerClass::PushClientData( Key , Data , Size ); };
	static bool PopClientData( const char* Key , void* Data , int Size ) { return CliNetWakerClass::PopClientData( Key , Data , Size ); } ;
};

#pragma managed
//-----------------------------------------------------------------------------------

