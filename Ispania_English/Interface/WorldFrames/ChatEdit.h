#pragma once

#include <vector>
#include <string>
#include "../interface.h"

class CChatEdit;
extern CChatEdit*					g_pChatEdit;

using namespace std;

enum ChatTypeInfo {
	eChatType_SAY				= 0,			// 範圍	
	eChatType_YELL,								// 世界頻道
	eChatType_ZONE,								// 區域
	eChatType_WHISPER,							// 密語
	eChatType_PARTY,							// 隊伍
	eChatType_RAID,								// 團隊
	eChatType_GUILD,							// 公會
	eChatType_GM,								// GM頻道
	eChatType_LFG,								// 招幕
	eChatType_SALE,								// 買賣
	eChatType_CHANNEL,							// 自定頻道
	eChatType_BG,								// 戰場
};

// 已經修改為上面類型
enum {
	eChatType_NULL				= 0,
	eChatType_Whisper,
	eChatType_Range,
	eChatType_Zone,
	eChatType_GMCommand,
	eChatType_System
};

// -----------------------------------------------------------------------------
typedef void(*com_func)( const char* );

// -----------------------------------------------------------------------------
struct CommandInfo
{
	com_func	func;
	string		explain;
	int			powerLevel;		// 一般玩家用指令為 <= 0
};

// -----------------------------------------------------------------------------
class CSplitArgString
{
public:
	CSplitArgString();
	CSplitArgString(const char* str);
	~CSplitArgString();

	void						SetString(const char* str);
	string 						GetString(int start, int end=-1);
	int                         GetArgSize() { return (int)m_endIndex.size();}
protected:
	char*						m_str;
	vector<int>					m_startIndex;
	vector<int>					m_endIndex;
};

class CChatEdit : public CInterfaceBase
{
public:
	CChatEdit(CInterface* object);
	virtual ~CChatEdit();

	virtual	void				RegisterFunc();	
	virtual	void				Update(float elapsedTime);
	virtual	void				RegisterCmd(const char* cmdStr, const char* explain, com_func func, int powerLevel=1);

	virtual	void				SendChatMessage(ChatTypeInfo type, const char* text, const char* language, const char* extra);
	virtual	void				SendChatMessage(const char* text, int channel=-1);
	virtual	void				SetChannel(int channel)						{ m_channel = channel; }
	virtual	void				ParseText(const char* text);
	virtual	void				SetWhisper(const char* name)				{ m_tellTarget = name; }
	virtual void				SetBanChannelTime(int channel, float time);

	void						InsertUndesirable(const char* name);
	void						RemoveUndesirable(const char* name);
	bool						IsUndesirable(const char* name);

	void						RL_MoneyVendorReport(int count);

protected:
	virtual	void				Translator(const char* src, char* dest);
	virtual void				IgnoreColorCode(char* src);

protected:
	map<string, CommandInfo>	m_command;
	int							m_channel;
	string						m_tellTarget;

	float						m_sendCount[20];
	float						m_banTime[20];

	map<string, float>			m_undesirable;				// 檢舉玩家名單
};