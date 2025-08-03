#ifndef _GAMEMAIN_H
#define _GAMEMAIN_H

#include <vector>
#include <set>
#include <string>

#include "RuneDev.h"
#include "GameMainDef.h"
#include "ACTActionEventsManager.h"
#include "ReadUpdateIni.h"
#include "..\tools\CpuUsage\CpuUsage.h"
#ifdef KALYDO
// Add the Kalydo ROM application class so we can use the download defines.
#include "../../Kalydo/SocialIntegration/TRunesOfMagicApplication.h"
#endif // KALYDO
#define REPLACE_PLAYER_NUM					50
#define REPLACE_MAX_PLAYER_NUM				250
#define REPLACE_MEMORY_SIZE					1.35 * 1024
#define REDUCE_TEXTURE_DETAIL_SIZE			0.9 * 1024

//�i�H�y��Ȥl���Z��
#define DF_SIT_TO_CHAIR_MAX_DISTANCE        20

// �y�t�r��
#define LANGUAGE_CODE_RW					"RW"
#define LANGUAGE_CODE_TW					"TW"
#define LANGUAGE_CODE_CN					"CN"
#define LANGUAGE_CODE_DE					"DE"
#define LANGUAGE_CODE_JP					"JP"
#define LANGUAGE_CODE_KR					"KR"
#define LANGUAGE_CODE_ENUS					"ENUS"
#define LANGUAGE_CODE_ENEU					"ENEU"
#define LANGUAGE_CODE_SG					"SG"
#define LANGUAGE_CODE_BR					"BR"
#define LANGUAGE_CODE_TR					"TR"
#define LANGUAGE_CODE_RU					"RU"
#define LANGUAGE_CODE_FR					"FR"
#define LANGUAGE_CODE_PT					"PT"
#define LANGUAGE_CODE_PL					"PL"
#define LANGUAGE_CODE_VN					"VN"
#define LANGUAGE_CODE_ES					"ES"				// ��Z��
#define LANGUAGE_CODE_ID					"ID"				// �L��
#define LANGUAGE_CODE_NL					"NL"				// ����
#define LANGUAGE_CODE_RO					"RO"				// ù������
#define LANGUAGE_CODE_PH					"PH"				// ��߻�
#define LANGUAGE_CODE_PL					"PL"				// �i��
#define LANGUAGE_CODE_TH					"TH"				// ����
#define LANGUAGE_CODE_SAES					"SAES"				// ���n���w��Z����
#define LANGUAGE_CODE_SAPT					"SAPT"				// ���n���w�帲����
#define LANGUAGE_CODE_AE					"AE"				// ���ԧB�p�X�j����
#define LANGUAGE_CODE_ENAR					"ENAR"				// ���ԧB�p�X�j����(�^��)
#define LANGUAGE_CODE_AUS					"AUS"				// �D�{
#define LANGUAGE_CODE_IT					"IT"				// �q�j�Q
#define MAX_LANGUAGES						29

#define MAX_FOCUS_SPRITES					12
#define TOMB_MAP_ICON_ID					0x7f000000

#define LUA_PASSWORD_FUNC					"__uiLuaPassword_Callback__"
#define LUA_CHECKPLAYER_FUNC				"__uiLuaCheckPlayer_Callback__"

#define IS_DETECT_MEMORY_LEAKS_ENABLE 1 // �O�_�����O���鬪�|.

// **********************************************************************************************
// Globals
class	CGameMain;

extern	CGameMain							*g_pGameMain;
extern	char								*g_languageCode[MAX_LANGUAGES];

// **********************************************************************************************

using namespace std;

/*
typedef struct 
{
	union {
		int value;
		struct {
			unsigned char background;
			unsigned char foreground;
			unsigned char transient;
			unsigned char auxiliary;
		};
	}actor;
	union {
		int value;
		struct {
			bool	isUnholster				:1;		// true ���X�Z��
			bool	isWalk					:1;		// true ����
			bool	isCombat				:1;		// true �԰���
			bool	isDead					:1;		// true ���`��
			bool	isSit					:1;		// true ���U
		};
	}state;
}PLAYER_ACTOR_STATE;
*/

struct PetCommandStruct
{
	union {
		int _value;
		struct {
			bool		movable : 1;				// ����(���H���a)
			bool		strikeBack : 1;				// ����
		};
	};
};

// �d���򥻸��
struct PetInfo
{
	int						GUID;					// 
	int						ItemID;					//
	int						MagicID;				//
	PetCommandStruct		Status;					// �d�����A
};

typedef void (*pfSpellRangeCallback) (int spellID, float x, float y, float z);

enum CASTSPELLERROR
{
	CASTSPELL_ERROR_EXIST				= -1,			// ���s�b
	CASTSPELL_ERROR_ACTIVE				= -2,			// �Q�ʧޯण��I�k
	CASTSPELL_ERROR_CLASS				= -3,			// ¾�~�~��,�D��¾�~
	CASTSPELL_ERROR_HP					= -4,			// �ͩR�Ȥ���
	CASTSPELL_ERROR_MP					= -5,			// �]�O�Ȥ���
	CASTSPELL_ERROR_SP_WARRIOR			= -6,			// warrior sp ����
	CASTSPELL_ERROR_SP_RANGER			= -7,			// Ranger sp ����
	CASTSPELL_ERROR_SP_ROGUE			= -8,			// Rogue sp ����
	CASTSPELL_ERROR_GUN					= -9,			// �ݨD�j
	CASTSPELL_ERROR_AMMO				= -10,			// �u�Ĥ���
	CASTSPELL_ERROR_BOW					= -11,			// �ݨD�}
	CASTSPELL_ERROR_ARROW				= -12,			// �b�ڤ���
	CASTSPELL_ERROR_ITEM				= -13,			// ���~�ݨD����
	CASTSPELL_ERROR_COLDOWN				= -14,			// �N�j�ɶ�
	CASTSPELL_ERROR_TARGET				= -15,			// �ݭn�@�ӥؼ�
	CASTSPELL_ERROR_DITANCE				= -16,			// �Z���L��
	CASTSPELL_ERROR_MOVE				= -17,			// ���ಾ�ʬI�k
	CASTSPELL_ERROR_DISABLEJOBSKILL		= -18,			// �L�k�ϥ�¾�~�ޯ�
	CASTSPELL_ERROR_LEVEL				= -19,			// ���Ť���
	CASTSPELL_ERROR_CANTCTRL			= -20,			// �L�k���� (�߱���

	//�ݨD
	CASTSPELL_ERROR_NEED_WEAPON			= -100,			//�ݭn�Y�Z��
	CASTSPELL_ERROR_NEED_EQ				= -101,			//�ݭn�Y�˳�
	CASTSPELL_ERROR_NEED_SUIT			= -102,			//�ݭn�Y�M��
	CASTSPELL_ERROR_NEED_BUFF			= -103,			//�ݭn�YBuff	
	CASTSPELL_ERROR_NEED_NOBUFF			= -104,			//���঳�YBuff
	CASTSPELL_ERROR_NEED_UNARMED		= -105,			//�ݭn�Ť�
	CASTSPELL_ERROR_NEED_BLADE			= -106,			//�ݭn�˼C
	CASTSPELL_ERROR_NEED_DAGGER			= -107,			//�ݭn�ˤP��
	CASTSPELL_ERROR_NEED_WAND			= -108,			//�ݭn�ˤ��
	CASTSPELL_ERROR_NEED_AXE			= -109,			//�ݭn�˩��Y
	CASTSPELL_ERROR_NEED_BLUDGEON		= -110,			//�ݭn�˳����
	CASTSPELL_ERROR_NEED_CLAYMORE		= -111,			//�ݭn������C
	CASTSPELL_ERROR_NEED_STAFF			= -112,			//�ݭn�������
	CASTSPELL_ERROR_NEED_2H_AXE			= -113,			//�ݭn�������
	CASTSPELL_ERROR_NEED_2H_HAMMER		= -114,			//�ݭn��������
	CASTSPELL_ERROR_NEED_POLEARM		= -115,			//�ݭn�˪���
	CASTSPELL_ERROR_NEED_SWORD_TYPE		= -116,			//�ݭn�˼C���Z��
	CASTSPELL_ERROR_NEED_AXE_TYPE		= -117,			//�ݭn�˩����Z��
	CASTSPELL_ERROR_NEED_SHIELD			= -118,			//�ݭn�ˬ�
	CASTSPELL_ERROR_NEED_HP_LOWER		= -119,			//�ݭnHP�C�@��
	CASTSPELL_ERROR_NEED_HP_HIGHER		= -120,			//�ݭnHP���@��
	CASTSPELL_ERROR_NEED_JOB			= -121,			//¾�~�����D
	CASTSPELL_ERROR_NEED_LONGWEAPON		= -122,			//�ݭn�˪��Z���Z��
	CASTSPELL_ERROR_NEED_HAMMER_TYPE	= -123,			//�ݭn�������Z��
	CASTSPELL_ERROR_NEED_2H_WEAPON_TYPE	= -124,			//�ݭn������Z��
	CASTSPELL_ERROR_NEED_BUFF_GROUP		= -125,			//�ݭn�Y�@������BUFF

	CASTSPELL_ERROR_NEED_CRITICAL		= -126,			//�P�R�@����~�i�ϥ�
	CASTSPELL_ERROR_NEED_BECRITICAL		= -127,			//�Q�H�P�R�@����~�i�ϥ�
	CASTSPELL_ERROR_NEED_DODGE			= -128,			//�{�׫�~�i�ϥ�
	CASTSPELL_ERROR_NEED_MISS			= -129,			//MISS��~�i�ϥ�
	CASTSPELL_ERROR_NEED_PARRY			= -130,			//�j�ɫ�~�i�ϥ�
	CASTSPELL_ERROR_NEED_BEPARRY		= -131,			//�Q�H�j�ɫ�~�i�ϥ�
	CASTSPELL_ERROR_NEED_BEDODGE		= -132,			//�Q�H�{�׫�~�i�ϥ�

	CASTSPELL_ERROR_NEED_COMBAT			= -133,			//�ݭn�԰���
	CASTSPELL_ERROR_NEED_NOCOMBAT		= -134,			//�D�԰���
	CASTSPELL_ERROR_NEED_DITANCE		= -135,			//�Z���L��L�k�I�k

	CASTSPELL_ERROR_TARGET_NEED_BUFF		= -136,		//��H�ؼлݭn�YBUFF
	CASTSPELL_ERROR_TARGET_NEED_NOBUFF		= -137,		//��H�ؼлݭn�YBUFF
	CASTSPELL_ERROR_TARGET_NEED_NOCOMBAT	= -138,		//��H�ؼлݭn�԰���
	CASTSPELL_ERROR_TARGET_NEED_COMBAT		= -139,		//��H�ؼЫD�԰���	
};

enum ItemCastingType
{
	ITEM_CASTING_NONE					= 0,
	ITEM_CASTING_USEITEM				= 1,			// �ϥέI�]�����~
	ITEM_CASTING_DISSOLUTION			= 2,			// �ϥΤ��ѧޯ�[�S��]
	ITEM_CASTING_PARTNER		        = 3,            //�I�s�ּ֪��٦�

	// �H�U�����i��L����
	ITEM_CASTING_REFINE					= 4,
};

enum eCastSpellState
{
	CASTSPELL_NONE						= 0,
	CASTSPELL_WAITING_LOCATION			= 1,
	CASTSPELL_WAITING_TARGET			= 2,
	CASTSPELL_CASTING					= 3,
	CASTSPELL_WAITING_LOCATION_EXTRA	= 4,
	CASTSPELL_WAITING_TARGET_EXTRA		= 5,
};

enum ItemCommandEnum
{
	ITEM_COMMAND_NONE					= 0,
	ITEM_COMMAND_USE_BAG				= 1,
	ITEM_COMMAND_BAG_TO_BANK			= 2,
};

enum PacketReduceState
{
	PACKET_REDUCE_NONE					= 0,			// �S������(�����ʥ]�I��)
	PACKET_REDUCE_PLAYER_MOVE			= 1,			// �������a���ʫʥ](�٬O���w��ʥ])
	PACKET_REDUCE_PLAYER_MOVE_ALL		= 2,			// �������a�Ҧ����ʫʥ](�]�S���w��ʥ],�ݨ��L���a���ӷ|�O�θ���)
	PACKET_REDUCE_MOVE_ALL				= 3,			// �����Ҧ����ʫʥ](NPC�P���a)
};

enum TargetLineEffectType
{	
	TargetLineEffect_None				= 0,
	TargetLineEffect_Line				= 1,			// �u�q
	TargetLineEffect_Parabola			= 2,			// �ߪ��u
};

struct ItemCommand
{
	ItemCommandEnum type;
	float			startTime;
	union {
		int data[8];

		// ITEM_COMMAND_USE_BAG
		struct {
			int index;
			bool fromBag;
			int objiectID;
			int	createTime;
			int	serial;
		};

		struct {
			int src;
			int dest;
		};
	};
};

typedef union {
	int value;
	struct {
		unsigned char background;
		unsigned char foreground;
		unsigned char transient;
		unsigned char auxiliary;
	};
}PLAYER_ACTOR_STATE;

// �O�dCLIENT�ݳ]�w��T
struct ClientSaveSetting
{
	ClientSaveSetting()
	{
		memset(this, 0, sizeof(ClientSaveSetting));
	}

	void Init()
	{
		isUseServerData				= false;
		isNPCTitleVisible			= true;
		isPlayerTitleVisible		= true;
		isAllSCTVisible				= false;
		isMouseMoveEnable			= true;
		isDisableTitleHide			= false;
		isSelfCast					= true;
		isSelfTitleVisible			= true;
		isLastSelectedRealm			= false;
		isLastAccountName			= false;
		isShowGemePromrt			= true;
		isNPCRealHealth				= true;
		isTitleVisible				= true;
		isGuildVisible				= true;
		isTitleIconVisible			= true;
		isCameraYReverse			= false;
		isLButtonCancelTarget		= true;
		isRBuutonCancelTarget		= false;
		isLButtonCameraRotateEnable	= true;
		isCameraFollowEnable		= false;
		isDisableDisplayNPCTalk		= true;
		isCameraSelectTarget		= false;
		isPlayerLootMotion			= true;
		isAutoTakeLoot				= false;
		isChatDisplayClassColor		= false;
		isAggroPrompt				= false;
		isAutoOpenGoodsPack			= true;
		isHideOtherPlayerEffect		= false;
	}

	union 
	{
		int _value[4];
		struct {
			bool			isUseServerData					:1;			// �O�_�}�Ҧ��A���x�s���
			bool			isNPCTitleVisible				:1;			// ���NPC�����Y���W��
			bool			isPlayerTitleVisible			:1;			// ��ܪ��a�����Y���W��
			bool			isAllSCTVisible					:1;			// ��ܩҦ������ƭ�
			bool			isMouseMoveEnable				:1;			// �O�_�}�ҷƹ�����
			bool            isDisableTitleHide				:1;			// �b�ٸ��˳Ƥ����� ���i�˳ƪ��ٸ��n���n���
			bool			isSelfCast						:1;			// �ҰʦۧڬI�k(��k�N�L�ݭn�ؼЮ�)			
			bool			isSelfTitleVisible				:1;			// ��ܦۤv�Y���W��
			bool			isLastSelectedRealm				:1;			// �ϥγ̫��ܪ����A���n�J
			bool			isLastAccountName				:1;			// �O���̫�n�J�b��
			bool			isShowGemePromrt				:1;			// ��ܹC������
			bool			isNPCRealHealth					:1;			// ��ܫD���a�͵���������q
			bool			isTitleVisible					:1;			// ��ܺٸ�
			bool			isGuildVisible					:1;			// ��ܤ��|
			bool			isTitleIconVisible				:1;			// ��ܨ���¾�~�P���c��ICON
			bool			isCameraYReverse				:1;			// ���YY�b����
			bool			isLButtonCancelTarget			:1;			// �ƹ���������ؼ�
			bool			isRBuutonCancelTarget			:1;			// �ƹ��k������ؼ�
			bool			isLButtonCameraRotateEnable		:1;			// �ƹ��������Y����
			bool			isCameraFollowEnable			:1;			// ���H���Y
			bool			isDisableDisplayNPCTalk			:1;			// �����NPC��ͰT��
			bool			isCameraSelectTarget			:1;			// �ϥ����Y�����I�������
			bool			isPlayerLootMotion				:1;			// ����B���ʧ@
			bool			isAutoTakeLoot					:1;			// �۰ʬB��
			bool			isChatDisplayClassColor			:1;			// ���¾�~����C��
			bool			isAggroPrompt					:1;			// ����ĵ�i����
			bool			isAutoOpenGoodsPack				:1;			// �۰ʶ}�Ұӫ��I�]
			bool			isRequestServerSaveData			:1;			// �߰ݬO�_�Ѧ��A���O�����
			bool			isHideOtherPlayerEffect			:1;			// ���è�L���a�S��
		};
	};
};

struct SystemFunctionState
{
	SystemFunctionState()
	{
		guildBattleGround = 1;
		ch3Login = 1;
		vivoxUserAgreement = 0;
		loginEpisode = 0;
	};

	union {
		int _value;
		struct {
			bool				guildBattleGround			: 1;
			bool				ch3Login					: 1;
			bool				vivoxUserAgreement			: 1;
			char				loginEpisode				: 5;		// �ĴX��(0, 1���N��Ĥ@��)
		};
	};
};

struct EilteBellStruct 
{
	float m_destroyTime;			// �����ɶ�
	int m_GItemID;					// �ϰ�s��
}; 

struct AttachObjectInfo
{
	int sourceGItemID;				// �ӷ� Server GItem
	int targetGItemID;				// �ؼ� Server GItem
	int AttachType;					// �s���Ҧ�
	string sourceLinePoint;			// �ӷ��Ѧ��I
	string targetLinePoint;			// �ؼаѦ��I
};

//-----------------------------------------------------------------------------
// Ĳ�o�ƥ�
typedef void (*ZoneDescriptorCallback)	(CRuWorld_ZoneDescriptor* zoneDescriptor);

class CGameMain : public CRuneDev
{
public:
	CGameMain();
	virtual ~CGameMain();

	virtual void				LoadData();
	virtual void				Init();
	virtual void				Release();
	virtual void				Update();
	virtual void				SendCreateRoleSprite( CRoleSprite* pRoleSprite );
	virtual void				SendDeleteRoleSprite( CRoleSprite* pRoleSprite );
	virtual void				SendPlayerGItemIDChange( CRoleSprite* pRoleSprite, int newGItemID );
	virtual LRESULT				WndProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam );
#ifdef KALYDO
	// in browser version always use the language set by either the website, or default = eneu
	virtual	void				LoadINI( const char* fileName, const char* language);
#else
	virtual	void				LoadINI( const char* fileName );
#endif // KALYDO
	virtual void				WriteINI( const char* fileName );
	virtual	void				LoadLuaFile(const char* fileName);
	virtual	void				SpriteMsgProc(IBaseSprite* sprite, SpriteMessageType msg, INT64 lParam, INT64 rParam);
	virtual	void				UnitMsgProc(const char* unit, SpriteMessageType msg, CRoleSprite* sprite, int lParam, int rParam);
	virtual	void				LoadLanguage();

	// Font
	virtual string				GetDefaultFontPath();

	virtual void				SetGItemID(int GItemID);
	virtual int					GetGItemID()							{ return m_GItemID; }
	virtual	void				SetIP(const char* ip)					{ m_ip = ip; }
	virtual	const char*			GetIP()									{ return m_ip.c_str(); }

	virtual	void				SetMac(const char* mac)					{ m_macaddress = mac; }
	virtual	const char*			GetMac()								{ return m_macaddress.c_str(); }

	virtual	void				SetPort(int port)						{ m_port = port; }
	virtual	int					GetPort()								{ return m_port; }
	virtual void				SetVersion(const char* version)			{ SecureZeroMemory(m_version, sizeof(m_version)); sprintf_s(m_version, sizeof(m_version), "%s", version);}
	virtual	const char*			GetVersion()							{ return m_version; }
	virtual void				SetVersionNO(const char* version)			{ SecureZeroMemory(m_versionNO, sizeof(m_versionNO)); sprintf_s(m_versionNO, sizeof(m_versionNO), "%s", version);}
	virtual	const char*			GetVersionNO()							{ return m_versionNO; }
	virtual void				SetVersionURL(const char* versionURL)	{ m_versionURL = versionURL;}
	virtual const char*			GetVersionURL()							{ return m_versionURL.c_str();}
	virtual void				SetNoticeURL(const char* noticeURL)		{ m_noticeURL = noticeURL; }
	virtual const char*			GetNoticeURL()							{ return m_noticeURL.c_str(); }
	virtual void				SetActivateURL(const char* activateURL)	{ m_activateURL = activateURL; }
	virtual const char*			GetActivateURL()						{ return m_activateURL.c_str(); }
	virtual void				SetActivateURLArg(const char* activateURLArg)	{ m_activateURLArg = activateURLArg; }
	virtual const char*			GetActivateURLArg()						{ return m_activateURLArg.c_str(); }
	virtual const UpdateInfInfo&	GetUpdateInfo()						{ return m_updateInf; }
	virtual int					GetCpuUsage()										{ return (int)m_CpuUsage.GetUsage(); }
	//virtual MEMORYSTATUSEX	GetMemoryUsage()								{ return m_ProcessMemory;}
	
	virtual void				SetDownloadPort(int port)				{ m_downloadPort = port;}
	virtual int					GetDownloadPort()						{ return m_downloadPort;}
	virtual	void				SetGameTime(int time, int timeZone);
	virtual	int					GetDBID()								{ return m_DBID; }
	virtual void				SetZoneDBID(int DBID)					{ m_zoneDBID = DBID; }
	virtual int					GetZoneDBID()							{ return m_zoneDBID; }
	virtual	int					GetPlayerWorldGUID()					{ return m_playerID; }
	virtual	void				SetPlayerPos(CRuVector3& pos);
	virtual	void				SetZoneID(int zoneID);
	virtual	int					GetZoneID()								{ return m_zoneID; }
	virtual void				SetRoomID(int roomID);
	virtual int					GetRoomID(int roomID)					{ return m_roomID; }
	virtual	void				SetMapName(const char* name);
	virtual	const char*			GetMapName()							{ return m_mapName.c_str(); }
	virtual bool				IsGlobalLoadComplete();
	virtual bool				InsertGlobalObject(int GItemID);
	virtual	void				SetMouseInPlayer(CRoleSprite* sprite);
	virtual	CRoleSprite*		GetMouseInPlayer()						{ return m_pMouseInPlayer; }
	virtual	void				SetPlayer(CRoleSprite* sprite, CRuVector3& pos, CRuVector3& dir);
	virtual	CRoleSprite*		GetPlayer()								{ return m_pPlayer; }	
	virtual	void				SetPlayerID(int playerID);
	virtual	void				SetPlayerTarget(CRoleSprite* sprite);
	virtual	CRoleSprite*		GetPlayerTarget();	
	virtual CRoleSprite*		GetPlayerPet();
	virtual void				SetFocusSprite(int index, CRoleSprite* sprite);
	virtual CRoleSprite*		GetFocusSprite(int index);
	virtual void				ClearFocusSprite(CRoleSprite* sprite);
	virtual	CRoleSprite*		GetTargetNearest(bool enemy=true, bool reverse=false);
	virtual	bool				IsEnterWorld()							{ return m_isEnterWorld; }
	virtual	const char*			GetPlayerName()							{ return m_playerName.c_str(); }
	virtual	void				EnterWorld(int DBID, const char* playerName);
	virtual	void				LeaveWorld();	
	virtual CRoleSprite*		GetUnitSprite(const char* str, CRoleSprite* src=NULL);

	virtual const char*			GetLocalIP()							{ return m_localIP.c_str(); }
	
	virtual	void				SendWarningMsg(const char* msg);
 	virtual	void				SendWarningMsgEx(const char* msg, ...);
	virtual	void				SendSystemMsg(const char* msg);
	virtual void				SendSystemMsgEx(const char* msg, ...);
	virtual void				SendSystemChatMsg(const char* msg);
	virtual void				SendSystemChatMsgEx(const char* msg, ...);

	virtual	void				SendScrollBannerMessage(const char* msg);			//�]���O
	virtual void				SendScrollBannerMessageEx(const char* msg, ...);
	virtual void				SendMessageDialog(const char* msg);				    //�T����ܲ� �@�q�T�� �@���������s
	virtual void				SendMessageDialogEx(const char* msg, ...);

	virtual void				SendPlayTimeQuataNotify(const char* msg);		//���I�g�q����pop dialog, �@�����۰�����, �L������s

	virtual const char*			GetZoneName()							{ return m_zoneName.c_str(); }
	virtual const char*			GetZoneName2()							{ return m_zoneName2.c_str(); }
	virtual int					GetWorldMapID()							{ return m_WorldMapID; }

	virtual	float				GetDistanceWithPlayer(CRoleSprite* sprite);
	virtual	float				GetDistanceWithPlayer(int GItemID)		{ return GetDistanceWithPlayer(FindSpriteByID(GItemID)); }
	virtual	void				SetCombatState(bool combatState);
	virtual	bool				GetCombatState()						{ return m_isCombatState; }	
	virtual	void				AttackTarget(CRoleSprite* target);
	virtual	void				CancelAttack();
	virtual	void				SetPlayerAction(int action);		
	virtual	void				Jump();
	virtual	void				DoEmotion(CRuFusion_ActorState emotion);
	virtual	void				TranslateBaseValue(CRoleSprite* sprite, int mp, int sp, int sub_sp, int* outValue);
	virtual	void				ReceivePing( int iTime );
	virtual	int					GetPing()								{ return m_ping; }

	virtual	void				WorldToViewport(CRuVector3 &point);
	virtual	void				WorldToScreen(CRuVector3 &point);
	virtual	void				SetCursor(CursorType type, const char* dragItem=NULL);

	virtual	void				PlayerDead(bool selfRevive, int resurrectTimeRemaining);
	virtual	void				ResurrectRequest(int source, int magicBaseID);
	virtual	void				PlayerResurrect();
	virtual	bool				GetSelfRevive()							{ return m_selfRevive; }
	virtual	float				GetResurrectTimeRemaining()				{ return m_resurrectTime; }		
	virtual bool				GetResurrectState()						{ return m_resurrect; }

	// ���o�C���ɶ�(0 ~ 239) 240 = �C��1��
	virtual	int					GetCurrentGameTime()					{ return m_currentGameTime; }
	virtual time_t				GetGameTime()							{ return m_gameTime + (time_t)(m_frameTime - m_startFrameTime); }
	virtual time_t				GetServerTimeZone()						{ return m_serverTimeZone; }
	virtual	void				SetPlayerCameraPos(CRuVector3& pos);	

	// �I�k�ˬd
	virtual	int					CanUseSkill(int skillID, int* outNeedType=NULL, int* outNeedValue=NULL);
	virtual	bool				CastSpellByID(int ID, CRoleSprite* target=NULL);
	virtual bool				CastSpellByName(const char* name, int level, CRoleSprite* target=NULL);
	virtual	bool				SetBeginSpell(CRoleSprite* target);
	virtual	void				SetCastSpellState(eCastSpellState state, bool forceReset=false);	
	virtual	eCastSpellState		GetCastSpellState()						{ return m_castSpellState; }
	virtual	bool				SpellStopCasting();
	virtual void				ClearCastSpellCount()					{ m_sendCastSpellCount = 0; }
	virtual void				ClearCastSpellTime()					{ m_lastCastSpellTime = 0.0; }
	virtual void				ClearCancelSpellState()					{ m_isCancelCastSpell = false; }
	virtual	void				CancelCastSpell();
	virtual void				ClearSendSpell()						{ m_sendSpellTarget = NULL; m_sendSpellMagicID = 0; }

	// Control Argument
	virtual	void				SetPlayerTitleVisible(bool show);
	virtual	void				SetNPCTitleVisible(bool show);
	virtual	void				SetSelfTitleVisible(bool show);
	virtual	void				SetTitleVisible(bool show);
	virtual	void				SetGuildVisible(bool show);
	virtual	void				SetTitleIconVisible(bool show);
	virtual	void				SetMouseMoveEnable(bool enable)			{ m_clientSetting.isMouseMoveEnable = enable; }
	virtual	void				SetDisableTitleHide(bool hide)			{ m_clientSetting.isDisableTitleHide = hide; }
	virtual	void				SetDisplayAllSCT(bool show)				{ m_clientSetting.isAllSCTVisible = show; }
	virtual	void				SetSelfCastState(bool selfCast)			{ m_clientSetting.isSelfCast = selfCast; }
	virtual	void				SetLastRealmState(bool selected)		{ m_clientSetting.isLastSelectedRealm = selected; }
	virtual	void				SetLastAccountNameState(bool selected)	{ m_clientSetting.isLastAccountName = selected; }
  	virtual	void				SetShowGemePromrt(bool selected)		{ m_clientSetting.isShowGemePromrt = selected; }
	virtual	void				SetNPCRealHealthState(bool selected)	{ m_clientSetting.isNPCRealHealth = selected; }
	virtual void				SetCameraYReverse(bool reverse)			{ m_clientSetting.isCameraYReverse = reverse; }
	virtual void				SetLButtonCancelTarget(bool state)		{ m_clientSetting.isLButtonCancelTarget = state; }
	virtual void				SetRButtonCancelTarget(bool state)		{ m_clientSetting.isRBuutonCancelTarget = state; }
	virtual void				SetLButtonCameraRotateEnable(bool state){ m_clientSetting.isLButtonCameraRotateEnable = state; }	
	virtual void				SetCameraFollowEnable(bool state)		{ m_clientSetting.isCameraFollowEnable = state; }
	virtual void				SetDisableDisplayNPCTalk(bool state)	{ m_clientSetting.isDisableDisplayNPCTalk = state; }
	virtual void				SetCameraSelectTarget(bool state)		{ m_clientSetting.isCameraSelectTarget = state; }
	virtual void				SetLootPlayerMotion(bool state)			{ m_clientSetting.isPlayerLootMotion = state; }	
	virtual void				SetChatDisplayClassColor(bool enbale)	{ m_clientSetting.isChatDisplayClassColor = enbale; }
	virtual void				SetAutoTakeLoot(bool enbale)			{ m_clientSetting.isAutoTakeLoot = enbale; }	
	virtual void				SetAggroPrompt(bool enbale)				{ m_clientSetting.isAggroPrompt = enbale; }		
	virtual void				SetAutoOpenGoodsPack(bool enbale)		{ m_clientSetting.isAutoOpenGoodsPack = enbale; }	
	virtual void				SetHideOtherPlayerEffect(bool enable)	{ m_clientSetting.isHideOtherPlayerEffect = enable; }
	virtual	bool				GetPlayerTitleVisible()					{ return m_clientSetting.isPlayerTitleVisible; }
	virtual	bool				GetNPCTitleVisible()					{ return m_clientSetting.isNPCTitleVisible; }
	virtual	bool				GetSelfTitleVisible()					{ return m_clientSetting.isSelfTitleVisible; }
	virtual	bool				GetMouseMoveEnable()					{ return m_clientSetting.isMouseMoveEnable; }
	virtual	bool				GetDisableTitleHide()					{ return m_clientSetting.isDisableTitleHide; }
	virtual	bool				GetDisplayAllSCT()						{ return m_clientSetting.isAllSCTVisible ; }
	virtual	bool				GetSelfCastState()						{ return m_clientSetting.isSelfCast; }
	virtual	bool				GetLastRealmState()						{ return m_clientSetting.isLastSelectedRealm; }
	virtual	bool				GetLastAccountNameState()				{ return m_clientSetting.isLastAccountName; }
 	virtual	bool				GetShowGemePromrt()						{ return m_clientSetting.isShowGemePromrt; }
	virtual	bool				GetNPCRealHealthState()					{ return m_clientSetting.isNPCRealHealth; }
	virtual	bool				GetTitleVisible()						{ return m_clientSetting.isTitleVisible; }
	virtual	bool				GetGuildVisible()						{ return m_clientSetting.isGuildVisible; }
	virtual bool				GetTitleIconVisible()					{ return m_clientSetting.isTitleIconVisible; }
	virtual bool				GetCameraYReverse()						{ return m_clientSetting.isCameraYReverse; }
	virtual bool				GetLButtonCancelTarget()				{ return m_clientSetting.isLButtonCancelTarget; }
	virtual bool				GetRButtonCancelTarget()				{ return m_clientSetting.isRBuutonCancelTarget; }
	virtual bool				GetLButtonCameraRotateEnable()			{ return m_clientSetting.isLButtonCameraRotateEnable; }
	virtual bool				GetCameraFollowEnable()					{ return m_clientSetting.isCameraFollowEnable; }
	virtual bool				GetDisableDisplayNPCTalk()				{ return m_clientSetting.isDisableDisplayNPCTalk; }
	virtual bool				GetCameraSelectTarget()					{ return m_clientSetting.isCameraSelectTarget; }
	virtual bool				GetLootPlayerMotion()					{ return m_clientSetting.isPlayerLootMotion; }	
	virtual bool				GetChatDisplayClassColor()				{ return m_clientSetting.isChatDisplayClassColor; }
	virtual bool				GetAutoTakeLoot()						{ return m_clientSetting.isAutoTakeLoot; }
	virtual bool				GetAggroPrompt()						{ return m_clientSetting.isAggroPrompt; }
	virtual bool				GetAutoOpenGoodsPack()					{ return m_clientSetting.isAutoOpenGoodsPack; }
	virtual bool				GetHideOtherPlayerEffect()				{ return m_clientSetting.isHideOtherPlayerEffect; }
	virtual bool				CheckDisplayEffect(int id, int targetId=0);
	virtual bool				CheckDisplayEffect(CRoleSprite* ownerSprite, CRoleSprite* targetSprite=NULL);
	virtual void				ResetControlArgument();	
	
	// virtual client setting
	virtual void				SetServerSaveClientData(bool state)		{ m_clientSetting.isUseServerData = state; }
	virtual bool				GetServerSaveClientData()				{ return m_clientSetting.isUseServerData; }

	virtual bool				IsChatLog()								{ return m_IsChatLog;}

	// Item Casting
	virtual bool				ItemCastingStart(ItemCastingType type, int srcPos, int destType, int destPos);
	virtual void				ItemCastingInterrupt();
	virtual void				ItemCastingStop();
	virtual	void				ItemCastingReset();
	virtual	bool				ItemCastingEnable()						{ return m_itemCasting.type != ITEM_CASTING_NONE; }
	virtual	bool				UseBagItem(int index, int x=0.0f, int y=0.0f, int z=0.0f);

	// Follow
	virtual	void				FollowSprite(CRoleSprite* sprite);
	virtual CRoleSprite*		GetFollowSprite()						{ return m_follow.target; }

	// Move
	virtual	void				MoveForwardState(bool start);
	virtual	void				MoveBackwardState(bool start);
	virtual	void				TurnRightState(bool start);
	virtual	void				TurnLeftState(bool start);
	virtual	void				StrafeRightState(bool start);
	virtual	void				StrafeLeftState(bool start);
	virtual	void				ToggleAutoRune(int forceState=0);
	virtual	void				ToggleRun();
	virtual	void				ToggleSheath();
	virtual	void				SitOrStand();
	
	virtual	int					OnClick(CRoleSprite* target);	

	//MoveToScreenPoint
   	virtual	void				MoveToScreenPoint_AddSprite( int SpriteID );
   	virtual	void				MoveToScreenPoint_DelSprite( int SpriteID );
	virtual	void				MoveToScreenPoint_ClearSprite( );

	virtual	void				MoveToScreenPoint(int x,  int y);
	virtual	void				SetMoveToPoint(CRuVector3& point, CRoleSprite* sprite=NULL, bool forceMove=true);
	virtual	void				ClearMoveToPoint();
	virtual	bool				IsMoveingToPoint()						{ return m_mouseMove.enable; }

	virtual	void				RotateCamera(float xDelta, float yDelta, bool updatePlayerDir=false);
	virtual void				SetCameraForward(CRuVector3 forward, bool immediately=true);

	// Tutorial Flag
	virtual	void				SetTutorialState(int index, bool state);
	virtual	bool				GetTutorialState(int index);
	virtual	void				ClearTutorialState();	

	// 
	virtual void				ClearCastSpellToItem()					{ m_castSpellToItem = 0; }
	virtual	int					GetCastSpellToItemID()					{ return m_castSpellToItem; }

	virtual	void				TakeScreenshot();	

	virtual void				ReloadClientLua()						{ m_isReloadClientLua = true; }
	virtual	void				LoadClientLua();

	virtual	void				SaveEnvironmentVariables();
	virtual	void				LoadEnvironmentVariables();

	// Login Screen
	virtual void				LoadLoginScreen();

	// Mount
	virtual void				Dismount();	

	// WDB / FDB Journaling
	CRuDataStore_Disk*			m_wdbJournalDS;

	void						FDBJournal_Begin(const char *journalName)
	{
		g_ruResourceManager->BeginJournal(journalName);
	}

	void						FDBJournal_End()
	{
		g_ruResourceManager->EndJournal();
	}

	void						WDBJournal_Begin(const char *journalName)
	{
		// Create journaling WDB
		m_wdbJournalDS = ruNEW CRuDataStore_Disk();
		m_wdbJournalDS->Create(journalName);
		m_wdbJournalDS->DisablePagePadding(TRUE);

		// Begin data store journal
		CRuDataStore_Disk *srcDS = static_cast<CRuDataStore_Disk *>(static_cast<CRuOptimalTerrain_DataSource_DS *>(GetRuTerrain()->GetDataSource())->GetDataStore());
		m_wdbJournalDS->BeginJournal(srcDS);

		// Copy required tables in their entirety
		srcDS->CopyTo(m_wdbJournalDS, "header");
		srcDS->CopyTo(m_wdbJournalDS, "backdrop_header");
		srcDS->CopyTo(m_wdbJournalDS, "texture_list");
		srcDS->CopyTo(m_wdbJournalDS, "water_descriptor");
		srcDS->CopyTo(m_wdbJournalDS, "worldDoodadContainer_DoodadTemplate");
		srcDS->CopyTo(m_wdbJournalDS, "world_zone_descriptors");
	}

	void						WDBJournal_End()
	{
		m_wdbJournalDS->EndJournal();
		m_wdbJournalDS->Flush();
		ruSAFE_RELEASE(m_wdbJournalDS);
	}

	// Pattern
	virtual void				PushPatternString(const BYTE* pattern);	
	virtual void				RemovePatternString(const BYTE* pattern , int type);

	virtual bool				IsPatternStringVisible(const BYTE* pattern);

	virtual void				MatchPatternString(BYTE* text);	
	virtual bool				CheckPatternString( const char* text);
	virtual void				ReturnCharacterSelect();

	virtual bool				IsShowServerDebugMsg()					{ return m_displaySrvDebugMsg; }

	virtual bool				CheckRightfulName(const char* name);
	virtual float				GetTerrainLoadStatus();
	
	virtual void				ResetSpriteNameVisible(CRoleSprite* sprite);
	virtual void				SetSpriteGuildID(CRoleSprite* sprite, int guildID);
	virtual void				GuildEnemyUpdate();

	// �󴫤��|�W��
	virtual void				ChangeGuildName(int guildID);	

	virtual void				SetCampState(bool state);
	virtual bool				GetCampState()							{ return m_campState; }
	virtual void				ChangeParallelID(int id);
	virtual int					GetChangeParallelID()					{ return m_parallelID; }
	virtual void				ClearChangeParalleID()					{ m_parallelID = -1; }

	virtual float				GetTotalFPS()							{ return m_totalFps; }
	virtual float				GetTotalPing()							{ return m_totalPing; }
	virtual int					GetTotalCount()							{ return m_totalCount; }
	virtual void				ClearTotalCount();	
	virtual bool				GetOSDisplayString(char* pszOS , size_t cchLen);

	// ���o���aPVP���A
	virtual bool				GetSpritePVPState(CRoleSprite* sprite);

	// �ϰ�PK
	virtual void				UpdatePVPZoneState(int state = -1);
	// �]�w���aPVP�X��(BUFF ����)	
	virtual void				UpdateSpritePKBuff();
	// �}�����
	virtual void				SetSpriteCampId(CRoleSprite* sprite, int campId, bool force=false);

	// ���]���a�I��X�l�㹳����
	virtual void				ResetObjectGuildFlag(int guildID, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor);

	// ���o�G���K�X	
	virtual bool				CheckSecondPassword();
	
	virtual const char*			GetSecondPassword()						{ return m_secondPassword.c_str(); }
	virtual void				SetSecondPassword(const char* password);
	virtual void				SendSecondPassword(const char* password);
	virtual void				ClearSecondPassword();

	// �]�w���Y��
	virtual void				SetCampTableObj(int id);

	virtual const char*			GetLanguage()							{ return m_language.c_str(); }
	virtual const char*			GetCountry()							{ return m_country.c_str(); }
	virtual const char*			GetInstall()							{ return m_install.c_str(); }
	virtual const char*			GetImageLocation(const char* section=NULL);

	virtual bool				PathMoveTo(CRuVector3 target);
	virtual void				ClearPathMove();
	virtual bool				IsPlayerMoveLocked();

	virtual void				SetHyperlinkCursor(bool enable)			{ m_cursorHyperlink = enable; }
	virtual void				SetPVEState(bool isPVE);
	virtual bool				GetPVEState()							{ return m_isPVE; }

	// ���ۦP�W�٪��a
	void						ComparePlayerName();	

	// �]�w�ɶ�
	void						SetTimeGroupType(int groupType)			{ m_timeGroupType = groupType; }
	
	// �ӸO����
	void						SetTombInfo(int zoneId, CRuVector3 position, float leftTime, int debtExp, int debtTp, int exp);
	void						ClearTomb();
	void						UpdateTomb();

	// �d��
	void						CreatePet(int petType, int petGUID, int petItemID, int petMagicID);
	void						DeletePet(int petType, int petGUID);
	void						UpdatePetSkill();
	void						SetPetMovable(bool enable);
	bool						IsPetMovable()							{ return m_petStatus.movable; }
	void						SetPetStrikeBackEnable(bool enable);
	bool						IsPetStrikeBackEnable()					{ return m_petStatus.strikeBack; }
	void						UpdatePetSprite(CRoleSprite* player);

	const char*					GetDataLanguage()						{ return m_DataLanguage.c_str(); }
	const char*					GetStringLanguage()						{ return m_StringLanguage.c_str(); }	

	bool						IsAttackAction()						{ return m_attackWorldGUID != 0; }
	bool						IsCycleSpellID(int id)					{ return m_cycleMagicId == id; }
	void						SetCycleSpell(int id, int index);
	void						ClearCycleSpell();

	void						AddPartitionPosition(CRoleSprite* sprite, CRuVector3 position);
	CRuVector3*					FindPartitionPosition(CRoleSprite* sprite);
	void						ClearPartitionPosition()				{ m_partitionPosition.clear(); }	

	int							GetPetGUID();

	void						SwitchUpdateElapsedTime(bool show)		{ m_showUpdateElapsedTime = show; }

	// ��^ĵ�i
	void						AddEliteBell(int GItemID);
	void						UpdateEliteBell();


	// ���~������椺�e
	int							PushItemCommand(ItemCommand command);
	void						ExcuteItemCommand(int id);
	void						RemoveItemCommand(int id);

	// 
	const char*					GetGameServerIP()						{ return m_gameServerIP.c_str(); }
	int							GetGameServerPort()						{ return m_gameServerPort; }

#ifdef KALYDO
	void						SetGameServerIP(const char* ip)			{ m_gameServerIP = ip; }
	void						SetGameServerPort(int port)				{ m_gameServerPort = port; }
#endif // KALYDO

	// �S��ƥ�
	string						GetSpecialEventString(int id);	

	// �ϰ��T
	void						SetDisableTrade(bool disable)			{ m_isDisableTrdae = disable; }	
	void						SetDisableDuel(bool disable)			{ m_isDisableDuel = disable; }
	void						SetDisableParty(bool disable)			{ m_isDisableParty = disable; }
	void						SetEnableBGParty(bool enable)			{ m_isEnableBGParty = enable; }
	void						SetBattleGroundZone(bool inBattle)		{ m_isBattleGround = inBattle; }
	void						SetNoSwimming(bool noSwimming)			{ m_isNoSwimming = noSwimming; }
	bool						IsDisableTrade()						{ return m_isDisableTrdae; }
	bool						IsDisableDuel()							{ return m_isDisableDuel; }	
	bool						IsDisableParty()						{ return m_isDisableParty; }	
	bool						IsEnableBGParty()						{ return m_isEnableBGParty; }	
	bool						IsBattleGroundZone()					{ return m_isBattleGround; }
	bool						IsNoSwimming()							{ return m_isNoSwimming; }
	
	int                         GetBattleGroundType();
	bool						IsEnableBattleGroup();
	void						SetWorldBattleGround(bool state)		{ m_isWorldBattleGround = state; }
	bool						IsWorldBattleGround()					{ return m_isWorldBattleGround; }

	// �Գ�
	void						ClearAllBattleMember();
	bool						AddBattleMember(int GItemID, CRoleSprite* sprite);
	bool						DeleteBettleMember(int GItemID);
	CRoleSprite*				FindBettleMemberByGItemID(int GItemID);
	CRoleSprite*				FindBettleMemberByDBID(int DBID);
	CRoleSprite*				FindBettleMemberByWorldID(int WorldID, const char* playerName);
	CRoleSprite*				FindBattleMemberByName(const char* playerName);
	int							GetBattleGroundNumPlayers()				{ return m_numBattleGroundPlayers; }

	// �b�K
	void						SetAccountName(const char* account)				{ m_account = account; }
	void						SetAccountPassword(const char* password)		{ m_password = password; }
	const char*					GetAccountName()								{ return m_account.c_str(); }
	const char*					GetAccountPassword()							{ return m_password.c_str(); }

	// ���s����
	void						AddAttachObj(int source, int target, int mode, const char* sourceLink, const char* targetLink);
	void						DeleteAttachObj(int GItemID);
	void						ClearAttachObj();
	void						UpdateAttachObj();

	// �O�����a�Ӽ�
	int							GetPlayerSpriteCount();
	void						UpdateDummySprite();

	bool						InStressZone();
	void						SetServerInfo(const char* country, int IPNum);
	const char*					GetServerCountry()						{ return m_serverCountry.c_str(); }
	int							GetServerIPNum()						{ return m_IPNum; }

	void						AddPlayerRefCount()						{ m_playerRefCount++; }
	void						DelPlayerRefCount()						{ m_playerRefCount--; }
	int							PlayerRefCount()						{ return m_playerRefCount; }
	bool						IsDiaplayMaintainServer()				{ return m_displayMaintainServer; }	

	bool						IsUIUpdate();	

	bool						SaveRoleData(const wchar_t* name);
	bool						LoadRoleData(const wchar_t* name);

	// �O�_�b����
	bool						IsInWater();

	// �]�w���y�Ӽ�
	void						SetZoneChannel(int ZoneID, bool boOpen);
	set<int>&					GetZoneChannel()						{ return m_zoneOpen; }
	
	void						SetBotLocked(bool state);
	bool						GetBotLocked()							{ return m_botLocked; }

	// �M���ƥ��T�ݶi��
	void						RequestInstanceRecord();
	void						ClearInstanceWaitTime();
	void						AcceptInstanceRecord();
	void						DeclineInstanceRecord();

	void						CheckMemoryStatus(bool forceUpdate=false);
	DWORD						GetMemoryLoad()							{ return m_memoryLoad; }
	DWORD						GetCurrentMemory()						{ return m_currentMemory; }

	// �s������
	void						AttachPoint(CRuVector3 position, CRuVector3 dir, CRuFusion_ActorState actorState);

	//����Entity �W �p�G�L�� p_sit_chair01 �Ѧ��I
	bool						SitToEntity( CRuEntity* pEntity );

	//Entity �O���O�Ȥl �p�G�L�� p_sit_chair01 �Ѧ��I
	//poutDistance ���a��Ѧ��I���Z��
 	bool						EntityIsChair( CRuEntity* pEntity, float* poutDistance = NULL );

	// Client Data
	void						SaveClientSetting(bool Immediately=false);
	void						SaveClientData(int keyID, int size, char* data);
	void						SendLoadClientData();
	void						ReceiveLoadClientData(int keyID, int size, char* data);

	// VoiceSettings
	void						SetVoiceDisable( bool isDisable ) { m_bIsVoiceSettingsDisable = isDisable; }
	bool						GetVoiceDisable() { return m_bIsVoiceSettingsDisable; }

	bool						CheckCountry( const char* country );

	// Room Event Flag
	void						PushRoomValue(int keyID, int value);
	map<int,int>::iterator		RemoveRoomValue(int keyID);
	map<int,int>::iterator		RemoveRoomValue(map<int, int>::iterator iter);
	void						ClearRoomValue();

	void						SetFreeCameraEnalbe(bool enable)					{ m_isFreeCamera = enable; }
	bool						GetFreeCameraEnalbe(bool enable)					{ return m_isFreeCamera; }
	void						SetCameraFollowSprite(CRoleSprite* sprite);

	void						CreateTargetEffect(TargetLineEffectType targetType, CRoleSprite* referenceSprite);
	void						ReleaseTargetEffect();
	bool						RotateTargetEffect(float xDelta, float yDelta);	
	bool						UpdateTargetEffectPosition();
	void						SetTargetEffectDirection(CRuVector3 dir);
	void						SetTargetEffectType(TargetLineEffectType targetType, int height);
	void						SetTargetEffectParabolaHeight(float height)			{ m_targetParabolaHeight = height; }
	float						GetTargetEffectParabolaHeight()						{ return m_targetParabolaHeight; }
	void						SetTargetEffectMaxRange(float range)				{ m_targetMaxRange = range; }
	float						GetTargetEffectMaxRange()							{ return m_targetMaxRange; }
	void						SetTargetEffectMaxAngle(float angle)				{ m_targetMaxAngle = angle; }
	float						GetTargetEffectMaxAngle()							{ return m_targetMaxAngle; }
	TargetLineEffectType		GetTargetEffectType()								{ return m_targetLineType; }
	

	void						RegisterZoneChangeCallBack(ZoneDescriptorCallback event);

	BOOL						QueryCollision(CRuVector3& beginPoint, CRuVector3& endPoint, CRuVector3& outCollisionPoint, bool boTerrain = true, bool spriteCollision = false, CRuEntity* ignoreEntity=NULL);
	float						GetMagicFlyTime(CRuVector3& p1, CRuVector3& p2, int magicID, int parabolaHeight=0);

	bool						InsertAnchorHeightSprite(int id, CRuVector3& position);
	bool						IsAnchorHeightSpriteLoadComplete();

	void						SetPaperdollTextureDetail(int level);
	int							GetPaperdollTextureDetail();
	void						SetTextureDetail(int level);
	int							GetTextureDetail();


	void						SetGuildBattleGroundState(bool enable)				{ m_systemEnable.guildBattleGround = enable; }
	bool						IsGuildBattleGroundEnable()							{ return m_systemEnable.guildBattleGround; }

	void						SetCh3LoginState(bool enable)						{ m_systemEnable.ch3Login = enable; }
	bool						IsCh3LoginEnable()									{ return m_systemEnable.ch3Login; }

	void						SetLoginEpisodeVersion(int episodeVersion)			{ m_systemEnable.loginEpisode = episodeVersion; }
	int							GetLoginEpisodeVersion()							{ return m_systemEnable.loginEpisode; }

	void						SetVivoxUserAgreementState(bool enable)				{ m_systemEnable.vivoxUserAgreement = enable; }
	bool						IsVivoxUserAgreementEnable()						{ return m_systemEnable.vivoxUserAgreement; }

	void						MovePartition(bool enable);

	bool						getMerchant()										{return m_bMerchantFrame;}
	void						setMerchant(bool enable=true)						{m_bMerchantFrame=enable;}

	void						SetUpdateInfInfo(const UpdateInfInfo& info)			{ m_updateInf = info; }

	bool						TestHeight(CRuVector3 position, float& outHeight);

	//�K�X���A����
	bool						GetIsMD5Password()									{return m_IsMD5Password;}
	bool						GetIsAutoHashPassword()								{return m_IsAutoHashPassword;}
	int							GetPasswordHashType()								{return m_PasswordHashType;}

	void						SkipCastSpellCheck(bool isSkip)						{ m_skipCastSpellCheck = isSkip; }

	//��L
	int							GetItemMallSessionID();
	int							GetZShopSessionID();
	void						SetCameraControlMode(bool enabled)					{ m_cameraControl = enabled; }
	bool						GetCameraControlMode()								{ return m_cameraControl; }

	int							GetPlayerCount();

	//ZShop
	bool						GetEnableZShop()									{ return m_enableZShop; }
	void						SetEnableZShop(bool value)							{ m_enableZShop = value; }
	const char*					GetZShopURL()										{ return m_ZShopURL.c_str(); }
	void						SetZShopURL(const char* value)						{ m_ZShopURL = value; }
	

	int							GetWBWidth()										{ return m_iWBWidth; }
	int							GetWBHeight()										{ return m_iWBHeight; }

	void						SetWBWidth( int iw )								{ m_iWBWidth = iw; }
	void						SetWBHeight( int ih )								{ m_iWBHeight = ih; }

	//Game Force platform
	bool						IsGameForgeCountries();

protected:	
	virtual	void				UpdateWorld();
	virtual	void				CheckTouchSprite();
	virtual void				FixedPosition();
	virtual	void				SendPlayerPos(bool forceSend=false, bool setPos=false);
	virtual	void				SendPlayerActor();
	virtual	void				UpdateCursor();
	virtual	void				ControlCamera();
	virtual	void				UpdateSneakSprite();
	virtual	void				MovePlayerPos(CRuVector3& pos);

	virtual	void				UpdateMouseMove();
	virtual void				UpdatePathMove();
	virtual void				SendPacketReduceMode(PacketReduceState state);

	virtual void				CheckPlayerPositionHeight();

protected:
	// net information
	string						m_ip;
	string						m_macaddress;
	int							m_port;
	string						m_gameServerIP;
	int							m_gameServerPort;
	int							m_currentGameTime;				// �ثe�C���ɶ�
	int							m_gameTime;						// �C���ɶ�
	int							m_serverTimeZone;				// server �ɰϮɶ�	

	double						m_startFrameTime;				// �}�l�p�ɮɶ�

	string						m_StringLanguage;				
	string						m_DataLanguage;		

	string						m_playerName;					// ���a�W��
	string						m_zoneName;						// �ϰ�W��
	string						m_zoneName2;					// �ϰ�W��2
	int							m_WorldMapID;					// �a��id(UI��)
	int							m_playerID;						// ���a����s��(�@�ɽs��)
	int							m_GItemID;						// �x�s����s��(�ϰ�s��)
	int							m_DBID;							// SERVER����ߤ@�N�X
	int							m_zoneID;						// �ϰ�W��
	int							m_roomID;						// �ж����X
	string						m_mapName;						// �a�ϦW��	
	bool						m_receiveWaitLoading;			// ���ݸ��J��
	int							m_changeTerrainStep;			// �󴫦a��
	bool						m_isEnterWorld;					// �O�_�i�J�C���@��
	bool						m_isFirstViewpoint;				// �Ĥ@�H�ٵ���	
	bool						m_isCombatState;				// �i�J��������,�����A������SERVER�e�X�T��
	bool						m_isPVE;						// PVE ���A
	int							m_playerRefCount;				// ���a�p�ƾ�
	CRuVector3					m_changeTerrainPos;	
	
	CRoleSprite*				m_pMouseInPlayer;	
	float						m_followTime;
	int							m_attackWorldGUID;
	CRuArrayList<int>			m_activeColTriggersThisCycle;
	CRuArrayList<int>			m_activeColTriggersLastCycle;

	CRuVector3					m_moveVector;
	CRuVector3					m_direction;
	CRuVector3					m_position;
	CRuFusion_ActorState		m_sendActorState;
	int							m_parentMountId;
	float						m_sendMoveTime;	
	bool						m_firstJump;
	bool						m_pingState;
	bool						m_falling;
	int							m_startPing;
	int							m_ping;	
	
	PLAYER_ACTOR_STATE			m_actorState;
	RoleSpriteStatus			m_playerState;
	CursorType					m_cursorType;
	bool						m_cursorHyperlink;				// �O�_�bHyperlink����W
	string						m_dragItemStr;
	
	bool						m_selfRevive;
	bool						m_resurrect;
	float						m_resurrectTime;
	
	eCastSpellState				m_castSpellState;				// �I�k���A
	int							m_castSpellID;					// �I�k�s��
	int							m_castSpellIndex;				// �I�k���޽s��(�@�묰-1,�_�h�h�N��b�����I��ؼ�)
	int							m_castSpellToItem;				// �磌�~�ϥ�	
	int							m_sendCastSpellCount;			// �w�g�e�X���I�k�Ӽ�
	bool						m_isCancelCastSpell;			// �����I�k
	double						m_lastCastSpellTime;			// �̫�e�X���I�k�ɶ�

	CRoleSprite*				m_sendSpellTarget;				// �e�X���I�k��H
	int							m_sendSpellMagicID;				// �e�X���I�k�s��

	bool						m_IsChatLog;
	char						m_version[256];                 // �{�bClient�ݪ�����
	char						m_versionNO[256];               // �{�bClient��data������ 
	string                      m_versionURL;                   // ��s���A���WVersion.Txt��URL
	string						m_noticeURL;					// ���i��T
	string						m_activateURL;					// K2 �E���b��
	string						m_activateURLArg;				// �Ѽ�
	int                         m_downloadPort;                 // ��s���A��http port

	// �a��
	bool						m_needLoadTerrain;				// �ݭn���s���J�a��
	float						m_loadTerrainDelayTime;			// �@����J�a�Ϯɶ��L��,����LOADING�e���X�{�b�����J�ʧ@
	float						m_sendLoadOkDelayTime;			// �e�^SERVER�}�l�[�J���󩵿�ɶ�

	// ���SERVER�Ӫ��T��
	bool						m_displaySrvDebugMsg;

	bool						m_IsMD5Password;				//�n�J��, �ϥΪ̿�J���K�X�O�_��MD5 Hash�L���K�X
	bool						m_IsAutoHashPassword;			//�۰�Hash�ϥΪ̱K�X�X��
	int							m_PasswordHashType;				//Hash�K�X�覡 0=md5, 1=bcrypt �۰�Hash�ϥΪ̱K�X�X�ж}�Үɦ���

	// ���ʺX��
	union {
		int Bytes;
		struct {
			bool			forward			:1;			// �V�e����
			bool			backward		:1;			// �V�Ჾ��
			bool			strafeRight		:1;			// �V�k����
			bool			strafeLeft		:1;			// �V������
			bool			turnRight		:1;			// ��a�V�k��V
			bool			turnLeft		:1;			// ��a�V����V
			bool			autoRun			:1;			// �۰ʦV�e�]
			bool			walk			:1;			// ����
		};
	}m_moveState;

	// ����
	struct {
		int					dir;						// ���e��V
		float				time;						// ��s�ɶ�
	}m_fear;

	// �ƹ�����
	struct {
		CRuVector3			target;
		CRoleSprite*		sprite;
		bool				enable;
		float				update;
		float				distance;
	}m_mouseMove;

	// ���|����	

	// ���H
	struct {
		CRoleSprite*		target;						// ���H��H
		float				touchRange;					// �@�ζZ��
		float				update;						// ���s���o�y�Щ���ɶ�
		float				distance;					// �W����s�P�ؼ��I�Z����
	}m_follow;	

	// �O�����~�磌�~�ϥΤ�(�ݭn�]�ɶ��b,�����ݮɶ�)
	struct {
		ItemCastingType		type;						// �ӷ����~�ϥ�����
		int					bagPos;						// �I�]��m
		int					destType;					// �ت����� 0�I�] 1�˳� 2�Ȧ�
		int					destPos;					// �ت������Ҧb��m
		int					actor;						// �ʧ@�N�X
		bool				hasEffect;					// �O�_���S�Ĳ���
	}m_itemCasting;

	vector<CRuAlgorithm_PatternMatch_FJS*> m_patternMatch;

	CACTActionEventsManager	m_ACTActionEventsManager;	
	float					m_worldCameraFarPlane;
	int						m_PVPZoneState;				// �ϰ�PK�X��

	bool					m_campState;				// �n�X���A(true �n�X��)
	float					m_campUpdateTime;			// Ĳ�o�ɶ�
	int						m_parallelID;				// ���y���A	

	float					m_totalFps;					// �`�i��
	float					m_totalPing;				// �`�XPing
	double					m_totalLastTime;			// �̫��s�ɶ�
	int						m_totalCount;				// �W�Ǹ�ƭp�ƾ�

	CRuVector3					m_rightPosition;		// �̫�T�{�y���I
	CRuArrayList<CRuVector3>	m_obstructPosition;		// �J����ɪ��y���I

	string					m_secondPassword;			// �G���K�X
	bool					m_sendPassword;

	string					m_install;					// �w�˰�a�N�X
	string					m_country;					// ��a�N�X
	string					m_language;					// �y���N�X
	string					m_saveFolder;				// �x�s�ؿ��W��
	int						m_campTableId;				// ���Y��

	vector<CRuVector3>		m_pathPositions;			// ���|���
	CRuVector3				m_pathTargetPosition;		// ���|���ʥؼ��I	
	float					m_pathMoveTime;				// ���|���ʨ쑴�ؼЩһݮɶ�	
	float					m_pathMoveUpdate;			// ���|���ʧ�s�ɶ�

	int						m_resetFontIndexStep;		// ��s�r��
 
	set<int>				m_MoveToScreenPointSpriteIDs; //MoveToScreenPoint�� �]�ݭn�˴����S���I�쪺SpriteID

	CRoleSprite*			m_focusSprites[MAX_FOCUS_SPRITES];	

	CRuVector3				m_cameraCurrentVector;		// ��v���ثe��V
	CRuVector3				m_cameraFinalVector;		// �̫���v���¦V��V

	int						m_timeGroupType;			// �]�w�ɶ�����(1~4)	

	CRuVector3				m_tombPosition;				// �y�Ц�m
	float					m_tombLeftTime;				// �ѩ�ɶ� ( > 0 ) �s�b
	int						m_tombZoneId;				// �ϰ�s��
	int						m_tombDebtExp;				// �t�Ÿg���
	int						m_tombDebtTp;				// �t�ŧޯ��I��
	int						m_tombExp;					// �ѩ�g���
	
	// �d����T
	int						m_petGUID;					// 
	int						m_petItemID;				//
	int						m_petMagicID;				//
	PetCommandStruct		m_petStatus;				// �d�����A
	map<int, PetInfo>		m_petInfo;

	bool					m_isWorldBattleGround;		// �@�ɾԳ�
	bool					m_isBattleGround;			// �O�_�b�Գ���
	bool					m_isDisableTrdae;			// ������
	bool					m_isDisableDuel;			// ������
	bool					m_isDisableParty;			// �����ܽвն�
	bool					m_isEnableBGParty;			// �}�ҾԳ����ն�
	bool					m_inWater;					// �O�_�b����
	bool					m_isNoSwimming;				// �ϰ줣���a
	int						m_numBattleGroundPlayers;	// �Գ��̤j�H�Ƴ]�w

	int						m_cycleMagicId;				// �s��I�i�k�N�s��
	int						m_cycleMagicPos;			// �s��I�i�k�N�Ѧ�m	

	CRuRendererPlugin_SubmergeFX*	m_submergeFX;		// Submerge FX plugin

	map<CRoleSprite*, CRuVector3>	m_partitionPosition;

	bool					m_isWaitGlobalObject;		// ���ݥ��쪫����J
	vector<int>				m_globalObject;				// ���쪫��

	bool					m_showUpdateElapsedTime;	// ��ܨC��Frame��s�ɶ�

	map<int, float>			m_eliteBell;				// ��^����ĵ�i

	int						m_playTimeQuota;			// ���I�g�T��
	float					m_QuotaUpdateTime;

	map<int, ItemCommand>	m_itemCommand;
	int						m_lastItemCommandID;		// �̫�@�Ӱ���s��

	string					m_localIP;					// �� Server �Ǧ^�����a Client IP
	bool					m_isReloadClientLua;			// ���s���J

	map<int, string>		m_specialEventMap;			// �S��r�����

	map<int, CRoleSprite*>	m_BattleMember;				// �Գ��S���a���

	string					m_account;					// ��l�t�Υѥ~���ǤJ�b�K
	string					m_password;					// ��l�t�Υѥ~���ǤJ�b�K	

	vector<AttachObjectInfo>	m_attachObject;

	double					m_lastDummySpriteUpdateTime;
	string					m_serverCountry;
	int						m_IPNum;

	PacketReduceState		m_packetReduceState;			// �ʥ]�I��A
	double					m_nextPacketReduceUpdateTime;	// �U����s�ɶ�
	int						m_packetReducePing;				// �W��Ping�ɶ�

	float					m_magicDistance;			// �I�k�Z��
	bool					m_displayMaintainServer;	// ��ܺ��פ����A��

	bool					m_isFirstLoading;			// �즸LOADING
	float					m_playerHeight;				// ���a�ҦbY�b����	
	UINT					m_checkPlayerHeight;		//
	float					m_playerCheckValue;			// �ˬdY�b�y�ЭȬO�_���T
	float					m_playerHeightErrorTime;	// �����y�п��~�ɶ�
	float					m_lastCheckHeightTime;		// �W���ˬd���I�y�бײv	
	CRuVector3				m_lastCheckHeightPosition;	// �W���ˬd�y���I
	int						m_illegalPositionCount;		// �ˬd�y���I���~����

	int						m_zoneDBID;					// ���ZONE,�@�묰�Գ���DBID
	set<int>				m_zoneOpen;					// �w�g�}�Ҫ��ϰ�
	
	bool					m_botLocked;				// �����H�欰�Q���
	
	float					m_instanceWaitTime;			// ���ݪ��a�M�w�O�_�P�N�ƥ��i��
	bool					m_requestInstance;			// ���s�n�D�ƥ��P�N
	
	DWORD					m_currentMemory;			// �ثe�O����ϥζq
	DWORD					m_memoryLoad;				// �O����ϥΤ��	
	double					m_lastMemoryUpdateTime;
	bool					m_reduceTextureState;		// ���C�����ӫת��A
	RuPropertyType			m_paperdollTextureDetail;	// �����l�K�Ϻ�ӫ�
	RuPropertyType			m_textureDetail;			// �@��K�Ϻ�ӫ�

	int						m_attachMountId;			// ���[����s��
	bool					m_isLoadClientData;			// �O�_��SERVER���J�s�ɸ��
	map<int, float>			m_sendKeyTime;				// �e�X���o�S�w�X�Юɶ�
	bool					m_inMount;					// �O�_�b����W
	bool					m_inImplement;				// �O�_�b����W
	float					m_delayGetImplementTime;	// ���ݮɶ�����k�N����

	bool					m_bIsVoiceSettingsDisable;	// �P�_�O�_���}��y���]�w
	map<int, int>			m_roomValue;				// �i�J�W�ߩж��S���Ѽ�

	ClientSaveSetting		m_clientSetting;			// �Ȥ�ݰO�����
	ClientSaveSetting		m_clientSettingBackup;		// �Ȥ�ݳ̫��x�s���
	double					m_lastClientSaveTime;		// �̫�����x�s�ɶ�
	bool					m_isFreeCamera;				// �۰ʲ������Y
	CRoleSprite*			m_cameraFollowSprite;		// ��w�������Y�b�t�@������W

	TargetLineEffectType	m_targetLineType;			// 
	CRoleSprite*			m_targetReferenceSprite;	// �ت��ѦҪ���
	CRuVector3				m_targetDirection;			// ���𨤫�
	float					m_targetParabolaHeight;		// �ߪ��u����
	float					m_targetMaxRange;			// �̻������Z��
	float					m_targetMaxAngle;			// �ت�����
	float					m_targetDisplayDuration;	// �̫���ܮɶ�

	// �󴫦a��
	vector<ZoneDescriptorCallback>	m_ZoneDescriptorList;
	map<int, CRuVector3>			m_spriteAnchorHeight;		// ���ݪ��a�����J�b���հ���


	SystemFunctionState		m_systemEnable;

	bool					m_movePartitionEnable;		// ���ʭ��s�]�w���ΰ�
	bool					m_vipDisplay;
	bool					m_vipValid;		
	bool					m_bMerchantFrame;
	bool					m_cameraControl;			// ��v���ഫ�� script �۰ʱ���

	UpdateInfInfo			m_updateInf;

	int						m_mountObjectId;
	float					m_mountHeight;
	bool					m_skipCastSpellCheck;	
	CRuVector3				m_lastFixedPosition;
	float					m_lastFixedPositionTime;

	//�į�O����
	vector<float>			m_Fpslog;
	vector<float>			m_Cpulog;
	vector<float>			m_Memlog;
	CpuUsage				m_CpuUsage;
	double					m_checkMemoryTime;
	bool					m_boReportSelf;
	DWORD					m_checkHeightCount;

	//ZShop
	bool					m_enableZShop;				//ZShop�}��
	string					m_ZShopURL;					//ZShop���}

	int						m_iWBWidth;
	int						m_iWBHeight;	

#ifdef KALYDO
	static void detectWDBParts(CRuDataStore_MultiFile* mapFileDataStore);

	void handleWDB(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength);

	void finishWDB();
	// A handler for the package result for downloading the WDB's.
	THandler* m_HandleWDBPackage;

	// A boolean whether or not we are downloading a WDB.
	bool					m_isDownloading;
	// A boolean whether or not the 'SetCameraPos' function has already been called;
	// as this function contains the loading of the WDB files.
	bool					m_shouldSetCameraPos;
	// The position given to the SetCameraPos function.
	CRuVector3				m_cameraPos;

	// The progress for the WDB download
	float					m_WDBDownloadProgress;
#endif // KALYDO
};

#endif //_GAMEMAIN_H