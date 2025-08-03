#ifndef _RUNEDEV_H
#define _RUNEDEV_H

#define _WIN32_WINNT 0x0500
#define _WIN32_WINDOWS 0x0400

#define WIN32_LEAN_AND_MEAN

#define SAFE_DELETE(p)			if (p) { delete p; p=NULL; }
#define DEFAULT_FONT_FILENAME	"fonts\\DFHEIMDU.TTF"	// 內定使用字型

#include <windows.h>
#include <tchar.h>
//#include <mmsystem.h>
#include <map>
#include <list>

#include "Rune Engine/Rune/Rune.h"						// 3D引擎
#include "Rune Engine/Rune/Rune Engine Audio.h"
#include "Rune Engine/Rune/Rune Engine D3D.h"
#include "Rune Engine/Rune/Script/RuScript_LuaIntegration.h"
#include "Rune Engine/Rune/Renderer/Plugin/RuRendererPlugin_SubmergeFX.h"
#include "Rune Engine/DirectInput/directinput.h"		// 輸入裝置
#include "../ErrorList/errorlist.h"						// 錯誤輸出
#include "../sprite/sprite.h"							// 角色,特效
#include "../interface/interface.h"						// 界面

// GKy 091214 for Vivox Settings

#define CONFIG_SETTINGS_VOICE_CHAT "voice chat"
#define CONFIG_SETTINGS_VOICE_CHAT_ENABLE "enable"
#define CONFIG_SETTINGS_VOICE_CHAT_INPUT_DEV_IDX "capture device index"
#define CONFIG_SETTINGS_VOICE_CHAT_INPUT_VOL "mic vol"
#define CONFIG_SETTINGS_VOICE_CHAT_INPUT_SEN "mic sen"
#define CONFIG_SETTINGS_VOICE_CHAT_INPUT_MUTE "mic mute"
#define CONFIG_SETTINGS_VOICE_CHAT_INPUT_MODE "mic mode"
#define CONFIG_SETTINGS_VOICE_CHAT_OUTPUT_DEV_IDX "render device index"
#define CONFIG_SETTINGS_VOICE_CHAT_OUTPUT_VOL "speaker vol"

#define ENGINE_SETTINGS_VOICE_CHAT_ENABLE "vivox:enable"
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_DEV_IDX "vivox:capture device index"
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_VOL "vivox:mic vol"
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_SEN "vivox:mic sen"
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_MUTE "vivox:mic mute"
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_MODE "vivox:mic mode"
#define ENGINE_SETTINGS_VOICE_CHAT_OUTPUT_DEV_IDX "vivox:render device index"
#define ENGINE_SETTINGS_VOICE_CHAT_OUTPUT_VOL "vivox:speaker vol"

#define ENGINE_SETTINGS_VOICE_CHAT_ENABLE_DEF FALSE
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_DEV_IDX_DEF 1
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_VOL_DEF 30 // 0~30~100 <> ~40~50~75~
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_SEN_DEF 25
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_MUTE_DEF FALSE
#define ENGINE_SETTINGS_VOICE_CHAT_INPUT_MODE_DEF 1
#define ENGINE_SETTINGS_VOICE_CHAT_OUTPUT_DEV_IDX_DEF 1
#define ENGINE_SETTINGS_VOICE_CHAT_OUTPUT_VOL_DEF 30 //



//#pragma comment (lib, "winmm")

// **********************************************************************************************
// Globals
class	CRuneDev;

extern	CRuneDev							*g_pRuneDev;

extern	BOOL								g_enableProfilerOutput;

// **********************************************************************************************

using namespace std;

class CRuneDev
{
public:
	CRuneDev();
	virtual ~CRuneDev();

	// System settings
	bool					SaveSystemVariables();
	bool					LoadSystemVariables();

#ifdef KALYDO
	void					SetIsWindowed(bool windowed) { m_isWindowed = windowed; }
#endif // KALYDO

	// for Kalydo: added window parameter so we can render in browser.
	bool					RuneDevInitialize( HINSTANCE hInstance, int screenWidth, int screenHeight, bool isWindowed, HWND window = NULL );
	bool					RuneDevInitializeDefaultSettings();
	void					RuneDevRelease();

	bool					SetScreenSize( DWORD width, DWORD height, bool isWindowed, bool forceWindowSizeConform);

	void					SetResourcePath( const char* pathStr );
	void					UpdateSubSystemResourcePaths();
	const char*				GetFullFilePath( const char* fileStr );
	int						Run();
	float					GetFramerate()								{ return m_fps; }

	// Fog
	void					SetFogDistance( float fmin, float fmax );

	// Glow
	void					SetGlow( bool isEnable )					{ m_isGlow = isEnable; }
	bool					IsGlow()									{ return m_isGlow; }

	// Camera
	void					DefaultCamera();
	CRuCamera*				GetCamera()									{ return m_pCamera; }
	CRuController_Camera*	GetCameraController()						{ return m_pRuCameraController; }
	float					GetCameraNearPlane()						{ return m_cameraNearPlane; }
	float					GetCameraFarPlane()							{ return m_cameraFarPlane; }
	float					GetCameraFOV()								{ return m_cameraFOV; }	
	float					GetCameraMoveTime()							{ return m_cameraMoveTime; }
	void					SetCameraFarPlane( float farPlane );
	void					SetCameraMoveTo(CRuVector3 targetPosition, float moveTime);
	void					SetCameraPos(CRuVector3& pos);
	void					SetCameraTargetPos(CRuVector3& pos);
	void					SetCameraUpVector(CRuVector3& upVector);
	void					MoveCameraPos( CRuVector3& pos );	
	void					ZoomCamera( REAL zoomDelta );
	void					SetCameraZoomLocked( bool locked )			{ m_cameraZoomLocked = locked; }
	bool					GetCameraZoomLocked()						{ return m_cameraZoomLocked; }
	void					SetCameraControllerEnable(bool enable)		{ m_updateCameraController = enable; }
	bool					GetCameraControllerEnable()					{ return m_updateCameraController; }

	// 游標
	void					SetCursorIcon( const char* fileName );
	void					SetDragCursorIcon( const char* fileName );
	void					ClearCursorIcon();
	void					ClearDragCursorIcon();
	void					RenderCursor();
	void					SetCursorPosition(int x, int y);
	void					SetCursorVisible( bool isVisible )			{ m_isCursorVisible = isVisible; }
	bool					IsCursorVisible()							{ return m_isCursorVisible; }
	CTextureImage*			GetCursorImage()							{ return m_pCursorIcon; }
	CTextureImage*			GetDragCursorImage()						{ return m_pDragCursorIcon; }
	int						GetCursorPtX()								{ return m_xCursor; }
	int						GetCursorPtY()								{ return m_yCursor; }

	// 
	void					SetShowFps( bool isShow )					{ m_isShowFps = isShow; }

	// 地表
	void					ReleaseTerrain();
	void					ReleaseOldTerrain();
	bool					LoadTerrain( const char* resourceName );
	void					UpdateTerrain();
	void					RenderTerrain();
	bool					ChangeScreenPoint(int x, int y, CRuVector3& pos);
	void					SetTerrainDetailLevel(int level);
	bool					IsLoadingTerrain()							{ return m_loadingTerrain; }
	float					GetTarrainLoadingStatus()					{ return m_pRuTerrain->GetLoadingStatus(); }
	CRuOptimalTerrain*		GetRuTerrain()								{ return m_pRuTerrain; }

	// Other status
	RuWorld::ZoneType		GetActiveZoneType()							{ return m_activeZoneType; }

	// 取得地表
	CRuFusion_Minimap*		GetFusionMinimap()							{ return m_pRuMinimap; }

	// 環境
	void					ReleaseEnvironment();
	void					InitialEnvironment();
	void					UpdateEnvironment( CRuVector3& pos, bool forceUpdate = false );
	void					RenderEnvironment();
	void					SetEnvironmentEnable()						{ m_isEnvironmentUpdate = true; }
	void					SetEnvironmentDisable()						{ m_isEnvironmentUpdate = false; }
	void					SetEnvironmentShow()						{ m_isEnvironmentRender = true; }
	void					SetEnvironmentHide()						{ m_isEnvironmentRender = false; }

	// 界面
	void					UpdateInterface();
	CInterface*				GetInterface()								{ return m_interface; }	
	void					SetMiniMapMaskFile( const char* cszFile )	{ m_minimapMaskFile = cszFile; }
	bool					SetMiniMapMask();

	// 角色
	void					SetSpriteVisible( bool isVisible )			{ m_isSprite_Render = isVisible; }
	bool					IsSpriteVisible()							{ return m_isSprite_Render; }
	void					SetSpriteEnable( bool isEnable )			{ m_isSprite_Update = isEnable; }
	bool					IsSpriteEnable()							{ return m_isSprite_Update; }

	CRoleSprite*			CreateSprite(int ID, int DBID, int worldGUID, bool &outNew);
	CRoleSprite*			FindSpriteByID(int ID);	 //GItemID
	CRoleSprite*			FindSpriteByDBID(int DBID);
	CRoleSprite*			FindSpriteByWorldGUID(int worldGUID);
	virtual	bool			DeleteSprite(int worldGUID);	
	virtual	void			UpdateSprite();
	virtual	void			RenderSprite();	
	virtual	void			ForceDeleteSprite(CRoleSprite* sprite);
	virtual	void			ProtectSprite(CRoleSprite* sprite);	
	virtual	void			SpriteMsgProc(IBaseSprite* sprite, SpriteMessageType msg, INT64 arg1, INT64 arg2);

	virtual	CRoleSprite*	GetTopRoleSprite(int x, int y, CRuArrayList<CRoleSprite*>& ignoreList);
	virtual	void			SetHighlightSprite(CRoleSprite* sprite);
	virtual	void			ResetRoleSpriteWidget();
	virtual void			SendCreateRoleSprite( CRoleSprite* pRoleSprite ) {}
	virtual void			SendDeleteRoleSprite( CRoleSprite* pRoleSprite ) {}
	virtual void			SendPlayerGItemIDChange( CRoleSprite* pRoleSprite, int newGItemID ) {}
	virtual	map<int, CRoleSprite*>&	GetSpriteMap()						{ return m_spriteContainer; }		

	// Sprite picking and editing
	void					SprEdit_Begin();
	void					SprEdit_End();
	BOOL					SprEdit_PickAtCursor(INT32 x, INT32 y, IBaseSprite **spriteOut, REAL *distanceOut);
	BOOL					SprEdit_BeginManipulation(IBaseSprite *sprite);
	BOOL					SprEdit_EndManipulation(IBaseSprite *sprite);
	void					SprEdit_RepositionToCursor(IBaseSprite *sprite, INT32 x, INT32 y);
	void					SprEdit_SnapToNormalAtCursor(IBaseSprite *sprite, INT32 x, INT32 y);

	// Flag picking and editing
	void					FlagEditor_ClearAll();
	INT32					FlagEditor_CreateFlag(const CRuVector3 &position, REAL yRotation, INT32 alternateIndex = 0xFFFFFFFF);
	BOOL					FlagEditor_GetFlagAtCursor(INT32 x, INT32 y, INT32 *flagIndexOut, CRuEntity **flagOut, REAL *distanceOut);
	CRuEntity*				FlagEditor_GetFlagAtIndex(INT32 index);
	BOOL					FlagEditor_RepositionFlagToCursor(CRuEntity *flag, INT32 x, INT32 y, CRuVector3* outPosition=NULL);
	void					FlagEditor_RenderFlags();
	void					FlagEditor_UpdateFlags();

	// Paperdoll cache management
	void					PaperdollCache_ActivateWrites(BOOL activateWrites);

	// System
	void					Exit()										{ m_isExit = TRUE; }
	HWND					GetWnd()									{ return m_hFrameWnd; }
	bool					IsWindowed()								{ return m_isWindowed; }
	SIZE					GetResolution(bool windowed)				{ return windowed ? m_windowedResolution : m_fullscreenResolution; }
	float					GetElapsedTime()							{ return m_elapsedTime; }
	double					GetFrameTime()								{ return m_frameTime; }
	TCHAR*					GetResourceRootDir()						{ return m_resourceRootDirectoryStr; }	
	RECT					GetBoundsRect()								{ return m_clientRect; }
	int						GetScreenWidth()							{ return m_xScreen; }
	int						GetScreenHeight()							{ return m_yScreen; }
	float					GetUIWidth()								{ return m_UIWidth; }
	float					GetUIHeight()								{ return m_UIHeight; }

	void					LockCursorPos()								{ m_isCursorLock = true; }
	void					UnlockCursorPos()							{ m_isCursorLock = false; }
	void					SetStableFrame(bool isStable)				{ m_isStableFrame = isStable; }
	bool					GetStableFrame()							{ return m_isStableFrame; }
	
	IRuSwapChain*			GetRuSwapChain()							{ return m_pRuSwapChain; }	
	CRuPerformanceTimer*	GetRuTimer()								{ return m_pRuTimer; }

	void					SetPlayerAnchor( CRuVector3& pos )			{ m_playerPos = pos; }

	void					CreateWidgetGrid(CRuVector3 pos, bool isLock, float halfSize, float spacing);
	void					ReleaseWidgetGrid();	

	void					DisplaySelectAreaEntity(float distance, float scale=1.0f);
	CRuEntity*				GetSelectAreaEntity()						{ return m_pRuSelectAreaEntity; }
	void					ClearSelectAreaEntity();

	void					DisplayMoveTargetEntity(CRuVector3& position);
	void					ClearMoveTargetEntity();

	// 取得路徑
	int						FindPath(CRuVector3 origin, CRuVector3 destination);

	// 用X Z 跟一參考Y 取得地表高度 路徑搜索用
	bool					FindHeight( CRuVector3& position );

	// Special Screen effect
	void					SetBlindEffect(bool blind)					{ m_isBlind = blind; }
	bool					GetBlindEffect()							{ return m_isBlind; }

	// Special Event
	void					PushSpecialEvent(const char* festival);
	void					RemoveSpecialEvent(const char* festival);
	void					ClearSpecialEvent();
	void					UpdateSpecialEvent();

	virtual void			Init()										{}
	virtual void			Release()									{}
	virtual void			LoadData()									{}
	virtual void			Update();
	virtual LRESULT			WndProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam );

	// Font
	virtual string			GetDefaultFontPath();

	// 自動調降貼圖
	virtual void			SetDebaseTexture(bool debaseTexture)		{ m_debaseTexture = debaseTexture; }
	virtual BOOL			IsDebaseTexture()							{ return m_debaseTexture; }

	virtual void			SetDisableMemoryWarning(bool disable)		{ m_disableMemoryWarning = disable; }
	virtual BOOL			IsDisableMemoryWarning()					{ return m_disableMemoryWarning; }	

	// 格線測試用
	CRuEntity_Widget_Grid*	m_pRuWidgetGrid;
	CRuVector3				m_gridPos;
	bool					m_isGridLock;
	float					m_gridHalfSize;
	float					m_gridSpacing;

	virtual void			SetLoginWdbFileName( const char* szName ){ m_strLoginWdbFileName = szName; }
	virtual const char*		GetLoginWdbFileName(){ return m_strLoginWdbFileName.c_str(); }

	virtual void			CreateTargetEntity();
	virtual void			RelaseTargetEntity();
	virtual void			SetTargetEntityTarget(CRuEntity* parentEntity, const char* point);
	virtual CRuEntity*		GetTargetEntity()							{ return m_targetEntity; }
	virtual CRuEntity*		GetTargetPolyLineEntity()					{ return m_targetPolyLineEntity; }
	virtual void			SetTargetEntityVisible(bool visible);
protected:	
	bool					CreateWnd( HINSTANCE hInstance, int screenWidth, int screenHeight, HWND parent = NULL );
	bool					AdjustWindowForChange();	
#ifdef KALYDO
public:
	// For Kalydo, we need to know whether the game has been activated.
	bool					GetIsActived() const { return m_isActived; }
	bool					GetIsExit() const { return m_isExit; }
	// For Kalydo, we need to be able to do single loops so the Kalydo Player can have control when needed,
	// so this function needs to be public.
#endif // KALYDO
	bool					FrameLoop();

protected:
	
	HINSTANCE				m_hInstance;
	HWND					m_hFrameWnd;
	bool					m_isWindowed;
	SIZE					m_windowedResolution;
	SIZE					m_fullscreenResolution;
	bool					m_isExit;
	bool					m_isActived;
	bool					m_isStableFrame;
	DWORD					m_windowStyle;
	RECT					m_clientRect;
	RECT					m_windowRect;
	RECT					m_restoredWindowRect;
	DWORD					m_xScreen;
	DWORD					m_yScreen;	
	double					m_frameTime;
	DWORD					m_frameTimeMS;
	float					m_elapsedTime;
	float					m_fps;	
	TCHAR					m_windowName[MAX_PATH];
	TCHAR					m_resourceRootDirectoryStr[MAX_PATH];
	TCHAR					m_specialEventFilter[MAX_PATH];
	vector<string>			m_specialEvent;
	bool					m_isHardwaveCursor;
	int						m_xCursor, m_yCursor;
	bool					m_isCursorVisible;
	int						m_isCursorLock;
	bool					m_isRuneRelease;
	bool					m_isAdjustWindow;
	DWORD					m_forceMainLightColor;				// 設定場景主光源

	lua_State*				m_pRuVM;
	CRuFusion_AudioLibrary*	m_pRuAudioLibrary;
	IRuSwapChain*			m_pRuSwapChain;
	INT32					m_failedEngineResetCount;
	INT32					m_failedPresentCount;
	CRuPerformanceTimer*	m_pRuTimer;	
	BOOL					m_isFirstPersonCamera;
	CRuEntity_Environment*	m_pRuWorldEnvironment;
	RuWorld::ZoneType		m_activeZoneType;
	REAL					m_timeSinceLastWorldEnvironmentUpdate;
	INT32					m_timeGroup;
	CRuOptimalTerrain*		m_pRuTerrain;
	CRuOptimalTerrain*		m_pOldRuTerrain;
	string					m_terrainFileName;
	string					m_oldTerrainFileName;
	double					m_lastLoadTerrainTime;
	double					m_FpsLogTime;					//最後一次紀錄FPS的時間
	double					m_SendPerformanceTime;					//最後一次發送Performance的時間
	CRuFusion_Minimap*		m_pRuMinimap;
	CRuController_Camera*	m_pRuCameraController;	
	CInputCOM*				m_pInputCOM;
	CInputDevice*			m_pKeyboard;
	CInputDevice*			m_pMouse;

	CRuPathFinder_AStar*		m_pRuPathFinder;
	IRuPathHierarchy_Base*		m_pRuPathHierarchy;
	CRuArrayList<CRuVector3>	m_pathPoints;				// 路徑點

	CRuCamera*				m_pCamera;
	CRuVector3				m_cameraMoveVector;
	float					m_cameraMoveTime;
	float					m_cameraDistance;
	float					m_cameraYPosition;

	CTextureImage*			m_pCursorIcon;
	CTextureImage*			m_pDragCursorIcon;

	IRuTexture*				m_pCursorIcon_HW;
	INT32					m_pCursorIcon_HW_Iteration;
	IRuTexture*				m_pDragCursorIcon_HW;
	INT32					m_pDragCursorIcon_HW_Iteration;

	IRuTexture*				m_pCursor_HW;

	CRuFontType*			m_pSysFontType;
	CRuFontType*			m_pSysFontType_Small;

	// 界面
	CInterface*				m_interface;
	float					m_UIWidth;
	float					m_UIHeight;

	// 角色
	CRuVector3				m_playerPos;
	CRuController_Highlighter*	m_pRuHighlighter;
	map<int, CRoleSprite*>	m_spriteContainer;
	map<int, int>			m_DBIDMap;
	map<int, int>			m_worldGUIDMap;

	// 攝影機控制	
	bool					m_updateCameraController;		// 更新攝影機位置
	float					m_cameraNearPlane;
	float					m_cameraFarPlane;
	float					m_cameraFOV;
	float					m_cameraBackdropNearPlane;		// 鏡頭背景近距
	float					m_cameraBackdropFarPlane;		// 鏡頭背景遠距
	float					m_cameraTempFarPlane;			// 暫時以此值為遠距
	bool					m_cameraZoomLocked;				// 鎖定鏡頭遠距

	bool					m_isSprite_Render;
	bool					m_isSprite_Update;
	bool					m_isShowFps;
	bool					m_isGlow;
	bool					m_isEnvironmentUpdate;
	bool					m_isEnvironmentRender;

	float					m_fogMin;
	float					m_fogMax;

	// Memory tracking
	INT32					m_memoryBookmark;
	bool					m_loadingTerrain;

	// MinimapMaskfile
	string					m_minimapMaskFile;

	// Editing stuff
	bool					m_editingModeOn;
	bool					m_editingDragOn;
	IBaseSprite*			m_editingSprite;

	// Flag editor
	CRuArrayList<CRuEntity *>	m_flagEditor_Flags;
	bool						m_flagEditor_DragOn;
	int							m_flagEditor_EditFlagIdx;
	CRuEntity*					m_flagEditor_EditFlag;

	// Paperdoll cache
	CRuPaperdollCache*			m_paperdollCache;
	BOOL						m_paperdollCache_ActivateWrites;
	CRuArrayList<CRoleSprite *>	m_paperdollCache_UnprocessedSprites;

	// Global actor collision state flags
	INT32					m_forceColDataResetCycles;			// Number of cycles remaining to force reset of actor collision datafi

	// 滑鼠地表圈選特效
	CRuEntity*				m_pRuSelectAreaEntity;
	CRuEntity*				m_pRuSelectAreaStopEntity;
	CRuEntity*				m_pRuMoveTargetEntity;
	float					m_selectAreaDistance;

	// special screen effect
	bool					m_isBlind;

	CRuEntity*				m_uifx_ModelEntity;
	CRuFX2D_Model*			m_uifx_Model;	

	CRoleSprite*				m_pPlayer;

	int						m_deleteRoleSpriteCount;	// 記錄刪除 RoleSprite 物件個數	
	int						m_lastFrameRoleSpriteCount;	// 上個張數 RoleSprite 物件個數

	string                  m_strLoginWdbFileName;	
	BOOL					m_debaseTexture;				// 降低貼圖顯示
	BOOL					m_disableMemoryWarning;			// 不顯示記憶體使用過量警告

	BOOL					m_targetEntityVisible;			// 隱藏
	CRuEntity*				m_targetEntity;					// 目的物件
	CRuEntity*				m_targetPolyLineEntity;			// 目的物件串接特效
	CRuEntity*				m_targetParentEntity;
	string					m_targetAttachPoint;
};

#endif //_RUNEDEV_H

