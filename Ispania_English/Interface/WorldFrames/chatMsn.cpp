#include <string>
#include <vector>
using namespace std;
#include <windows.h>
#include "chatMsn.h"
#include "..\interface.h"
#include <time.h>
#include "RoleData\ObjectDataClass.h"
#include "GameTooltip.h"
#include "PG\PG_Guild.h"
#include "..\..\netWaker_member\Net_Channel\NetCli_Channel.h"
#include "..\..\netWaker_member\Net_Channel\NetCli_ChannelChild.h"
#include "../mainproc/NetGlobal.h"
#include "../mainproc/GameMain.h"
#include "..\socal\UI_Socal.h"
#include "PG\NetWakerPGEnum.h"
#include "GuildFrame.h"
#include "ChatFrame.h"
#include "..\mainproc\NetGlobal.h"
MsnManager* _gMsnManager;
int g_StringAlige = 0;
void MsnManager_SetAnchor(CUiAnchor& anchor)
{
	if (g_StringAlige == 0)
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
	}
	else
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
	}
}
void SetNewFontSize(CUiFontString* fontString, int fontSize)
{
	if (fontString)
	{
		CRuFontType* oldFontType = fontString->GetFontType();
		if (oldFontType)
		{
			CRuFontType* pFontType = CRuFontType::GetFontType(oldFontType->GetFontName(), fontSize, oldFontType->GetWeight(), oldFontType->GetOutLine());
			if (pFontType)
			{
				fontString->SetFontType(pFontType);
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------
// Cli to Cli 要求加入頻道
struct PG_CtoC_InviteChannel
{
	CliToCliPGCommandEnum Command;
	StaticString <_MAX_NAMESTRING_LEN_> ChannelName; //頻道名稱
	StaticString <_MAX_NAMESTRING_LEN_> ChannelPass; //頻道密碼
	PG_CtoC_InviteChannel()
	{
		Command = EM_PG_CtoC_InviteChannel;
		ChannelName.Clear();
		ChannelPass.Clear();
	}
};
struct ChatListItem
{
	int type;
	int state;
	int mode;
	bool add;
	string m_name;
	ChatListItem() :add(false), type(-1), state(-1), mode(1)
	{
	}
};
class ChatListManager
{
public:
	void Initial(ChatMsnInfo* msn);
	void SetToggle(int index);
	int AddMember(int type, int state, const char* name, bool force = false);
	static void GetNameAndPassword(string& name, string& password);
	void InviteListOK(void);
	ChatListItem* GetInfo(int index);
	int GetCount();
	void CalAllList(void);
public:
	string m_channelName;
	string m_channelPassword;
	ChatMsnInfo* m_pmsn;
	vector<ChatListItem> m_lists;
};
void ChatListManager::GetNameAndPassword(string& name, string& password)
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	char temp[512];
	time_t NowTime;
	time(&NowTime);
	struct tm* gmt;
	gmt = localtime(&NowTime);
	sprintf(temp, "%s_%d", Role->RoleName(), gmt->tm_sec);
	name = temp;
	sprintf(temp, "chat_%d", g_pGameMain->GetDBID());
	password = temp;
}
void ChatListManager::InviteListOK(void)
{
	int count = (int)m_lists.size();
	//if (count>0)
	{
		MSN_TYPE type = m_pmsn->GetType();
		if (type == MT_WHISPER)
		{
			GetNameAndPassword(m_channelName, m_channelPassword);
			//NetCli_Channel::S_CreateChannel(m_channelName.c_str(),m_channelPassword.c_str());
			g_pChatFrame->JoinChannelByName(m_channelName.c_str(), m_channelPassword.c_str(), -1, true);
			m_pmsn->SetNameAndPassword(m_channelName, m_channelPassword);
			int count = m_pmsn->GetMemberCount();
			RoleDataEx* Role = CNetGlobal::RoleData();
			const char* Leader = Role->RoleName();
			for (int i = 0; i < count; i++)
			{
				const char* name = m_pmsn->MemberGetName(i);
				if (strcmp(Leader, name))
				{
					PG_CtoC_InviteChannel Send;
					Send.ChannelName = m_channelName.c_str();
					Send.ChannelPass = m_channelPassword.c_str();
					NetCli_ChannelChild::S_DataTransmitToCli_Name((char*)name, &Send, sizeof(Send));
				}
			}
		}
		else if (type == MT_CHANNEL)
		{
			m_pmsn->GetNameAndPassword(m_channelName, m_channelPassword);
			for (int i = 0; i < count; i++)
			{
				ChatListItem& info = m_lists[i];
				if (info.mode && info.add)
				{
					PG_CtoC_InviteChannel Send;
					Send.ChannelName = m_channelName.c_str();
					Send.ChannelPass = m_channelPassword.c_str();
					NetCli_ChannelChild::S_DataTransmitToCli_Name((char*)info.m_name.c_str(), &Send, sizeof(Send));
				}
			}
		}
	}
}
ChatListItem* ChatListManager::GetInfo(int index)
{
	ChatListItem* info = NULL;
	if (index >= 0 && index < m_lists.size())
	{
		info = &m_lists[index];
	}
	return info;
}
int ChatListManager::GetCount()
{
	return (int)m_lists.size();
}
void ChatListManager::SetToggle(int index)
{
	if (index >= 0 && index < m_lists.size())
		m_lists[index].add = !m_lists[index].add;
}
void ChatListManager::Initial(ChatMsnInfo* msn)
{
	m_pmsn = msn;
	m_lists.clear();
}
int ChatListManager::AddMember(int type, int state, const char* name, bool force)
{
	vector<string>& lists = m_pmsn->m_members;
	int mode = 1;
	int count;
	bool add = false;
	if (force == false)
	{
		count = (int)lists.size();
		for (int i = 0; i < count; i++)
		{
			if (lists[i] == name)
			{
				return -1;
			}
		}
	}
	count = (int)m_lists.size();
	for (int i = 0; i < count; i++)
	{
		if (m_lists[i].m_name == name)
			return i;
	}
	int index = (int)m_lists.size();
	ChatListItem item;
	item.m_name = name;
	item.type = type;
	item.state = state;
	item.add = add;
	item.mode = mode;
	m_lists.push_back(item);
	return index;
}
void ChatListManager::CalAllList(void)
{
	int count = g_pGuildFrame->GetNumGuildMembers();
	GuildMemberListInfo* info;
	for (int i = 0; i < count; i++)
	{
		info = g_pGuildFrame->GetGuildRosterInfo(i);
		if (info)
		{
			GuildMemberBaseInfoStruct* pmember = info->member;
			bool bOnLine = pmember->IsOnline;
			AddMember(0, bOnLine ? 1 : 0, pmember->MemberName.Begin());
		}
	}
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo(EM_FriendListType_FamilyFriend, &pList, MaxCount);
	count = 0;
	for (int i = 0; i < MaxCount; ++i)
	{
		if (pList[i].IsEmpty() == false)
		{
			int dbid = pList[i].DBID;
			bool bOnLine = g_pUi_Socal->m_PlayerOnlineSet.find(dbid) != g_pUi_Socal->m_PlayerOnlineSet.end();
			AddMember(1, bOnLine ? 1 : 0, pList[i].Name);
			++count;
		}
	}
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo(EM_FriendListType_Friend, &pList, MaxCount);
	count = 0;
	for (int i = 0; i < MaxCount; ++i)
	{
		if (pList[i].IsEmpty() == false)
		{
			int dbid = pList[i].DBID;
			bool bOnLine = g_pUi_Socal->m_PlayerOnlineSet.find(dbid) != g_pUi_Socal->m_PlayerOnlineSet.end();
			AddMember(2, bOnLine ? 1 : 0, pList[i].Name);
			++count;
		}
	}
#ifdef _DEBUGx
	{
		char temp[256];
		for (int i = 0; i < 40; i++)
		{
			sprintf(temp, "test_%d", i);
			AddMember(0, 1, temp);
		}
	}
#endif
}
void UiCallback_Chat_NameLink_OnMouseUp(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if (_gMsnManager)
		_gMsnManager->NameLink_OnMouseUp(uiFrame, (int)lParam, (int)rParam);
}
void UiCallback_Chat_NameLink_OnMouseDown(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_NameLink_OnMouseEnter(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_NameLink_OnMouseLeave(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_ItemLink_OnMouseUp(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if (_gMsnManager)
		_gMsnManager->ItemLink_OnMouseUp(uiFrame, (int)lParam, (int)rParam);
}
void UiCallback_Chat_ItemLink_OnMouseDown(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_ItemLink_OnMouseEnter(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if (_gMsnManager)
		_gMsnManager->ItemLink_OnMouseEnter(uiFrame, (int)lParam, (int)rParam);
}
void UiCallback_Chat_ItemLink_OnMouseLeave(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if (_gMsnManager)
		_gMsnManager->ItemLink_OnMouseLeave(uiFrame, lParam, rParam);
}
void UiCallback_Chat_Button_OnMouseUp(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if (_gMsnManager)
		_gMsnManager->Button_OnMouseUp(uiFrame, (int)lParam, (int)rParam);
}
void UiCallback_Chat_Button_OnMouseDown(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_Button_OnMouseEnter(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
void UiCallback_Chat_Button_OnMouseLeave(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
}
struct MsgLine
{
	CUiFrame* m_frame;
	WCHAR* m_str;
	int m_color;
	int m_time;
	float m_alpha;
	int m_count;
	CUiSize m_size;
	void SetShow(bool bShow);
	MsgLine();
	~MsgLine() { Finish(); }
	void Initial(CUiFrame* parent, MsgLine* prenm, int index);
	void Finish(void);
	void AddElement(CUiLayout* PrenLineFirst, CUiLayout* Pren, CUiLayout* Element);
	void AddMessage(const WCHAR* msg, CUiColor& color, CUiFontString* colneString, int limiteWidth, int id);
	void Parse(CUiColor& color, CUiFontString* colneString, int limiteWidth, int id);
	CUiLayout* AddButton(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& size, CUiColor& color, CUiFontString* colneString);
	CUiLayout* AddNameLink(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& size, CUiColor& color, CUiFontString* colneString);
	CUiLayout* AddItemLink(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& size, CUiColor& color, CUiFontString* colneString, int limitWidth);
	CUiLayout* AddFace(CUiLayout* LineFrame, int index, CUiSize& size);
	CUiLayout* AddString(CUiLayout* LineFrame, const WCHAR* text, CUiSize& size, CUiColor& color, CUiFontString* font);
	void CalsRect(CUiSize& line, CUiSize& rc);
	//int GetWord(WCHAR* string,int &lineW,CUiSize&size,CUiFontString* font  ,WCHAR* trim);
	void AddLineSize(CUiLayout* PrenLineFirst, CUiSize& line);
	CUiLayout* GetNewLine(CUiLayout* Pren);
};
//int MsgLine_GetWord(WCHAR* string,int &w,CUiSize&size,CUiFontString* font ,WCHAR* trim,int trims);
void MsgLine::SetShow(bool bShow)
{
	m_frame->SetVisible(bShow);
}
MsgLine::MsgLine() :m_frame(NULL), m_str(NULL), m_color(-1), m_time(10), m_alpha(1.0f)
{
}
void MsgLine::AddLineSize(CUiLayout* frame, CUiSize& line)
{
	frame->SetAnchorSize(line);
	m_size.m_y += line.m_y;
	if (m_size.m_x < line.m_x)
		m_size.m_x = line.m_x;
}
void MsgLine::CalsRect(CUiSize& line, CUiSize& src)
{
	line.m_x += src.m_x;
	if (line.m_y < src.m_y)
		line.m_y = src.m_y;
}
CUiLayout* MsgLine::AddItemLink(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& linesize, CUiColor& color, CUiFontString* colneString, int limitWidth)
{
	char name[128];
	sprintf(name, "%s_ItemLink%d", m_frame->GetName(), m_count++);
	CUi* ui = m_frame->GetUi();
	CUiButton* but = static_cast<CUiButton*>(ui->CreateObject(CUiLayout::eUiType_Button, name, LineFrame));
	lua_State* L = ui->GetLuaState();
	char* tmp = WCharToUFT8(text);
	lua_getglobal(L, name);
	lua_pushstring(L, "data");
	lua_pushstring(L, tmp);
	lua_settable(L, -3);
	delete[]tmp;
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = -2;
	CUiLayout* pren = m_frame;
	CUiFontString* font = new CUiFontString(NULL, but, ui);
	if (font)
	{
		font->SetFontType(colneString->GetFontType());
		font->SetFontShadow(colneString->GetFontShadow());
		font->SetJustifyHType(colneString->GetJustifyHType());
		font->SetJustifyVType(colneString->GetJustifyVType());
		font->SetSpacing(colneString->GetSpacing());
		font->SetColor(color.m_r, 0, color.m_b);
		font->SetAnchorSize(CUiSize(0.0f, 0.0f));
		but->SetNormalText(font);
		anchor.m_relativeTo = but;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
		font->SetAnchor(anchor);
	}
	WCHAR butName[1024];
	wsprintfW(butName, L"%s", text);
	but->SetText(butName);
	CUiSize src;
	src.m_x = font->GetWidth() + 2;
	src.m_y = font->GetHeight();
	// 當每行超出最大顯示寬度
	if (src.m_x > limitWidth)
	{
		font->SetAnchorSize(CUiSize((float)limitWidth, 0.0f));
		src.m_x = font->GetWidth() + 2;
		src.m_y = font->GetHeight();
	}
	but->SetAnchorSize(src);
	//m_frame->ClearAllAnchors();
	but->m_pfOnMouseUp = UiCallback_Chat_ItemLink_OnMouseUp;
	but->m_pfOnMouseDown = UiCallback_Chat_ItemLink_OnMouseDown;
	but->m_pfOnMouseEnter = UiCallback_Chat_ItemLink_OnMouseEnter;
	but->m_pfOnMouseLeave = UiCallback_Chat_ItemLink_OnMouseLeave;
	//	but->SetScripts(CUiFrame::eEventType_MouseUp,"Char_ButtonClick(this);");
	but->SetID(id);
	CalsRect(linesize, src);
	return but;
}
char* NameLink_GetName(char* tmp)
{
	char* ret = 0;
	int len = (int)strlen(tmp);
	if (strncmp(tmp, "|Hplayer:", 9) == 0)
	{
		ret = &tmp[9];
		for (int i = 9; i < len - 1; i++)
		{
			if (tmp[i] == '|' && tmp[i + 1] == 'h')
			{
				tmp[i] = 0;
				break;
			}
		}
	}
	return ret;
}
CUiLayout* MsgLine::AddNameLink(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& linesize, CUiColor& color, CUiFontString* colneString)
{
	char name[128];
	sprintf(name, "%s_NameLink%d", m_frame->GetName(), m_count++);
	CUi* ui = m_frame->GetUi();
	CUiButton* but = static_cast<CUiButton*>(ui->CreateObject(CUiLayout::eUiType_Button, name, LineFrame));
	lua_State* L = ui->GetLuaState();
	char* tmp = WCharToUFT8(text);
	//	char *Namedata=NameLink_GetName(tmp);
	lua_getglobal(L, name);
	lua_pushstring(L, "data");
	lua_pushstring(L, tmp);
	lua_settable(L, -3);
	delete[]tmp;
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = -2;
	CUiLayout* pren = m_frame;
	CUiFontString* font = new CUiFontString(NULL, but, ui);
	if (font)
	{
		font->SetFontType(colneString->GetFontType());
		font->SetFontShadow(colneString->GetFontShadow());
		font->SetJustifyHType(colneString->GetJustifyHType());
		font->SetJustifyVType(colneString->GetJustifyVType());
		font->SetSpacing(colneString->GetSpacing());
		font->SetColor(color.m_r, color.m_g, color.m_b);
		font->SetAnchorSize(CUiSize(0.0f, 0.0f));
		but->SetNormalText(font);
		anchor.m_relativeTo = but;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
		font->SetAnchor(anchor);
	}
	WCHAR butName[1024];
	wsprintfW(butName, L"%s", text);
	but->SetText(butName);
	CUiSize src;
	src.m_x = font->GetWidth() + 2;
	src.m_y = font->GetHeight() + 2;
	but->SetAnchorSize(src);
	//m_frame->ClearAllAnchors();
	but->m_pfOnMouseUp = UiCallback_Chat_NameLink_OnMouseUp;
	but->m_pfOnMouseDown = UiCallback_Chat_NameLink_OnMouseDown;
	but->m_pfOnMouseEnter = UiCallback_Chat_NameLink_OnMouseEnter;
	but->m_pfOnMouseLeave = UiCallback_Chat_NameLink_OnMouseLeave;
	//	but->SetScripts(CUiFrame::eEventType_MouseUp,"Char_ButtonClick(this);");
	but->SetID(id);
	CalsRect(linesize, src);
	return but;
}
CUiLayout* MsgLine::AddButton(int id, CUiLayout* LineFrame, const WCHAR* text, CUiSize& linesize, CUiColor& color, CUiFontString* colneString)
{
	char name[128];
	sprintf(name, "%s_but%d", m_frame->GetName(), m_count++);
	CUi* ui = m_frame->GetUi();
	CUiButton* but = static_cast<CUiButton*>(ui->CreateObject(CUiLayout::eUiType_Button, name, LineFrame));
	lua_State* L = ui->GetLuaState();
	char* tmp = WCharToUFT8(text);
	lua_getglobal(L, name);
	lua_pushstring(L, "data");
	lua_pushstring(L, tmp);
	lua_settable(L, -3);
	delete[]tmp;
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = -2;
	CUiLayout* pren = m_frame;
	CUiFontString* font = new CUiFontString(NULL, but, ui);
	if (font)
	{
		font->SetFontType(colneString->GetFontType());
		font->SetFontShadow(colneString->GetFontShadow());
		font->SetJustifyHType(colneString->GetJustifyHType());
		font->SetJustifyVType(colneString->GetJustifyVType());
		font->SetSpacing(colneString->GetSpacing());
		font->SetColor(color.m_r, color.m_g, color.m_b);
		font->SetAnchorSize(CUiSize(0.0f, 0.0f));
		but->SetNormalText(font);
		anchor.m_relativeTo = but;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
		font->SetAnchor(anchor);
	}
	/*
	{
	CTextureBackdrop backdrop;
	backdrop.SetEdgeSize(16.0f);
	backdrop.SetTileSize(16.0f);
	backdrop.SetEdgeFileName("Interface\\Tooltips\\SayBubble-Border");
	backdrop.SetTileFileName("Interface\\Tooltips\\SayBubble-Background");
	backdrop.SetBackgroundOffset(CRuRectangle(3.0f, 3.0f, 3.0f, 3.0f));
	but->SetBackdrop(&backdrop);
	}*/
	WCHAR butName[1024];
	wsprintfW(butName, L"[%s]", text);
	but->SetText(butName);
	CUiSize src;
	src.m_x = font->GetWidth() + 2;
	src.m_y = font->GetHeight() + 2;
	but->SetAnchorSize(src);
	//m_frame->ClearAllAnchors();
	but->m_pfOnMouseUp = UiCallback_Chat_Button_OnMouseUp;
	but->m_pfOnMouseDown = UiCallback_Chat_Button_OnMouseDown;
	but->m_pfOnMouseEnter = UiCallback_Chat_Button_OnMouseEnter;
	but->m_pfOnMouseLeave = UiCallback_Chat_Button_OnMouseLeave;
	//	but->SetScripts(CUiFrame::eEventType_MouseUp,"Char_ButtonClick(this);");
	but->SetID(id);
	CalsRect(linesize, src);
	return but;
}
void MsgLine::AddElement(CUiLayout* PrenLineFirst, CUiLayout* Pren, CUiLayout* Element)
{
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0.0f;
	anchor.m_offset.m_y = 0.0f;
	if (g_StringAlige == 0)
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
	else
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
	if (Pren)
	{//跟在後面
		anchor.m_relativeTo = Pren;
		if (g_StringAlige == 0)
			anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
		else
			anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
	}
	else
	{
		//新一行的開頭
		anchor.m_relativeTo = PrenLineFirst;
		if (g_StringAlige == 0)
			anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
		else
			anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
	}
	if (Element)
		Element->SetAnchor(anchor);
}
CUiLayout* MsgLine::GetNewLine(CUiLayout* Pren)
{
	CUiLayout* newFrame = new CUiFrame(NULL, m_frame, m_frame->GetUi());
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0.0f;
	anchor.m_offset.m_y = 0.0f;
	if (g_StringAlige == 0)
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
	else
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
	//新一行的開頭
	anchor.m_relativeTo = m_frame;
	if (g_StringAlige == 0)
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
	else
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
	newFrame->SetAnchor(anchor);
	if (Pren)
	{
		Pren->ClearAllAnchors();
		if (g_StringAlige == 0)
			anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		else
			anchor.m_point = CUiAnchor::eAnchor_BottomRight;
		//新一行的開頭
		anchor.m_relativeTo = newFrame;
		if (g_StringAlige == 0)
			anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
		else
			anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
		Pren->SetAnchor(anchor);
	}
	return newFrame;
}
CUiLayout* MsgLine::AddFace(CUiLayout* LineFrame, int index, CUiSize& linesize)
{
	CUi* ui = m_frame->GetUi();
	char temp[128];
	sprintf(temp, "ChatEmotion_em%dAnim", index + 1);
	CUiTexture* facetex = static_cast<CUiTexture*>(ui->FindObject(temp));
	if (facetex == NULL)
	{
		sprintf(temp, "ChatEmotion_em%dAnim", 1);
		facetex = static_cast<CUiTexture*>(ui->FindObject(temp));
		if (facetex == NULL)
			return NULL;
	}
	CUiFrame* frame = new CUiFrame(NULL, LineFrame, ui);
	CUiSize src(32, 34);
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0.0f;
	anchor.m_offset.m_y = -1.0f;
	CUiTexture* tex = new CUiTexture(NULL, frame, ui);
	if (tex)
	{
		tex->SetAnchorSize(src);
		tex->SetTexture(facetex);
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativeTo = frame;
		tex->SetAnchor(anchor);
		tex->SetScale(1.0f);
		frame->SetLayers(CUiFrame::eLayerType_Artwork, tex);
		src.m_x = facetex->GetWidth();
		src.m_y = facetex->GetHeight() + 2;
		tex->SetSize(CUiSize(facetex->GetWidth(), facetex->GetHeight()));
	}
	frame->SetAnchorSize(src);
	CalsRect(linesize, src);
	return frame;
}
CUiLayout* MsgLine::AddString(CUiLayout* LineFrame, const WCHAR* text, CUiSize& linesize, CUiColor& color, CUiFontString* colneString)
{
	CUi* ui = m_frame->GetUi();
	CUiFrame* frame = new CUiFrame(NULL, LineFrame, ui);
	CUiFontString* font = new CUiFontString(NULL, frame, ui);
	if (font)
	{
		font->SetFontType(colneString->GetFontType());
		font->SetFontShadow(colneString->GetFontShadow());
		font->SetJustifyHType(colneString->GetJustifyHType());
		font->SetJustifyVType(colneString->GetJustifyVType());
		font->SetSpacing(colneString->GetSpacing());
		font->SetColor(color.m_r, color.m_g, color.m_b);
		font->SetAnchorSize(CUiSize(0.0f, 0.0f));
	}
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = -2;
	anchor.m_relativeTo = frame;
	anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
	anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
	font->SetAnchor(anchor);
	font->SetColor(color.m_r, color.m_g, color.m_b);
	font->SetAlpha(1.0f);
	font->SetText(text);
	CUiSize src;
	src.m_x = font->GetWidth();
	src.m_y = font->GetHeight();
	frame->SetAnchorSize(src);
	frame->SetLayers(CUiFrame::eLayerType_Artwork, font);
	CalsRect(linesize, src);
	return frame;
}
void MsgLine::AddMessage(LPCWSTR  msg, CUiColor& color, CUiFontString* colneString, int width, int id)
{
	if (m_str)
		delete[]m_str;
	m_str = NULL;
	if (msg)
	{
		int len = (int)wcslen(msg);
		if (len > 0)
		{
			m_str = new WCHAR[len + 1];
			wcscpy(m_str, msg);
			Parse(color, colneString, width, id);
		}
	}
}
void MsgLine::Initial(CUiFrame* parent, MsgLine* _pren, int index)
{
	CUi* ui = parent->GetUi();
	m_size.m_x = 0;
	m_size.m_y = 0;
	m_count = 0;
	m_color = -1;
	m_time = 10;
	m_alpha = 1.0f;
	char name[128];
	sprintf(name, "%s%d", parent->GetName(), index);
	m_frame = static_cast<CUiFrame*>(ui->CreateObject(CUiLayout::eUiType_Frame, name, parent));
	//m_frame=;//new CUiFrame(temp,parent,ui);
	CUiFrame* pren = NULL;
	if (_pren)
		pren = _pren->m_frame;
	m_frame->SetFrameLevel(-1);
	m_frame->SetAlpha(1.0f);
	//m_frame->SetAnchorSize(CUiSize(100.0f, 100.0f));
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = 0;
	//m_frame->ClearAllAnchors();
	if (g_StringAlige == 0)
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
	}
	else
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
	}
	anchor.m_relativeTo = parent;
	m_frame->SetAnchor(anchor);
	if (pren)
	{
		pren->ClearAllAnchors();
		if (g_StringAlige == 0)
		{
			anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
			anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
		}
		else
		{
			anchor.m_point = CUiAnchor::eAnchor_BottomRight;
			anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
		}
		anchor.m_relativeTo = m_frame;
		pren->SetAnchor(anchor);
	}
	/*
	{
	CTextureBackdrop backdrop;
	backdrop.SetEdgeSize(16.0f);
	backdrop.SetTileSize(16.0f);
	backdrop.SetEdgeFileName("Interface\\Tooltips\\SayBubble-Border");
	backdrop.SetTileFileName("Interface\\Tooltips\\SayBubble-Background");
	backdrop.SetBackgroundOffset(CRuRectangle(3.0f, 3.0f, 3.0f, 3.0f));
	m_frame->SetBackdrop(&backdrop);
	}
	*/
}
enum _MSN_ELEMENT_TYPE
{
	MT_FACE,
	MT_BUTTON,
	MT_WORD,
	MT_ITEMLINK,
	MT_NAMELINK,
	MT_NEXTLINE,
	MT_MAX
};
struct unitWord
{
	int m_type;
	int m_width;
	int m_len;
	WCHAR* m_pStr;
	unitWord() :m_type(-1), m_width(0), m_pStr(NULL), m_len(0)
	{
	}
	int GetNumber(WCHAR* string, int len);
	int GetButton(WCHAR* string, int len);
	int GetWord(WCHAR* string, int len, CUiFontString* font, int maxWidth);
	int NextLine(WCHAR* string, int len);
	int GetItemLink(WCHAR* string, int len);
	int GetNameLink(WCHAR* string, int len);
	void SetWidth(CUiFontString* font);
	int GetWidth()
	{
		return m_width;
	}
};
void  unitWord::SetWidth(CUiFontString* font)
{
	m_width = 0;
	switch (m_type)
	{
	case MT_FACE:
	{
		m_width = 32;
		int index = _wtoi(m_pStr);
		char temp[128];
		sprintf(temp, "ChatEmotion_em%dAnim", index + 1);
		CUi* ui = _gMsnManager->m_iface->GetUi();
		CUiTexture* facetex = static_cast<CUiTexture*>(ui->FindObject(temp));
		if (facetex)
			m_width = (int)facetex->GetWidth();
	}
	break;
	case MT_WORD:
	case MT_BUTTON:
	case MT_ITEMLINK:
	case MT_NAMELINK:
	{
		WCHAR temp[1024];
		memcpy(temp, m_pStr, m_len * sizeof(WCHAR));
		temp[m_len] = 0;
		font->SetText(L"");
		font->SetText(temp);
		m_width = (int)font->GetWidth();
	}
	break;
	}
}
int unitWord::NextLine(WCHAR* string, int len)
{
	m_type = MT_NEXTLINE;
	m_pStr = string;
	return 1;
}
int unitWord::GetNameLink(WCHAR* string, int len)
{
	int count = 0;
	m_type = MT_NAMELINK;
	m_pStr = string;
	bool findend = false;
	int endN = 0;
	for (int t = 0; t < len; t++)
	{
		if (string[t] == '|' && string[t + 1] == 'h')
		{
			endN++;
			t++;
			count += 2;
			if (endN >= 2)
			{
				findend = true;
				break;
			}
		}
		else
			count++;
	}
	if (!findend)
		count = -1;
	m_len = count;
	return count;
}
int unitWord::GetItemLink(WCHAR* string, int len)
{
	int count = 0;
	m_type = MT_ITEMLINK;
	m_pStr = string;
	bool findend = false;
	int endN = 0;
	for (int t = 0; t < len; t++)
	{
		if (string[t] == '|' && string[t + 1] == 'h')
		{
			endN++;
			t++;
			count += 2;
			if (endN >= 2)
			{
				findend = true;
				break;
			}
		}
		else
			count++;
	}
	if (!findend)
		count = -1;
	m_len = count;
	return count;
}
int unitWord::GetButton(WCHAR* string, int len)
{
	int count = 0;
	m_type = MT_BUTTON;
	m_pStr = string;
	bool findend = false;
	for (int t = 0; t < len; t++)
	{
		if (string[t] == '$')
		{
			findend = true;
			break;
		}
		else
			count++;
	}
	if (!findend)
		count = -1;
	m_len = count;
	return count;
}
int unitWord::GetWord(WCHAR* string, int len, CUiFontString* font, int maxWidth)
{
	int count = 0;
	m_type = MT_WORD;
	m_pStr = string;
	WCHAR temp[1024];
	bool First = true;
	int width = 0;
	bool Ansi = true;
	for (int t = 0; t < len; t++)
	{
		int c = string[t];
		//CRuFontChar* chr=CRuFontChar::GetFontChar(font->GetFontType(),c);
		{
			temp[t] = c;
			temp[t + 1] = 0;
			font->SetText(temp);
			width = (int)font->GetWidth();
		}
		//width+=chr->GetXSize();
		if (width >= maxWidth)
		{
			break;
		}
		if (c > 0x255)
		{
			if (First)
				Ansi = false;
			else if (Ansi)
				break;
			count++;
		}
		else if (c == '\n' || c == '\t' || c == ' ' && First)
		{
			count++;
			break;
		}
		else if ((c == '|' || c == '$' || c == '#' || c == '\n' || c == '\t' || c == ' ') && !First)
		{
			break;
		}
		else
		{
			First = false;
			if (!Ansi)
				break;
			else
				count++;
		}
	}
	m_len = count;
	return count;
}
int  unitWord::GetNumber(WCHAR* string, int len)
{
	int count = 0;
	m_type = MT_FACE;
	m_pStr = string;
	for (int t = 0; t < len; t++)
	{
		if (string[t] < '0' || string[t]>'9')
			break;
		else
			count++;
	}
	m_len = count;
	if (count == 0)
		count = -1;
	return count;
}
int Msn_GetColor(WCHAR* str, int len, CUiColor& color)
{
	int count = -1;
	char tempStr[3];
	tempStr[2] = 0;
	if (len > 12 && str[10] == '|' && str[11] == 'r')
	{
		int t;
		tempStr[0] = (char)str[2];
		tempStr[1] = (char)str[3];
		sscanf(tempStr, "%02x", &t);
		color.m_a = t / 255.0f;
		tempStr[0] = (char)str[4];
		tempStr[1] = (char)str[5];
		sscanf(tempStr, "%02x", &t);
		color.m_r = t / 255.0f;
		tempStr[0] = (char)str[6];
		tempStr[1] = (char)str[7];
		sscanf(tempStr, "%02x", &t);
		color.m_g = t / 255.0f;
		tempStr[0] = (char)str[8];
		tempStr[1] = (char)str[9];
		sscanf(tempStr, "%02x", &t);
		color.m_b = t / 255.0f;
		count = 12;
	}
	return count;
}
int MsgLine_GetUnits(WCHAR* str, vector<unitWord>& units, CUiFontString* font, int maxW, CUiColor& color)
{
	int len = lstrlenW(str);
	//	WCHAR tempStr[1024];
	BOOL bString = false;
	int stringlen = 0;
	int LineW = 0;
	int WordW = 0;
	WCHAR* pStr = str;
	unitWord unit;
	int c;
	while (len > 0)
	{
		int count = -1;
		bool isWord = true;
		c = *pStr;
		if (c == '|')
		{
			if (pStr[1] == 'H')
			{
				//	if (pStr[2]=='i'&&pStr[3]=='t'&&pStr[4]=='e'&&pStr[5]=='m')
					//	count=unit.GetItemLink(pStr,len);
				if (pStr[2] == 'p' && pStr[3] == 'l' && pStr[4] == 'a' && pStr[5] == 'y' && pStr[6] == 'e' && pStr[7] == 'r')
					count = unit.GetNameLink(pStr, len);
				else
					count = unit.GetItemLink(pStr, len);
				if (count != -1)
					isWord = false;
			}
			else if (pStr[1] == 'c')
			{
				count = Msn_GetColor(pStr, len, color);
				if (count != -1)
				{
					len -= count;
					pStr += count;
					continue;
				}
			}
		}
		else if (c == '#')
		{
			pStr++;
			count = unit.GetNumber(pStr, len);
			if (count != -1)
			{
				len -= 1;
				isWord = false;
			}
			else
				pStr--;
		}
		else if (c == '$')
		{
			pStr++;
			count = unit.GetButton(pStr, len);
			if (count != -1)
			{
				pStr++;
				len -= 2;
				isWord = false;
			}
			else
				pStr--;
		}
		else if (c == '\n')
		{
			count = unit.NextLine(pStr, len);
			if (count != -1)
				isWord = false;
		}
		if (isWord)
		{
			count = unit.GetWord(pStr, len, font, maxW);
		}
		if (count > 0)
		{
			unit.SetWidth(font);
			len -= count;
			pStr += count;
			units.push_back(unit);
		}
		else
			break;
	}
	return (int)units.size();
}
void MsgLine::Parse(CUiColor& color, CUiFontString* font, int limiteWidth, int id)
{
	vector<unitWord> m_units;
	m_units.clear();
	MsgLine_GetUnits(m_str, m_units, font, limiteWidth, color);
	WCHAR tempStr[1024];
	int LineW = 0;
	int WordW = 0;
	int StringW = 0;
	int stringlen = 0;
	CUiSize linesize(0, 0);
	int size = (int)m_units.size();
	CUiLayout* PrenLineFirst = NULL;
	CUiLayout* pren = NULL;
	CUiLayout* Elemt = NULL;
	CUiLayout* LinePren = NULL;
	CUiLayout* pFrame = GetNewLine(NULL);
	for (int i = 0; i < size; i++)
	{
		bool append;
		stringlen = 0;
		StringW = 0;
		do
		{
			append = false;
			unitWord& unit = m_units[i];
			WordW = unit.GetWidth();
			StringW += WordW;
			if ((LineW + StringW) > limiteWidth)
			{
				if (LineW != 0)
				{
					LineW = 0;
					AddLineSize(pFrame, linesize);
					pFrame = GetNewLine(pFrame);
					linesize.m_x = 0;
					linesize.m_y = 0;
					pren = NULL;
					PrenLineFirst = LinePren;
				}
			}
			memcpy(&tempStr[stringlen], unit.m_pStr, unit.m_len * sizeof(WCHAR));
			stringlen += unit.m_len;
			tempStr[stringlen] = 0;
			if (unit.m_type != MT_WORD && unit.m_type != MT_NEXTLINE)
			{
				switch (unit.m_type)
				{
				case MT_FACE:
				{
					int index = (_wtoi(tempStr)) % 64;
					Elemt = AddFace(pFrame, index, linesize);
				}
				break;
				case MT_BUTTON:
					Elemt = AddButton(id, pFrame, tempStr, linesize, color, font);
					//Elemt->SetID(id);
					break;
				case MT_ITEMLINK:
					Elemt = AddItemLink(id, pFrame, tempStr, linesize, color, font, limiteWidth);
					//Elemt->SetID(id);
					break;
				case MT_NAMELINK:
					Elemt = AddNameLink(id, pFrame, tempStr, linesize, color, font);
					//Elemt->SetID(id);
					break;
				}
			}
			else
			{
				if ((i + 1) < size)
				{
					bool IsString = m_units[i + 1].m_type == m_units[i].m_type;
					WordW = m_units[i + 1].GetWidth();
					if (IsString && ((LineW + (StringW + WordW)) < limiteWidth))
					{
						append = true; i++;
					}
				}
				if (!append)
				{
					if (unit.m_type == MT_WORD || unit.m_type == MT_NEXTLINE)
						Elemt = AddString(pFrame, tempStr, linesize, color, font);
					else
					{
						CUiSize src(0, 8);
						CalsRect(linesize, src);
						AddLineSize(pFrame, linesize);
						pFrame = GetNewLine(pFrame);
						LineW = 0;
						linesize.m_x = 0;
						linesize.m_y = 0;
						pren = NULL;
					}
				}
			}
		} while (append);
		if (Elemt)
		{
			AddElement(pFrame, pren, Elemt);
			LineW += StringW;
			if (pren == NULL)
				LinePren = Elemt;
			pren = Elemt;
		}
	}
	AddLineSize(pFrame, linesize);
	m_frame->SetAnchorSize(m_size);
}
void MsgLine::Finish(void)
{
	if (m_str)
		delete[]m_str;
	m_str = NULL;
	if (m_frame)
		delete m_frame;
	m_frame = NULL;
}
ChatMsnInfo::ChatMsnInfo() :m_fontSize(12), m_used(false), m_channel(-1), m_state(MS_NOME), m_alpha(1), m_data(NULL), m_scrollBar(NULL), m_id(0), m_MsnType(MT_WHISPER), m_font(NULL)
{
}
void ChatMsnInfo::SetFontSize(int fsize)
{
	SetNewFontSize(m_font, fsize);
	m_fontSize = fsize;
}
bool ChatMsnInfo::AutoLeaveChannel(void)
{
	bool ret = false;
	if (!strncmp("chat", m_channelPassword.c_str(), 4))
		ret = true;
	return ret;
}
int ChatMsnInfo::GetFontSize(void)
{
	return m_fontSize;
}
MSN_TYPE ChatMsnInfo::GetType(void)
{
	return m_MsnType;
}
void ChatMsnInfo::ClearAllMsg()
{
	SetMaxLines(120);
}
void  ChatMsnInfo::Finish(void)
{
	ClearAllLine();
	m_maxLines = 0;
	m_usedLines = 0;
	m_bottomLine = 0;
	m_scrollBar = NULL;
	m_scroll_count = 0;
	m_topLine = 0;
	m_inputLines = 0;
}
const char* ChatMsnInfo::GetChannelName(void)
{
	return m_channelName.c_str();
}
const char* ChatMsnInfo::GetChannelPass(void)
{
	return m_channelPassword.c_str();
}
const char* ChatMsnInfo::GetChannelOwner(void)
{
	int cid = NetCli_ChannelChild::GetChannelID(m_channelName.c_str());
	const char* Leader = NetCli_ChannelChild::GetChannelLeader(cid);
	return Leader;
}
void  ChatMsnInfo::SetChannel(int ChannelNo)
{
	m_channel = ChannelNo;
}
void ChatMsnInfo::SetPopup(int x, int y, const char* who, const char* msg/*,ChatMsnInfo* pren*/)
{
	m_alpha = 1;
	//m_next=pren;
	m_used = true;
	m_target = who;
	if (msg)
	{
		/*
		char temp[];
		int len=strlen(msg);
		if (len>8)
		{
			memset(temp,'.',32);
			strncpy(temp,msg,8);
			temp[12]=0;
		}
		else
			strcpy(temp,msg);
		*/
		m_msg = msg;
	}
	m_state = MS_POPUP_IN;
	m_x = (float)x;
	m_y = (float)y;
	m_tempX = m_x;
	m_tempY = m_y + 120;
}
void ChatMsnInfo::Update(float delate)
{
	if (m_state == MS_POPUP_IN)
	{
		m_tempY -= delate * 50;
		if (m_tempY <= m_y)
		{
			m_tempY = m_y;
			m_state = MS_POPUPED;
			m_time = 8;
		}
	}
	else if (m_state == MS_POPUPED)
	{
		m_time -= delate * 5;
		if (m_time <= 0)
		{
			m_state = MS_POPUP_OUT;
		}
	}
	else if (m_state == MS_POPUP_OUT)
	{
		m_tempY += delate * 12;
		m_alpha = (120.0f - (m_tempY - m_y)) / 120.0f;
		if (m_tempY >= (m_y + 120))
		{
			m_tempY = m_y + 120;
			m_state = MS_NOME;
			m_used = false;
			m_alpha = 1.0f;
		}
	}
	/*
	else if  (m_state==MS_SMALL_FLASH)
	{
		m_time-=delate*5;
		m_alpha=m_time+0.5;
		if (m_time<=0)
		{
			m_time=0.5;
		}
	}
	*/
}
void ChatMsnInfo::SetTellTarget(const char* playername)
{
	m_used = true;
	m_MsnType = MT_WHISPER;
	m_channel = -1;
	m_target = playername;
}
void ChatMsnInfo::SetOpen(int x, int y)
{
	m_alpha = 1.0f;
	m_state = MS_OPEN;
	m_tempX = m_x = (float)x;
	m_tempY = m_y = (float)y;
}
void ChatMsnInfo::ClearAllLine()
{
	if (m_data)
		delete[]m_data;
	m_data = NULL;
}
const char* ChatMsnInfo::MemberGetName(int index)
{
	const char* name = NULL;
	if (index >= 0 && index < m_members.size())
		name = m_members[index].c_str();
	return name;
}
void ChatMsnInfo::SetMaxLines(int maxLines)
{
	ClearAllLine();
	if (maxLines > 0)
		m_data = new MsgLine[maxLines];
	m_maxLines = maxLines;
	m_usedLines = 0;
	m_bottomLine = 0;
	m_scroll_count = 0;
	m_topLine = 0;
	m_inputLines = 0;
}
int ChatMsnInfo::GetMemberCount(void)
{
	return (int)m_members.size();
}
void ChatMsnInfo::DelMember(const char* who)
{
	vector<string>::iterator it = m_members.begin();
	vector<string>::iterator endit = m_members.end();
	for (; it != endit; ++it)
	{
		if (*it == who)
		{
			m_members.erase(it);
			return;
		}
	}
}
void ChatMsnInfo::AddMember(const char* who)
{
	int count = (int)m_members.size();
	for (int i = 0; i < count; i++)
	{
		if (m_members[i] == who)
			return;
	}
	m_members.push_back(who);
}
void ChatMsnInfo::Initial(int maxLines, int id)
{
	m_id = id;
	m_members.clear();
	m_fontSize = 12;
	m_font = NULL;
	SetMaxLines(maxLines);
	SetSize(320, 275 - 48);
}
void ChatMsnInfo::SetSize(int w, int h)
{
	m_ChildSizeW = w;
	m_ChildSizeH = h;
}
void ChatMsnInfo::Update()
{
	if (m_state != MS_OPEN)
		return;
	//if (m_bottomLine==m_topLine)
	//	return;
	int lastLine = 0;
	if (m_scrollBar)
		lastLine = (int)m_scrollBar->GetValue();
	int x = m_usedLines - lastLine;
	m_bottomLine = m_inputLines - x;
	if (m_bottomLine < 0)
		m_bottomLine += m_maxLines;
	int d = 0;
	for (int i = m_usedLines - 1; i >= 0; --i)
	{
		//bool show=false;
		MsgLine* item = &m_data[i];
		CUiFrame* frame = item->m_frame;
		frame->ClearAllAnchors();
		item->SetShow(false);
	}
	CUiAnchor	anchor;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = 0;
	if (g_StringAlige == 0)
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
	}
	else
	{
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
	}
	CUiFrame* pren = NULL;
	int height = 0;
	int count = 0;
	for (int i = m_bottomLine - 1; height < m_ChildSizeH; --i)
	{
		if (i < 0)
			i = m_maxLines - 1;
		//if (m_bottomLine==m_topLine)
		//	break;
		MsgLine* item = &m_data[i];
		CUiFrame* frame = item->m_frame;
		if (!frame)
			break;
		if (item->m_size.m_y <= 0)
			break;
		height += (int)item->m_size.m_y;
		if (height > m_ChildSizeH)
			break;
		if (!pren)
		{
			anchor.m_relativeTo = m_frame;
			if (g_StringAlige == 0)
			{
				anchor.m_offset.m_x = 0;
				anchor.m_relativePoint = CUiAnchor::eAnchor_BottomLeft;
			}
			else
			{
				anchor.m_offset.m_x = -40;
				anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
			}
		}
		else
		{
			anchor.m_relativeTo = pren;
			anchor.m_offset.m_x = 0;
			if (g_StringAlige == 0)
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
			else
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
		}
		frame->SetAnchor(anchor);
		pren = frame;
		m_topLine = i;
		item->SetShow(true);
		count++;
	}
	if (m_scrollBar)
		m_scrollBar->SetMinValue((float)count);
	/*
	m_scrollBar->SetMaxValue(m_usedLines);
	d=m_usedLines-m_bottomLine;
	int n=m_topLine+d;
	int t=m_usedLines-n;
	m_scrollBar->SetMinValue(t);
	if (t<m_usedLines)
		m_scrollBar->SetVisible(true);
	else
		m_scrollBar->SetVisible(false);
*/
}
void MsnManager::ItemLink_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam)
{
	lua_State* L = m_iface->GetLuaState();
	CUiButton* but = static_cast<CUiButton*>(uiFrame);
	CUiLuaLayout::PushLayoutTable(L, but);
	const char* name = WCharToUFT8(but->GetNormalText()->GetText());
	if (!lua_isnil(L, -1))
	{
		lua_pushstring(L, "data");
		lua_gettable(L, -2);
		name = luaL_checkstring(L, -1);
	}
	int id = uiFrame->GetID();
	if (lParam & 1) //右鍵
	{
		lua_pushstring(L, name);
		lua_setglobal(L, "arg1");
		m_iface->SendWorldEvent("CHAT_MSN_ADDITEMLINK");
	}
	else
	{
		CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>(m_iface->GetUi()->FindObject("GameTooltipHyperLink"));
		g_pGameTooltip->SetHyperLink(tooltip, name);
		lua_pushstring(L, name);
		lua_setglobal(L, "arg1");
		m_iface->SendWorldEvent("CHAT_MSN_ITEMPREVIEW");
	}
}
void MsnManager::ItemLink_OnMouseEnter(CUiFrame* uiFrame, int lParam, int rParam)
{
	/*
	lua_State* L = m_iface->GetLuaState();
	CUiButton* but=static_cast<CUiButton*>(uiFrame);
	const char *name=WCharToUFT8(but->GetNormalText()->GetText());
	CUiLuaLayout::PushLayoutTable(L, but);
	if (!lua_isnil(L, -1))
	{
		lua_pushstring(L,"data");
		lua_gettable(L,-2);
		name=luaL_checkstring(L,-1);
	}
	*/
	//tooltip->ClearAllA
}
void MsnManager::ItemLink_OnMouseLeave(CUiFrame* uiFrame, int lParam, int rParam)
{/*
	CUiTooltip *tooltip= dynamic_cast<CUiTooltip*>( m_iface->GetUi()->FindObject("GameTooltipItemLink") );
	g_pGameTooltip->SetHyperLink(tooltip,NULL);
	*/
}
int MsnManager::MemberGetCount(int id)
{
	int count = 0;
	ChatMsnInfo* msn = GetMsnInfo(id);
	if (msn)
		count = msn->GetMemberCount();
	return count;
}
void MsnManager::NameLink_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam)
{
	lua_State* L = m_iface->GetLuaState();
	CUiButton* but = static_cast<CUiButton*>(uiFrame);
	CUiLuaLayout::PushLayoutTable(L, but);
	const char* name = WCharToUFT8(but->GetNormalText()->GetText());
	if (!lua_isnil(L, -1))
	{
		lua_pushstring(L, "data");
		lua_gettable(L, -2);
		name = luaL_checkstring(L, -1);
	}
	int id = uiFrame->GetID();
	if (lParam & 1) //右鍵
	{
		CUiLuaLayout::PushLayoutTable(L, uiFrame);
		lua_setglobal(L, "arg1");
		char temps[1024];
		strcpy(temps, name);
		char* pdata = NameLink_GetName(temps);
		lua_pushstring(L, pdata);
		lua_setglobal(L, "arg2");
		m_iface->SendWorldEvent("MSN_MENU_OPEN");
	}
	else
	{
		//	lua_pushnumber(L,id);
		//	lua_setglobal(L, "arg1");
		lua_pushstring(L, name);
		lua_setglobal(L, "arg1");
		m_iface->SendWorldEvent("CHAT_MSN_ADDITEMLINK");
	}
}
void MsnManager::Button_OnMouseUp(CUiFrame* uiFrame, int lParam, int rParam)
{
	lua_State* L = m_iface->GetLuaState();
	CUiButton* but = static_cast<CUiButton*>(uiFrame);
	CUiLuaLayout::PushLayoutTable(L, but);
	const char* name = WCharToUFT8(but->GetNormalText()->GetText());
	if (!lua_isnil(L, -1))
	{
		lua_pushstring(L, "data");
		lua_gettable(L, -2);
		name = luaL_checkstring(L, -1);
	}
	int id = uiFrame->GetID();
	if (lParam & 1) //右鍵
	{
		CUiLuaLayout::PushLayoutTable(L, uiFrame);
		lua_setglobal(L, "arg1");
		lua_pushstring(L, name);
		lua_setglobal(L, "arg2");
		m_iface->SendWorldEvent("MSN_MENU_OPEN");
	}
	else
	{
		lua_pushnumber(L, id);
		lua_setglobal(L, "arg1");
		lua_pushstring(L, name);
		lua_setglobal(L, "arg2");
		m_iface->SendWorldEvent("CHAT_MSN_ADDBUTTON");
	}
}
void ChatMsnInfo::SetBottomLine(int line)
{
	if (line >= 0 && line < m_maxLines && m_bottomLine != line)
	{
		m_bottomLine = line;
		int height = 0;
		int i;
		int d = 0;
		int count = 0;
		m_topLine = m_bottomLine - 1;
		for (i = m_bottomLine - 1;; --i)
		{
			if (i < 0)
				i = m_maxLines - 1;
			MsgLine* item = &m_data[i];
			MsgLine* topItem = NULL;
			CUiFrame* frame = item->m_frame;
			if (!frame)
				break;
			height += (int)item->m_size.m_y;
			if (height > m_ChildSizeH)
			{
				d = 1;
				break;
			}
			m_topLine = i;
			count++;
		}
		int t = m_inputLines - m_bottomLine;
		if (t < 0)
			t += m_maxLines;
		if (m_scrollBar)
		{
			m_scrollBar->SetMaxValue((float)m_usedLines);
			m_scrollBar->SetMinValue((float)count);
			m_scrollBar->SetValue((float)(m_usedLines - t));
			m_scrollBar->SetVisible(t || d);
			Update();
		}
	}
}
void ChatMsnInfo::SetNameAndPassword(string& name, string& password)
{
	m_MsnType = MT_CHANNEL;
	m_channelName = name;
	m_channelPassword = password;
	m_target = "";
}
void ChatMsnInfo::GetNameAndPassword(string& name, string& password)
{
	name = m_channelName;
	password = m_channelPassword;
}
void ChatMsnInfo::PushLine(LPCWSTR msg, int icolor)
{
	//if ( m_nowLines < m_maxLines )
	{
		CUiColor color = icolor;
		MsgLine* pitem = NULL;
		int pindex = m_inputLines - 1;
		if (pindex < 0)
			pindex = m_maxLines - 1;
		pitem = &m_data[pindex];
		MsgLine* item = &m_data[m_inputLines];
		item->Initial(m_frame, pitem, m_inputLines);
		int width = (int)m_frame->GetWidth() - 32;
		item->AddMessage(msg, color, m_font, width, m_id);
		m_usedLines++;
		if (m_usedLines > m_maxLines)
			m_usedLines = m_maxLines;
		m_inputLines++;
		if (m_inputLines >= m_maxLines)
			m_inputLines = 0;
		//	if ( m_nowLines == m_lastLine+1 )
		SetBottomLine(m_inputLines);
		//Update();
	}
}
/*
void ChatMsnInfo::PopLine(void)
{
	if (m_nowLines > 0)
	{
		m_nowLines--;
		UI_SAFE_DELETE(m_data[0].m_frame);
		UI_SAFE_DELETE_ARRAY(m_data[0].m_str);
		int i;
		for (i=0;i<m_nowLines-1;i++)
		{
			m_data[i].m_frame=m_data[i+1].m_frame;
			m_data[i].m_str=m_data[i+1].m_str;
		}
		m_data[i].m_frame=NULL;
		m_data[i].m_str=NULL;
	}
}*/
bool ChatMsnInfo::AddMessage(const WCHAR* msg, int color)
{
	if (msg)
	{
		// pop front
		//if ( m_nowLines >= m_maxLines )
			//PopLine();
		// push back
		PushLine(msg, color);
		return true;
	}
	return false;
}
bool ChatMsnInfo::AddMessage(LPCSTR msg, int color)
{
	WCHAR wbuff[1024];
	int nLen = MultiByteToWideChar(CP_UTF8, 0, msg, -1, wbuff, 1024);
	wbuff[nLen] = 0;
	if (nLen)
	{
		return AddMessage(wbuff, color);
	}
	return false;
}
void ChatMsnInfo::Close()
{
	SetMaxLines(120);
	m_target = "";
	m_channel = -1;
	m_state = MS_NOME;
	m_used = false;
	m_members.clear();
	m_channelName = "";
	m_channelPassword = "";
}
void _PG_CtoC_InviteChannel(void* Data, int Size)
{
	char temp[512];
	PG_CtoC_InviteChannel* cmd = (PG_CtoC_InviteChannel*)Data;
	g_pChatFrame->JoinChannelByName(cmd->ChannelName, cmd->ChannelPass, -1, false);
	const  char* pMsg = g_ObjectData->GetString("SYS_JOIN_CHANNEL");
	sprintf(temp, "%s-%s", pMsg, cmd->ChannelName.Begin());
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp, "");
	//g_pChatFrame->SendMsgEvent();
	//NetCli_Channel::S_JoinChannel(cmd->ChannelName,cmd->ChannelPass,true);
}
void MsnManager::RegC2C(void)
{
	static bool _init = false;
	if (!_init)
	{
		ClitoCli_NetReg(PG_CtoC_InviteChannel);
		_init = true;
	}
}
MsnManager::MsnManager(CInterface* object) :m_pTop(NULL), m_iface(object)
{
	_gMsnManager = this;
	m_pListManager = new ChatListManager;
	RegC2C();
	int i;
	for (i = 0; i < MT_COUNT; i++)
		m_autoOpen[i] = true;
	if (!strcmp(g_pGameMain->GetLanguage(), "AE"))
	{
		g_StringAlige = 1;
	}
}
MsnManager::~MsnManager()
{
	delete  m_pListManager;
	m_pListManager = NULL;
}
ChatMsnInfo* MsnManager::FindChannel(const char* name, int& index)
{
	ChatMsnInfo* ret = NULL;
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		if (m_msnfo[i].m_used)
		{
			if (m_msnfo[i].m_MsnType == MT_CHANNEL && m_msnfo[i].m_channelName == name)
			{
				index = i;
				ret = &m_msnfo[i];
				break;
			}
		}
	}
	return ret;
}
void MsnManager::UpdateChatList(int index)
{
	lua_State* L = m_iface->GetLuaState();
	lua_pushnumber(L, index + 1);
	lua_setglobal(L, "arg1");
	m_iface->SendWorldEvent("CRG_UPDATE_MEMBERLIST");
}
void MsnManager::AddChannelMember(const char* name, int channelNo, const char* Name)
{
	int index = -1;
	ChatMsnInfo* ret = FindChannel(name, index);
	if (ret)
	{
		ret->AddMember(Name);
		UpdateChatList(index);
		if (m_pListManager->m_pmsn == ret)
		{
			ReflashList(index);
			//	m_iface->SendWorldEvent("CRG_UPDATE_LIST");
		}
	}
}
void MsnManager::DelChannelMember(const char* name, int channelNo, const char* Name)
{
	int index = -1;
	ChatMsnInfo* ret = FindChannel(name, index);
	if (ret)
	{
		ret->DelMember(Name);
		UpdateChatList(index);
		if (m_pListManager->m_pmsn == ret)
		{
			ReflashList(index);
			//m_iface->SendWorldEvent("CRG_UPDATE_LIST");
		}
	}
	if (m_selfName == Name)
	{
		int	 id = NetCli_ChannelChild::GetChannelID(name);
		g_pNetCli_ChannelChild->removeChannel(id);
		//CloseMsn(name);
	}
}
void MsnManager::CloseMsn(const char* name)
{
	int index = -1;
	ChatMsnInfo* ret = FindChannel(name, index);
	if (ret)
	{
		lua_State* L = m_iface->GetLuaState();
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");
		//ret->Close();
		m_iface->SendWorldEvent("CHAT_MSN_CLOSE");
	}
}
void MsnManager::ChangeChannelOwner(const char* name, int channelNo, const char* LeaderName)
{
	int index = -1;
	ChatMsnInfo* ret = FindChannel(name, index);
	if (ret)
	{
		lua_State* L = m_iface->GetLuaState();
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");
		lua_pushstring(L, LeaderName);
		lua_setglobal(L, "arg2");
		m_iface->SendWorldEvent("CRG_UPDATE_OWNER");
	}
}
ChatMsnInfo* MsnManager::FindMsn(int msnType, int ChannelId, int& index)
{
	ChatMsnInfo* ret = NULL;
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		if (m_msnfo[i].m_used)
		{
			if (m_msnfo[i].m_channel == ChannelId && m_msnfo[i].m_MsnType == msnType)
			{
				index = i;
				ret = &m_msnfo[i];
				break;
			}
		}
	}
	return ret;
}
const char* MsnManager::MemberGetName(int id, int index)
{
	const char* name = 0;
	ChatMsnInfo* msn = GetMsnInfo(id);
	if (msn)
		name = msn->MemberGetName(index);
	return name;
}
int  MsnManager::ListGetCount(void)
{
	return m_pListManager->GetCount();
}
void MsnManager::ReflashList(int msnid)
{
	if (msnid >= 0 && msnid < NUM_MAX_CHATMSN_INFO)
	{
		ChatMsnInfo* msn = GetMsnInfo(msnid);
		if (msn)
		{
			m_pListManager->Initial(msn);
			m_pListManager->CalAllList();
			m_iface->SendWorldEvent("CRG_UPDATE_LIST");
		}
	}
}
void MsnManager::SetAutoCheck(int type, bool cb)
{
	if (type >= 0 && type < MT_COUNT)
		m_autoOpen[type] = cb;
}
ChatMsnInfo* MsnManager::FindMsn(const char* who, int& index)
{
	ChatMsnInfo* ret = NULL;
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		if (m_msnfo[i].m_used)
		{
			if (m_msnfo[i].m_target == who)
			{
				index = i;
				ret = &m_msnfo[i];
				break;
			}
		}
	}
	return ret;
}
void MsnManager::HaveNewChannel(int msnType, int channelNo, const char* who, const char* msg, int type, const char* ChannleName, const char* passWord)
{
	if (!m_autoOpen[msnType])
		return;
	int index;
	//	g_pError->OutputToFile( "Channel=%d ,msnType=%dmsg=%s",channelNo,msnType,msg );
	ChatMsnInfo* info = FindMsn(msnType, channelNo, index);
	lua_State* L = m_iface->GetLuaState();
	if (info == NULL)
	{
		GroupObjectTypeEnum otype;
		info = GetFreeInfo(index);
		const char* ChannelName = NULL;
		switch (msnType)
		{
		case MT_PARTY://隊伍
			ChannelName = g_ObjectData->GetString("MSNTYPE_PARTY");
			otype = EM_GroupObjectType_Party;
			break;
		case MT_GUILD://工會
			ChannelName = g_ObjectData->GetString("MSNTYPE_GUILD");
			otype = EM_GroupObjectType_Guild;
			break;
		case MT_ZONE://每個區域都有的
			ChannelName = g_ObjectData->GetString("MSNTYPE_ZONE");
			otype = EM_GroupObjectType_SysZoneChannel;
			break;
		case MT_CHANNEL:	//玩家的
			ChannelName = g_ObjectData->GetString("MSNTYPE_CHANNEL");
			otype = EM_GroupObjectType_PlayerChannel;
			if (!ChannleName)
			{
				vector<UserDefineChannel>& la = g_pChatFrame->GetChannelList();
				for (vector<UserDefineChannel>::iterator iter = la.begin(); iter != la.end(); iter++)
				{
					if (iter->id == channelNo)
					{
						ChannleName = iter->name;
						passWord = iter->password;
						break;
					}
				}
				//ChannleName=NetCli_ChannelChild::GetChannelNameByID(channelNo);
			}
			//g_pError->OutputToFile( "Channel=%s",ChannleName);
			break;
		case MT_YELL://系統
			ChannelName = g_ObjectData->GetString("MSNTYPE_YELL");
			otype = EM_GroupObjectType_SysChannel;
			break;
		}
		if (ChannleName)
		{
			int channelID = NetCli_ChannelChild::GetChannelID(ChannleName);
			if (info)
			{
				info->SetNameAndPassword(string(ChannleName), string(passWord));
			}
			NetCli_Channel::S_ChannelPlayerList(otype, channelID);
		}
		if (info)
		{
			int x = -120;
			int y = -30;
			char temp[512];
			bool bEventMsg = false;
			if (m_pTop)
			{
				x = (int)m_pTop->m_x;
				y = (int)m_pTop->m_y - 120;
			}
			info->SetChannel(channelNo);
			info->SetType((MSN_TYPE)msnType);
			switch (type)
			{
			case 0:
				bEventMsg = true;
				sprintf(temp, msg);
				break;
			case 1:
				info->SetPopup(x, y, ChannelName, NULL);
				SetMsnInfoOpen(index);
				break;
			case 2:
				info->SetPopup(x, y, ChannelName, NULL);
				lua_pushnumber(L, index + 1);
				lua_setglobal(L, "arg1");
				m_iface->SendWorldEvent("CHAT_MSN_OPEN");
				info->m_alpha = 1;
				info->SetState(MS_NOME);
				lua_pushnumber(L, index + 1);
				lua_setglobal(L, "arg1");
				m_iface->SendWorldEvent("CHAT_MSN_SMALL");
				break;
			}
			if (bEventMsg)
			{
				info->SetPopup(x, y, ChannelName, temp);
				m_pTop = info;
				lua_pushnumber(L, index + 1);
				lua_setglobal(L, "arg1");
				m_iface->SendWorldEvent("CHAT_MSN_POPUP");
			}
		}
	}
	if (info && msg)
	{
		const char* sender;
		char temp[512];
		char Name[512];
		if (type)
			sender = m_selfName.c_str();
		else
			sender = who;
		sprintf(temp, "%s", msg);
		sprintf(Name, "%s", sender);
		//info->AddMessage(x,y,who,msg);
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");
		lua_pushstring(L, Name);
		lua_setglobal(L, "who");
		lua_pushstring(L, temp);
		lua_setglobal(L, "msg");
		CUi* ui = m_iface->GetUi();
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_Message", index + 1);
		info->m_frame = static_cast<CUiFrame*>(ui->FindObject(temp));
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_Message_Font", index + 1);
		CUiFontString* font = static_cast<CUiFontString*>(ui->FindObject(temp));
		SetNewFontSize(font, info->m_fontSize);
		if (info->m_state != MS_OPEN)
		{
			info->m_state = MS_POPUP_IN;
			info->m_alpha = 1.0f;
		}
		info->m_font = font;
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_ScrollBar", index + 1);
		info->m_scrollBar = static_cast<CUiSlider*>(ui->FindObject(temp));
		m_iface->SendWorldEvent("CHAT_MSN_ADD");
	}
}
void MsnManager::SetSelfName(const char* name)
{
	m_selfName = name;
}
void MsnManager::HaveNewWhisper(const char* who, const char* msg, int type)
{
	if (!m_autoOpen[0]) //祕語開關
		return;
	int index;
	ChatMsnInfo* info = FindMsn(who, index);
	lua_State* L = m_iface->GetLuaState();
	if (info == NULL)
	{
		info = GetFreeInfo(index);
		if (info)
		{
			int x = -120;
			int y = -30;
			char temp[512];
			bool bEventMsg = false;
			if (m_pTop)
			{
				x = (int)m_pTop->m_x;
				y = (int)m_pTop->m_y - 120;
			}
			info->SetType(MT_WHISPER);
			info->AddMember(who);
			switch (type)
			{
			case 2://好友上線
			{
				bEventMsg = true;
				time_t timet;
				time(&timet);
				char temp1[512];
				struct tm* gmt;
				gmt = localtime(&timet);
				sprintf(temp1, "%02d:%02d", gmt->tm_hour, gmt->tm_min);
				const char* msg = g_ObjectData->GetString("GUILD_MSG_MEMBERLOGIN_GUILD");
				sprintf(temp, msg, who, temp1);
			}
			break;
			case 0: //密語
			{
				bEventMsg = true;
				sprintf(temp, "%s", msg);
			}
			break;
			case 1:// 我的
				info->SetPopup(x, y, who, NULL);
				SetMsnInfoOpen(index);
				break;
			}
			if (bEventMsg)
			{
				info->SetPopup(x, y, who, temp);
				m_pTop = info;
				lua_pushnumber(L, index + 1);
				lua_setglobal(L, "arg1");
				m_iface->SendWorldEvent("CHAT_MSN_POPUP");
			}
		}
	}
	if (info && msg)
	{
		const char* sender;
		char temp[512];
		char Name[512];
		if (type)
			sender = m_selfName.c_str();
		else
			sender = who;
		sprintf(temp, "%s", msg);
		sprintf(Name, "%s", sender);
		//info->AddMessage(x,y,who,msg);
		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");
		lua_pushstring(L, Name);
		lua_setglobal(L, "who");
		lua_pushstring(L, temp);
		lua_setglobal(L, "msg");
		CUi* ui = m_iface->GetUi();
		if (info->m_state != MS_OPEN)
		{
			info->m_state = MS_POPUP_IN;
			info->m_alpha = 1.0f;
		}
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_Message", index + 1);
		info->m_frame = static_cast<CUiFrame*>(ui->FindObject(temp));
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_Message_Font", index + 1);
		CUiFontString* font = static_cast<CUiFontString*>(ui->FindObject(temp));
		info->m_font = font;
		sprintf(temp, "CRG_Chat%d_TalkFrame_MF_ScrollBar", index + 1);
		info->m_scrollBar = static_cast<CUiSlider*>(ui->FindObject(temp));
		m_iface->SendWorldEvent("CHAT_MSN_ADD");
	}
}
void MsnManager::Initial(CUi* ui)
{
	m_selfName = "";
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		m_msnfo[i].m_used = false;
		m_msnfo[i].Initial(120, i + 1);
	}
}
void MsnManager::Update(float elapsedTime)
{
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		if (m_msnfo[i].m_used)
		{
			m_msnfo[i].Update(elapsedTime);
		}
	}
	if (m_pTop)
	{
		if (m_pTop->m_used == false)
			m_pTop = NULL;
	}
}
void  ChatMsnInfo::SetState(MSN_STATE state)
{
	m_state = state;
}
void ChatMsnInfo::SetType(MSN_TYPE type)
{
	m_used = true;
	m_MsnType = type;
}
void MsnManager::InviteListOK(void)
{
	m_pListManager->InviteListOK();
}
ChatMsnInfo* MsnManager::GetMsnInfo(int id)
{
	ChatMsnInfo* ret = NULL;
	if (id >= 0 && id < NUM_MAX_CHATMSN_INFO)
		ret = &m_msnfo[id];
	return ret;
}
void MsnManager::SetMsnClose(int id)
{
	ChatMsnInfo* ret = NULL;
	if (id >= 0 && id < NUM_MAX_CHATMSN_INFO)
	{
		ret = &m_msnfo[id];
		if (ret->m_MsnType == MT_CHANNEL)
		{
			const char* Name = ret->GetChannelOwner();
			const char* playerName = NULL;
			CRoleSprite* player = g_pGameMain->GetPlayer();
			//if ( player )
			//	playerName= player->GetName() ;
			if (ret->AutoLeaveChannel())
				g_pChatFrame->LeaveChannelByName(ret->m_channelName.c_str());
		}
		ret->Close();
		if (m_pTop == ret)
			m_pTop = NULL;
	}
}
void MsnManager::SetMsnInfoOpen(int id)
{
	ChatMsnInfo* ret = NULL;
	if (id >= 0 && id < NUM_MAX_CHATMSN_INFO)
	{
		int x = -100 + (id * 32);
		int y = -100 + (id * 32);
		lua_State* L = m_iface->GetLuaState();
		ret = &m_msnfo[id];
		//ret->SetState(MS_OPEN);
		ret->SetOpen(x, y);
		lua_pushnumber(L, id + 1);
		lua_setglobal(L, "arg1");
		m_iface->SendWorldEvent("CHAT_MSN_OPEN");
		if (m_pTop == ret)
			m_pTop = NULL;
	}
}
void MsnManager::Finish(void)
{
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		m_msnfo[i].m_used = false;
		m_msnfo[i].Finish();
	}
}
ChatMsnInfo* MsnManager::GetFreeInfo(int& index)
{
	ChatMsnInfo* ret = NULL;
	int i;
	for (i = 0; i < NUM_MAX_CHATMSN_INFO; i++)
	{
		if (m_msnfo[i].m_used == false)
		{
			index = i;
			ret = &m_msnfo[i];
			break;
		}
	}
	return ret;
}
void MsnManager::JoinChannelOK(const char* name, int channelNo, const char* pass)
{
	int index;
	ChatMsnInfo* info = FindChannel(name, index);
	if (info)
	{
		info->SetChannel(channelNo);
		SetMsnInfoOpen(index);
		//m_pListManager->InviteListOK();
	}
	else
	{	//g_pDebugMessage->DebugOutput(0, "Join Channel %s(%d)pass", name ,channelNo,pass);
		if (pass && !strncmp("chat", pass, 4))
			HaveNewChannel(MT_CHANNEL, channelNo, name, NULL, 2, name, pass);
	}
}
void MsnManager::CreateChannelOK(const char* name, int channelNo)
{
	int index;
	ChatMsnInfo* info = FindChannel(name, index);
	if (info)
	{
		info->SetChannel(channelNo);
		SetMsnInfoOpen(index);
		m_pListManager->InviteListOK();
	}
}
const char* MsnManager::GetChannelName(int index)
{
	const char* Name = NULL;
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
		Name = msn->GetChannelName();
	return Name;
}
const char* MsnManager::GetChannelPass(int index)
{
	const char* Pass = "";
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
		Pass = msn->GetChannelPass();
	return Pass;
}
const char* MsnManager::GetChannelOwner(int index)
{
	const char* Name = "";
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
		Name = msn->GetChannelOwner();
	return Name;
}
void MsnManager::ListSetToggle(int index)
{
	m_pListManager->SetToggle(index);
}
void MsnManager::ListGetInfo(int index, int& _type, int& state, std::string& name, bool& add, int& mode)
{
	ChatListItem* info = m_pListManager->GetInfo(index);
	if (info)
	{
		_type = info->type;
		state = info->state;
		name = info->m_name;
		add = info->add;
		mode = info->mode;
	}
}
void MsnManager::InviteChannel(int index, const char* Name)
{
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
	{
		m_pListManager->Initial(msn);
		int t = m_pListManager->AddMember(0, 1, Name);
		if (t >= 0)
			m_pListManager->SetToggle(t);
		m_pListManager->InviteListOK();
	}
}
int MsnManager::GetFontSize(int index)
{
	int fsize = 0;
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
	{
		fsize = msn->GetFontSize();
	}
	return fsize;
}
void MsnManager::SetFontSize(int index, int fsize)
{
	ChatMsnInfo* msn = GetMsnInfo(index);
	if (msn)
	{
		msn->SetFontSize(fsize);
	}
}
void MsnManager::CopyToClipboard(const char* utf8name)
{
	WCHAR wbuff[1024];
	int nLen = MultiByteToWideChar(CP_UTF8, 0, utf8name, -1, wbuff, 1024);
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		if (nLen > 0)
		{
			HGLOBAL hBlock = GlobalAlloc(GMEM_MOVEABLE, sizeof(WCHAR) * (nLen + 1));
			if (hBlock)
			{
				WCHAR* tmpTextStr = (WCHAR*)GlobalLock(hBlock);
				memcpy(tmpTextStr, wbuff, sizeof(WCHAR) * (nLen));
				tmpTextStr[nLen] = 0;  // Terminate it
				GlobalUnlock(hBlock);
			}
			SetClipboardData(CF_UNICODETEXT, hBlock);
			// We must not free the object until CloseClipboard is called.
			if (hBlock)
				GlobalFree(hBlock);
		}
		CloseClipboard();
	}
}
int LuaFunc_CRG_InviteListOK(lua_State* L)
{
	if (_gMsnManager)
		_gMsnManager->InviteListOK();
	return 0;
}
int LuaFunc_CRG_ListSetToggle(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if (_gMsnManager)
		_gMsnManager->ListSetToggle(index);
	return 0;
}
int LuaFunc_CRG_ListGetInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int type = -1;
	int state;
	bool add;
	int mode;
	string name;
	if (_gMsnManager)
		_gMsnManager->ListGetInfo(index, type, state, name, add, mode);
	lua_pushnumber(L, type);
	lua_pushnumber(L, state);
	lua_pushstring(L, name.c_str());
	lua_pushboolean(L, add);
	lua_pushnumber(L, mode);
	return 5;
}
int LuaFunc_CRG_ListGetCount(lua_State* L)
{
	int count = 0;
	if (_gMsnManager)
		count = _gMsnManager->ListGetCount();
	lua_pushnumber(L, count);
	return 1;
}
int LuaFunc_CRG_ListReflash(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if (_gMsnManager)
		_gMsnManager->ReflashList(index);
	return 0;
}
int LuaFunc_CRG_MemberGetCount(lua_State* L)
{
	int count = 0;
	int id = luaL_checkint(L, 1) - 1;
	if (_gMsnManager)
		count = _gMsnManager->MemberGetCount(id);
	lua_pushnumber(L, count);
	return 1;
}
int LuaFunc_CRG_MemberGetName(lua_State* L)
{
	int id = luaL_checkint(L, 1) - 1;
	int index = luaL_checkint(L, 2) - 1;
	const char* name = NULL;
	if (_gMsnManager)
		name = _gMsnManager->MemberGetName(id, index);
	lua_pushstring(L, name);
	return 1;
}
//-----------------------------------
int LuaFunc_CRG_InviteChannel(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* Name = luaL_checkstring(L, 2);
	if (_gMsnManager)
	{
		_gMsnManager->InviteChannel(index, Name);
	}
	return 0;
}
//pass
int LuaFunc_CRG_GetChannelPass(lua_State* L)
{
	int ChannelID = luaL_checkint(L, 1) - 1;
	const char* password = "";
	if (_gMsnManager)
	{
		password = _gMsnManager->GetChannelPass(ChannelID);
	}
	lua_pushstring(L, password);		// password
	return 1;
}
//-----------------------------------
int LuaFunc_CRG_GetChannelOwner(lua_State* L)
{
	int ChannelID = luaL_checkint(L, 1) - 1;
	const char* OwnerName = "";
	if (_gMsnManager)
	{
		OwnerName = _gMsnManager->GetChannelOwner(ChannelID);
	}
	lua_pushstring(L, OwnerName);		// Name
	return 1;
}
//-----------------------------------
int LuaFunc_CRG_SetMsnFontSize(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int fsize = luaL_checkint(L, 2);
	if (_gMsnManager)
		_gMsnManager->SetFontSize(index, fsize);
	return 0;
}
//-----------------------------------
int LuaFunc_CRG_GetMsnFontSize(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int fsize = 10;
	if (_gMsnManager)
	{
		fsize = _gMsnManager->GetFontSize(index);
	}
	lua_pushnumber(L, fsize);
	return 1;
}
//==
int LuaFunc_CRG_GetChannelName(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* name = NULL;
	if (_gMsnManager)
	{
		name = _gMsnManager->GetChannelName(index);
	}
	lua_pushstring(L, name);		// Name
	return 1;
}
int LuaFunc_CRG_SetAutoCheck(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	bool cb = (lua_toboolean(L, 2) != 0);
	if (_gMsnManager)
		_gMsnManager->SetAutoCheck(index, cb);
	//	char temp[120];
	//	sprintf(temp,"SetCheck %d= %s",index,cb ? "true":"flase");
	//	g_pChatFrame->AddMessage(5, temp);	
	return 0;
}