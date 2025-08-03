#ifndef _NETGLOBAL_H
#define _NETGLOBAL_H

#include "../NetWaker/CliNetWaker.h"
#include "RoleData/ObjectDataClass.h"
#include "../NetWaker/ServerList_Cli/ServerListCliclass.h"
//#include "../roledata/RoleDataEx.h"

using namespace std;

struct  ServerID
{
    ServerID()
    {
        m_iMaster		= -1;
        m_iLocal_Battle = -1;
		m_iLocal_Base	= -1;
		m_iChat			= -1;
    };

    int						m_iMaster;
    int                     m_iLocal_Battle;
    int                     m_iLocal_Base;
	int						m_iChat;
};

//-----------------------------------------------------------------------------
// 觸發事件
typedef void (*PGCallback)		(int Sockid , int Size , void* Data);

class CPGTempInfo
{
public:
	CPGTempInfo() : m_sockid(0), m_size(0), m_data(NULL), m_func(NULL) {}
	~CPGTempInfo() { Clear(); }

	void Clear()
	{
		if ( m_data ) {
			delete [] m_data;
			m_data = NULL;
		}
		m_size = 0;
		m_func = NULL;
	}

	void Call()
	{
		if ( m_func && m_data )
			m_func(m_sockid, m_size, m_data);
	}

	void SetInfo(int sockid, int size, char* data, PGCallback cb)
	{
		Clear();

		m_size = size;
		m_sockid = sockid;
		m_func = cb;
		if ( data )
		{
			m_data = new char[size];
			memcpy(m_data, data, size);
		}
	}

	const CPGTempInfo& operator = ( const CPGTempInfo& src )
	{
		SetInfo(src.m_sockid, src.m_size, src.m_data, src.m_func);
		return *this;
	}

private:
	int m_sockid;
	int m_size;
	char* m_data;
	PGCallback m_func;
};


class SrvListEvent:public ServerListSCliListener
{
public:
	virtual void	OnRevServerList( int LastLoginWorldID , const char* Account , const char* Country , int IPNum , int Age , int ValidTime , int VivoxLicenseTime  ,  vector<ServerListInfoStruct>& SrvList );
	virtual void	OnConnectFailed( );
	virtual void	OnConnect( );
	virtual void	OnDisConnect( );
	virtual void	OnError( ServerListFailed_ENUM ErrCode );
	virtual void	OnErrorData( ServerListFailed_ENUM ErrCode, void* Data, unsigned long Size);
	virtual void	OnGameRoleCount( char RoleCount[30] , int FileSize , char* FileData );
};

class CNetGlobal
{
public:
	static	void				Initial(const char* db_filename, const char* datalanguage, const char* stringlanguage );
	static	void				Release();
	static	void				Process( double frameTime, FLOAT elapsedTime );
    static  FunctionSchedularClass* Schedular();

	static GameObjDbStructEx*	St_GetObjDB(int OrgID);
	static int					GetKeyMagicID( int iKeyID )	{ return g_ObjectData->GetKeyMagicID( iKeyID ); }
	//當角色狀態有改變時
	static  void				St_UpDateEvent(RoleDataEx *Role , RoleUpdateEvent Event);

	static	GameObjDbStructEx*	GetObj( int ObjID );
	static	RoleDataEx*         RoleData()						{ return (RoleDataEx*)&_RoleData; }
	static	RoleValueData*		RoleBaseAttr()					{ return &_RoleBaseAttr; }
	static	bool				RoleDataLoaded()				{ return _RoleDataLoaded; }

	static	void				NetRecvEvent(bool boEvent);
	static	void				ClearTempEvent();
	static	void				PushTempEvent(int Sockid , int Size , void* Data, PGCallback func);
	static	void				SetUIFinish(bool boEnd);
	static bool     SendToLocalPublic( int Size , void* Data		);
protected:

	static bool							_IsInitial;
	static CliNetWaker			        _Net;
	static CliNetWaker			        _NetBattle;
	static SrvListEvent					_SrvListListener;
	static ServerID			            _ServerNetID;
	static PlayerRoleData				_RoleData;						//角色資料
	static bool							_RoleDataLoaded;				//角色資料已經載入
	static RoleValueData				_RoleBaseAttr;					//沒加上法術的基本能力	 	
	static bool							_InBattleGround;
	
	static	set<int>					s_localServerList;
	static	float						s_waitConnectTime;
	static int							s_lastNetProcTime;
	static	bool						s_boFinishUI;					// 界面是否載入結束
	static	vector<CPGTempInfo>			s_PGEvent;						// 封包


    static void		SetSrvID_Master			( int iServerID )			{ _ServerNetID.m_iMaster = iServerID; }
    static void		SetSrvID_Local_Base		( int iServerID );
	static void		SetSrvID_Local_Battle	( int iServerID );
	static void		SetSrvID_Chat			( int iServerID )			{ _ServerNetID.m_iChat = iServerID; }

//    static void     Print( int iLV, char* format,... );
//	static bool		NetConnectSrvList( char* IP , int Port , char* Account , char* Password );
    static bool     NetConnect( char* IP , int Port , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword );
    static bool     Logout( ClientNetCloseTypeENUM Type );

    // 封包傳輸	
	static bool     SendToLocal_Battle	( int Size , void* Data		);
	static bool     SendToLocal_Base	( int Size , void* Data		);
	//送給目前致能的Server
    static bool     SendToLocal		( int Size , void* Data		);
    static bool		SendToMaster	( int iSize, void* pData	);
    static bool     SendToChat		( int Size , void* Data		);
	static bool		SendToServer	( int iSrvID, int Size , void* Data );

	static int		GetRespondTime	();

	// 玩家載入
	static void		InitializeBaseData();
	static void		ReleaseBaseData();

};
//-------------------------------------------------------------------------------------------------
#define Def_PrintLV1     1	// 訊息輸出
#define Def_PrintLV2     2	// 特殊訊息
#define Def_PrintLV3     3	// 不可能會發生的錯誤, 如玩家發送錯誤封包
#define Def_PrintLV4     4	// 內不不可能產生的問題
#define Def_PrintLV5     5	// 會當
//-------------------------------------------------------------------------------------------------
#endif