#pragma once

#include "../interface.h"
#include "MyHttpFile/MyHttpFile.h"

class CAccountLogin;
extern CAccountLogin*			g_pAccountLogin;

#define	LOGIN_DISCONNECT		0
#define	LOGIN_CONNECTING		1
#define	LOGIN_CONNECTED			2

enum LoginProcessStep
{
	eLoginProcess_None				= 0,			// 等待輸入帳號密碼
	eLoginProcess_ConnectRealm		= 1,			// 連線Server List
};

enum BoardStringDownloadStatus
{
	eBoardStringNotDownloaded		 = 0,			//公告字串尚未被下載過
	eBoardStringReadytoDownloading   = 1,			//正在下載公告字串
	eBoardStringDownloaded			 = 2			//公告字串下載完畢
};

#define OnBoardStringDownloaded  boost::function<void ( const char* pszBoardString) >

//
struct sCameraLayout
{
	sCameraLayout()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
		pitch = 0.0f;
		yaw = 0.0f;
		roll = 0.0f;
		zoom = 0.0f;
		time = 0.0f;
	}

	sCameraLayout( float x, float y, float z, float _pitch, float _yaw, float _roll, float _zoom, float _time )
	{
		pos.x = x;
		pos.y = y;
		pos.z = z;
		pitch = _pitch;
		yaw = _yaw;
		roll = _roll;
		zoom = _zoom;
		time = _time;
	}

	CRuVector3 pos;
	float yaw   ;
	float pitch ;
	float roll  ;
	float zoom  ;
	float time  ;
};

struct ServerListInfo
{
	ServerListInfo()				{ worldID = 0; characters = 0; isPVP = false; isTop = false; isMaintain = false; load = 0.0f; }

	string		serverName;							// 伺服器名稱
	string		location;							// 地區編號
	string		serverIP;							// IP
	string		flagText;							// 不顯示國旗顯示字串
	int			serverPort;							// Port
	int			worldID;							// 伺服器編號
	int			characters;							// 角色人數
	int			serverStatus;						// 伺服器狀態(0 正常 1維修 2隱藏)
	bool		isPVP;								// 類型是否為PVP
	bool		isTop;								// 置頂
	bool		isLogin;							// 是否可以登入	
	bool		isMaintain;							// 維修中
	bool		isNew;								// 新伺服器
	bool		isLimitNewAccount;					// 伺服器已滿不能進入
	float		load;								// 負載量(0 伺服器未開啟)
	int			age;								// 年齡限制0 不限制
};

struct CompressStringData
{
	CompressStringData();
	~CompressStringData();

	void Clear();
	void SetString(const char* str);
	std::string GetString();

	char*		data;
	int			decodeSize;
	int			size;
};


class CAccountLogin : public CInterfaceBase
{
public:
	CAccountLogin(CInterface* object);
	virtual ~CAccountLogin();
	
	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	void				Login(const char* name, const char* password);
	virtual void				ReLogin();
	virtual	void				LoginResult(int resultType);
	virtual void				LoginFailedData(int resultType, void* data, unsigned long size);
	virtual	void				Logout(int closeType, bool forceLogout=false);
	virtual	void				ReceiveLogout();
	
	virtual	void				OnConnectRealm();
	virtual	void				OnDisconnectRealm();
	virtual	void				OnConnectRealmFailed(int resultType);
	virtual	void				OnConnectRealmFailedData(int resultType, void* data, unsigned long size);
	virtual void				UpdateRealmList(float delayTime=2.0f);
	virtual void				SetRealmName(const char* name, bool update=true);
	virtual	string				GetRealmName()						{ return m_realmName; }
	virtual int					GetSelectedRegion()					{ return m_selectedRegion; }
	virtual	void				SetAccountName(const char* name);
	virtual	const char*			GetAccountName();
	virtual	void				SetServerPassword(const char* password);
	virtual	string				GetServerPassword();
	virtual void				SetSelectedPlayerName(const char* name)	{ m_selctedPlayerName = name; }
	virtual const char*			GetSelectedPlayerName()				{ return m_selctedPlayerName.c_str(); }

	virtual void				SendSecondPassword();
	virtual int					SetSecondPassword(const char* password1, const char* password2);

	void						SetCameraLayout( sCameraLayout& rCameraLayout );
 	void						AddControlCamera( sCameraLayout& rCameraLayout );
   	void						CleanControlCamera();
	void						UpdateCamera( sCameraLayout& rCameraLayout );

  	void						GetCameraLayout( sCameraLayout& rCameraLayout );

	void						SetParalleZoneID(int paralleZoneID)	{ m_paralleZoneID = paralleZoneID; }
	int							GetParalleZoneID()					{ return m_paralleZoneID; }	

	void						ChangeParallelID(int id);
	void						ClearChangeParallelID()					{ m_changeParallelID = -1; }
	int							GetChangeParallelID()					{ return m_changeParallelID; }

	void						SetLastLoginWorldID(int lastWorldID)	{ m_lastLoginWorldID = lastWorldID; }
	bool						CheckServerVersion();
	void						LoadServerList(char* data, int size, int characters[30]);
	void						SortServerList(const char* item, int forceOrder = 0);

	//misc
	void						DownloadBoardString(OnBoardStringDownloaded cbDownloaded);
	void						CheckBoardStringDownloadStatus();
	static void					ReceiveDownloadBoardString(const char* text);
	const char*					GetBoardString()						{ return m_boardString.c_str(); }

	void						SetServerAccount(const char* text);
	const char*					GetServerAccount();	

	const char*					FindWorldNameByID(int worldID);
	int							FindWorldIDByName(const char* serverName);
	int							GetWorldID()							{ return m_worldID; }

	// Server List
	void						CloseServerList();
	void						RequestServerList();
	void						SetIPCountry(const char* country)		{ m_ipCountry = country; }
	void						ClearRealmName()						{ m_realmName.clear(); }	

	// second password
	void						SendConfirmSecondPassword(const char* password);
	void						RM_SecondPasswordRequest();
	void						RM_SecondPasswordResult(bool Result);
	
	// second password v2
	void						SM_SecondPassword2(const char* password);
	void						SM_CaptchaRefresh();
	void						SM_CaptchaReply( const char* Captcha );
	void						RM_SecondPasswordRequest2();
	void						RM_SecondPasswordResult2( bool Result );
	void						RM_CaptchaCheck(void* Captcha, int CaptchaSize);
	void						RM_CaptchaCheckResult( int Result );

	void						SetAge(int age)							{ m_age = age; }
	int							GetAge()								{ return m_age; }
	int							GetServerAge()							{ return m_serverAge; }
	void						SetValidTime(int validTime)				{ m_validTime = validTime; }
	int							GetValidTime()							{ return m_validTime; }
	void						SetVivoxLicenseTime(int vivoxTime);
	int							GetVivoxLicenseTime()					{ return m_vivoxLincenseTime; }

	void						SetPasswordErrorCount(int count)		{ m_inputErrorCount = count; }
	int							GetPasswordErrorCount()					{ return m_inputErrorCount; }

	void						SetReturnSelectCharacter(bool locked)	{ m_returnSelectCharacter = locked; }
	bool						GetReturnSelectCharacter()				{ return m_returnSelectCharacter; }

#ifdef KALYDO
	// For single sign on, we need a boolean whether the credentials are available, so the game knows when to login automatically.
	void						SetShouldLogin(bool shouldLogin)		{ m_ShouldLogin = shouldLogin; }
	bool						GetShouldLogin()						{ return m_ShouldLogin; }
#endif // KALYDO

protected:

	struct {
		int						gameServer;
		int						realmList;
	}m_connect;	
	
	string						m_realmName;
	string						m_account;
	string						m_serverAccountName;
	string						m_gameServerIP;
	string						m_selctedPlayerName;
	string						m_boardString;
	int							m_gameServerPort;
	int							m_selectedRegion;
	int							m_paralleZoneID;
	float						m_delayLoginTime;
	bool						m_ignoreLogout;
	int							m_changeParallelID;
	int							m_worldID;
	bool						m_serverListData;					// 是否已伺服器連線
	float						m_lastReciveServerListTime;			// 最後收到伺服器列表清單
	int							m_lastLoginWorldID;					// 最後登入世界ID

	int							m_passwordFailedCount;
	bool						m_showConfirmPassword;
	string						m_secondPassword;					// 輸入二次密碼
	string						m_tempSecondPassword;				// 等待確認
	string						m_serverListAccount;
	string						m_ipCountry;						// IP所在地區

	//board string
	MyHttpFile					m_boardStringFile;
	int							m_boardStringDownloadStatus;
	OnBoardStringDownloaded		m_cbBoardStringDownloaded;
	int							m_serverAge;						// 伺服器限制年齡
	int							m_age;								// 年齡限制
	int							m_validTime;						// 有效時間(秒)
	int							m_vivoxLincenseTime;				// 記錄是否有確認過時間
	int							m_inputErrorCount;					// 二次密碼輸入錯誤次數
	bool						m_returnSelectCharacter;			// 返迴角色選單

	int							m_PasswordErrorCount;				// 密碼輸入錯誤次數
	int							m_MaxPasswordErrorCount;			// 最大可允許的密碼輸入錯誤次數
	int							m_FreezeTime;						// 密碼輸入錯誤次數超過上限時, 帳號會被凍結的時間(秒)
	int							m_AccountFreezeRemainingTime;		// 帳號凍結剩餘時間

#ifdef KALYDO
	bool						m_ShouldLogin;
#endif // KALYDO
 public:
	 CRuVector3 m_CameraLookAt;
	 CRuVector3 m_CameraEye;
	 CRuVector3 m_CameraUp;
	 CRuVector3 m_SmoothCameraLookAt;
	 CRuVector3 m_SmoothCameraEye;
	 CRuVector3 m_SmoothCameraUp;
	 sCameraLayout m_CurrentCameraLayout;
	 std::list<sCameraLayout> m_ControlCameraLayoutList;
	 float m_fControlCameraTime;

	 string						m_selectServerName;

	 int						m_selectServerList;
	 vector<ServerListInfo>		m_serverList;

public:
	//Login畫面 起始的鏡頭設定
	//sCameraLayout m_AccountLoginCameraLayoutBegin;

	//Login畫面 最後的鏡頭設定
	//sCameraLayout m_AccountLoginCameraLayout;
protected:
	//Login畫面 鏡頭設定
	std::list<sCameraLayout> m_AccountLoginCameraLayout;
public:
	sCameraLayout& GetAccountLoginCameraLayoutBegin();
	sCameraLayout& GetAccountLoginCameraLayoutEnd();

 	void DoCameraMotion();

public:


	float m_fPlayerHeight;
	CRuVector3 m_PlayerDirection;

	void InitLoginEnvironment();


	//CompressStringData			m_accountData;
	CompressStringData			m_passwordData;
};