#pragma once

#include <Windows.h>
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "DefRegisterEvent.h"
#include "../ui/core/ui.h"

class CInterface;
extern	CInterface*			g_pInterface;

using namespace std;

#define INTERFACE_SAVE_PATH		"OneForAll"

#define MONEY_MODE_COPPER		"copper"							// 遊戲幣
#define MONEY_MODE_ACCOUNT		"account"							// 帳號幣(商品貨幣)
#define MONEY_MODE_BONUS		"bonus"								// 紅利幣
#define MONEY_MODE_BILLDIN		"billdin"							//必爾丁

#define DELAY_SAVE_TIME			30.0f								// 延遲儲存時間

// 儲存資料 ID 類型
enum eSaveClientDataType
{
	eSaveClientData_Header				= 0,						// 檔頭
	eSaveClientData_Setting				,							// 設定資料
	eSaveClientData_Action				,							// 快捷
	eSaveClientData_Macro				,							// 巨集
	eSaveClientData_Chat				,							// 交談
	eSaveClientData_Channel				,							// 頻道
	eSaveClientData_QuestTrack			,							// 任務追蹤
	eSaveClientData_MSNInfo				,							// MSN位置
	eSaveClientData_ActionBarSetting	,							// 快捷列設定
	eSaveClientData_ItemMallHistory		,							// 歷史購物
	eSaveClientData_KeyBinding			,							// 熱鍵設置
	//請在下面加新的列舉 不要中插
	eSaveClientData_SkillPlate			,							//套裝技能技能盤的
	eSaveClientData_EquipSlotTab		,							//快速換裝表
	eSaveClientData_GameBar				,							//轉盤

	eSaveClientData_Last				,							// 最後一筆
};

enum TempFilePosition
{
	eTempFilePosition_Begin		= 0,			// 開始
	eTempFilePosition_Current	= 1,			// 目前位置
	eTempFilePosition_End		= 2,			// 結尾
};

struct CClientSaveData 
{
	CClientSaveData()	
	{
		m_size = 0;
		m_data = NULL;
	}

	void Release()
	{
		m_size = 0;
		if ( m_data ) delete [] m_data; m_data = NULL;
	}

	int		m_size;
	char*	m_data;
};

class CTempFile
{
public:
	CTempFile();
	CTempFile(int size);
	CTempFile(char* inData, int inDataSize);
	~CTempFile();	

	void		Open(int newSize=65535);
	void		Close();
	int			Read(void* outData, int outDataSize);
	int			Write(void* data, int dataSize);
	void		SetData(char* data, int size);
	int			SetPosition(TempFilePosition position, int offset);
	char*		GetData()							{ return m_data; }	
	int			GetSize()							{ return m_dataSize; }
	int			GetPosition()						{ return m_position; }

private:
	void		Clear();

private:
	bool		m_isNew;
	char*		m_data;
	int			m_dataSize;
	int			m_position;
};

class CInterfaceBase
{
public:
	CInterfaceBase(CInterface* object);
	virtual ~CInterfaceBase();
	
	virtual	void				Update(float elapsedTime)				{}
	virtual void				Render()								{}
	virtual	void				RegisterFunc()							{}
	virtual	lua_State*			GetLuaStateLogin()						{ return GetLuaState(); }
	virtual	lua_State*			GetLuaStateWorld()						{ return GetLuaState(); }
	virtual	lua_State*			GetLuaState();
	virtual	void				SendLoginEvent(const char* event);
	virtual	void				SendWorldEvent(const char* event);
	virtual	void				LockedItem()							{}
	virtual	void				Save()									{}
	virtual	void				Load()									{}				// 載入參數
	virtual void				ReloadFrames()							{}				// 界面重新被載入後觸發
	virtual	void				BuildFrames()							{}				// UI ROOT 已建立,但未載入XML之前
	virtual void				DestroyFrames()							{}				// UI ROOT 將被刪除
	virtual void				ChangeZone()							{}				// 當玩家改變區域,清除對GAMESERVER鎖定狀態
	virtual void				SaveClientData()						{}				// 要求 Server 儲存資料
	virtual void				LoadClientData(int type, int size, char* data)	{}		// 取得由 Server 讀取資料	
	
	//virtual void				LeaveFrames()							{}				// 當離開遊戲世界時
	virtual bool				IsUpdate();
	virtual void				SetSaveUpdateTime(float time=DELAY_SAVE_TIME);								// 儲存後需將值設定為零
	virtual float				GetSaveUpdateTime()						{ return m_saveUpdateTime; }		// 剩於儲存時間	

protected:

	virtual	bool				GetGlobalPath(const char* name, char* outPath, int outSize);	
	virtual	bool				GetLocalPath(const char* name, char* outPath, int outSize);

	virtual	wstring				GetGlobalPathW(const wchar_t* filename);
	virtual	wstring				GetLocalPathW(const wchar_t* filename);

protected:
	CInterface*					m_interface;			
	float						m_saveUpdateTime;						// 最後要求儲存時間
};

class CInterface
{
public:
	CInterface(const char* resourcePath=NULL, HWND hWnd=NULL);	
	~CInterface();

	enum {
		eInterface_Process_Login		= 1,
		eInterface_Process_World,
	};	

	void				Initialize(const char* location, const char* dataLanguage, const char* country, const char* saveFolder=NULL);
	
	void				BuildFramesLogin(float width, float height);
	void				BuildFramesWorld(float width, float height);	
	void				LeaveFramesWorld();
	bool				WndProc(UINT iMsg, WPARAM wParam, LPARAM lParam);
	void				Update(float elapsedTime, InputState &state);
	void				Render();
	void				PlayerChangeZone();
	void				SetScreenSize(float x, float y);
	void				SetScale(float scale);
	float				GetScale()								{ return m_scale; }
	void				SetResourcePath(const char* resourcePath);
	const char*			GetResourcePath()						{ return m_resourcePath.c_str(); }
	void				SetVisible(bool isVisible)				{ m_isVisible = isVisible; }
	bool				IsVisible()								{ return m_isVisible; }
	void				SetEnable(bool isEnable)				{ m_isEnable = isEnable; }
	bool				IsEnable()								{ return m_isEnable; }
	CUi*				GetUi();
	CUi*				GetUiLogin()							{ return m_uiLogin; }
	CUi*				GetUiWorld()							{ return m_uiWorld; }
	CUiInput*			GetUiInput();
	void				SetProcessType(int type);
	int					GetProcessType()						{ return m_processType; }
	void				SetCapture();
	void				ReleaseCapture();
	void				SetPlayerName(const char* playerName);
	const char*			GetPlayerName()							{ return m_playerName.c_str(); }
	const char*			GetSavePath()							{ return m_savePath.c_str(); }
	lua_State*			GetLuaState()							{ return m_luaState; }

	void				LoadPlayerData();
	void				SavePlayerData();

	void				SaveLuaVariable(const char* variableName, bool perCharacter=false);
	void				SaveVariablesToFile(const char* filename);
	void				LoadVariablesFromFile(const char* filename);
	void				WriteFile(void* data, int size);

	void				ReloadUI();
	void				VariablesLoaded();

	bool				OpenWorldXml();
	bool				CloseWorldXml();
	float				GetXmlLoadingStatus();
	void				LoadXmlFolders(const char* folder);
	void				LoadXmlFile(const char* tocFile, const char* pathname=NULL);
	void				LoadXmlFromMemory(const char* memory, int memorySize, const char*pathname=NULL);
	void				LoadXmlFromFile(const char* filename, const char*pathname=NULL);
	void				AddXmlFile(const char* filename, const char* pathname=NULL);
	void				LoadLuaFile(const char* filename);

	void				SendWorldEvent(const char* event);
	void				SendLoginEvent(const char* event);

	float				GetDelayReloadUiTime()						{ return m_loadUIDelayTime; }

	void				ResetUI();
	void				SetLocation( const char* location, const char* dataLanguage );
	const char*			GetLocation()								{ return m_location.c_str(); }
	bool				IsLoadVariables()							{ return m_isLoadVariables; }
	bool				IsReloadUI()								{ return m_isReloadUI; }
	bool				IsUpdate()									{ return m_isUpdate; }
	bool				IsLoadingUI();

	wstring				GetGlobalPathW(const wchar_t* filename);
	wstring				GetLocalPathW(const wchar_t* filename);

	void				PushClientData(int keyID, int size, char* data);
	void				ClearClientData();
	void				LoadClientData();
	void				SaveClientData();

	void				PushUIComponent(CUiLayout* layout);
	void				UpdateUIComponent();
	void				ClearUIComponent()							{ m_createComponent.clear(); }	

protected:
	bool				IsTocFile(const char* filename);
	void				LoadFiles(CUi* ui, const char* filename);
	void				LoadFolders(CUi* ui, const char* foldername);
	void				CreateUiLogin(float width, float height);
	void				CreateUiWorld(float width, float height);
	void				DestroyUiWorld();

protected:	
	HWND						m_hWnd;
	lua_State*					m_luaState;
	CUi*						m_uiLogin;
	CUi*						m_uiWorld;
	string						m_resourcePath;
	string						m_playerName;
	string						m_savePath;
	string						m_location;
	string						m_dataLanguage;
	string						m_country;
	int							m_processType;	
	bool						m_isVisible;
	bool						m_isEnable;
	bool						m_isReloadUI;
	float						m_loadUIDelayTime;
	float						m_width;
	float						m_height;
	float						m_scale;

	FILE*						m_stream;
	set<string>					m_saveVariables;
	set<string>					m_saveVariablesPerCharacter;
	vector<CInterfaceBase*>		m_loginFrames;
	vector<CInterfaceBase*>		m_worldFrames;

	vector<string>				m_xmlFiles;
	int							m_loadingPosition;
	bool						m_isOpenWorldXml;

	vector<string>				m_addonsList;
	bool						m_isLoadVariables;					// 資料載入結束
	bool						m_isUpdate;
	vector<CUiLayout*>			m_createComponent;

	map<int, CClientSaveData>	m_clientSaveData;					// 客戶端儲存資料
};