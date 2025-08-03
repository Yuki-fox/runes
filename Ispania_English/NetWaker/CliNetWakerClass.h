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
//�B�z�PSwitch & Proxy ���q�T�ǿ骺Class
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
//�B�z�PSwitch & Proxy ���q�T�ǿ骺Class
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
	//client �ݧ��a��ơ@���^�ǫʥ]�B�z�ɨ��X
	static KeyDataClass			_KeyData;
	//static MemoryBlockStruct	_Send_CliData;
	//static MemoryBlockStruct*	_Recvie_CliData;
	static map< string , vector<char> >		_Send_CliData;
	static map< string , vector<char> >*	_Recvie_CliData;
	short					_ClientDataKey;		
	//////////////////////////////////////////////////////////////////////////

    NewWakerEventListener*  _Listener;

    FunctionSchedularClass  _Schedular;

    CNetBridge*			    _NetBSwitch;		//�D�n�O�γs��Switch
    CNetBridge*			    _NetBProxy;  		//�D�n�O�γs��Proxy
    LBSwitch*		        _LBSwitch;          //��ƦC�|Class
    LBProxy* 		        _LBProxy;           //��ƦC�|Class
    int                     _ProxyNetID;
    int                     _SwitchNetID;

    int                     _ProxyID;           //�ҳs�쪺Proxy �� ID
    int                     _CliID;

    bool                    _ExitFlag;          //�����X��
    bool                    _IsReady;           //�n�Jok
	bool					_HashedPassword;	//�K�X�w�g�g�LMD5�B�z



    set<int>			    _SrvConnect;	    //Cli �P ���� GSrv �s��

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
    vector<PGBaseInfo>	    _PGInfo;			//�ʥ]�޲z����T
    //-----------------------------------------------------------------------------------
    // �ʥ]Call Back �B�z
    //-----------------------------------------------------------------------------------
    void _PGxUnKnoew( int NetID , PGSysNULL* PG , int Size );
    //-----------------------------------------------------------------------------------
    //(Switch)
    //�n�J��ƭn�D
    void _PG_LoginInfoRequest(  int NetID , PGSysNULL* PG , int Size );
    
    //�n�J Proxy �n�D
    void _PG_LoginInProxyNotify( int NetID , PGSysNULL* PG , int Size );

	//�n�J���Ѯ��B�~�^�Ǫ����
	void _PG_LoginFailedData( int NetID , PGSysNULL* PG , int Size );

    //-----------------------------------------------------------------------------------
    //(Proxy)
    //�n�J��ƭn�D
    void _PG_LoginProxyRequest( int NetID , PGSysNULL* PG , int Size );

	void _PG_LoginProxyInfoOK( int NetID , PGSysNULL* PG , int Size );

    //�P GSrv���߳s�u
    void _PGxGSrvOnConnect( int NetID , PGSysNULL* PG , int Size );
    //�P GSrv�_�u
    void _PGxGSrvOnDisonnect( int NetID , PGSysNULL* PG , int Size );
    //���� GSrv ��e�� Client ���ʥ]
    void _PGxDataGSrvToCli( int NetID , PGSysNULL* PG , int Size );

    //����L�����a�Φ��b�����s�n�J
    void _PGxCliRelogin( int NetID , PGSysNULL* PG , int Size );

    //-----------------------------------------------------------------------------------
    //���s�s�� Switch Server
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

    //�w�ɳB�z
    bool	Process( );

    //�n�X
    bool    Logout( ClientNetCloseTypeENUM Type );

	bool	IsLoginOK()		{ return _IsReady; }

    //�]�w�ƥ�^�I�禡
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
    // ��l�� Client �P Server �� CallBack�禡
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
