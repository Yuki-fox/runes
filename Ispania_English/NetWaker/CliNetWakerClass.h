#pragma  once
#include "ServerList_Cli\ServerListPackageDef.h"
#include "NetWaker/NetWakerPackage.h"
#include "functionschedular\functionschedular.h"

#pragma unmanaged

#include "netbridge/NetBridge.h"
#include "SmallObj/SmallObj.h"


class CliNetWakerClass;
using namespace std;
using namespace NetWakerPackage;
using namespace NetWakerServerListPackage;
//---------------------------------------------------------------------------------
typedef void  (CliNetWakerClass::*NetPGCallBack) ( int	NetID , PGSysNULL* PG , int Size );
//#define NetPGCallBack         boost::function<void ( int , PGSysNULL*, int ) >
//---------------------------------------------------------------------------------
//處理與Switch & Proxy 間通訊傳輸的Class
/*
class LSwitch: public INewConnectionListener, public INetEventListener 
{ 
public:
    CliNetWakerClass	*Parent;
    INetEventListener* LSAPI OnNewConnection( LNETID ){return this;}
    void LSAPI OnConnected      ( LNETID NetID );
    void LSAPI OnDisconnected   ( LNETID NetID );
    void LSAPI OnPeerShutdown   ( LNETID NetID );
    bool LSAPI OnPacket         ( LNETID NetID, HBUFFER hIncomingData );
    void LSAPI OnConnectFailed  ( int NetID );
};
*/
class LBSwitch: public CEventObj
{
    CliNetWakerClass	*_Parent;
public:
    LBSwitch( CliNetWakerClass* value ) { _Parent = value; };
    virtual bool		OnRecv			( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData );
    virtual void		OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode );
    virtual void		OnConnect		( unsigned long dwNetID );
    virtual	void		OnDisconnect	( unsigned long dwNetID );
    virtual CEventObj*	OnAccept		( unsigned long dwNetID );
};
//---------------------------------------------------------------------------------
//處理與Switch & Proxy 間通訊傳輸的Class
/*
class LProxy: public INewConnectionListener, public INetEventListener 
{ 
public:
    CliNetWakerClass	*Parent;
    INetEventListener* LSAPI OnNewConnection( LNETID ){return this;}
    void LSAPI OnConnected      ( LNETID NetID );
    void LSAPI OnDisconnected   ( LNETID NetID );
    void LSAPI OnPeerShutdown   ( LNETID NetID );
    bool LSAPI OnPacket         ( LNETID NetID, HBUFFER hIncomingData );
    void LSAPI OnConnectFailed  ( int NetID );
};
*/
class LBProxy: public CEventObj
{
    CliNetWakerClass	*_Parent;
public:
    LBProxy( CliNetWakerClass* value ) { _Parent = value; };
    virtual bool		OnRecv			( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData );
    virtual void		OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode );
    virtual void		OnConnect		( unsigned long dwNetID );
    virtual	void		OnDisconnect	( unsigned long dwNetID );
    virtual CEventObj*	OnAccept		( unsigned long dwNetID );
};
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
class CliNetWakerClass
{
public:
    friend	LBSwitch;	
    friend	LBProxy;	

	//////////////////////////////////////////////////////////////////////////
	//client 端夾帶資料　等回傳封包處理時取出
	static KeyDataClass			_KeyData;
	//static MemoryBlockStruct	_Send_CliData;
	//static MemoryBlockStruct*	_Recvie_CliData;
	static map< string , vector<char> >		_Send_CliData;
	static map< string , vector<char> >*	_Recvie_CliData;
	short					_ClientDataKey;		
	//////////////////////////////////////////////////////////////////////////

    NewWakerEventListener*  _Listener;

    FunctionSchedularClass  _Schedular;

    CNetBridge*			    _NetBSwitch;		//主要是用連接Switch
    CNetBridge*			    _NetBProxy;  		//主要是用連接Proxy
    LBSwitch*		        _LBSwitch;          //資料列舉Class
    LBProxy* 		        _LBProxy;           //資料列舉Class
    int                     _ProxyNetID;
    int                     _SwitchNetID;

    int                     _ProxyID;           //所連到的Proxy 的 ID
    int                     _CliID;

    bool                    _ExitFlag;          //結束旗標
    bool                    _IsReady;           //登入ok
	bool					_HashedPassword;	//密碼已經經過MD5處理



    set<int>			    _SrvConnect;	    //Cli 與 哪些 GSrv 連結

    char                    _Account[ _Max_CliAccount_Name_Size_ ];
    char                    _Password[ _Max_CliPassword_Name_Size_ ];
	char                    _MacAddress[ _Max_CliMac_Address_Size_ ];


	string					_LastLoginIP;
	string					_LastLoginTime;

	int						_PlayTimeQuota;

    void	_OnSwitchConnected        ( DWORD NetID );
    void	_OnSwitchDisconnected     ( DWORD NetID );
    bool	_OnSwitchPacket           ( DWORD NetID, DWORD Size , void* Data );
    void    _OnSwitchConnectFailed    ( DWORD NetID );


    void	_OnProxyConnected        ( DWORD NetID );
    void	_OnProxyDisconnected     ( DWORD NetID );
    bool	_OnProxyPacket           ( DWORD NetID, DWORD Size , void* Data );
    void    _OnProxyConnectFailed    ( DWORD NetID );

    NetPGCallBack		    _PGCallBack[ EM_SysNet_Packet_Count ];
    vector<PGBaseInfo>	    _PGInfo;			//封包管理的資訊
    //-----------------------------------------------------------------------------------
    // 封包Call Back 處理
    //-----------------------------------------------------------------------------------
    void _PGxUnKnoew( int NetID , PGSysNULL* PG , int Size );
    //-----------------------------------------------------------------------------------
    //(Switch)
    //登入資料要求
    void _PG_LoginInfoRequest(  int NetID , PGSysNULL* PG , int Size );
    
    //登入 Proxy 要求
    void _PG_LoginInProxyNotify( int NetID , PGSysNULL* PG , int Size );

	//登入失敗時額外回傳的資料
	void _PG_LoginFailedData( int NetID , PGSysNULL* PG , int Size );

    //-----------------------------------------------------------------------------------
    //(Proxy)
    //登入資料要求
    void _PG_LoginProxyRequest( int NetID , PGSysNULL* PG , int Size );

	void _PG_LoginProxyInfoOK( int NetID , PGSysNULL* PG , int Size );

    //與 GSrv成立連線
    void _PGxGSrvOnConnect( int NetID , PGSysNULL* PG , int Size );
    //與 GSrv斷線
    void _PGxGSrvOnDisonnect( int NetID , PGSysNULL* PG , int Size );
    //收到 GSrv 轉送給 Client 的封包
    void _PGxDataGSrvToCli( int NetID , PGSysNULL* PG , int Size );

    //有其他的玩家用此帳號重新登入
    void _PGxCliRelogin( int NetID , PGSysNULL* PG , int Size );

    //-----------------------------------------------------------------------------------
    //重新連結 Switch Server
//    int  _ReConnectSwitch( SchedularInfo* Obj , void* InputClass );

    //-----------------------------------------------------------------------------------
    void	_ConnectProxy( char* IP , int Port , char* LastLoginIP , char* LastLoginTime );
public:

    CliNetWakerClass( );
    virtual ~CliNetWakerClass( );

	void	Init();

    void	ConnectSwitch( char* IP,int Port , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword );

	const char*	LastLoginIP()	{ return _LastLoginIP.c_str(); }
	const char*	LastLoginTime()	{ return _LastLoginTime.c_str(); } 

    //定時處理
    bool	Process( );

    //登出
    bool    Logout( ClientNetCloseTypeENUM Type );

	bool	IsLoginOK()		{ return _IsReady; }

    //設定事件回呼函式
    void    SetListener(  NewWakerEventListener* );

    //
    bool    SendToSwitch( int Size , void* Data );
    bool    SendToProxy( int Size , void* Data );

    bool    SendToGSrv( int GSrvID , int Size , void* Data );

	DWORD	GetProxyRespondTime();

	int		PlayTimeQuota()	{ return _PlayTimeQuota;}

	int		CliID()	{ return _CliID; }

	static void	PushClientData	( const char* Key , void* Data , int Size );
	static bool PopClientData	( const char* Key , void* Data , int Size );
    //-----------------------------------------------------------------------------------
    // 初始化 Client 與 Server 的 CallBack函式
    //-----------------------------------------------------------------------------------
    void	InitPGCallBack( );

#ifdef _CLIENT
	static void	OutputNetEngineProxy( int lv, const char* pszMsg );
	static void	OutputNetEngineSwitch( int lv, const char* pszMsg );

	static void	OutputNetEngineBGProxy( int lv, const char* pszMsg );
	static void	OutputNetEngineBGSwitch( int lv, const char* pszMsg );
#endif

	FunctionSchedularClass*  Schedular() { return &_Schedular;};
};

#pragma managed
