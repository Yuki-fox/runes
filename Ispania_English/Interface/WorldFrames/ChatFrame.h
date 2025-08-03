#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include "../interface.h"

#define NUM_MAX_CHAT_WINDOWS			10
#define CHAT_HEADER_CHECK_CODE			0x10101010


#define UIEVENT_UPDATE_CHAT_FRAMES		"UPDATE_CHAT_FRAMES"
#define UIEVENT_CHAT_MSG_CHANNEL_JOIN	"CHAT_MSG_CHANNEL_JOIN"
#define UIEVENT_CHAT_MSG_CHANNEL_LEAVE	"CHAT_MSG_CHANNEL_LEAVE"
#define UIEVENT_CHAT_MSG_CHANNEL_CREATE	"CHAT_MSG_CHANNEL_CREATE"
#define UIEVENT_CHAT_MSG_CHANNEL_COLOR_CHANGE	"CHAT_MSG_CHANNEL_COLOR_CHANGE"

#define	CHAT_MSG_SYSTEM					"CHAT_MSG_SYSTEM"					// �t���W�D
#define	CHAT_MSG_SYSTEM_GET				"CHAT_MSG_SYSTEM_GET"				// �t�Ψ��o
#define	CHAT_MSG_SYSTEM_VALUE			"CHAT_MSG_SYSTEM_VALUE"				// �t�μƭ�
#define	CHAT_MSG_SAY					"CHAT_MSG_SAY"						// �d�򻡸�
#define	CHAT_MSG_ZONE					"CHAT_MSG_ZONE"						// �ϰ컡��
#define	CHAT_MSG_YELL					"CHAT_MSG_YELL"						// �ϰ�۸�
#define CHAT_MSG_CHANNEL				"CHAT_MSG_CHANNEL"					// �W�D����
#define	CHAT_MSG_WHISPER				"CHAT_MSG_WHISPER"					// �K�y
#define	CHAT_MSG_WHISPER_INFORM			"CHAT_MSG_WHISPER_INFORM"			// ������K�y
#define	CHAT_MSG_WHISPER_OFFLINE		"CHAT_MSG_WHISPER_OFFLINE"			// ���������u�T��
#define	CHAT_MSG_PARTY					"CHAT_MSG_PARTY"					// ����
#define	CHAT_MSG_BG						"CHAT_MSG_BG"						// �Գ�����
#define	CHAT_MSG_RAID					"CHAT_MSG_RAID"						// �ζ�
#define	CHAT_MSG_GUILD					"CHAT_MSG_GUILD"					// ���|
#define	CHAT_MSG_LFG					"CHAT_MSG_LFG"						// �ܽвն�(���W)
#define	CHAT_MSG_SALE					"CHAT_MSG_SALE"						// �R���W�D(���W)
#define	CHAT_MSG_LOOT					"CHAT_MSG_LOOT"						// 
#define	CHAT_MSG_COMBAT					"CHAT_MSG_COMBAT"					// �԰��T��
#define	CHAT_MSG_COMBAT_SELF			"CHAT_MSG_COMBAT_SELF"				// �԰��T���ۤv
#define CHAT_MSG_EMOTE					"CHAT_MSG_EMOTE"					// ���W�D
#define CHAT_MSG_GM						"CHAT_MSG_GM"						// GM �W�D
#define CHAT_MSG_GM_TALK				"CHAT_MSG_GM_TALK"					// GM �K�y���(�缾�a)
#define CHAT_MSG_DEBUG					"CHAT_MSG_DEBUG"					// DEBUG

#define NPC_HEADER						0x01								// NPC�}�Y

class CChatFrame;
extern CChatFrame*				g_pChatFrame;

using namespace std;

enum {
	eChatGroup_System					= 0,
	eChatGroup_Say,	
	eChatGroup_Zone,
	eChatGroup_Whisper,
	eChatGroup_Party,
	eChatGroup_Raid,
	eChatGroup_Guild,
	eChatGroup_Yell,
	eChatGroup_LFG,
	eChatGroup_Sale,
	eChatGroup_Loot,
	eChatGroup_Combat,
	eChatGroup_Emote,
	eChatGroup_Channel,
	eChatGroup_GM,
	eChatGroup_GM_Talk,
	eChatGroup_BG,						// �Գ��W�D

	eChatGroup_Last,
};

#define MESSAGE_TYPE_SYSTEM				"SYSTEM"
#define MESSAGE_TYPE_SAY				"SAY"
#define MESSAGE_TYPE_ZONE				"ZONE"
#define MESSAGE_TYPE_WHISPER			"WHISPER"
#define MESSAGE_TYPE_PARTY				"PARTY"
#define MESSAGE_TYPE_RAID				"RAID"
#define MESSAGE_TYPE_GUILD				"GUILD"
#define MESSAGE_TYPE_YELL				"YELL"
//#define MESSAGE_TYPE_LFG				"LFG"
//#define MESSAGE_TYPE_SALE				"SALE"
//#define MESSAGE_TYPE_LOOT				"LOOT"
#define MESSAGE_TYPE_COMBAT				"COMBAT"
#define MESSAGE_TYPE_EMOTE				"EMOTE"
#define MESSAGE_TYPE_CHANNEL			"CHANNEL"
#define MESSAGE_TYPE_GM					"GM"
#define MESSAGE_TYPE_GM_TALK			"GM_TALK"
#define MESSAGE_TYPE_BG					"BG"

struct ChatWindowInfo
{	
	set<int>					typeList;
	set<int>					channelList;
	string						name;
	CUiColor					color;
	bool						isLocked;
	bool						isVisible;
	int							docked;
	int							fontSize;

	static int					GetTypeListNum(const char* type)
	{
		if ( strcmp(type, MESSAGE_TYPE_SYSTEM) == 0 )
			return eChatGroup_System;
		else if ( strcmp(type, MESSAGE_TYPE_SAY) == 0 )
			return eChatGroup_Say;
		else if ( strcmp(type, MESSAGE_TYPE_ZONE) == 0 )
			return eChatGroup_Zone;
		else if ( strcmp(type, MESSAGE_TYPE_WHISPER) == 0 )
			return eChatGroup_Whisper;
		else if ( strcmp(type, MESSAGE_TYPE_PARTY) == 0 )
			return eChatGroup_Party;
		else if ( strcmp(type, MESSAGE_TYPE_RAID) == 0 )
			return eChatGroup_Raid;
		else if ( strcmp(type, MESSAGE_TYPE_GUILD) == 0 )
			return eChatGroup_Guild;		
		else if ( strcmp(type, MESSAGE_TYPE_YELL) == 0 )
			return eChatGroup_Yell;
		/*
		else if ( strcmp(type, MESSAGE_TYPE_LFG) == 0 )
			return eChatGroup_LFG;
		else if ( strcmp(type, MESSAGE_TYPE_SALE) == 0 )
			return eChatGroup_Sale;
		else if ( strcmp(type, MESSAGE_TYPE_LOOT) == 0 )
			return eChatGroup_Loot;
		*/
		else if ( strcmp(type, MESSAGE_TYPE_COMBAT) == 0 )
			return eChatGroup_Combat;
		else if ( strcmp(type, MESSAGE_TYPE_EMOTE) == 0 )
			return eChatGroup_Emote;
		else if ( strcmp(type, MESSAGE_TYPE_CHANNEL) == 0 )
			return eChatGroup_Channel;
		else if ( strcmp(type, MESSAGE_TYPE_GM) == 0 )
			return eChatGroup_GM;
		else if ( strcmp(type, MESSAGE_TYPE_GM_TALK) == 0 )
			return eChatGroup_GM_Talk;
		else if ( strcmp(type, MESSAGE_TYPE_BG) == 0 )
			return eChatGroup_BG;
		return -1;
	}
	static const char*			GetTypeListText(int type)
	{
		switch (type)
		{
		case eChatGroup_System:
			return MESSAGE_TYPE_SYSTEM;
		case eChatGroup_Say:
			return MESSAGE_TYPE_SAY;
		case eChatGroup_Zone:
			return MESSAGE_TYPE_ZONE;
		case eChatGroup_Whisper:
			return MESSAGE_TYPE_WHISPER;
		case eChatGroup_Party:
			return MESSAGE_TYPE_PARTY;
		case eChatGroup_Raid:
			return MESSAGE_TYPE_RAID;
		case eChatGroup_Guild:
			return MESSAGE_TYPE_GUILD;		
		case eChatGroup_Yell:
			return MESSAGE_TYPE_YELL;
		/*
		case eChatGroup_LFG:		
			return MESSAGE_TYPE_LFG;
		case eChatGroup_Sale:
			return MESSAGE_TYPE_SALE;
		case eChatGroup_Loot:
			return MESSAGE_TYPE_LOOT;
		*/			
		case eChatGroup_Combat:
			return MESSAGE_TYPE_COMBAT;
		case eChatGroup_Channel:
			return MESSAGE_TYPE_CHANNEL;
		case eChatGroup_Emote:
			return MESSAGE_TYPE_EMOTE;
		case eChatGroup_GM:
			return MESSAGE_TYPE_GM;
		case eChatGroup_GM_Talk:
			return MESSAGE_TYPE_GM_TALK;
		case eChatGroup_BG:
			return MESSAGE_TYPE_BG;
		}
		return "";
	}
};

struct ChatWindowSaveData
{
	ChatWindowSaveData()
	{
		header = CHAT_HEADER_CHECK_CODE;					// �ˬd���c��

		memset(name, 0, sizeof(name));
		memset(typeList, 0, sizeof(typeList));
		memset(channelList, 0, sizeof(channelList));

		color.m_a	= 1.0f;
		color.m_r	= 1.0f;
		color.m_g	= 1.0f;
		color.m_b	= 1.0f;
		isLocked	= true;
		isVisible	= true;
		docked		= 0;
		fontSize	= 14;
	}

	int					header;
	char				name[256];
	unsigned char		typeList[32];
	unsigned char		channelList[32];	
	CUiColor			color;
	bool				isLocked;
	bool				isVisible;
	int					docked;
	int					fontSize;
};

struct UserDefineChannel
{
	UserDefineChannel()
	{
		header = CHAT_HEADER_CHECK_CODE;					// �ˬd���c��
		id = 0;
		memset(name, 0, sizeof(name));
		memset(password, 0, sizeof(password));
	}

	bool operator < (const UserDefineChannel& arg)
	{
		return id < arg.id;
	}

	int header;
	int id;
	char name[256];
	char password[256];
};

struct ChatColorSaveData
{
	ChatColorSaveData()
	{
		header = CHAT_HEADER_CHECK_CODE;					// �ˬd���c��
		memset(chatType, 0, sizeof(chatType));
		color = 0xFFFFFFFF;
	}

	void Set(const char* str, int c)
	{		
		strncpy(chatType, str, 63);
		chatType[63] = 0;
		color = c;
	}

	int header;
	char chatType[64];
	int color;
};

struct ChatOfflineMessage
{
	string sender;
	string msg;
};


// �y���W�D.
enum e_VoiceChannelType
{
	EVCT_NONE = 0,
	EVCT_GUILD,
	EVCT_PARTY,
	EVCT_USER,
};

struct s_VoiceChannelInfo
{
	e_VoiceChannelType  eType;
    std::string     ChannelName;    // �W�D�W��
	std::string     LeaderName;     // �޲z��
	int             LeaderDBID;

	// �W�D����.
	bool            bSent;
	int             LastTime;
	bool            bDirty;         // żż?!

	std::vector<std::string>    vecMemberNameList;

    s_VoiceChannelInfo()
    : eType(EVCT_NONE),
      LeaderDBID(0),
      bSent(false),
      LastTime(-1),
      bDirty(true)
    {
    };
};


// ----------------------------------------------------------------------------
class CChatFrame : public CInterfaceBase
{
public:
	CChatFrame(CInterface* object);
	virtual ~CChatFrame();	
	
	void RegisterFunc();

	void DefaultWindowInfo();
	void Save();
	void Load();
	void SaveClientData();
	void LoadClientData(int type, int size, char* data);
	void BuildFrames();
	void DestroyFrames();
	void Update(float elapsedTime);	

	void LeaveWorld();

	// �s�B�z�禡
	void					SendChatMessage(const char* type, const char* sender, const char* msg, int vocation = -1);
	void					SendChatMessageEx(const char* type, const char* sender, const char* format, ...);
	void					AddOfflineMessage(const char* sender, const char* msg);
	void					ShowOfflineMessage();
	string					AddClassColor(const char* name, int voc);

	// �ª��禡
	void					SendMsgEvent(const char* type, const char* sender, const char* msg);
	void					AddMessage(int channel, const char* text);	
	
	ChatWindowInfo*			GetChatWindowInfo(int id);	
	int						JoinChannelByName(const char* name, const char* password, int channel, bool isCreate, bool isForce=false);
	int						LeaveChannelByID(int id, bool receive=false);
	int						LeaveChannelByName(const char* name, bool receive=false);
	void					ReceiveJoinChannel(const char* name);
	const char*				GetChannelPass(int id);
	void					SendChannelLeave(int id);
	
	const char*				GetChannelName(int id);
	int						GetChannelID(const char* name);
	int						GetSendChannelID(int id);

	bool					SendChannelMsg(int id, const char* playerName, const char* msg);
	bool					SetChannelColor(const char* channel, int color);
	int						GetChannelColor(const char* channel);
	vector<UserDefineChannel>&	GetChannelList()						{ return m_channel; }

	// ChatMsg
	void HaveNewWhisper(const char *who, const char *msg,int type);
	void HaveNewChannel(int msnType,int channelNo,const char *who, const char *msg,int type); 	
	void CreateChannelOK(const char *name,int channelNo);

	void ChangeChannelOwner(const char *name,int channelNo,const char *LeaderName);

	void DelChannelMember(const char* ChannelName, int channelNo, const char* MemberName);
	void AddChannelMember(const char* ChannelName, int channelNo, const char* MemberName);

	void R_ChannelPlayerListResult( GroupObjectTypeEnum Type , int ChannelID , vector<string> &Name );

	void GetVoiceChannelToken( e_VoiceChannelType etype, const string& strName, string& outToken );

	void JoinVoiceChannel( e_VoiceChannelType etype, const string& strName );
	void ExitVoiceChannel( );

	inline e_VoiceChannelType GetVoiceChannelType() { return m_eVoiceChannelType; }
	inline const char* GetVoiceChannelName() { return m_strVoiceChannelName.c_str(); }

	//�ШD���oChannelInfo
	void VoiceChannelInfoRequest( e_VoiceChannelType type, const char* name );
	s_VoiceChannelInfo* GetVoiceChannelInfo( e_VoiceChannelType type, const char* name );

	// Cli to Cli �ܽХ[�J�W�D
	void R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password );

protected:
	void LoadWindowInfo(FILE* fptr);

protected:
	ChatWindowInfo              m_windowInfo[NUM_MAX_CHAT_WINDOWS];
	std::map<std::string, int>  m_chatColor;							// �C���W�D�C���
	std::vector<UserDefineChannel>  m_channel;
	std::vector<ChatOfflineMessage> m_offlineMessage;
	bool						m_loadServerData;

	s_VoiceChannelInfo			m_stVoiceChannelInfo; 
	e_VoiceChannelType          m_eVoiceChannelType;
    std::string                 m_strVoiceChannelName;
};
