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

//¥i¥H®y¨ì´È¤lªº¶ZÂ÷
#define DF_SIT_TO_CHAIR_MAX_DISTANCE        20

// »y¨t¦r¦ê
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
#define LANGUAGE_CODE_ES					"ES"				// ¦è¯Z¤ú
#define LANGUAGE_CODE_ID					"ID"				// ¦L¥§
#define LANGUAGE_CODE_NL					"NL"				// ²üÄõ
#define LANGUAGE_CODE_RO					"RO"				// Ã¹°¨¥§¨È
#define LANGUAGE_CODE_PH					"PH"				// µá«ß»«
#define LANGUAGE_CODE_PL					"PL"				// ªiÄõ
#define LANGUAGE_CODE_TH					"TH"				// ®õ°ê
#define LANGUAGE_CODE_SAES					"SAES"				// ªü«n¬ü¬w¦è¯Z¤ú¤å
#define LANGUAGE_CODE_SAPT					"SAPT"				// ªü«n¬ü¬wµå¸²¤ú¤å
#define LANGUAGE_CODE_AE					"AE"				// ªü©Ô§BÁp¦X¤j¤½°ê
#define LANGUAGE_CODE_ENAR					"ENAR"				// ªü©Ô§BÁp¦X¤j¤½°ê(­^¤å)
#define LANGUAGE_CODE_AUS					"AUS"				// ¿D¦{
#define LANGUAGE_CODE_IT					"IT"				// ¸q¤j§Q
#define MAX_LANGUAGES						29

#define MAX_FOCUS_SPRITES					12
#define TOMB_MAP_ICON_ID					0x7f000000

#define LUA_PASSWORD_FUNC					"__uiLuaPassword_Callback__"
#define LUA_CHECKPLAYER_FUNC				"__uiLuaCheckPlayer_Callback__"

#define IS_DETECT_MEMORY_LEAKS_ENABLE 1 // ¬O§_°»´ú°O¾ÐÅé¬ªº|.

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
			bool	isUnholster				:1;		// true ¨ú¥XªZ¾¹
			bool	isWalk					:1;		// true ¨«¸ô
			bool	isCombat				:1;		// true ¾Ô°«¤¤
			bool	isDead					:1;		// true ¦º¤`¤¤
			bool	isSit					:1;		// true §¤¤U
		};
	}state;
}PLAYER_ACTOR_STATE;
*/

struct PetCommandStruct
{
	union {
		int _value;
		struct {
			bool		movable : 1;				// ²¾°Ê(¸òÀHª±®a)
			bool		strikeBack : 1;				// ªðÀ»
		};
	};
};

// Ãdª«°ò¥»¸ê®Æ
struct PetInfo
{
	int						GUID;					// 
	int						ItemID;					//
	int						MagicID;				//
	PetCommandStruct		Status;					// Ãdª«ª¬ºA
};

typedef void (*pfSpellRangeCallback) (int spellID, float x, float y, float z);

enum CASTSPELLERROR
{
	CASTSPELL_ERROR_EXIST				= -1,			// ¤£¦s¦b
	CASTSPELL_ERROR_ACTIVE				= -2,			// ³Q°Ê§Þ¯à¤£¯à¬Iªk
	CASTSPELL_ERROR_CLASS				= -3,			// Â¾·~»~¿ù,«D¥»Â¾·~
	CASTSPELL_ERROR_HP					= -4,			// ¥Í©R­È¤£¨¬
	CASTSPELL_ERROR_MP					= -5,			// Å]¤O­È¤£¨¬
	CASTSPELL_ERROR_SP_WARRIOR			= -6,			// warrior sp ¤£¨¬
	CASTSPELL_ERROR_SP_RANGER			= -7,			// Ranger sp ¤£¨¬
	CASTSPELL_ERROR_SP_ROGUE			= -8,			// Rogue sp ¤£¨¬
	CASTSPELL_ERROR_GUN					= -9,			// »Ý¨Dºj
	CASTSPELL_ERROR_AMMO				= -10,			// ¼uÃÄ¤£¨¬
	CASTSPELL_ERROR_BOW					= -11,			// »Ý¨D¤}
	CASTSPELL_ERROR_ARROW				= -12,			// ½b¥Ú¤£¨¬
	CASTSPELL_ERROR_ITEM				= -13,			// ª««~»Ý¨D¤£¨¬
	CASTSPELL_ERROR_COLDOWN				= -14,			// §Nƒj®É¶¡
	CASTSPELL_ERROR_TARGET				= -15,			// »Ý­n¤@­Ó¥Ø¼Ð
	CASTSPELL_ERROR_DITANCE				= -16,			// ¶ZÂ÷¹L»·
	CASTSPELL_ERROR_MOVE				= -17,			// ¤£¯à²¾°Ê¬Iªk
	CASTSPELL_ERROR_DISABLEJOBSKILL		= -18,			// µLªk¨Ï¥ÎÂ¾·~§Þ¯à
	CASTSPELL_ERROR_LEVEL				= -19,			// µ¥¯Å¤£¨¬
	CASTSPELL_ERROR_CANTCTRL			= -20,			// µLªk±±¨î (¤ß±±¤¤

	//»Ý¨D
	CASTSPELL_ERROR_NEED_WEAPON			= -100,			//»Ý­n¬YªZ¾¹
	CASTSPELL_ERROR_NEED_EQ				= -101,			//»Ý­n¬Y¸Ë³Æ
	CASTSPELL_ERROR_NEED_SUIT			= -102,			//»Ý­n¬Y®M¸Ë
	CASTSPELL_ERROR_NEED_BUFF			= -103,			//»Ý­n¬YBuff	
	CASTSPELL_ERROR_NEED_NOBUFF			= -104,			//¤£¯à¦³¬YBuff
	CASTSPELL_ERROR_NEED_UNARMED		= -105,			//»Ý­nªÅ¤â
	CASTSPELL_ERROR_NEED_BLADE			= -106,			//»Ý­n¸Ë¼C
	CASTSPELL_ERROR_NEED_DAGGER			= -107,			//»Ý­n¸Ë¤P­º
	CASTSPELL_ERROR_NEED_WAND			= -108,			//»Ý­n¸Ë¤â§ú
	CASTSPELL_ERROR_NEED_AXE			= -109,			//»Ý­n¸Ë©òÀY
	CASTSPELL_ERROR_NEED_BLUDGEON		= -110,			//»Ý­n¸Ë³æ¤âÂñ
	CASTSPELL_ERROR_NEED_CLAYMORE		= -111,			//»Ý­n¸ËÂù¤â¼C
	CASTSPELL_ERROR_NEED_STAFF			= -112,			//»Ý­n¸ËÂù¤â§ú
	CASTSPELL_ERROR_NEED_2H_AXE			= -113,			//»Ý­n¸ËÂù¤â©ò
	CASTSPELL_ERROR_NEED_2H_HAMMER		= -114,			//»Ý­n¸ËÂù¤âÂñ
	CASTSPELL_ERROR_NEED_POLEARM		= -115,			//»Ý­n¸Ëªø¥Ù
	CASTSPELL_ERROR_NEED_SWORD_TYPE		= -116,			//»Ý­n¸Ë¼CÃþªZ¾¹
	CASTSPELL_ERROR_NEED_AXE_TYPE		= -117,			//»Ý­n¸Ë©òÃþªZ¾¹
	CASTSPELL_ERROR_NEED_SHIELD			= -118,			//»Ý­n¸Ë¬Þ
	CASTSPELL_ERROR_NEED_HP_LOWER		= -119,			//»Ý­nHP§C¤@¨Ç
	CASTSPELL_ERROR_NEED_HP_HIGHER		= -120,			//»Ý­nHP°ª¤@¨Ç
	CASTSPELL_ERROR_NEED_JOB			= -121,			//Â¾·~¦³°ÝÃD
	CASTSPELL_ERROR_NEED_LONGWEAPON		= -122,			//»Ý­n¸Ëªø¶ZÂ÷ªZ¾¹
	CASTSPELL_ERROR_NEED_HAMMER_TYPE	= -123,			//»Ý­n¸ËÂñÃþªZ¾¹
	CASTSPELL_ERROR_NEED_2H_WEAPON_TYPE	= -124,			//»Ý­n¸ËÂù¤âªZ¾¹
	CASTSPELL_ERROR_NEED_BUFF_GROUP		= -125,			//»Ý­n¬Y¤@ºØÃþªºBUFF

	CASTSPELL_ERROR_NEED_CRITICAL		= -126,			//­P©R¤@À»«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_BECRITICAL		= -127,			//³Q¤H­P©R¤@À»«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_DODGE			= -128,			//°{Á×«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_MISS			= -129,			//MISS«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_PARRY			= -130,			//¹jÀÉ«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_BEPARRY		= -131,			//³Q¤H¹jÀÉ«á¤~¥i¨Ï¥Î
	CASTSPELL_ERROR_NEED_BEDODGE		= -132,			//³Q¤H°{Á×«á¤~¥i¨Ï¥Î

	CASTSPELL_ERROR_NEED_COMBAT			= -133,			//»Ý­n¾Ô°«¤¤
	CASTSPELL_ERROR_NEED_NOCOMBAT		= -134,			//«D¾Ô°«¤¤
	CASTSPELL_ERROR_NEED_DITANCE		= -135,			//¶ZÂ÷¹LªñµLªk¬Iªk

	CASTSPELL_ERROR_TARGET_NEED_BUFF		= -136,		//¹ï¶H¥Ø¼Ð»Ý­n¬YBUFF
	CASTSPELL_ERROR_TARGET_NEED_NOBUFF		= -137,		//¹ï¶H¥Ø¼Ð»Ý­n¬YBUFF
	CASTSPELL_ERROR_TARGET_NEED_NOCOMBAT	= -138,		//¹ï¶H¥Ø¼Ð»Ý­n¾Ô°«¤¤
	CASTSPELL_ERROR_TARGET_NEED_COMBAT		= -139,		//¹ï¶H¥Ø¼Ð«D¾Ô°«¤¤	
};

enum ItemCastingType
{
	ITEM_CASTING_NONE					= 0,
	ITEM_CASTING_USEITEM				= 1,			// ¨Ï¥Î­I¥]¤ºª««~
	ITEM_CASTING_DISSOLUTION			= 2,			// ¨Ï¥Î¤À¸Ñ§Þ¯à[¯S®í]
	ITEM_CASTING_PARTNER		        = 3,            //©I¥s§Ö¼Öªº¹Ù¦ñ

	// ¥H¤UÃþ«¬¥i¯àµL°µ¥Î
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
	PACKET_REDUCE_NONE					= 0,			// ¨S¦³°µ¥Î(¥¼°µ«Ê¥]°I´î)
	PACKET_REDUCE_PLAYER_MOVE			= 1,			// ²¾°£ª±®a²¾°Ê«Ê¥](ÁÙ¬O¦³©w¦ì«Ê¥])
	PACKET_REDUCE_PLAYER_MOVE_ALL		= 2,			// ²¾°£ª±®a©Ò¦³²¾°Ê«Ê¥](¤]¨S¦³©w¦ì«Ê¥],¬Ý¨ì¨ä¥Lª±®aÀ³¸Ó·|¬O¥Î¸õªº)
	PACKET_REDUCE_MOVE_ALL				= 3,			// ²¾°£©Ò¦³²¾°Ê«Ê¥](NPC»Pª±®a)
};

enum TargetLineEffectType
{	
	TargetLineEffect_None				= 0,
	TargetLineEffect_Line				= 1,			// ½u¬q
	TargetLineEffect_Parabola			= 2,			// ©ßª«½u
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

// «O¯dCLIENTºÝ³]©w¸ê°T
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
			bool			isUseServerData					:1;			// ¬O§_¶}±Ò¦øªA¾¹Àx¦s¸ê®Æ
			bool			isNPCTitleVisible				:1;			// Åã¥ÜNPCª«¥óÀY¹³¦WºÙ
			bool			isPlayerTitleVisible			:1;			// Åã¥Üª±®a¨¤¦âÀY¹³¦WºÙ
			bool			isAllSCTVisible					:1;			// Åã¥Ü©Ò¦³§ðÀ»¼Æ­È
			bool			isMouseMoveEnable				:1;			// ¬O§_¶}±Ò·Æ¹«²¾°Ê
			bool            isDisableTitleHide				:1;			// ¦bºÙ¸¹¸Ë³Æ¤¶­±¤¤ ¤£¥i¸Ë³ÆªººÙ¸¹­n¤£­nÅã¥Ü
			bool			isSelfCast						:1;			// ±Ò°Ê¦Û§Ú¬Iªk(·íªk³NµL»Ý­n¥Ø¼Ð®É)			
			bool			isSelfTitleVisible				:1;			// Åã¥Ü¦Û¤vÀY¹³¦WºÙ
			bool			isLastSelectedRealm				:1;			// ¨Ï¥Î³Ì«á¿ï¾Üªº¦øªA¾¹µn¤J
			bool			isLastAccountName				:1;			// °O¿ý³Ì«áµn¤J±b¸¹
			bool			isShowGemePromrt				:1;			// Åã¥Ü¹CÀ¸´£¥Ü
			bool			isNPCRealHealth					:1;			// Åã¥Ü«Dª±®a¤Íµ½Ãþ«¬ªº¦å¶q
			bool			isTitleVisible					:1;			// Åã¥ÜºÙ¸¹
			bool			isGuildVisible					:1;			// Åã¥Ü¤½·|
			bool			isTitleIconVisible				:1;			// Åã¥Ü¨¤¦âÂ¾·~»Pµ½´c­ÈICON
			bool			isCameraYReverse				:1;			// ÃèÀYY¶b¤ÏÂà
			bool			isLButtonCancelTarget			:1;			// ·Æ¹«¥ªÁä¨ú®ø¥Ø¼Ð
			bool			isRBuutonCancelTarget			:1;			// ·Æ¹«¥kÁä¨ú®ø¥Ø¼Ð
			bool			isLButtonCameraRotateEnable		:1;			// ·Æ¹«¥ªÁäÃèÀY±ÛÂà
			bool			isCameraFollowEnable			:1;			// §ÀÀHÃèÀY
			bool			isDisableDisplayNPCTalk			:1;			// ¤£Åã¥ÜNPC¥æ½Í°T®§
			bool			isCameraSelectTarget			:1;			// ¨Ï¥ÎÃèÀY§¤¼ÐÂI¿ï¨ú¨¤¦â
			bool			isPlayerLootMotion				:1;			// ¼·©ñ¬B¨ú°Ê§@
			bool			isAutoTakeLoot					:1;			// ¦Û°Ê¬B¨ú
			bool			isChatDisplayClassColor			:1;			// ¥æ½ÍÂ¾·~Åã¥ÜÃC¦â
			bool			isAggroPrompt					:1;			// ¤³«ëÄµ§i´£¥Ü
			bool			isAutoOpenGoodsPack				:1;			// ¦Û°Ê¶}±Ò°Ó«°­I¥]
			bool			isRequestServerSaveData			:1;			// ¸ß°Ý¬O§_¥Ñ¦øªA¾¹°O¿ý¸ê®Æ
			bool			isHideOtherPlayerEffect			:1;			// ÁôÂÃ¨ä¥Lª±®a¯S®Ä
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
			char				loginEpisode				: 5;		// ²Ä´X³¹(0, 1³£¥Nªí²Ä¤@³¹)
		};
	};
};

struct EilteBellStruct 
{
	float m_destroyTime;			// ®ø¥¢®É¶¡
	int m_GItemID;					// °Ï°ì½s¸¹
}; 

struct AttachObjectInfo
{
	int sourceGItemID;				// ¨Ó·½ Server GItem
	int targetGItemID;				// ¥Ø¼Ð Server GItem
	int AttachType;					// ³s±µ¼Ò¦¡
	string sourceLinePoint;			// ¨Ó·½°Ñ¦ÒÂI
	string targetLinePoint;			// ¥Ø¼Ð°Ñ¦ÒÂI
};

//-----------------------------------------------------------------------------
// Ä²µo¨Æ¥ó
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

	virtual	void				SendScrollBannerMessage(const char* msg);			//¶]°¨¿O
	virtual void				SendScrollBannerMessageEx(const char* msg, ...);
	virtual void				SendMessageDialog(const char* msg);				    //°T®§¹ï¸Ü²° ¤@¬q°T®§ ¤@­ÓÃö³¬«ö¶s
	virtual void				SendMessageDialogEx(const char* msg, ...);

	virtual void				SendPlayTimeQuataNotify(const char* msg);		//¨¾¨I°g³qª¾¥Îpop dialog, ¤@¤ÀÄÁ¦Û°ÊÃö³¬, µL¥ô¦ó«ö¶s

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

	// ¨ú±o¹CÀ¸®É¶¡(0 ~ 239) 240 = ¹CÀ¸1¤Ñ
	virtual	int					GetCurrentGameTime()					{ return m_currentGameTime; }
	virtual time_t				GetGameTime()							{ return m_gameTime + (time_t)(m_frameTime - m_startFrameTime); }
	virtual time_t				GetServerTimeZone()						{ return m_serverTimeZone; }
	virtual	void				SetPlayerCameraPos(CRuVector3& pos);	

	// ¬IªkÀË¬d
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

	// §ó´«¤½·|¦WºÙ
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

	// ¨ú±oª±®aPVPª¬ºA
	virtual bool				GetSpritePVPState(CRoleSprite* sprite);

	// °Ï°ìPK
	virtual void				UpdatePVPZoneState(int state = -1);
	// ³]©wª±®aPVPºX¼Ð(BUFF ²£¥Í)	
	virtual void				UpdateSpritePKBuff();
	// °}Àç§ïÅÜ
	virtual void				SetSpriteCampId(CRoleSprite* sprite, int campId, bool force=false);

	// ­«³]ª±®a­I«áºX¤lÅã¹³ª«¥ó
	virtual void				ResetObjectGuildFlag(int guildID, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor);

	// ¨ú±o¤G¦¸±K½X	
	virtual bool				CheckSecondPassword();
	
	virtual const char*			GetSecondPassword()						{ return m_secondPassword.c_str(); }
	virtual void				SetSecondPassword(const char* password);
	virtual void				SendSecondPassword(const char* password);
	virtual void				ClearSecondPassword();

	// ³]©wÃö«Yªí
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

	// ¤ñ¹ï¬Û¦P¦WºÙª±®a
	void						ComparePlayerName();	

	// ³]©w®É¶¡
	void						SetTimeGroupType(int groupType)			{ m_timeGroupType = groupType; }
	
	// ¹Ó¸Oª«¥ó
	void						SetTombInfo(int zoneId, CRuVector3 position, float leftTime, int debtExp, int debtTp, int exp);
	void						ClearTomb();
	void						UpdateTomb();

	// Ãdª«
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

	// ºë­^Äµ§i
	void						AddEliteBell(int GItemID);
	void						UpdateEliteBell();


	// ª««~«áÄò°õ¦æ¤º®e
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

	// ¯S®í¨Æ¥ó
	string						GetSpecialEventString(int id);	

	// °Ï°ì¸ê°T
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

	// ¾Ô³õ
	void						ClearAllBattleMember();
	bool						AddBattleMember(int GItemID, CRoleSprite* sprite);
	bool						DeleteBettleMember(int GItemID);
	CRoleSprite*				FindBettleMemberByGItemID(int GItemID);
	CRoleSprite*				FindBettleMemberByDBID(int DBID);
	CRoleSprite*				FindBettleMemberByWorldID(int WorldID, const char* playerName);
	CRoleSprite*				FindBattleMemberByName(const char* playerName);
	int							GetBattleGroundNumPlayers()				{ return m_numBattleGroundPlayers; }

	// ±b±K
	void						SetAccountName(const char* account)				{ m_account = account; }
	void						SetAccountPassword(const char* password)		{ m_password = password; }
	const char*					GetAccountName()								{ return m_account.c_str(); }
	const char*					GetAccountPassword()							{ return m_password.c_str(); }

	// Ãö³sª«¥ó
	void						AddAttachObj(int source, int target, int mode, const char* sourceLink, const char* targetLink);
	void						DeleteAttachObj(int GItemID);
	void						ClearAttachObj();
	void						UpdateAttachObj();

	// °O¿ýª±®a­Ó¼Æ
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

	// ¬O§_¦b¤ô¤¤
	bool						IsInWater();

	// ³]©w¤À¬y­Ó¼Æ
	void						SetZoneChannel(int ZoneID, bool boOpen);
	set<int>&					GetZoneChannel()						{ return m_zoneOpen; }
	
	void						SetBotLocked(bool state);
	bool						GetBotLocked()							{ return m_botLocked; }

	// ²M°£°Æ¥»°T°Ý¶i«×
	void						RequestInstanceRecord();
	void						ClearInstanceWaitTime();
	void						AcceptInstanceRecord();
	void						DeclineInstanceRecord();

	void						CheckMemoryStatus(bool forceUpdate=false);
	DWORD						GetMemoryLoad()							{ return m_memoryLoad; }
	DWORD						GetCurrentMemory()						{ return m_currentMemory; }

	// ³sµ²ª«¥ó
	void						AttachPoint(CRuVector3 position, CRuVector3 dir, CRuFusion_ActorState actorState);

	//§¤¨ìEntity ¤W ¦pªG¥L¦³ p_sit_chair01 °Ñ¦ÒÂI
	bool						SitToEntity( CRuEntity* pEntity );

	//Entity ¬O¤£¬O´È¤l ¦pªG¥L¦³ p_sit_chair01 °Ñ¦ÒÂI
	//poutDistance ª±®a¨ì°Ñ¦ÒÂIªº¶ZÂ÷
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

	//±K½Xª¬ºA¬ÛÃö
	bool						GetIsMD5Password()									{return m_IsMD5Password;}
	bool						GetIsAutoHashPassword()								{return m_IsAutoHashPassword;}
	int							GetPasswordHashType()								{return m_PasswordHashType;}

	void						SkipCastSpellCheck(bool isSkip)						{ m_skipCastSpellCheck = isSkip; }

	//¨ä¥L
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
	int							m_currentGameTime;				// ¥Ø«e¹CÀ¸®É¶¡
	int							m_gameTime;						// ¹CÀ¸®É¶¡
	int							m_serverTimeZone;				// server ®É°Ï®É¶¡	

	double						m_startFrameTime;				// ¶}©l­p®É®É¶¡

	string						m_StringLanguage;				
	string						m_DataLanguage;		

	string						m_playerName;					// ª±®a¦WºÙ
	string						m_zoneName;						// °Ï°ì¦WºÙ
	string						m_zoneName2;					// °Ï°ì¦WºÙ2
	int							m_WorldMapID;					// ¦a¹Ïid(UI¥Î)
	int							m_playerID;						// ª±®a¨¤¦â½s¸¹(¥@¬É½s¸¹)
	int							m_GItemID;						// Àx¦s¨¤¦â½s¸¹(°Ï°ì½s¸¹)
	int							m_DBID;							// SERVER¨¤¦â°ß¤@¥N½X
	int							m_zoneID;						// °Ï°ì¦WºÙ
	int							m_roomID;						// ©Ð¶¡¸¹½X
	string						m_mapName;						// ¦a¹Ï¦WºÙ	
	bool						m_receiveWaitLoading;			// µ¥«Ý¸ü¤J¤¤
	int							m_changeTerrainStep;			// §ó´«¦a¹Ï
	bool						m_isEnterWorld;					// ¬O§_¶i¤J¹CÀ¸¥@¬É
	bool						m_isFirstViewpoint;				// ²Ä¤@¤HºÙµø¨¤	
	bool						m_isCombatState;				// ¶i¤J§ðÀ»ª¬ºµ,¦¹ª¬ºA¬°±µ¦¬SERVER°e¥X°T®§
	bool						m_isPVE;						// PVE ª¬ºA
	int							m_playerRefCount;				// ª±®a­p¼Æ¾¹
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
	bool						m_cursorHyperlink;				// ¬O§_¦bHyperlink¤¸¥ó¤W
	string						m_dragItemStr;
	
	bool						m_selfRevive;
	bool						m_resurrect;
	float						m_resurrectTime;
	
	eCastSpellState				m_castSpellState;				// ¬Iªkª¬ºA
	int							m_castSpellID;					// ¬Iªk½s¸¹
	int							m_castSpellIndex;				// ¬Iªk¯Á¤Þ½s¸¹(¤@¯ë¬°-1,§_«h«h¥Nªí¦bµ¥«ÝÂI¿ï¥Ø¼Ð)
	int							m_castSpellToItem;				// ¹ïª««~¨Ï¥Î	
	int							m_sendCastSpellCount;			// ¤w¸g°e¥Xªº¬Iªk­Ó¼Æ
	bool						m_isCancelCastSpell;			// ¨ú®ø¬Iªk
	double						m_lastCastSpellTime;			// ³Ì«á°e¥Xªº¬Iªk®É¶¡

	CRoleSprite*				m_sendSpellTarget;				// °e¥Xªº¬Iªk¹ï¶H
	int							m_sendSpellMagicID;				// °e¥Xªº¬Iªk½s¸¹

	bool						m_IsChatLog;
	char						m_version[256];                 // ²{¦bClientºÝªºª©¥»
	char						m_versionNO[256];               // ²{¦bClientºÝdataªºª©¥» 
	string                      m_versionURL;                   // §ó·s¦øªA¾¹¤WVersion.TxtªºURL
	string						m_noticeURL;					// ¤½§i¸ê°T
	string						m_activateURL;					// K2 ¿E¬¡±b¸¹
	string						m_activateURLArg;				// °Ñ¼Æ
	int                         m_downloadPort;                 // §ó·s¦øªA¾¹http port

	// ¦aªí
	bool						m_needLoadTerrain;				// »Ý­n­«·s¸ü¤J¦a¹Ï
	float						m_loadTerrainDelayTime;			// ¤@¯ë¸ü¤J¦a¹Ï®É¶¡¹Lªø,¥ýÅýLOADINGµe­±¥X²{¦b°µ¸ü¤J°Ê§@
	float						m_sendLoadOkDelayTime;			// °e¦^SERVER¶}©l¥[¤Jª«¥ó©µ¿ð®É¶¡

	// Åã¥ÜSERVER¨Óªº°T®§
	bool						m_displaySrvDebugMsg;

	bool						m_IsMD5Password;				//µn¤J®É, ¨Ï¥ÎªÌ¿é¤Jªº±K½X¬O§_¬°MD5 Hash¹Lªº±K½X
	bool						m_IsAutoHashPassword;			//¦Û°ÊHash¨Ï¥ÎªÌ±K½XºX¼Ð
	int							m_PasswordHashType;				//Hash±K½X¤è¦¡ 0=md5, 1=bcrypt ¦Û°ÊHash¨Ï¥ÎªÌ±K½XºX¼Ð¶}±Ò®É¦³®Ä

	// ²¾°ÊºX¼Ð
	union {
		int Bytes;
		struct {
			bool			forward			:1;			// ¦V«e²¾°Ê
			bool			backward		:1;			// ¦V«á²¾°Ê
			bool			strafeRight		:1;			// ¦V¥k²¾°Ê
			bool			strafeLeft		:1;			// ¦V¥ª²¾°Ê
			bool			turnRight		:1;			// ­ì¦a¦V¥kÂà¦V
			bool			turnLeft		:1;			// ­ì¦a¦V¥ªÂà¦V
			bool			autoRun			:1;			// ¦Û°Ê¦V«e¶]
			bool			walk			:1;			// ¨«¸ô
		};
	}m_moveState;

	// ®£Äß
	struct {
		int					dir;						// ©¹«e¤è¦V
		float				time;						// §ó·s®É¶¡
	}m_fear;

	// ·Æ¹«²¾°Ê
	struct {
		CRuVector3			target;
		CRoleSprite*		sprite;
		bool				enable;
		float				update;
		float				distance;
	}m_mouseMove;

	// ¸ô®|²¾°Ê	

	// ¸òÀH
	struct {
		CRoleSprite*		target;						// ¸òÀH¹ï¶H
		float				touchRange;					// §@¥Î¶ZÂ÷
		float				update;						// ­«·s¨ú±o®y¼Ð©µ¿ð®É¶¡
		float				distance;					// ¤W¦¸§ó·s»P¥Ø¼ÐÂI¶ZÂ÷­È
	}m_follow;	

	// °O¿ýª««~¹ïª««~¨Ï¥Î¤¤(»Ý­n¶]®É¶¡¶b,¦³µ¥«Ý®É¶¡)
	struct {
		ItemCastingType		type;						// ¨Ó·½ª««~¨Ï¥ÎÃþ«¬
		int					bagPos;						// ­I¥]¦ì¸m
		int					destType;					// ¥ØªºÃþ«¬ 0­I¥] 1¸Ë³Æ 2»È¦æ
		int					destPos;					// ¥ØªºÃþ«¬©Ò¦b¦ì¸m
		int					actor;						// °Ê§@¥N½X
		bool				hasEffect;					// ¬O§_¦³¯S®Ä²£¥Í
	}m_itemCasting;

	vector<CRuAlgorithm_PatternMatch_FJS*> m_patternMatch;

	CACTActionEventsManager	m_ACTActionEventsManager;	
	float					m_worldCameraFarPlane;
	int						m_PVPZoneState;				// °Ï°ìPKºX¼Ð

	bool					m_campState;				// µn¥Xª¬ºA(true µn¥X¤¤)
	float					m_campUpdateTime;			// Ä²µo®É¶¡
	int						m_parallelID;				// ¤À¬yª¬ºA	

	float					m_totalFps;					// Á`±i¼Æ
	float					m_totalPing;				// Á`¦XPing
	double					m_totalLastTime;			// ³Ì«á§ó·s®É¶¡
	int						m_totalCount;				// ¤W¶Ç¸ê®Æ­p¼Æ¾¹

	CRuVector3					m_rightPosition;		// ³Ì«á½T»{®y¼ÐÂI
	CRuArrayList<CRuVector3>	m_obstructPosition;		// ¹J¨ìªýÀÉªº®y¼ÐÂI

	string					m_secondPassword;			// ¤G¦¸±K½X
	bool					m_sendPassword;

	string					m_install;					// ¦w¸Ë°ê®a¥N½X
	string					m_country;					// °ê®a¥N½X
	string					m_language;					// »y¨¥¥N½X
	string					m_saveFolder;				// Àx¦s¥Ø¿ý¦WºÙ
	int						m_campTableId;				// Ãö«Yªí

	vector<CRuVector3>		m_pathPositions;			// ¸ô®|¸ê®Æ
	CRuVector3				m_pathTargetPosition;		// ¸ô®|²¾°Ê¥Ø¼ÐÂI	
	float					m_pathMoveTime;				// ¸ô®|²¾°Ê¨ì‘´¥Ø¼Ð©Ò»Ý®É¶¡	
	float					m_pathMoveUpdate;			// ¸ô®|²¾°Ê§ó·s®É¶¡

	int						m_resetFontIndexStep;		// §ó·s¦r«¬
 
	set<int>				m_MoveToScreenPointSpriteIDs; //MoveToScreenPoint®É ¤]»Ý­nÀË´ú¦³¨S¦³ÂI¨ìªºSpriteID

	CRoleSprite*			m_focusSprites[MAX_FOCUS_SPRITES];	

	CRuVector3				m_cameraCurrentVector;		// Äá¼v¾÷¥Ø«e¤è¦V
	CRuVector3				m_cameraFinalVector;		// ³Ì«áÄá¼v¾÷´Â¦V¤è¦V

	int						m_timeGroupType;			// ³]©w®É¶¡Ãþ«¬(1~4)	

	CRuVector3				m_tombPosition;				// ®y¼Ð¦ì¸m
	float					m_tombLeftTime;				// ³Ñ©ó®É¶¡ ( > 0 ) ¦s¦b
	int						m_tombZoneId;				// °Ï°ì½s¸¹
	int						m_tombDebtExp;				// ­t¶Å¸gÅç­È
	int						m_tombDebtTp;				// ­t¶Å§Þ¯àÂI¼Æ
	int						m_tombExp;					// ³Ñ©ó¸gÅç­È
	
	// Ãdª«¸ê°T
	int						m_petGUID;					// 
	int						m_petItemID;				//
	int						m_petMagicID;				//
	PetCommandStruct		m_petStatus;				// Ãdª«ª¬ºA
	map<int, PetInfo>		m_petInfo;

	bool					m_isWorldBattleGround;		// ¥@¬É¾Ô³õ
	bool					m_isBattleGround;			// ¬O§_¦b¾Ô³õ¤¤
	bool					m_isDisableTrdae;			// ¤£¯à¥æ©ö
	bool					m_isDisableDuel;			// ¤£¯à¤ñ¸Õ
	bool					m_isDisableParty;			// ¤£¯àÁÜ½Ð²Õ¶¤
	bool					m_isEnableBGParty;			// ¶}±Ò¾Ô³õ¤¤²Õ¶¤
	bool					m_inWater;					// ¬O§_¦b¤ô¤¤
	bool					m_isNoSwimming;				// °Ï°ì¤£¯à´åªa
	int						m_numBattleGroundPlayers;	// ¾Ô³õ³Ì¤j¤H¼Æ³]©w

	int						m_cycleMagicId;				// ³sÄò¬I®iªk³N½s¸¹
	int						m_cycleMagicPos;			// ³sÄò¬I®iªk³N®Ñ¦ì¸m	

	CRuRendererPlugin_SubmergeFX*	m_submergeFX;		// Submerge FX plugin

	map<CRoleSprite*, CRuVector3>	m_partitionPosition;

	bool					m_isWaitGlobalObject;		// µ¥«Ý¥þ°ìª«¥ó¸ü¤J
	vector<int>				m_globalObject;				// ¥þ°ìª«¥ó

	bool					m_showUpdateElapsedTime;	// Åã¥Ü¨C¦¸Frame§ó·s®É¶¡

	map<int, float>			m_eliteBell;				// ºë­^´£¥ÜÄµ§i

	int						m_playTimeQuota;			// ¨¾¨I°g°T®§
	float					m_QuotaUpdateTime;

	map<int, ItemCommand>	m_itemCommand;
	int						m_lastItemCommandID;		// ³Ì«á¤@­Ó°õ¦æ½s¸¹

	string					m_localIP;					// ¥Ñ Server ¶Ç¦^ªº¥»¦a Client IP
	bool					m_isReloadClientLua;			// ­«·s¸ü¤J

	map<int, string>		m_specialEventMap;			// ¯S®í¦r¦ê¯Á¤Þ

	map<int, CRoleSprite*>	m_BattleMember;				// ¾Ô³õ¯S®íª±®a¸ê®Æ

	string					m_account;					// ¾ï¤l¨t²Î¥Ñ¥~³¡¶Ç¤J±b±K
	string					m_password;					// ¾ï¤l¨t²Î¥Ñ¥~³¡¶Ç¤J±b±K	

	vector<AttachObjectInfo>	m_attachObject;

	double					m_lastDummySpriteUpdateTime;
	string					m_serverCountry;
	int						m_IPNum;

	PacketReduceState		m_packetReduceState;			// «Ê¥]°I´îª¬ºA
	double					m_nextPacketReduceUpdateTime;	// ¤U¦¸§ó·s®É¶¡
	int						m_packetReducePing;				// ¤W¦¸Ping®É¶¡

	float					m_magicDistance;			// ¬Iªk¶ZÂ÷
	bool					m_displayMaintainServer;	// Åã¥Üºû­×¤¤¦øªA¾¹

	bool					m_isFirstLoading;			// ªì¦¸LOADING
	float					m_playerHeight;				// ª±®a©Ò¦bY¶b°ª«×	
	UINT					m_checkPlayerHeight;		//
	float					m_playerCheckValue;			// ÀË¬dY¶b®y¼Ð­È¬O§_¥¿½T
	float					m_playerHeightErrorTime;	// ª±¨¤®y¼Ð¿ù»~®É¶¡
	float					m_lastCheckHeightTime;		// ¤W¦¸ÀË¬d¬ÛÂI®y¼Ð±×²v	
	CRuVector3				m_lastCheckHeightPosition;	// ¤W¦¸ÀË¬d®y¼ÐÂI
	int						m_illegalPositionCount;		// ÀË¬d®y¼ÐÂI¿ù»~¦¸¼Æ

	int						m_zoneDBID;					// ¸ó°ÏZONE,¤@¯ë¬°¾Ô³õ¤ºDBID
	set<int>				m_zoneOpen;					// ¤w¸g¶}±Òªº°Ï°ì
	
	bool					m_botLocked;				// ¾÷¾¹¤H¦æ¬°³QÂê¦í
	
	float					m_instanceWaitTime;			// µ¥«Ýª±®a¨M©w¬O§_¦P·N°Æ¥»¶i«×
	bool					m_requestInstance;			// ­«·s­n¨D°Æ¥»¦P·N
	
	DWORD					m_currentMemory;			// ¥Ø«e°O¾ÐÅé¨Ï¥Î¶q
	DWORD					m_memoryLoad;				// °O¾ÐÅé¨Ï¥Î¤ñ¨Ò	
	double					m_lastMemoryUpdateTime;
	bool					m_reduceTextureState;		// ­°§C§÷½èºë²Ó«×ª¬ºA
	RuPropertyType			m_paperdollTextureDetail;	// ¨¤¦â­ì©l¶K¹Ïºë²Ó«×
	RuPropertyType			m_textureDetail;			// ¤@¯ë¶K¹Ïºë²Ó«×

	int						m_attachMountId;			// ªþ¥[ª«¥ó½s¸¹
	bool					m_isLoadClientData;			// ¬O§_¥ÑSERVER¸ü¤J¦sÀÉ¸ê®Æ
	map<int, float>			m_sendKeyTime;				// °e¥X¨ú±o¯S©wºX¼Ð®É¶¡
	bool					m_inMount;					// ¬O§_¦b¸ü¨ã¤W
	bool					m_inImplement;				// ¬O§_¦b¸ü¨ã¤W
	float					m_delayGetImplementTime;	// µ¥«Ý®É¶¡§ì¨úªk³NÃþ«¬

	bool					m_bIsVoiceSettingsDisable;	// §PÂ_¬O§_¤£¶}©ñ»y­µ³]©w
	map<int, int>			m_roomValue;				// ¶i¤J¿W¥ß©Ð¶¡¯S¦³°Ñ¼Æ

	ClientSaveSetting		m_clientSetting;			// «È¤áºÝ°O¿ý¸ê®Æ
	ClientSaveSetting		m_clientSettingBackup;		// «È¤áºÝ³Ì«áÀx¦s¸ê®Æ
	double					m_lastClientSaveTime;		// ³Ì«á§ïÅÜÀx¦s®É¶¡
	bool					m_isFreeCamera;				// ¦Û°Ê²¾°ÊÃèÀY
	CRoleSprite*			m_cameraFollowSprite;		// Âê©w¨¤¦âÃèÀY¦b¥t¤@¨¤ª«¥ó¤W

	TargetLineEffectType	m_targetLineType;			// 
	CRoleSprite*			m_targetReferenceSprite;	// ¥Øªº°Ñ¦Òª«¥ó
	CRuVector3				m_targetDirection;			// ¯¥¶ð¨¤«×
	float					m_targetParabolaHeight;		// ©ßª«½u°ª«×
	float					m_targetMaxRange;			// ³Ì»·§ðÀ»¶ZÂ÷
	float					m_targetMaxAngle;			// ¥Øªº¨¤«×
	float					m_targetDisplayDuration;	// ³Ì«áÅã¥Ü®É¶¡

	// §ó´«¦a¹Ï
	vector<ZoneDescriptorCallback>	m_ZoneDescriptorList;
	map<int, CRuVector3>			m_spriteAnchorHeight;		// µ¥«Ýª±®a³£¸ü¤J¦b´ú¸Õ°ª«×


	SystemFunctionState		m_systemEnable;

	bool					m_movePartitionEnable;		// ²¾°Ê­«·s³]©w¤À³Î°Ï
	bool					m_vipDisplay;
	bool					m_vipValid;		
	bool					m_bMerchantFrame;
	bool					m_cameraControl;			// Äá¼v¾÷Âà´«¬° script ¦Û°Ê±±¨î

	UpdateInfInfo			m_updateInf;

	int						m_mountObjectId;
	float					m_mountHeight;
	bool					m_skipCastSpellCheck;	
	CRuVector3				m_lastFixedPosition;
	float					m_lastFixedPositionTime;

	//®Ä¯à°O¿ý¥Î
	vector<float>			m_Fpslog;
	vector<float>			m_Cpulog;
	vector<float>			m_Memlog;
	CpuUsage				m_CpuUsage;
	double					m_checkMemoryTime;
	bool					m_boReportSelf;
	DWORD					m_checkHeightCount;

	//ZShop
	bool					m_enableZShop;				//ZShop¶}Ãö
	string					m_ZShopURL;					//ZShopºô§}

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