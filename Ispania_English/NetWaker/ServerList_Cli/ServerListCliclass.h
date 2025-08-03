#pragma once
#pragma warning (disable:4786)

#include <set>
#include <vector>
#include <map>
#include <string>

#include "NetBridge/NetBridge.h"

#include "functionschedular/functionschedular.h"

#include "ServerListPackage.h"

//---------------------------------------------------------------------------------
using namespace std;
using namespace NetWakerServerListPackage;

class ServerListCliClass;
//---------------------------------------------------------------------------------
struct ServerListInfoStruct
{
	string	Name;
	string	IP;
	int		Port;
	int		UpdateTime;				//多久前Update
	int		WorldID;
	vector <float>	Loading;		//每個平行空間的人數Loading
	int		ServerListMode;			//0 表正常, 1 表維修.
	int		ServerStatus;			//0 為正常[ 預設 ], 1 為[維修], 2, 為隱藏
	float	TotalPlayerRate;

	float			GetAverageLoading()
	{
		float load = 0.0f;
		for ( vector<float>::iterator iter = Loading.begin(); iter != Loading.end(); iter++ )
			load += min(1.0f, *iter);
		load /= max(1, Loading.size());
		return load;
	}

	int				GetGroupID()
	{
		return WorldID >> 16;
	}

	int				GetWorldID()
	{
		return WorldID & 0xFFFF;
	}

	bool			operator<(ServerListInfoStruct& src)
	{
		return GetAverageLoading() < src.GetAverageLoading();
	}	
};
//---------------------------------------------------------------------------------
class ServerListSCliListener
{
public:
	virtual void	OnRevServerList( int LastLoginWorldID , const char* Account , const char* Country , int IPNum , int Age , int ValidTime , int VivoxLicenseTime ,  vector<ServerListInfoStruct>& SrvList ) = 0;
	virtual void	OnConnectFailed( )=0;
	virtual void	OnConnect( )=0;
	virtual void	OnDisConnect( )=0;
	virtual void	OnError( ServerListFailed_ENUM ErrCode )=0;
	virtual void	OnErrorData( ServerListFailed_ENUM ErrCode, void* Data, unsigned long Size)=0;
	virtual void	OnGameRoleCount( char RoleCount[30] , int FileSize , char* FileData ) = 0;
	virtual void	OnResetPassword() {}
};
//---------------------------------------------------------------------------------
typedef void  (ServerListCliClass::*SrvList_NetPGCallBack) ( int NetID , ServerListNet_SysNull* PG , int Size );
//---------------------------------------------------------------------------------
//處理與Server間通訊傳輸的Class
class LBServerList: public CEventObj
{
    ServerListCliClass	*_Parent;
public:
    LBServerList( ServerListCliClass* value ) { _Parent = value; };
    virtual bool		OnRecv			( unsigned long dwNetID, unsigned long dwPackSize, PVOID pPacketData );
    virtual void		OnConnectFailed	( unsigned long dwNetID, unsigned long dwFailedCode );
    virtual void		OnConnect		( unsigned long dwNetID );
    virtual	void		OnDisconnect	( unsigned long dwNetID );
    virtual CEventObj*	OnAccept		( unsigned long dwNetID );
};
//---------------------------------------------------------------------------------
//#define _Sys_Max_CallbackCount_ 256
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
class ServerListCliClass  
{
	friend	LBServerList;	

    CNetBridge*			    		_BNet;		//主要是用來給Game Server連結用的
	LBServerList*					_BListener;	//資料列舉Class
    ServerListSCliListener* 		_Listener;

	bool							_IsReady;

	string							_IP;
	int								_Port;
	int								_NetID;

	string							_Account;
	string							_Password;
	bool							_HashedPassword;
	vector<ServerListInfoStruct>	_SrvListInfo;
	//-------------------------------------------------------------------------------------
	void	_OnConnected( DWORD id );
	void	_OnDisconnected( DWORD id );
	bool	_OnPacket( DWORD id, DWORD Size , void* Data );
	void	_OnConnectFailed( int id );
	//-----------------------------------------------------------------------------------
	// 封包Call Back 處理
	//-----------------------------------------------------------------------------------
	void    _ServerListNet_StoC_ServerListResult		( int NetID , ServerListNet_SysNull* PG , int Size );
	void    _ServerListNet_StoC_ServerListFailed		( int NetID , ServerListNet_SysNull* PG , int Size );
	void    _ServerListNet_StoC_GameRoleCount			( int NetID , ServerListNet_SysNull* PG , int Size );
	void    _ServerListNet_StoC_ResetPassword			( int NetID , ServerListNet_SysNull* PG , int Size );
	void    _ServerListNet_StoC_ResetPasswordResult		( int NetID , ServerListNet_SysNull* PG , int Size );
	void    _ServerListNet_StoC_ServerListFailedData	( int NetID , ServerListNet_SysNull* PG , int Size );

    //-----------------------------------------------------------------------------------
    SrvList_NetPGCallBack   _PGCallBack[ EM_ServerListNet_Packet_Count ];
    void    _PGxUnKnoew( int NetID , ServerListNet_SysNull* PG , int Size ){};
public:
	ServerListCliClass();
	virtual ~ServerListCliClass();

	void	Init	();
    //-----------------------------------------------------------------------------------
	void	Connect( char* IP , int Port , string Account , string Password, bool HashedPassword , ServerListSCliListener* Listener );

	//定時處理
	void	Process();

	//要求ServerList 資料
	void	ServerListRequest( );

	//設定Vivox的同意時間
	void	SetVivoxLicenseTime( );
	//關閉連線
	void	NetClose();

	//-----------------------------------------------------------------------------------
	// 初始化 Client 與 Server 的 CallBack函式
	//-----------------------------------------------------------------------------------
	void	InitPGCallBack();

};

//-----------------------------------------------------------------------------------------------

