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

#define MONEY_MODE_COPPER		"copper"							// �C����
#define MONEY_MODE_ACCOUNT		"account"							// �b����(�ӫ~�f��)
#define MONEY_MODE_BONUS		"bonus"								// ���Q��
#define MONEY_MODE_BILLDIN		"billdin"							//�����B

#define DELAY_SAVE_TIME			30.0f								// �����x�s�ɶ�

// �x�s��� ID ����
enum eSaveClientDataType
{
	eSaveClientData_Header				= 0,						// ���Y
	eSaveClientData_Setting				,							// �]�w���
	eSaveClientData_Action				,							// �ֱ�
	eSaveClientData_Macro				,							// ����
	eSaveClientData_Chat				,							// ���
	eSaveClientData_Channel				,							// �W�D
	eSaveClientData_QuestTrack			,							// ���Ȱl��
	eSaveClientData_MSNInfo				,							// MSN��m
	eSaveClientData_ActionBarSetting	,							// �ֱ��C�]�w
	eSaveClientData_ItemMallHistory		,							// ���v�ʪ�
	eSaveClientData_KeyBinding			,							// ����]�m
	//�Цb�U���[�s���C�| ���n����
	eSaveClientData_SkillPlate			,							//�M�˧ޯ�ޯ�L��
	eSaveClientData_EquipSlotTab		,							//�ֳt���˪�
	eSaveClientData_GameBar				,							//��L

	eSaveClientData_Last				,							// �̫�@��
};

enum TempFilePosition
{
	eTempFilePosition_Begin		= 0,			// �}�l
	eTempFilePosition_Current	= 1,			// �ثe��m
	eTempFilePosition_End		= 2,			// ����
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
	virtual	void				Load()									{}				// ���J�Ѽ�
	virtual void				ReloadFrames()							{}				// �ɭ����s�Q���J��Ĳ�o
	virtual	void				BuildFrames()							{}				// UI ROOT �w�إ�,�������JXML���e
	virtual void				DestroyFrames()							{}				// UI ROOT �N�Q�R��
	virtual void				ChangeZone()							{}				// ���a���ܰϰ�,�M����GAMESERVER��w���A
	virtual void				SaveClientData()						{}				// �n�D Server �x�s���
	virtual void				LoadClientData(int type, int size, char* data)	{}		// ���o�� Server Ū�����	
	
	//virtual void				LeaveFrames()							{}				// �����}�C���@�ɮ�
	virtual bool				IsUpdate();
	virtual void				SetSaveUpdateTime(float time=DELAY_SAVE_TIME);								// �x�s��ݱN�ȳ]�w���s
	virtual float				GetSaveUpdateTime()						{ return m_saveUpdateTime; }		// �ѩ��x�s�ɶ�	

protected:

	virtual	bool				GetGlobalPath(const char* name, char* outPath, int outSize);	
	virtual	bool				GetLocalPath(const char* name, char* outPath, int outSize);

	virtual	wstring				GetGlobalPathW(const wchar_t* filename);
	virtual	wstring				GetLocalPathW(const wchar_t* filename);

protected:
	CInterface*					m_interface;			
	float						m_saveUpdateTime;						// �̫�n�D�x�s�ɶ�
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
	bool						m_isLoadVariables;					// ��Ƹ��J����
	bool						m_isUpdate;
	vector<CUiLayout*>			m_createComponent;

	map<int, CClientSaveData>	m_clientSaveData;					// �Ȥ���x�s���
};